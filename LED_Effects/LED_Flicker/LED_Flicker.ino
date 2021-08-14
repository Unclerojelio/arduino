#define CANDLELED 9
#define FLICKER_INTERVAL 25

long previousMillis = 0;

void setup() {
  pinMode(CANDLELED, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > FLICKER_INTERVAL)
  {
    previousMillis = currentMillis;
    analogWrite(CANDLELED, random(0, 256));
  }
}
