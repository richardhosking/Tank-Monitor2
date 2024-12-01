/*
  *@File  : DFRobot_Distance_A02.ino 
  *@Brief : This example use A02YYUW ultrasonic sensor to measure distance
  *         With initialization completed, We can get distance value 
  *@Copyright [DFRobot](https://www.dfrobot.com),2016         
  *           GUN Lesser General Pulic License
  *@version V1.0           
  *@data  2019-8-28
*/

#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include "LowPower.h"
#include "uSsensor.h"



// supply power to US transducer via a pin on the arduino
#define US_power 12

unsigned char data[4]={};
float distance;
int read_interval = 5; // time in sec between US transducer readings
int count = 0;
int Tx = 11;
int Rx = 10;
long BAUD = 9600;
bool inverse = false;
// Serial data available flag for ISR
volatile bool serialData = false;
volatile int serialDatacount = 0;
// Declare instance of US sensor
// port pins and baud are in class header file
USsensor sensor(Tx,Rx, inverse);

// ISR (Interrupt service routine) for Serial Port
// Must have no parameters and no return
// Cannot call Class members?
void serviceSerial(void){
  // set data flag
  // loop will poll
  serialData = true;
  serialDatacount++;


  }


void setup()
{
  // Start serial port to board for debugging purposes
 Serial.begin(9600);
 delay(200);
 Serial.println("Serial port active");
 
 // Setup UART  and power to transducer 
 pinMode(US_power,OUTPUT);
 //pinMode(Tx,OUTPUT);
 digitalWrite(US_power,HIGH);
 //digitalWrite(Tx,HIGH);
 delay(500);
 
  // Set up serial port interrupt
  // pinMode for Rx is setin constructor
  // Use Hardware interrupts on pin 2,3
  attachInterrupt(digitalPinToInterrupt(Rx), serviceSerial, CHANGE); 

  // Start serial port
 sensor.ser.begin(BAUD); // Looks like this transducer will only work at 9600 baud
 delay(300);
  sensor.ser.listen();
  Serial.print("Sensor read output  :");
  Serial.println(sensor.ser.read());
  serialData = false;
}

void loop()
{
  /*
  *  @brief  Read the distance value from the ultrasonic sensor
  * @param  None
  * @return None
  * Watchdog timer will start transducer every <interval> seconds
  */
  
  /*digitalWrite(US_power,LOW);
  Serial.println("Entering power down for 5sec");
  //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
  //digitalWrite(US_power,LOW);
  delay(300);
  Serial.println("Out of Low power mode");
  //digitalWrite(Tx,HIGH);
  serialData = false; // Clear data flag
  digitalWrite(US_power,HIGH);
  digitalWrite(Tx,LOW);// set up sensor for continuous output
  sensor.ser.listen();
  delay(500);
  // Test interrupt routine 
  //pinMode(Rx,OUTPUT);
  //digitalWrite(Rx,HIGH);
  //digitalWrite(Rx, LOW);
  */
 //pinMode(Rx,INPUT_PULLUP);
  Serial.println(sensor.ser.read());
  Serial.println(serialDatacount);
  if (serialData == true){
    Serial.println("data flag set"); 
    // data is available - read serial port
    if (sensor.ser.read() == 255){
      data[0] = 255;
      for (int i = 1; i < 4; i++){
        data[i] = sensor.ser.read();
        Serial.print("data[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(data[i]);
      }
    int sum;
    sum=(data[0]+data[1]+data[2])&0x00FF;
    Serial.print("sum = ");
    Serial.println(sum);
    Serial.print("Checksum = ");
    Serial.println(data[3]);
    if(sum == data[3]){
      distance=(data[1]<<8)+data[2];
      if(distance>30){
        Serial.print("distance=");
        Serial.print(distance/10);
        Serial.println("cm");
      }
      else{
        Serial.println("Below the lower limit");
        }
    }
    else Serial.println("ERROR");
  }
  
  
}

delay(5000);
Serial.println("post delay");
serialData = false; // clear data flag
}

  
  
