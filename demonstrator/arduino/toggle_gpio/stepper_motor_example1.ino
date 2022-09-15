const int stepPin = 9;
const int dirPin = 2;
const int enPin = 8;
const int microstep = 800;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
  Serial.begin(9600);
}

void loop() {
  count += 1;
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Each iteration completes a rev.
  for (int x = 0; x < microstep; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(1000);
  }
  Serial.println((String)"Rev " + count + " has finished.");
}
