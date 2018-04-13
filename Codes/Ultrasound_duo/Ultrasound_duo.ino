#define ULTRASOUND1 8
#define ULTRASOUND2 7
#define TIMEOUT 30000

double echo() {
  double duration;
  double distance;
  
  pinMode(ULTRASOUND2, OUTPUT);
  digitalWrite(ULTRASOUND2, LOW);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND2, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND2, LOW);
  
  pinMode(ULTRASOUND2, INPUT);
  duration = pulseIn(ULTRASOUND2, HIGH, TIMEOUT);
  distance = duration / 2.0 * 0.034;
  
  //Serial.print("\n\nDistance to the front wall: ");
  //Serial.println(distance);
  return distance;
}

double echo2() {
  double duration;
  double distance;
  
  pinMode(ULTRASOUND1, OUTPUT);
  digitalWrite(ULTRASOUND1, LOW);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND1, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND1, LOW);
  
  pinMode(ULTRASOUND1, INPUT);
  duration = pulseIn(ULTRASOUND1, HIGH, TIMEOUT);
  distance = duration / 2.0 * 0.034;
  
  //Serial.print("\n\nDistance to the front wall: ");
  //Serial.println(distance);
  return distance;
}

double echolocation() {
  int i = 0;
  double sum = 0;
  double temperary = echo();
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

double echolocation2() {
  int i = 0;
  double sum = 0;
  double temperary = echo2();
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
  Serial.print(echolocation());
  Serial.print("    vs    ");
  Serial.println(echolocation2());
}
