void Injection()
{
  lcd.clear();
  lcd.print("Injection");
  
  delay(3000);
  digitalWrite (YellowLedPin, HIGH);
  digitalWrite (GreenLedPin, LOW);
  lcd.clear();
  lcd.print("Appuyez encore");
  lcd.setCursor(0,1);
  lcd.print("quand c'est ok");
  Serial.print(digitalRead(InjectionPin));


  while (digitalRead(InjectionPin) == LOW)
    {
      ValJoystick = analogRead(JoystickPin); 
          while ((ValJoystick < 250))
          {
            Moteur1_Injecteur.moveTo(-StepsInjection);
            Moteur1_Injecteur.run();
            StepsInjection++;
            delay(1);
            ValJoystick = analogRead(JoystickPin);
          }
        Moteur1_Injecteur.setCurrentPosition(0);
        StepsInjection=0;
            while ((ValJoystick > 750))
            {
              Moteur1_Injecteur.moveTo(StepsInjection);
              Moteur1_Injecteur.run();
              StepsInjection++;
              delay(1);
              ValJoystick = analogRead(JoystickPin);
            }
        Moteur1_Injecteur.setCurrentPosition(0);
        StepsInjection=0;
    }
  lcd.clear();  
}

