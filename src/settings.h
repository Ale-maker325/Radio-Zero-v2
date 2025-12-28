#pragma once
/*   Этот файл предназначен для проверки наличия некоторых флагов по умолчанию */
/*   Только код PreCompiler должен быть установлен в этом файле                */
#include <Arduino.h>

#define DEBUG_PRINT

/**
 * @brief Следует раскомментировать одно из определений, а другое закомментировать.
 *  - RADIO_1 - раскомментировать если используется одно радио
 * 
 */
#define RADIO_1


/**
 * @brief Выбор режима работы радио при компиляции. Раскомментировать то, что необходимо
 * 
 */

 #define TRANSMITTER             //раскомментировать, если модуль будет использоваться как простой передатчик
 //#define RECEIVER                //раскомментировать, если модуль будет использоваться как простой приёмник


 #define TCA8418_KEYBOARD   //раскомментировать, если используется клавиатура TCA8418



//**************************************************** Параметры радио для компиляции ************************************************

//Задаём параметры конфигурации радиотрансивера 1
#define RADIO_1_FREQ 433
#define RADIO_1_BANDWIDTH 125
#define RADIO_1_SPREAD_FACTOR 9
#define RADIO_1_CODING_RATE 7
#define RADIO_1_SYNC_WORD RADIOLIB_LR11X0_LORA_SYNC_WORD_PRIVATE
#define RADIO_1_OUTPUT_POWER 5
#define RADIO_1_CURRENT_LIMIT 320
#define RADIO_1_PREAMBLE_LENGTH 8
#define RADIO_1_GAIN 0

//**************************************************** Параметры радио для компиляции ************************************************


#ifdef TRANSMITTER 
  String RADIO_1_NAME = F("TX_1");
#endif

#ifdef RECEIVER
  String RADIO_1_NAME = F("RX_1");
#endif


//################## пины дисплея ##################

#define I2C_SDA 3
#define I2C_SCL 5

// Пины второй шины (Клавиатура и периферия)
#define I2C_SDA_2 38
#define I2C_SCL_2 39

//########### конец пинов дисплея ##################

#define FUN 17
#define LED 21
	
//################## пины радио ##################

#define NSS 13
#define DIO1 10
#define NRST 12
#define BUSY 11
#define MOSI 8
#define MISO 9
#define SCK 7
#define TXEN 2
#define RXEN 1
	
//############## конец пинов радио ##############

