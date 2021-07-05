/*
#include "CircularBuffer.h"

CircularBuffer<int, 4> buffer1;

unsigned long time = 0;

#define SAMPLE_PIN A0

void setup100() {
	Serial.begin(9600);
	pinMode(SAMPLE_PIN, INPUT);
	time = millis();
}

void loop100() {
	// samples A0 and prints the average of the latest hundred samples to console every 500ms
	int reading = analogRead(SAMPLE_PIN);

	buffer1.push(reading);

	if (millis() - time >= 500) {
		time = millis();
		float avg = 0.0;
		// the following ensures using the right type for the index variable
		using index_t = decltype(buffer1)::index_t;
		for (index_t i = 0; i < buffer1.size(); i++) {
			avg += buffer1[i] / buffer1.size();
		}
		Serial.print("Average is ");
		Serial.println(avg);
	}
}
*/