/*
 I, Sam Gershkovich, student number 000801766, certify that this material is my original work.
 No other person's work has been used without due acknowledgment and neither have I made my work available to anyone else.

 Stage 5
*/

#include <Arduino.h>

#define PIN_PIR D5
#define PIN_BUTTON D6
#define LOOP_DELAY 1
#define LED_ON_TIME 125

// *************************************************************
void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, HIGH);
}

bool alarmActivated = false;
bool alarmDeactivated = false;
bool buttonReleased = true;
bool triggered = false;
bool bPIR = false;
int timeSinceTriggered = 0;
bool ledState = true;

void checkButtonPressed()
{
  if (digitalRead(PIN_BUTTON) > 0) // button down
  {
    if (buttonReleased) // first button press frame
    {
      buttonReleased = false;
      if (triggered && !alarmActivated)
      {
        alarmDeactivated = true;
        triggered = false;
        timeSinceTriggered = 0;
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else if (alarmDeactivated)
      {
        alarmDeactivated = false;
      }
    }
  }
  else if (digitalRead(PIN_BUTTON) == 0) // button up
    buttonReleased = true;
}

void motionDetection()
{
  if (!triggered && !alarmActivated && !alarmDeactivated)
  {
    bPIR = digitalRead(PIN_PIR);
    if (bPIR)
    {
      bPIR = false;
      triggered = true;
      timeSinceTriggered = 0;
    }
  }
}

void blinkLED()
{
  if (triggered)
  {
    timeSinceTriggered += 1;
    if (timeSinceTriggered >= 10000)
    {
      triggered = false;
      alarmActivated = true;
      timeSinceTriggered = 0;
      digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
      if (timeSinceTriggered % LED_ON_TIME == 0)
      {
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
      }
    }
  }
}

// *************************************************************
void loop()
{
  checkButtonPressed();

  motionDetection();

  blinkLED();

  delay(LOOP_DELAY);
}