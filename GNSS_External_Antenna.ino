

#include "GNSS.h"
#include "STM32L0.h"
#include "RTC.h"
#include "TimerMillis.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#define PPS_PIN 8
#define PPS_PIN A3
GNSSLocation myLocation;
GNSSSatellites mySatellites;
void setup_gps(void)
{

	GNSS.begin(Serial1, GNSS.MODE_UBLOX, GNSS.RATE_1HZ);

	while (GNSS.busy()) {}

	GNSS.setConstellation(GNSS.CONSTELLATION_GPS_AND_GLONASS);

	while (GNSS.busy()) {}

	//GNSS.setAntenna(GNSS.ANTENNA_INTERNAL);
	GNSS.setAntenna(GNSS.ANTENNA_EXTERNAL);

	while (GNSS.busy()) {}

	//GNSS.enableWakeup();
	pinMode(PPS_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(PPS_PIN), wakeup_pps, RISING);
}
void wakeup_pps(void)
{

	if (Reason == Clear_Alarm)
	{
		Reason = PPS_Alarm;
	}
	STM32L0.wakeup();
}

void loop_gps(void)
{
	STM32L0.stop();

	display_gps_rt_clock();
}
