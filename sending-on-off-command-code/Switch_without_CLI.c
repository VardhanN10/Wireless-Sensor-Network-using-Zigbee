/***************************************************************************//**
 * @file app.c
 * @brief Switch device that can join networks and send ON/OFF commands
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
#include "app/framework/plugin/network-creator/network-creator.h"
#include "app/framework/plugin/network-creator-security/network-creator-security.h"

/*** Variables for Button Change Interrupts**/
#define BUTTON0 0  // Button 0 triggers network join and sends ON command
#define BUTTON1 1  // Button 1 sends OFF command
volatile bool button0Pressed;
volatile bool button1Pressed;
/*********************/

/** EVENT declare**/
static sl_zigbee_event_t joinNetwork;  // Event for joining network
static void joinNetworkHandler(sl_zigbee_event_t *event);
/*****/

//*AUTO GEN CODE**//
/** @brief Network steering completion callback.
 *
 * This gets called automatically when the network steering process finishes.
 * Prints out detailed status including beacon count and join attempts to help
 * with debugging network issues.
 *
 * @param status Shows if joining was successful (EMBER_SUCCESS) or what went 
 * wrong if it failed. Ver.: always
 *
 * @param totalBeacons How many 802.15.4 beacons were detected during the scan,
 * including beacons from other networks with the same PAN ID. Ver.: always
 * 
 * @param joinAttempts Number of times we tried to join a network. Ver.: always
 *
 * @param finalState The final state when steering completed - tells you which
 * channel mask and key were used. Ver.: always
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  sl_zigbee_app_debug_println("Network Steering complete: 0x%02X (beacons:%u attempts:%u state:0x%02X)",
                              status, totalBeacons, joinAttempts, finalState);
  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Joined network successfully.");
  } else {
    emberAfCorePrintln("Failed to join network (0x%02X).", status);
  }
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

////AUTO GEN CODE- END/////////

/** @brief Button state change callback.
 *
 * Called whenever a button is pressed or released. We set flags here when
 * buttons are released. Button 0 both joins the network and sends ON commands,
 * Button 1 sends OFF commands. We use flags instead of doing network operations
 * directly since the SDK doesn't like that in interrupt context.
 *
 * @param handle Pointer to the button that changed state. Ver.: always
 */
void sl_button_on_change(const sl_button_t *handle)
{
  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON0) == handle){
    if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED){
      button0Pressed = true;
    }
  }
  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON1) == handle){
    if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED){
      button1Pressed= true;
    }
  }
}

/** @brief Handle button presses and send Zigbee commands.
 *
 * This gets called from the main tick callback when buttons are pressed.
 * Button 0 triggers both network joining (via event) and sends an ON command.
 * Button 1 sends an OFF command. Commands are sent via unicast to node 0x0000
 * (the coordinator/Light device).
 */
void sendMessage()
{
  EmberStatus status;

  if (button0Pressed)
  {
    // Trigger network join event in case we're not in a network yet
    sl_zigbee_event_set_active(&joinNetwork);
    
    // Send ON command
    emberAfFillCommandOnOffClusterOn();
    emberAfCorePrintln("Button0 is pressed");
    emberAfCorePrintln("Command is zcl on-off ON");

    emberAfSetCommandEndpoints(emberAfPrimaryEndpoint(), 1);
    status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);

    if (status == EMBER_SUCCESS)
    {
      emberAfCorePrintln("Command is successfully sent");
    }
    else
    {
      emberAfCorePrintln("Failed to send");
      emberAfCorePrintln("Status code: 0x%x", status);
    }

    button0Pressed = false;
  }

  if (button1Pressed)
  {
    // Send OFF command
    emberAfFillCommandOnOffClusterOff();
    emberAfSetCommandEndpoints(emberAfPrimaryEndpoint(), 1);
    emberAfCorePrintln("Sending Zigbee On Command...");
    button1Pressed = false;
    status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);

    if (status == EMBER_SUCCESS) {
      emberAfCorePrintln("Command Sent Successfully");
    } else {
      emberAfCorePrintln("Command Failed, Status: 0x%x", status);
    }
  }
}

/** @brief Network join event handler.
 *
 * This runs when the join network event is triggered (Button 0 pressed).
 * Checks if we're already in a network - if not, it starts the network steering
 * process to scan for and join an available network.
 *
 * @param event Pointer to the event that triggered this handler. Ver.: always
 */
static void joinNetworkHandler(sl_zigbee_event_t *event)
{
  EmberNetworkStatus networkStatus = emberAfNetworkState();
  if (networkStatus == EMBER_NO_NETWORK) {
    emberAfCorePrintln("Starting Network Steering...");
    EmberStatus status = emberAfPluginNetworkSteeringStart();
    emberAfCorePrintln("emberAfPluginNetworkSteeringStart() -> 0x%02X", status);
  } else {
    emberAfCorePrintln("Already in a network(state:%u).", networkStatus);
  }
}

/** @brief Main application tick callback.
 *
 * Called repeatedly in the main loop. We use this to check button flags
 * and trigger the appropriate actions. Runs outside interrupt context so 
 * it's safe for network operations.
 */
void emberAfMainTickCallback(void)
{
  if (button0Pressed || button1Pressed)
  {
    sendMessage();
  }
}

/** @brief Main initialization callback.
 *
 * This function is called from the application's main function during startup.
 * It gives us a chance to initialize anything we need before the network and
 * plugins start up. Here we initialize the join network event and connect it
 * to its handler function.
 */
void emberAfMainInitCallback(void)
{
  sl_zigbee_event_init(&joinNetwork, joinNetworkHandler);
}

