/*This code was created by H.I Electronic Tech channel
 * I hope this code helps you in your projects 
 * if you want to support me, you could put a Like Share and subscribe to my channel
 * here the youtube video if you have any question put it in the comments
 *https://youtu.be/na-jyyerjHM
*/
#include <ESP8266WiFi.h>//be sure this line changes to the red when you select the ESP board

const char* SSID = "Edina_lak";       //your WiFi name
const char* password = "Edina.Edina";//your WiFi password

// Setup GPIO2
int pinGPIO2 = 2; //To control Relay
int pinGPIO0 = 0; //To control LED
int LStatus = 0; //0=off,1=on,
int RStatus = 0; //0=off,1=on,

// Create the ESP Web Server on port 80
WiFiServer WebServer(80);

// Web Client
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Setup the GPIO2 LED Pin
  pinMode(pinGPIO2, OUTPUT);
  digitalWrite(pinGPIO2, LOW);
  // Setup the GPIO0 Relay Pin
  pinMode(pinGPIO0, OUTPUT);
  digitalWrite(pinGPIO2, LOW);
  
  // Connect to WiFi network
  Serial.println();
  WiFi.disconnect();//disconnect from any wifi network
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, password);//connect to the wifi

  while (WiFi.status() != WL_CONNECTED) {//while the esp8266 trying to connect with WiFi print dot(..)
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected to WiFi");

  // Start the Web Server
  WebServer.begin();
  Serial.println("Web Server started");

  // Print the IP address
  Serial.print("You can connect to the ESP8266 at this URL: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  //if the WiFi restart or abruptly turned off the esp will reconnect with it.
  if(WiFi.status() != WL_CONNECTED){
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    Serial.print("Reconnecting to ");
    Serial.println(SSID);
    WiFi.begin(SSID, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("Connected");
  }
  
  // Check if a user has connected
  client = WebServer.available();
  if (!client) {
    return;     //restart loop
  }

  // Wait until the user sends some data
  Serial.println("New User");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r\n');
  Serial.println(request);
  client.flush();

  // Process the request:
  if (request.indexOf("/ONR") != -1) {
    digitalWrite(pinGPIO2, HIGH);
    RStatus = 1;
  }
  if (request.indexOf("/OFFR") != -1) {
    digitalWrite(pinGPIO2, LOW);
    RStatus = 0;
  }
  if (request.indexOf("/ONL") != -1) {
    digitalWrite(pinGPIO0, HIGH);
    LStatus = 1;
  }
  if (request.indexOf("/OFFL") != -1) {
    digitalWrite(pinGPIO0, LOW);
    LStatus = 0;
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>ESP8266 controller</title><style type=\"text/css\">body{background-color: gray;}.button {background: rgba(225,225,222,1);");
  client.println("font-size:23px;border-radius:15px;}.button:hover{box-shadow:0px 0px 8px 2px #333;cursor:pointer}; a {text-decoration: none;} ");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("</br></br><center><h1 style=\"text-align: center;\">Esp8266 WiFi controller</h1>");
  client.println("<a href=\"/\">Refresh Status</a>");
  //check the Relay status
  if (RStatus == 0) {
    client.print("<h1>Relay is Off</h1></br>");
    client.println("<h2>Turn the Relay</h2><a href=\"/ONR\"><button class=\"button\">ON</button></a></br>");
  } 
  else if (RStatus == 1) {
    client.print("<h1>Relay is On</h1></br>");
    client.println("<h2>Turn the Relay</h2><a href=\"/OFFR\"><button class=\"button\">OFF</button></a></br>");
  }
  //check the LED status
  if (LStatus == 0) {
    client.print("<h1>LED is Off</h1></br>");
    client.println("<h2>Turn the LED</h2><a href=\"/ONL\"><button class=\"button\">ON</button></a></br>");
  } 
  else if (LStatus == 1) {
    client.print("<h1>LED is On</h1></br>");
    client.println("<h2>Turn the LED</h2><a href=\"/OFFL\"><button class=\"button\">OFF</button></a></br>");
  }
  client.println("</center></br>");
  client.println("</body>");
  client.println("</html>");

  delay(1);
  Serial.println("User disconnected");
  Serial.println("");

}
