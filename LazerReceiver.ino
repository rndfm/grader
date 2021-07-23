const int dataPin = 3;

long pulseStartTime = 0;
long lastPulseTime = 0;
int pulses = 0;
volatile byte gradeState = 0; // 0 = unknown, 1 = way too low, 2 = too low, 3 = level, 4 = too high, 5 = way too high.

byte getGradeState()
{
  return gradeState;
}

void setupLazerReceiver() {
  pinMode(dataPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(dataPin), interrupt, CHANGE);
}

void interrupt() {
  Serial.println("I");
  if (digitalRead(dataPin) == HIGH)
  {
    // Rising
    long duration = micros() - pulseStartTime;
    if (duration < 1400)
    {
      if (millis() - lastPulseTime < 250)
      {
        
        if (pulses < 2)
        {
          pulses++;
        }
        else
          gradeState = toState(duration);  
      }
      else
      {
        gradeState = 0;
        pulses = 0;
      }

      lastPulseTime = millis();
    }
  }
  else
  {
    // Falling
    pulseStartTime = micros();
  }
}

byte toState(long duration)
{
  switch (duration) {
    case 230 ... 270:
    return 1;
    case 530 ... 570:
    return 2;
    case 780 ... 850:
    return 3;
    case 1030 ... 1100:
    return 4;
    case 1290 ... 1400:
    return 5;
    default:
    return 0;
  }
}

String getStateName(int state)
{
  switch (state)
  {
    case 0:
    return "No signal";

    case 1:
    return "Way too low";

    case 2:
    return "Low";

    case 3:
    return "On grade";

    case 4:
    return "High";

    case 5:
    return "Way too high";

    default:
    return "";
  }
}
