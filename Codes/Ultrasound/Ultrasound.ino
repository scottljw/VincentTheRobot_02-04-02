#define ULTRASOUND_Trig 12
#define ULTRASOUND_Echo 11
#define TIMEOUT 30000

double echolocation() {
  double duration;
  double distance;
  
  pinMode(ULTRASOUND_Trig, OUTPUT);
  pinMode(ULTRASOUND_Echo, INPUT);
  
  digitalWrite(ULTRASOUND_Trig, LOW);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND_Trig, LOW);
  
  duration = pulseIn(ULTRASOUND_Echo, HIGH, TIMEOUT);
  distance = duration / 2.0 * 0.034;
  
  //Serial.print("\n\nDistance to the front wall: ");
  //Serial.println(distance);
  return distance;
}

double data_filter() {
  int i = 0;
  double sum = 0;
  while (i < 10) {
    sum += echolocation();
    i++;
  }
  return sum / 10;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(data_filter());
}
