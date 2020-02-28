int sensorPin = A0;
const int sensorValue = 0;
const float ADC_value =0.0048828125;
float LDR_value, lux ,lastLux;
void setup() {
    pinMode(A0, INPUT);
    Serial.begin(9600);
    lastLux = 0.00;
}

void loop() {
  delay(1500);
  LDR_value = analogRead(A0);
  lux = (250.000000/(ADC_value*LDR_value))-50.000000;
  if (lux != lastLux) {
    String luxStr = "<" + String(lux) + ">";
    Serial.print(luxStr);
    lastLux = lux;
  }
  delay(1500);
}
