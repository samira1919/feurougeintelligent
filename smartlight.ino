#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

//fir-feurouge-default-rtdb.firebaseio.com/ 

#define FIREBASE_HOST "smartleds-27d91-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "NEKwBuxMtQMwe6hbIczdFXwUaGFwnAbG2oe4MKov"



// Replace with your network credentials
//const char* ssid     = "Redmi Note 9 Pro";
//const char* password = "12346789";



const char* ssid     = "CPH1609";
const char* password = "samira123456";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
 
// Auxiliar variables to store the current output state
String r16 = "off";
String o5 = "off";
String v4 = "off";

String a1 = "off";


// Assign output variables to GPIO pins
const int ledr = 16;
const int ledo = 5;
const int ledv = 4;

const int ledr1 = 15;
const int ledo1 = 3;
const int ledv1 = 1;

#define LED_R1 15
#define LED_O1 3
#define LED_V1 1


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(ledr, OUTPUT);
  pinMode(ledo, OUTPUT);
  pinMode(ledv, OUTPUT);

  pinMode(ledr1,OUTPUT);
  pinMode(ledo1,OUTPUT);
  pinMode(ledv1,OUTPUT);
  
  
  // Set outputs to LOW
  digitalWrite(ledr, LOW);
  digitalWrite(ledo, LOW);
  digitalWrite(ledv, LOW);

  digitalWrite(ledr1, LOW);
  digitalWrite(ledo1, LOW);
  digitalWrite(ledv1, LOW);


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  
  //INITIALISATION of firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /16/on") >= 0) {
              Serial.println("Rouge on");
             r16="on";
              digitalWrite(ledr, HIGH); 
              Firebase.setString("/Leds/Rouge", "on");}

              else if (header.indexOf("GET /16/off")>= 0){
                 Serial.println("Rouge off");
             r16="off";
              digitalWrite(ledr, LOW); 
              Firebase.setString("/Leds/Rouge", "off");}

                
               else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("Vert on");
             v4 = "on";
              digitalWrite(ledv, HIGH);
              Firebase.setString("/Leds/Vert", "on");}
              
              else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("Vert off");
             v4 = "off";
              digitalWrite(ledv, LOW);
              Firebase.setString("/Leds/Vert", "off"); }
            
            else if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("Orange on");
              o5 = "on";
              digitalWrite(ledo, HIGH);
              Firebase.setString("/Leds/Orange", "on");}

             else if (header.indexOf("GET /5/off") >= 0){
              Serial.println("Orange off");
              o5 = "off";
              digitalWrite(ledo, LOW);
              Firebase.setString("/Leds/Orange", "off");}
              
              else if (header.indexOf("GET /1/on") >= 0){
              Serial.println("Automatic on");
              if(a1 = "on"){
       
              digitalWrite(ledr1, HIGH);
              digitalWrite(ledo1, LOW);
              digitalWrite(ledv1, LOW);
              delay(3000);
                // Etat 2: 
              digitalWrite(ledr1, HIGH);
              digitalWrite(ledo1, LOW);
              digitalWrite(ledv1, LOW);
              
              delay(1000);
                // Etat 3: 
              digitalWrite(ledr1, LOW);
              digitalWrite(ledo1, LOW);
              digitalWrite(ledv1, HIGH);
          
              delay(3000);
              // Etat 4: 
              digitalWrite(ledr1, LOW);
              digitalWrite(ledo1, HIGH);
              digitalWrite(ledv1, LOW);
              
              delay(1000);

              
              digitalWrite(ledr1, HIGH);
              digitalWrite(ledo1, LOW);
              digitalWrite(ledv1, LOW);
              delay(3000);
                // Etat 2: 
              digitalWrite(ledr1, HIGH);
              digitalWrite(ledo1, LOW);
              digitalWrite(ledv1, LOW);
              
              delay(1000);
                // Etat 3: 
              digitalWrite(ledr1, LOW);
              digitalWrite(ledo1, LOW);
              digitalWrite(ledv1, HIGH);
          
              delay(3000);
              // Etat 4: 
              digitalWrite(ledr1, LOW);
              digitalWrite(ledo1, HIGH);
              digitalWrite(ledv1, LOW);
              
              delay(1000);
              }
              }
              else if (header.indexOf("GET /1/off") >= 0){
              Serial.println("Automatic off");
              a1 = "off";
              digitalWrite(ledr1, LOW);
              digitalWrite(ledo1, LOW);
              digitalWrite(ledv1, LOW);}

    
              
             
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
           // client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            //client.println(".button { background-color: #E73711; border: none; color: white; padding: 16px 40px; border-radius: 60%;");
            //client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            //client.println(".button2 {background-color: #77878A;}</style></head>");


            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".buttonRed { background-color: #ff0000; border: none; color: white; padding: 16px 40px; border-radius: 60%;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".buttonGreen { background-color: #00ff00; border: none; color: white; padding: 16px 40px; border-radius: 60%;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".buttonYellow { background-color: #feeb36; border: none; color: white; padding: 16px 40px; border-radius: 60%;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".buttonOff { background-color: #77878A; border: none; color: white; padding: 16px 40px; border-radius: 70%;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Feu Rouge</h1>");
            
        
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>Vert " +v4 + "</p>");
            // If thev4 is off, it displays the ON button       
            if (v4=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button buttonGreen\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button buttonOff\">OFF</button></a></p>");
            }


                // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>Orange " + o5 + "</p>");
            // If the o5 is off, it displays the ON button       
            if (o5=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button buttonYellow\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button buttonOff\">OFF</button></a></p>");
            } 
               

            // Display current state, and ON/OFF buttons for GPIO 16 
            client.println("<p>Rouge " +r16 + "</p>");
            // If the output16State is off, it displays the ON button       
            if (r16=="off") {
              client.println("<p><a href=\"/16/on\"><button class=\"button buttonRed\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/16/off\"><button class=\"button buttonOff\">OFF</button></a></p>");
            } 
            
             client.println("<p>Automatic " +a1 + "</p>");
            // If the output16State is off, it displays the ON button       
            if (a1=="off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
