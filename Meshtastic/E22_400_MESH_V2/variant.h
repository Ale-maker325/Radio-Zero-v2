#undef LED_PIN 
#undef EXT_NOTIFY_OUT

//*************************************************  RGB NEOPIXEL   ****************************************************/
// Board has RGB LED 21
//#define ENABLE_AMBIENTLIGHTING
#define HAS_NEOPIXEL                         // Enable the use of neopixels
#define NEOPIXEL_COUNT 1                     // How many neopixels are connected
#define NEOPIXEL_DATA 21                     // gpio pin used to send data to the neopixels
#define NEOPIXEL_TYPE (NEO_RGB + NEO_KHZ800) // type of neopixels in use
//************************************************  END LED  ***************************************************/


//*************************************************   OLED   ****************************************************/
// Дисплей OLED
#define HAS_SCREEN 1
// #define USE_SH1106 1
#define USE_SSD1306
#define I2C_SCL 5
#define I2C_SDA 3
//***********************************************   END OLED  ***************************************************/


//*************************************************   I2C2   ****************************************************/
// Другий I2C для підключення зовнішніх пристроїв
//#define I2C_SDA1 42
//#define I2C_SCL1 41
//*************************************************  END I2C2  ***************************************************/

//*************************************************   GPS   ******************************************************/
// GPS
// FIXME: unsure what to define HAS_GPS as if GPS isn't always present
// #define HAS_GPS 1 // Don't need to set this to 0 to prevent a crash as it doesn't crash if GPS not found, will probe by default
// #define PIN_GPS_EN 00000
// #define GPS_EN_ACTIVE 00000
// #define GPS_TX_PIN 00000
// #define GPS_RX_PIN 00000
//*************************************************  END GPS  *****************************************************/


//*************************************************   BUTTON   ****************************************************/
// Пін кнопки
#define BUTTON_PIN 0   
#define BUTTON_NEED_PULLUP

//*************************************************  END BUTTON  ***************************************************/


//***************************************************   UART   *****************************************************/
// UART
#define UART_TX 43
#define UART_RX 44

//*************************************************   END UART   ****************************************************/


//***************************************************   FAN    ******************************************************/
//#define RF95_FAN_EN 17 // Пін для керування вентилятором якщо буде це потрібно. Теоретично достатньо радіатора,
                        // або навіть можна обійтись і без радіатора, якщо не буде суттєвого нагріву модема, бо 
                        // проєкт меш-мережі не передбачає високих навантажень на радіомодем
//*************************************************   END FAN   *****************************************************/


//*************************************************   Піни для інтерфейсу радіомодема    ******************************************************/

 
#define USE_SX1268 // E22-400M30S використовує SX1268
#define SX126X_MAX_POWER 22 // Максимальна потужність передавача
#define SX126X_DIO3_TCXO_VOLTAGE 1.8

//SPI
#define SX126X_CS 13    // EBYTE module's NSS pin
#define SX126X_SCK 7    // EBYTE module's SCK pin
#define SX126X_MOSI 8  // EBYTE module's MOSI pin
#define SX126X_MISO 9  // EBYTE module's MISO pin
#define SX126X_RESET 12 // EBYTE module's NRST pin
#define SX126X_BUSY 11  // EBYTE module's BUSY pin
#define SX126X_DIO1 10  // EBYTE module's DIO1 pin

#define SX126X_TXEN 2 // Schematic connects EBYTE module's TXEN pin to MCU
#define SX126X_RXEN 1 // Schematic connects EBYTE module's RXEN pin to MCU

#define LORA_CS SX126X_CS     // Compatibility with variant file configuration structure
#define LORA_SCK SX126X_SCK   // Compatibility with variant file configuration structure
#define LORA_MOSI SX126X_MOSI // Compatibility with variant file configuration structure
#define LORA_MISO SX126X_MISO // Compatibility with variant file configuration structure
#define LORA_DIO1 SX126X_DIO1 // Compatibility with variant file configuration structure

//*************************************************  END Піни для інтерфейсу радіомодема   *****************************************************/