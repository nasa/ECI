
/* Include gaurds */
#ifndef SGP4WRAPPER_H
#define SGP4WRAPPER_H

/* Depedencies */
#include "TLE.h"
#include "eci_app.h"
#include <stdbool.h>
#include <stdint.h>

/* Define types */
typedef struct  {
	uint8_t header[12]; /* Reserve space for packet header */
    double r[3];
    double v[3];
    double t;
    bool invalid;
} propState_t;

typedef struct  {
    char line1[70];
    char line2[70];
} tle_lines_t;

typedef struct  {
    bool failed;
    int expected;
    int computed;
} line_check_t;

/* Declare extern variables for ECI*/
extern propState_t propState;
extern TLE tle;
extern tle_lines_t *tle_lines;
extern bool checksum1Passed, checksum2Passed;

/* Function prototypes for ECI use */
void propagate(void);
int32_t validateTLE(void * Ptr);

#endif
