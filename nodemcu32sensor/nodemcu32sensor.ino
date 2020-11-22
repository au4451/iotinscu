const int analogInPin = 36;   // NodeMCU-32S ADC0
const int analogOutPin = 25;  // NodeMCU-32S DAC1

int sensorValue = 0;          // value read from ADC0
int outputValue = 0;          // value output to DAC1

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 350, 0, 255);
  // change the analog out value:
  dacWrite(analogOutPin, outputValue);

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);
  delay(1000);
}
