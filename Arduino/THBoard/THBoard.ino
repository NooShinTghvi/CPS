#include <Arduino.h>
#include <Wire.h>
#include <AltSoftSerial.h>

// SHT25 I2C address is 0x40(64)
#define Addr 0x40
float temperature, humidity, lastTemperature, lastHumidity;

void setup() {
    Wire.begin(); // Initialise I2C communication as MASTER
    Serial.begin(9600);
    lastTemperature = 0.00;
    lastHumidity = 0.00;
}

void loop() {
    unsigned int data[2];
    
    Wire.beginTransmission(Addr); // Start I2C transmission
    Wire.write(0xF5); // Send humidity measurement command, NO HOLD master
    Wire.endTransmission(); // Stop I2C transmission
    delay(500);
    Wire.requestFrom(Addr, 2); // Request 2 bytes of data
    if (Wire.available() == 2)  { // Read 2 bytes of data ,humidity msb, humidity lsb
      data[0] = Wire.read();
      data[1] = Wire.read();
      humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
//      Serial.print("Relative Humidity :"); Serial.print(humidity);Serial.println(" %RH"); 
    }

    Wire.beginTransmission(Addr); // Start I2C transmission
    Wire.write(0xF3); // Send temperature measurement command, NO HOLD master
    Wire.endTransmission(); // Stop I2C transmission
    delay(500);
    Wire.requestFrom(Addr, 2); // Request 2 bytes of data
    if (Wire.available() == 2)  { // Read 2 bytes of data ,temp msb, temp lsb
      data[0] = Wire.read();
      data[1] = Wire.read();
      temperature = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85; 
//      Serial.print("Temperature in Celsius :"); Serial.print(temperature);Serial.println(" C"); 
    }

    if(temperature != lastTemperature | humidity != lastHumidity){
        Serial.print(linkTemperatureAndHumidity(temperature, humidity));
        lastTemperature = temperature;
        lastHumidity = humidity;
    }
    
    delay(2000);
}

String linkTemperatureAndHumidity(float tempreture, float humidity) {
    String tempretureStr = String(tempreture);
    String humidityStr = String(humidity);
    return "<" + tempretureStr + ":" + humidityStr + ">";
}
