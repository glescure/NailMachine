  /* tant que j'active pas le switch, le moteur tourne CW

  while (digitalRead(Switch_Home_Moteur1))
  {
    Moteur1_Injecteur.setSpeed(HomingSpeed_28BYJ48);
    Moteur1_Injecteur.step(10);
  }

  delay(2000);

  // le moteur fait un demi-tour CCW
  Moteur1_Injecteur.setSpeed(HomingSpeed_28BYJ48);
  Moteur1_Injecteur.step(-StepsPerRevolution_28BYJ48 / 2);

  delay(2000);*/
