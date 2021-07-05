#include "STM32L0.h"
#include "LoRaWAN.h"
#include "TimerMillis.h"
#include "arduino.h"
#include "pins_arduino.h"
#include "Wire.h"
#include "BQ35100.h"
#include "math.h"
#include "RTC.h"
#include "gnss.h"
#include "CircularBuffer.h"
//const char* devAddr = "0100000A";
//const char* nwkSKey = "2B7E151628AED2A6ABF7158809CF4F3C";
//const char* appSKey = "2B7E151628AED2A6ABF7158809CF4F3C";
const char* devAddr = "0100000B";
const char* nwkSKey = "22220000000000000000000000000002";
const char* appSKey = "22220000000000000000000000000002";
 static bool IsLoRaWanOcupied = false;
unsigned int CountDown = 3;
/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 35;
const byte hours = 12;
const byte day = 6;
const byte month = 11;
const byte year = 20;

#define myLed 10 // red led
//BQ35100 bq35100;
float VDDA, VBAT, VUSB, Temperature;
uint32_t UID[3] = { 0, 0, 0 };
char buffer[32];
CircularBuffer<float, 1> circular_buffer;
volatile static enum {
	Voltage_Alarm = 0x01,
	Continue_Alarm = 0x02,
	Timer_Alarm = 0x04,
	PPS_Alarm = 0x8,
	Clear_Alarm = 0x16,
	Error=0x32
}Reason=Clear_Alarm;

//BQ35100 bq;

#define VOLTAGE_VALUE_NUM 3
#define PACKET_LENGTH 28
struct data {
	uint32_t id;
	float voltage[VOLTAGE_VALUE_NUM];
	uint32_t subseconds;
	byte reason;
	byte seconds;
	byte minutes;
	byte hours;
	byte day;
	byte month;
	byte year;
	byte firstOrContinue;
}; 
static union {
	data DATA;
	byte RawData[PACKET_LENGTH];
} PACKET;
//13,12,11,10,9
//int PIN_Voltage_Wakeup = 9;//Test
//int PIN_Voltage = A0;//test
int PIN_Voltage_Wakeup = A4;
int PIN_Voltage = A0;
TimerMillis TimerOff, TimerOn, TimerGPS;
int LedOnPeriod = 2000;
int LedOnTime = 100;

void setup_serial(void)
{
	pinMode(myLed, OUTPUT);
	digitalWrite(myLed, LOW);  // start with leds off, since active HIGH
	
	Serial.begin(115200);
	delay(2000);
	//while (!Serial) {}
}
void setup_rtc(void) {
//	RTC.setTime(hours, minutes, seconds);
//	RTC.setDate(day, month, year);
//	RTC.setAlarmTime(1, 2, 3);
//	RTC.enableAlarm(RTC.MATCH_SS); //per min
	RTC.enableAlarm(RTC.MATCH_ANY);//per second
	RTC.attachInterrupt(alarmMatch);
}
void setup_voltage(void) {

	//  pinMode(x, INPUT_PULLUP);
	//pinMode(PIN_Voltage_Wakeup, INPUT_PULLDOWN);
	pinMode(PIN_Voltage_Wakeup, INPUT);
	attachInterrupt(digitalPinToInterrupt(PIN_Voltage_Wakeup), wakeup_voltage, RISING); //LOW,CHANGE,RISING,FALLING,HIGH
	//bq35100.begin(0x55);
}

long int mill = millis();

static float rms = 0;
static float voltage = 0.0;
 int value;

void setup(void)
{
	setup_serial();
	setup_gps();
	setup_voltage();
	setup_rtc();
	setup_lora();
	//setup_ole();
	pinMode(PIN_LED, OUTPUT);
	TimerOn.start(callbackOn, 0, LedOnPeriod);
}

void loop(void)
{

	STM32L0.stop();

	switch (Reason)
	{
	case Voltage_Alarm:
		Serial.println("Cable Alarm!");
		Reason = Clear_Alarm;
		load_data();
		//PACKET.DATA.firstOrContinue=0;
	
		loop_lora();
		break;
	//case continue_alarm:
	//	serial.println(packet.data.firstorcontinue);
	//	serial.println("countine_alarm!");
	//	packet.data.firstorcontinue++;
	//	loop_lora();
	//	break;
	case Timer_Alarm:
		//Serial.println("Timer");
		load_data();
		loop_lora();
		Reason = Clear_Alarm;
		break;
	case PPS_Alarm:
		//Serial.println("PPS Alarm!");
		//ole_display();
	/*	if (!GNSS.busy())
		{
			display_gps_rt_clock();
		}*/
		//if ((int)RTC.getSeconds() == 0)
		//{
		//	Serial.println("Timer Alarm!");
		//	load_data();
		//	loop_lora();
			
		//}
		Reason = Clear_Alarm;
		break;
	default:
		Serial.println("OK");
		break;

	}

	//blink();
	
}
void load_data(void)
{

	//RTC.getDate(PACKET.DATA.day, PACKET.DATA.month, PACKET.DATA.year);
	//RTC.getTime(PACKET.DATA.hours, PACKET.DATA.minutes, PACKET.DATA.seconds, PACKET.DATA.subseconds);
	PACKET.DATA.seconds = RTC.getSeconds();
	PACKET.DATA.minutes = RTC.getMinutes();

	PACKET.DATA.hours = RTC.getHours();
	PACKET.DATA.day = RTC.getDay();

	PACKET.DATA.month = RTC.getMonth();
	PACKET.DATA.year = RTC.getYear();
	PACKET.DATA.subseconds = RTC.getSubSeconds();

	PACKET.DATA.reason = Reason;
	PACKET.DATA.firstOrContinue = 0;

	PACKET.DATA.id = (int)strtol(devAddr, NULL, 16);//atoi(devAddr);


	for (int i = 0; i < VOLTAGE_VALUE_NUM; i++)
	{
		value = analogRead(PIN_Voltage);
		PACKET.DATA.voltage[i] = (float)value / 1023.0F * 3.3F;
	}

	 
	 print_date_time();

}
void blink(void)
{
	for (int i = 0; i < 10; i++) {
		digitalWrite(myLed, HIGH); // toggle red led on
		delay(100);                // wait 100 millisecond
		digitalWrite(myLed, LOW);  // toggle red led off
		delay(100);
	}
}
void wakeup_voltage(void)
{

	if (Reason != Voltage_Alarm)
	{
		Reason = Voltage_Alarm;
	}
	STM32L0.wakeup();
	//Serial.println("INT");
}
void alarmMatch()
{
	if (Reason == Clear_Alarm)
	{
		Reason = Timer_Alarm;
	}
	STM32L0.wakeup();
}

void callbackOn(void)
{
	digitalWrite(PIN_LED, LOW);
	TimerOff.start(callbackOff, LedOnTime);
}
void callbackOff(void)
{
	digitalWrite(PIN_LED, HIGH);

}