#ifndef MODEL_H_
#define MODEL_H_

#include "iec61850_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sModelNode ModelNode;
typedef struct sDataAttribute DataAttribute;
typedef struct sDataObject DataObject;
typedef struct sLogicalNode LogicalNode;
typedef struct sLogicalDevice LogicalDevice;
typedef struct sIedModel IedModel;

typedef enum {
  IEC61850_BOOLEAN = 0,/* int */
  IEC61850_INT8 = 1,   /* int8_t */
  IEC61850_INT16 = 2,  /* int16_t */
  IEC61850_INT32 = 3,  /* int32_t */
  IEC61850_INT64 = 4,  /* int64_t */
  IEC61850_INT128 = 5, /* no native mapping! */
  IEC61850_INT8U = 6,  /* uint8_t */
  IEC61850_INT16U = 7, /* uint16_t */
  IEC61850_INT24U = 8, /* uint32_t */
  IEC61850_INT32U = 9, /* uint32_t */
  IEC61850_FLOAT32 = 10, /* float */
  IEC61850_FLOAT64 = 11, /* double */
  IEC61850_ENUMERATED = 12,
  IEC61850_OCTET_STRING_64 = 13,
  IEC61850_OCTET_STRING_6 = 14,
  IEC61850_OCTET_STRING_8 = 15,
  IEC61850_VISIBLE_STRING_32 = 16,
  IEC61850_VISIBLE_STRING_64 = 17,
  IEC61850_VISIBLE_STRING_65 = 18,
  IEC61850_VISIBLE_STRING_129 = 19,
  IEC61850_VISIBLE_STRING_255 = 20,
  IEC61850_UNICODE_STRING_255 = 21,
  IEC61850_TIMESTAMP = 22,
  IEC61850_QUALITY = 23,
  IEC61850_CHECK = 24,
  IEC61850_CODEDENUM = 25,
  IEC61850_GENERIC_BITSTRING = 26,
  IEC61850_CONSTRUCTED = 27,
  IEC61850_ENTRY_TIME = 28,
  IEC61850_PHYCOMADDR = 29,
  IEC61850_CURRENCY = 30,
  IEC61850_OPTFLDS = 31, /* bit-string(10) */
  IEC61850_TRGOPS = 32 /* bit-string(6) */
} DataAttributeType;

typedef enum {
    LogicalDeviceModelType,
    LogicalNodeModelType,
    DataObjectModelType,
    DataAttributeModelType
} ModelNodeType;

struct sIedModel {
    char* name;
    LogicalDevice* firstChild;
};

struct sLogicalDevice {
    ModelNodeType modelType;
    char* name;
    ModelNode* parent;
    ModelNode* sibling;
    ModelNode* firstChild;
};

struct sModelNode {
    ModelNodeType modelType;
    char* name;
    ModelNode* parent;
    ModelNode* sibling;
    ModelNode* firstChild;
};

struct sLogicalNode {
    ModelNodeType modelType;
    char* name;
    ModelNode* parent;
    ModelNode* sibling;
    ModelNode* firstChild;
};

struct sDataObject {
    ModelNodeType modelType;
    char* name;
    ModelNode* parent;
    ModelNode* sibling;
    ModelNode* firstChild;

    int elementCount; /* > 0 if this is an array */
};

struct sDataAttribute {
    ModelNodeType modelType;
    char* name;
    ModelNode* parent;
    ModelNode* sibling;
    ModelNode* firstChild;

    int elementCount; /* > 0 if this is an array */

    //FunctionalConstraint fc;
    DataAttributeType type;

    uint8_t triggerOptions; /* TRG_OPT_DATA_CHANGED | TRG_OPT_QUALITY_CHANGED | TRG_OPT_DATA_UPDATE */

    //MmsValue* mmsValue;

    uint32_t sAddr;
};



#ifdef __cplusplus
}
#endif


#endif /* MODEL_H_ */
