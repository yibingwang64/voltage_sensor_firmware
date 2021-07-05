#include "BQ35100.h"
BQ35100::BQ35100() {
}
BQ35100::~BQ35100() {
}

/**
 * Initialize BQ35100 communication via I2C
 * @param address the address of BQ35100, default to 0x55
 * @return true/false indicates whether the BQ35100 component is reachable or not
 */
bool BQ35100::begin(uint8_t address) {
    Wire.begin();
    if (address != 0) {
        this->i2cAddress = address;
    }

    Wire.beginTransmission(this->i2cAddress);
    return Wire.endTransmission() == 0;
}


/**
 * Read the voltage of the battery
 * @return voltage in mV unit
 */
uint16_t BQ35100::readVoltage() {
    Wire.beginTransmission(this->i2cAddress);
    Wire.write(BQ35100_READ_VOLTAGE_MSB);
    Wire.write(BQ35100_READ_VOLTAGE_LSB);
    Wire.endTransmission();

    uint16_t result = 0;

    Wire.requestFrom(this->i2cAddress, 2);
    
    if (Wire.available() >= 2) {
        result = Wire.read(); // MSB first
        result = (result << 8) | Wire.read();
    }

    return result;
}

battery_status_t BQ35100::readBatteryStatus() {
    Wire.beginTransmission(this->i2cAddress);
    Wire.write(BQ35100_READ_BATTERY_STATUS);
    Wire.endTransmission();
    battery_status_t status;

    uint8_t result = 0;
    Wire.requestFrom(this->i2cAddress, 1);

    if (Wire.available() >= 1) {
        result = Wire.read();
    }

    if (result == 0) {
        status.isDischargeDetected = false;
        status.isAlertActive = false;
    } else {
        status.isDischargeDetected = ((result >> 2) & 0x01);
        status.isAlertActive = ((result >> 0) & 0x01);
    }

    return status;
}

int16_t BQ35100::readCurrent() {
    Wire.beginTransmission(this->i2cAddress);
    Wire.write(BQ35100_READ_CURRENT_MSB);
    Wire.write(BQ35100_READ_CURRENT_LSB);
    Wire.endTransmission();

    int16_t result = 0;

    Wire.requestFrom(this->i2cAddress, 2);

    if (Wire.available() >= 2) {
        result = Wire.read(); // MSB first
        result = (result << 8) | Wire.read();
    }

    return result;
}
