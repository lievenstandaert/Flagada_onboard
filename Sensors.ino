void readSonar(){
  altitude2=analogRead(0);
  altitude2=(5.5/0.0098)*2.54*altitude2; //altitude in cm

  
}
