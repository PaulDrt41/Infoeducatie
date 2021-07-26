//We always have to include the library
#include "LedControl.h"
#define Xj A0
#define Yj A1
#define but 2
#define R 3
#define G 5
#define B 6

int xprec;
int yprec;
int yvalt = 4;
int xvalt = 3;
bool aprins = true;
long pzyc = 0;
long pzxc = 0;
bool state[10];
int vals[10][2]; 
int nop = 0;
int dub = 0;

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=250;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,6);
  /* and clear the display */
  lc.clearDisplay(0);
  Serial.begin(9600);
  pinMode(but, INPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
}
void dubdub(){
 byte a[8] ={B00000110, B10001001, B10011111, B11111111, B11111111, B10011111, B10001001, B00000110};
 lc.setRow(0,0,a[0]);
 lc.setRow(0,1,a[1]);
 lc.setRow(0,2,a[2]);
 lc.setRow(0,3,a[3]);
 lc.setRow(0,4,a[4]);
 lc.setRow(0,5,a[5]);
 lc.setRow(0,6,a[6]);
 lc.setRow(0,7,a[7]);
 delay(100);
 
}

void el(){
  byte a[8] ={B00000000, B01111110, B01111110, B01100000, B01100000, B01100000, B00000000, B00000000};
  lc.setRow(0,0,a[0]);
 lc.setRow(0,1,a[1]);
 lc.setRow(0,2,a[2]);
 lc.setRow(0,3,a[3]);
 lc.setRow(0,4,a[4]);
 lc.setRow(0,5,a[5]);
 lc.setRow(0,6,a[6]);
 lc.setRow(0,7,a[7]);
}
void timedGenerator()
{
  nop++;
  if (nop >5)
  {
    nop = 1;
  }

  state[nop] = 1;
  vals[nop][1] = random(7); /// valoarea pe X
  vals[nop][0] = random(7); /// valoarea pe Y
}



void circles2(int vyc,int vxc)
{
  static unsigned long long lastTime = millis();
  if(millis() - lastTime > 1000)
  {
    timedGenerator();
    lastTime = millis();
  }
  if(digitalRead(but))
  {
    for (int i = 1; i <= 5; i++)
    {
      if (state[i] && vxc == vals[i][1] && vyc == vals[i][0])
        {
          state[i] = 0;
          dub++;
          lc.setLed(0, vals[i][1], vals[i][0]+1, aprins);
    lc.setLed(0, vals[i][1]+1 , vals[i][0], aprins);
    lc.setLed(0, vals[i][1]-1, vals[i][0], aprins);
    lc.setLed(0, vals[i][1], vals[i][0]-1, aprins);
    digitalWrite(G, 30);
  delay(30);
  digitalWrite(G, LOW);
  
        }
    }
  }
  for(int i = 1; i <= 5; i++)
    lc.setLed(0, vals[i][1], vals[i][0], state[i]); 
}

void circles(int vyc,int vxc)
{
  
  if (!aprins)
  {
    pzxc = random(7);
   pzyc = random(7);
   aprins = 1;
   
  }
  lc.setLed(0, pzxc, pzyc, aprins);
  int button = digitalRead(but);
  if (button)
  {
    if(pzxc == vxc && pzyc == vyc)
  {
    lc.setLed(0, pzxc, pzyc+1, aprins);
    lc.setLed(0, pzxc+1 , pzyc, aprins);
    lc.setLed(0, pzxc-1, pzyc, aprins);
    lc.setLed(0, pzxc, pzyc-1, aprins);
    aprins = 0;
  digitalWrite(G, 30);
  delay(30);
  digitalWrite(G, LOW);
  }
  else
  {
  digitalWrite(R, 30);
  delay(30);
  digitalWrite(R, LOW);
  aprins = 0;
  }
  }
}
void loop() {
  
  lc.clearDisplay(0);
  
  int buton = digitalRead(but);
  int xval = analogRead(Xj);
  int yval = analogRead(Yj);
  Serial.print("button ");
  Serial.println(buton);
  if (yval < 500){
    yvalt++;
    if(yvalt >= 7){
      yvalt = 7;
    }
  }
  else if (yval > 600){
    yvalt -=1;
    if(yvalt <=0)
    {
      yvalt = 0;
    }
    
  }
  if (xval > 510)
  {
    xvalt -= 1;
    if(xvalt <= 0)
    {
      xvalt = 0;
      
    }
    
  }
  else if ( xval < 490){
    xvalt++;
    if (xvalt >= 7)
    xvalt = 7;
  }
  circles2(yvalt, xvalt);
  Serial.print(xvalt); Serial.print(' ');
  Serial.println(yvalt);

  lc.setLed(0, xvalt, yvalt, 1);
  xprec = xvalt;
  yprec = yvalt;
  
  if (dub == 15)
  {
    lc.clearDisplay(0);
    while(true)
    {
      lc.clearDisplay(0);
      dubdub();
      delay(500);
    }
  }
  if (dub < 15 && millis() >= 20000)
  {
    lc.clearDisplay(0);
    while(true)
    {
      lc.clearDisplay(0);
      el();
      delay(500);
    }
  }
    
 
delay(100);
}
