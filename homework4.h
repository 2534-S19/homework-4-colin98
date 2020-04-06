/*
 * homework4.h
 *
 *  Created on: 04/02/2020
 *      Author: Colin Farrell
 */

#ifndef HOMEWORK4_H_
#define HOMEWORK4_H_

// This function initializes the board by turning off the Watchdog Timer.
void initBoard();
bool charFSM(char rChar);

// TODO: Define any constants that are local to homework.c using #define
#define UART_9600_BAUDRATE_PRESCALAR        19
#define UART_9600_BAUDRATE_FIRST_MOD_REG    8
#define UART_9600_BAUDRATE_SECOND_MOD_REG   0x55
#define NO_CHARACTER_RECEIVED               0xFF
#define FIRST_CHAR                          '2'
#define SECOND_CHAR                         '5'
#define THIRD_CHAR                          '3'
#define FOURTH_CHAR                         '4'
#define END_OF_STRING                       '\0'

#endif /* HOMEWORK4_H_ */
