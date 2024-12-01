/*
file USsensor.h
@brief DFRobot_A02_Distance class
Modified from the Python code as the Arduino is not well documented
Richard Hosking Apr2024
@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
@license     The MIT License (MIT)
@author      Arya(xue.peng@dfrobot.com)
@version     V1.0
@date        2021-08-30
@url https://github.com/DFRobot/DFRobot_RaspberryPi_A02YYUW
*/

#ifndef USsensor_h
#define USsensor_h

#include <Arduino.h>
#include <SoftwareSerial.h>

//int tx = 10;
//int rx = 11;
//int baud = 9600;

// US status
enum status_t
{
  STA_OK = 0x00,
  STA_ERR_CHECKSUM = 0x01,
  STA_ERR_SERIAL = 0x02,
  STA_ERR_CHECK_OUT_LIMIT = 0x03,
  STA_ERR_CHECK_LOW_LIMIT = 0x04,
  STA_ERR_DATA = 0x05
};

// rebuild class from Python code - inherit SoftwareSerial library
class USsensor : public SoftwareSerial {
  public:
  // seem to need explicit constructor to avoid error
  //USsensor();   // : SoftwareSerial(TX, RX) {};
  USsensor(int RX, int TX, bool inverse);
  //int port(int TX, int RX);
  void begin(int BAUD);
  void set_dis_range(int min, int max);
  int getDistance();
  int get_last_operate_status();
  
  void _measure();
  int check_sum(unsigned char l[]);
  int _check_out_limit(int distance);
  int _check_low_limit(int distance);

  int distance_max;
  int distance_min;
  int range_max;
  int last_operate_status;
  int distance;
  int sum;
  SoftwareSerial ser;
  private:
  // Array to hold data from serial port
  // Byte[0] = 0xFF if valid data 
  // Byte[1] = high byte of distance
  // Byte[2] = low byte of distance
  // Byte[3] = checksum
  
  unsigned char data[4]={};
  
};

#endif
