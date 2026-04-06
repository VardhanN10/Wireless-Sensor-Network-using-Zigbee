#include "app/framework/include/af.h"
#include "stack/include/network-formation.h"
#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"

#define TARGET_PAN_ID       0x1234
#define COORDINATOR_NODE_ID 0x0000
#define BUTTON0             0
#define BUTTON1             1
#define LED_ON              0
#define LED_OFF             1

volatile bool button0Pressed = false;
volatile bool button1Pressed = false;

static sl_zigbee_event_t scan_network_event;
static sl_zigbee_event_t send_message_event;

static void scan_network_event_handler(sl_zigbee_event_t *event);
static void send_message_event_handler(sl_zigbee_event_t *event);


static EmberAfAttributeMetadata router_attribs[] = {
  { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, ATTRIBUTE_MASK_SINGLETON, { (uint8_t *)3 } }
};

static EmberAfCluster router_clusters[] = {
  { 0x0006, router_attribs, 1, 2, CLUSTER_MASK_CLIENT, NULL }
};

static EmberAfEndpointType router_ep_type = {
  router_clusters, 1, 0
};

static void configure_endpoint(void)
{
  sli_zigbee_af_endpoints[0].endpoint      = 1;
  sli_zigbee_af_endpoints[0].profileId     = 0x0104;
  sli_zigbee_af_endpoints[0].deviceId      = 0x0007;
  sli_zigbee_af_endpoints[0].deviceVersion = 0;
  sli_zigbee_af_endpoints[0].endpointType  = &router_ep_type;
  sli_zigbee_af_endpoints[0].networkIndex  = 0;
  sli_zigbee_af_endpoints[0].bitmask       = EMBER_AF_ENDPOINT_ENABLED;
}


void emberAfRadioNeedsCalibratingCallback(void)
{
  sl_mac_calibrate_current_channel();
}

void sl_button_on_change(const sl_button_t *handle)
{
  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON0) == handle) {
    if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED) {
      button0Pressed = true;
    }
  }
  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON1) == handle) {
    if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED) {
      button1Pressed = true;
    }
  }
}


void emberAfMainTickCallback(void)
{
  if (button0Pressed || button1Pressed) {
    if (button0Pressed) {
      emberAfCorePrintln("Button 0 pressed");
    } else {
      emberAfCorePrintln("Button 1 pressed");
    }
    sl_zigbee_event_set_active(&send_message_event);
  }
}

void emberAfMainInitCallback(void)
{
  emberAfCorePrintln("Router MainInit");
  configure_endpoint();
  sl_zigbee_event_init(&scan_network_event, scan_network_event_handler);
  sl_zigbee_event_init(&send_message_event, send_message_event_handler);
  sl_zigbee_event_set_active(&scan_network_event);
}

static void scan_network_event_handler(sl_zigbee_event_t *event)
{
  sl_zigbee_event_set_inactive(event);

  if (emberNetworkState() == EMBER_JOINED_NETWORK) {
    emberAfCorePrintln("Already joined");
    return;
  }

  EmberInitialSecurityState securityState;
  memset(&securityState, 0, sizeof(EmberInitialSecurityState));
  securityState.bitmask = EMBER_NO_SECURITY;
  emberSetInitialSecurityState(&securityState);

  EmberStatus status = emberStartScan(EMBER_ACTIVE_SCAN,
                                      EMBER_ALL_802_15_4_CHANNELS_MASK,
                                      4);
  emberAfCorePrintln("emberStartScan: 0x%02X", status);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Scan failed, retrying in 5s");
    sl_zigbee_event_set_delay_ms(event, 5000);
  }
}

void emberAfNetworkFoundCallback(EmberZigbeeNetwork *networkFound)
{
  emberAfCorePrintln("Beacon: PAN=0x%04X  CH=%d  join=%d",
                     networkFound->panId,
                     networkFound->channel,
                     networkFound->allowingJoin);

  if (networkFound->panId == TARGET_PAN_ID && networkFound->allowingJoin) {
    emberAfCorePrintln("Target network found, joining");
    emberStopScan();

    EmberNetworkParameters parameters;
    memset(&parameters, 0, sizeof(EmberNetworkParameters));
    parameters.panId        = networkFound->panId;
    parameters.radioChannel = networkFound->channel;
    parameters.radioTxPower = 0;
    parameters.joinMethod   = EMBER_USE_MAC_ASSOCIATION;

    EmberStatus status = emberJoinNetwork(EMBER_ROUTER, &parameters);
    emberAfCorePrintln("emberJoinNetwork: 0x%02X", status);
  }
}

static void sendByte(uint8_t value)
{
  EmberApsFrame apsFrame;
  memset(&apsFrame, 0, sizeof(EmberApsFrame));
  apsFrame.profileId           = 0x0104;
  apsFrame.clusterId           = 0x0006;
  apsFrame.sourceEndpoint      = 1;
  apsFrame.destinationEndpoint = 1;
  apsFrame.options             = EMBER_APS_OPTION_NONE;

  EmberMessageBuffer message = emberFillLinkedBuffers(&value, 1);
  if (message == EMBER_NULL_MESSAGE_BUFFER) {
    emberAfCorePrintln("Buffer alloc failed");
    return;
  }

  EmberStatus status = emberSendUnicast(EMBER_OUTGOING_DIRECT,
                                        COORDINATOR_NODE_ID,
                                        &apsFrame,
                                        message);
  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Sent: %d", value);
  } else {
    emberAfCorePrintln("Send failed: 0x%02X", status);
    emberReleaseMessageBuffer(message);
  }
}

static void send_message_event_handler(sl_zigbee_event_t *event)
{
  sl_zigbee_event_set_inactive(event);

  if (emberNetworkState() != EMBER_JOINED_NETWORK) {
    emberAfCorePrintln("Not in a network");
    return;
  }

  if (button0Pressed) {
    emberAfCorePrintln("Button 0 -> LED ON");
    sendByte(LED_ON);
    button0Pressed = false;
  }

  if (button1Pressed) {
    emberAfCorePrintln("Button 1 -> LED OFF");
    sendByte(LED_OFF);
    button1Pressed = false;
  }
}

void emberAfStackStatusCallback(EmberStatus status)
{
  if (status == EMBER_NETWORK_UP) {
    emberAfCorePrintln("Joined!  NodeID=0x%04X  PAN=0x%04X  CH=%d",
                       emberGetNodeId(),
                       emberGetPanId(),
                       emberGetRadioChannel());
  } else if (status == EMBER_JOIN_FAILED) {
    emberAfCorePrintln("Join failed, rescanning in 5s");
    sl_zigbee_event_set_delay_ms(&scan_network_event, 5000);
  } else if (status == EMBER_NETWORK_DOWN) {
    emberAfCorePrintln("Network DOWN, rescanning");
    sl_zigbee_event_set_inactive(&send_message_event);
    sl_zigbee_event_set_delay_ms(&scan_network_event, 2000);
  } else {
    emberAfCorePrintln("Stack status: 0x%02X", status);
  }
}
