// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "NitinS";
const char* password = "12345678";
#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  16          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  4          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  0           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  2           // L298N in4 motors Left            GPIO0(D3)
int sharpPin = A0;        //A0
int speedCar = 200;         // 400 - 1023.
int speed_Coeff = 3;
int Obj = 0;
int twoTimesBack = 0;
  
// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";
bool refreshed = false;
// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;


void setup() {
  Serial.begin(9600);
  // Initialize the output variables as outputs
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(sharpPin, INPUT);


  // Set outputs to LOW

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
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
    Serial.println(analogRead(sharpPin));
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
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
            if (Obj == 1 ) {
              client.print("Object Detected");
              Obj = 0;
            }

            // turns the GPIOs on and off
            if (header.indexOf("GET /w/on") >= 0) {
              Serial.println("Forward");
              //output5State = "on";
              goAhead();
              client.print("Moving Forward");


            }
            else if (header.indexOf("GET /a/on") >= 0) {
              Serial.println("Left");
              //output5State = "on";
                            goAheadLeft();
              client.print("Moving Left");
            }
            else if (header.indexOf("GET /s/on") >= 0) {
              Serial.println("Back");
              //output5State = "on";
              goBack();
              client.print("Moving Back");
            }
            else if (header.indexOf("GET /d/on") >= 0) {
              Serial.println("Right");
              //output5State = "on";
                            goAheadRight();
              client.print("Moving Right");
            }

            else if (header.indexOf("GET /x/on") >= 0) {
              Serial.println("Stop");
              //output5State = "on";
              stopRobot();
              client.print("Bot Stopped");
            }
            else if (header.indexOf("GET /q/on") >= 0) {
              Serial.println("Stop");
              //output5State = "on";
              goLeft();
            }
            else if (header.indexOf("GET /e/on") >= 0) {
              Serial.println("Stop");
              //output5State = "on";
              goRight();
            }
            else if (header.indexOf("GET /z/on") >= 0) {
              Serial.println("Stop");
              //output5State = "on";
              goBackLeft();
            }
            else if (header.indexOf("GET /c/on") >= 0) {
              Serial.println("Stop");
              //output5State = "on";
              goBackRight();
            }
            
  
if (map(analogRead(A0),0 , 1023, 0,1023) > 500 ){
     Obj = 1;
    if(twoTimesBack != 2){
    goBack();
    twoTimesBack++;
    }
    else{stopRobot();}
     
     delay(100);
     
     stopRobot();
     
     Serial.println("Object Detected");
     
     Serial.println(map(analogRead(A0),0 , 1023, 255, 0));
     
          }
 
   
  
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta http-equiv = \"refresh\" content = \"2 \">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP8266 Mercury Labs</h1>");
            client.println("<p id='log'></p>");
            client.println("<p><a href=\"/q/on\"><button class=\"button\">Q</button></a>");
            client.println("<a href=\"/w/on\"><button class=\"button\">W</button></a>");
            client.println("<a href=\"/e/on\"><button class=\"button\">E</button></a></p>");

            client.println("<p><a href=\"/a/on\"><button class=\"button\">A</button></a>");
            client.println("<a href=\"/s/on\"><button class=\"button\">S</button></a>");
            client.println("<a href=\"/d/on\"><button class=\"button\">D</button></a></p>");
            client.println("<p><a href=\"/z/on\"><button class=\"button\">Z</button></a>");
            client.println("<a href=\"/x/on\"><button class=\"button\">X</button></a>");
            client.println("<a href=\"/c/on\"><button class=\"button\">C</button></a></p>");
             client.print("<h3><b>Distance:</b></h3>");
               client.print(map(analogRead(A0), 1023, 0 , 0,30));
             client.println("cm ");

            client.println("</body> </html>");

            client.println("<script>");
            client.println("const log = document.getElementById('log');");
            client.println("document.addEventListener('keypress', logKey);");
            client.println("function logKey(e) {");
            client.println("  if('KeyW'==`${e.code}`){");
            client.println("  log.textContent = 'Moving Forward' ;");
            client.println("    window.location.href='/w/on';}");
            client.println("  if('KeyS'==`${e.code}`){");
            client.println("  log.textContent = 'Moving Back';");
            client.println("    window.location.href='/s/on';}");
            client.println("  if('KeyD'==`${e.code}`){");
            client.println("  log.textContent ='Moving Right' ;");
            client.println("    window.location.href='/d/on';}");
            client.println("  if('KeyA'==`${e.code}`){");
            client.println("  log.textContent = 'Moving Left';");
            client.println("    window.location.href='/a/on';}");
            client.println("  if('Space'==`${e.code}`){");
            client.println("  log.textContent ='Bot Stopped' ;");
            client.println("    window.location.href='/x/on';}");
            client.println("  if('KeyQ'==`${e.code}`){");
            client.println("  log.textContent ='Moving Forward Left' ;");
            client.println("    window.location.href='/q/on';}");
            client.println("  if('KeyE'==`${e.code}`){");
            client.println("  log.textContent ='Moving Forward Right' ;");
            client.println("    window.location.href='/e/on';}");
            client.println("  if('KeyZ'==`${e.code}`){");
            client.println("  log.textContent ='Moving Back Left' ;");
            client.println("    window.location.href='/z/on';}");
            client.println("  if('KeyC'==`${e.code}`){");
            client.println("  log.textContent ='Moving Back Right' ;");
            client.println("    window.location.href='/c/on';}}");

            client.println("</script>");



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
void goAhead() {
  twoTimesBack = 0;
  
  //else {
    Serial.print("goAhead");
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    analogWrite(ENA, speedCar);

    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
    analogWrite(ENB, speedCar);

    
  //}
}

void goBack() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
  Serial.print("goBack");
}

void goRight() {
  twoTimesBack = 0;
  
  Serial.print("goRight");
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);

 
}

void goLeft() {
  twoTimesBack = 0;
  
  Serial.print("goLeft");
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);

 
}

void goAheadRight() {
twoTimesBack = 0;
  
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar / speed_Coeff);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
  
 
}

void goAheadLeft() {
twoTimesBack = 0;
  
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar / speed_Coeff);
}

void goBackRight() {
twoTimesBack = 0;
  
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar / speed_Coeff);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);


}

void goBackLeft() {
twoTimesBack = 0;
  
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar / speed_Coeff);

 
}

void stopRobot() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}
