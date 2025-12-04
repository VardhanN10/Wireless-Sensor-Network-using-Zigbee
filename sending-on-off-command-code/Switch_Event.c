/***************************************************************************//**
 * @file app.c
 * @brief Switch device with event-based command sending
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

// Button definitions
#define BUTTON0 0
#define BUTTON1 1
volatile bool button0Pressed;// Flag for Button 0
volatile bool button1Pressed;// Flag for Button 1

/** @brief Network steering completion callback.
 *
 * This gets called automatically when the network steering process finishes.
 * Just prints out the join status to help with debugging.
 *
 * @param status Shows if joining was successful (EMBER_SUCCESS) or what went 
 * wrong if it failed. Ver.: always
 *
 * @param totalBeacons How many 802.15.4 beacons were detected during the scan,
 * including beacons from other networks. Ver.: always
 * 
 * @param joinAttempts Number of times we tried to join a network. Ver.: always
 *
 * @param finalState The final state when steering completed - tells you which
 * channel and key were used. Ver.: always
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  sl_zigbee_app_debug_println("%s network %s: 0x%02X", "Join", "complete", status);
}

/** @brief Radio calibration callback.
 *
 * Called when the radio needs to be recalibrated. We just calibrate the 
 * current channel to keep things running smoothly.
 */
void emberAfRadioNeedsCalibratingCallback(void)
{
  sl_mac_calibrate_current_channel();
}

/*
 * Event declarations for sending ON and OFF commands
 * Using events gives us better control over when commands are actually sent
 */
static sl_zigbee_event_t sendCommandEventOn;
static sl_zigbee_event_t sendCommandEventOff;
static void sendCommandEventOffHandler(sl_zigbee_event_t *event);

/** @brief Button state change callback.
 *
 * Called whenever a button is pressed or released. We set flags here when
 * buttons are released - Button 0 triggers ON command, Button 1 triggers OFF.
 * We don't send commands directly from here since the newer SDK doesn't like
 * network operations in interrupts.
 *
 * @param handle Pointer to the button that changed state. Ver.: always
 */
void sl_button_on_change(const sl_button_t *handle)
{
  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON0) == handle){
    if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED){
        button0Pressed = true;
        emberAfCorePrintln("Button 0 Pressed");
    }
  }
    if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON1) == handle){
      if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED){
         button1Pressed= true;
         emberAfCorePrintln("Button 1 Pressed");
    }
  }
}

/** @brief Trigger command events based on button flags.
 *
 * Checks which button was pressed and activates the corresponding event.
 * The actual Zigbee commands get sent in the event handlers, not here.
 */
void sendMessage(){
  if(button0Pressed){
      sl_zigbee_event_set_active(&sendCommandEventOn); // Setting event On active
  }
  if(button1Pressed){
      sl_zigbee_event_set_active(&sendCommandEventOff); // Setting event OFF active
  }
}

/** @brief Event handler for ON command.
 *
 * This runs when the ON event is triggered (Button 0 pressed). Fills in the
 * ON command, sets up the endpoints, and sends it via unicast to the Light
 * device at address 0x0000.
 *
 * @param event Pointer to the event that triggered this handler. Ver.: always
 */
static void sendCommandEventOnHandler(sl_zigbee_event_t *event)
{
  EmberStatus status;
  emberAfFillCommandOnOffClusterOn();
  emberAfSetCommandEndpoints(emberAfPrimaryEndpoint(), 1);
  emberAfCorePrintln("Sending Zigbee On Command...");
  button0Pressed =false;
  status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);
  if (status == EMBER_SUCCESS) {
      emberAfCorePrintln("Command Sent Successfully");
  } else {
      emberAfCorePrintln("Command Failed, Status: 0x%x", status);
  }
}

/** @brief Event handler for OFF command.
 *
 * This runs when the OFF event is triggered (Button 1 pressed). Fills in the
 * OFF command, sets up the endpoints, and sends it via unicast to the Light
 * device at address 0x0000.
 *
 * @param event Pointer to the event that triggered this handler. Ver.: always
 */
static void sendCommandEventOffHandler(sl_zigbee_event_t *event)
{
  EmberStatus status;
  emberAfFillCommandOnOffClusterOff();
  emberAfSetCommandEndpoints(emberAfPrimaryEndpoint(), 1);
  emberAfCorePrintln("Sending Zigbee On Command...");
  button1Pressed=false;
  status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);
  if (status == EMBER_SUCCESS) {
      emberAfCorePrintln("Command Sent Successfully");
  } else {
      emberAfCorePrintln("Command Failed, Status: 0x%x", status);
  }
}

/** @brief Main application tick callback.
 *
 * Called repeatedly in the main loop. We use this to check button flags
 * and trigger events. Runs outside interrupt context so it's safe for
 * network operations.
 */
void emberAfMainTickCallback(void)
{
        sendMessage();
}

/** @brief Main initialization callback.
 *
 * Called once during startup. We initialize both command events here and
 * connect them to their handler functions so they're ready to go when
 * buttons get pressed.
 */
void emberAfMainInitCallback(void){
  sl_zigbee_event_init(&sendCommandEventOn, sendCommandEventOnHandler);
  sl_zigbee_event_init(&sendCommandEventOff,sendCommandEventOffHandler);
}
