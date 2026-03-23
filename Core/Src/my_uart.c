/*
 * my_uart.c
 *
 *  Created on: Feb 22, 2026
 *      Author: ozker
 */
#include "my_uart.h"
#include "usart.h"


static char message_buffer[MAX_MESSAGE_LENGTH];
static volatile uint8_t message_length = 0;
static volatile bool message_ready;
static uint8_t rx_byte;

/*
Nadpisanie funkcji __io_putchar aby móc używać funkcji printf która jest wygodna
*/
int __io_putchar(int ch){
    if(ch == '\n'){
        uint8_t ch2 = '\r';
        HAL_UART_Transmit(&huart2, &ch2, 1, HAL_MAX_DELAY);
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return 1;
}

// rozpoczecie odbioru danych
void my_uart_start_receiving(void){
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
}


// Obsluga przerwania. Jesli przyjdzie jakas literka to zostaje ona przypisana w kolejne miejsce bufora. Jesli przyjdzie znak konca linii
//to zostaje ustawiona flaga pozwalajaca na przepisanie zawartosci bufora do jakiejs tablicy
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(rx_byte == '\n' || rx_byte == '\r'){
        if(message_length > 0){
            message_buffer[message_length] = '\0';
            message_ready = true;
        }
    }else if(!message_ready){
        if(message_length >= (MAX_MESSAGE_LENGTH - 1)){
            message_length = 0;
        }
        message_buffer[message_length] = rx_byte;
        message_length++;
        
    }
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
}


// po ustawieniu flagi opisanej powyżej zostaje skopiowany bufor do tablicy podanej jako argument tej funkcji
bool my_uart_get_message(char *message){
    if(message_ready){
        for(uint8_t pos = 0; pos <= message_length; pos ++){
            message[pos] = message_buffer[pos];
        }
        message_length = 0;
        message_ready = false;
        return true;
    }else{
        return false;
    }
}
