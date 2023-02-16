void setup(){
    pinMode(D1,OUTPUT);
}
 
void loop(){
    tone(D1,220,125);
    delay(125);
    tone(D1,2093,250);
    delay(250);
    tone(D1,82,125);
    delay(125);

//digitalWrite(D1, HIGH);
//delay(1000);
//digitalWrite(D1, LOW);
//delay(1000);

}
