const int lcdChars = 16;


byte charNoSignal[8] =
{
0b00000,
0b10100,
0b01000,
0b10100,
0b00001,
0b00101,
0b10101,
0b10101
};

byte charHigh2[8] =
{
0b11111,
0b11111,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000
};

byte charHigh[8] =
{
0b00000,
0b00000,
0b11111,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000
};

byte charGrade[8] =
{
0b00000,
0b00000,
0b00000,
0b11111,
0b11111,
0b00000,
0b00000,
0b00000
};

byte charLow[8] =
{
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b11111,
0b00000,
0b00000
};

byte charLow2[8] =
{
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b11111,
0b11111
};

void setupLCD()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
  
  // create a new character
  lcd.createChar(0, charNoSignal);
  lcd.createChar(1, charLow2);
  lcd.createChar(2, charLow);
  lcd.createChar(3, charGrade);
  lcd.createChar(4, charHigh);
  lcd.createChar(5, charHigh2);
}

void clearLCDLine(int line)
{               
  lcd.setCursor(0,line);
  for(int n = 0; n < lcdChars; n++)
  {
    lcd.print(" ");
  }
  lcd.setCursor(0,line);
}

void lcdprintln(String msg, int line)
{
  lcdprintln(msg, line, lcdChars);
}

void lcdprintln(String msg, int line, int clearLength)
{
  lcd.setCursor(0,line);
  int length = lcd.print(msg);
  for(int n = length; n <= clearLength - 1; n++)
  {
    lcd.print(" ");
  }
}
