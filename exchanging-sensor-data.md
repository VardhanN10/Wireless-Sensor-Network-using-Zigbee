#  Interaction and Sensor Data Exchange

After successfully forming and joining the Zigbee network, this phase enhances device interactivity and introduces sensor data communication. The implementation utilizes the onboard **buttons** and **LEDs** of the EFR32MG12 boards to demonstrate a simple Zigbee-based control and reporting system.

---

##  Objectives

- Enable button-triggered **On/Off command exchange** between devices.
- Enable **sensor data reporting** from the Router (Switch) to the Coordinator (Light).
- Use the built-in LEDs and buttons for real-time visual feedback and testing.

---

##  Hardware Setup

- **EFR32MG12 Zigbee Boards**
- **Built-in Buttons**: PB0 and PB1
- **Built-in LEDs**: LED1 and LED2

---

##  Functional Overview

### Coordinator (Light)

- Reacts to commands from the Router:
  - PB0 pressed on Router → **LED ON**
  - PB1 pressed on Router → **LED OFF**
- Acts as the **Zigbee Trust Center**
- Receives and processes **sensor data** via custom endpoints

### Router (Switch)

- **PB0**: Initiates joining the Zigbee network
- **PB1**: Enables periodic sensor data reporting (e.g., temperature, humidity)
- Sends On/Off commands and sensor data to the Coordinator

---

##  Flashing and Building

1. Integrate the custom logic into the application code:
   - Coordinator → Handle incoming On/Off commands and display via LEDs
   - Router → Send commands on button press and report sensor data via endpoint

2. Build the project.

3. Flash `.s37` file into device via Simplicity Studio.

---

##  Workflow

### Light (Coordinator)

1. **Form the Network**

```bash
plugin network-creator start 1
plugin network-creator-security open-network
```
2. **Enable Binding Mode**
```
plugin find_and_bind target 1
```
This allows the Coordinator to accept and bind with the joining Switch device.

##  Switch (Router)

All functionality is embedded in the application and triggered by button presses:

| Button | Function                   |
|--------|----------------------------|
| PB0    | Join the Zigbee Network    |
| PB1    | Enable Sensor Reporting    |

>  Sensor data is transmitted periodically after **PB1** is pressed.

---

##  Testing & Observation

- Connect devices via **USB**
- Monitor via **Serial Console 1** in **Simplicity Studio**
- Observe log output to confirm:
  -  Successful join and binding
  -  On/Off commands received
  -  Sensor data being reported

---

##  Notes

- Ensure **both devices are erased** before flashing to avoid retained network information.
- Bindings must be successfully established using the `find_and_bind` plugin before communication occurs.
- Sensor data **endpoint must be configured** properly in the **ZCL (Zigbee Cluster Library)** section of Simplicity Studio on both sender and receiver.

---

##  Summary

This section demonstrates full **Zigbee device interactivity**:

-  Real-time **On/Off control** using buttons and LEDs
-  **Sensor data exchange** between Zigbee Router and Coordinator
-  Dynamic behavior triggered by hardware events
