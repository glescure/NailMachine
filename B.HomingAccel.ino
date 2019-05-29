void HomingAccel() 
{
digitalWrite (YellowLedPin, HIGH);
lcd.clear();
lcd.print("Init Hauteur");  
HomingAccelCommon(Moteur6162_Hauteur,Sens_Rotation_Moteur6162,Switch_Home_Moteur6162,Steps_ReadyToGo_Moteur6162, HomingSpeed[5], VernissageSpeed[5], HomingAcceleration[5], VernissageAcceleration[5]);
lcd.clear();
lcd.print("Hauteur ok");
delay(500);
lcd.clear();
lcd.print("Init Injecteur");
HomingAccelCommon(Moteur1_Injecteur,Sens_Rotation_Moteur1,Switch_Home_Moteur1,Steps_ReadyToGo_Moteur1, HomingSpeed[0], VernissageSpeed[0], HomingAcceleration[0], VernissageAcceleration[0]);
lcd.clear();
lcd.print("Injecteur ok");
delay(500);
lcd.clear();
lcd.print("Init Roulis");
HomingAccelCommon(Moteur2_Roulis,Sens_Rotation_Moteur2,Switch_Home_Moteur2,Steps_ReadyToGo_Moteur2, HomingSpeed[1], VernissageSpeed[1], HomingAcceleration[1], VernissageAcceleration[1]);
lcd.clear();
lcd.print("Roulis ok");
delay(500);
lcd.clear();
lcd.print("Init Profondeur");
HomingAccelCommon(Moteur4_Profondeur,Sens_Rotation_Moteur4,Switch_Home_Moteur4,Steps_ReadyToGo_Moteur4, HomingSpeed[3], VernissageSpeed[3], HomingAcceleration[3], VernissageAcceleration[3]);
lcd.clear();
lcd.print("Profondeur ok");
delay(500);
lcd.clear();
lcd.print("Init Tangage");
HomingAccelCommon(Moteur3_Tangage,Sens_Rotation_Moteur3,Switch_Home_Moteur3,Steps_ReadyToGo_Moteur3, HomingSpeed[2], VernissageSpeed[2], HomingAcceleration[2], VernissageAcceleration[2]);
lcd.clear();
lcd.print("Tangage ok");
delay(500);
lcd.clear();
lcd.print("Init Largeur");
HomingAccelCommon(Moteur5_Largeur,Sens_Rotation_Moteur5,Switch_Home_Moteur5,Steps_ReadyToGo_Moteur5, HomingSpeed[4], VernissageSpeed[4], HomingAcceleration[4], VernissageAcceleration[4]);
lcd.clear();
lcd.print("Largeur ok");
delay(1000);
digitalWrite (YellowLedPin, LOW);
digitalWrite (GreenLedPin, HIGH);
lcd.clear();
lcd.print("Initialisation");
lcd.setCursor(0,1);
lcd.print("terminee");
delay(3000);
}

//Fonction de homing commune à tous les moteurs
void HomingAccelCommon(AccelStepper Moteur, int Direction, int SwitchMoteur, int StepsReadytoGo, int HomingSpeed, int VernissageSpeed, int HomingAcceleration, int VernissageAcceleration)
{
  Moteur.setMaxSpeed(HomingSpeed);
  Moteur.setAcceleration(HomingAcceleration);
  while (digitalRead(SwitchMoteur))
  {
    Moteur.moveTo(Direction * StepsHoming);
    Moteur.run();
    StepsHoming++;
    //delay(1);
  }
  Moteur.setCurrentPosition(0);  
  Moteur.setMaxSpeed(HomingSpeed);      
  Moteur.setAcceleration(HomingAcceleration);
  StepsHoming = 1;
  while (!digitalRead(SwitchMoteur))
  {
    Moteur.moveTo(-Direction * StepsHoming);
    Moteur.run();
    StepsHoming++;
    //delay(1); 
  }
  delay(500);
  Moteur.setCurrentPosition(0); // on définit désormais le position actuelle comme l'origine - ça sera le zéro pour toute la partie vernissage
  Moteur.moveTo(StepsReadytoGo);
  Moteur.setMaxSpeed(VernissageSpeed);     
  Moteur.setAcceleration(VernissageAcceleration); 
  while (Moteur.distanceToGo() != 0)
    {
      Moteur.run();
    }
        
  //Moteur.runToPosition();
}