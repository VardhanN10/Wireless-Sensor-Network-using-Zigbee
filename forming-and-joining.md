
# Forming and Joining the Zigbee Network

This section explains how to build and configure a **Coordinator (Light)** and a **Router (Switch)** from scratch using the **EFR32MG12 Silicon Labs Boards** and the **Zigbee protocol**.

---

##  Overview

The **Light device** acts as both the **Coordinator** and **Trust Center**, forming and securing the Zigbee network. It permits other devices to join and manages security keys.

The **Switch device** is configured as a Router. It joins the network initiated by the Light and sends **On/Off commands** using physical buttons.

---

##  1. Creating the Light Device (Coordinator)

###  Step 1: Create New Project

- Create a new Zigbee project using the `Zigbee - Minimal` example in **Simplicity Studio**.

###  Step 2: Configure Software Components in `.slcp`

Install the following components:

- `Zigbee - Utility > Zigbee Device Config` → **Set to Coordinator**
- `Zigbee 3.0 - Network Creator`
- `Zigbee 3.0 - Network Creator Security`
- `Services > CLI - Global Configuration`  → Set **Command Prompt name** to: `Light`
-  `Zigbee - Common > Reporting`
-  `Zigbee - Zigbee 3.0  > Find and Bind Target`

###  Step 3: Uninstall Unnecessary Components

Remove components not needed for Coordinator:

- `Network Steering`
- `Update TC Link Key`

###  Step 4: Configure ZCL Endpoint

Open **ZCL Configurator** and:

- Edit the existing endpoint
- Select: `HA On/Off Light (0x0100)`
- Ensure all required clusters and attributes are installed properly

 #### Configure the endpoint for sending the sensor data 

 - In the Measurement and Sensing section enable the **Temperature Measurement** as **Server**
 - Now Configure the attributes by enabling the reporting status 

###  Step 5: Build and Flash

- Build the project
- Flash the generated `.s37` file to the board

>  **Important:** Perform an *Erase* operation before flashing to avoid legacy network conflicts.

###  Step 6: Verify Device

- In **Device Adapters**, select the board
- Open **Serial Console 1**
- Press `Enter` → You should see the Light device boot logs

---

##  2. Creating the Switch Device (Router)

###  Step 1: Use Zigbee - Minimal Example

This example is already configured as a Router.

###  Configure Software Components in `.slcp`

Install the following components:

- `Zigbee - Common > Identify Feedback`
- `Zigbee - Common > Reporting`
- `Bluetooth - Application > Miscellaneous > Relative Humidity and Temperature Sensor`
- `Zigbee - Zigbee 3.0  > Find and Bind Target`
- `Platform - Button > Simple Button > Add two instances`

###  Step 3: Configure ZCL Endpoint

Open **ZCL Configurator** and:

- Edit the endpoint
- Select: `HA On/Off Switch (0x0000)`
- Confirm component installation

 #### Configure the endpoint for sending the sensor data 

 - In the Measurement and Sensing section enable the **Temperature Measurement** as **Client**
 - Now Configure the attributes by enabling the reporting status 

###  Step 4: Build and Flash

- Build the project
- Flash the `.s37` binary to the second board

---

##  3. Verify Install Code (Security Step)

###  Step 1: Check Existing Install Code

Run the following command with only the **Switch device** connected:

```bash
C:\SiliconLabs\SimplicityStudio\v5\developer\adapter_packs\commander\commander.exe tokendump --tokengroup znet
```
If install code is present, you’ll see a valid token output

If not, proceed to the next step

##  4. Derive and Program Install Code

Create a new batch file `install_code.bat` in the project directory with the following contents:

```batch
@echo off

:: Automatic Installation Code Programming

if "%PATH_SCMD%"=="" (
  set COMMANDER="C:\SiliconLabs\SimplicityStudio\v5\developer\adapter_packs\commander\commander.exe"
) else (
  set COMMANDER=%PATH_SCMD%\commander.exe
)

set DEFAULT_INSTALL_CODE="83FED3407A939723A5C639B26916D505"

cd %~dp0

if not exist "%COMMANDER%" (
  echo Error: Simplicity Commander not found.
  pause
  goto:eof
)

echo **********************************************************************
echo Programming Installation Code...
echo **********************************************************************

%COMMANDER% flash --tokengroup znet --token "Install Code: !ERASE!"
%COMMANDER% flash --tokengroup znet --token "Install Code:%DEFAULT_INSTALL_CODE%"
%COMMANDER% tokendump --tokengroup znet --token TOKEN_MFG_INSTALLATION_CODE

pause

```
Run the batch file via Command Prompt to flash the install code.

---

##  4. Network Formation – Centralized (With and Without Security)

After completing the initial setup of both Light (Coordinator) and Switch (Router) devices, you can now form a centralized Zigbee network in two ways:

- **With Security** (Zigbee 3.0 standard-compliant, uses Install Codes and Link Keys)
-  **Without Security** (faster setup, not recommended for production)

---
### Derive a link key from the install code

####  Step 1: Derive Link Key from Install Code

To allow a secure join, the Coordinator (Light) must know the install code of the joining device (Switch), and derive the **link key** from it.

1. **Connect the Light (Coordinator) board via USB.**

2. **Open the Serial Console 1** for the **Switch** device.

3. **Get the EUI64 (physical address)** of the Switch by entering the following command:

```bash
info
```
Note the EUI64 shown in the output. It should look something like:
```
EUI64: 90 FD 9F FF FE 19 B1 FC
```
On the Coordinator (Light) console, use the following command to add the install code and derive the link key:
```
option install-code 0 {<Switch EUI64>} {<Install Code + CRC>}
```
 Replace placeholders with actual values. Here's an example:
```
option install-code 0 {90 FD 9F FF FE 19 B1 FC} {83 FE D3 40 7A 93 97 23 A5 C6 39 B2 69 16 D5 05 C3 B5}
```
Explanation:

- 0 is the link key table index (can be any free index).

- {EUI64} is the physical address of the joining node (Switch).

- {Install Code + CRC} is the 16-byte install code plus 2-byte CRC.

#### How to find the CRC:

After running your install_code.bat file (from the earlier setup), it shows the derived key and CRC.

The CRC is displayed in little-endian format — reverse the two bytes to use in the CLI.

#### Example:

CRC output: B5 C3 (little endian)

Use as: C3 B5

#### Verify the link key is added correctly by running on the Light (Coordinator):

```
keys print
```
You should see the transient key listed, e.g.:
```
Transient Key Table:
66 B6 90 09 81 E1 EE 3C A4 20 6B 6B 86 1C 02 BB
```
---

### Form the Centralized Network **With Security**
### On the Light (Coordinator):
```
plugin network-creator start 1
```
Then check the PAN ID of the network:
```
network id
```
####  Open the Network to Allow Join
Still on the Light, use this command to allow the Switch (Router) to join:
```
plugin network-creator-security open-with-key {<Switch EUI64>} {<Link Key>}
```
Example:
```
plugin network-creator-security open-with-key {90 FD 9F FF FE 19 B1 FC} {66 B6 90 09 81 E1 EE 3C A4 20 6B 6B 86 1C 02 BB}
```
#### Join the Network on the Switch
On the Switch (Router) console:
```
plugin network-steering start 0
```
If successful, the Switch will join the network and exchange keys with the Coordinator.
---
 ### Forming a Centralized Network Without Security

#### Step 1: Erase and Reset Devices (Optional but Recommended)
Make sure old settings are erased. Re-flash the .s37 files and open Serial Console 1 for each device.

#### Step 2: Form the Network (No Security)
On the Light (Coordinator) console:
```
plugin network-creator form 1 0x1234 3 12
plugin network-creator-security open-network
```

#### Step 3: Join the Network on the Switch
On the Switch (Router) console:
```
plugin network-steering start 0
```
The Switch should now join the network without using install codes or keys.
---
## Summary

You have now:

- Configured a Coordinator (Light) and Router (Switch)
- Created and flashed the firmware
- Verified and programmed the Install Code for secure joining

#### Proceed to the next section:  
- [ Sending On/Off Commands](./exchange-sensor-data.md)
---
