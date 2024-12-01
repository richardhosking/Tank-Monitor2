#include <SoftwareSerial.h>
#include "uSsensor.h"

// Constructor implicitly called when class is instantiated

//USsensor::USsensor()
//{
  // Dummy constructor
//}

USsensor::USsensor(int TX, int RX, bool inverse)
{
  distance_max = 400;
  distance_min = 30;
  last_operate_status = STA_OK;
  distance = 0;
  sum = 0;
  ser = SoftwareSerial(TX, RX);
}
// Open serial port
void USsensor::begin(int BAUD){
  ser.begin(BAUD);
  delay(200);
  if (ser.available() > 0) {
    last_operate_status = STA_OK;
  }
  else
  {
    last_operate_status = STA_ERR_SERIAL;
  }
}

void USsensor::set_dis_range(int min, int max)
{
  distance_max = max;
  distance_min = min;
}

int USsensor::getDistance()
{
  _measure();
  return distance;
}

int USsensor::check_sum(unsigned char l[])
{
  return (l[0] + l[1] + l[2])&0x00ff;
}

void USsensor::_measure()
{
  // Wait for start char 0xFF
  do{
     for(int i=0;i<4;i++)
     {
       data[i]=ser.read();
     }
  }while(ser.read()==0xff);

  ser.flush();

  if(data[0]==0xff)
  {
    sum = check_sum(data);
      if (sum != data[3])
      {
        last_operate_status = STA_ERR_CHECKSUM;
      }
      else
      {
        distance = data[1]*256 + data[2];
        last_operate_status = STA_OK;
      }
      if (distance > distance_max)
      {
        last_operate_status = STA_ERR_CHECK_OUT_LIMIT;
        distance = distance_max;
      }
      else if (distance < distance_min)
      {
        last_operate_status = STA_ERR_CHECK_LOW_LIMIT;
        distance = distance_min;
      }
    }
    else
    {
      last_operate_status = STA_ERR_DATA;
    }
  }

int USsensor::get_last_operate_status()
{
  return last_operate_status;
}