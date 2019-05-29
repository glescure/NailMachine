#include <AccelStepper.h>
#include <LiquidCrystal.h>

// Les pins de l'écran LCD
LiquidCrystal lcd(44, 45, 46, 47, 48, 49); // rs, enable, d4, d5, d6, d7

// Pins des switchs pour le homing
const int Switch_Home_Moteur1 = 8;
const int Switch_Home_Moteur2 = 9;
const int Switch_Home_Moteur3 = 10;
const int Switch_Home_Moteur4 = 11;
const int Switch_Home_Moteur5 = 12;
const int Switch_Home_Moteur6162 = 13; // le moteur 62 aura le même switch.

AccelStepper Moteur1_Injecteur(4, 22, 24, 23, 25);
AccelStepper Moteur2_Roulis(4, 26, 28, 27, 29);
AccelStepper Moteur3_Tangage(4, 30, 32, 31, 33);
AccelStepper Moteur4_Profondeur(4, 34, 36, 35, 37);
AccelStepper Moteur5_Largeur(4, 38, 40, 39, 41);
AccelStepper Moteur6162_Hauteur (1, 42, 43); //step sur 42 et Dir sur 43

// Pas par révolution
const float StepsPerRevolution_28BYJ48 = 32;
const float StepsPerRevolution_Nema17 = 200;

int Steps_Counter_Moteur1;
// Vitesse de Homing (max) & Acceleration
//const float HomingSpeed_28BYJ48 = 600.0; //il s'agit de steps par minute
//const int HomingAcceleration_28BYJ48 = 500; // les moteurs accélerent/décélèrent à xxx steps/s par seconde
//const float HomingSpeed_Nema17= 100.0; // il s'agit de microsecondes entre chaque pas. Mis par défaut à 9600 pour être équivalente à 1000.
//const int HomingAcceleration_Nema17 = 100; // les moteurs accélerent/décélèrent à 100 steps/s par seconde

// Vitesse de Vernissage (max) & Acceleration
//const float VernissageSpeed_28BYJ48 = 1000.0; //il s'agit de steps par minute
//const int VernissageAcceleration_28BYJ48= 250;
//const float VernissageSpeed_Nema17= 800;
//const int VernissageAcceleration_Nema17 = 300;

       const int HomingSpeed[] = {1000, 200, 200, 600, 600, 100}; // décalé pour faciliter la lecture
const int HomingAcceleration[] = {1000, 200, 200, 500, 500, 100};

       const int VernissageSpeed[] = {500, 100, 100, 1000, 1000, 200}; // décalé pour faciliter la lecture
const int VernissageAcceleration[] = {500, 100, 100, 250, 250, 200};

//  Nombre de pas max par moteur depuis son point de Homing
const int Steps_MaxFromHoming_Moteur1 = 6000; //TBD
const int Steps_MaxFromHoming_Moteur2 = -500; //CONFIRME
const int Steps_MaxFromHoming_Moteur3 = 250; //CONFIRME
const int Steps_MaxFromHoming_Moteur4 = -1000; //TBD
const int Steps_MaxFromHoming_Moteur5 = 2500;//TBD
const int Steps_MaxFromHoming_Moteur6162 = -1000; //CONFIRME


// Nombre de pas par moteur pour qu'ils se positionnent en position ready to go depuis le point de Homing
const int Steps_ReadyToGo_Moteur1 = -3000; //CONFIRME a 300 - 1000 pour la demo
const int Steps_ReadyToGo_Moteur2 = -265; //CONFIRME
const int Steps_ReadyToGo_Moteur3 = 150; //CONFIRME
const int Steps_ReadyToGo_Moteur4 = -500; //CONFIRME
const int Steps_ReadyToGo_Moteur5 = 1700; //CONFIME
const int Steps_ReadyToGo_Moteur6162 = -400; //CONFIRME


// Sens de rotation de chacun des moteurs (1=CW, -1=CCW) - on parle du sens pour aller au point de homing à l'initialisation
const int Sens_Rotation_Moteur1 = 1; //CONFIRME
const int Sens_Rotation_Moteur2 = 1;//CONFIRME 
const int Sens_Rotation_Moteur3 = -1; //CONFIRME
const int Sens_Rotation_Moteur4 = 1;//CONFIRME
const int Sens_Rotation_Moteur5 = -1; //CONFIRME
const int Sens_Rotation_Moteur6162 = 1; //CONFIRME Attention au branchement, broche métallique non visible (vers l'intérieur)

//Variable pour incrémenter les pas de tous les moteurs pendant le Homing - Initialisé à 1 (la direction donne le - si besoin)
long StepsHoming = 1;

int ValJoystick = 0;
int StepsInjection = 0;

//la Pin du joystick, en analog
int JoystickPin = A15;


//LED de suivi:
const int GreenLedPin = 7;
const int YellowLedPin = 50;
const int RedLedPin = 51;

//Boutons de sélection des 3 modes (1 Vernissage, 2 reglage injecteur, 3 Homing)
const int VernissagePin = 51;
const int InjectionPin = 52;
const int HomingPin = 53;

void setup() {
  //setup en input des 6 switchs
  pinMode(Switch_Home_Moteur1, INPUT_PULLUP);
  pinMode(Switch_Home_Moteur2, INPUT_PULLUP);
  pinMode(Switch_Home_Moteur3, INPUT_PULLUP);
  pinMode(Switch_Home_Moteur4, INPUT_PULLUP);
  pinMode(Switch_Home_Moteur5, INPUT_PULLUP);
  pinMode(Switch_Home_Moteur6162, INPUT_PULLUP);

  //Setup en input des 3 boutons de sélection du mode. CHECKER SI JE PEUX PAS LES METTRE EN PULLUP ET PAS ME TAPER DE RAJOUTER DES CONDENSATEURS
  pinMode (VernissagePin, INPUT);
  pinMode(InjectionPin, INPUT);
  pinMode (HomingPin, INPUT);

  //Setup en input du joystick
  pinMode(JoystickPin, INPUT);

  Serial.begin(9600);

  //setup en output de 2 pins du Nema17
  //pinMode(DirPin_Moteur6162,OUTPUT);
  //pinMode(StepPin_Moteur6162,OUTPUT);

  //Setup en output des 3 led de suivi
  pinMode (GreenLedPin, OUTPUT);
  pinMode (YellowLedPin, OUTPUT);
  pinMode (RedLedPin, OUTPUT);

  digitalWrite (GreenLedPin, HIGH);
  delay(600);
  digitalWrite (GreenLedPin, LOW);
  digitalWrite (YellowLedPin, HIGH);
  delay(600);
  digitalWrite (YellowLedPin, LOW);
  digitalWrite (RedLedPin, HIGH);
  delay(600);
  digitalWrite (RedLedPin, LOW);
  
  lcd.begin(16,2); //A verifier
  lcd.print("Guillaume the best !");
 
  //je le fais au setup quand il n'y a pas de homing.
  Moteur1_Injecteur.setMaxSpeed(1000);  // Set speed fast enough to follow pot rotation
  Moteur1_Injecteur.setAcceleration(1000);
}

void loop() {
  HomingAccelCommon(Moteur6162_Hauteur,Sens_Rotation_Moteur6162,Switch_Home_Moteur6162,Steps_ReadyToGo_Moteur6162, HomingSpeed[5], VernissageSpeed[5], HomingAcceleration[5], VernissageAcceleration[5]);
  delay(500);
  HomingAccelCommon(Moteur1_Injecteur,Sens_Rotation_Moteur1,Switch_Home_Moteur1,Steps_ReadyToGo_Moteur1, HomingSpeed[0], VernissageSpeed[0], HomingAcceleration[0], VernissageAcceleration[0]);
  delay(500);
  HomingAccelCommon(Moteur2_Roulis,Sens_Rotation_Moteur2,Switch_Home_Moteur2,Steps_ReadyToGo_Moteur2, HomingSpeed[1], VernissageSpeed[1], HomingAcceleration[1], VernissageAcceleration[1]);
  delay(500);
  HomingAccelCommon(Moteur3_Tangage,Sens_Rotation_Moteur3,Switch_Home_Moteur3,Steps_ReadyToGo_Moteur3, HomingSpeed[2], VernissageSpeed[2], HomingAcceleration[2], VernissageAcceleration[2]);
  delay(500);
  HomingAccelCommon(Moteur4_Profondeur,Sens_Rotation_Moteur4,Switch_Home_Moteur4,Steps_ReadyToGo_Moteur4, HomingSpeed[3], VernissageSpeed[3], HomingAcceleration[3], VernissageAcceleration[3]);
  delay(500);
  HomingAccelCommon(Moteur5_Largeur,Sens_Rotation_Moteur5,Switch_Home_Moteur5,Steps_ReadyToGo_Moteur5, HomingSpeed[4], VernissageSpeed[4], HomingAcceleration[4], VernissageAcceleration[4]);
  delay(500);

  delay(2000);
  //Injection();
}

//Fonction de homing commune à tous les moteurs
void HomingAccelCommon(AccelStepper Moteur, int Direction, int SwitchMoteur, int StepsReadytoGo, int HomingSpeed, int VernissageSpeed, int HomingAcceleration, int VernissageAcceleration)
{
  Moteur.setMaxSpeed(HomingSpeed);
  Moteur.setAcceleration(HomingAcceleration);
  while (digitalRead(SwitchMoteur))
  {
    //Moteur.setSpeed(HomingSpeed_Nema17);
    Moteur.moveTo(Direction * StepsHoming);
    Moteur.run();
    StepsHoming++;
    delay(5);
  }
  Moteur.setCurrentPosition(0);  // Set the current position as zero for now
  Moteur.setMaxSpeed(HomingSpeed);      // Set Max Speed of Stepper (Slower to get better accuracy)
  Moteur.setAcceleration(HomingAcceleration);  // Set Acceleration of Stepper
  StepsHoming = 1;
  while (!digitalRead(SwitchMoteur)) // Make the Stepper move CW until the switch is deactivated
  {
    Moteur.moveTo(-Direction * StepsHoming);
    Moteur.run();
    StepsHoming++;
    delay(1);
  }
  delay(1000);

  Moteur.setCurrentPosition(0); // on définit désormais le position actuelle comme l'origine
  //Moteur.setMaxSpeed(HomingSpeed_28BYJ48);
  //Moteur.setAcceleration(Acceleration);
  Moteur.moveTo(StepsReadytoGo);
  Moteur.setMaxSpeed(VernissageSpeed);      // Set Max Speed of Stepper (faster)
  Moteur.setAcceleration(VernissageAcceleration);  // Set Acceleration of Stepper
  //Moteur.setSpeed(200.0);

  Moteur.runToPosition();
  /* if ((Moteur.distanceToGo() != 0))
    {
        Moteur.run();  // Move Stepper into position
    }
  */

}

void Injection()
{
  ValJoystick = analogRead(JoystickPin); 
  Serial.print("Données Joystick: ");  
  Serial.println(ValJoystick);
    while ((ValJoystick < 250))
  {
    Moteur1_Injecteur.moveTo(-StepsInjection);
    Moteur1_Injecteur.run();
    StepsInjection++;
    delay(5);
   Serial.println(StepsInjection);
    ValJoystick = analogRead(JoystickPin);
  }
   Moteur1_Injecteur.setCurrentPosition(0);
   StepsInjection=0;
      while ((ValJoystick > 750))
  {
    Moteur1_Injecteur.moveTo(StepsInjection);
    Moteur1_Injecteur.run();
    StepsInjection++;
    delay(5);
    Serial.println(StepsInjection);
    ValJoystick = analogRead(JoystickPin);
  }
  Moteur1_Injecteur.setCurrentPosition(0);
  StepsInjection=0;
}
