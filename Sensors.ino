void readSonar(){
  altitude2=analogRead(0);
}

void calcRateofRotation(){
  
  
  if ((millis()-prevtime)>250){
    long timediff=millis()-prevtime;
    //Serial.print("diff=");
   // Serial.println(timediff);
    
    int headingdiff=compass2 -prevheading;
    //Serial.print("headingdiff=");
   // Serial.println(headingdiff);
    rotationrate=4*headingdiff/timediff;
    prevtime=millis();
      prevheading=compass2;
  } 
  
  
 //prevtime=currenttime;

  

}
