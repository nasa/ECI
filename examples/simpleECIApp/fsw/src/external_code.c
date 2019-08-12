/* This file contains an example of external code which is integrated 
 * into a CFS app via the ECI. The code in this file need not be modified
 * for integration into ECI.
 */
 
#include "external_code.h"

/* Declare globals */
/* Flags */
bool dataUpdated;
bool isOverThresh;

/* Tables */
convParamStruct_t       convParams;
checkParamStruct_t      checkParams;
stateStruct_t           stateData;

/* I/O */
inputStruct_t           inputData;
inputCmdPkt_t           input_CmdPkt;
outputStruct_t          outputData;

/* Functions */
/* setup() does some things at code startup (like allocating memory, 
 * initalizing values, etc.
 */
void setup(void){
    dataUpdated = false;
    isOverThresh = false;
}

/* term() does some things at code ending (like freeing memory, etc.)
 */
void teardown(void){
    
}

/* run() does the all of the processing 
 */
void run(void){
    
    /* Only process the input data when we're commanded to */
    if(input_CmdPkt.processData){
        
        /* Convert the input data to produce the output */
        outputData.converted_Value = convParams.scaleFactor * inputData.sensorMeas + convParams.biasFactor;
        /* Produce */
        dataUpdated = true;        
    }
    /* If data has been update, do some checks on it*/
    if(dataUpdated){
        /* Compare the value to a table-defined threshold */
        isOverThresh = outputData.converted_Value > checkParams.checkThresh;
        
        /* Record if its out-of-bounds */
        if (isOverThresh){
            stateData.numOverThresh++;
        }
        outputData.overThresh = isOverThresh;
    }
    
    /* Keep track of how many times the app has run */
    stateData.cycleCounter++;
}

/* Normal code would have a main function which is the entry point 
 * of the program and orchestrates its execution flow. The ECI does 
 * not use this main function, but we provide an example main here 
 * to demonstrate that this code runs standalone.
 */
int main(void){
        
    setup();

    /* TODO: read parameters from file as better demonstration of more
    realistic code setup */
    /* TODO: read inputs from somewhere */

    for (int i = 0; i < 100; i++){
        run();
    }

    /* TODO: write outputs to somewhere */

    teardown();
}