// Left motor controller
int LeftRPWM = 10;//3;
int LeftLPWM = 5;
int LeftR_EN = 2;
int LeftL_EN = 4;

// Right motor controller
int RightRPWM = 6;
int RightLPWM = 9;
int RightR_EN = 7;
int RightL_EN = 8;

int leftSpeed = 0;
bool leftReverse = false;
int rightSpeed = 0;
bool rightReverse = false;

void setupMotors()
{
  pinMode(LeftRPWM, OUTPUT);
  pinMode(LeftLPWM, OUTPUT);
  pinMode(LeftR_EN, OUTPUT);
  pinMode(LeftL_EN, OUTPUT);
  
  pinMode(RightRPWM, OUTPUT);
  pinMode(RightLPWM, OUTPUT);
  pinMode(RightR_EN, OUTPUT);
  pinMode(RightL_EN, OUTPUT);
}

void motorSet(int motor, int speed, bool reverse)
{
  if (motor == motorLeft)
  {
    if (speed > 125 && (leftSpeed == 0 || leftReverse != reverse)) // Soft start
      speed = 125;

    leftSpeed = speed;
    leftReverse = reverse;
    
    digitalWrite(LeftL_EN, HIGH);
    digitalWrite(LeftR_EN, HIGH);
    
    if (reverse)
    {
      analogWrite(LeftRPWM, 0);
      analogWrite(LeftLPWM, speed);
    }
    else
    {
      analogWrite(LeftLPWM, 0);
      analogWrite(LeftRPWM, speed);
    }
  }

  if (motor == motorRight)
  {
    if (speed > 125 && (rightSpeed == 0 || rightReverse != reverse)) // Soft start
      speed = 125;

    rightSpeed = speed;
    rightReverse = reverse;
    
    digitalWrite(RightL_EN, HIGH);
    digitalWrite(RightR_EN, HIGH);
    
    if (reverse)
    {
      analogWrite(RightRPWM, 0);
      analogWrite(RightLPWM, speed);
    }
    else
    {
      analogWrite(RightLPWM, 0);
      analogWrite(RightRPWM, speed);
    }
  }
}
