/*
 * Simulated Single Board Computer, (c) 2020 Christopher A. Bohn
 *
 * Student: Fateh Sandhu
 * Evaluate for bonus credit: yes
 */

#include "ssbc.h"
#include "stdlib.h"

/**
 * This function converts a binary coded decimal into a bit vector that lights ups a seven segment display
 */
uint8_t bcdToSegment(uint16_t number);

/**
 * This function clears a segment display
 */
void clearSegment(uint8_t *segementAddress);

/**
 * This functions gives value of dirty bit (0/1)
 */
uint8_t getDirtyBit(uint16_t *address);

/**
 * This function displays the given 4 digit number to the segment display
 */
void displayToSegment (uint16_t number, uint8_t *display_controls);

int running = 1;
pthread_mutex_t *mutex;
uint8_t *display_controls;
uint8_t *toggle_control;
uint16_t *dirty_bit_address;
uint16_t *keypad_control;
uint16_t globalOperand1 = 0;
uint16_t globalOperand2 = 0;

int main() {

    ssbc_launch();
    mutex = ssbc_get_mutex();
    display_controls = ssbc_get_7_segment_address();
    toggle_control = ssbc_get_toggle_address();
    dirty_bit_address = ssbc_get_keypad_address();
    keypad_control = dirty_bit_address+1;
    uint8_t *segment0 = display_controls;
    uint8_t *segment1 = display_controls+1;
    uint8_t *segment2 = display_controls+2;
    uint8_t *segment3 = display_controls+3;
    uint8_t dirtyBit;
    uint16_t operand1 = 0;
    uint16_t operand2 = 0;
    uint16_t backupOperand1 = 0;
    uint16_t backupOperand2 = 0;
    int operand1_builder = 1;
    int operand2_builder = 1;
    uint16_t addition_result = 0;
    uint16_t subtraction_result = 0;


    while (running) {
        pthread_mutex_lock(mutex);
        uint8_t toggle_positions = *toggle_control;
        dirtyBit = getDirtyBit(dirty_bit_address);
        pthread_mutex_unlock(mutex);

        if (!toggle_positions) {
            pthread_mutex_lock(mutex);
            clearSegment(segment3);
            clearSegment(segment2);
            clearSegment(segment1);
            clearSegment(segment0);
            *dirty_bit_address = *dirty_bit_address &0x1u;
            operand1 = 0;
            operand2 = 0;
            operand1_builder = 1;
            operand2_builder = 1;
            backupOperand1 = globalOperand1;
            backupOperand2 = globalOperand2;
            pthread_mutex_unlock(mutex);
        } else if ((toggle_positions & 0x8u) == 0x8u) {
            if (dirtyBit) {
                pthread_mutex_lock(mutex);
                if (operand1_builder == 1) {
                    operand1 = *keypad_control;
                } else if (operand1_builder == 2) {
                    uint16_t temp = operand1;
                    operand1 = temp*10 + (*keypad_control);
                } else if (operand1_builder == 3) {
                    uint16_t temp = operand1;
                    operand1 = temp*10 + (*keypad_control);
                } else if (operand1_builder == 4) {
                    uint16_t temp = operand1;
                    operand1 = temp*10 + (*keypad_control);
                }
                if (operand1 != 0) {
                operand1_builder++;
                }

                *dirty_bit_address = *dirty_bit_address &0x1u;
                globalOperand1 = operand1;
                ssbc_print("operand1 = %d  ", operand1);
                ssbc_print("globalOperand1 = %d\n", globalOperand1);
                pthread_mutex_unlock(mutex);
            }
            if (operand1_builder > 5) {
                    *segment2 = 0x7Cu;
                    *segment1 = 0x48u;
                    *segment0 = 0x48u;
                    clearSegment(segment3);
                    globalOperand1 = backupOperand1;
                    ssbc_print("operand too large, setting to backup operand = %d\n", backupOperand1);
                } else {
                 displayToSegment(operand1, display_controls);
                }  
        } else if ((toggle_positions & 0x4u) == 0x4u) {
            if (dirtyBit) {
                pthread_mutex_lock(mutex);
                if (operand2_builder == 1) {
                    operand2 = *keypad_control;
                } else if (operand2_builder == 2) {
                    uint16_t temp = operand2;
                    operand2 = temp*10 + (*keypad_control);
                } else if (operand2_builder == 3) {
                    uint16_t temp = operand2;
                    operand2 = temp*10 + (*keypad_control);
                } else if (operand2_builder == 4) {
                    uint16_t temp = operand2;
                    operand2 = temp*10 + (*keypad_control);
                }
                if (operand2 != 0) {
                operand2_builder++;
                }
                *dirty_bit_address = *dirty_bit_address &0x1u;
                globalOperand2 = operand2;
                ssbc_print("operand2 = %d  ", operand2);
                ssbc_print("globalOperand2 = %d\n", globalOperand2);
                pthread_mutex_unlock(mutex);
            }

            if (operand2_builder > 5) {
                    *segment2 = 0x7Cu;
                    *segment1 = 0x48u;
                    *segment0 = 0x48u;
                    clearSegment(segment3);
                    globalOperand2 = backupOperand2;
                    ssbc_print("operand too large, setting to backup operand = %d\n", backupOperand2);
                } else {
                displayToSegment(operand2, display_controls);
            }
        } else if ((toggle_positions & 0x2u) == 0x2u) {
            pthread_mutex_lock(mutex);
            addition_result = globalOperand1+globalOperand2;
            if (addition_result <= 9999) {
                displayToSegment(addition_result, display_controls);
            } else {
                *segment2 = 0x7Cu;
                *segment1 = 0x48u;
                *segment0 = 0x48u;
                clearSegment(segment3);
            }
            pthread_mutex_unlock(mutex);
        } else if ((toggle_positions & 0x1u) == 0x1u) {
            pthread_mutex_lock(mutex);
            subtraction_result = globalOperand1-globalOperand2;
            int subtraction_Int = globalOperand1-globalOperand2;
            if (subtraction_Int >= -999 && subtraction_Int <= -1) {
                *segment3 = 0x40u;
                *segment2 = bcdToSegment(abs(((subtraction_Int%1000)-(subtraction_Int%100)))/100);
                *segment1 = bcdToSegment(abs(((subtraction_Int%100)-(subtraction_Int%10)))/10);
                *segment0 = bcdToSegment(abs(subtraction_Int%10));

                if (*segment2 == 0x3F) {
                    clearSegment(segment3);
                    *segment2 = 0x40u;
                }

                if (*segment1 == 0x3F && *segment2 == 0x3F) {
                    clearSegment(segment3);
                    clearSegment(segment2);
                    *segment1 = 0x40u;
                } else if (*segment1 == 0x3F && *segment2 == 0x40u) {
                    clearSegment(segment3);
                    clearSegment(segment2);
                    *segment1 = 0x40u;
                }

            } else if (subtraction_Int < -999) {
                *segment2 = 0x7Cu;
                *segment1 = 0x48u;
                *segment0 = 0x48u;
                clearSegment(segment3);
            } else {
                displayToSegment(subtraction_result, display_controls);
            }
            pthread_mutex_unlock(mutex);
        }
    }

    ssbc_terminate();
    return 0;
}

uint8_t bcdToSegment(uint16_t number) {  
    switch (number) {
        case 0:
            return 0x3F;
        case 1:
            return 0x03;
        case 2:
            return 0x6D;
        case 3:
            return 0x67;
        case 4:
            return 0x53;
        case 5:
            return 0x76;
        case 6:
            return 0x7E;
        case 7:
            return 0x23;
        case 8:
            return 0x7F;
        case 9:
            return 0x77;
    }
    return 0;
}

void clearSegment(uint8_t *segementAddress) {
    *segementAddress = 0;
}

uint8_t getDirtyBit(uint16_t *address) {
    return *address & 0x2u;
}

void displayToSegment (uint16_t number, uint8_t *display_controls) {
    uint8_t *segment0 = display_controls;
    uint8_t *segment1 = display_controls+1;
    uint8_t *segment2 = display_controls+2;
    uint8_t *segment3 = display_controls+3;

    *segment0 = bcdToSegment(number%10);

    if (number > 0x9u) {
        uint16_t temp = ((number%100)-(number%10))/10;
        *segment1 = bcdToSegment(temp);
    }

    if (number > 0x63u) {
        uint16_t temp = ((number%1000)-(number%100))/100;
        *segment2 = bcdToSegment(temp);
    }

    if (number > 0x3E7u) {
        uint16_t temp = (number-(number%1000))/1000;
        *segment3 = bcdToSegment(temp);
    }
}