
void startManual()
{
}

void manualControl()
{
  transferJoystickToPitches();
  setRPM(c_pot_r);
  setTilt(map(c_slider, 0, 1024, -90, 90)); 
}

void transferControlToPitches(int controlX, int controlY)
{
  int differential = map(controlX, 0, 1024, -30, 30);
  int pitch = map(controlY, 0, 1024, -45, 45);

  int pitch_angle_l = constrain(pitch + differential, -45, 45);
  int pitch_angle_r = constrain(pitch - differential, -45, 45);
  setPitches(pitch_angle_l, pitch_angle_r);
}

void transferJoystickToPitches()
{
  transferControlToPitches(c_joy_x, c_joy_y);
}

void stopManual()
{

}


