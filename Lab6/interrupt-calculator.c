/*
 * Simulated Single Board Computer, (c) 2020 Christopher A. Bohn
 *
 * Student: <place your name here>
 * Evaluate for bonus credit: <yes/no>
 */

#include "ssbc.h"

int running = 1;

int main() {
    ssbc_launch();
    while (running) {
        ;
    }
    ssbc_terminate();
    return 0;
}
