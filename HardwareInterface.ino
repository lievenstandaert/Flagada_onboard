//------------------HARTDWARE_INTERFACE-------------------
Servo pitch_l, rpm_l, angle_l; //servo definitions
Servo pitch_r, rpm_r, angle_r;
//------------------HARTDWARE_INTERFACE-------------------


//#################__RPM__########################
//rpm ranges are from 0 to 1024
int rpm_right = 89;
int rpm_left = 89;

void hardwareSetup()
{
  pitch_l.attach(2);
  rpm_l.attach(4);
  angle_l.attach(3);

  pitch_r.attach(7);
  rpm_r.attach(9);
  angle_r.attach(8);
}

void setLeftRPM(int rpm)
{
  rpm_left = map(rpm, 0, 1024, 89, 130);
}

void setRightRPM(int rpm)
{
 rpm_right = map(rpm, 0, 1024, 89, 130);
}

void setRPM(int rpm)
{
  setLeftRPM(rpm);
  setRightRPM(rpm);
}

void applyRPM()
{ 
  if(c_joy_fire_index && receiver.timeSinceLastMessage() < 300)
  {
    rpm_l.write(rpm_left);
    rpm_r.write(rpm_right);
  }
  else
  {
    rpm_l.write(89);
    rpm_r.write(89);
  }
}
//#################__RPM__########################


//#################__PITCH__######################
//pitch ranges are from -45 to 45
int pitch_angle_l = 0, pitch_angle_r = 0;

void setLeftPitch(int angle)
{
  pitch_angle_l = angle;
}

void setRightPitch(int angle)
{
  pitch_angle_r = angle;
}

void setPitches(int leftAngle, int rightAngle)
{
  setLeftPitch(leftAngle);
  setRightPitch(rightAngle);
}

void applyPitch()
{
  pitch_r.write(map(pitch_angle_r, -45, 45, minRightPitch, maxRightPitch));
  pitch_l.write(map(pitch_angle_l, -45, 45, minLeftPitch, maxLeftPitch));
}
//#################__PITCH__######################


//#################__TILT__#######################
//Tilt range is from -90 to 90
int tilt_l = 0, tilt_r = 0;

void setLeftTilt(int tilt)
{
  tilt_l = tilt;
}

void setRightTilt(int tilt)
{
  tilt_r = tilt;
}

void setTilt(int tilt)
{
  tilt_l = tilt;
  tilt_r = tilt;
}

void applyTilt()
{
  angle_l.write(constrain((tilt_l + 90) + motorAngleOffsetLeft, 0, 180));
  angle_r.write(constrain(180 - ((tilt_r + 90) + motorAngleOffsetRight), 0, 180));
}

