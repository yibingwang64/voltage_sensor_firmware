#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
void setup_ole() {
 
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        //for (;;); // Don't proceed, loop forever
    }
    display.clearDisplay();
}

void ole_display(void)
{
    display.clearDisplay();

    // display.setTextSize(1);             // Normal 1:1 pixel scale
     //display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    //display.println(F("Latium High Votlatage Sensor"));

   // display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
   // display.println(3.141592);

    uint32_t milliSeconds = ((RTC.getSubSeconds() >> 17) * 1000 + 16384) / 32768;
    display.setTextSize(2);             // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    
    int seconds = RTC.getSeconds();
    

    if (seconds <= 9) {
        display.print("0");
    }
    display.print(seconds);
    display.print(".");
    if (milliSeconds <= 9) {
        display.print("0");
    }
    if (milliSeconds <= 99) {
        display.print("0");
    }

    display.println(milliSeconds);
    display.print(RTC.getHours()); display.print(":"); display.println(RTC.getMinutes());
    display.print(RTC.getDay()); display.print("/"); display.print(RTC.getMonth()); display.print("/"); display.println(RTC.getYear());
    float value = analogRead(PIN_Voltage);
    display.println( (float)value / 1023.0F * 3.3F);
    //display.print(F("0x")); display.println(0xDEADBEEF, HEX);

    display.display();
}