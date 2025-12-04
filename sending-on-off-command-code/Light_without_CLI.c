/***************************************************************************//**
 * @file app.c
 * @brief Callbacks implementation and application specific code for Light device.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "app/framework/include/af.h"
#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"
#include "sl_simple_led_instances.h"
#include "sl_simple_led.h"
#include "app/framework/plugin/network-creator/network-creator.h"
#include "app/framework/plugin/network-creator-security/network-creator-security.h"

/***************************************************************************//**
 * @brief Button instance definitions and variables
 ******************************************************************************/
#define BUTTON0 0                    // Button 0 identifier
volatile bool button0Pressed;        // Flag to indicate button 0 press event

/***************************************************************************//**
 * @brief Event declarations for network creation
 ******************************************************************************/
static sl_zigbee_event_t createNetwork;              // Event for network creation
static void createNetworkFunc(sl_zigbee_event_t *event); // Event handler function

/***************************************************************************//**
 * AUTO-GENERATED CALLBACK FUNCTIONS
 ******************************************************************************/

/** @brief Complete network steering.
 *
 * This callback is fired when the Network Steering plugin is complete.
 *
 * @param status On success this will be set to EMBER_SUCCESS to indicate a
 * network was joined successfully. On failure this will be the status code of
 * the last join or scan attempt. Ver.: always
 *
 * @param totalBeacons The total number of 802.15.4 beacons that were heard,
 * including beacons from different devices with the same PAN ID. Ver.: always
 * 
 * @param joinAttempts The number of join attempts that were made to get onto
 * an open Zigbee network. Ver.: always
 *
 * @param finalState The finishing state of the network steering process. From
 * this, one is able to tell on which channel mask and with which key the
 * process was complete. Ver.: always
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  sl_zigbee_app_debug_println("%s network %s: 0x%02X", "Join", "complete", status);
}

/** @brief Radio needs calibrating.
 *
 * This callback is fired when the radio needs calibration. It is the 
 * application framework equivalent of ::emberRadioNeedsCalibratingHandler.
 * The application must calibrate the current channel to ensure optimal
 * radio performance.
 */
void emberAfRadioNeedsCalibratingCallback(void)
{
  sl_mac_calibrate_current_channel();
}

/***************************************************************************//**
 * END OF AUTO-GENERATED CALLBACK FUNCTIONS
 ******************************************************************************/

/** @brief Button state change callback.
 *
 * This callback is triggered whenever a button state changes (pressed or released).
 * It sets a flag when Button 0 is released, which will be processed in the main
 * tick callback to trigger network creation.
 *
 * @param handle Pointer to the button instance that triggered the callback. Ver.: always
 */
void sl_button_on_change(const sl_button_t *handle)
{
  // Check if the callback is from Button 0
  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON0) == handle){
      // Check if button was released (to avoid triggering on press)
      if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED){
          button0Pressed = true;  // Set flag to indicate button press event
      }
  }
}

/** @brief Button press interrupt wrapper.
 *
 * This function processes the button press flag and triggers the network creation
 * event. It acts as a wrapper between the button interrupt and the event system,
 * ensuring the event is triggered in the main application context rather than
 * interrupt context.
 */
void buttonPressInterruptWrapper()
{
  // Check if button 0 was pressed
  if (button0Pressed)
  {
    sl_zigbee_event_set_active(&createNetwork); // Activate the network creation event
    button0Pressed = false;                      // Reset the flag after processing
  }
}

/** @brief Network creation event handler.
 *
 * This function is called when the network creation event is triggered (typically
 * by a button press). It checks the current network status and attempts to create
 * a new Zigbee network if the device is not already part of a network.
 *
 * @param event Pointer to the event that triggered this function. Ver.: always
 */
static void createNetworkFunc(sl_zigbee_event_t *event)
{
  EmberNetworkStatus networkStatus;
  EmberStatus status;
  
  // Get the current network state of the device
  networkStatus = emberAfNetworkState();
  
  // Check if device is not currently in any network
  if(networkStatus == EMBER_NO_NETWORK)
  {
    // Attempt to create a new centralized network
    status = emberAfPluginNetworkCreatorStart(true);
    
    if(status == EMBER_SUCCESS){
      emberAfCorePrintln("Network Creation success");
    }
    else{
      emberAfCorePrintln("Network Creation failed");
    }
  }
  else{
    emberAfCorePrintln("Already In a network");
  }
}

/** @brief Stack status callback.
 *
 * This callback is fired when the stack status changes. It is used to open the
 * network for joining when the network is successfully formed, allowing other
 * devices to join the network.
 *
 * @param status The new stack status. EMBER_NETWORK_UP indicates the network
 * is up and running. Ver.: always
 */
void emberAfStackStatusCallback(EmberStatus status)
{
  // Check if network formation was successful
  if (status == EMBER_NETWORK_UP) {
    // Open the network to allow other devices to join
    // Equivalent to CLI command: plugin network-creator-security open-network
    EmberStatus sec = emberAfPluginNetworkCreatorSecurityOpenNetwork();
    emberAfCorePrintln("open-network -> 0x%02X", sec);
  }
}

/** @brief Main initialization callback.
 *
 * This callback is called from the application's main function during initialization.
 * It is used to initialize application-specific events and variables before the
 * main event loop starts.
 */
void emberAfMainInitCallback(void)
{
  // Initialize the network creation event with its handler function
  sl_zigbee_event_init(&createNetwork, createNetworkFunc);
}

/** @brief Main tick callback.
 *
 * This callback is called repeatedly in the main application loop. It is used
 * to process button press events and trigger corresponding actions. The tick
 * callback provides a safe context (outside of interrupt) to handle events.
 */
void emberAfMainTickCallback(void)
{
  // Check if button 0 was pressed and process the event
  if (button0Pressed)
  {
    buttonPressInterruptWrapper();
  }
}

/** @brief Post attribute change callback.
 *
 * This callback is fired after an attribute value has been changed. It monitors
 * the On/Off cluster attribute and controls LED0 based on the attribute value,
 * providing visual feedback of the device's On/Off state.
 *
 * @param endpoint The endpoint on which the attribute change occurred. Ver.: always
 *
 * @param clusterId The cluster ID of the attribute that changed. Ver.: always
 *
 * @param attributeId The ID of the specific attribute that changed. Ver.: always
 *
 * @param mask Indicates whether the attribute is a client or server attribute
 * (CLUSTER_MASK_CLIENT or CLUSTER_MASK_SERVER). Ver.: always
 *
 * @param manufacturerCode The manufacturer code associated with the attribute,
 * or 0 if it is a standard ZCL attribute. Ver.: always
 *
 * @param type The data type of the attribute. Ver.: always
 *
 * @param size The size of the attribute value in bytes. Ver.: always
 *
 * @param value Pointer to the new attribute value. Ver.: always
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
  // Check if the changed attribute is the On/Off attribute of the On/Off cluster
  // on the server side
  if (clusterId == ZCL_ON_OFF_CLUSTER_ID
      && attributeId == ZCL_ON_OFF_ATTRIBUTE_ID
      && mask == CLUSTER_MASK_SERVER) {
    bool onOff;
    
    // Read the current value of the On/Off attribute from the server
    if (emberAfReadServerAttribute(endpoint,
                                   ZCL_ON_OFF_CLUSTER_ID,
                                   ZCL_ON_OFF_ATTRIBUTE_ID,
                                   (uint8_t *)&onOff,
                                   sizeof(onOff))
        == EMBER_ZCL_STATUS_SUCCESS) {
      // Control LED0 based on the attribute value
      if (onOff) {
        sl_led_turn_on(&sl_led_led0);  // Turn on LED0 when attribute is ON (true)
      } else {
        sl_led_turn_off(&sl_led_led0); // Turn off LED0 when attribute is OFF (false)
      }
    }
  }
}
