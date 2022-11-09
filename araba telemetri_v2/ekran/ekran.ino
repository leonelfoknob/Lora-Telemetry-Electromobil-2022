#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

int hiz;
int max_temp;
int volts;
int currents;
int power;
int energy;



void setup() {
lcd.init();
  lcd.backlight();
  lcd.begin (20, 4);
  lcd.setCursor(2, 0);
  lcd.print("Tel ve AKS veri");
  Serial.begin(9600);
}

void loop() {


   lcd.setCursor(0, 1);
    lcd.print("hiz: ");
    lcd.setCursor(4, 1);
    lcd.print(hiz);
    lcd.print("km/h");

    lcd.setCursor(12, 1);
    lcd.print("Temp:");
    lcd.setCursor(17, 1);
    lcd.print(max_temp);

    lcd.setCursor(0, 2);
    lcd.print("ger: ");
    lcd.setCursor(4, 2);
    lcd.print(volts);
    lcd.print("V");

    lcd.setCursor(12, 2);
    lcd.print("Amp: ");
    lcd.setCursor(16, 2);
    lcd.print(currents);
    lcd.print("A");

    lcd.setCursor(0, 3);
    lcd.print("pwr: ");
    //lcd.print(":");
    lcd.setCursor(4, 3);
    lcd.print("%");
    lcd.print(power);

    lcd.setCursor(12, 3);
    lcd.print("Eng: ");
    lcd.setCursor(16, 3);
    lcd.print(energy);
    lcd.print("W");
}
