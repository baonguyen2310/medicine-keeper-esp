#include <IRremote.h> // thư viện hỗ trợ IR remote của shirrif
//http://arduino.vn/bai-viet/288-infrared-remote-control-dieu-khien-bang-hong-ngoai-voi-arduino


const int receiverPin = D1; // chân digital 8 dùng để đọc tín hiệu
IRrecv irrecv(receiverPin); // tạo đối tượng IRrecv mới
decode_results results;// lưu giữ kết quả giải mã tín hiệu

const int RED = D2;// LED đỏ
const int YELLOW = D3;// LED vàng
const int GREEN = D4;// LED xanh
/* trạng thái của các LEDs*/
boolean stateRED = false;
boolean stateYELLOW = false;
boolean stateGREEN = false;
void setup()
{
  Serial.begin(9600);// serial 
  irrecv.enableIRIn(); // start the IR receiver
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
}
// translate IR signals
void translateIR()
{
  switch(results.value)
  {
    case 0xFFA25D:  stateRED = !stateRED; //A
                digitalWrite(RED, stateRED); 
                break;
    case 0xFF629D: stateYELLOW = !stateYELLOW;  //B 
                digitalWrite(YELLOW, stateYELLOW); 
                break;
    case 0xFFE21D: stateGREEN = !stateGREEN; //C
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF22DD: stateRED = stateYELLOW = stateGREEN = false; //D
                digitalWrite(RED, 0);
                digitalWrite(YELLOW, 0);
                digitalWrite(GREEN, 0);
                break;
    case 0xFF02FD: stateGREEN = !stateGREEN; //UP
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFFC23D: stateGREEN = !stateGREEN; //E
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFFE01F: stateGREEN = !stateGREEN; //LEFT
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFFA857: stateGREEN = !stateGREEN; //SETUP
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF906F: stateGREEN = !stateGREEN; //RIGHT
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF5897: stateGREEN = !stateGREEN; //0
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF9867: stateGREEN = !stateGREEN; //DOWN
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFFB04F: stateGREEN = !stateGREEN; //F
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF30CF: stateGREEN = !stateGREEN; //1
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF18E7: stateGREEN = !stateGREEN; //2
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF7A85: stateGREEN = !stateGREEN; //3
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF10EF: stateGREEN = !stateGREEN; //4
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF38C7: stateGREEN = !stateGREEN; //5
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF5AA5: stateGREEN = !stateGREEN; //6
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF42BD: stateGREEN = !stateGREEN; //7
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF4AB5: stateGREEN = !stateGREEN; //8
                digitalWrite(GREEN, stateGREEN);  
                break;
    case 0xFF52AD: stateGREEN = !stateGREEN; //9
                digitalWrite(GREEN, stateGREEN);  
                break;          
  }
}
void loop()
{
  if (irrecv.decode(&results)) // nếu nhận được tín hiệu
  {
    translateIR();
    Serial.println(results.value, HEX);
    delay(200);
    irrecv.resume(); // nhận giá trị tiếp theo
  }
}
