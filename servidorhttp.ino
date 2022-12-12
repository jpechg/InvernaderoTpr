#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

uint8_t GPIO1pin = 4;
bool GPIO1status = LOW;

uint8_t GPIO2pin = 5;
bool GPIO2status = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(GPIO1pin, OUTPUT);
  pinMode(GPIO2pin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/GPIO1on", handle_GPIO1on);
  server.on("/GPIO1off", handle_GPIO1off);
  server.on("/GPIO2on", handle_GPIO2on);
  server.on("/GPIO2off", handle_GPIO2off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(GPIO1status)
  {digitalWrite(GPIO1pin, HIGH);}
  else
  {digitalWrite(GPIO1pin, LOW);}
  
  if(GPIO2status)
  {digitalWrite(GPIO2pin, HIGH);}
  else
  {digitalWrite(GPIO2pin, LOW);}
}

void handle_OnConnect() {
  GPIO1status = LOW;
  GPIO2status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(GPIO1status,GPIO2status)); 
}

void handle_GPIO1on() {
  GPIO1status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(true,GPIO2status)); 
}

void handle_GPIO1off() {
  GPIO1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false,GPIO2status)); 
}

void handle_GPIO2on() {
  GPIO2status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(GPIO1status,true)); 
}

void handle_GPIO2off() {
  GPIO2status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(GPIO1status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t GPIO1stat,uint8_t GPIO2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>GPIO Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(GPIO1stat)
  {ptr +="<p>GPIO1 Status: ON</p><a class=\"button button-off\" href=\"/GPIO1off\">OFF</a>\n";}
  else
  {ptr +="<p>GPIO1 Status: OFF</p><a class=\"button button-on\" href=\"/GPIO1on\">ON</a>\n";}

  if(GPIO2stat)
  {ptr +="<p>GPIO2 Status: ON</p><a class=\"button button-off\" href=\"/GPIO2off\">OFF</a>\n";}
  else
  {ptr +="<p>GPIO2 Status: OFF</p><a class=\"button button-on\" href=\"/GPIO2on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
