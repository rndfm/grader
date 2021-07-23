void showMenu()
{
  lcd.clear();
  lcd.print("Menu");
  lcd.setCursor(0, 1);
  switch (activeSetting)
  {
    case calibrateLevel:
    lcd.print("Calibrate level");
    break;
    
    case showGraderInfo:
    lcd.print("Show grader info");
    break;

    case setMode:
    lcd.print("Set mode");
    break;
  }
  
}

void handleCalibrateLevel()
{
    lcd.clear();
    lcd.print("Cur:");
    lcd.setCursor(12, 0);
    lcd.print("Set:");
    lcd.setCursor(0, 1);
    float avg = average(getLevel());
    lcd.print(avg);

    lcd.setCursor(11, 1);
    lcd.print(plumOffset);
    delay(100);
    //Check if save button is pressed.
    if (!digitalRead(buttonLeftDown))
    {
      plumOffset = avg;
    }
}

void handleSetMode()
{
    lcd.clear();
    lcd.print("Mode:");
    lcd.setCursor(0, 1);

    if (buttonPressed(buttonRightUp))
    {
      selectMode--;
      if (selectMode < manualMode)
        selectMode = graderMode;
    }

    if (buttonPressed(buttonRightDown))
    {
      selectMode++;
      if (selectMode > graderMode)
        selectMode = manualMode;
    }

    switch (selectMode)
    {
      case manualMode:
      lcd.print("Manual");
      break;

      case graderMode:
      lcd.print("Grader");
      break;
    }


    
    //Check if save button is pressed.
    if (!digitalRead(buttonLeftDown))
    {
      mode = selectMode;
      settingActive = false;
      menuActive = false;
      showMode();
    }

    delay(250);
}

bool handleMenu()
{
    if (!digitalRead(buttonLeftUp) && !digitalRead(buttonLeftDown))
    {
      // Entering menu. Stop motors.
      motorSet(motorLeft, 0, false);
      motorSet(motorRight, 0, false);
      menuActive = true;
      showMenu();
      delay(1000);
      return true;
    }
    
    if (!menuActive)
    {
      return false;
    }
    
    if (buttonPressed(buttonLeftUp))
    {
      //Exit menu/setting.
      if (settingActive)
      {
        settingActive = false;
        showMenu();
        delay(250);
        return true;
      }
      
      menuActive = false;
      showMode();
      delay(250);
      return true;
    }

    if (settingActive)
    {
      switch (activeSetting)
      {
        case calibrateLevel:
        handleCalibrateLevel();
        break;

        case setMode:
        handleSetMode();
        break;
      }
      
      return true;
    }
    
    if (buttonPressed(buttonRightUp))
    {
      activeSetting--;
      if (activeSetting < calibrateLevel)
        activeSetting = setMode;

      showMenu();
      delay(250);
    }

    if (buttonPressed(buttonRightDown))
    {
      activeSetting++;
      if (activeSetting > setMode)
        activeSetting = calibrateLevel;

      showMenu();
      delay(250);
    }

    if (buttonPressed(buttonLeftDown))
    {
      settingActive = true;
      lcd.clear();
      delay(250);
    }

    delay(100);
    return true;
}
