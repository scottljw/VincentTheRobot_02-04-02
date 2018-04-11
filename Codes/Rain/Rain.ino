#define RAINDROP_AO A3

int rain_detect() {
  pinMode(RAINDROP_AO, INPUT);
  int i = 0;
  int rain, sum = 0;
  while (i < 10) {
    rain = analogRead(RAINDROP_AO);
    sum += rain;
    if (rain - (sum / (i + 1)) > 100 || rain - (sum / (i + 1)) < -100) {
      i = 0;
      sum = 0;
      continue;
    }
    i++;
  }
  return sum / 10;

  /*
  //analog output
  if(analogRead(RAINDROP_AO)<300) Serial.println("Heavy Rain");
  else if(analogRead(RAINDROP_AO)<500) Serial.println("Moderate Rain");
  else Serial.println("No Rain");

  //digital output
  if(digitalRead(RAINDROP_DO) == HIGH) Serial.println("No Rain Detected");
  else Serial.println("Rain Detected"); */
}

int rain_print() {
  int rain = rain_detect();
  if(rain < 300) Serial.println("Heavy Rain");
  else if(rain < 500) Serial.println("Moderate Rain");
  else Serial.println("No Rain");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(rain_detect());
}
