/*
Author: Bigyan Chapagain
28 Oct, 2017
*/

/*Caution: nodeMCU cannot tolerate 5V
Metro(x) is the x milliseconds of non blocking delay.*/
#include <OneWire.h>
/** This is OneWire.h library is for nodeMCU/ESP8266. 
  * OneWire.h library made for Arduino is not compatible for nodeMCU right out of the box.
  * This OneWire.h was found on github (as of Sept 30, 2017):
  * https://github.com/Yveaux/esp8266-Arduino/blob/master/esp8266com/esp8266/libraries/OneWire/OneWire.h
  * */
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 6 //connect sensor to pin6 of arduino
#include <SPI.h>
#include <Wire.h>//TWI/IIC lib
#include <Adafruit_GFX.h>//GUI lib
#include <Adafruit_SSD1306.h>//oled h/w lib
#include <TimerOne.h>
#define OLED_RESET LED_BUILTIN
#define WIFI_CHECK_PIN 8
//In nodeMCU the LED_BUILTIN is GPIO16

Adafruit_SSD1306 display(OLED_RESET);//Initialize Adafruit display
OneWire oneWire(ONE_WIRE_BUS);//ds18b20 & oneWire library code : copied-pasted
DallasTemperature sensors(&oneWire);
String systemStatus="LOADING...";
double temp;
String wifiStatus="Trying to Connect";
const int driverPin=9; //relay_driver
volatile byte inrPin=3;//Increasing Temp Button connected to ISR Pin
volatile byte dcrPin=2;//Decreasing Temp Button connected to ISR Pin
int userSetTemp=30;//default temp will be 30 degree Celcius

unsigned long lastInrPush=0;
unsigned long lastDcrPush=0;
unsigned long debounceDelay = 200;
boolean wifi_check=false;
void setup() {
  sensors.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
//  ticker1.attach(.5,disp);//tickerObj.attach(timeInSecs,isrFunction): Timer ISR
  pinMode(driverPin,OUTPUT);
	pinMode(inrPin,INPUT);
	pinMode(dcrPin,INPUT);
  pinMode(WIFI_CHECK_PIN,INPUT);
//  Timer1.initialize(6000000);//1min
//  Timer1.attachInterrupt(wifiCheck);
	attachInterrupt(digitalPinToInterrupt(inrPin), inrTemp, FALLING);
  //digitalPinToInterrupt(3) returns 1 (in Arduino UNO, not in nodeMCU), which is int1
	attachInterrupt(digitalPinToInterrupt(dcrPin), dcrTemp, FALLING);
}
void loop(){
  tempCtrl();//not time critical event. Hence, not called via time interrupt
  disp();
  if(digitalRead(WIFI_CHECK_PIN)==HIGH&&!wifi_check){//Doesn't work perfect; will edit later as we get funding.
    wifiStatus="WiFi Connected";
    wifi_check=true;
  }
}
