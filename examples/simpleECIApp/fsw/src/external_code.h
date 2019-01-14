
#ifndef EXTERNAL_CODE_H
#define EXTERNAL_CODE_H

#include <stdbool.h>
#include <stdint.h>

/* Parameter tables */
typedef struct {
    double scaleFactor;
    double biasFactor;
} convParamStruct_t;

typedef struct {
    double checkThresh;
} checkParamStruct_t;

/* State table */
typedef struct {
    uint32_t cycleCounter;
    uint32_t numOverThresh;
} stateStruct_t;

/* Packets */
typedef struct{
    uint8_t sensorMeas;
} inputStruct_t;

typedef struct{
    bool processData;
} inputCmdPkt_t;

typedef struct{
    uint8_t converted_Value;
    bool overThresh;
} outputStruct_t;

/* Function Prototypes */
void setup(void);
void teardown(void);
void run(void);

#endif