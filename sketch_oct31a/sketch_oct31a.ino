#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <TM1637.h>

int CLK = D2;
int DIO = D3;

TM1637 tm(CLK,DIO);

const char *ssid     = "Nt GIA HUY";
const char *password = "hhhhhhhhh";

const long utcOffsetInSeconds = 7*60*60;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  
  tm.init();
  //set brightness; 0-7
  tm.set(2);
  
  timeClient.begin();

  pinMode(D1, OUTPUT);
  
}

void loop() {
  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

//  tm.display(0,timeClient.getHours()/10);
//  tm.display(1,timeClient.getHours()%10);
//  tm.point(1);
//  tm.display(2,timeClient.getMinutes()/10);
//  tm.display(3,timeClient.getMinutes()%10);

  tm.display(0,timeClient.getMinutes()/10);
  tm.display(1,timeClient.getMinutes()%10);
  tm.point(1);
  tm.display(2,timeClient.getSeconds()/10);
  tm.display(3,timeClient.getSeconds()%10);

  if (timeClient.getSeconds()%10 == 0){
    digitalWrite(D1, HIGH);
  }
  else if (timeClient.getSeconds()%10 == 5){
    digitalWrite(D1, LOW);  
  } 

  delay(1000);
}
//

//
//void setup() {
//  // put your setup code here, to run once:
//  tm.init();
//
//  //set brightness; 0-7
//  tm.set(2);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//
//  // example: "12:ab"
//  // tm.display(position, character);
//  tm.display(0,2);
//  tm.display(1,3);
//  tm.point(1);
//  tm.display(2,1);
//  tm.display(3,0);
//
//  delay(1000);
//
//  // example: "1234"
//  displayNumber(2505);
//
//  delay(1000);
//}
//
//void displayNumber(int num){   
//    tm.display(3, num % 10);   
//    tm.display(2, num / 10 % 10);   
//    tm.display(1, num / 100 % 10);   
//    tm.display(0, num / 1000 % 10);
//}
