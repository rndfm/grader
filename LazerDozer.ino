#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

float plumOffset = 0;
float plumDeadband = 0.5;

int mode = 0;
int selectMode = 0;
bool menuActive = false;
bool settingActive = false;
int activeSetting = 0;
bool graderMotorEnabled = false;


const int manualMode = 0;
const int graderMode = 1;

const int calibrateLevel = 0;
const int showGraderInfo = 1;
const int setMode = 2;

const int buttonLeftUp = A0;
const int buttonLeftDown = A3;
const int buttonRightUp = A1;
const int buttonRightDown = A2;

const int motorLeft = 0;
const int motorRight = 1;

// 0 = unknown, 1 = way too low, 2 = too low, 3 = level, 4 = too high, 5 = way too high.
const int gradeNoSignal = 0;
const int gradeWayHigh = 5;
const int gradeHigh = 4;
const int grade = 3;
const int gradeLow = 2;
const int gradeWayLow = 1; 

void setup()
{
  setupLCD();
  lcd.clear();
  lcd.print("Lazerdozer");
  lcd.setCursor(0,1);
  lcd.print("starting up...");
  Serial.begin(9600); // open the serial port at 9600 bps:
  Serial.println("Lazerdozer starting up...");

  delay(1000);
  
  setupMotors();
  initAccelerometer();
  setupAverage();
  setupLazerReceiver();
  

  pinMode(buttonLeftUp, INPUT_PULLUP);
  pinMode(buttonLeftDown, INPUT_PULLUP);
  pinMode(buttonRightUp, INPUT_PULLUP);
  pinMode(buttonRightDown, INPUT_PULLUP);

  showMode();
}

bool buttonPressed(int button)
{
  return !digitalRead(button);
}

void showMode()
{
  switch (mode)
  {
    case manualMode:
    lcd.clear();
    lcd.print("Manual");
    break;
    
    case graderMode:
    lcd.clear();
    lcd.print("Grader");
    lcd.setCursor(13, 0);
    if (graderMotorEnabled)
    {
      lcd.print(" ON");
    }
    else
    {
      lcd.print("OFF");
    }
    break;
  }
}

void handleGradeMode()
{
  int gradeState = getGradeState();
  int leftState = gradeState;
  int rightState = gradeState;
  float level = average(getLevel());
  level -= plumOffset;
  
  String gradeName = getStateName(gradeState);
  int motorLeftSpeed = 0;
  bool motorLeftReverse = false;
  int motorRightSpeed = 0;
  bool motorRightReverse = false;
  
  if (gradeState == 0) // No signal
  {
    lcdprintln(gradeName, 1, 15);
    lcd.setCursor(15, 1);
    lcd.write(byte(0));
  }
  else
  {
    if (level + plumDeadband < 0) // Tilt to the left
    {
      leftState = gradeState - 1;
      rightState = gradeState + 1;
    } 
    else if (level - plumDeadband > 0) // Tilt to the right
    {
      leftState = gradeState + 1;
      rightState = gradeState - 1;
    }

    if (leftState < 1)
      leftState = 1;
    if (leftState > 5)
      leftState = 5;

    if (rightState < 1)
      rightState = 1;
    if (rightState > 5)
      rightState = 5;
      
   
    if (leftState > 3) // grade left high 
    {
      motorLeftSpeed = 255;
      motorLeftReverse = true;
    }

    if (rightState > 3) // grade right high 
    {
      motorRightSpeed = 255;
      motorRightReverse = true;
    }

    if (leftState < 3) // grade left low
    {
      motorLeftSpeed = 255;
    }

    if (rightState < 3) // grade right low
    {
      motorRightSpeed = 255;
    }
    
    lcdprintln(gradeName, 1, 13);
    lcd.setCursor(13, 1);
    lcd.write(byte(leftState));
    lcd.write(byte(gradeState));
    lcd.write(byte(rightState));
  }

  if (graderMotorEnabled)
  {
    motorSet(motorLeft, motorLeftSpeed, motorLeftReverse);
    motorSet(motorRight, motorRightSpeed, motorRightReverse);
  }
  else
  {
    motorSet(motorLeft, 0, false);
    motorSet(motorRight, 0, false);
  }
  if (buttonPressed(buttonRightUp))
  {
    graderMotorEnabled = !graderMotorEnabled;
    motorSet(motorLeft, 0, false);
    motorSet(motorRight, 0, false);
    showMode();
    delay(250);
  }
  delay(100);
}

void handleManualMode()
{
  int speedLeft = 0;
  bool reverseLeft = false;
  int speedRight = 0;
  bool reverseRight = false;
  
  if (buttonPressed(buttonLeftUp) && !buttonPressed(buttonLeftDown))
  {
    // Left motor down
    speedLeft = 255;
  }

  if (buttonPressed(buttonLeftDown) && !buttonPressed(buttonLeftUp))
  {
    // Left motor up
    speedLeft = 255;
    reverseLeft = true;
  }

  if (buttonPressed(buttonRightUp) && !buttonPressed(buttonRightDown))
  {
    // Right motor down
    speedRight = 255;
  }

  if (buttonPressed(buttonRightDown) && !buttonPressed(buttonRightUp))
  {
    // Right motor up
    speedRight = 255;
    reverseRight = true;
  }

  motorSet(motorLeft, speedLeft, reverseLeft);
  motorSet(motorRight, speedRight, reverseRight);

  // print left motor icon
  lcd.setCursor(0, 1);
  if (speedLeft == 0)
  {
    drawGradeIcon(grade);
  }
  if (speedLeft == 255)
  {
    if (reverseLeft)
    {
      drawGradeIcon(gradeLow);
    }
    else
    {
      drawGradeIcon(gradeHigh);
    }
  }

  // print right motor icon
  lcd.setCursor(13, 1);
  if (speedRight == 0)
  {
    drawGradeIcon(grade);
  }
  if (speedRight == 255)
  {
    if (reverseRight)
    {
      drawGradeIcon(gradeLow);
    }
    else
    {
      drawGradeIcon(gradeHigh);
    }
  }
}

void drawGradeIcon(int gradeState)
{
  lcd.write(byte(gradeState));
  lcd.write(byte(gradeState));
  lcd.write(byte(gradeState));
}

void loop()
{
  if (handleMenu())
    return;
  
  switch (mode)
  {
    case manualMode:
    handleManualMode();
    break;
    
    case graderMode:
    handleGradeMode();
    break;
  }
}



