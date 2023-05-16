#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Input 
int BUTTONUP = 3;
int BUTTONDOWN = 4;
int MANUALON = 5;
int MANUALOFF = 6;

// Output
int SOLENOID = 7;
int LEDGREEN = 8;
int LEDRED = 9;

// Time
long hour = 0, minute = 0, second = 0;
long countdown_time = 0;    // edit here with seconds
int millisIntervalTime = 1000;
unsigned long miilisPrevTime = 0;

// Control
int modeControl = 0;          // 0 - auto / 1 - manual
bool isManualOn = false;        // manual on off solenoid

// Debounce
int millisIntervalPin = 100;
unsigned long miilisPrevPin = 0;


// Start
// =============================================================================
void setup() 
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("    STARTING    ");

  // input 
  pinMode(BUTTONUP, INPUT_PULLUP);
  pinMode(BUTTONDOWN, INPUT_PULLUP);
  pinMode(MANUALON, INPUT_PULLUP);
  pinMode(MANUALOFF, INPUT_PULLUP);

  // output
  pinMode(SOLENOID, OUTPUT);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDRED, OUTPUT);
}


// Loop
// =============================================================================
void loop() 
{
  // Millis
  unsigned long millisCurrent = millis();

  // Input
  int bValUp = digitalRead(BUTTONUP);
  int bValDown = digitalRead(BUTTONDOWN);
  int bValOn = digitalRead(MANUALON);
  int bValOff = digitalRead(MANUALOFF);

  // Check Input
  if (!bValUp)
  {
    if ((unsigned long)(millisCurrent - miilisPrevPin) >= millisIntervalPin)
    {
      miilisPrevPin = millisCurrent;

      modeControl = 0;
      countdown_time += 60;
    }
  }

  // Check Input
  if (!bValDown)
  {
    if ((unsigned long)(millisCurrent - miilisPrevPin) >= millisIntervalPin)
    {
      miilisPrevPin = millisCurrent;

      modeControl = 0;
      countdown_time -= 60;
      if (countdown_time <= 0)
      {
        countdown_time = 0;
      }
    }
  }

  // Check Input
  if (!bValOn)
  {
    if ((unsigned long)(millisCurrent - miilisPrevPin) >= millisIntervalPin)
    {
      miilisPrevPin = millisCurrent;

      modeControl = 1;
      isManualOn = true;
    }
  }

  // Check Input
  if (!bValOff)
  {
    if ((unsigned long)(millisCurrent - miilisPrevPin) >= millisIntervalPin)
    {
      miilisPrevPin = millisCurrent;

      modeControl = 1;
      isManualOn = false;
    }
  }

  // Mode: MANUAL
  if (modeControl == 1)
  {
    // Manual
    if (isManualOn)
    {
      // Display
      lcd.setCursor(0, 0);
      lcd.print("   STATUS: ON   ");
      lcd.setCursor(0, 1);
      lcd.print("   MANUALMODE   ");
  
      // Solenoid
      digitalWrite(SOLENOID, HIGH);
      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDRED, LOW);
    }
  
    // Manual
    if (!isManualOn)
    {
      // Display
      lcd.setCursor(0, 0);
      lcd.print("   STATUS:OFF   ");
      lcd.setCursor(0, 1);
      lcd.print("   MANUALMODE   ");
  
      // Solenoid
      digitalWrite(SOLENOID, LOW);
      digitalWrite(LEDGREEN, LOW);
      digitalWrite(LEDRED, HIGH);
    }
  }

  // Mode: AUTO
  else
  {
    // Timer
    if ((unsigned long)(millisCurrent - miilisPrevTime) >= millisIntervalTime)
    {
      miilisPrevTime = millisCurrent;
      countdown_time -= 1;
  
      if (countdown_time <= 0)
      {
        countdown_time = 0;
      }
    }
  
    // Timer Run
    if (countdown_time > 0) 
    {
      // Display
      lcd.setCursor(0, 0);
      lcd.print("   STATUS: ON   ");
      long countdown_hour = countdown_time / 3600;
      long countdown_minute = ((countdown_time / 60)%60);
      long countdown_sec = countdown_time % 60;
      lcd.setCursor(0, 1);
      lcd.print("    ");
      if (countdown_hour < 10) {
        lcd.print("0");
      }
      lcd.print(countdown_hour);
      lcd.print(":");
      if (countdown_minute < 10) {
        lcd.print("0");
      }
      lcd.print(countdown_minute);
      lcd.print(":");
      if (countdown_sec < 10) {
        lcd.print("0");
      }
      lcd.print(countdown_sec);
      lcd.print("    ");
  
      // Solenoid
      digitalWrite(SOLENOID, HIGH);
      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDRED, LOW);
    }
  
    // Timer Off
    else
    {
      // Display
      lcd.setCursor(0, 0);
      lcd.print("   STATUS:OFF   ");
      lcd.setCursor(0, 1);
      lcd.print("    00:00:00    ");
  
      // Solenoid
      digitalWrite(SOLENOID, LOW);
      digitalWrite(LEDGREEN, LOW);
      digitalWrite(LEDRED, HIGH);
    }
  }
}
