void setup_lora(void)
{

    LoRaWAN.begin(US915);
    LoRaWAN.setSubBand(2);
   // LoRaWAN.setADR(true);
    LoRaWAN.setDataRate(2);
    // LoRaWAN.setDutyCycle(false);
    // LoRaWAN.setAntennaGain(2.0);
    LoRaWAN.joinABP(devAddr, nwkSKey, appSKey);

    Serial.println("JOIN");

}
void loop_lora(void)
{
    if (LoRaWAN.joined() && !LoRaWAN.busy())
    {
        LoRaWAN.beginPacket();
      
        LoRaWAN.write((const char*) (&PACKET.RawData[0]), PACKET_LENGTH);
        LoRaWAN.endPacket();
       /* while (LoRaWAN.busy()) {}

        if (LoRaWAN.confirmed())
        {
            Reason = Clear_Alarm;
            print_date_time();
            Serial.println("ACK");
            clear_data_packet();
        }
        else
        {
           
            if (Reason == Voltage_Alarm)
            {
                Reason = Continue_Alarm;
            }
            if(Reason == Timer_Alarm)
            {
                Reason = Clear_Alarm;
            }

            Serial.println("NACK");
        }*/
    }

    //delay(10000);
}


