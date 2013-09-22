/*
written september 2013

This sketch is meant to run onboard Flagada,
on a 3.3V Arduino Pro Mini,
using an XBEE on channel 1911 BAUD 19200

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

#include <ValueSync.h> //library written by Maximillian Ernestus to synchronise values between diffeerent arduino's
#include <EEPROM.h>
#include <Servo.h>

Servo pitch_l, rpm_l, angle_l; //servo definitions
Servo pitch_r, rpm_r, angle_r;

ValueReceiver<9> receiver;  //number of values to be synched

//Here we sotre the current values
int c_joy_x = 512;
int c_joy_y = 512;

int c_joy_fire_index;
int c_joy_fire_thumb;
int c_slider = 512; 

int c_pot_l = 512;
int c_pot_r = 0;

int c_switch_l;
int c_switch_r;

int maxRightPitch = 156, minRightPitch = 50;
int maxLeftPitch = 170, minLeftPitch = 50;

int motorAngleOffsetRight = 0, motorAngleOffsetLeft = 0;





void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(19200);
  //Wire.begin();
  pitch_l.attach(2);
  rpm_l.attach(4);
  angle_l.attach(3);

  pitch_r.attach(7);
  rpm_r.attach(9);
  angle_r.attach(8);


  receiver.addValue(&c_joy_x); //adds this variable to the synchronisation system
  receiver.addValue(&c_joy_y); //the & means you pass the adress to the variable, not the value of the variable

  receiver.addValue(&c_joy_fire_index);
  receiver.addValue(&c_joy_fire_thumb);
  receiver.addValue(&c_slider); 

  receiver.addValue(&c_pot_l);
  receiver.addValue(&c_pot_r);

  receiver.addValue(&c_switch_l);
  receiver.addValue(&c_switch_r);

  rpm_l.write(90);
  rpm_r.write(90);


  maxRightPitch = EEPROM.read(0); // reads 1 byte from EPROM for stored trimming values
  minRightPitch = EEPROM.read(1);
  maxLeftPitch = EEPROM.read(2);
  minLeftPitch = EEPROM.read(3);
  motorAngleOffsetRight = EEPROM.read(4);
  motorAngleOffsetLeft = EEPROM.read(5);

}

int pitch_angle_l;
int pitch_angle_r;

void loop()
{
  readSomeValues();  //this synchs values zith other arduino

  //c_heading = compass.getHeading();
  int rpm = map(c_pot_r, 0, 1024, 89, 140);
  if(!c_joy_fire_index)
    rpm = 89; //deadmanswitch

  if(receiver.timeSinceLastMessage() > 300)
    rpm = 89; //transmitter failsafe

  //d_heading = c_heading;
  if(false) //Auto Pilot
  {
    /*
    headingPID.SetMode(AUTOMATIC);      
     
     d_heading += map(c_joy_x, 0, 1024, -2, 2);
     
     headingPID.SetTunings(c_slider/10.0, c_pot_l/10.0, 0);
     headingPID.Compute();
     
     computePitches(controlX, 0);
     applyPitches();
     rpm_l.write(rpm);
     rpm_r.write(rpm);
     */
     

  }
  else //Manual Control
  {
    //headingPID.SetMode(MANUAL);
    computePitches(c_joy_x, c_joy_y);

    if((c_switch_r && c_switch_l)) //Normal mode
    {

      applyPitches();

      rpm_l.write(rpm);
      rpm_r.write(rpm);
      applyMotorAngle();
    }
    else //Trim mode
    {
      while(!(c_switch_r && c_switch_l))
      {
        readSomeValues();
        computePitches(c_joy_x, c_joy_y);
        applyMotorAngle();

        if(c_joy_fire_index)
        {
          if(!c_switch_r && !c_switch_l) //Trim the motor angle servos using right and left potmeter
          {
            motorAngleOffsetRight = map(c_pot_r, 0, 1024, -90, 90);
            motorAngleOffsetLeft = map(c_pot_l, 0, 1024, -90, 90);
          }
          else if(!c_switch_r) //Trim the right blade pitch. You set the min angle with the left pot and max angle with the right pot
          {
            minRightPitch = map(c_pot_l, 0, 1024, 0, 180);
            maxRightPitch = map(c_pot_r, 0, 1024, 0, 180);
          }
          else if(!c_switch_l) //Trim the left blade pitch. You set the min angle with the left pot and max angle with the right pot
          {
            minLeftPitch = map(c_pot_l, 0, 1024, 0, 180);
            maxLeftPitch = map(c_pot_r, 0, 1024, 0, 180);
          }
        }

        applyPitches();

        rpm = 89;
        rpm_l.write(rpm);
        rpm_r.write(rpm);
      }

      EEPROM.write(0, maxRightPitch);
      EEPROM.write(1, minRightPitch);
      EEPROM.write(2, maxLeftPitch);
      EEPROM.write(3, minLeftPitch);
      EEPROM.write(4, motorAngleOffsetRight);
      EEPROM.write(5, motorAngleOffsetLeft);
    }
  }

  delay(10);

}

void computePitches(int controlX, int controlY)
{
  int differential = map(controlX, 0, 1024, -15, 15);
  int pitch = map(controlY, 0, 1024, -45, 45);
  
  differential *= -1;

  pitch_angle_l = constrain(pitch + differential, -45, 45);
  pitch_angle_r = constrain(pitch - differential, -45, 45);


}

void applyPitches()
{
  pitch_r.write(map(pitch_angle_l, -45, 45, minRightPitch, maxRightPitch));
  pitch_l.write(map(pitch_angle_r, -45, 45, minLeftPitch, maxLeftPitch));
}

void applyMotorAngle()
{
  int motorAngle = map(c_slider, 0, 1024, 0, 180);
  angle_l.write(constrain(motorAngle + motorAngleOffsetLeft, 0, 180));
  angle_r.write(constrain(180 - (motorAngle + motorAngleOffsetRight), 0, 180));
}

void readSomeValues()
{
  int valuesRead = 0;
  while(Serial.available() > 3)
  {
    receiver.receiveValue();
  }
}
