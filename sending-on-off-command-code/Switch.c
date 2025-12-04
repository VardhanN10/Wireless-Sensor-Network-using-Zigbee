/***************************************************************************//**
 * @file app.c
 * @brief Switch device implementation - handles button presses and sends commands
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

#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"

// Button definitions
#define BUTTON0 0
#define BUTTON1 1
volatile bool button0Pressed; // flag for button0
volatile bool button1Pressed; // flag for button1

/* 
 * Button interrupt handler
 * 
 * This gets called whenever a button state changes. We're using flags here instead of 
 * sending messages directly because the newer SDK versions don't allow sending from 
 * interrupts (and it's not good practice anyway). So we just set a flag when a button 
 * is released, and handle the actual message sending later in the main loop.
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
 * Send ON/OFF commands based on which button was pressed
 * 
 * Button 0 sends ON command, Button 1 sends OFF command.
 * Commands are sent via unicast to node 0x0000 (the coordinator/Light device).
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
 * Main application loop callback
 * 
 * Gets called repeatedly in the main loop. We check here if any buttons were pressed
 * and handle sending the messages. This runs outside of interrupt context so it's 
 * safe to do network operations here.
 */
void emberAfMainTickCallback(void)
{
    if (button0Pressed || button1Pressed)
    {
        sendMessage();
    }
}
