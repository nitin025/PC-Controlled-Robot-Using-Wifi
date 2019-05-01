int inPin1 = 7;    // pushbutton connected to digital pin 7
int inPin2 = 8;
int inPin3 = 9;
int inPin4 = 10;

int val = 0;      // variable to store the read value

void setup() {
  Serial.begin(9600);
  pinMode(inPin1, INPUT);  // sets the digital pin 13 as output
  pinMode(inPin2, INPUT);  // sets the digital pin 13 as output  
  pinMode(inPin3, INPUT);  // sets the digital pin 13 as output
  pinMode(inPin4, INPUT);  // sets the digital pin 13 as output

     // sets the digital pin 7 as input
}

void loop() {
  val = digitalRead(inPin1);   // read the input pin
  val = val*10;
  val = val +  digitalRead(inPin2);   // read the input pin
  
  val = val*10;
  val = val +  digitalRead(inPin3);   // read the input pin

  val = val*10;
  val = val +  digitalRead(inPin4);   // read the input pin

  if(val == 1001)
    Serial.print("Right\n");
  else if(val == 110){
    Serial.print("Left");
    Serial.print("\n0");
  }
  else if(val == 101){
    Serial.print("Forward");
    Serial.print("\n0");
  }
  else if(val == 1010)
    Serial.print("Backward\n");

  
  Serial.print(val);
  val = 0;
  Serial.print("\n");
  Serial.print("\n");
  Serial.print("\n");

  delay(1000);
}
