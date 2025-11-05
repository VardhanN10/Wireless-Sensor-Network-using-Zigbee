#include "app/framework/include/af.h"
#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"
#include "sl_simple_led_instances.h"
#include "sl_simple_led.h"
#include "app/framework/plugin/network-creator/network-creator.h"
#include "app/framework/plugin/network-creator-security/network-creator-security.h"

/*** Variables for Button Change Interrupts**/
#define BUTTON0 0
volatile bool button0Pressed;
/*********************/

/** EVENT declare**/
static sl_zigbee_event_t createNetwork;
static void createNetworkFunc(sl_zigbee_event_t *event);
/*****/


//**AUTO GEN CODE**//
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

/** @brief
 *
 * Application framework equivalent of ::emberRadioNeedsCalibratingHandler
 */
void emberAfRadioNeedsCalibratingCallback(void)
{
  sl_mac_calibrate_current_channel();
}

////AUTO GEN CODE- END/////////

void sl_button_on_change(const sl_button_t *handle)
{
  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON0) == handle){
      if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED){
          button0Pressed = true;
      }
  }
}

void buttonPressInterruptWrapper()
{

if (button0Pressed)
  {

    sl_zigbee_event_set_active(&createNetwork); // Setting event On active
    button0Pressed = false; // Resetting the Flag
  }
}

static void createNetworkFunc(sl_zigbee_event_t *event)
{

  EmberNetworkStatus networkStatus;
  EmberStatus status;
  networkStatus = emberAfNetworkState();/** @brief Indicate whether the stack is currently up.
   *
   *   Returns true if the stack is joined to a network and
   *   ready to send and receive messages.  This reflects only the state
   *   of the local node; it does not indicate whether other nodes are
   *   able to communicate with this node.
   *
   * @return True if the stack is up, false otherwise.
   */
  if(networkStatus == EMBER_NO_NETWORK)
    {
      status = emberAfPluginNetworkCreatorStart(true);
      if(status == EMBER_SUCCESS){
           emberAfCorePrintln("Network Creation success");

       }else{
           emberAfCorePrintln("Network Creation failed");
       }
    }
  else{
      emberAfCorePrintln("Already In a network");
  }

}

void emberAfStackStatusCallback(EmberStatus status)
{

  if (status == EMBER_NETWORK_UP) {
    // Equivalent to: plugin network-creator-security open-network
    EmberStatus sec = emberAfPluginNetworkCreatorSecurityOpenNetwork();
    emberAfCorePrintln("open-network -> 0x%02X", sec);
  }
}

void emberAfMainInitCallback(void)
{

  sl_zigbee_event_init(&createNetwork, createNetworkFunc);
}

void emberAfMainTickCallback(void){

  if (button0Pressed)
    {
      buttonPressInterruptWrapper();
    }
}

void emberAfPostAttributeChangeCallback(uint8_t endpoint,
                                        EmberAfClusterId clusterId,
                                        EmberAfAttributeId attributeId,
                                        uint8_t mask,
                                        uint16_t manufacturerCode,
                                        uint8_t type,
                                        uint8_t size,
                                        uint8_t* value)
{
  if (clusterId == ZCL_ON_OFF_CLUSTER_ID
      && attributeId == ZCL_ON_OFF_ATTRIBUTE_ID
      && mask == CLUSTER_MASK_SERVER) {
    bool onOff;
    if (emberAfReadServerAttribute(endpoint,
                                   ZCL_ON_OFF_CLUSTER_ID,
                                   ZCL_ON_OFF_ATTRIBUTE_ID,
                                   (uint8_t *)&onOff,
                                   sizeof(onOff))
        == EMBER_ZCL_STATUS_SUCCESS) {
      if (onOff) {
        sl_led_turn_on(&sl_led_led0);
      } else {
        sl_led_turn_off(&sl_led_led0);
      }
    }
  }
}
