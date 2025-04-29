//// LIGHT COORDINATOR CODE ////

#include "app/framework/include/af.h"
#include "sl_simple_led_instances.h"
#include "sl_simple_led.h"

/**
 * @brief Callback function triggered upon completion of the network steering process.
 *
 * This function prints the status of the network joining process after attempting to join.
 *
 * @param status         Result of the network steering operation.
 * @param totalBeacons   Total beacons received during scanning.
 * @param joinAttempts   Number of join attempts made.
 * @param finalState     Final state of the network steering process.
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  sl_zigbee_app_debug_println("%s network %s: 0x%02X", "Join", "complete", status);
}

/**
 * @brief Callback function to calibrate the radio if needed.
 *
 * This ensures optimal communication by recalibrating the radio frequency of the current channel.
 */
void emberAfRadioNeedsCalibratingCallback(void)
{
  sl_mac_calibrate_current_channel();
}

/**
 * @brief Attribute change handler for On/Off cluster.
 *
 * This function toggles the LED on or off based on received commands. It's commonly used
 * to provide visual feedback or control external devices.
 *
 * @param endpoint          The endpoint where the change occurred.
 * @param clusterId         The cluster ID of the attribute.
 * @param attributeId       The attribute ID that changed.
 * @param mask              Cluster mask (client/server).
 * @param manufacturerCode  Manufacturer-specific code (not used here).
 * @param type              Data type of the attribute.
 * @param size              Size of the data.
 * @param value             Pointer to the new value.
 */
void emberAfPostAttributeChangeCallback(uint8_t endpoint,
                                        EmberAfClusterId clusterId,
                                        EmberAfAttributeId attributeId,
                                        uint8_t mask,
                                        uint16_t manufacturerCode,
                                        uint8_t type,
                                        uint8_t size,
                                        uint8_t* value)
{
  // Check for On/Off server cluster
  if (clusterId == ZCL_ON_OFF_CLUSTER_ID
      && attributeId == ZCL_ON_OFF_ATTRIBUTE_ID
      && mask == CLUSTER_MASK_SERVER) {

    bool onOff;
    // Read the new attribute value
    if (emberAfReadServerAttribute(endpoint,
                                   ZCL_ON_OFF_CLUSTER_ID,
                                   ZCL_ON_OFF_ATTRIBUTE_ID,
                                   (uint8_t *)&onOff,
                                   sizeof(onOff)) == EMBER_ZCL_STATUS_SUCCESS) {
      // Control LED based on the On/Off command
      if (onOff) {
        sl_led_turn_on(&sl_led_led0);
      } else {
        sl_led_turn_off(&sl_led_led0);
      }
    }
  }
}

/**
 * @brief Custom callback to handle attribute reporting from sensor nodes.
 *
 * This function processes incoming reports specifically from the Relative Humidity and Temperature
 * clusters. It decodes the reported values and prints them in a human-readable format.
 *
 * @param clusterId   The ID of the cluster reporting the attributes.
 * @param buffer      Pointer to the received data buffer.
 * @param bufLen      Length of the received data buffer.
 * @return            True if the report was handled, false otherwise.
 */
bool emberAfReportAttributesCallback(EmberAfClusterId clusterId,
                                     int8u *buffer,
                                     int16u bufLen)
{
  int16_t attribute;

  // Handle only Temperature and Relative Humidity clusters
  if (clusterId == ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID ||
      clusterId == ZCL_TEMP_MEASUREMENT_CLUSTER_ID) {

    // Extract attribute value (2 bytes: LSB then MSB)
    attribute = (buffer[bufLen-1] << 8) | (buffer[bufLen-2]);

    // Print formatted sensor data
    if (clusterId == ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID) {
      emberAfCorePrintln("RH: %2d.%2d %%", attribute / 100, attribute % 100);
    } else {
      emberAfCorePrintln("Temperature: %2d.%2d C", attribute / 100, attribute % 100);
    }
    return true;
  }

  return false; // Unhandled cluster
}

