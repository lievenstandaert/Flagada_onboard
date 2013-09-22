/*
written september 2013
 
 This sketch is meant to run onboard Flagada,
 on a 3.3V Arduino Pro Mini,
 using an XBEE on channel 1911 BAUD 19200 DL 0xFFFF (broadcast mode) MY (adress) 42
 
 -Values from the remote are being read and trimming values are stored in EPROM
 
 +++JOYSTICK BUTTONS+++
 IF both switches are in up position then
 Right pot = throttle, trigger is deadmanswitch
 Joystick Y controls rotorpitch
 Joystick X controls difference in rotorpitch
 Slider controls rotortilt
 
 IF left switch is down
 then left and right pots set min and max values for left pitchservo
 The values are applied when you hit fire
 
 IF right switch is down
 then left and right pots set min and max values for right pitchservo
 The values are applied when you hit fire
 
 
 IF both switches are down
 then left and right pots controil the offset for left and right rotortilt
 The values are applied when you hit fire
 
 +++++++++++++++++++++++++
 
 
 */

#include <ValueSync.h> //library written by Maximillian Ernestus to synchronise values between different arduino's
#include <EEPROM.h>
#include <Servo.h>


//------------------COMMUNICATION-------------------------
ValueReceiver<9> receiver;  //number of values to be synched with joystick
ValueSender<4> sender;  //number of values to be synched with datamonitor
//------------------COMMUNICATION-------------------------


//------------------REMOTE_CONTROL------------------------
int c_joy_x = 512;
int c_joy_y = 512;

int c_joy_fire_index;
int c_joy_fire_thumb;
int c_slider = 512; 

int c_pot_l = 512;
int c_pot_r = 0;

int c_switch_l=1;
int c_switch_r=1;
//------------------REMOTE_CONTROL------------------------


#define MANUAL_MODE 0
#define CALIBRATION_MODE 1

int currentMode = MANUAL_MODE;

//Piezo speaker on pin 5
//Use of the tone() function will interfere with PWM output on pins 3 and 11 (on boards other than the Mega).
int speakerpin=5;
long speakertimer=0;
long speakerprevtimer=0;

int rpm2=0;


void setup()
{

  pinMode(13, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(19200);

  receiver.addValue(&c_joy_x); //adds this variable to the synchronisation system
  receiver.addValue(&c_joy_y); //the & means you pass the adress to the variable, not the value of the variable

  receiver.addValue(&c_joy_fire_index);
  receiver.addValue(&c_joy_fire_thumb);
  receiver.addValue(&c_slider); 

  receiver.addValue(&c_pot_l);
  receiver.addValue(&c_pot_r);

  receiver.addValue(&c_switch_l);
  receiver.addValue(&c_switch_r);


  sender.addValue(&currentMode);
  sender.addValue(&c_switch_l);
  sender.addValue(&c_switch_r);

  hardwareSetup();

  loadCalibration();
}



void loop()
{
  readSomeValues();  //this synchs values zith other arduino

  int newMode = MANUAL_MODE;

  if(!c_switch_l || !c_switch_r)
    newMode = CALIBRATION_MODE;


  if(newMode != currentMode)
  {
    stopMode(currentMode);
    startMode(newMode);
  }

  currentMode = newMode;

  runMode(currentMode);
  
  applyRPM();
  applyPitch();
  applyTilt();

/*
  sender.syncValues();
  if(sender.timeSinceLastMessage() > 1000)
  {
    sender.sendAllValues();
    Serial.println();
  }
  */

  delay(10);
}

void startMode(int mode)
{
  switch(mode)
  {
  case MANUAL_MODE:
    startManual();
    break;
  case CALIBRATION_MODE:
    startCalibration();
    break;
  }
}

void stopMode(int mode)
{
  switch(mode)
  {
  case MANUAL_MODE:
    stopManual();
    break;
  case CALIBRATION_MODE:
    stopCalibration();
    break;
  }
}

void runMode(int mode)
{
  switch(mode)
  {
  case MANUAL_MODE:
    manualControl();
    break;
  case CALIBRATION_MODE:
    calibration();
    break;
  }
}

void readSomeValues()
{
  int valuesRead = 0;
  while(Serial.available() > 3)
  {
    receiver.receiveValue();
  }
}

void speakeralarm(){

  if (millis()>speakertimer+2000){
    tone(speakerpin, 250, 100);
    delay(100);
    tone(speakerpin, 500, 300);
    speakertimer=millis();
  }
}


