/*
 * Simulated Single Board Computer, (c) 2020 Christopher A. Bohn
 */

#include "ssbc.h"

pthread_mutex_t *mutex;
uint32_t *display_controls;
uint8_t *toggle_control;
uint16_t *keypad_control;
int running = 1;

int main() {
    ssbc_launch();
    uint8_t letters[] = {0x78, 0x4F, 0x76, 0x6F}; // {f,d,s,a}
    mutex = ssbc_get_mutex();
    display_controls = ssbc_get_7_segment_address();
    toggle_control = ssbc_get_toggle_address();
    keypad_control = ssbc_get_keypad_address();
    keypad_control += 1;
    uint8_t lastNumber = 0x0F;
    while (running) {
        pthread_mutex_lock(mutex);
        uint8_t toggle_positions = *toggle_control;
        uint8_t number = *keypad_control & 0x0Fu;
        pthread_mutex_unlock(mutex);
        for (unsigned int i = 0; i < 4; i++) {
            if (toggle_positions & 0x1u << i) {
                pthread_mutex_lock(mutex);
                uint32_t clear_position = ~(0xFFu << (8 * i));
                uint32_t character_in_position = (uint32_t)(letters[i]) << (8 * i);
                *display_controls = (*display_controls & clear_position) | character_in_position;
                pthread_mutex_unlock(mutex);
            } else {
                pthread_mutex_lock(mutex);
                uint32_t clear_position = ~(0xFFu << (8 * i));
                *display_controls = *display_controls & clear_position;
                pthread_mutex_unlock(mutex);
            }
        }
        if (number != lastNumber) {
            pthread_mutex_lock(mutex);
            ssbc_print("%d\n", number);
            pthread_mutex_unlock(mutex);
        }
        lastNumber = number;
        if (number == 0) {
            running = 0;
        }
    }
    ssbc_terminate();
    return 0;
}
