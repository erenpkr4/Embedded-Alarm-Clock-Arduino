#include <LiquidCrystal.h>


const int rs=12, en=11, d5=6, d6=5, d7=4, d8=3;
LiquidCrystal lcd(rs,en,d5,d6,d7,d8);

const int buzzer = 2;
const int pushButton1 = 10;
const int pushButton2 = 9;
const int pushButton3 = 8;
const int pushButton4 = 7;
const int pushButton5 = 13;

int buttonState1;
int buttonState2;
int buttonState3;
int buttonState4;

int mode = 0;

int degree = 0;
float realDegree = 0.0;

int isAlarm = 0;
int fahr = 0;
int mode24 = 0;

int hh = 12;
int mm = 59;
int ss = 55;
int mls = 0;
int lastTime = 0;
int now;
int ampm = 0;

int hhAlarm = 1;
int mmAlarm = 0;
int ssAlarm = 0;
int ampmAlarm = 0;

int blinkState = 0;

///////////////////////////////SETUP///////////////////////
void setup()
{
  lcd.begin(16,2);
  pinMode(pushButton1, INPUT);
  pinMode(pushButton2, INPUT);
  pinMode(pushButton3, INPUT);
  pinMode(pushButton4, INPUT);
  pinMode(pushButton5, INPUT);
  pinMode(buzzer, OUTPUT);
  
  Serial.begin(9600);
  
}

//////////////////////////LOOP//////////////////////////////
void loop()
{
   
  if(mode==0)
  {
    if(mode24==0)
    {
      clock();
      printDegree();
      if(digitalRead(pushButton5)==1)
         {
           noTone(buzzer);
           mmAlarm = mmAlarm+5;
         }
    }
    
    else if(mode24==1)
    {
      clock24();
      printDegree();
      if(digitalRead(pushButton5)==1)
         {
           noTone(buzzer);
           mmAlarm = mmAlarm+5;
         }
    }

  }
  
  else if(mode==1)
  {
    setTime();
  }
  
  else if(mode==2)
  {
    setAlarm();
  }
  
  else if(mode==3)
  {
    if(mode24==0){mode24=1;}
    else if(mode24==1){mode24=0;}
    mode = 0;
    lcd.clear();
  }
  
  else if(mode==4)
  {
    if(fahr==0){fahr=1;}
    else if(fahr==1){fahr=0;}
    mode = 0;
  }
  
}

////////////////////CHECK FOR STATE UPDATE/////////////////
void checkState(){
  buttonState1=digitalRead(pushButton1);
  buttonState2=digitalRead(pushButton2);
  buttonState3=digitalRead(pushButton3);
  buttonState4=digitalRead(pushButton4);
  if(buttonState1|buttonState2|buttonState3|buttonState4) {
     if(buttonState1==1){
       mode=1;
     }
     if(buttonState2==1){
       mode=2;
     }
     if(buttonState3==1){
       mode=3;
     }
     if(buttonState4==1){
       mode=4;
     }
    }
     else {
       mode=0;
    }
}

/////////////////////////PRINT DEGREE/////////////////////////
void printDegree()
{
  degree = analogRead(0);
  realDegree = degree * 4.68;
  realDegree /= 1024.0;
  int finalDegree = (realDegree - 0.5) * 100;
  
  if(fahr==1){finalDegree = finalDegree*1.8 + 32;}
  
  if(finalDegree <= -10)
  {
    //lcd.clear();
    lcd.setCursor(11,0);
    lcd.print(finalDegree);
  }
  if(finalDegree <= 0 && finalDegree > -10)
  {
    //lcd.clear();
    lcd.setCursor(11,0);
    lcd.print("  ");
    lcd.setCursor(12,0);
    lcd.print(finalDegree);
  }
  if(finalDegree > 0 && finalDegree < 10)
  {
    //lcd.clear();
    lcd.setCursor(11,0);
    lcd.print("  ");
    lcd.setCursor(13,0);
    lcd.print(finalDegree);
  } 
  if(finalDegree >= 10)
  {
    //lcd.clear();
    lcd.setCursor(11,0);
    lcd.print(" ");
    lcd.setCursor(12,0);
    lcd.print(finalDegree);
  }
  if(finalDegree >=100)
  {
    //lcd.clear();
    lcd.setCursor(11,0);
    lcd.print(finalDegree);
  }
  
  lcd.setCursor(14,0);
  lcd.print(char(178));
  lcd.setCursor(15,0);
  if(fahr==0){lcd.print("C");}
  if(fahr==1){lcd.print("F");}
}

///////////////////////////////CLOCK//////////////////////////
void clock()
{
  lcd.setCursor(0,0);
  if(hh<10)lcd.print("0");
  lcd.print(hh);lcd.print(":");
  if(mm<10)lcd.print("0");
  lcd.print(mm);lcd.print(":");
  if(ss<10)lcd.print("0");
  lcd.print(ss);
  
  if(ampm==0)lcd.print("AM");
  if(ampm==1)lcd.print("PM");
  
  
  if(isAlarm==1)
  {
     lcd.setCursor(0,1);
     if(hhAlarm<10)lcd.print("0");
     lcd.print(hhAlarm);
     lcd.print(":");
     if(mmAlarm<10)lcd.print("0");
     lcd.print(mmAlarm);
     lcd.print(":");
     if(ssAlarm<10)lcd.print("0");
     lcd.print(ssAlarm);
     if(ampmAlarm==0) lcd.print("AM");
     if(ampmAlarm==1) lcd.print("PM");
     lcd.print(":ALARM");
     lcd.setCursor(0,0);
  }
  
  checkState();
  delay(200);
  runClock(&hh, &mm, &ss, &now, &lastTime, &mls, &ampm);
}

//////////////////////////////CLOCK-24//////////////////////
void clock24()
{
  lcd.setCursor(0,0);
  if(ampm==0)
  {
    if(hh<10){lcd.print("0");}
    if(hh==12){lcd.print("00");}
    else{lcd.print(hh);}
    lcd.print(":");
    if(mm<10)lcd.print("0");
    lcd.print(mm);lcd.print(":");
    if(ss<10)lcd.print("0");
    lcd.print(ss);
  }
  
  else if(ampm==1)
  {
    if(hh==12){lcd.print(hh);}
    else{lcd.print(hh+12);}
    lcd.print(":");
    if(mm<10)lcd.print("0");
    lcd.print(mm);lcd.print(":");
    if(ss<10)lcd.print("0");
    lcd.print(ss);
  }
  
  if(isAlarm==1)
  {
     lcd.setCursor(0,1);
     if(hhAlarm<10)lcd.print("0");
     lcd.print(hhAlarm);
     lcd.print(":");
     if(mmAlarm<10)lcd.print("0");
     lcd.print(mmAlarm);
     lcd.print(":");
     if(ssAlarm<10)lcd.print("0");
     lcd.print(ssAlarm);
     if(ampmAlarm==0) lcd.print("AM");
     if(ampmAlarm==1) lcd.print("PM");
     lcd.print(":ALARM");
     lcd.setCursor(0,0);
  }
  
  checkState();
  delay(200);
  runClock(&hh, &mm, &ss, &now, &lastTime, &mls, &ampm);
}


///////////////////////////RUN CLOCK FUNCTION///////////////////
void runClock(int* _hh,int* _mm, int* _ss, int* _now, int* _lastTime, int* _mls, int* _ampm)
{ 
 *_now=(millis());
 *_mls+=(*_now-*_lastTime);
 *_lastTime=*_now;

 if(*_mls >= 1000){
  *_ss=*_ss+1; 
  *_mls=0;
  }
 if(*_ss >= 60){
   *_mm=*_mm+1;
   *_ss=0;
  }
 if(*_mm >= 60){
    *_hh=*_hh+1;
   	*_mm=0;
   
      if(*_hh == 12){
    if(*_ampm==0)*_ampm=1;
    else if(*_ampm==1)*_ampm=0;
    }
  }
 
  if(*_hh>=13){
    *_hh=1;
  } 
  
  
  if(isAlarm)checkAlarm();
}

/////////////////////////////SET TIME///////////////////////////
void setTime() {
  int *main_var;
  int X, Y;
  lcd.clear();
  
  int changingValue = 1;
  while(true) 
  {
    lcd.setCursor(0,0);
  	lcd.print("Set Time");
  	lcd.setCursor(0,1);
  	if(hh<10)lcd.print("0");
  	lcd.print(hh);
  	lcd.print(":");
  	if(mm<10)lcd.print("0");
  	lcd.print(mm);
  	lcd.print(":");
  	if(ss<10)lcd.print("0");
  	lcd.print(ss);

  	if(ampm==0) lcd.print("AM");
  	if(ampm==1) lcd.print("PM");
    
   	buttonState1=digitalRead(pushButton1);
  	buttonState2=digitalRead(pushButton2);
  	buttonState3=digitalRead(pushButton3);
  	buttonState4=digitalRead(pushButton4);
    
    if(buttonState1==1){
      mode=0;
      delay(200);
      break;
    }
    if(buttonState3==1){
      if(changingValue==3)changingValue=0;
      changingValue+=1;
      delay(200);
    }
    if(changingValue==1){
      main_var = &hh;
      X = 0;
      Y = 1;
     
      if(digitalRead(pushButton2)==1){
        hh+=1;
        if(hh==13)hh=1;
        delay(200);
      }
    }
    if(changingValue==2){
      main_var = &mm;
      X = 3;
      Y = 1;
     
      if(digitalRead(pushButton2)==1){
        mm+=1;
        if(mm==60)mm=0;
        delay(200);
      }
    }
    if(changingValue==3){
      main_var = &ss;
      X = 6;
      Y = 1;
     
      if(digitalRead(pushButton2)==1){
        ss+=1;
        if(ss==60)ss=0;
        delay(200);
      }
    }
    if(buttonState4==1){
      if(ampm==0)ampm=1;
      else if(ampm==1)ampm=0;
      delay(200);
    }
    blink(*main_var,X,Y);
	runClock(&hh, &mm, &ss, &now, &lastTime, &mls, &ampm);
  }
    lcd.clear();
}


//////////////////////////SET ALARM//////////////////////
void setAlarm() {
  int *main_var;
  int X, Y;
  lcd.clear();
  
  int changingValue = 1;
  while(true) 
  {
    lcd.setCursor(0,0);
  	lcd.print("Set Alarm");
  	lcd.setCursor(0,1);
  	if(hhAlarm<10)lcd.print("0");
  	lcd.print(hhAlarm);
  	lcd.print(":");
  	if(mmAlarm<10)lcd.print("0");
  	lcd.print(mmAlarm);
  	lcd.print(":");
  	if(ssAlarm<10)lcd.print("0");
  	lcd.print(ssAlarm);

  	if(ampmAlarm==0) lcd.print("AM");
  	if(ampmAlarm==1) lcd.print("PM");
    
   	buttonState1=digitalRead(pushButton1);
  	buttonState2=digitalRead(pushButton2);
  	buttonState3=digitalRead(pushButton3);
  	buttonState4=digitalRead(pushButton4);
    
    if(buttonState1==1){
      mode=0;
      delay(200);
      break;
    }
    if(buttonState3==1){
      if(changingValue==3)changingValue=0;
      changingValue+=1;
      delay(200);
    }
    if(changingValue==1){
      main_var=&hhAlarm;
      X = 0;
      Y = 1;
     
      if(digitalRead(pushButton2)==1){
        hhAlarm+=1;
        if(hhAlarm==13)hhAlarm=1;
        delay(200);
      }
    }
    if(changingValue==2){
      main_var=&mmAlarm;
      X = 3;
      Y = 1;
     
      if(digitalRead(pushButton2)==1){
        mmAlarm+=1;
        if(mmAlarm==60)mmAlarm=0;
        delay(200);
      }
    }
    if(changingValue==3){
      main_var=&ssAlarm;
      X = 6;
      Y = 1;
     
      if(digitalRead(pushButton2)==1){
        ssAlarm+=1;
        if(ssAlarm==60)ssAlarm=0;
        delay(200);
      }
    }
    if(buttonState4==1){
      if(ampmAlarm==0)ampmAlarm=1;
      else if(ampmAlarm==1)ampmAlarm=0;
      delay(200);
    }
    blink(*main_var,X,Y);
  }
  isAlarm=1;
  lcd.clear();
  
  runClock(&hh, &mm, &ss, &now, &lastTime, &mls, &ampm);
}

//////////////////////CHECK ALARM AND BUZZER////////////////
void checkAlarm(){
  if(hh==hhAlarm & mm==mmAlarm & ss==ssAlarm & ampm==ampmAlarm){
    tone(buzzer, 1000);
    //isAlarm=false;
    lcd.clear();
  }
}


////////////////////////BLINK FUNCTION////////////////////
void blink(int main_var, int X, int Y){
  lcd.setCursor(X, Y);
  	if(blinkState == 0){ 
  	  if(main_var<10)lcd.print("0");
  	  lcd.print(main_var);
  	  delay(200);
  	}
  	if(blinkState == 1){
 	  lcd.print("  ");
 	  delay(200);
    }
  	blinkState=(blinkState+1)%2;
}
