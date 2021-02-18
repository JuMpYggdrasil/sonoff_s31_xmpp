#ifndef IEC61850_COMMON_H_
#define IEC61850_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

//"ctlModel" attribute)
typedef enum {
    CONTROL_MODEL_STATUS_ONLY = 0,// No support for control functions. Control object only support status information.
    CONTROL_MODEL_DIRECT_NORMAL = 1,//Operate, TimeActivatedOperate (O),and Cancel (optional).
    CONTROL_MODEL_SBO_NORMAL = 2,//Select, Operate, TimeActivatedOperate (optional),and Cancel (optional).
    CONTROL_MODEL_DIRECT_ENHANCED = 3,//enhanced security includes the  service CommandTermination
    CONTROL_MODEL_SBO_ENHANCED = 4
} ControlModel;

typedef union {
    uint8_t val[8];
} Timestamp;

//Timestamp* Timestamp_create(void);
//
//Timestamp* Timestamp_createFromByteArray(uint8_t* byteArray);
//
//void Timestamp_destroy(Timestamp* self);
//
//void Timestamp_clearFlags(Timestamp* self);
//
//uint32_t Timestamp_getTimeInSeconds(Timestamp* self);
//
//uint64_t Timestamp_getTimeInMs(Timestamp* self);
//
//bool Timestamp_isLeapSecondKnown(Timestamp* self);
//
//void Timestamp_setLeapSecondKnown(Timestamp* self, bool value);
//
//bool Timestamp_hasClockFailure(Timestamp* self);
//
//void Timestamp_setClockFailure(Timestamp* self, bool value);
//
//bool Timestamp_isClockNotSynchronized(Timestamp* self);
//
//void Timestamp_setClockNotSynchronized(Timestamp* self, bool value);
//
//int Timestamp_getSubsecondPrecision(Timestamp* self);
//
///**
//    \brief Set the subsecond precision value of the time stamp
//
//    \param subsecondPrecision the number of significant bits of the fractionOfSecond part of the time stamp
//*/
//void Timestamp_setSubsecondPrecision(Timestamp* self, int subsecondPrecision);
//
//void Timestamp_setTimeInSeconds(Timestamp* self, uint32_t secondsSinceEpoch);
//
//void Timestamp_setTimeInMilliseconds(Timestamp* self, uint64_t millisSinceEpoch);
//
//void Timestamp_setByMmsUtcTime(Timestamp* self, MmsValue* mmsValue);
//
///**
//    \brief Set an MmsValue instance of type UTCTime to the timestamp value
//
//    \param self the Timestamp instance
//    \param mmsValue the mmsValue instance, if NULL a new instance will be created
//*/
//MmsValue* Timestamp_toMmsValue(Timestamp* self, MmsValue* mmsValue);


#ifdef __cplusplus
}
#endif

#endif /* IEC61850_COMMON_H_ */
