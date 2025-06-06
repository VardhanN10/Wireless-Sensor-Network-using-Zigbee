// This file is generated by ZCL Advanced Platform generator. Please don't edit manually.

// The following is brought in by the endianness component, which all
// applications must include
#include "sl_endianness.h"

#ifndef SILABS_AF_ENDPOINT_CONFIG
#define SILABS_AF_ENDPOINT_CONFIG 1

// Default values for the attributes longer than a pointer, in a form of a
// binary blob. All attribute values with size greater than 2 bytes. Excluding 0
// values and externally saved values. Separate block is generated for
// big-endian and little-endian cases.

#define GENERATED_DEFAULTS_COUNT (0)
#define GENERATED_DEFAULTS { }


// This is an array of EmberAfAttributeMinMaxValue structures.

#define GENERATED_MIN_MAX_DEFAULT_COUNT (0)
	#define GENERATED_MIN_MAX_DEFAULTS { }


#define GENERATED_ATTRIBUTE_COUNT (20)

// This is an array of EmberAfAttributeMetadata structures.
#define GENERATED_ATTRIBUTES { \
  { 0x0000, ZCL_INT8U_ATTRIBUTE_TYPE, 1, (ATTRIBUTE_MASK_SINGLETON), { (uint8_t*)8  } }, /* 0 Cluster: Basic, Attribute: ZCL version, Side: server*/ \
		  { 0x0007, ZCL_ENUM8_ATTRIBUTE_TYPE, 1, (ATTRIBUTE_MASK_SINGLETON), { (uint8_t*)0  } }, /* 1 Cluster: Basic, Attribute: power source, Side: server*/ \
		  { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (ATTRIBUTE_MASK_SINGLETON), { (uint8_t*)3  } }, /* 2 Cluster: Basic, Attribute: cluster revision, Side: server*/ \
		  { 0x0000, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (ATTRIBUTE_MASK_WRITABLE), { (uint8_t*)0  } }, /* 3 Cluster: Identify, Attribute: identify time, Side: server*/ \
		  { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)2  } }, /* 4 Cluster: Identify, Attribute: cluster revision, Side: server*/ \
		  { 0x0000, ZCL_BITMAP8_ATTRIBUTE_TYPE, 1, (0x00), { (uint8_t*)0  } }, /* 5 Cluster: Groups, Attribute: name support, Side: server*/ \
		  { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)3  } }, /* 6 Cluster: Groups, Attribute: cluster revision, Side: server*/ \
		  { 0x0000, ZCL_INT8U_ATTRIBUTE_TYPE, 1, (0x00), { (uint8_t*)0  } }, /* 7 Cluster: Scenes, Attribute: scene count, Side: server*/ \
		  { 0x0001, ZCL_INT8U_ATTRIBUTE_TYPE, 1, (0x00), { (uint8_t*)0  } }, /* 8 Cluster: Scenes, Attribute: current scene, Side: server*/ \
		  { 0x0002, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)0  } }, /* 9 Cluster: Scenes, Attribute: current group, Side: server*/ \
		  { 0x0003, ZCL_BOOLEAN_ATTRIBUTE_TYPE, 1, (0x00), { (uint8_t*)0  } }, /* 10 Cluster: Scenes, Attribute: scene valid, Side: server*/ \
		  { 0x0004, ZCL_BITMAP8_ATTRIBUTE_TYPE, 1, (0x00), { (uint8_t*)0  } }, /* 11 Cluster: Scenes, Attribute: name support, Side: server*/ \
		  { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)3  } }, /* 12 Cluster: Scenes, Attribute: cluster revision, Side: server*/ \
		  { 0x0000, ZCL_BOOLEAN_ATTRIBUTE_TYPE, 1, (0x00), { (uint8_t*)0  } }, /* 13 Cluster: On/off, Attribute: on/off, Side: server*/ \
		  { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)2  } }, /* 14 Cluster: On/off, Attribute: cluster revision, Side: server*/ \
		  { 0x0000, ZCL_INT16S_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)32768  } }, /* 15 Cluster: Temperature Measurement, Attribute: measured value, Side: server*/ \
		  { 0x0001, ZCL_INT16S_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)32768  } }, /* 16 Cluster: Temperature Measurement, Attribute: min measured value, Side: server*/ \
		  { 0x0002, ZCL_INT16S_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)32768  } }, /* 17 Cluster: Temperature Measurement, Attribute: max measured value, Side: server*/ \
		  { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)3  } }, /* 18 Cluster: Temperature Measurement, Attribute: cluster revision, Side: server*/ \
		  { 0xFFFE, ZCL_ENUM8_ATTRIBUTE_TYPE, 1, (0x00), { (uint8_t*)0  } } /* 19 Cluster: Temperature Measurement, Attribute: reporting status, Side: server*/ \
		 }

 

#define GENERATED_CLUSTER_COUNT (6)
	
// This is an array of EmberAfCluster structures.
#define GENERATED_CLUSTERS { \
  { 0x0000, (EmberAfAttributeMetadata*)&(generatedAttributes[0]), 3, 0, CLUSTER_MASK_SERVER, NULL }, /* 0, Endpoint Id: 1, Cluster: Basic, Side: server*/ \
	  { 0x0003, (EmberAfAttributeMetadata*)&(generatedAttributes[3]), 2, 4, CLUSTER_MASK_SERVER, NULL }, /* 1, Endpoint Id: 1, Cluster: Identify, Side: server*/ \
	  { 0x0004, (EmberAfAttributeMetadata*)&(generatedAttributes[5]), 2, 3, CLUSTER_MASK_SERVER, NULL }, /* 2, Endpoint Id: 1, Cluster: Groups, Side: server*/ \
	  { 0x0005, (EmberAfAttributeMetadata*)&(generatedAttributes[7]), 6, 8, CLUSTER_MASK_SERVER, NULL }, /* 3, Endpoint Id: 1, Cluster: Scenes, Side: server*/ \
	  { 0x0006, (EmberAfAttributeMetadata*)&(generatedAttributes[13]), 2, 3, CLUSTER_MASK_SERVER, NULL }, /* 4, Endpoint Id: 1, Cluster: On/off, Side: server*/ \
	  { 0x0402, (EmberAfAttributeMetadata*)&(generatedAttributes[15]), 5, 9, CLUSTER_MASK_SERVER, NULL } /* 5, Endpoint Id: 1, Cluster: Temperature Measurement, Side: server*/ \
	 }

 
#define GENERATED_ENDPOINT_TYPE_COUNT (1)

// This is an array of EmberAfEndpointType structures.
#define GENERATED_ENDPOINT_TYPES { \
  { ((EmberAfCluster*)&(generatedClusters[0])), 6, 27 }, \
	 } 
// Largest attribute size is needed for various buffers
#define ATTRIBUTE_LARGEST (2)

// Total size of singleton attributes
#define ATTRIBUTE_SINGLETONS_SIZE (4)

// Total size of attribute storage
#define ATTRIBUTE_MAX_SIZE (27)

// Number of fixed endpoints	
#define FIXED_ENDPOINT_COUNT (1)

// Array of endpoints that are supported, the data inside the array is the
// endpoint number.
#define FIXED_ENDPOINT_ARRAY { \
  1 \
}

// Array of profile ids
#define FIXED_PROFILE_IDS { \
  260 \
}

// Array of device ids
#define FIXED_DEVICE_IDS { \
  256 \
}

// Array of device versions
#define FIXED_DEVICE_VERSIONS { \
  1 \
}

// Array of endpoint types supported on each endpoint
#define FIXED_ENDPOINT_TYPES { \
  0 \
}

// Array of networks supported on each endpoint
#define FIXED_NETWORKS { \
  0 \
}

// Array of EmberAfCommandMetadata structs.
#define ZAP_COMMAND_MASK(mask) COMMAND_MASK_ ## mask
#define EMBER_AF_GENERATED_COMMAND_COUNT  (29)
#define GENERATED_COMMANDS { \
  { 0x0003, 0x00, COMMAND_MASK_INCOMING_SERVER }, /* 0, Cluster: Identify, Command: Identify*/ \
	  { 0x0003, 0x00, COMMAND_MASK_OUTGOING_SERVER }, /* 1, Cluster: Identify, Command: IdentifyQueryResponse*/ \
	  { 0x0003, 0x01, COMMAND_MASK_INCOMING_SERVER }, /* 2, Cluster: Identify, Command: IdentifyQuery*/ \
	  { 0x0004, 0x00, COMMAND_MASK_INCOMING_SERVER }, /* 3, Cluster: Groups, Command: AddGroup*/ \
	  { 0x0004, 0x00, COMMAND_MASK_OUTGOING_SERVER }, /* 4, Cluster: Groups, Command: AddGroupResponse*/ \
	  { 0x0004, 0x01, COMMAND_MASK_INCOMING_SERVER }, /* 5, Cluster: Groups, Command: ViewGroup*/ \
	  { 0x0004, 0x01, COMMAND_MASK_OUTGOING_SERVER }, /* 6, Cluster: Groups, Command: ViewGroupResponse*/ \
	  { 0x0004, 0x02, COMMAND_MASK_INCOMING_SERVER }, /* 7, Cluster: Groups, Command: GetGroupMembership*/ \
	  { 0x0004, 0x02, COMMAND_MASK_OUTGOING_SERVER }, /* 8, Cluster: Groups, Command: GetGroupMembershipResponse*/ \
	  { 0x0004, 0x03, COMMAND_MASK_INCOMING_SERVER }, /* 9, Cluster: Groups, Command: RemoveGroup*/ \
	  { 0x0004, 0x03, COMMAND_MASK_OUTGOING_SERVER }, /* 10, Cluster: Groups, Command: RemoveGroupResponse*/ \
	  { 0x0004, 0x04, COMMAND_MASK_INCOMING_SERVER }, /* 11, Cluster: Groups, Command: RemoveAllGroups*/ \
	  { 0x0004, 0x05, COMMAND_MASK_INCOMING_SERVER }, /* 12, Cluster: Groups, Command: AddGroupIfIdentifying*/ \
	  { 0x0005, 0x00, COMMAND_MASK_INCOMING_SERVER }, /* 13, Cluster: Scenes, Command: AddScene*/ \
	  { 0x0005, 0x00, COMMAND_MASK_OUTGOING_SERVER }, /* 14, Cluster: Scenes, Command: AddSceneResponse*/ \
	  { 0x0005, 0x01, COMMAND_MASK_INCOMING_SERVER }, /* 15, Cluster: Scenes, Command: ViewScene*/ \
	  { 0x0005, 0x01, COMMAND_MASK_OUTGOING_SERVER }, /* 16, Cluster: Scenes, Command: ViewSceneResponse*/ \
	  { 0x0005, 0x02, COMMAND_MASK_INCOMING_SERVER }, /* 17, Cluster: Scenes, Command: RemoveScene*/ \
	  { 0x0005, 0x02, COMMAND_MASK_OUTGOING_SERVER }, /* 18, Cluster: Scenes, Command: RemoveSceneResponse*/ \
	  { 0x0005, 0x03, COMMAND_MASK_INCOMING_SERVER }, /* 19, Cluster: Scenes, Command: RemoveAllScenes*/ \
	  { 0x0005, 0x03, COMMAND_MASK_OUTGOING_SERVER }, /* 20, Cluster: Scenes, Command: RemoveAllScenesResponse*/ \
	  { 0x0005, 0x04, COMMAND_MASK_INCOMING_SERVER }, /* 21, Cluster: Scenes, Command: StoreScene*/ \
	  { 0x0005, 0x04, COMMAND_MASK_OUTGOING_SERVER }, /* 22, Cluster: Scenes, Command: StoreSceneResponse*/ \
	  { 0x0005, 0x05, COMMAND_MASK_INCOMING_SERVER }, /* 23, Cluster: Scenes, Command: RecallScene*/ \
	  { 0x0005, 0x06, COMMAND_MASK_INCOMING_SERVER }, /* 24, Cluster: Scenes, Command: GetSceneMembership*/ \
	  { 0x0005, 0x06, COMMAND_MASK_OUTGOING_SERVER }, /* 25, Cluster: Scenes, Command: GetSceneMembershipResponse*/ \
	  { 0x0006, 0x00, COMMAND_MASK_INCOMING_SERVER }, /* 26, Cluster: On/off, Command: Off*/ \
	  { 0x0006, 0x01, COMMAND_MASK_INCOMING_SERVER }, /* 27, Cluster: On/off, Command: On*/ \
	  { 0x0006, 0x02, COMMAND_MASK_INCOMING_SERVER }, /* 28, Cluster: On/off, Command: Toggle*/ \
	 } 
// Array of EmberAfManufacturerCodeEntry structures for commands.
#define GENERATED_COMMAND_MANUFACTURER_CODE_COUNT (0)
#define GENERATED_COMMAND_MANUFACTURER_CODES { \
  { 0x00, 0x00 }  \
																													 } 
// This is an array of EmberAfManufacturerCodeEntry structures for clusters.
#define GENERATED_CLUSTER_MANUFACTURER_CODE_COUNT (0)
#define GENERATED_CLUSTER_MANUFACTURER_CODES { \
  { 0x00, 0x00 }  \
						 } 
// This is an array of EmberAfManufacturerCodeEntry structures for attributes.
#define GENERATED_ATTRIBUTE_MANUFACTURER_CODE_COUNT (0)
#define GENERATED_ATTRIBUTE_MANUFACTURER_CODES { \
  { 0x00, 0x00 } \
																				 } 
// Array of EmberAfPluginReportingEntry structures.
#define EMBER_AF_GENERATED_REPORTING_CONFIG_DEFAULTS_TABLE_SIZE (2)
#define EMBER_AF_GENERATED_REPORTING_CONFIG_DEFAULTS { \
  { EMBER_ZCL_REPORTING_DIRECTION_REPORTED, 0x0001, 0x0006, 0x0000, CLUSTER_MASK_SERVER, 0x0000, 1, 65534, 0 }, /* Endpoint Id: 1, Cluster: On/off, Attribute: on/off */ \
	  { EMBER_ZCL_REPORTING_DIRECTION_REPORTED, 0x0001, 0x0402, 0x0000, CLUSTER_MASK_SERVER, 0x0000, 1, 65534, 0 }, /* Endpoint Id: 1, Cluster: Temperature Measurement, Attribute: measured value */ \
	 } 

#define EMBER_AF_MANUFACTURER_CODE 0x1049
#define EMBER_AF_DEFAULT_RESPONSE_POLICY_ALWAYS

// All Enabled Clusters
#define ZCL_USING_BASIC_CLUSTER_SERVER
#define ZCL_USING_IDENTIFY_CLUSTER_SERVER
#define ZCL_USING_GROUPS_CLUSTER_SERVER
#define ZCL_USING_SCENES_CLUSTER_SERVER
#define ZCL_USING_ON_OFF_CLUSTER_SERVER
#define ZCL_USING_TEMP_MEASUREMENT_CLUSTER_SERVER

// Cluster Counts
#define EMBER_AF_BASIC_CLUSTER_SERVER_ENDPOINT_COUNT (1)
#define EMBER_AF_IDENTIFY_CLUSTER_SERVER_ENDPOINT_COUNT (1)
#define EMBER_AF_GROUPS_CLUSTER_SERVER_ENDPOINT_COUNT (1)
#define EMBER_AF_SCENES_CLUSTER_SERVER_ENDPOINT_COUNT (1)
#define EMBER_AF_ON_OFF_CLUSTER_SERVER_ENDPOINT_COUNT (1)
#define EMBER_AF_TEMP_MEASUREMENT_CLUSTER_SERVER_ENDPOINT_COUNT (1)

// All Enabled Cluster Attributes
#define ZCL_USING_BASIC_CLUSTER_VERSION_ATTRIBUTE
#define ZCL_USING_BASIC_CLUSTER_POWER_SOURCE_ATTRIBUTE
#define ZCL_USING_BASIC_CLUSTER_CLUSTER_REVISION_SERVER_ATTRIBUTE
#define ZCL_USING_IDENTIFY_CLUSTER_IDENTIFY_TIME_ATTRIBUTE
#define ZCL_USING_IDENTIFY_CLUSTER_CLUSTER_REVISION_SERVER_ATTRIBUTE
#define ZCL_USING_GROUPS_CLUSTER_GROUP_NAME_SUPPORT_ATTRIBUTE
#define ZCL_USING_GROUPS_CLUSTER_CLUSTER_REVISION_SERVER_ATTRIBUTE
#define ZCL_USING_SCENES_CLUSTER_SCENE_COUNT_ATTRIBUTE
#define ZCL_USING_SCENES_CLUSTER_CURRENT_SCENE_ATTRIBUTE
#define ZCL_USING_SCENES_CLUSTER_CURRENT_GROUP_ATTRIBUTE
#define ZCL_USING_SCENES_CLUSTER_SCENE_VALID_ATTRIBUTE
#define ZCL_USING_SCENES_CLUSTER_SCENE_NAME_SUPPORT_ATTRIBUTE
#define ZCL_USING_SCENES_CLUSTER_CLUSTER_REVISION_SERVER_ATTRIBUTE
#define ZCL_USING_ON_OFF_CLUSTER_ON_OFF_ATTRIBUTE
#define ZCL_USING_ON_OFF_CLUSTER_CLUSTER_REVISION_SERVER_ATTRIBUTE
#define ZCL_USING_TEMP_MEASUREMENT_CLUSTER_TEMP_MEASURED_VALUE_ATTRIBUTE
#define ZCL_USING_TEMP_MEASUREMENT_CLUSTER_TEMP_MIN_MEASURED_VALUE_ATTRIBUTE
#define ZCL_USING_TEMP_MEASUREMENT_CLUSTER_TEMP_MAX_MEASURED_VALUE_ATTRIBUTE
#define ZCL_USING_TEMP_MEASUREMENT_CLUSTER_CLUSTER_REVISION_SERVER_ATTRIBUTE
#define ZCL_USING_TEMP_MEASUREMENT_CLUSTER_REPORTING_STATUS_SERVER_ATTRIBUTE

#define EMBER_AF_SUPPORT_COMMAND_DISCOVERY


#endif // SILABS_AF_ENDPOINT_CONFIG