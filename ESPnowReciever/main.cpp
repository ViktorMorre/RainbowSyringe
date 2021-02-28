#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>
using namespace std;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// SETUP LEDSTRIP //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#define red 12
#define green 13
#define blue 14
 //On Board LED

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// setting PWM properties
const int freq = 5000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;
const int resolution = 8; //Resolution 8, 10, 12, 15

//=======================================================================
//                    Power on setup
//=======================================================================
void setupLed() {
  Serial.begin(115200);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue,OUTPUT);
  
  
  // configure LED PWM functionalitites
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);
  
  
  // attach the channel to the GPIO2 to be controlled
  ledcAttachPin(red, greenChannel);
  ledcAttachPin(green, greenChannel);
  ledcAttachPin(blue, blueChannel);
  
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value){
  ledcWrite(redChannel, red_light_value);
  ledcWrite(greenChannel, green_light_value);
  ledcWrite(blueChannel, blue_light_value);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// SETUP RECIEVER /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Replace with your network credentials (STATION)
const char* ssid = "????";
const char* password = "?????";
int colors[7];

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int ok;
  int kleuren[7];
} struct_message;



struct_message incomingReadings;

JSONVar board;

AsyncWebServer server(80);
AsyncEventSource events("/events");

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  // Copies the sender mac address to a string
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  
  board["ok"] = incomingReadings.ok;
  board["kleuren"] = incomingReadings.kleuren;
  String jsonString = JSON.stringify(board);
  //string jsonString = incomingReadings.ok;
  events.send(jsonString.c_str(), "new_readings", millis());
  
  Serial.printf("message: %u \n", incomingReadings.ok);
  for(int i=0; i<7; i++){
    Serial.printf("kleur %u: %u \n", i, incomingReadings.kleuren[i]);
  }
  if(incomingReadings.ok==1){
    for(int i=0; i<7; i++){
      colors[i] = incomingReadings.kleuren[i];
    }
  }
  Serial.println();
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP-NOW DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .packet { color: #bebebe; }
    .card.temperature { color: #fd7e14; }
    .card.humidity { color: #1b78e2; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>ESP-NOW DASHBOARD</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> BOARD #1 - TEMPERATURE</h4><p><span class="reading"><span id="t1"></span> &deg;C</span></p><p class="packet">Reading ID: <span id="rt1"></span></p>
      </div>
      <div class="card humidity">
        <h4><i class="fas fa-tint"></i> BOARD #1 - HUMIDITY</h4><p><span class="reading"><span id="h1"></span> &percnt;</span></p><p class="packet">Reading ID: <span id="rh1"></span></p>
      </div>
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> BOARD #2 - TEMPERATURE</h4><p><span class="reading"><span id="t2"></span> &deg;C</span></p><p class="packet">Reading ID: <span id="rt2"></span></p>
      </div>
      <div class="card humidity">
        <h4><i class="fas fa-tint"></i> BOARD #2 - HUMIDITY</h4><p><span class="reading"><span id="h2"></span> &percnt;</span></p><p class="packet">Reading ID: <span id="rh2"></span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('new_readings', function(e) {
  console.log("new_readings", e.data);
  var obj = JSON.parse(e.data);
  document.getElementById("t"+obj.id).innerHTML = obj.temperature.toFixed(2);
  document.getElementById("h"+obj.id).innerHTML = obj.humidity.toFixed(2);
  document.getElementById("rt"+obj.id).innerHTML = obj.readingId;
  document.getElementById("rh"+obj.id).innerHTML = obj.readingId;
 }, false);
}
</script>
</body>
</html>)rawliteral";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// SETUP //////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RF24 radio(7, 8); // CE, CSN

const byte address1[6] = "00001";
const byte address2[6] = "00010";
const byte address3[6] = "00011";

void radio1Setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address1);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void radio2Setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address2);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void radio3Setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address3);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void setup() {

  // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);
  
  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
   
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
  Serial.begin(9600); //belangrijk

  setupLed();

}

int teller;
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
int kleur = 6;
int recieved1;
int recieved2;
int recieved3;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// METHODS ////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  green = 0
  red = 1
  blue = 2
  yellow = 3
  purple = 4
  cyan = 5
  zwart= 6
*/


//Met behulp van de ontvangen kleuren de gemaakt kleur bepalen
void colorMaker(int mColor1, int mColor2, int mColor3){  //groen
  if(((mColor1 || mColor2 || mColor3)==0) && ((mColor1 || mColor2 || mColor3)==6) && ((mColor1==mColor2) || (mColor2 == mColor3)|| (mColor3==mColor1))) {
      kleur=0;
      RGB_color(0, 255, 0);
    }
  //rood
  if(((mColor1 || mColor2 || mColor3)==1) && ((mColor1 || mColor2 || mColor3)==6) && ((mColor1==mColor2) || (mColor2 == mColor3)|| (mColor3==mColor1))) {
      kleur=1;
      RGB_color(255, 0, 0);
    }
  //blauw
  if(((mColor1 || mColor2 || mColor3)==2) && ((mColor1 || mColor2 || mColor3)==6) && ((mColor1==mColor2) || (mColor2 == mColor3)|| (mColor3==mColor1))) {
      kleur=2;
      RGB_color(0, 0, 255);
    }
  //geel
  if(((mColor1 || mColor2 || mColor3)==0) && ((mColor1 || mColor2 || mColor3)==1) && ((mColor1 || mColor2 || mColor3)==6)){
      kleur=3;
      RGB_color(255, 255, 0);
    }
  //paars
  if(((mColor1 || mColor2 || mColor3)==1) && ((mColor1 || mColor2 || mColor3)==2) && ((mColor1 || mColor2 || mColor3)==6)){
      kleur=4;
      RGB_color(255, 0, 255);
    }
  //cyaan
  if(((mColor1 || mColor2 || mColor3)==0) && ((mColor1 || mColor2 || mColor3)==2) && ((mColor1 || mColor2 || mColor3)==6)){
      kleur=5;
      RGB_color(0, 255, 255);
    }
  //zwart
  if(((mColor1 || mColor2 || mColor3)==6) && ((mColor1==mColor2) || (mColor2 == mColor3)|| (mColor3==mColor1))) {
      kleur=2;

    }
}

 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// LOOP  //////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 5000;
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    events.send("ping",NULL,millis());
    lastEventTime = millis();
  }
  delay(100);

  radio1Setup();
  if (radio.available()) {
    int recieved1;
    radio.read(&recieved1, sizeof(recieved1));
    Serial.println(recieved1);
  }
  radio2Setup();
  if (radio.available()) {
    int recieved2;
    radio.read(&recieved2, sizeof(recieved2));
    Serial.println(recieved2);
  }
  radio3Setup();
  if (radio.available()) {
    int recieved3;
    radio.read(&recieved3, sizeof(recieved3));
    Serial.println(recieved3);
  }

  colorMaker(recieved1, recieved2, recieved3);

  
}