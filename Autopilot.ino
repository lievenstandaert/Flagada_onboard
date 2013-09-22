
void startAuto()
{
}





void engageAutopilot(){
  if (c_joy_fire_thumb==1 && c_prev_joy_fire_thumb==0){
    

    if (automode==0){
      automode=1; 
      digitalWrite(13,HIGH);
      compassheading=compass2;
      tone(5,400,200);
    } 
    else{
      automode=0;
      digitalWrite(13,LOW);
       tone(5,400,200);
    }

  }
  c_prev_joy_fire_thumb=c_joy_fire_thumb;
}






void autoControl()
{
  autopilot();
  setRPM(c_pot_r);
  setTilt(map(c_slider, 0, 1024, -90, 90)); 
}

void autopilot(){

  int desired_rotation= 0; // zero for CCW, one for CW

  int compassdiff1= abs(compass2-compassheading);
  int compassdiff2=abs(compass2+360-compassheading);
  int compassdiff3=abs(compass2-360-compassheading);

  int compassdiff=min(compassdiff1,compassdiff2);
  compassdiff= min(compassdiff, compassdiff3);


  if (compassheading > compass2 && (compassheading -compass2)<180){
    desired_rotation= 1;
    compassdiff=(compassheading-compass2);
  }
  if (compassheading > compass2 && (compassheading -compass2)>180){
    desired_rotation= 0;
    compassdiff=360-(compassheading-compass2);
  }

  if (compassheading < compass2 && (compass2 -compassheading)<180){
    desired_rotation= 0;
    compassdiff=compass2-compassheading;
  }
  if (compassheading < compass2 && (compass2 -compassheading)>180){
    desired_rotation= 1;
    compassdiff=360-(compass2-compassheading);
  }

  if (desired_rotation=0){
    compassdiff=map(compassdiff,0,180,512,0);
  }
  else{  
    compassdiff=map(compassdiff,0,180,512,1024);  
  }
  transferControlToPitchesAuto(compassdiff, c_joy_y  );
}


void transferControlToPitchesAuto(int controlX, int controlY)
{
  int differential = map(controlX, 0, 1024, -30, 30);
  int pitch = map(controlY, 0, 1024, -45, 45);

  int pitch_angle_l = constrain(pitch + differential, -45, 45);
  int pitch_angle_r = constrain(pitch - differential, -45, 45);
  setPitches(pitch_angle_l, pitch_angle_r);
}


