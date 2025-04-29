// SWITCH APPLICATION - ZIGBEE CLIENT SIDE //

#include "app/framework/include/af.h"
#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"
#include "app/framework/util/af-main.h"
#include "app/util/common/common.h"
#include "app/framework/plugin/network-steering/network-steering.h"
#include "app/framework/plugin/find-and-bind-initiator/find-and-bind-initiator.h"
#include "sl_sensor_rht.h"

// Button Definitions
#define BUTTON0 0                             // Used to trigger network joining
#define BUTTON1 1                             // Used to enable sensor data reporting

// Timings
#define FIND_AND_BIND_DELAY_MS 3000           // Delay before starting Find & Bind process
#define REPORT_PERIOD_MS 4000                 // Sensor reporting interval

// Flags for button events and state tracking
volatile bool button0Pressed;                 // True when BUTTON0 is pressed
volatile bool button1Pressed;                 // True when BUTTON1 is pressed
static bool commissioning = false;            // Tracks if device is in commissioning mode
static bool binding = false;                  // Tracks if binding has been completed

// Attribute reporting helper
static EmberStatus reportAttribute(EmberAfClusterId cluster,
                                   EmberAfAttributeId attributeID,
                                   uint8_t attribute_type,
                                   uint8_t buff[2]);

// Radio calibration callback (called when needed by stack)
void emberAfRadioNeedsCalibratingCallback(void)
{
  sl_mac_calibrate_current_channel();
}

// Zigbee event declarations
static sl_zigbee_event_t sendCommandEventOn;              // Event for sending 'On' command
static sl_zigbee_event_t sendCommandEventOff;             // Event for sending 'Off' command
static sl_zigbee_event_t network_steering_event_control;  // Event for starting network join
static sl_zigbee_event_t finding_and_binding_event_control; // Event for initiating Find & Bind
static sl_zigbee_event_t attribute_report_event_control;  // Event for periodic sensor reporting

// Event handlers
static void sendCommandEventOnHandler(sl_zigbee_event_t *event);             // Handles sending 'On' command
static void sendCommandEventOffHandler(sl_zigbee_event_t *event);            // Handles sending 'Off' command
static void network_steering_event_handler(sl_zigbee_event_t *event);        // Handles network join logic
static void finding_and_binding_event_handler(sl_zigbee_event_t *event);     // Handles Find & Bind process
static void attribute_report_event_handler(sl_zigbee_event_t *event);        // Handles periodic attribute reporting

/* Summary:
 *  Button0 triggers Zigbee network join using network steering
 *  If already joined, triggers Find & Bind to bind with a target
 *  Button1 enables reporting of temperature/humidity via Zigbee attributes
 */


/**
 * @brief Callback function called upon completion of the network steering process.
 *
 * This function is triggered when the device completes its attempt to join a Zigbee network
 * using the Network Steering plugin. If the join is successful, the function schedules the
 * Find and Bind procedure after a predefined delay. If unsuccessful, it flags commissioning as false.
 *
 * @param[in] status         The result of the network steering attempt.
 * @param[in] totalBeacons   Total number of beacons detected during the scan.
 * @param[in] joinAttempts   Number of join attempts made.
 * @param[in] finalState     The final state of the join process.
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  sl_zigbee_app_debug_print("%s network %s: 0x%02X\n", "Join", "complete", status);

  if (status != EMBER_SUCCESS) {
    commissioning = false;
  } else {
    // On successful join, schedule the Find and Bind event
    sl_zigbee_event_set_delay_ms(&finding_and_binding_event_control,
                                 FIND_AND_BIND_DELAY_MS);
  }
}

/**
 * @brief Callback function called upon completion of the Find and Bind Initiator process.
 *
 * This function is invoked when the device finishes the Find and Bind process after joining
 * a Zigbee network. If successful, it schedules periodic attribute reporting. If not,
 * it disables reporting and resets the binding state.
 *
 * @param[in] status  The result of the Find and Bind procedure.
 */
void emberAfPluginFindAndBindInitiatorCompleteCallback(EmberStatus status)
{
  sl_zigbee_app_debug_print("Find and bind initiator %s: 0x%X\n", "complete", status);

  if (status == EMBER_SUCCESS) {
    sl_zigbee_event_set_delay_ms(&attribute_report_event_control,
                                 REPORT_PERIOD_MS);
  } else {
    sl_zigbee_app_debug_print("Ensure a valid binding target!\n");
    sl_zigbee_event_set_inactive(&attribute_report_event_control);
    binding = false;
  }
}


/**
 * @brief Button state change handler callback.
 *
 * This function is triggered whenever the state of a button changes.
 * It checks if either BUTTON0 or BUTTON1 was released, and sets the
 * corresponding flags (`button0Pressed` or `button1Pressed`) to true.
 * These flags are later used to trigger corresponding actions.
 *
 * @param[in] handle  Pointer to the button instance whose state changed.
 */
void sl_button_on_change(const sl_button_t *handle)
{
  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON0) == handle) {
    if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED) {
      button0Pressed = true;
    }
  }

  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON1) == handle) {
    if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED) {
      button1Pressed = true;
    }
  }
}

/**
 * @brief Processes button press flags and triggers corresponding Zigbee events.
 *
 * This function checks the flags `button0Pressed` and `button1Pressed`, which are
 * set when BUTTON0 or BUTTON1 is released. Depending on the button pressed, it
 * activates Zigbee-related events such as sending a command or starting network
 * steering. Additionally, for BUTTON1, it schedules periodic attribute reporting.
 */
void sendMessage()
{
  if (button0Pressed) {
    sl_zigbee_event_set_active(&sendCommandEventOn);
    sl_zigbee_event_set_active(&network_steering_event_control);
  }

  if (button1Pressed) {
    sl_zigbee_event_set_active(&sendCommandEventOff);
    sl_zigbee_event_set_delay_ms(&attribute_report_event_control,
                                 REPORT_PERIOD_MS);
  }
}

/**
 * @brief Handler for the "On" Zigbee command event.
 *
 * This function is triggered by the `sendCommandEventOn` event. It prepares
 * and sends a Zigbee On command to the target device at address 0x0000.
 * It also resets the `button0Pressed` flag after sending.
 *
 * @param[in] event Pointer to the event structure (unused).
 */
static void sendCommandEventOnHandler(sl_zigbee_event_t *event)
{
  EmberStatus status;
  emberAfFillCommandOnOffClusterOn();
  emberAfSetCommandEndpoints(emberAfPrimaryEndpoint(), 1);
  emberAfCorePrintln("Sending Zigbee On Command...");
  button0Pressed = false;

  status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);

  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Command Sent Successfully");
  } else {
    emberAfCorePrintln("Command Failed, Status: 0x%x", status);
  }
}

/**
 * @brief Handler for the "Off" Zigbee command event.
 *
 * This function is triggered by the `sendCommandEventOff` event. It prepares
 * and sends a Zigbee Off command to the target device at address 0x0000.
 * It also resets the `button1Pressed` flag after sending.
 *
 * @param[in] event Pointer to the event structure (unused).
 */
static void sendCommandEventOffHandler(sl_zigbee_event_t *event)
{
  EmberStatus status;
  emberAfFillCommandOnOffClusterOff();
  emberAfSetCommandEndpoints(emberAfPrimaryEndpoint(), 1);
  emberAfCorePrintln("Sending Zigbee Off Command...");
  button1Pressed = false;

  status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);

  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Command Sent Successfully");
  } else {
    emberAfCorePrintln("Command Failed, Status: 0x%x", status);
  }
}

/**
 * @brief Main loop callback.
 *
 * This callback is periodically called by the Zigbee stack. It checks for
 * button press flags and sends commands accordingly by calling `sendMessage()`.
 */
void emberAfMainTickCallback(void)
{
  sendMessage();
}

/**
 * @brief Event handler for initiating or continuing network steering.
 *
 * Checks whether the device is already joined to a Zigbee network. If joined,
 * it activates the finding and binding process if not already bound.
 * Otherwise, it starts network steering to join a network.
 *
 * @param[in] event Pointer to the triggered event (unused).
 */
static void network_steering_event_handler(sl_zigbee_event_t *event)
{
  EmberStatus status;

  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    if (!binding) {
      sl_zigbee_event_set_active(&finding_and_binding_event_control);
    }
  } else {
    status = emberAfPluginNetworkSteeringStart();
    sl_zigbee_app_debug_print("%s network %s: 0x%X\n", "Join", "start", status);
    commissioning = true;
  }
}

/**
 * @brief Event handler for initiating Find and Bind procedure.
 *
 * This event initiates the Find and Bind process as an initiator on endpoint 1.
 * Updates the global `binding` flag on success or failure.
 *
 * @param[in] event Pointer to the triggered event (unused).
 */
static void finding_and_binding_event_handler(sl_zigbee_event_t *event)
{
  EmberStatus status = emberAfPluginFindAndBindInitiatorStart(1);

  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Bind and Find successful");
  } else {
    emberAfCorePrintln("Bind and Find failed, Status: 0x%x", status);
  }

  sl_zigbee_app_debug_print("Find and bind initiator %s: 0x%X\n", "start", status);
  binding = true;
}

/**
 * @brief Periodic event handler for reading sensor data and reporting attributes.
 *
 * Reads temperature and humidity values from the RHT sensor.
 * Sends the temperature (scaled) to the Zigbee network using attribute reporting.
 * If the device is not in a network, the function exits early.
 *
 * @param[in] event Pointer to the triggered event (unused).
 */
static void attribute_report_event_handler(sl_zigbee_event_t *event)
{
  EmberStatus status = EMBER_SUCCESS;
  uint32_t rh;
  int32_t t;
  union {
    int16_t t;
    uint16_t rh;
  } attribute;

  sl_zigbee_event_set_delay_ms(&attribute_report_event_control, REPORT_PERIOD_MS);

  if (emberAfNetworkState() != EMBER_JOINED_NETWORK) {
    return;
  }

  if (SL_STATUS_OK == sl_sensor_rht_get(&rh, &t)) {
    sl_zigbee_app_debug_print("RH: %d, Temperature: %d\n", rh, t);

    attribute.t = t / 10;

    status = reportAttribute(ZCL_TEMP_MEASUREMENT_CLUSTER_ID,
                             ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID,
                             ZCL_INT16S_ATTRIBUTE_TYPE,
                             (uint8_t *)&attribute);

    if (status == EMBER_SUCCESS) {
      emberAfCorePrintln("Temperature Report bind successful");
    } else {
      emberAfCorePrintln("Temperature report bind failed, Status: 0x%x", status);
    }

    sl_zigbee_app_debug_print("%s reported: 0x%X\n", "Temp - MeasuredValue", status);
  } else {
    sl_zigbee_app_debug_print("Failed to read RHT sensor\n");
  }
}

/**
 * @brief Reports a Zigbee attribute using unicast bindings.
 *
 * Fills and sends a Zigbee ZCL global report attributes command with
 * the specified cluster, attribute ID, and value.
 *
 * @param[in] cluster Zigbee cluster ID.
 * @param[in] attributeID Zigbee attribute ID.
 * @param[in] attribute_type Data type of the attribute.
 * @param[in] buff Pointer to a 2-byte buffer containing attribute data.
 * @return EmberStatus Result of the send operation.
 */
static EmberStatus reportAttribute(EmberAfClusterId cluster,
                                   EmberAfAttributeId attributeID,
                                   uint8_t attribute_type,
                                   uint8_t buff[2])
{
  EmberStatus status = EMBER_SUCCESS;

  uint8_t attribute_record[] = {
    LOW_BYTE(attributeID),
    HIGH_BYTE(attributeID),
    attribute_type,
    buff[0], buff[1],
  };

  emberAfFillCommandGlobalServerToClientReportAttributes(
      cluster,
      attribute_record,
      sizeof(attribute_record) / sizeof(uint8_t));

  emberAfSetCommandEndpoints(1, 1);

  status = emberAfSendCommandUnicastToBindings();

  return status;
}

/**
 * @brief Initialization callback executed during Zigbee stack startup.
 *
 * Initializes the RHT sensor and all defined application event handlers.
 * If sensor initialization fails, an error message is printed.
 */
void emberAfMainInitCallback(void)
{
  sl_status_t sc;

  sc = sl_sensor_rht_init();
  if (sc != SL_STATUS_OK) {
    sl_zigbee_app_debug_print("Relative Humidity and Temperature sensor initialization failed.\n");
  }

  sl_zigbee_event_init(&sendCommandEventOn, sendCommandEventOnHandler);
  sl_zigbee_event_init(&sendCommandEventOff, sendCommandEventOffHandler);
  sl_zigbee_event_init(&network_steering_event_control, network_steering_event_handler);
  sl_zigbee_event_init(&finding_and_binding_event_control, finding_and_binding_event_handler);
  sl_zigbee_event_init(&attribute_report_event_control, attribute_report_event_handler);
}
