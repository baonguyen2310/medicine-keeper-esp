#include <TM1637.h> //thêm thư viện tm1637 sheed studio
//Look for NTPClient by Fabrice Weinberg


int CLK = D7;
int DIO = D8;

TM1637 tm(CLK, DIO);

int modeD1 = 0;
int modeD2 = 0;
int prevD1 = 0; //Trước đó D1 có nhấn hay không
int prevD2 = 0;
int prevD3 = 0;
int prevD4 = 0;

int setHour = 0;
int setMinute = 0;

//Giờ đã xác nhận hẹn cho 3 buổi
int hour1 = 0;
int minute1 = 0;
int hour2 = 0;
int minute2 = 0;
int hour3 = 0;
int minute3 = 0;

int deleted1 = 0; //đã xóa
int deleted2 = 0;
int deleted3 = 0;

//Giờ hiện tại lấy từ internet
int hourNow = 0;
int minuteNow = 0;


void setup() {
  pinMode(D1, INPUT); //chuyển mode theo buổi
  pinMode(D2, INPUT); //chuyển mode chọn giờ, phút, xác nhận
  pinMode(D3, INPUT); //tăng hoặc xóa
  pinMode(D4, INPUT); //tắt thông báo
  pinMode(D5, OUTPUT); //loa
  pinMode(D6, OUTPUT); //đèn

  tm.init(); //khởi động mặt đồng hồ
  tm.set(2); //độ sáng
}

void loop() {

tm.display(0, 2); //chia lấy nguyên
    tm.display(1,5); //chia lấy dư
    tm.point(1); //dấu hai chấm
    tm.display(2,0);
    tm.display(3, 5);
  
  //CHUYỂN MODE
  if ((digitalRead(D1) == HIGH) && (prevD1 == 0)){  //Trước đó không nhấn, bây giờ nhấn mới tính
    prevD1 = 1;
    modeD1 = modeD1 + 1;
    if (modeD1 == 4){
      modeD1 = 0;
    }
  }
  else if ((digitalRead(D1) == LOW)){
    prevD1 = 0;
  }

  //CHUYỂN CHẾ ĐỘ GIỜ PHÚT XÁC NHẬN
  if ((digitalRead(D2) == HIGH) && (prevD2 == 0)){  //Trước đó không nhấn, bây giờ nhấn mới tính
    prevD2 = 1;
    modeD2 = modeD2 + 1;
    if (modeD2 == 4){
      modeD2 = 0;
    }
  }
  else if ((digitalRead(D2) == LOW)){
    prevD2 = 0;
  }

  //TĂNG GIỜ PHÚT
  if ((digitalRead(D3) == HIGH) && (prevD3 == 0) && (modeD2 == 1) && (modeD1 != 0)){  //Trước đó không nhấn, bây giờ nhấn mới tính
    prevD3 = 1;
    setHour = setHour + 1;
    if (setHour == 24){
      setHour = 0;
    }
  }
  else if ((digitalRead(D3) == LOW)){
    prevD3 = 0;
  }

  if ((digitalRead(D3) == HIGH) && (prevD3 == 0) && (modeD2 == 2) && (modeD1 != 0)){  //Trước đó không nhấn, bây giờ nhấn mới tính
    prevD3 = 1;
    setMinute = setMinute + 5;
    if (setMinute == 60){
      setMinute = 0;
    }
  }
  else if ((digitalRead(D3) == LOW)){
    prevD3 = 0;
  }

  //XÁC NHẬN HẸN CHO 3 BUỔI
  if (modeD2 == 3) {
    //Kết hợp buổi của modeD1 và giờ phút của D3
    if (modeD1 == 1) {
      hour1 = setHour;
      minute1 = setMinute;
      deleted1 = 0;
    }
    else if (modeD1 == 2){
      hour2 = setHour;
      minute2 = setMinute;
      deleted2 = 0;
    }
    else if (modeD1 == 3){
      hour3 = setHour;
      minute3 = setMinute;
      deleted3 = 0;
    }
  }

  //SO SÁNH GIỜ HẸN VỚI GIỜ HIỆN TẠI
  if ((hour1 == hourNow) && (minute1 == minuteNow) && (deleted1 == 0)){
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
  }
  if ((hour2 == hourNow) && (minute2 == minuteNow) && (deleted2 == 0)){
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
  }
  if ((hour3 == hourNow) && (minute3 == minuteNow) && (deleted3 == 0)){
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
  }

  //XÓA HẸN GIỜ
  if ((digitalRead(D3) == HIGH) && (prevD3 == 0) && (modeD2 == 0)){ 
    if (modeD1 == 1) {
      deleted1 = 1;
    }
    else if (modeD1 == 2) {
      deleted2 = 1;
    }
    else if (modeD1 == 3) {
      deleted3 = 1;
    }
  }
  else if ((digitalRead(D3) == LOW)){
    prevD3 = 0;
  }

  //TẮT THÔNG BÁO (ĐÃ UỐNG THUỐC)
  if ((digitalRead(D4) == HIGH) && (prevD4 == 0)){  //Trước đó không nhấn, bây giờ nhấn mới tính
    prevD4 = 1;
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
  }
  else if ((digitalRead(D4) == LOW)){
    prevD4 = 0;
  }

  //HIỂN THỊ ĐỒNG HỒ
  if (modeD1 == 0){
    tm.display(0, hourNow / 10); //chia lấy nguyên
    tm.display(1, hourNow % 10); //chia lấy dư
    tm.point(1); //dấu hai chấm
    tm.display(0, minuteNow / 10);
    tm.display(1, minuteNow % 10);
  }
  else if (modeD1 != 0){
    tm.display(0, setHour / 10); //chia lấy nguyên
    tm.display(1, setHour % 10); //chia lấy dư
    tm.point(1); //dấu hai chấm
    tm.display(0, setMinute / 10);
    tm.display(1, setMinute % 10);
  }


  //LẤY THỜI GIAN TỪ INTERNET
  //THÔNG BÁO KHI QUÁ 30 PHÚT CHO ĐIỆN THOẠI
  
}
