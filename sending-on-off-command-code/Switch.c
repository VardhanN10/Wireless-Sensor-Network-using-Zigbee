#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"
#define BUTTON0 0
#define BUTTON1 1

volatile bool button0Pressed; // flag for button0
volatile bool button1Pressed; // flag for button1

/* As we are sending commands based on button change we are using this
 Interrupt. Generally we can call the send message function within interrupt but the new version of SDK wont allow it and also not the best practice. So this function enables the flag if the button is pressed.

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

/*
We Use this function for sending messages based on the flag status.
*/
void sendMessage()
{
    EmberStatus status;

    if (button0Pressed)
    {
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

        button0Pressed = false; // Resetting the Flag
    }

    if (button1Pressed)
    {
        emberAfFillCommandOnOffClusterOff();
        emberAfCorePrintln("Button1 is pressed");
        emberAfCorePrintln("Command is zcl on-off OFF");

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

        button1Pressed = false; // Resetting the Flag
    }
}

/*
Main Tick.
Whenever main application tick is called, this callback will be called at the end of the main tick execution
This function is called in each iteration of the main application loop and can be used to perform periodic functions. The frequency with which this function is called depends on how quickly the main loop runs. If the application blocks at any time during the main loop, this function will not be called until execution resumes.

*/
void emberAfMainTickCallback(void)
{
    if (button0Pressed || button1Pressed)
    {
        sendMessage();
    }
}
