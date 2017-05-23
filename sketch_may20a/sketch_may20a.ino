#include <Arduino.h>
char out[32];
int n=0;
void setup() {
  // put your setup code here, to run once:
  out[0]='\0';
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    //Serial.read();
    //Serial.write('|');
}
int readChar(char*dest)
{
  //int start=1;
  char incoming=0;
  while(incoming==0)
  {
  if(Serial.available())break;
  //delay(1);
  }
  int i=0;
  incoming='|';
  while(incoming!=0)
  {
    if(i>=31)
    {
      //dest[30]='|';
      dest[31]='\0';
      return 0;
    }
    
    if(Serial.available())
    {
      incoming = Serial.read();
      if(incoming=='|')break;
      dest[i]=incoming;
      //Serial.println(&incoming);
      i++;
    }
  }
  //dest[i]='|';
  dest[i]='\0';
  return 1;
}
void loop() {
  // put your main code here, to run repeatedly:
  int res=readChar(out);
  //delay(10000);
  //Serial.println(out);
  delay(1);
  if(res>0)
  {
    if(out[0]=='c'&&out[1]=='\0')
    {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000); 
      Serial.write('o');// wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(1000);
      Serial.write("k\r\n");     
    }
    else
    {
          if(out[0]=='s')Serial.println(n);
          else
          {
            Serial.println("nocommandsinbot");
          }
    }
  }
  else
  {
    Serial.println("mistake|");
  }
  //delay(100);
  n++;
  //char h[3]="ih\0";
  //Serial.write("i|");
}
