/*
  Youkaiwatch hack project
*/
#include <RCS620S.h>
#include "Wire.h"
#include "medal.h"
#define COMMAND_TIMEOUT 400

int ms = 10;

int pin[4] = {2, 3, 4, 5};

RCS620S rcs620s;

//ここから妖怪ウォッチデータ　QRコードを下にした状態で凹凸を読む　凹の場合は0　凸の場合は1
//規則性
//最初の段の両端は必ず1になるようになっている。
//最後の段は必ず1 0 1 0になる

Medal medal142()
{
  Medal m = {
      B00001001,
      B00001011,
      B00001111,
      B00001010,
  };
  return m;
}

Medal medaltest()
{
  Medal m = {
      B00001101,
      B00001000,
      B00001110,
      B00001010,
  };

  return m;
}

Medal medalbouketu()
{
  Medal m = {
      B00001001,
      B00001101,
      B00001110,
      B00001010,
  };

  return m;
}

Medal medalError()
{
  Medal m = {
      B00001001,
      B00000110,
      B00000110,
      B00001010,
  };

  return m;
}

// pinsetup
void setup()
{
  int ret;
  pinMode(LED_BUILTIN, OUTPUT);

  seti2c();
  randomSeed(analogRead(0));

  Serial.begin(115200); // for RC-S620/S

  // initialize RC-S620/S
  ret = rcs620s.initDevice();

  while (!ret)
  {
  }
}

void seti2c()
{
  Wire.begin();

  Wire.beginTransmission(0x20);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(0x20);
  Wire.write(0x01);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(0x21);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(0x21);
  Wire.write(0x01);
  Wire.write(0x00);
  Wire.endTransmission();
}

void DigitalOutC1PortA(int i)
{
  Wire.beginTransmission(0x20);
  Wire.write(0x12);
  Wire.write(i);
  Wire.endTransmission();
}

void DigitalOutC1PortB(int i)
{
  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.write(i);
  Wire.endTransmission();
}

void DigitalOutC2PortA(int i)
{
  Wire.beginTransmission(0x21);
  Wire.write(0x12);
  Wire.write(i);
  Wire.endTransmission();
}

void DigitalOutC2PortB(int i)
{
  Wire.beginTransmission(0x21);
  Wire.write(0x13);
  Wire.write(i);
  Wire.endTransmission();
}

void DigitalOutAllOn()
{
  Wire.beginTransmission(0x20);
  Wire.write(0x12);
  Wire.write(255);
  Wire.endTransmission();

  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.write(255);
  Wire.endTransmission();

  Wire.beginTransmission(0x21);
  Wire.write(0x12);
  Wire.write(255);
  Wire.endTransmission();

  Wire.beginTransmission(0x21);
  Wire.write(0x13);
  Wire.write(255);
  Wire.endTransmission();
}

void DigitalOutAllOff()
{
  Wire.beginTransmission(0x20);
  Wire.write(0x12);
  Wire.write(0);
  Wire.endTransmission();

  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.write(0);
  Wire.endTransmission();

  Wire.beginTransmission(0x21);
  Wire.write(0x12);
  Wire.write(0);
  Wire.endTransmission();

  Wire.beginTransmission(0x21);
  Wire.write(0x13);
  Wire.write(0);
  Wire.endTransmission();
}
#define WILD_SYSTEM_CODE 0xFFFF
#define COMMON_SYSTEM_CODE 0xFE00

void loop()
{
  uint8_t buf[30];
  int ret;
  rcs620s.timeout = COMMAND_TIMEOUT;
  if (rcs620s.polling(WILD_SYSTEM_CODE))
  {
    PlayRandom();
  }
}
ResetMedal();
rcs620s.rfOff();
delay(10);
}

void PlayMedal(Medal m)
{
  digitalWrite(LED_BUILTIN, HIGH);
  SendSignal(m.signal0);
  SendSleepSignal();
  SendSignal(m.signal1);
  SendSleepSignal();
  SendSignal(m.signal2);
  SendSleepSignal();
  SendSignal(m.signal3);
  SendSleepSignal();
  //再生待機
  delay(14000);
  digitalWrite(LED_BUILTIN, LOW);
}

void PlayMedalError()
{
  digitalWrite(LED_BUILTIN, HIGH);
  Medal m = medalError();
  SendSignal(m.signal0);
  SendSleepSignal();
  SendSignal(m.signal1);
  SendSleepSignal();
  SendSignal(m.signal2);
  SendSleepSignal();
  SendSignal(m.signal3);
  SendSleepSignal();
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);
}

void SendSignal(int s)
{
  DigitalOutC1PortB(s);
  delay(ms);
}

void SendSleepSignal()
{
  DigitalOutAllOff();
  delay(ms);
}

void ResetMedal()
{
  DigitalOutAllOn();
  delay(ms);
  DigitalOutAllOff();
}

int temp = 0;
void PlayRandom()
{
  int randNumber = random(0, 3);
  if (temp == randNumber)
  {
    int randNumber = random(0, 3);
  }
  temp = randNumber;

  switch (randNumber)
  {
  case 0:
    PlayMedal(medal142());
    break;
  case 1:
    PlayMedal(medaltest());
    break;
  case 2:
    PlayMedal(medalbouketu());
    break;
  }
}
