/*
 ****************************  DEMONSTRATEUR TOBBY - Code principal        *****************************************
*/

//LIBRAIRIES********************************************************************************************************

//  AccelStepper, gestion du LCD
#include <LiquidCrystal.h>
#include <AccelStepper.h>
#include "Tobby.h"
// Manque encore : le port série qui parle avec le raspberryPi. en digital il me reste 2 à 7 sûr, peut-être le 0 et le 1. on est bien.
// si je veux mettre une caméra a priori je n'ai besoin que de 4 pins.


//CONSTANTES********************************************************************************************************

//Tableau des steps pour la partie vernissage
const int StepsVernissage[]={0, 0, 0, -13, 0, -570, 0, 0, 5, -5, 0, -570, 0, 0, -20, -73, 0, -570, 0, 0, -10, -98, 0, -520, 0, 0, -10, -98, 0, -370, 0, 0, 0, -13, 0, -270, 0, 85, 0, -13, -163, -270, 0, 85, 0, -9, -163, -600, 0, 85, 5, 0, -163, -600, 0, 82, -20, -73, -159, -600, 0, 85, -10, -98, -159, -550, 0, 82, -10, -98, -159, -370, 0, 0, 0, -13, 0, -270, 0, -85, 0, -13, 163, -270, 0, -85, 0, -9, 163, -600, 0, -85, 5, 0, 163, -600, 0, -82, -20, -73, 159, -600, 0, -82, -10, -98, 159, -550, 0, -82, -10, -98, 159, -370, 0, 0, 0, 0, 0, -20, 
};

// Sens de rotation de chacun des moteurs (1=CW, -1=CCW) - on parle du sens pour aller au point de homing à l'initialisation
const int Sens_Rotation_Moteur1 = 1; //CONFIRME
const int Sens_Rotation_Moteur2 = 1;//CONFIRME 
const int Sens_Rotation_Moteur3 = -1; //CONFIRME
const int Sens_Rotation_Moteur4 = 1;//CONFIRME
const int Sens_Rotation_Moteur5 = -1; //CONFIRME
const int Sens_Rotation_Moteur6162 = 1; //CONFIRME Attention au branchement, broche métallique non visible (vers l'intérieur)

// Nombre de pas par moteur pour qu'ils se positionnent en position ready to go depuis le point de Homing
const int Steps_ReadyToGo_Moteur1 = -1000; //CONFIRME - INJECTEUR
const int Steps_ReadyToGo_Moteur2 = -26; // CONFIRME - ROULIS
const int Steps_ReadyToGo_Moteur3 = 8; // CONFIRME - TANGUAGE
const int Steps_ReadyToGo_Moteur4 = -120; //CONFIRME - PROFONDEUR
const int Steps_ReadyToGo_Moteur5 = 160; //CONFIME - LARGEUR
const int Steps_ReadyToGo_Moteur6162 = -300; //CONFIRME - HAUTEUR - 

//  Nombre de pas max par moteur depuis son point de Homing
const int Steps_MaxFromHoming_Moteur1 = -10000; //TBD
const int Steps_MaxFromHoming_Moteur2 = -500; //CONFIRME
const int Steps_MaxFromHoming_Moteur3 = 250; //CONFIRME
const int Steps_MaxFromHoming_Moteur4 = -1100; //TBD
const int Steps_MaxFromHoming_Moteur5 = 2500;//TBD
const int Steps_MaxFromHoming_Moteur6162 = -1000; //CONFIRME

//Valeurs de vitesse et d'accélération pour le setup
       const int HomingSpeed[] = {500, 80, 60, 150, 150, 150}; // décalé pour faciliter la lecture
const int HomingAcceleration[] = {500, 40, 40, 150, 150, 150};

//Valeurs de vitesse et d'accélération pour le vernissage
       const int VernissageSpeed[] = {500, 80, 60, 150, 150, 150}; // décalé pour faciliter la lecture
const int VernissageAcceleration[] = {500, 40, 40, 150, 150, 150};

//il faudrait idéalement être plsu lent pour le roulis et le tanguage

// Pins des switchs pour le homing
const int Switch_Home_Moteur1 = 8;
const int Switch_Home_Moteur2 = 9;
const int Switch_Home_Moteur3 = 10;
const int Switch_Home_Moteur4 = 11;
const int Switch_Home_Moteur5 = 12;
const int Switch_Home_Moteur6162 = 13; // le moteur 62 aura le même switch.

//Pins de connexion des moteurs avec initialisation de l'instance tant que stepper (séquence 1-3-2-4 par rapport aux pins pour le 28BYJ48)
AccelStepper Moteur1_Injecteur(4, 22, 24, 23, 25);  
AccelStepper Moteur2_Roulis(AccelStepper::DRIVER, 26, 27); //step sur 26 et Dir sur 27
AccelStepper Moteur3_Tangage(AccelStepper::DRIVER, 28, 29); //step sur 28 et Dir sur 29
AccelStepper Moteur4_Profondeur(AccelStepper::DRIVER, 30, 31); //step sur 30 et Dir sur 31
AccelStepper Moteur5_Largeur(AccelStepper::DRIVER, 32, 33); // step sur 32 et Dir sur 33
AccelStepper Moteur6162_Hauteur (AccelStepper::DRIVER, 38, 39); //step sur 38 et Dir sur 39

// Les pins de l'écran LCD
LiquidCrystal lcd(40, 41, 42, 43, 44, 45); // rs, enable, d4, d5, d6, d7  - violet - marron - gris - blanc - jaune - orange
//modèle :  rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

//la Pin du joystick, en analog
int JoystickPin = A15;

//Pin des LED de suivi:
const int GreenLedPin = 48;
const int YellowLedPin = 46;
const int RedLedPin = 47;

//Boutons de sélection des 2 modes (1 (gauche) reglage injecteur, 2 (droite) Vernissage)
const int InjectionPin = 49; //violet
const int VernissagePin = 50; //gris


// Pas par révolution
const float StepsPerRevolution_28BYJ48 = 32;
const float StepsPerRevolution_Nema17 = 200;


//VARIABLES ************************************************************************************************************************

//Variable pour incrémenter les pas de tous les moteurs pendant le Homing - Initialisé à 1 (la direction donne le - si besoin)
long StepsHoming = 1;

//Variables pour récupérer la valeur du Joystick & incrémenter les pas pendant l'injection
int ValJoystick = 0;
long StepsInjection = 0;

//Compteur de pas pendant le vernissage. Le homing va l'initialiser aux pas ReadyToGo de chaque moteur
long Steps_Counter_Moteur1 = 0;
long Steps_Counter_Moteur2  = 0;
long Steps_Counter_Moteur3 = 0;
long Steps_Counter_Moteur4 = 0;
long Steps_Counter_Moteur5 = 0;
long Steps_Counter_Moteur6162 = 0;


//Variable pour allumer la LED rouge si besoin
boolean yaunprob = 0;

//Varible pour faire défiler du texte
int positionCounter = 0;

//Variables pour les boucles
int i = 0;
int j = 0;


//SETUP*****************************************************************************************************************************

void setup()
{
//Lancement de l'écran LCD
lcd.begin(16,2); //A verifier
lcd.print("Welcome !");

//Lancement du port série
Serial.begin(9600);

//setup en input des 6 switchs
pinMode(Switch_Home_Moteur1, INPUT_PULLUP);
pinMode(Switch_Home_Moteur2, INPUT_PULLUP);
pinMode(Switch_Home_Moteur3, INPUT_PULLUP);
pinMode(Switch_Home_Moteur4, INPUT_PULLUP);
pinMode(Switch_Home_Moteur5, INPUT_PULLUP);
pinMode(Switch_Home_Moteur6162, INPUT_PULLUP);

//Setup en input des 2 boutons de sélection du mode.
pinMode (VernissagePin,INPUT);
pinMode(InjectionPin, INPUT);

//Setup en input du joystick
pinMode(JoystickPin, INPUT);

//Setup en output des 3 led de suivi
pinMode (GreenLedPin, OUTPUT);
pinMode (YellowLedPin, OUTPUT);
pinMode (RedLedPin, OUTPUT);

// On éteint toutes les LED par défaut après les avoir allumés 1 seconde pour checker que c'est good
delay(2000);
digitalWrite (GreenLedPin, HIGH);
delay(600);
digitalWrite (GreenLedPin, LOW);
digitalWrite (YellowLedPin, HIGH);
delay(600);
digitalWrite (YellowLedPin, LOW);
digitalWrite (RedLedPin, HIGH);
delay(600);
digitalWrite (RedLedPin, LOW);

// on met à zéro la variable d'identification de souci.
yaunprob = 0;

// on fait le homing et on se met en position "ready to go"
delay(2000);
HomingAccel();

}



//LOOP ********************************************************************

void loop()
{
digitalWrite (GreenLedPin, HIGH);
digitalWrite (YellowLedPin, LOW);
digitalWrite (RedLedPin, LOW);
lcd.clear();
lcd.home();
lcd.print("<< Injection");
lcd.setCursor(0,1);
lcd.print("Vernissage >>");
delay(3000);
HomingAccel();
//Vernissage();
}
/*
if (yaunprob == 1)
  {
    lcd.clear();
    lcd.print("On a un pb :(");
  }
    else if (digitalRead(InjectionPin) == HIGH)
          {
            Injection();
          }
              else if (digitalRead(VernissagePin) == HIGH)
              {
                Vernissage();
              }
*/

/*
if (digitalRead(InjectionPin) == HIGH)
          {
            Injection();
          }
              else if (digitalRead(VernissagePin) == HIGH)
              {
                Vernissage();
              }
}
*/

//Vernissage();
//lcd.clear();
//lcd.print(Moteur4_Profondeur.currentPosition());
//delay(5000);

/*
delay(1000);

lcd.clear();
lcd.print(Moteur2_Roulis.currentPosition());
delay(5000);
i=0;

    Moteur1_Injecteur.setAcceleration(VernissageAcceleration[0]);
    Moteur2_Roulis.setAcceleration(VernissageAcceleration[1]);
    Moteur3_Tangage.setAcceleration(VernissageAcceleration[2]);
    Moteur4_Profondeur.setAcceleration(VernissageAcceleration[3]);
    Moteur5_Largeur.setAcceleration(VernissageAcceleration[4]);
    Moteur6162_Hauteur.setAcceleration(VernissageAcceleration[5]);

    Moteur1_Injecteur.setMaxSpeed(VernissageSpeed[0]);
    Moteur2_Roulis.setMaxSpeed(VernissageSpeed[1]);
    Moteur3_Tangage.setMaxSpeed(VernissageSpeed[2]);
    Moteur4_Profondeur.setMaxSpeed(VernissageSpeed[3]);
    Moteur5_Largeur.setMaxSpeed(VernissageSpeed[4]);
    Moteur6162_Hauteur.setMaxSpeed(VernissageSpeed[5]);

    
    Moteur1_Injecteur.moveTo(StepsVernissage[i]);
    Moteur2_Roulis.moveTo(StepsVernissage[i+1]);
    Moteur3_Tangage.moveTo(StepsVernissage[i+2]);
    Moteur4_Profondeur.moveTo(StepsVernissage[i+3]);
    Moteur5_Largeur.moveTo(StepsVernissage[i+4]);
    Moteur6162_Hauteur.moveTo(StepsVernissage[i+5]);
    Moteur1_Injecteur.runToPosition();
    Moteur2_Roulis.runToPosition();
    Moteur3_Tangage.runToPosition();
    Moteur4_Profondeur.runToPosition();
    Moteur5_Largeur.runToPosition();
    Moteur6162_Hauteur.runToPosition();
}
*/

