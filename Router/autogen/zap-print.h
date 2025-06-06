// This file is generated by ZCL Advanced Platform generator. Please don't edit manually.

// Enclosing macro to prevent multiple inclusion
#ifndef SILABS_PRINT_CLUSTER
#define SILABS_PRINT_CLUSTER


// This is the mapping of IDs to cluster names assuming a format according
// to the "EmberAfClusterName" defined in the ZCL header.
// The names of clusters that are not present, are removed.

  #define SILABS_PRINTCLUSTER_BASIC_CLUSTER { ZCL_BASIC_CLUSTER_ID, 0x0000, "Basic" },
  #define SILABS_PRINTCLUSTER_IDENTIFY_CLUSTER { ZCL_IDENTIFY_CLUSTER_ID, 0x0000, "Identify" },
  #define SILABS_PRINTCLUSTER_ON_OFF_CLUSTER { ZCL_ON_OFF_CLUSTER_ID, 0x0000, "On/off" },
  #define SILABS_PRINTCLUSTER_TEMP_MEASUREMENT_CLUSTER { ZCL_TEMP_MEASUREMENT_CLUSTER_ID, 0x0000, "Temperature Measurement" },

#define CLUSTER_IDS_TO_NAMES \
  SILABS_PRINTCLUSTER_BASIC_CLUSTER \
  SILABS_PRINTCLUSTER_IDENTIFY_CLUSTER \
  SILABS_PRINTCLUSTER_ON_OFF_CLUSTER \
  SILABS_PRINTCLUSTER_TEMP_MEASUREMENT_CLUSTER \

#endif // SILABS_PRINT_CLUSTER
