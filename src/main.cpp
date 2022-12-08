#include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <WiFiUdp.h>

// WiFiUDP Udp;

// void setup() {
//   int cnt = 0;
//   //Khởi tạo baud 115200
//   Serial.begin(115200);
//   //Mode wifi là station
//   WiFi.mode(WIFI_STA);
//   //Chờ kết nối
//   while(WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//     if(cnt++ >= 10){
//        WiFi.beginSmartConfig();
//        while(1){
//            delay(1000);
//            //Kiểm tra kết nối thành công in thông báo
//            if(WiFi.smartConfigDone()){
//              Serial.println("SmartConfig Success");
//              break;
//            }
//        }
//     }
//   }
  
//   Serial.println("");
//   Serial.println("");
  
//   WiFi.printDiag(Serial);
  
//   // Khởi tạo server
//   Udp.begin(49999);
//   Serial.println("Server started");

//   // In địa chỉ IP
//   Serial.println(WiFi.localIP());
// }

// void loop() {
//   // Nhận gói tin gửi từ ESPTouch
//   Udp.parsePacket();
//   //In IP của ESP8266
//   while(Udp.available()){
//     Serial.println(Udp.remoteIP());
//     Udp.flush();
//     delay(5);
//   }
// }


// #include <ESP8266WiFi.h>
// //A4:CF:12:F1:F7:94 s
// // D8:BF:C0:06:35:39 r
// void setup(){
//   Serial.begin(115200);
//   Serial.println();
//   Serial.print("ESP8266 Board MAC Address:  ");
//   Serial.println(WiFi.macAddress());
// }
 
// void loop(){

// }
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// #include <ESP8266WiFi.h>
// #include <espnow.h>

// // Structure example to receive data
// // Must match the sender structure
// typedef struct struct_message {
//     char a[32];
//     int b;
//     float c;
//     String d;
//     bool e;
// } struct_message;

// // Create a struct_message called myData
// struct_message myData;

// // Callback function that will be executed when data is received
// void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
//   memcpy(&myData, incomingData, sizeof(myData));
//   Serial.print("Bytes received: ");
//   Serial.println(len);
//   Serial.print("Char: ");
//   Serial.println(myData.a);
//   Serial.print("Int: ");
//   Serial.println(myData.b);
//   Serial.print("Float: ");
//   Serial.println(myData.c);
//   Serial.print("String: ");
//   Serial.println(myData.d);
//   Serial.print("Bool: ");
//   Serial.println(myData.e);
//   Serial.println();
//   if(myData.b ==  1){
//     digitalWrite(15,HIGH);
//   }else
//   digitalWrite(15,LOW);
// }
 
// void setup() {
//   // Initialize Serial Monitor
//   Serial.begin(115200);
//   pinMode(15,OUTPUT);
//   // Set device as a Wi-Fi Station
//   WiFi.mode(WIFI_STA);

//   // Init ESP-NOW
//   if (esp_now_init() != 0) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }
  
//   // Once ESPNow is successfully Init, we will register for recv CB to
//   // get recv packer info
//   esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
//   esp_now_register_recv_cb(OnDataRecv);
// }

// void loop() {
  
// }



/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
// D8:BF:C0:06:35:39
uint8_t broadcastAddress[] = {0xD8, 0xBF, 0xC0, 0x06, 0x35, 0x39};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    strcpy(myData.a, "THIS IS A CHAR");
    if(myData.b ==1 )
    myData.b = 0;
    else
    myData.b=1;
    myData.c = 1.2;
    myData.d = "Hello";
    myData.e = false;

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
  }
}