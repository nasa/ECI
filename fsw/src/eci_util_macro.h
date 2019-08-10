/**
 * @file
 * @brief Defines utility macros which are used throughout the code.
 */
 
#ifndef ECI_UTIL_MACRO_H
#define ECI_UTIL_MACRO_H

/**@defgroup util_macros ECI Utility Macros
 * Various utility macros.
 * @{
 */

/**
 * Converts Define or Enum to string.
 *
 * @param x value to be stringified by the preprocessor
 * @returns string representation of x
 */
#define D2STR(x) #x

/**
 * Returns the number of elements in the array.
 * **This uses sizeof(ar[0]) to determine the size
 * which means the array must contain at least one
 * element (should be null)**
 *
 * @param ar array to calculate size of
 * @returns size of the array
 */
#define SIZEOF_ARRAY(ar) (sizeof(ar)/sizeof(ar[0]))

/**
 * Returns the number of bytes for the member of the structure.
 * @param memberOf struct which contains the member
 * @param member name of the member to calculate
 * @returns size of the struct member
 */
#define SIZEOF_MEMBER(memberOf, member) sizeof(((memberOf*)0)->member)

/**
 * Returns the number of elements of an array for a member of a structure.
 * @param memberOf struct which contains the member
 * @param member array to calculate the size of
 * @returns size of the struct array
 */
#define SIZEOF_ARRAY_MEMBER(memberOf, member) (sizeof(((memberOf*)0)->member)/sizeof(((memberOf*)0)->member[0]))

/**
 * Swaps the low and high bytes of a 16 bit value
 * @param x 16 bit value to swap
 * @returns a byte swapped x
 */
#define BSWAP_16(x)  (((uint16_t)(x) << 8) | \
                      ((uint16_t)(x) >> 8))

/**
 * Swaps the 2 lowest and 2 highest bytes of a 32 bit value
 * @param x 32 bit value to swap
 * @returns a byte swapped x
 */
#define BSWAP_32(x)  (((uint32_t)(x) << 24) | \
                     (((uint32_t)(x) <<  8) & 0xff0000) | \
                     (((uint32_t)(x) >>  8) & 0xff00) | \
                      ((uint32_t)(x) >> 24))
/**
 * Swap the 4 lowest and 4 highest bytes of a 64 bit value
 * @param x 64 bit value to swap
 * @returns a byte swapped x
 */
#define BSWAP_64(x)  (((uint64_t)(x) << 56) | \
                     (((uint64_t)(x) << 40) & 0xff000000000000ULL) | \
                     (((uint64_t)(x) << 24) & 0xff0000000000ULL) | \
                     (((uint64_t)(x) <<  8) & 0xff00000000ULL) | \
                     (((uint64_t)(x) >>  8) & 0xff000000ULL) | \
                     (((uint64_t)(x) >> 24) & 0xff0000ULL) | \
                     (((uint64_t)(x) >> 40) & 0xff00ULL) | \
                      ((uint64_t)(x) >> 56))

/*
 * Binary Constant Generator Macros
 *   By Tom Torfs - donated to the public domain
 *
 * All macro's evaluate to compile-time constants.
 */

/**
 * Turn a numeric literal into a hex constant. This avoids problems with
 * leading zeroes. 8-bit constants have a max value of 0x11111111, which always
 * fits in an unsigned long
 * @param n literal to be converted
 * @returns hex representation of n
 */
#define HEX__(n) 0x##n##LU

/**
 * 8-bit Conversion Function
 */
#define B8__(x)   ((x&0x0000000FLU)?1:0) \
                + ((x&0x000000F0LU)?2:0) \
                + ((x&0x00000F00LU)?4:0) \
                + ((x&0x0000F000LU)?8:0) \
                + ((x&0x000F0000LU)?16:0) \
                + ((x&0x00F00000LU)?32:0) \
                + ((x&0x0F000000LU)?64:0) \
                + ((x&0xF0000000LU)?128:0)

/** For up to 8-bit binary constants. */
#define B8(d) ((uint8_t)B8__(HEX__(d)))

/** For up to 16-bit binary constants, MSB first. */
#define B16(dmsb, dlsb) (((uint16_t)B8(dmsb)<< \
                       + B8(dlsb))

/** For up to 32-bit binary constants, MSB first. */
#define B32(dmsb, db2, db3, dlsb) (((uint32_t)B8(dmsb) << 24) \
                                 + ((uint32_t)B8(db2) << 16) \
                                 + ((uint32_t)B8(db3) << \
                                 + B8(dlsb))

/**@}*/
#endif /* ECI_UTIL_MACRO_H */
