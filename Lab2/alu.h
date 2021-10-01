/**
 * Integer Lab
 * Starter Code and Sample Solution (c)2018-2019 Christopher A. Bohn (bohn-at-unl-dot-edu), University of Nebraska-Lincoln
 *
 * This header provides the data structures to store the operands, operator, and result; and it provides the function prototypes.
 */
#ifndef ALU_H
#define ALU_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

bool is_signed;

typedef struct {
    uint16_t result;
    bool overflow;
} addition_subtraction_result;

typedef struct {
    uint16_t product;
    uint32_t full_product;
    bool overflow;
} multiplication_result;

typedef struct {
    unsigned short quotient;
    unsigned short remainder;
    bool division_by_zero;
} division_result;


/* Adds the two arguments and stores the sum in the return structure's result field.  If the operation overflowed then the
 * overflow flag is set. */
addition_subtraction_result add( uint16_t augend, uint16_t addend );

/* Subtracts the second argument from the first, stores the difference in the return structure's result field.  If the
 * operation overflowed then the overflow flag is set. */
addition_subtraction_result subtract( uint16_t menuend, uint16_t subtrahend );

/* Multiplies the two arguments.  The function stores lowest 16 bits of the product in the return structure's product field and
 * the full 32-bit product in the full_product field.  If the product doesn't fit in the 16-bit product field then the overflow
 * flag is set. */
multiplication_result multiply( uint16_t multiplicand, uint16_t multiplier );

/* Divides the first argument by the second.  The function stores the quotient in the return structure's quotient field and the
 * the quotient in the remainder field.  If the divisor is zero then the quotient and remainder fields should be ignored, and the
 * adivision_by_zero field should be set. */
division_result divide( uint16_t dividend, uint16_t divisor );


#endif //ALU_H
