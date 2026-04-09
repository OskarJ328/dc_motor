/*
 * my_uart.h
 *
 *  Created on: Feb 22, 2026
 *      Author: ozker
 */

#ifndef INC_MY_UART_H_
#define INC_MY_UART_H_

#include "stdbool.h"

#define MAX_MESSAGE_LENGTH 80

bool my_uart_receive_message(char* buffer);

void my_uart_start_receiving(void);
bool my_uart_get_message(char* message);
#endif /* INC_MY_UART_H_ */
