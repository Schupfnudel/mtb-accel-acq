
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = A5; //CS für Sd Kartenleser eh wirkungslos
const int accel_kettenstrebe = 9;
const int accel_lenker = 10;
const int accel_rahmen = 11;
const int n_messwerte = 6000;
const int MPU_addr = 0x69; //MPUs have two different adresses available, to use three via I2C I toggle between them by setting one AD0 pin at a time HIGH and then reading
int16_t daten [n_messwerte][3][3];  // 3Darray for data, 1. number of buffered values, 2. three axes, 3. three sensors
unsigned long zeit [n_messwerte];   //timestamp, needed to calculate freq and ftt later on

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(accel_kettenstrebe, OUTPUT);
  pinMode(accel_lenker, OUTPUT);
  pinMode(accel_rahmen, OUTPUT);

  Wire.begin();
  Wire.setClock(400000);


  for (int m = 0; m < 3; m++) { //initialising all 3 MPUs
    if (m == 0) {
      digitalWrite(accel_kettenstrebe, HIGH);
      digitalWrite(accel_lenker, LOW);
      digitalWrite(accel_rahmen, LOW);
    }
    else if (m == 1) {
      digitalWrite(accel_kettenstrebe, LOW);
      digitalWrite(accel_lenker, HIGH);
      digitalWrite(accel_rahmen, LOW);
    }
    else {
      digitalWrite(accel_kettenstrebe, LOW);
      digitalWrite(accel_lenker, LOW);
      digitalWrite(accel_rahmen, HIGH);
    }


    Wire.beginTransmission(MPU_addr);
    Wire.write(0x1C);  //Range register
    Wire.write(0x18);     // 00 = 2g, 01 = 4g, 10 = 8g, 11 = 16g
    Wire.endTransmission(true);
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x19);  // ODR register
    Wire.write(0);     // ODR = 8kHz/wert
    Wire.endTransmission(true);
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);  // PWR register
    Wire.write(0);     // wake up MPU6050
    Wire.endTransmission(true);
  }



  if (!SD.begin(chipSelect)) { //test if sd card present
    while (1);
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }



}




void loop() {


  for (int i = 0; i < n_messwerte; i++) {

    for (int k = 0; k < 3; k++) {  //looping thru 3 MPUs to acq data, switching takes about 0,1ms, should be fine
      switch (k) {
        case 0:
          digitalWrite(accel_kettenstrebe, HIGH);
          digitalWrite(accel_lenker, LOW);
          digitalWrite(accel_rahmen, LOW);
          break;
        case 1:
          digitalWrite(accel_kettenstrebe, LOW);
          digitalWrite(accel_lenker, HIGH);
          digitalWrite(accel_rahmen, LOW);
          break;
        case 2:
          digitalWrite(accel_kettenstrebe, LOW);
          digitalWrite(accel_lenker, LOW);
          digitalWrite(accel_rahmen, HIGH);
          break;
        default:
          break;
      }
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr, 6, true); // request a total of 6 registers
      zeit[i] = micros();
      daten[i][0][k] = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
      daten[i][1][k] = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      daten[i][2][k] = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Wire.endTransmission(true);

    }
  }

  File data = SD.open("accel.txt", FILE_WRITE); //save data to SD when RAM buffer is full

  for (int j = 0; j < n_messwerte; j++) { //format of csv for matlab extraction: timestamp, sensor1x, sensor1y, sensor1z, sensor2x ..... 
    data.print(zeit[j], DEC);
    data.write(", ");
    for (int l = 0; l < 3; l++) {
      data.print(daten[j][0][l], DEC);
      data.write(", ");
      data.print(daten[j][1][l], DEC);
      data.write(", ");
      data.print(daten[j][2][l], DEC);
      data.write(", ");
    }
    data.println();
  }
  data.flush();
}
