
/* Include gaurds */
#ifndef SGP4WRAPPER_TEST_H
#define SGP4WRAPPER_TEST_H

#include "sgp4Wrapper.h"

/* Add some prototypes for internal functions so they
 * can be tested */
bool validateTLEChecksum(char line[70]);
int computeTLEChecksum(char line[70]);
int digitToInt(char digit);
    
#endif
