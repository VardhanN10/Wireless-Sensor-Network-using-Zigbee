
# 📡 Forming and Joining the Zigbee Network

This section explains how to build and configure a **Coordinator (Light)** and a **Router (Switch)** from scratch using the **EFR32MG12 Silicon Labs Boards** and the **Zigbee protocol**.

---

## ⚙️ Overview

The **Light device** acts as both the **Coordinator** and **Trust Center**, forming and securing the Zigbee network. It permits other devices to join and manages security keys.

The **Switch device** is configured as a Router. It joins the network initiated by the Light and sends **On/Off commands** using physical buttons.

---

## 🔧 1. Creating the Light Device (Coordinator)

### 🧪 Step 1: Create New Project

- Create a new Zigbee project using the `Zigbee - Minimal` example in **Simplicity Studio**.

### 🛠 Step 2: Configure Software Components in `.slcp`

Install the following components:

- `Zigbee - Utility > Zigbee Device Config` → **Set to Coordinator**
- `Zigbee 3.0 - Network Creator`
- `Zigbee 3.0 - Network Creator Security`
- `Services > CLI - Global Configuration`  
  → Set **Command Prompt name** to: `Light`

### 🔄 Step 3: Uninstall Unnecessary Components

Remove components not needed for Coordinator:

- `Network Steering`
- `Update TC Link Key`

### 🔌 Step 4: Configure ZCL Endpoint

Open **ZCL Configurator** and:

- Edit the existing endpoint
- Select: `HA On/Off Light (0x0100)`
- Ensure all required clusters and attributes are included

### 🧱 Step 5: Build and Flash

- Build the project
- Flash the generated `.s37` file to the board

> ⚠️ **Important:** Perform an *Erase* operation before flashing to avoid legacy network conflicts.

### 📟 Step 6: Verify Device

- In **Device Adapters**, select the board
- Open **Serial Console 1**
- Press `Enter` → You should see the Light device boot logs

---

## 🖲 2. Creating the Switch Device (Router)

### 🧪 Step 1: Use Zigbee - Minimal Example

This example is already configured as a Router.

### 🛠 Step 2: Modify CLI Configuration

- `Services > CLI - Global Configuration`  
  → Set **Command Prompt name** to: `Switch`

### 🔌 Step 3: Configure ZCL Endpoint

Open **ZCL Configurator** and:

- Edit the endpoint
- Select: `HA On/Off Switch (0x0000)`
- Confirm component installation

### 🧱 Step 4: Build and Flash

- Build the project
- Flash the `.s37` binary to the second board

---

## 🧩 3. Verify Install Code (Security Step)

### 🔍 Step 1: Check Existing Install Code

Run the following command with only the **Switch device** connected:

```bash
C:\SiliconLabs\SimplicityStudio\v5\developer\adapter_packs\commander\commander.exe tokendump --tokengroup znet
