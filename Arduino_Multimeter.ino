const int pinv1 = A1;
const int pinv2 = A2;
int v1, v2;

void setup() {
  pinMode(pinv1, INPUT);
  pinMode(pinv2, INPUT);
  Serial.begin(115200);
}

void loop() {

  v1 = analogRead(pinv1);
  v2 = analogRead(pinv2);

  Serial.print(v1);
  Serial.print(" , ");
  Serial.println(v2);
  delay(25);
}
