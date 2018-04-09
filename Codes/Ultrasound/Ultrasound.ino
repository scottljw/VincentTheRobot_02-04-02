#define ULTRASOUND 8
#define TIMEOUT 30000

double echolocation() {
  double duration;
  double distance;
  
  pinMode(ULTRASOUND, OUTPUT);
  digitalWrite(ULTRASOUND, LOW);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND, LOW);
  
  pinMode(ULTRASOUND, INPUT);
  duration = pulseIn(ULTRASOUND, HIGH, TIMEOUT);
  distance = duration / 2.0 * 0.034;
  
  //Serial.print("\n\nDistance to the front wall: ");
  //Serial.println(distance);
  return distance;
}

double data_filter() {
  int i = 0;
  double sum = 0;
  double temperary = echolocation();
  while (i < 10) {
    sum += temperary;
    if (temperary - (sum / (i + 1)) > 1.0 || temperary - (sum / (i + 1)) < -1.0) {
      i = 0;
      sum = 0;
      continue;
    }
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
