
//------------------CALIBRATION---------------------------
int maxRightPitch = 156, minRightPitch = 50;
int maxLeftPitch = 170, minLeftPitch = 50;
int motorAngleOffsetRight = 0, motorAngleOffsetLeft = 0;
//------------------CALIBRATION---------------------------

void startCalibration()
{
  setRPM(0);
}

void calibration()
{
  transferJoystickToPitches();
  setTilt(c_slider);

  if(c_joy_fire_index) //We only apply the configuration values if fire is hit
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
}

void stopCalibration()
{
  storeCalibration();
}



