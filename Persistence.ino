void loadCalibration()
{
  maxRightPitch = EEPROM.read(0); // reads 1 byte from EPROM for stored trimming values
  minRightPitch = EEPROM.read(1);
  maxLeftPitch = EEPROM.read(2);
  minLeftPitch = EEPROM.read(3);
  motorAngleOffsetRight = EEPROM.read(4);
  motorAngleOffsetLeft = EEPROM.read(5);
}

void storeCalibration()
{
  EEPROM.write(0, maxRightPitch);
  EEPROM.write(1, minRightPitch);
  EEPROM.write(2, maxLeftPitch);
  EEPROM.write(3, minLeftPitch);
  EEPROM.write(4, motorAngleOffsetRight);
  EEPROM.write(5, motorAngleOffsetLeft);
}

