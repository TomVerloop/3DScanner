/*
 * uart.c
 *
 *  Created on: Jun 17, 2014
 *      Author: tom
 */

#include "LPC11xx.h"
#include "sw_uart.h"
#include "timer.h"

const int Hz = 1;
const int kHz = 1000;
const int MHz = 1000 * 1000;

const unsigned int LSR_RDR = 0x01;
const unsigned int LSR_OE = 0x02;
const unsigned int LSR_PE = 0x04;
const unsigned int LSR_FE = 0x08;
const unsigned int LSR_BI = 0x10;
const unsigned int LSR_THRE = 0x20;
const unsigned int LSR_TEMT = 0x40;
const unsigned int LSR_RXFE = 0x80;

const int clock_frequency = 12 * 1000 * 1000;

const int baudrate = 38400;

void baudrate_set(unsigned int baud)
{
	int Fdiv;
	LPC_SYSCON->UARTCLKDIV = 0x1;     // divided by 1
	Fdiv = clock_frequency / ( LPC_SYSCON->SYSAHBCLKDIV * 16 * baud);
	LPC_UART->DLM = Fdiv / 256;
	LPC_UART->DLL = Fdiv % 256;
}

void uart_init()
{
	//SET UP UART (section 13.2 in datasheet "BASIC CONFIGURATION")
	// enable IO config
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);     //enable IOCON
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12);     //enable UART

	// UART I/O config
	LPC_IOCON->PIO1_6 &= ~0x07;
	LPC_IOCON->PIO1_6 |= 0x01;     // UART RXD
	LPC_IOCON->PIO1_7 &= ~0x07;
	LPC_IOCON->PIO1_7 |= 0x01;     // UART TXD

	// Enable UART clock
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12);

	LPC_UART->LCR = 0x83;             // 8 bits, no Parity, 1 Stop bit
	baudrate_set(38400);
	LPC_UART->LCR = 0x03;   // DLAB = 0
	LPC_UART->FCR = 0x07;   // Enable and reset TX and RX FIFO.

	// Read to clear the line status.
	(void) LPC_UART->LSR;

	// Ensure a clean start, no data in either TX or RX FIFO.
	while ((LPC_UART->LSR & (LSR_THRE | LSR_TEMT)) != (LSR_THRE | LSR_TEMT))
		;
	while ( LPC_UART->LSR & LSR_RDR)
	{
		(void) LPC_UART->RBR; // Dump data from RX FIFO
	}
}

int uart_receive_blocking()
{
	while (1)
	{               //wait for transmitted byte to loop back and be received
		if (LPC_UART->LSR & 0x01) //if Receiver Data Ready bit set (sec 13.5.9)
			break;
	}
	return LPC_UART->RBR;               //store received data (sec 13.5.1)
	int data = LPC_UART->RBR;
	if (data != 0x01)
	{
		uart_send(0x01);
	}
	return 0;
}

int uart_receive_non_blocking()
{
	if (LPC_UART->LSR & 0x01) //if Receiver Data Ready bit set (sec 13.5.9)
	{
		int data = LPC_UART->RBR;
		if (data != 0x01)
		{
			uart_send(0x01);
		}
		return data;
	}
	else
		return -1;
}

char uart_send(unsigned int data)
{
	LPC_UART->THR |= data & 0xFF;
	if (data == 0x01)
	{
		return 1;
	}
	int x = now() + 100 * 1000;
	while (x > now())
	{
		unsigned int ack = uart_receive_non_blocking();
		if (ack == 0x01)
		{
			return 1;
		}
	}
	return 0;
}
