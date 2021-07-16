/* Header providing definitions for unit testing code. The
 * prototypes in this header are for functions which are
 * considered internal functions and should not be used for
 * any other purpose save testing.
 */

#ifndef SGP4WRAPPER_TEST_H
#define SGP4WRAPPER_TEST_H

/* Include normal header to expose all "public" prototypes */
#include "sgp4Wrapper.h"

/* Add some prototypes for internal functions so they
 * can be tested */
bool validateTLEChecksum(char line[70]);
int  computeTLEChecksum(char line[70]);
int  digitToInt(char digit);

#endif
