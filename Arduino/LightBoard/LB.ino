float lux = 0.00, ADC_value =0.0048828125, LDR_value;
int sensorPin = A0;
int sensorValue = 0;
void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);

}

void loop() {
 LDR_value = analogRead(A0);
 lux = (250.000000/(ADC_value*LDR_value))-50.000000;
 Serial.write(convertToString(lux));
 Serial.println(convertToString(lux));
 delay(1100);
}
String convertToString(float num)
{
  int n = round(num);
  return "<" + String(n) + ">";
}
