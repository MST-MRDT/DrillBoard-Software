#include "RoveSense.h"

//DS18B20 digital thermometer communicates over a 1-Wire bus and derives parasite power power directly from data line 
float RoveSense_TempDS18B20_read(int data_pin) 
{
  OneWire Thermometer(data_pin);

  byte data[12];
  byte addr[8];
  Thermometer.reset();

  // Broadcasting to all devices. Todo find single address: Thermometer.select(addr);
  Thermometer.write(DS_BROADCAST_ALL);

  // start conversion, with parasite power on at the end 
  Thermometer.write(DS_CONVERT_TEMP,1); 

  byte present = Thermometer.reset();

  Thermometer.write(DS_BROADCAST_ALL);

  // Read Scratchpad 
  Thermometer.write(DS_READ_SCRATCHPAD); 

  // we need 9 bytes
  for (int i = 0; i < 9; i++) 
  { 
    data[i] = Thermometer.read();
  }//end for

  Thermometer.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  // using two's complement
  float tempRead = ((MSB << 8) | LSB); 
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;
}//end fnctn

//4-wire LM393 Comparator  VCC: 3.3V-5V, GND, DO: digital output, AO: Analog Output Interface
float RoveSense_HumidityXD28_read(int data_pin) 
{
  int32_t sensorValue = (-static_cast<int32_t>(analogRead(data_pin)) + 1023) * 100 / 706;

  if(sensorValue >= 100)
  {
      return 100;
  }else{

    return static_cast<int>(sensorValue);
  }//end if 
}//end fnctn

//H-bridge, voltage flipping cheap two-probe soil moisture sensor without electrolysis
float RoveSense_HumidityCustom_read(int analog_read_pin, int data_flip_pin1, int data_flip_pin2)
{
  digitalWrite(data_flip_pin1, HIGH);
  digitalWrite(data_flip_pin2, LOW);
  
  int reading_1 = analogRead(analog_read_pin);
  
  delay(DATA_FLIP_DELAY);
  
  digitalWrite(data_flip_pin1, LOW);
  digitalWrite(data_flip_pin2, HIGH);
  
  int reading_2 = 1023 - analogRead(analog_read_pin);
  
  float average = (reading_1 + reading_2) / 2;
}//end fnctn
