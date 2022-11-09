
     int hiz = 50; 
     int gerilim = 100;
     int sicaklik= 37;
     int sarj_durum = 40;
     int akim = 10;
     int p = 3;

void setup() {
  Serial.begin(9600);

}

void loop() {    Serial.print(hiz);
    Serial.print(",");
    Serial.print(gerilim);
    Serial.print(",");
    Serial.print(sicaklik);
    Serial.print(",");
    Serial.print(sarj_durum);
    Serial.print(",");
    Serial.print(akim );
    Serial.print(",");
    Serial.print(p);
    delay(200);
}
