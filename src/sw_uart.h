/*
 * uart.h
 *
 *  Created on: Jun 17, 2014
 *      Author: tom
 */




void baudrate_set(unsigned int baud);

void uart_init();

int uart_receive_blocking();

int uart_receive_non_blocking();

char uart_send(char data);
