#define IR A0

int infrared() {
  pinMode(IR, INPUT);
  int i = 0;
  int reflection, sum = 0;
  while (i < 10) {
    reflection = digitalRead(IR);
    Serial.println(reflection);
    sum += reflection;
    if (reflection - (sum / (i + 1)) > 100 || reflection - (sum / (i + 1)) < -100) {
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
   Serial.println(infrared());
//  infrared();
}
