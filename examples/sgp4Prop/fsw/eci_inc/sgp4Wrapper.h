
/* Include gaurds */
#ifndef SGP4WRAPPER_H
#define SGP4WRAPPER_H

/* Depedencies */
#include "TLE.h"
#include "eci_app.h"
#include <stdint.h>

/* Define types */
typedef struct  {
    double r[3];
    double v[3];
    double t;
} propState_t;

typedef struct  {
    char line1[70];
    char line2[70];
} tle_lines_t;

/* Declare extern variables for ECI*/
extern propState_t propState;
extern TLE tle;
extern tle_lines_t *tle_lines;
extern bool checksum1Passed, checksum2Passed;

/* Function prototypes for ECI use */
void propagate(void);
int32_t validateTLE(tle_lines_t * TblPtr);

#endif