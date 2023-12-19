#include <Fuzzy.h>
#include <Servo.h>

//Function to Read Sensor
float sensorRead(int analog)
{
  int value;
  value = analogRead(analog); // read the value from the sensor
  return value;
}

Servo servohori;  //Initialize Horizontal Servo Motor
Servo servoverti; //Initialize Vertical Servo Motor

int servoh = 0; //Store the current Angle of Horizontal Motor
int servov = 0; //Store the current Angle of Vertical Motor

int sensitivity = 1;

//Sensor Reading
int AnalogTop = A2; // select the input pin for LDR
int TopSensor = 0; // variable to store the value coming from the sensor

int AnalogBottom = A1; // select the input pin for LDR
int BottomSensor = 0; // variable to store the value coming from the sensor

int AnalogLeft = A3; // select the input pin for LDR
int LeftSensor = 0; // variable to store the value coming from the sensor

int AnalogRight = A0; // select the input pin for LDR
int RightSensor = 0; // variable to store the value coming from the sensor

// Fuzzy
Fuzzy *fuzzy = new Fuzzy();

// Top Sensor Fuzzy Input
FuzzySet *Tdim = new FuzzySet(0, 0, 100, 200);
FuzzySet *Tnormal = new FuzzySet(150, 250, 250, 350);
FuzzySet *Tbright = new FuzzySet(300, 400, 500, 500);

// Bottom Sensor Fuzzy Input
FuzzySet *Bdim = new FuzzySet(0, 0, 100, 200);
FuzzySet *Bnormal = new FuzzySet(150, 250, 250, 350);
FuzzySet *Bbright = new FuzzySet(300, 400, 500, 500);

// Left Sensor Fuzzy Input
FuzzySet *Ldim = new FuzzySet(0, 0, 100, 200);
FuzzySet *Lnormal = new FuzzySet(150, 250, 250, 350);
FuzzySet *Lbright = new FuzzySet(300, 400, 500, 500);

// Right Sensor Fuzzy Input
FuzzySet *Rdim = new FuzzySet(0, 0, 100, 200);
FuzzySet *Rnormal = new FuzzySet(150, 250, 250, 350);
FuzzySet *Rbright = new FuzzySet(300, 400, 500, 500);

// Vertical Motor Fuzzy Output
FuzzySet *Vlow = new FuzzySet(0, 45, 45, 65);
FuzzySet *Vmedium = new FuzzySet(65, 90, 90, 115);
FuzzySet *Vhigh = new FuzzySet(115, 135, 180, 180);

// Horizontal Motor Fuzzy Output
FuzzySet *Hlow = new FuzzySet(0, 45, 45, 65);
FuzzySet *Hmedium = new FuzzySet(65, 90, 90, 115);
FuzzySet *Hhigh = new FuzzySet(115, 135, 180, 180);


void setup()
{
  servohori.attach(6); 
  servoverti.attach(7); 
  Serial.begin(9600); //sets serial port for communication
  servoverti.write(90);
  servohori.write(90);

  // FuzzyInput Top
  FuzzyInput *sensorTop = new FuzzyInput(1);

  sensorTop->addFuzzySet(Tdim);
  sensorTop->addFuzzySet(Tnormal);
  sensorTop->addFuzzySet(Tbright);
  fuzzy->addFuzzyInput(sensorTop);

  // FuzzyInput Bottom
  FuzzyInput *sensorBottom = new FuzzyInput(2);

  sensorBottom->addFuzzySet(Bdim);
  sensorBottom->addFuzzySet(Bnormal);
  sensorBottom->addFuzzySet(Bbright);
  fuzzy->addFuzzyInput(sensorBottom);

  // FuzzyInput Left
  FuzzyInput *sensorLeft = new FuzzyInput(3);

  sensorLeft->addFuzzySet(Ldim);
  sensorLeft->addFuzzySet(Lnormal);
  sensorLeft->addFuzzySet(Lbright);
  fuzzy->addFuzzyInput(sensorLeft);

  // FuzzyInput Right
  FuzzyInput *sensorRight = new FuzzyInput(4);

  sensorRight->addFuzzySet(Rdim);
  sensorRight->addFuzzySet(Rnormal);
  sensorRight->addFuzzySet(Rbright);
  fuzzy->addFuzzyInput(sensorRight);

  // FuzzyOutput Vertical Motor
  FuzzyOutput *VMotor = new FuzzyOutput(1);

  VMotor->addFuzzySet(Vlow);
  VMotor->addFuzzySet(Vmedium);
  VMotor->addFuzzySet(Vhigh);
  fuzzy->addFuzzyOutput(VMotor);

  // FuzzyOutput Horizontal Motor
  FuzzyOutput *HMotor = new FuzzyOutput(2);

  HMotor->addFuzzySet(Hlow);
  HMotor->addFuzzySet(Hmedium);
  HMotor->addFuzzySet(Hhigh);
  fuzzy->addFuzzyOutput(HMotor);

  // North West
  // Building FuzzyRule Rule 1: if Top Bright And Left Bright then Vertical Servo Motor Low
  FuzzyRuleAntecedent *IfTopBrightAndLeftBright = new FuzzyRuleAntecedent();
  IfTopBrightAndLeftBright->joinWithAND(Tbright, Lbright);
  
  FuzzyRuleConsequent *thenVerticalMotorLow = new FuzzyRuleConsequent();
  thenVerticalMotorLow->addOutput(Vlow);
  
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, IfTopBrightAndLeftBright, thenVerticalMotorLow);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Building FuzzyRule Rule 2: if Top Bright And Left Bright then Horizontal Servo Motor High
  FuzzyRuleConsequent *thenHorizontalMotorHigh = new FuzzyRuleConsequent();
  thenHorizontalMotorHigh->addOutput(Hhigh);
  
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, IfTopBrightAndLeftBright, thenHorizontalMotorHigh);
  fuzzy->addFuzzyRule(fuzzyRule2);

  //North East
  // Building FuzzyRule Rule 3: if Top Bright And Right Bright then Vertical Servo Motor Low
  FuzzyRuleAntecedent *IfTopBrightAndRightBright = new FuzzyRuleAntecedent();
  IfTopBrightAndRightBright->joinWithAND(Tbright, Rbright);
  
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, IfTopBrightAndRightBright, thenVerticalMotorLow);
  fuzzy->addFuzzyRule(fuzzyRule3);

  // Building FuzzyRule Rule 4: if Top Bright And Right Bright then Horizontal Servo Motor Low
  FuzzyRuleConsequent *thenHorizontalMotorLow = new FuzzyRuleConsequent();
  thenHorizontalMotorLow->addOutput(Vlow);
  
  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, IfTopBrightAndRightBright, thenHorizontalMotorLow);
  fuzzy->addFuzzyRule(fuzzyRule4);

  //South East
  // Building FuzzyRule Rule 5: if Bottom Bright And Right Bright then Vertical Servo Motor High
  FuzzyRuleAntecedent *IfBottomBrightAndRightBright = new FuzzyRuleAntecedent();
  IfBottomBrightAndRightBright->joinWithAND(Bbright, Rbright);

  FuzzyRuleConsequent *thenVerticalMotorHigh = new FuzzyRuleConsequent();
  thenVerticalMotorHigh->addOutput(Vhigh);
  
  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, IfBottomBrightAndRightBright, thenVerticalMotorHigh);
  fuzzy->addFuzzyRule(fuzzyRule5);

  // Building FuzzyRule Rule 6: if Bottom Bright And Right Bright then Horizontal Servo Motor High
  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, IfBottomBrightAndRightBright, thenHorizontalMotorHigh);
  fuzzy->addFuzzyRule(fuzzyRule6);

  //South West
  // Building FuzzyRule Rule 7: if Bottom Bright And Left Bright then Vertical Servo Motor High
  FuzzyRuleAntecedent *IfBottomBrightAndLeftBright = new FuzzyRuleAntecedent();
  IfBottomBrightAndLeftBright->joinWithAND(Lbright, Bbright);
  
  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, IfBottomBrightAndLeftBright, thenVerticalMotorHigh);
  fuzzy->addFuzzyRule(fuzzyRule7);

  // Building FuzzyRule Rule 8: if Bottom Bright And Left Bright then Horizontal Servo Motor Low
  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, IfBottomBrightAndLeftBright, thenHorizontalMotorLow);
  fuzzy->addFuzzyRule(fuzzyRule8);

  //Normal Top and Bottom
  // Building FuzzyRule Rule 9: if Bottom Normal And Top Normal then Vertical Servo Motor Medium
  FuzzyRuleAntecedent *IfBottomNormalAndTopNormal = new FuzzyRuleAntecedent();
  IfBottomNormalAndTopNormal->joinWithAND(Bnormal, Tnormal);

  FuzzyRuleConsequent *thenVerticalMotorMedium = new FuzzyRuleConsequent();
  thenVerticalMotorMedium->addOutput(Vmedium);
  
  FuzzyRule *fuzzyRule9 = new FuzzyRule(9, IfBottomNormalAndTopNormal, thenVerticalMotorMedium);
  fuzzy->addFuzzyRule(fuzzyRule9);

  // Building FuzzyRule Rule 10: if Bottom Normal And Top Normal then Horizontal Servo Motor Medium
  FuzzyRuleConsequent *thenHorizontalMotorMedium = new FuzzyRuleConsequent();
  thenHorizontalMotorMedium->addOutput(Hmedium);

  FuzzyRule *fuzzyRule10 = new FuzzyRule(10, IfBottomNormalAndTopNormal, thenHorizontalMotorMedium);
  fuzzy->addFuzzyRule(fuzzyRule10);

  //Normal Left and Right
  // Building FuzzyRule Rule 11: if Left Normal And Right Normal then Vertical Servo Motor Medium
  FuzzyRuleAntecedent *IfLeftNormalAndRightNormal = new FuzzyRuleAntecedent();
  IfLeftNormalAndRightNormal->joinWithAND(Lnormal, Rnormal);
  
  FuzzyRule *fuzzyRule11 = new FuzzyRule(11, IfLeftNormalAndRightNormal, thenVerticalMotorMedium);
  fuzzy->addFuzzyRule(fuzzyRule11);

  // Building FuzzyRule Rule 12: if Left Normal And Right Normal then Horizontal Servo Motor Medium
  FuzzyRule *fuzzyRule12 = new FuzzyRule(12, IfLeftNormalAndRightNormal, thenHorizontalMotorMedium);
  fuzzy->addFuzzyRule(fuzzyRule12);

  //Dim Top and Bottom
  // Building FuzzyRule Rule 13: if Bottom Dim And Top Dim then Vertical Servo Motor Medium
  FuzzyRuleAntecedent *IfBottomDimAndTopDim = new FuzzyRuleAntecedent();
  IfBottomDimAndTopDim->joinWithAND(Bdim, Tdim);
  
  FuzzyRule *fuzzyRule13 = new FuzzyRule(13, IfBottomDimAndTopDim, thenVerticalMotorMedium);
  fuzzy->addFuzzyRule(fuzzyRule13);

  // Building FuzzyRule Rule 14: if Bottom Dim And Top Dim then Horizontal Servo Motor Medium
  FuzzyRule *fuzzyRule14 = new FuzzyRule(14, IfBottomDimAndTopDim, thenHorizontalMotorMedium);
  fuzzy->addFuzzyRule(fuzzyRule14);

  //Dim Left and Right
  // Building FuzzyRule Rule 15: if Left Dim And Right Dim then Vertical Servo Motor Medium
  FuzzyRuleAntecedent *IfLeftDimAndRightDim = new FuzzyRuleAntecedent();
  IfLeftDimAndRightDim->joinWithAND(Ldim, Rdim);
  
  FuzzyRule *fuzzyRule15 = new FuzzyRule(15, IfLeftDimAndRightDim, thenVerticalMotorMedium);
  fuzzy->addFuzzyRule(fuzzyRule15);

  // Building FuzzyRule Rule 16: if Left Normal And Right Normal then Horizontal Servo Motor Medium
  FuzzyRule *fuzzyRule16 = new FuzzyRule(16, IfLeftDimAndRightDim, thenHorizontalMotorMedium);
  fuzzy->addFuzzyRule(fuzzyRule16);

}

void loop()
{ 
  //sensorRead(int analog)
  TopSensor = sensorRead(AnalogTop);
  BottomSensor = sensorRead(AnalogBottom);
  LeftSensor = sensorRead(AnalogLeft); 
  RightSensor = sensorRead(AnalogRight); 

  // Check and cap the sensor values at a maximum of 500
  if (TopSensor >= 500) {
      TopSensor = 500;
  }

  if (BottomSensor >= 500) {
      BottomSensor = 500;
  }

  if (LeftSensor >= 500) {
      LeftSensor = 500;
  }

  if (RightSensor >= 500) {
      RightSensor = 500;
  }

  Serial.println("\n\n\nSensor Values: ");
  Serial.print("\t\t\tTop Sensor: ");
  Serial.print(TopSensor);
  Serial.print(", Bottom Sensor: ");
  Serial.print(BottomSensor);
  Serial.print(", Left Sensor: ");
  Serial.println(LeftSensor);
  Serial.print(", Right Sensor: ");
  Serial.println(RightSensor);

  fuzzy->setInput(1, TopSensor);
  fuzzy->setInput(2, BottomSensor);
  fuzzy->setInput(3, LeftSensor);
  fuzzy->setInput(4, RightSensor);

  fuzzy->fuzzify();

  //Top Sensor Value
  Serial.println("Input: ");
  Serial.print("\tTop Sensor: Dim-> ");
  Serial.print(Tdim->getPertinence());
  Serial.print(", Normal-> ");
  Serial.print(Tnormal->getPertinence());
  Serial.print(", Bright-> ");
  Serial.println(Tbright->getPertinence());

  //Bottom Sensor Value
  Serial.println("Input: ");
  Serial.print("\tBottom Sensor: Dim-> ");
  Serial.print(Bdim->getPertinence());
  Serial.print(", Normal-> ");
  Serial.print(Bnormal->getPertinence());
  Serial.print(", Bright-> ");
  Serial.println(Bbright->getPertinence());

  //Left Sensor Value
  Serial.println("Input: ");
  Serial.print("\tLeft Sensor: Dim-> ");
  Serial.print(Ldim->getPertinence());
  Serial.print(", Normal-> ");
  Serial.print(Lnormal->getPertinence());
  Serial.print(", Bright-> ");
  Serial.println(Lbright->getPertinence());

  //Right Sensor Value
  Serial.println("Input: ");
  Serial.print("\tRight Sensor: Dim-> ");
  Serial.print(Rdim->getPertinence());
  Serial.print(", Normal-> ");
  Serial.print(Rnormal->getPertinence());
  Serial.print(", Bright-> ");
  Serial.println(Rbright->getPertinence());

  float VerticalAngle = fuzzy->defuzzify(1);
  float HorizontalAngle = fuzzy->defuzzify(2);

  //Set Limits
  if (VerticalAngle < 60){
    VerticalAngle = 60;
  }
  else if (VerticalAngle > 120){
    VerticalAngle = 120;
  }

  //Vertical Angle Details
  Serial.println("Output: ");
  Serial.print("\tVertical Angle: Low-> ");
  Serial.print(Vlow->getPertinence());
  Serial.print(", Medium-> ");
  Serial.print(Vmedium->getPertinence());
  Serial.print(", High-> ");
  Serial.println(Vhigh->getPertinence());

  //Horizontal Angle Details
  Serial.println("Output: ");
  Serial.print("\tHorizontal Angle: Low-> ");
  Serial.print(Hlow->getPertinence());
  Serial.print(", Medium-> ");
  Serial.print(Hmedium->getPertinence());
  Serial.print(", High-> ");
  Serial.println(Hhigh->getPertinence());


  //Result Details
  Serial.println("Result: ");
  Serial.print("\t\t\tVertical Angle: ");
  Serial.print(VerticalAngle);
  Serial.print(", and Horizontal Angle: ");
  Serial.println(HorizontalAngle);

  servov = servoverti.read();
  servoh = servohori.read();

  //Add Transition to Current Motor Angle to Sun Current Angle
  if (servov != VerticalAngle) {
    if (servov < VerticalAngle) {
      for (int i = servov; i <= VerticalAngle; i=i+sensitivity) {
        servoverti.write(i);
        delay(15); // Adjust delay as needed for smooth motion
      }
    } else {
      for (int i = servov; i >= VerticalAngle; i=i-sensitivity) {
        servoverti.write(i);
        delay(15); // Adjust delay as needed for smooth motion
      }
    }
  }

  if (servoh != HorizontalAngle) {
    if (servoh < HorizontalAngle) {
      for (int i = servoh; i <= HorizontalAngle; i=i+sensitivity) {
        servohori.write(i);
        delay(15); // Adjust delay as needed for smooth motion
      }
    } else {
      for (int i = servoh; i >= HorizontalAngle; i=i-sensitivity) {
        servohori.write(i);
        delay(15); // Adjust delay as needed for smooth motion
      }
    }
  }

  // wait 1 seconds
  delay(1000);
}
