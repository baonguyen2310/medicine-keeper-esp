void setup() {
  // put your setup code here, to run once:
  pinMode(D4, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(D4)); //bấm là 0, nhả là 1, D4 có sẵn điện trở kéo, nối trực tiếp
}
