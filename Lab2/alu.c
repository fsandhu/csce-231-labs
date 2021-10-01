/**
 * Integer Lab
 * Starter Code and Sample Solution (c)2018-2019 Christopher A. Bohn (bohn-at-unl-dot-edu), University of Nebraska-Lincoln
 *
 * This is the only file students should edit.
 *
 * In this lab, students are required to implement integer arithmetic for unsigned 16-bit integers and signed (twos complement)
 * 16-bit integers using only bitwise and (&), bitwise or (|), bitwise xor (^), bitwise complement (~), and bit shifts (<< and >>).
 * DO NOT USE built-in addition (+), subtraction (-), multiplication (*), division (/), nor modulo (%).
 */

/**
 * STUDENT:  Fateh Karan Singh Sandhu
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "alu.h"

/* Adds the two arguments and stores the sum in the return structure's result field.  If the operation overflowed then the
 * overflow flag is set. */
addition_subtraction_result add( uint16_t augend, uint16_t addend ) {
    addition_subtraction_result addition;
    if (!is_signed) {
     addition.result = augend;
      while(addend != 0) {
         uint16_t carry = addition.result & addend;
            addition.result = addition.result ^ addend;
            addend = carry << 1;
     }
     //checking overflow in unsigned with method defined in lecture
     if (addition.result < augend || addition.result < addend) {
           addition.overflow = true;
     } else {
            addition.overflow = false;   
        }                               
        return addition;
    } else {
        addition.result = augend;
        while(addend != 0) {
         uint16_t carry = addition.result & addend;
            addition.result = addition.result ^ addend;
            addend = carry << 1;
        }

        //checking overflow in signed with method defined in lecture
        if(((0x8000 & addend) && (0x8000 & augend) && (!(0x8000 & addition.result))) ||
         (!(0x8000 & addend) && !(0x8000 & augend) && ((0x8000 & addition.result)))){
            addition.overflow = true;
        } else {
            addition.overflow = false;   
        }
        return addition;
    }
}

/* Adds two 32bit unsigned integers to help with multiplication where full_product can be upto 32 bits */
uint32_t add32bit( uint32_t augend, uint32_t addend ) {  
    uint32_t result;
     result = augend;
      while(addend != 0) {
         uint16_t carry = result & addend;
            result = result ^ addend;
            addend = carry << 1;
    }
    return result;
} 

/* Subtracts the second argument from the first, stores the difference in the return structure's result field.  If the
 * operation overflowed then the overflow flag is set. */
addition_subtraction_result subtract( uint16_t menuend, uint16_t subtrahend ) {

    uint16_t one = 1;
    subtrahend = add(~subtrahend, one).result; //convert subtrahend using two's compelement to use in addition method above
    addition_subtraction_result result = add(menuend, subtrahend); //use addition to calculate subtraction
    // override overflow detection for subtraction
    if (!is_signed) {
     if (result.result > menuend) {
            result.overflow = true; //if result is greater than menuend, we have overflow
        } else {
            result.overflow = false;
        }
    } else if (is_signed) {
        if(((int16_t)menuend > 0 && (int16_t)subtrahend > 0 && (int16_t)result.result < 0) || 
        ((int16_t)menuend < 0 && (int16_t)subtrahend < 0 && (int16_t)result.result > 0)) {
            result.overflow = true;
        } else {
            result.overflow = false;
        }
    }
    return result;
}

/* Multiplies the two arguments.  The function stores lowest 16 bits of the product in the return structure's product field and
 * the full 32-bit product in the full_product field.  If the product doesn't fit in the 16-bit product field then the overflow
 * flag is set. */
multiplication_result multiply( uint16_t multiplicand, uint16_t multiplier ) {
    multiplication_result multiplication;
    if (multiplier == 0) {
      multiplication.product = 0;
      multiplication.full_product = 0;
      multiplication.overflow = false;
      return multiplication;
    } else if (multiplier == 1) {
      multiplication.product = multiplicand;
      multiplication.full_product = multiplicand;
      multiplication.overflow = false;
      return multiplication;
    } else if ((multiplier & add(~multiplier, (uint16_t)1).result) == multiplier) {
        multiplication.full_product = (uint32_t)multiplicand;
        while (multiplier != 1) {
            //keep shifting until multiplier is a one
            multiplication.full_product = multiplication.full_product << 1;
            multiplier = multiplier >> 1; 
        }
        multiplication.product = (uint16_t) multiplication.full_product;
        if (multiplication.full_product > multiplication.product) {
            multiplication.overflow = true;
        } else {
            multiplication.overflow = false;
        }
        return multiplication;
    } else {
        multiplication.full_product = 0;
        while (multiplier != 0) {
            //check if last bit is a 1
            if (multiplier & 0x0001) {
                multiplication.full_product = add32bit(multiplication.full_product, multiplicand);
            }
            multiplier = multiplier >> 1; //right shift to get next 1
            multiplicand = multiplicand << 1; //left shift to multiply with a power of 2
        }
        multiplication.product = (uint16_t) multiplication.full_product; // truncate 2w bits to w bits
        //if actual product > 16bit product, overflow occurs
        if (multiplication.full_product > multiplication.product) {
            multiplication.overflow = true;
        } else {
            multiplication.overflow = false;
        }
        return multiplication;
    }
}

/* Divides the first argument by the second.  The function stores the quotient in the return structure's quotient field and the
 * the quotient in the remainder field.  If the divisor is zero then the quotient and remainder fields should
 * be ignored, and the division_by_zero field should be set. */
division_result divide( uint16_t dividend, uint16_t divisor ) {
    division_result division;
    uint16_t divisor_copy = divisor;
    uint16_t dividend_copy = dividend;
    if (divisor == 0) {
        division.division_by_zero = true;
        division.quotient = 0;
        division.remainder = 0;
        return division;
    } else if (divisor == 1) {
        division.division_by_zero = false;
        division.quotient = dividend;
        division.remainder = 0;
        return division;
    } else if (divisor == dividend) {
        division.division_by_zero = false;
        division.quotient = 1;
        division.remainder = 0;
        return division;
    } else if (((divisor & add(~divisor, (uint16_t)1).result) == divisor) && ((dividend & add(~dividend, (uint16_t)1).result) == dividend)){
        division.quotient = dividend;
        while(divisor != 0) {
            division.quotient = division.quotient >> 1;
            divisor = divisor >> 1;
        }
        division.quotient = division.quotient << 1;
        division.remainder = 0;
        division.division_by_zero = false;
        return division;
    } else if ((divisor & add(~divisor, (uint16_t)1).result) == divisor) {
        division.quotient = dividend;
        division.remainder = (uint16_t) 0;
        while(divisor != 1) {
            //keep shifting quotient until divisor is 1
            division.quotient = division.quotient >> 1;
            divisor = divisor >> 1;
        }
        //use above functions to calculate remainder if both not a power of 2
        division.remainder = subtract(dividend_copy,(multiply(division.quotient, divisor_copy).product)).result;
        division.division_by_zero = false;
        return division;
    } else {
        printf("invalid divisor\n");
    }
}
