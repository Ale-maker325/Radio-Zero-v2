#include <Arduino.h>
#include <settings.h>
#include <rgb_led.h>
#include <radioLR.h>
#include <display.h>

// Глобальные объекты шин I2C
// display_wire уже объявлена внутри display.h, 
// поэтому kbd_wire объявляем здесь:
TwoWire kbd_wire = TwoWire(1);

#ifdef TCA8418_KEYBOARD
    #include <Adafruit_TCA8418.h>
    Adafruit_TCA8418 klava;

    bool isUkrainian = false; 
    String inputBuffer = ""; // Здесь хранится набираемое сообщение

    // Раскладки согласно вашей матрице 6x8
    const char* keymapEN = "QWERTYUIOP[]ASDFGHJKL;'\nZXCVBNM\b1234567890,. SL A"; 
    const char* keymapUA = "ЙЦУКЕНГШЩЗХЇФІВАПРОЛДЖЄ\nЯЧСМИТЬ\b1234567890,. SL A";

#endif

uint64_t count = 0;           //Счётчик для сохранения количества отправленных/полученных пакетов
boolean FUN_IS_ON = false;    //Логический флаг включения/отключения вентилятора охлаждения




void setup() {
    #ifdef DEBUG_PRINT
        Serial.begin(115200);
        Serial.println(" ");
        Serial.println(F("*************************  Serial ready 115200 *******************************"));
        Serial.println(" ");
    #endif

    SPI_MODEM.begin(SCK, MISO, MOSI);
    display_wire.begin(I2C_SDA, I2C_SCL, 100000);
    // Инициализация шины 1 для клавиатуры на пинах 38, 39
    kbd_wire.begin(I2C_SDA_2, I2C_SCL_2, 400000); // Для клавиатуры можно 400kHz (Fast Mode)

    
    #ifdef DEBUG_PRINT
        Serial.println(" ");
        Serial.print("Chip Model = ");
        Serial.println(ESP.getChipModel());
        Serial.print("Cpu Freq = ");
        Serial.println(ESP.getCpuFreqMHz());
        Serial.print("ChipRevision = ");
        Serial.println(ESP.getChipRevision());
        Serial.print("SDK Version = ");
        Serial.println(ESP.getSdkVersion());
        Serial.println(" ");
    #endif

    #ifdef DEBUG_PRINT
        Serial.print(TABLE_LEFT);
        Serial.print(F("DISPLAY INIT"));
        Serial.println(TABLE_RIGHT);
        Serial.println(SPACE);
    #endif

    //инициализируем дисплей
    displayInit();

    pinMode(FUN, OUTPUT);                   //Контакт управления вентилятором охлаждения
    


    WriteColorPixel(COLORS_RGB_LED::red);   //Зажигаем красный цвет

    setRadioMode();

    radio1.setRfSwitchPins(RXEN, TXEN);         //Назначаем контакты для управлением вкл./выкл. усилителем передатчика
    

    //Устанавливаем функцию, которая будет вызываться при отправке пакета данных
    radio1.setPacketSentAction(flag_operationDone_1);

    radioBeginAll();

    #ifdef DEBUG_PRINT
        Serial.println(SPACE);
    #endif

    //Устанавливаем наши значения, определённые ранее в структуре config_radio1
    radio_setSettings(radio1, config_radio1, "1");

    #ifdef TRANSMITTER   //Если определена работа модуля как передатчика
        

        //Начинаем передачу пакетов
        Serial.println(F("Sending first packet ... "));

        String str = F("START!");
        
        transmit_and_print_data(str);
        WriteColorPixel(COLORS_RGB_LED::green);

        // и вентилятор охлаждения не включен, то включаем вентилятор охлаждения
        if(config_radio1.outputPower > 1 && FUN_IS_ON != true)
        {
            //отмечаем вентилятор как включенный
            FUN_IS_ON = true;
            //и включаем его
            digitalWrite(FUN, HIGH);
        }

    #endif

    #ifdef TCA8418_KEYBOARD
        // Передаем ссылку на вторую шину &kbd_wire
        if (!klava.begin(TCA8418_DEFAULT_ADDR, &kbd_wire)) {
            Serial.println("Keypad TCA8418 NOT FOUND on Bus 1!");
            // Не вешаем систему совсем, если клавиатура не критична, 
            // но для теста оставим while(1)
            display.clearDisplay();
            display.setTextSize(1);
            display.setCursor(0,30);
            display.print("TCA8418 NOT FOUND");
            display.display();
            while (1); 
        }
        else {
            #ifdef DEBUG_PRINT
                Serial.println("Keypad TCA8418 found on Bus 1.");
            #endif
            display.clearDisplay();
            display.setTextSize(1);
            display.setCursor(0,30);
            display.print("Keypad TCA8418 found on Bus 1.");
            display.display();
            delay(200);
        }

        klava.matrix(6, 8);
        klava.flush();
        #ifdef DEBUG_PRINT
            Serial.println("Keypad TCA8418 initialized on Pins 38/39");
        #endif
    #endif
    

}






void loop() {
    #ifndef TCA8418_KEYBOARD
        // Проверяем нажатия клавиш и обрабатываем ввод
        handleKeyboardInput();
    delay(150);
    WriteColorPixel(COLORS_RGB_LED::black);
    
    #ifdef TRANSMITTER   //Если определен как передатчик проверяем, была ли предыдущая передача успешной
        
        // if(operationDone_1) {
            #ifdef DEBUG_PRINT
                Serial.println("..................................................");
            #endif
            //Сбрасываем сработавший флаг прерывания
            operationDone_1 = false;

            //готовим строку для отправки
            String str = "#" + String(count++);
            transmit_and_print_data(str);
            WriteColorPixel(COLORS_RGB_LED::green);
            
        // }

    #endif
    #endif

    // Небольшая задержка, чтобы не перегружать процессор
    delay(10); 
    
    #ifdef TCA8418_KEYBOARD
        if (klava.available() > 0) {
            int k = klava.getEvent();
            bool pressed = k & 0x80;
            k &= 0x7F;
            k--; // Приводим к индексу 0-47

            if (pressed) {
                // Координата кнопки для отладки
                int row = k / 8;
                int col = k % 8;

                // 1. Обработка переключения языка (кнопка L - индекс 46)
                if (k == 46) { 
                    isUkrainian = !isUkrainian;
                } 
                // 2. Обработка Backspace (индекс зависит от вашей пайки, в таблице ROW 3, COL 7 = 31)
                else if (k == 31) { 
                    if (inputBuffer.length() > 0) {
                        // Удаляем последний символ (учитываем UTF-8 для кириллицы)
                        // Для простоты в Arduino String:
                        inputBuffer.remove(inputBuffer.length() - 1);
                    }
                }
                // 3. Обработка Enter (ROW 2, COL 7 = 23)
                else if (k == 23) {
                    if (inputBuffer.length() > 0) {
                        transmit_and_print_data(inputBuffer); // Отправляем в LoRa
                        inputBuffer = ""; // Очищаем после отправки
                    }
                }
                // 4. Обычный ввод символов
                else if (k < 48) {
                    char c = isUkrainian ? keymapUA[k] : keymapEN[k];
                    // Добавляем символ, если это не служебные заглушки (S, L, A)
                    if (c != 'S' && c != 'L' && c != 'A') {
                        inputBuffer += c;
                    }
                }

                // --- ОБНОВЛЕНИЕ ЭКРАНА ---
                display.clearDisplay();
                
                // Статус-бар (верхняя часть)
                display.setTextSize(1);
                display.setCursor(0, 0);
                display.print(isUkrainian ? "UA" : "EN");
                display.setCursor(100, 0);
                display.print(FUN_IS_ON ? "FAN" : "");
                
                display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

                // Поле ввода (нижняя часть)
                display.setCursor(0, 20);
                display.print("Текст:");
                display.setCursor(0, 35);
                display.setTextSize(1); // Можно увеличить до 2, если текст короткий
                display.print(inputBuffer);
                
                display.display();
            }
        }
    #endif
}

