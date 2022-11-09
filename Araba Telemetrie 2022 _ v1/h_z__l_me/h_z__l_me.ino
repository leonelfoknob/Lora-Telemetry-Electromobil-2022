
const float circumference = 38.82; // Write Wheel Circumference *cm -2*pi*r-
uint8_t hall_Thresh = 5; // "10 value +25rpm" set number of hall trips for RPM reading (higher improves accuracy) ----------> 5şer 5şer artıyoruz
uint8_t hall_Count; // Counter for each spin

// Calculation Speed
void calc_Speed(float passed_Time)
{

  // Calculation Wheel Speed 
  float m_Speed = (circumference / 100000) * hall_Count / (passed_Time / 3600);
  Serial.print((int)m_Speed);
  Serial.println(" km/h");
}

// Calculation RPM
void calc_Rpm(float passed_Time)
{
  // Calculation RPM 
  float rpm_Val =(hall_Count/passed_Time) * 60;
  Serial.print((int)rpm_Val);
  Serial.println("rpm");
}

void clear_LCD_RpmSpeed(float start_Time)
{
    // set 0 values on lcd screen     
    float e_Time = millis();
    if((e_Time - start_Time) / 1000 > 12)
    {
    Serial.println("0  rpm");
    Serial.println("0  km/h");  
    }
}

void setup() {
pinMode(8, INPUT);

Serial.begin(9600);
Serial.println("0  rpm");

Serial.print("SPEED : ");
Serial.println(" 0  km/h");

}

void loop() {
  // Initialize values
  hall_Count = 1;
  float start = millis();
  bool on_state = false;

  // Calculation Spin Time
  while (1) {
    if (digitalRead(8) == 0) 
    {
      if (on_state == false) 
      {
        on_state = true;
        hall_Count++;                   // Increase counter in each spin 
      }
    }
    else 
    {
      on_state = false;
    }

    clear_LCD_RpmSpeed(start);          // Control whether movement

    if (hall_Count >= hall_Thresh)      // Control counter overflow as threshold 
    {
      break;
    }
  } 

  // Calculation Passed Time 
  float end_Time = millis();
  float time_Passed = (end_Time - start) / 1000;

  calc_Speed(time_Passed);
  calc_Rpm(time_Passed);

  delay(1);// delay in between reads for stability
}
