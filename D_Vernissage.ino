void Vernissage()
{
lcd.clear();
lcd.print("Vernissage");
delay(2000);
digitalWrite (YellowLedPin, HIGH);
digitalWrite (GreenLedPin, LOW);
lcd.clear();
lcd.print("On ne bouge plus");
lcd.setCursor(0,1);
lcd.print("C'est parti !");
delay(2000);
lcd.clear();
lcd.print("Vernissage");
lcd.setCursor(0,1);
lcd.print("en cours");
Moteur1_Injecteur.setMaxSpeed(VernissageSpeed[0]);
Moteur2_Roulis.setMaxSpeed(VernissageSpeed[1]);
Moteur3_Tangage.setMaxSpeed(VernissageSpeed[2]);
Moteur4_Profondeur.setMaxSpeed(VernissageSpeed[3]);
Moteur5_Largeur.setMaxSpeed(VernissageSpeed[4]);
Moteur6162_Hauteur.setMaxSpeed(VernissageSpeed[5]);

Moteur1_Injecteur.setAcceleration(VernissageAcceleration[0]);
Moteur2_Roulis.setAcceleration(VernissageAcceleration[1]);
Moteur3_Tangage.setAcceleration(VernissageAcceleration[2]);
Moteur4_Profondeur.setAcceleration(VernissageAcceleration[3]);
Moteur5_Largeur.setAcceleration(VernissageAcceleration[4]);
Moteur6162_Hauteur.setAcceleration(VernissageAcceleration[5]);


for (int j = 0; j <= 19; j++)
    {
    Mouvement(6*j);
    }
}


void Mouvement(int i)
{
/*if ((abs(StepsVernissage[i])< abs(Steps_MaxFromHoming_Moteur1)) 
&& (abs(StepsVernissage[i+1])< abs(Steps_MaxFromHoming_Moteur2)) 
&& (abs(StepsVernissage[i+2])< abs(Steps_MaxFromHoming_Moteur3))
&& (abs(StepsVernissage[i+3])< abs(Steps_MaxFromHoming_Moteur4))
&& (abs(StepsVernissage[i+4])< abs(Steps_MaxFromHoming_Moteur5))
&& (abs(StepsVernissage[i+5])< abs(Steps_MaxFromHoming_Moteur6162)))
    {*/
    Moteur1_Injecteur.moveTo(StepsVernissage[i]);
    Moteur2_Roulis.moveTo(StepsVernissage[i+1]);
    Moteur3_Tangage.moveTo(StepsVernissage[i+2]);
    Moteur4_Profondeur.moveTo(StepsVernissage[i+3]);
    Moteur5_Largeur.moveTo(StepsVernissage[i+4]);
    Moteur6162_Hauteur.moveTo(StepsVernissage[i+5]);
    
    while ((Moteur1_Injecteur.distanceToGo() != 0) || (Moteur2_Roulis.distanceToGo() != 0) || (Moteur3_Tangage.distanceToGo() != 0) ||
            (Moteur4_Profondeur.distanceToGo() != 0) || (Moteur5_Largeur.distanceToGo() != 0) || (Moteur6162_Hauteur.distanceToGo() != 0))
        {
        Moteur1_Injecteur.run();
        Moteur2_Roulis.run();
        Moteur3_Tangage.run();
        Moteur4_Profondeur.run();
        Moteur5_Largeur.run();
        Moteur6162_Hauteur.run();
        }
}