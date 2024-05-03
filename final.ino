#define enA 9
#define in1 11
#define in2 10
#define enB 8
#define in3 13
#define in4 12

#define sensor A0

int encoderLPin1 = 3;
int encoderLPin2 = 2;
int encoderRPin1 = 19;
int encoderRPin2 = 18;
volatile int lastEncodedL = 0;
volatile int lastEncodedR = 0;
volatile long encoderLValue = 0;
volatile long encoderRValue = 0;


void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.begin(9600);

  pinMode(encoderLPin1, INPUT_PULLUP); 
  pinMode(encoderLPin2, INPUT_PULLUP);
  pinMode(encoderRPin1, INPUT_PULLUP); 
  pinMode(encoderRPin2, INPUT_PULLUP);

  digitalWrite(encoderLPin1, HIGH);
  digitalWrite(encoderLPin2, HIGH);
  digitalWrite(encoderRPin1, HIGH);
  digitalWrite(encoderRPin2, HIGH);

  attachInterrupt(digitalPinToInterrupt(encoderLPin1), updateLEncoder, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(encoderLPin2), updateLEncoder, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(encoderRPin1), updateREncoder, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(encoderRPin2), updateREncoder, CHANGE); 
}

void loop() {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    Serial.print(encoderLValue*360/2767);
    Serial.print(" ");
    Serial.print(encoderRValue*360/2767);
    Serial.println();
    float pos = (float(encoderLValue+encoderRValue)/2767)*3.1425*26/10;
    if(pos>10){
      analogWrite(enA, 0);
      analogWrite(enB, 0);
    }
    else{
      analogWrite(enA, 50);
      analogWrite(enB, 50);
    }

    if(readFront()){
    turnAround();      
    }
}

void updateLEncoder(){
  int MSB = digitalRead(encoderLPin1);
  int LSB = digitalRead(encoderLPin2);

  int encoded = (MSB << 1) |LSB;
  int sum  = (lastEncodedL << 2) | encoded;

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderLValue --;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderLValue ++;


  lastEncodedL = encoded;
}

void updateREncoder(){
  int MSB = digitalRead(encoderRPin1);
  int LSB = digitalRead(encoderRPin2);

  int encoded = (MSB << 1) |LSB;
  int sum  = (lastEncodedR << 2) | encoded;

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderRValue --;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderRValue ++;


  lastEncodedR = encoded;
}

void turnAround(){
  while(true){
    encoderLValue = 0;
    if(float((float(encoderLValue)/2767)*3.1425*26/10)<10.21)
    {
      analogWrite(enA, 0);
      analogWrite(enB, 50);
    }
    else{
      analogWrite(enA, 0);
      analogWrite(enB, 0);
      delay(500);
      break;     
    }
  }
}

int readFront(){
  float volts = analogRead(sensor)*0.0048828125;
  int distance = 13*pow(volts, -1);
  
  if (distance <= 15){
    return 1;
  }
  else return 0;
}
