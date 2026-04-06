#include "app/framework/include/af.h"
#include "stack/include/network-formation.h"
#include "sl_simple_led_instances.h"
#include "sl_simple_led.h"

#define LED_ON   0
#define LED_OFF  1

static sl_zigbee_event_t form_network_event;
static void form_network_event_handler(sl_zigbee_event_t *event);
static void permit_join(void);


static EmberAfAttributeMetadata coord_attribs[] = {
  { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, ATTRIBUTE_MASK_SINGLETON, { (uint8_t *)3 } }
};

static EmberAfCluster coord_clusters[] = {
  { 0x0006, coord_attribs, 1, 2, CLUSTER_MASK_SERVER, NULL }  /* On/Off server */
};

static EmberAfEndpointType coord_ep_type = {
  coord_clusters, 1, 0
};

static void configure_endpoint(void)
{
  sli_zigbee_af_endpoints[0].endpoint      = 1;
  sli_zigbee_af_endpoints[0].profileId     = 0x0104;          /* Home Automation */
  sli_zigbee_af_endpoints[0].deviceId      = 0x0100;          /* On/Off Light    */
  sli_zigbee_af_endpoints[0].deviceVersion = 0;
  sli_zigbee_af_endpoints[0].endpointType  = &coord_ep_type;
  sli_zigbee_af_endpoints[0].networkIndex  = 0;
  sli_zigbee_af_endpoints[0].bitmask       = EMBER_AF_ENDPOINT_ENABLED;
}


void emberAfRadioNeedsCalibratingCallback(void)
{
  sl_mac_calibrate_current_channel();
}

void emberAfMainInitCallback(void)
{
  emberAfCorePrintln("Coordinator MainInit");
  configure_endpoint();
  sl_zigbee_event_init(&form_network_event, form_network_event_handler);
  sl_zigbee_event_set_active(&form_network_event);
}

static void form_network_event_handler(sl_zigbee_event_t *event)
{
  sl_zigbee_event_set_inactive(event);

  EmberNetworkStatus state = emberNetworkState();
  emberAfCorePrintln("Network state: 0x%02X", state);

  if (state == EMBER_JOINED_NETWORK) {
    emberAfCorePrintln("Already on network, leaving");
    emberLeaveNetwork();
    return;
  }

  if (state == EMBER_NO_NETWORK) {
    EmberInitialSecurityState securityState;
    memset(&securityState, 0, sizeof(EmberInitialSecurityState));
    securityState.bitmask = EMBER_NO_SECURITY;
    emberSetInitialSecurityState(&securityState);

    EmberNetworkParameters parameters;
    memset(&parameters, 0, sizeof(EmberNetworkParameters));
    parameters.panId        = 0x1234;
    parameters.radioChannel = 12;
    parameters.radioTxPower = 0;

    EmberStatus status = emberFormNetwork(&parameters);
    emberAfCorePrintln("Form Network status: 0x%02X", status);
  }
}

static void permit_join(void)
{
  EmberStatus status = emberPermitJoining(0xFF);
  emberAfCorePrintln("Permit Joining status: 0x%02X", status);
}

void emberAfStackStatusCallback(EmberStatus status)
{
  if (status == EMBER_NETWORK_UP) {
    emberAfCorePrintln("Network UP  NodeID=0x%04X  PAN=0x%04X  CH=%d",
                       emberGetNodeId(),
                       emberGetPanId(),
                       emberGetRadioChannel());
    permit_join();
  } else if (status == EMBER_NETWORK_DOWN) {
    emberAfCorePrintln("Network Down, reforming");
    sl_zigbee_event_set_active(&form_network_event);
  } else {
    emberAfCorePrintln("Stack status: 0x%02X", status);
  }
}

void emberAfTrustCenterJoinCallback(EmberNodeId       newNodeId,
                                    EmberEUI64        newNodeEui64,
                                    EmberNodeId       parentOfNewNode,
                                    EmberDeviceUpdate status,
                                    EmberJoinDecision decision)
{
  emberAfCorePrintln("TC Join: node=0x%04X  status=0x%02X  decision=0x%02X",
                     newNodeId, status, decision);
}

void emberAfChildJoinCallback(uint8_t       index,
                              bool          joining,
                              EmberNodeId   childId,
                              EmberEUI64    childEui64,
                              EmberNodeType childType)
{
  if (joining) {
    emberAfCorePrintln("Child Joined  NodeID=0x%04X  index=%d", childId, index);
  } else {
    emberAfCorePrintln("Child Left    NodeID=0x%04X  index=%d", childId, index);
  }
}

void emberAfIncomingMessageCallback(EmberIncomingMessageType type,
                                    EmberApsFrame           *apsFrame,
                                    EmberMessageBuffer       message)
{
  uint8_t received = emberGetLinkedBuffersByte(message, 0);

  emberAfCorePrintln("Message from 0x%04X  endpoint=%d  value=%d",
                     emberGetSender(),
                     apsFrame->destinationEndpoint,
                     received);

  if (received == LED_ON) {
    emberAfCorePrintln("LED On");
    sl_led_turn_on(&sl_led_led0);
  } else if (received == LED_OFF) {
    emberAfCorePrintln("LED Off");
    sl_led_turn_off(&sl_led_led0);
  }
}

