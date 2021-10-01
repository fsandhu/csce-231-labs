/*
 * Simulated Single Board Computer, (c) 2020 Christopher A. Bohn
 *
 * Student: Fateh Sandhu
 */

#include "ssbc.h"

/**
 * This function converts a binary coded decimal into a bit vector that lights ups a seven segment display
 */
uint8_t bcdToSegment(uint16_t number);

/**
 * This function clears a segment display
 */
void clearSegment(uint8_t *segementAddress);

int running = 1;
pthread_mutex_t *mutex;
uint8_t *display_controls;
uint8_t *toggle_control;
uint16_t *keypad_control;

int main() {
    
    ssbc_launch();
    mutex = ssbc_get_mutex();
    display_controls = ssbc_get_7_segment_address();
    toggle_control = ssbc_get_toggle_address();
    keypad_control = ssbc_get_keypad_address();
    keypad_control += 1;
    uint8_t *segment0 = display_controls;
    uint8_t *segment1 = display_controls+1;
    uint8_t *segment2 = display_controls+2;
    uint8_t *segment3 = display_controls+3;

    while (running) {
        pthread_mutex_lock(mutex);
        uint8_t toggle_positions = *toggle_control;
        pthread_mutex_unlock(mutex);

            if ((toggle_positions & 0x9u) == 0x9u) {
                pthread_mutex_lock(mutex);
                *segment0 = bcdToSegment(*keypad_control);
                *segment3 = bcdToSegment(2);
                *segment2 = bcdToSegment(3);
                *segment1 = bcdToSegment(1);
                pthread_mutex_unlock(mutex);
            } else if (toggle_positions & 0x1u) {
                pthread_mutex_lock(mutex);
                clearSegment(segment3);
                clearSegment(segment2);
                clearSegment(segment1);
                *segment0 = bcdToSegment(*keypad_control);
                pthread_mutex_unlock(mutex);
            } else if ((toggle_positions & 0x8u) == 0x8u) {
                pthread_mutex_lock(mutex);
                clearSegment(segment0);
                *segment3 = bcdToSegment(2);
                *segment2 = bcdToSegment(3);
                *segment1 = bcdToSegment(1);
                pthread_mutex_unlock(mutex);
            } else {
                pthread_mutex_lock(mutex);
                clearSegment(segment3);
                clearSegment(segment2);
                clearSegment(segment1);
                clearSegment(segment0);
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
