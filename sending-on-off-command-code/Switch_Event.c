#include "app/framework/include/af.h"
#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"
#define BUTTON0 0
#define BUTTON1 1

volatile bool button0Pressed;// Flag for Button 0

volatile bool button1Pressed;// Flag for Button 1

// autogen function
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  sl_zigbee_app_debug_println("%s network %s: 0x%02X", "Join", "complete", status);
}

// autogen function
void emberAfRadioNeedsCalibratingCallback(void)
{
  sl_mac_calibrate_current_channel();
}

/*** Declare Event Handlers **/
static sl_zigbee_event_t sendCommandEventOn;
static sl_zigbee_event_t sendCommandEventOff;


static void sendCommandEventOffHandler(sl_zigbee_event_t *event);
/*****/

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

void sendMessage(){

  if(button0Pressed){

      sl_zigbee_event_set_active(&sendCommandEventOn); // Setting event On active

  }
  if(button1Pressed){
      sl_zigbee_event_set_active(&sendCommandEventOff); // Setting event OFF active
  }
}
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
void emberAfMainTickCallback(void)
{
        sendMessage();
}
void emberAfMainInitCallback(void){
  sl_zigbee_event_init(&sendCommandEventOn, sendCommandEventOnHandler);
  sl_zigbee_event_init(&sendCommandEventOff,sendCommandEventOffHandler);
}

