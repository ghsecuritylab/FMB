#include <SPI.h>


int CS = 18;

void setup() {
  // put your setup code here, to run once:
 
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  Serial.begin(9600);

  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS, LOW);
  SPI.transfer(0x0A);
  SPI.transfer(0x2D);
  SPI.transfer(0b00000010);
  delay(1000);
  digitalWrite(CS, HIGH);
  SPI.endTransaction();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS, LOW);
  SPI.transfer(0x0B);
  SPI.transfer(0x0E);
  byte xdatal = SPI.transfer(0x00);
  byte xdatah = SPI.transfer(0x00);
  byte ydatal = SPI.transfer(0x00);
  byte ydatah = SPI.transfer(0x00);
  byte zdatal = SPI.transfer(0x00);
  byte zdatah = SPI.transfer(0x00);
  delay(1000);
  digitalWrite(CS, HIGH);
  SPI.endTransaction();
  
  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS, LOW);
  SPI.transfer(0x0B);
  SPI.transfer(0x2D);
  byte POWER_CTL = SPI.transfer(0x00);
  delay(1000);
  digitalWrite(CS, HIGH);
  SPI.endTransaction();


  
  Serial.println("X data l: " + xdatal);
 /* Serial.println("X data h: " + xdatah);
  Serial.println("Y data l: " + ydatal);
  Serial.println("Y data h: " + ydatah);
  Serial.println("Z data l: " + ydatal);
  Serial.println("Z data h: " + zdatah);
  */

}
