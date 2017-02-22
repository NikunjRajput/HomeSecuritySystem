#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <stdint.h>


// initialize serial port via micro USB on STM32F4 Discovery Board
void init_serial_port_usb(); 

// read a byte from the serial port (via micro USB); byte is returned in *c; function returns 1 if successfully read, 0 if not
uint8_t read_serial_usb_byte(uint8_t *c); 

// writes a sequence of n bytes (i.e., a char array) to the serial port (via micro USB)
// n should be the number of bytes; a should point to an array containing n bytes to be written
void write_serial_usb_bytes(uint8_t *a, int n);

#endif
