#include <Deuligne.h>
#include "message.hpp"

/*enum MessageType {
  MSGTYPE_READY = 0,
  MSGTYPE_SET_BPM = 2,
};

struct Message {
  unsigned int type, arg1, arg2, arg3;
};*/

Deuligne lcd;

void sendMessage(unsigned int type, unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
  byte buf[8];
  buf[0] = type / 256;
  buf[1] = type % 256;

  buf[2] = arg1 / 256;
  buf[3] = arg1 % 256;

  buf[4] = arg2 / 256;
  buf[5] = arg2 % 256;

  buf[6] = arg3 / 256;
  buf[7] = arg3 % 256;
  
  Serial.write(buf, 8);
}

Message receiveMessage() {
  byte buf[8];
  Serial.readBytes(buf, 8);

  Message msg;
  msg.type = buf[0] * 256 + buf[1];
  msg.arg1 = buf[2] * 256 + buf[3];
  msg.arg2 = buf[4] * 256 + buf[5];
  msg.arg3 = buf[6] * 256 + buf[7];

  return msg;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sendMessage(MSGTYPE_READY, 0, 0, 0);
  lcd.init();
  lcd.print("BPM:");
}

void setBpm(unsigned int bpm)
{
  lcd.setCursor(4, 0);
  lcd.print(bpm);  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() >= 8) {
    Message msg = receiveMessage();
    if (msg.type == MSGTYPE_SET_BPM) {
      setBpm(msg.arg1);
    }
  }
  /* static int8_t oldkey = -1;
  static int8_t key = lcd.get_key();
  if (key != oldkey) {
    oldkey = key;
    if (key >= 0) {
      // sendMessage(MSGTYPE_READY, 1, 2, 3);
    }
  }*/
}

void serialEvent() {
}

