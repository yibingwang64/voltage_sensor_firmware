
void print2digits(int number) {
    if (number < 10) {
        Serial.print("0"); // print a 0 before if the number is < than 10
    }
    Serial.print(number);
}
void print_date_time()
{
    circular_buffer.push(PACKET.DATA.voltage[0]);
    float avg = 0.0;
    using index_t = decltype(circular_buffer)::index_t;
    for (index_t i = 0; i < circular_buffer.size(); i++)
    {
        avg += circular_buffer[i] / circular_buffer.size();
    }
    Serial.print(avg); Serial.print("  ");
    //Serial.print(PACKET.DATA.voltage[0]); Serial.print("  ");
    // Print date...
    print2digits(PACKET.DATA.day);
    Serial.print("/");
    print2digits(PACKET.DATA.month);
    Serial.print("/");
    print2digits(PACKET.DATA.year);
    Serial.print(" ");

    // ...and time
    print2digits(PACKET.DATA.hours);
    Serial.print(":");
    print2digits(PACKET.DATA.minutes);
    Serial.print(":");
    print2digits(PACKET.DATA.seconds);
    Serial.print(".");
   
    uint32_t milliSeconds = ((PACKET.DATA.subseconds >> 17) * 1000 + 16384) / 32768;
    Serial.print(milliSeconds);
    Serial.print(" "); Serial.println(PACKET.DATA.firstOrContinue);

}

void clear_data_packet(void)
{
    for (int i = 0; i < VOLTAGE_VALUE_NUM; i++)
    {
        memcpy(PACKET.RawData, 0, PACKET_LENGTH);
    }
}
void display_gps_rt_clock(void)
{
    if (GNSS.location(myLocation))
    {
        uint8_t year, month, day, hours, minutes, seconds;
        uint32_t subSeconds, milliSeconds;

        static const char* fixTypeString[] = {
            "NONE",
            "TIME",
            "2D",
            "3D",
        };

        static const char* fixQualityString[] = {
            "",
            "",
            "/DIFFERENTIAL",
            "/PRECISE",
            "/RTK_FIXED",
            "/RTK_FLOAT",
            "/ESTIMATED",
            "/MANUAL",
            "/SIMULATION",
        };

        RTC.getDate(day, month, year);
        RTC.getTime(hours, minutes, seconds, subSeconds);

        milliSeconds = ((subSeconds >> 17) * 1000 + 16384) / 32768;

        Serial.print("RTC: ");
        Serial.print(2000 + year);
        Serial.print("/");
        Serial.print(month);
        Serial.print("/");
        Serial.print(day);
        Serial.print(" ");
        if (hours <= 9) {
            Serial.print("0");
        }
        Serial.print(hours);
        Serial.print(":");
        if (minutes <= 9) {
            Serial.print("0");
        }
        Serial.print(minutes);
        Serial.print(":");
        if (seconds <= 9) {
            Serial.print("0");
        }
        Serial.print(seconds);
        Serial.print(".");
        if (milliSeconds <= 9) {
            Serial.print("0");
        }
        if (milliSeconds <= 99) {
            Serial.print("0");
        }
        Serial.print(milliSeconds);
        Serial.println();

        Serial.print("LOCATION: ");
        Serial.print(fixTypeString[myLocation.fixType()]);

        if (myLocation.fixType() != GNSSLocation::TYPE_NONE)
        {
            Serial.print(fixQualityString[myLocation.fixQuality()]);
            Serial.print(" ");
            Serial.print(myLocation.year());
            Serial.print("/");
            Serial.print(myLocation.month());
            Serial.print("/");
            Serial.print(myLocation.day());
            Serial.print(" ");
            if (myLocation.hours() <= 9) {
                Serial.print("0");
            }
            Serial.print(myLocation.hours());
            Serial.print(":");
            if (myLocation.minutes() <= 9) {
                Serial.print("0");
            }
            Serial.print(myLocation.minutes());
            Serial.print(":");
            if (myLocation.seconds() <= 9) {
                Serial.print("0");
            }
            Serial.print(myLocation.seconds());
            Serial.print(".");
            if (myLocation.millis() <= 9) {
                Serial.print("0");
            }
            if (myLocation.millis() <= 99) {
                Serial.print("0");
            }
            Serial.print(myLocation.millis());
           
            
            Serial.println();
            return;
            
            if (myLocation.leapSeconds() != GNSSLocation::LEAP_SECONDS_UNDEFINED) {
                Serial.print(" ");
                Serial.print(myLocation.leapSeconds());
                if (!myLocation.fullyResolved()) {
                    Serial.print("D");
                }
            }

            if (myLocation.fixType() != GNSSLocation::TYPE_TIME)
            {
                Serial.print(" LLA=");
                Serial.print(myLocation.latitude(), 7);
                Serial.print(",");
                Serial.print(myLocation.longitude(), 7);
                Serial.print(",");
                Serial.print(myLocation.altitude(), 3);
                Serial.print(" EPE=");
                Serial.print(myLocation.ehpe(), 3);
                Serial.print(",");
                Serial.print(myLocation.evpe(), 3);
                Serial.print(" SATELLITES=");
                Serial.print(myLocation.satellites());
                Serial.print(" DOP=");
                Serial.print(myLocation.hdop(), 2);
                Serial.print(",");
                Serial.print(myLocation.vdop(), 2);
            }
        }

        Serial.println();
        return;
        GNSS.satellites(mySatellites);

        Serial.print("SATELLITES: ");
        Serial.print(mySatellites.count());

        Serial.println();

        for (unsigned int index = 0; index < mySatellites.count(); index++)
        {
            unsigned int svid = mySatellites.svid(index);

            if ((svid >= 1) && (svid <= 32))
            {
                Serial.print("    ");

                if (svid <= 9)
                {
                    Serial.print("  G");
                    Serial.print(svid);
                }
                else
                {
                    Serial.print(" G");
                    Serial.print(svid);
                }
            }
            else if ((svid >= 65) && (svid <= 96))
            {
                Serial.print("    ");

                if ((svid - 64) <= 9)
                {
                    Serial.print("  R");
                    Serial.print(svid - 64);
                }
                else
                {
                    Serial.print(" R");
                    Serial.print(svid - 64);
                }
            }
            else if ((svid >= 120) && (svid <= 158))
            {
                Serial.print("    ");
                Serial.print("S");
                Serial.print(svid);
            }
            else if ((svid >= 173) && (svid <= 182))
            {
                Serial.print("    ");
                Serial.print("  I");
                Serial.print(svid - 172);
            }
            else if ((svid >= 193) && (svid <= 197))
            {
                Serial.print("    ");
                Serial.print("  Q");
                Serial.print(svid - 192);
            }
            else if ((svid >= 211) && (svid <= 246))
            {
                Serial.print("    ");

                if ((svid - 210) <= 9)
                {
                    Serial.print("  E");
                    Serial.print(svid - 210);
                }
                else
                {
                    Serial.print(" E");
                    Serial.print(svid - 210);
                }
            }
            else if (svid == 255)
            {
                Serial.print("    ");
                Serial.print("R???");
            }
            else
            {
                continue;
            }

            Serial.print(": SNR=");
            Serial.print(mySatellites.snr(index));
            Serial.print(", ELEVATION=");
            Serial.print(mySatellites.elevation(index));
            Serial.print(", AZIMUTH=");
            Serial.print(mySatellites.azimuth(index));

            if (mySatellites.unhealthy(index)) {
                Serial.print(", UNHEALTHY");
            }

            if (mySatellites.almanac(index)) {
                Serial.print(", ALMANAC");
            }

            if (mySatellites.ephemeris(index)) {
                Serial.print(", EPHEMERIS");
            }

            if (mySatellites.autonomous(index)) {
                Serial.print(", AUTONOMOUS");
            }

            if (mySatellites.correction(index)) {
                Serial.print(", CORRECTION");
            }

            if (mySatellites.acquired(index)) {
                Serial.print(", ACQUIRED");
            }

            if (mySatellites.locked(index)) {
                Serial.print(", LOCKED");
            }

            if (mySatellites.navigating(index)) {
                Serial.print(", NAVIGATING");
            }

            Serial.println();
        }
    }
}