#include <IRremote.hpp>

#include <NTPClient.h>    //thư viện lấy giờ từ internet
#include <ESP8266WiFi.h>  //thư viện wifi
#include <WiFiUdp.h>      //thư viện wifi
#include <TM1637.h>       //thư viện hiển thị trên mặt đồng hồ led tm1637

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>  //https

//Cài arduinojson 6
#include <Arduino_JSON.h> //dùng các hàm liên quan get (vấn đề của thư viện)
#include <ArduinoJson.h>  //dùng các hàm liên quan post

// String serverNameTime = "https://medicine-keeper-server.onrender.com/esptime";
// String serverNameTook = "https://medicine-keeper-server.onrender.com/esptook";
// String serverNameIsNotify = "https://medicine-keeper-server.onrender.com/espisnotify";
//ngrok http 5000 tunnel hoặc dùng địa chỉ ip local
// String serverNameTime = "https://blue-violet-centipede-sock.cyclic.app/esptime";
// String serverNameTook = "https://blue-violet-centipede-sock.cyclic.app/esptook";
// String serverNameIsNotify = "https://blue-violet-centipede-sock.cyclic.app/espisnotify";

String serverNameTime = "https://medicine-keeper-server-production.up.railway.app/esptime";
String serverNameTook = "https://medicine-keeper-server-production.up.railway.app/esptook";
String serverNameIsNotify = "https://medicine-keeper-server-production.up.railway.app/espisnotify";

unsigned long lastTime = 0;
unsigned long timerDelay = 60000;  //Mỗi phút get time từ server 1 lần để đồng bộ

int mode = 0;  //Chế độ hiện tại
// Có 4 chế độ:
// - 0: Hiển thị thời gian hiện tại
// - 1: Hẹn giờ buổi sáng
// - 2: Hẹn giờ buổi trưa
// - 3: Hẹn giờ buổi tối

int pointer = 0;  //con trỏ trên mặt đồng hồ (ứng với 4 vị trí:0, 1, 2, 3)

//Thời gian người dùng đang thay đổi
int setHour = 0;
int setMinute = 0;

//Thời gian đã xác nhận hẹn cho 3 buổi
int hour1 = 0;
int minute1 = 0;
int hour2 = 0;
int minute2 = 0;
int hour3 = 0;
int minute3 = 0;

//Giờ hiện tại lấy từ internet
int hourNow = 0;
int minuteNow = 0;

//Trạng thái kích hoạt hẹn giờ cho 3 buổi: false là chưa kích hoạt, true là đã kích hoạt
bool actived1 = false;
bool actived2 = false;
bool actived3 = false;

//Trạng thái hẹn giờ của 1 buổi đang thông báo
int notifying1 = 0;
int notifying2 = 0;
int notifying3 = 0;

//Trạng thái đã hoàn thành việc uống thuốc cho 3 buổi: false là chưa hoàn thành, true là đã hoàn thành
bool completed1 = false;
bool completed2 = false;
bool completed3 = false;

int SIG = D0;  //chân D0 nhận tín hiệu từ điều khiển hồng ngoại
int LOA = D1;  //chân D1 phát tín hiệu ra loa
int CLK = D2;  //chân D2 và chân D3 hiển thị mặt đồng hồ
int DIO = D3;
int AS = A0;  //chân A0 nhận tín hiệu cảm biến ánh sáng



TM1637 tm(CLK, DIO);  //biến tm điều khiển mặt đồng hồ


const char *ssid = "Nt GIA HUY";     //tên wifi
const char *password = "hhhhhhhhh";  //mật khẩu wifi

const long utcOffsetInSeconds = 7 * 60 * 60;  //chênh lệch múi giờ +7

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(115200);  //Khởi động màn hình serial ở baudrate (tốc độ truyền) 115200

  WiFi.begin(ssid, password);  //Khởi động kết nối wifi

  while (WiFi.status() != WL_CONNECTED) {  //kiểm tra trạng thái kết nối wifi
    delay(500);
    Serial.print(".");
  }

  timeClient.begin();  //bắt đầu lấy giờ từ internet

  tm.init();  //khởi động mặt đồng hồ
  tm.set(2);  //thiết lập độ sáng cho mặt đồng hồ

  IrReceiver.begin(SIG);

  pinMode(D1, OUTPUT);  //khai báo chân D1 là chân phát tín hiệu
  pinMode(D5, INPUT);
}

void loop() {
  timeClient.update();  //cập nhật giờ liên tục
  hourNow = timeClient.getHours();
  minuteNow = timeClient.getMinutes();

  displayClock(mode);  //mặt đồng hồ hiển thị theo mode

  notify();

  //getAlarm();

  // Serial.print(daysOfTheWeek[timeClient.getDay()]);
  // Serial.print(", ");
  // Serial.print(timeClient.getHours());  //in giờ hiện tại ra màn hình serial
  // Serial.print(":");
  // Serial.print(timeClient.getMinutes());
  // Serial.print(":");
  // Serial.println(timeClient.getSeconds());

  //ĐỌC TÍN HIỆU CẢM BIẾN ÁNH SÁNG
  //Serial.println(analogRead(AS)); //Khoảng dưới 200 là tối
  if (analogRead(AS) > 200) {
    if (notifying1 == 1) {  //nào đang thông báo thì tắt buổi đó
      Serial.println("Đã uống thuốc buổi sáng");
      notifying1 = 0;     //tắt đang thông báo
      completed1 = true;  //đánh dấu đã hoàn thành
      digitalWrite(LOA, LOW);
      postTook();
    }
    if (notifying2 == 1) {
      Serial.println("Đã uống thuốc buổi trưa");
      notifying2 = 0;
      completed2 = true;
      digitalWrite(LOA, LOW);
      postTook();
    }
    if (notifying3 == 1) {
      Serial.println("Đã uống thuốc buổi tối");
      notifying3 = 0;
      completed3 = true;
      digitalWrite(LOA, LOW);
      postTook();
    }
  }


  //NHẬN TÍN HIỆU TỪ ĐIỀU KHIỂN HỒNG NGOẠI
  if (IrReceiver.decode())  // nếu nhận được tín hiệu
  {
    String buttonName = translateSignal(IrReceiver.decodedIRData.decodedRawData);  //Dịch tín hiệu thành tên nút bấm
    eventButton(buttonName);
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);  //in ra mà hình sertial tín hiệu nhận được
    //delay(200);
    IrReceiver.resume();  // nhận giá trị tín hiệu tiếp theo
  }

  delay(200);
}

//Get alarm từ server và đồng bộ vào esp mỗi phút
void getAlarm() {
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
      // Ignore SSL certificate validation
      client->setInsecure();

      //WiFiClient client;
      HTTPClient https;

      String serverPath = serverNameTime + "?ESPCODE=001";

      // Your Domain name with URL path or IP address with path
      https.begin(*client, serverPath.c_str());

      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

      // Send HTTP GET request
      int httpResponseCode = https.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = https.getString();
        //Serial.println(payload);
        JSONVar myJson = JSON.parse(payload);
        if (JSON.typeof(myJson) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
        Serial.print("JSON object = ");
        Serial.println(myJson);
        Serial.println(myJson["alarm1_hour"]);
        //ĐỒNG BỘ TỪ SERVER VÀO ESP8266
        hour1 = int(myJson["alarm1_hour"]) + 7;
        if (hour1 >= 24) {
          hour1 = hour1 - 24;
        }
        minute1 = myJson["alarm1_minute"];
        hour2 = int(myJson["alarm2_hour"]) + 7;
        if (hour2 >= 24) {
          hour2 = hour2 - 24;
        }
        minute2 = myJson["alarm2_minute"];
        hour3 = int(myJson["alarm3_hour"]) + 7;
        if (hour3 >= 24) {
          hour3 = hour3 - 24;
        }
        minute3 = myJson["alarm3_minute"];
        actived1 = myJson["checked1"];
        actived2 = myJson["checked2"];
        actived3 = myJson["checked3"];

        //Chỉ có esp post trạng thái isTook, chứ esp không được get trạng thái isTook từ server
        //completed1 = myJson["isTook1"];
        //completed2 = myJson["isTook2"];
        //completed3 = myJson["isTook3"];
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      https.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

//https://arduinojson.org/v6/how-to/use-arduinojson-with-httpclient/

void postAlarm() {
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    // Ignore SSL certificate validation
    client->setInsecure();

    //WiFiClient client;
    HTTPClient https;

    https.begin(*client, serverNameTime);
    https.addHeader("Content-Type", "application/json");

    // Prepare JSON document
    DynamicJsonDocument doc(2048);
    doc["ESPCODE"] = "001";
    doc["alarm1_hour"] = hour1 - 7;
    doc["alarm1_minute"] = minute1;
    doc["alarm2_hour"] = hour2 - 7;
    doc["alarm2_minute"] = minute2;
    doc["alarm3_hour"] = hour3 - 7;
    doc["alarm3_minute"] = minute3;
    doc["checked1"] = actived1;
    doc["checked2"] = actived2;
    doc["checked3"] = actived3;

    // Serialize JSON document
    String json;
    serializeJson(doc, json);

    Serial.print(json);

    int httpResponseCode = https.POST(json);

    //int httpResponseCode = http.POST("{\"ESPCODE\":\"001\",\"alarm1_hour\":\"hour1\",\"alarm2_hour\":\"hour2\",\"alarm3_hour\":\"49.54\",\"value3\":\"1005.14\"}");

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Free resources
    https.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}


void postTook() {
  if (WiFi.status() == WL_CONNECTED) {
   std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    // Ignore SSL certificate validation
    client->setInsecure();

    //WiFiClient client;
    HTTPClient https;

    https.begin(*client, serverNameTook);
    https.addHeader("Content-Type", "application/json");

    // Prepare JSON document
    DynamicJsonDocument doc(2048);
    doc["ESPCODE"] = "001";
    doc["isTook1"] = completed1;
    doc["isTook2"] = completed2;
    doc["isTook3"] = completed3;

    // Serialize JSON document
    String json;
    serializeJson(doc, json);
    Serial.print(json);

    int httpResponseCode = https.POST(json);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    https.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void postIsNotify() {
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    // Ignore SSL certificate validation
    client->setInsecure();

    //WiFiClient client;
    HTTPClient https;

    https.begin(*client, serverNameIsNotify);
    https.addHeader("Content-Type", "application/json");

    // Prepare JSON document
    DynamicJsonDocument doc(2048);
    doc["ESPCODE"] = "001";
    doc["checked1"] = actived1;
    doc["checked2"] = actived2;
    doc["checked3"] = actived3;

    // Serialize JSON document
    String json;
    serializeJson(doc, json);
    Serial.print(json);

    int httpResponseCode = https.POST(json);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    https.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

//Dịch tín hiệu hồng ngoại ra tên nút bấm
String translateSignal(uint32_t signal) {
  switch (signal) {
    case 0xBA45FF00: return "A";
    case 0xB946FF00: return "B";
    case 0xB847FF00: return "C";
    case 0xBB44FF00: return "D";
    case 0xBF40FF00: return "UP";
    case 0xBC43FF00: return "E";
    case 0xF807FF00: return "LEFT";
    case 0xEA15FF00: return "SETTING";
    case 0xF609FF00: return "RIGHT";
    case 0xE916FF00: return "0";
    case 0xE619FF00: return "DOWN";
    case 0xF20DFF00: return "F";
    // case 0xFF30CF: return "1";
    // case 0xFF18E7: return "2";
    // case 0xFF7A85: return "3";
    // case 0xFF10EF: return "4";
    // case 0xFF38C7: return "5";
    // case 0xFF5AA5: return "6";
    // case 0xFF42BD: return "7";
    // case 0xFF4AB5: return "8";
    // case 0xFF52AD: return "9";
    default: return "UNKNOWN";
  }
}

//Xử lý sự kiện nhất nút
void eventButton(String buttonName) {
  if (buttonName == "A") {
    setHour = hour1;  //để khi hàm displayClock hiển thị lại giờ đã hẹn trước đó
    setMinute = minute1;
    mode = 1;
  } else if (buttonName == "B") {
    setHour = hour2;  //để khi hàm displayClock hiển thị lại giờ đã hẹn trước đó
    setMinute = minute2;
    mode = 2;
  } else if (buttonName == "C") {
    setHour = hour3;  //để khi hàm displayClock hiển thị lại giờ đã hẹn trước đó
    setMinute = minute3;
    mode = 3;
  } else if (buttonName == "E") {
    mode = 0;
  } else if (buttonName == "D") {
    deleteTimer(mode);
    postIsNotify();
  } else if (buttonName == "UP") {
    if (pointer == 0) {
      if (setHour / 10 < 2) {  //hàng chục của giờ phải nhỏ hơn 2 thì mới tăng
        setHour = setHour + 10;
      }
    } else if (pointer == 1) {
      if (setHour % 10 < 9) {  //hàng đơn vị của giờ phải nhỏ hơn 9 thì mới tăng
        setHour = setHour + 1;
      }
    } else if (pointer == 2) {
      if (setMinute / 10 < 5) {  //hàng chục của phút phải nhỏ hơn 5 thì mới tăng
        setMinute = setMinute + 10;
      }
    } else {
      if (setMinute % 10 < 9) {  //hàng đơn vị của phút phải nhỏ hơn 9 thì mới tăng
        setMinute = setMinute + 1;
      }
    }
  } else if (buttonName == "DOWN") {
    if (pointer == 0) {
      if (setHour / 10 > 0) {  //hàng chục của giờ phải lớn hơn 0 thì mới giảm
        setHour = setHour - 10;
      }
    } else if (pointer == 1) {
      if (setHour % 10 > 0) {  //hàng đơn vị của giờ phải lớn hơn 0 thì mới giảm
        setHour = setHour - 1;
      }
    } else if (pointer == 2) {
      if (setMinute / 10 > 0) {  //hàng chục của phút phải lớn hơn 0 thì mới giảm
        setMinute = setMinute - 10;
      }
    } else {
      if (setMinute % 10 > 0) {  //hàng đơn vị của phút phải lớn hơn 0 thì mới giảm
        setMinute = setMinute - 1;
      }
    }
  } else if (buttonName == "LEFT") {
    if (pointer > 0) {
      pointer = pointer - 1;
    }
  } else if (buttonName == "RIGHT") {
    if (pointer < 3) {
      pointer = pointer + 1;
    }
  } else if (buttonName == "SETTING") {
    if (mode == 1) {
      hour1 = setHour;
      minute1 = setMinute;
      completed1 = false;
      actived1 = true;
      postAlarm();
    } else if (mode == 2) {
      hour2 = setHour;
      minute2 = setMinute;
      completed2 = false;
      actived2 = true;
      postAlarm();
    } else if (mode == 3) {
      hour3 = setHour;
      minute3 = setMinute;
      completed3 = false;
      actived3 = true;
      postAlarm();
    } else if (mode == 0) { //mode 0 + E: Đồng bộ
      getAlarm();
    }
  } else if (buttonName == "F") {
    if (notifying1 == 1) {  //nào đang thông báo thì tắt buổi đó
      Serial.println("Đã uống thuốc buổi sáng");
      notifying1 = 0;     //tắt đang thông báo
      completed1 = true;  //đánh dấu đã hoàn thành
      digitalWrite(LOA, LOW);
      postTook();
    }
    if (notifying2 == 1) {
      Serial.println("Đã uống thuốc buổi trưa");
      notifying2 = 0;
      completed2 = true;
      digitalWrite(LOA, LOW);
      postTook();
    }
    if (notifying3 == 1) {
      Serial.println("Đã uống thuốc buổi tối");
      notifying3 = 0;
      completed3 = true;
      digitalWrite(LOA, LOW);
      postTook();
    }
  } else {
    Serial.println("Unknown");
  }
}

//Xóa hẹn giờ 1 buổi
void deleteTimer(int mode) {
  switch (mode) {
    case 1:
      actived1 = false;
      break;
    case 2:
      actived2 = false;
      break;
    case 3:
      actived3 = false;
      break;
  }
}

//Hiển thị mặt đồng hồ theo giờ phút
void displayTime(int hour, int minute) {
  tm.display(0, hour / 10);
  tm.display(1, hour % 10);
  tm.point(1);
  tm.display(2, minute / 10);
  tm.display(3, minute % 10);
}

//Hiển thị mặt đồng hồ theo từng chế độ
void displayClock(int mode) {
  switch (mode) {
    case 0:
      displayTime(hourNow, minuteNow);
      break;
    case 1:
      displayTime(setHour, setMinute);  //dùng để hiển thị thời gian người dùng đang thay đổi
      break;
    case 2:
      displayTime(setHour, setMinute);
      break;
    case 3:
      displayTime(setHour, setMinute);
      break;
  }
}

//Thông báo
void notify() {
  if ((hourNow == hour1) && (minuteNow == minute1) && (completed1 == false) && (actived1 == true)) {
    Serial.println("Chưa uống thuốc buổi sáng");
    notifying1 = 1;
    digitalWrite(LOA, HIGH);
  }
  if ((hourNow == hour2) && (minuteNow == minute2) && (completed2 == false) && (actived2 == true)) {
    Serial.println("Chưa uống thuốc buổi trưa");
    notifying2 = 1;
    digitalWrite(LOA, HIGH);
  }
  if ((hourNow == hour3) && (minuteNow == minute3) && (completed3 == false) && (actived2 == true)) {
    Serial.println("Chưa uống thuốc buổi tối");
    notifying3 = 1;
    digitalWrite(LOA, HIGH);
  }
}