/**
 * Integer Lab
 * Starter Code and Sample Solution (c)2018-2019 Christopher A. Bohn (bohn-at-unl-dot-edu), University of Nebraska-Lincoln
 *
 * In this lab, students are required to implement integer arithmetic for unsigned 16-bit integers and signed (twos complement)
 * 16-bit integers using only bitwise and (&), bitwise or (|), bitwise xor (^), bitwise complement (~), and bit shifts (<< and >>).
 *
 * The program takes one command-line input to indicate whether the arithmetic should be treated as signed or unsigned.
 * The program will then prompt the user for a simple two-operator expression, such as -0x3 or 7+2.  The operands may be
 * decimal or hexadecimal.  The program will then compute and output the result; if either operand was entered in hexadecimal
 * then the output will be in hexadecimal.  The program will continue to prompt the user for expressions until the input is
 * newline without other input.
 *
 * Several inputs can be run by creating a text file with expressions (one per line, and ending with a blank line) that is piped
 * to the program's StdIn.  Similarly, StdOut can be piped to a text file and compared to the expected output.  For example:
 *      ./integer < input.txt > output.txt
 *      diff output.txt oracle.txt
 */

#include <stdio.h>
#include <string.h>
#include "alu.h"

const int MAXIMUM_EXPRESSION_STRING = 17;   // room for two integers, negating first, an operator, space around operator, & NUL

typedef struct {
    uint16_t operand1;
    uint16_t operand2;
    char operator;
} arithmetic_expression;

bool answer_in_hex;

arithmetic_expression *parse_expression( char *raw_expression, arithmetic_expression *parsed_expression );
bool is_negative( const char *expression, char *working_string );
char *get_operator_position( char *working_string, char *operator_char );
int dec_or_hex_atoi( const char *operand_string );
void print_usage();
void print_result( arithmetic_expression *expression, uint16_t result, const char *control_string );

arithmetic_expression *parse_expression( char *raw_expression, arithmetic_expression *parsed_expression ) {
    int first_operand_negation_multiplier = 1;
    char *operator_char = NULL;
    char working_string[MAXIMUM_EXPRESSION_STRING];
    char operand1_string[MAXIMUM_EXPRESSION_STRING];
    char operand2_string[MAXIMUM_EXPRESSION_STRING];
    int max, min;

    int p1 = 0, p2 = 0;
    while (p2 < MAXIMUM_EXPRESSION_STRING) {
        if ((raw_expression[p2] != '\n') && (raw_expression[p2] != '\t') && (raw_expression[p2] != ' ')) {
            raw_expression[p1++] = raw_expression[p2];
        }
        p2++;
    }

    if (is_negative(raw_expression, working_string)) first_operand_negation_multiplier = -1;

    operator_char = get_operator_position(working_string, operator_char);
    parsed_expression->operator = *operator_char;

    *operator_char = '\0';
    strncpy(operand1_string, working_string, MAXIMUM_EXPRESSION_STRING);
    strncpy(operand2_string, operator_char + 1, MAXIMUM_EXPRESSION_STRING);
    int local_operand1 = first_operand_negation_multiplier * dec_or_hex_atoi(operand1_string);
    int local_operand2 = dec_or_hex_atoi(operand2_string);

    if (!is_signed && ((local_operand1 < 0) || (local_operand2 < 0)))
        printf("ERROR: CANNOT USE NEGATIVE OPERANDS FOR UNSIGNED ARITHMETIC.  Casting.\n");
    max = is_signed && !answer_in_hex ? SHRT_MAX : USHRT_MAX;
    min = is_signed && !answer_in_hex ? SHRT_MIN : INT_MIN;
    if ((local_operand1 > max) || (local_operand1 < min) || (local_operand2 > max) || (local_operand2 < min))
        printf("ERROR: OPERAND CANNOT FIT IN 16 BITS.  Truncating.\n");

    parsed_expression->operand1 = (uint16_t)local_operand1;
    parsed_expression->operand2 = (uint16_t)local_operand2;
    return parsed_expression;
}

/* Copies the contents of "expression" into "working_string" but without the leading '-' if it exists.
 * Returns TRUE if a leading '-' was found.
 */
bool is_negative( const char *expression, char *working_string ) {
    bool operand_is_negated;
    if (expression[0] == '-') {
        operand_is_negated = true;
        strncpy(working_string, expression + 1, MAXIMUM_EXPRESSION_STRING);
    } else {
        operand_is_negated = false;
        strncpy(working_string, expression, MAXIMUM_EXPRESSION_STRING);
    }
    return operand_is_negated;
}

/* Determines where in "working_string" the arithmetic operator is and places a pointer to that operator in "operator_char,"
 * returning the pointer as well.
 */
char *get_operator_position( char *working_string, char *operator_char ) {
    if ((operator_char = strchr(working_string, '+')));
    else if ((operator_char = strchr(working_string, '*')));
    else if ((operator_char = strchr(working_string, '/')));
    else if ((operator_char = strchr(working_string, '-')));    // needs to be last so that we can handle a negative second operand
    else {  // only one operand
        printf("ERROR: ONLY ONE OPERAND FOUND.  Recovering.\n");
        strcat(working_string, "+0");
        operator_char = strchr(working_string, '+');
    }
    return operator_char;
}

/* Determines whether the input is expressed in decimal or hexadecimal. If hexadecimal then the global variable answer_in_hex is
 * set; if decimal then answer_in_hex is unchanged (across multiple calls, the effect is that only one input is required to set
 * answer_in_hex). Returns the input's value.
 */
int dec_or_hex_atoi( const char *operand_string ) {
    int value;
    char *control_string;
    if (strncmp(operand_string, "0x", 2) != 0) {
        control_string = "%d";
    } else {
        control_string = "%x";
        operand_string = operand_string + 2;
        answer_in_hex = true;
    }
    sscanf(operand_string, control_string, &value);
    return value;
}

void print_usage() { printf("Usage: integerlab signed|unsigned [noprompt]\n"); }

void print_result( arithmetic_expression *expression, uint16_t result, const char *control_string ) {
    if (is_signed && !answer_in_hex) {
        printf(control_string, (short)expression->operand1, expression->operator, (short)expression->operand2, (short)result);
    } else {
        printf(control_string, (unsigned short)expression->operand1,
               expression->operator, (unsigned short)expression->operand2, result);
    }
}

int main( int argc, const char **argv ) {
    bool done;
    bool prompt = true;
    char raw_expression[MAXIMUM_EXPRESSION_STRING];
    char *control_string;
    addition_subtraction_result addition;
    addition_subtraction_result subtraction;
    multiplication_result multiplication;
    division_result division;

    if (argc < 2) {
        print_usage();
        done = true;
    } else if (!strncmp(argv[1], "signed", 7)) {
        is_signed = true;
        done = false;
    } else if (!strncmp(argv[1], "unsigned", 8)) {
        is_signed = false;
        done = false;
    } else {
        print_usage();
        done = true;
    }

    if (argc > 2) {
        if (!strncmp(argv[2], "noprompt", 9))
            prompt = false;
        else {
            print_usage();
            done = true;
        }
    }

    while (!done) {
        if (prompt) printf("Input a simple two-operator arithmetic expression: ");
        fgets(raw_expression, MAXIMUM_EXPRESSION_STRING, stdin);
        if (raw_expression[0] == '\n') {
            done = true;
        } else {
            answer_in_hex = false;
            arithmetic_expression *parsed_expression = malloc(sizeof(arithmetic_expression));
            parsed_expression = parse_expression(raw_expression, parsed_expression);
            if (answer_in_hex) control_string = "0x%x %c 0x%x = 0x%x\n";
            else if (is_signed) control_string = "%d %c %d = %d\n";
            else control_string = "%u %c %u = %u\n";

            switch (parsed_expression->operator) {
                case '+':
                    addition = add(parsed_expression->operand1, parsed_expression->operand2);
                    print_result(parsed_expression, addition.result, control_string);
                    if (addition.overflow) printf("Overflow!\n");
                    break;
                case '-':
                    subtraction = subtract(parsed_expression->operand1, parsed_expression->operand2);
                    print_result(parsed_expression, subtraction.result, control_string);
                    if (subtraction.overflow) printf("Overflow!\n");
                    break;
                case '*':
                    multiplication = multiply(parsed_expression->operand1, parsed_expression->operand2);
                    print_result(parsed_expression, multiplication.product, control_string);
                    if (multiplication.overflow) {
                        printf("Overflow!  Full answer: 0x%x\n", multiplication.full_product);
                    }
                    break;
                case '/':
                    division = divide(parsed_expression->operand1, parsed_expression->operand2);
                    if (division.division_by_zero) {
                        printf("Division by zero!\n");
                    } else {
                        print_result(parsed_expression, division.quotient, control_string);
                        if (division.remainder != 0) {
                            if (answer_in_hex) control_string = "Full answer: 0x%x, remainder 0x%x\n";
                            else if (is_signed) control_string = "Full answer: %d, remainder %d\n";
                            else control_string = "Full answer: %u, remainder %u\n";
                            printf(control_string, (short)division.quotient, division.remainder);
                        }
                    }
                    break;
                default:
                    printf("UNKNOWN OPERATOR: %c.  No arithmetic performed.\n", parsed_expression->operator);
            }
        }
    }
    return 0;
}
