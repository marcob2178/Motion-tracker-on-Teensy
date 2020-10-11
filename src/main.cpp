#include <Arduino.h>
#include <Settings.h>
#include <Joystick.h>
#include <accels/Accelerometer.h>
#include <accels/AccelBno055.h>
#include <accels/AccelBno080.h>
#include <WeightSensor.h>
#include <Chest.h>
#include <Foot.h>

/*  
TODO: 


2) add setting for invert axes on all sensors

3) add in setting ps4/vr controller pressing button swith setup

4) i made a temporary uno with 2 accell and a shield to test button press for curiosity

What is on the milestone list is this:  

- vr joystick press /drift 
- clean code 
- optimize walk/run algorithm / continue fine tuning code for locomotion features
- create gui
- crouch detection (System should be able to recognise when the human is going down to crouch - verical movement) / maybe use external cameras
- vr driver injection
- wireless system

STACK:
- usb injection (https://github.com/TrueOpenVR/TrueOpenVR-Drivers)  
- GUI for smoother configuration of the system

- the sensitivity of the chest when walking/running with feet, can be changed and made less sensitive compared to how the chest works in normal condition when using only chest for moving
*/

#define NO_OUTPUT 0
#define RIGHT_SHOE_OUTPUT 1
#define LEFT_SHOE_OUTPUT 2
#define CHEST_OUTPUT 3
#define MOVEMENT_CHEST_OUTPUT 4
#define MOVEMENT_RIGHT_OUTPUT 5
#define MOVEMENT_LEFT_OUTPUT 6
#define MOVEMENT_TRANSLATING_OUTPUT 7
#define TIMING_OUTPUT 8
int currentOutput = 0;

void printAcceleration();
void printRawValues();
void printAccelerationOffset();

void printRawChest();
void printRawRightShoe();
void printRawLeftShoe();
void updateRawData();
void translateTheMovement();
void parseSerial();
void printTheMovement();
void updateJoysticks();

Joystick leftJoystick;  //run and move direction control
Joystick rightJoystick; //jump and crouch
AccelBNO055 *chestAccel;
AccelBNO055 *rightShoeAccel;
AccelBNO055 *leftShoeAccel;
WeightSensor *rightSideFoot;
WeightSensor *rightBackFoot;
WeightSensor *leftSideFoot;
WeightSensor *leftBackFoot;
Chest *chest;
Foot *rightFoot;
Foot *leftFoot;

void setup()
{
  Serial.begin(CUSTON_UART_SPEED);
  while (!Serial)
    ;
  Serial.println("Started!");

  Wire.begin();
  Wire.setClock(CUSTOM_WIRE_SPEED);

  Wire1.begin();
  Wire1.setClock(CUSTOM_WIRE1_SPEED);

  delay(500);

  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(CUSTOM_SPI_CLOCK_DIVIDER);
  SPI.begin();

  delay(500);

  Serial.println("Create instances!");
  chestAccel = new AccelBNO055(1, CHEST_ACCEL_TWI_ADRESS, &Wire1);
  rightShoeAccel = new AccelBNO055(2, LEFT_ACCEL_TWI_ADRESS, &Wire);
  leftShoeAccel = new AccelBNO055(3, RIGHT_ACCEL_TWI_ADRESS, &Wire);

  rightSideFoot = new WeightSensor(Sensor_SR);
  rightBackFoot = new WeightSensor(Sensor_BR);
  leftSideFoot = new WeightSensor(Sensor_SL);
  leftBackFoot = new WeightSensor(Sensor_BL);

  chest = new Chest(chestAccel);
  rightFoot = new Foot(rightShoeAccel, rightSideFoot, rightBackFoot);
  leftFoot = new Foot(leftShoeAccel, leftSideFoot, leftBackFoot);

  delay(200);

  Serial.println("Begin all");

  leftJoystick.begin(POT_0_CS, LEFT_BUTTON_PIN);
  rightJoystick.begin(POT_1_CS, RIGHT_BUTTON_PIN);
  leftJoystick.setCalibrationData(LEFT_HORIZONT_MIN, LEFT_HORIZONT_MAX, LEFT_HORIZONT_MIDDLE,
                                  LEFT_VERTICAL_MIN, LEFT_VERTICAL_MAX, LEFT_VERTICAL_MIDDLE);
  rightJoystick.setCalibrationData(RIGHT_HORIZONT_MIN, RIGHT_HORIZONT_MAX, RIGHT_HORIZONT_MIDDLE,
                                   RIGHT_VERTICAL_MIN, RIGHT_VERTICAL_MAX, RIGHT_VERTICAL_MIDDLE);

  rightShoeAccel->begin();
  delay(200);
  leftShoeAccel->begin();
  delay(200);
  chestAccel->begin();
  delay(200);

  Serial.println("Calibrating");
  unsigned long timer = millis();
  while (millis() - timer < CALIBRATING_DURATION)
  {
    leftShoeAccel->calibrate();
    rightShoeAccel->calibrate();
    chestAccel->calibrate();
    Serial.print(".");
    delay(CALIBRATING_PERIOD);
  }
  Serial.println("\nDone!");
  Serial.println("Program started!");
}

long long timer = 0;
void loop()
{
  if (millis() > timer)
  {
    timer = millis() + CALCULATING_PERIOD;
    updateRawData();

    if (currentOutput == TIMING_OUTPUT)
    {
      long time = CALCULATING_PERIOD - (timer - millis());
      Serial.println(String(time));
    }

    parseSerial();
    printTheMovement();
    translateTheMovement();
  }
}

//   printAccelerationOffset();
//   calculate();

//   //Serial.println("\tLoop time = " + String(int(timer - millis())));
// }

//=====================================================================
//  Serial Controller
//=====================================================================

void parseSerial()
{
  if (Serial.available() > 0)
  {
    char mess = Serial.read();
    if (mess == 'r')
      currentOutput = RIGHT_SHOE_OUTPUT;
    if (mess == 'l')
      currentOutput = LEFT_SHOE_OUTPUT;
    if (mess == 'c')
      currentOutput = CHEST_OUTPUT;
    if (mess == 'm')
      currentOutput = MOVEMENT_CHEST_OUTPUT;
    if (mess == '.')
      currentOutput = MOVEMENT_RIGHT_OUTPUT;
    if (mess == ',')
      currentOutput = MOVEMENT_LEFT_OUTPUT;
    if (mess == 'n')
      currentOutput = NO_OUTPUT;
    if (mess == 't')
      currentOutput = MOVEMENT_TRANSLATING_OUTPUT;
    if (mess == 'u')
      currentOutput = TIMING_OUTPUT;

    // for testing purpoe, should be ignored
    if (mess == '1')
      leftJoystick.pressButton();
    if (mess == '2')
      leftJoystick.releaseButton();
    if (mess == '3')
      rightJoystick.pressButton();
    if (mess == '4')
      rightJoystick.releaseButton();
  }
}

//=====================================================================
//  RAW data updating
//=====================================================================

void updateRawData()
{
  chestAccel->update();
  rightShoeAccel->update();
  leftShoeAccel->update();
  rightSideFoot->update();
  rightBackFoot->update();
  leftSideFoot->update();
  leftBackFoot->update();

  switch (currentOutput)
  {
  case RIGHT_SHOE_OUTPUT:
    printRawRightShoe();
    break;
  case LEFT_SHOE_OUTPUT:
    printRawLeftShoe();
    break;
  case CHEST_OUTPUT:
    printRawChest();
    break;
  }
}

//=====================================================================
//  Movement Translating
//=====================================================================

int left_x, left_y;
int left_button_state;
int right_x, right_y;
int right_button_state;
int delay_value = 0;

bool xchanged = false;
bool ychanged = false;

//=====================================================================
//  Walking

//calculated parameters - don't edit
float avg_a1 = 0;
float avg_a2 = 0;
float loc_max_a1 = 0;
float loc_max_a2 = 0;

float hyst_size = 20 * 0.01 * HYSTERESYS_COEFFICIENT;
float accum_err = 0;

float walk_speed_v = 0;
float walk_speed_max = 0;
float walk_speed = 0;

void translateWalking()
{
  float ax1 = rightShoeAccel->getLinX();
  float ay1 = rightShoeAccel->getLinY();
  float az1 = rightShoeAccel->getLinZ();
  float ax2 = leftShoeAccel->getLinX();
  float ay2 = leftShoeAccel->getLinY();
  float az2 = leftShoeAccel->getLinZ();
  float a1 = sqrt(ax1 * ax1 + ay1 * ay1 + az1 * az1);
  float a2 = sqrt(ax2 * ax2 + ay2 * ay2 + az2 * az2);

  float avg_param = 0.7;
  float loc_max_param = 0.99;
  avg_a1 = avg_param * avg_a1 + (1.0 - avg_param) * a1;
  avg_a2 = avg_param * avg_a2 + (1.0 - avg_param) * a2;
  loc_max_a1 *= loc_max_param;
  loc_max_a2 *= loc_max_param;
  if (avg_a1 > loc_max_a1)
    loc_max_a1 = avg_a1;
  if (avg_a2 > loc_max_a2)
    loc_max_a2 = avg_a2;

  if (loc_max_a1 < 2.5)
    loc_max_a1 = 2.5;
  if (loc_max_a2 < 2.5)
    loc_max_a2 = 2.5;

  if (avg_a1 < loc_max_a1 * 0.05 && avg_a2 < loc_max_a2 * 0.05)
    walk_speed_v *= 0.9;

  if (avg_a1 < loc_max_a1 * 0.3 && avg_a2 < loc_max_a2 * 0.3)
    walk_speed_v *= 0.99;

  float v_high = 0, v_low = 0;
  if (avg_a1 > loc_max_a1 * 0.3 && avg_a2 < loc_max_a2 * 0.3)
  {
    v_high = avg_a1;
    v_low = avg_a2;
  }
  if (avg_a1 < loc_max_a1 * 0.3 && avg_a2 > loc_max_a2 * 0.3)
  {
    v_high = avg_a2;
    v_low = avg_a1;
  }

  if (v_high < 0.5 && v_low < 0.5)
    walk_speed_v *= 0.99; //it's ok for a few frames, but not for too long
  else
  {
    walk_speed_v = walk_speed_v * 0.97 + 0.03 * (v_high * 10.0 * WALK_SENSITIVITY * 0.03);
  }

  if (walk_speed_v > walk_speed_max)
    walk_speed_max = walk_speed_v;

  walk_speed_max *= (0.995 - RESPONSIVENESS_COEFF * 0.0005);

  accum_err += 0.01 * (walk_speed_max - walk_speed);

  if (walk_speed_max > walk_speed + hyst_size || accum_err > 3 * hyst_size)
  {
    walk_speed = walk_speed_max;
    accum_err = 0;
  }

  if (walk_speed_max < walk_speed - hyst_size || accum_err < -3 * hyst_size || walk_speed_max < hyst_size / 2)
  {
    walk_speed = walk_speed_max;
    accum_err = 0;
  }

  if (walk_speed > 120)
    walk_speed = 120;
  if (walk_speed < 10)
    walk_speed = 0;
  if (walk_speed > 0 && walk_speed < 2 * DEAD_ZONE)
    walk_speed = DEAD_ZONE + walk_speed * 0.5;

  if (USE_DIGIPOT_REMAP && walk_speed < 100)
  {
    float j_res = 4.3;
    float d_res = DIGIPOT_RESISTANCE_KOHM / 2;
    float tgt_res = 2.5 - walk_speed * 0.025;
    walk_speed = (d_res - tgt_res) / d_res * 100;
  }

  if (walk_speed != 0)
  {
    left_y = walk_speed;
    ychanged = true;
  }
}

void translateBending()
{
  // if (chest->isBending())
  // {
  if ((chestAccel->getRoll() < -CHEST_BACKWARD_MIN) || (chestAccel->getRoll() > CHEST_FORWARD_MIN))
  {
    if (chestAccel->getRoll() < 0)
      left_y = map(-chestAccel->getRoll(), 0, ychanged ? RUNNING_BENDING_COEFF * CHEST_BACKWARD_MAX : CHEST_BACKWARD_MAX, 0, -100);
    else
      left_y = map(-chestAccel->getRoll(), 0, ychanged ? RUNNING_BENDING_COEFF * -CHEST_FORWARD_MAX : -CHEST_FORWARD_MAX, 0, 100);
    ychanged = true;
  }

  //calculating of right-left/horizontal movement
  if ((chestAccel->getPitch() < -CHEST_RIGHT_MIN) || (chestAccel->getPitch() > CHEST_LEFT_MIN))
  {
    if (chestAccel->getPitch() < 0)
      left_x = map(-chestAccel->getPitch(), 0, ychanged ? RUNNING_BENDING_COEFF * CHEST_RIGHT_MAX : CHEST_RIGHT_MAX, 0, 100);
    else
      left_x = map(-chestAccel->getPitch(), 0, ychanged ? RUNNING_BENDING_COEFF * -CHEST_LEFT_MAX : -CHEST_LEFT_MAX, 0, -100);
    xchanged = true;
  }
  //gotcha
  // }
}

// void translateBending()
// {
//   if (chest->isBending())
//   {
//     //if(chestAccel->getRoll() <CHEST_BACKWARD_MIN)
//     if (chestAccel->getRoll() < 0)
//       left_y = map(-chestAccel->getRoll(), 0, CHEST_BACKWARD_MAX, 0, -100);
//     else
//       left_y = map(-chestAccel->getRoll(), 0, -CHEST_FORWARD_MAX, 0, 100);

//     //calculating of right-left/horizontal movement
//     if (chestAccel->getPitch() < 0)
//       left_x = map(-chestAccel->getPitch(), 0, CHEST_RIGHT_MAX, 0, 100);
//     else
//       left_x = map(-chestAccel->getPitch(), 0, -CHEST_LEFT_MAX, 0, -100);
//     xchanged = true;
//     ychanged = true;
//   }
// }

void translateCruiseControl()
{
  rightFoot->isWalking();
  leftFoot->isWalking();

  if (rightFoot->isCruiseControl())
  {
    left_y = map(rightFoot->getCruiseControlPower(),
                 CRUISE_MIN_ANGLE,
                 CRUISE_MAX_ANGLE,
                 CRUISE_MIN_JOYSTICK_VALUE,
                 CRUISE_MAX_JOYSTICK_VALUE);
    ychanged = true;
  }

  if (leftFoot->isCruiseControl())
  {
    left_y = map(leftFoot->getCruiseControlPower(),
                 CRUISE_MIN_ANGLE,
                 CRUISE_MAX_ANGLE,
                 CRUISE_MIN_JOYSTICK_VALUE,
                 CRUISE_MAX_JOYSTICK_VALUE);
    ychanged = true;
  }
}

void translateSideMoving()
{
  if (rightFoot->isSideStep())
  {
    left_x = map(rightFoot->getSidePower(), RIGHT_SIDE_MIN_POWER, RIGHT_SIDE_MAX_POWER, 0, 100);
    xchanged = true;
  }
  else if (leftFoot->isSideStep())
  {
    left_x = -map(leftFoot->getSidePower(), LEFT_SIDE_MIN_POWER, LEFT_SIDE_MAX_POWER, 0, 100);
    xchanged = true;
  }
}

void translateBackMoving()
{
  if (rightFoot->isStepBack())
  {
    left_y = -map(rightFoot->getStepBackPower(), RIGHT_BACK_MIN_POWER, RIGHT_BACK_MAX_POWER, 0, 100);
    ychanged = true;
  }

  else if (leftFoot->isStepBack())
  {
    left_y = -map(leftFoot->getStepBackPower(), LEFT_BACK_MIN_POWER, LEFT_BACK_MAX_POWER, 0, 100);
    ychanged = true;
  }
}

void translateTheMovement()
{
  xchanged = false;
  ychanged = false;
  //bending control
  translateBending();
  //walking
  translateWalking();

  //cruise control
  translateCruiseControl();
  //side moving
  translateSideMoving();
  //back moving
  translateBackMoving();

  //default for moving left joystick

  if (!xchanged)
    left_x = 0;
  if (!ychanged)
    left_y = 0;

  right_x = 0;
  right_y = 0;

  //if running very fast - press the button
  if ((abs(left_x) >= MINIMUM_VALUE_FOR_RUNNING) || (abs(left_y) >= MINIMUM_VALUE_FOR_RUNNING))
    left_button_state = 1;
  else
    left_button_state = 0;

  //jump
  if (chest->isJumping())
    right_button_state = 1;
  else
    right_button_state = 0;

  updateJoysticks();

  if (currentOutput == MOVEMENT_TRANSLATING_OUTPUT)
  {
    Serial.print("Left joystick:\t" + String(left_x) + "\t" + String(left_y) + "\t" + "button\t" + String(left_button_state) + "\t");
    Serial.println("Right joystick:\t" + String(right_x) + "\t" + String(right_y) + "\t" + "button\t" + String(right_button_state) + "\t");
  }
}

void updateJoysticks()
{
  if (left_x > 100)
    left_x = 100;
  if (left_x < -100)
    left_x = -100;

  if (left_y > 100)
    left_y = 100;
  if (left_y < -100)
    left_y = -100;

  if (right_x > 100)
    right_x = 100;
  if (right_x < -100)
    right_x = -100;

  if (right_y > 100)
    right_y = 100;
  if (right_y < -100)
    right_y = -100;

  leftJoystick.setHor(-left_x * AXE_X_INVERTED);
  leftJoystick.setVer(-left_y * AXE_Y_INVERTED);

  if (left_button_state == 1)
    leftJoystick.pressButton();
  else
    leftJoystick.releaseButton();

  rightJoystick.setHor(right_x);
  rightJoystick.setVer(right_y);

  if (right_button_state == 1)
    rightJoystick.pressButton();
  else
    rightJoystick.releaseButton();
}

//=====================================================================
//  Prints
//=====================================================================

void printTheMovement()
{
  if (currentOutput == MOVEMENT_CHEST_OUTPUT)
  {
    //jump output
    Serial.print("\tJump:" + String(chest->isJumping() ? "\tDetected" : "\tNothing") + String("\t") + String(chest->isJumping() ? String(chest->getJumpingPower()) : "\t") + String("\t"));

    //bend output
    Serial.print("Chest\tBend:");

    if (!chest->isBending())
      Serial.print("\tStraight\t");
    else
    {
      if ((chest->getBendingDirection() > 315) || (chest->getBendingDirection() <= 45))
        Serial.print("\tbackward");

      else if ((chest->getBendingDirection() > 225) && (chest->getBendingDirection() <= 315))
        Serial.print("\tleft");

      else if ((chest->getBendingDirection() > 135) && (chest->getBendingDirection() <= 225))
        Serial.print("\tforward");

      else if ((chest->getBendingDirection() > 45) && (chest->getBendingDirection() <= 135))
        Serial.print("\tright");

      Serial.print(String("\t") + chest->getBendingDirection());
      Serial.print(String("\t") + chest->getBendingPower());
    }

    //crouch output F

    // Serial.print("\tCrouch:");
    // if (isCrouch())
    // {
    //   Serial.print("\tDetected" + String("\t") + String(getCrouchPower()));
    // }
    // else
    //   Serial.print("\tNothing");

    Serial.println();
  }

  // right

  if (currentOutput == MOVEMENT_RIGHT_OUTPUT)
  {
    //running output right

    Serial.print("Right foot\tWalk:");
    if (rightFoot->isWalking())
      Serial.print(String("\t") + String(rightFoot->getWalkingPower()));
    else
      Serial.print("\tNothing\t");

    //special things

    Serial.print("\tCruise");
    if (rightFoot->isCruiseControl())
    {
      Serial.print(String("\t") + String(rightFoot->getCruiseControlPower()));
    }
    else
      Serial.print("\tNothing\t");

    //step back

    Serial.print("\tStepback");
    if (rightFoot->isStepBack())
    {
      Serial.print(String("\t") + String(rightFoot->getStepBackPower()));
    }
    else
      Serial.print("\tNothing\t");

    //step side

    Serial.print("\tStepside");
    if (rightFoot->isSideStep())
    {
      Serial.print(String("\t") + String(rightFoot->getSidePower()));
    }
    else
      Serial.print("\tNothing\t");
    Serial.println();
  }

  // left

  if (currentOutput == MOVEMENT_LEFT_OUTPUT)
  {
    //running output right

    Serial.print("Left foot\tWalk:");
    if (leftFoot->isWalking())
      Serial.print(String("\t") + String(leftFoot->getWalkingPower()));

    else
      Serial.print("\tNothing\t");

    //special things

    Serial.print("\tCruise");
    if (leftFoot->isCruiseControl())
      Serial.print(String("\t") + String(leftFoot->getCruiseControlPower()));
    else
      Serial.print("\tNothing\t");

    //step back

    Serial.print("\tStepback");
    if (leftFoot->isStepBack())
    {
      Serial.print(String("\t") + String(leftFoot->getStepBackPower()));
    }
    else
      Serial.print("\tNothing\t");

    //step side

    Serial.print("\tStepside");
    if (leftFoot->isSideStep())
    {
      Serial.print(String("\t") + String(leftFoot->getSidePower()));
    }
    else
      Serial.print("\tNothing\t");
    Serial.println();
  }
}

void printRawChest()
{

  Serial.print("chest:");
  Serial.print("\tp\t" + String(chestAccel->getPitch()));
  Serial.print("\tr\t" + String(chestAccel->getRoll()));
  Serial.println("\taZ\t" + String(chestAccel->getLinZ()));
  //Serial.println("\talt\t" + String(chestAccel->getAltitude()));
}

void printRawRightShoe()
{
  Serial.print("rshoe:");
  Serial.print("\tax\t" + String(rightShoeAccel->getLinX()));
  Serial.print("\tay\t" + String(rightShoeAccel->getLinY()));
  Serial.print("\taz\t" + String(rightShoeAccel->getLinZ()));

  Serial.print("\ty\t" + String(rightShoeAccel->getYaw()));
  Serial.print("\tr\t" + String(rightShoeAccel->getRoll()));
  Serial.print("\tp\t" + String(rightShoeAccel->getPitch()));

  Serial.print("\tweight:");
  Serial.print("\ts\t" + String(rightSideFoot->readRaw()));
  Serial.println("\tb\t" + String(rightBackFoot->readRaw()));
}

void printRawLeftShoe()
{
  Serial.print("lshoe:");
  Serial.print("\tax\t" + String(leftShoeAccel->getLinX()));
  Serial.print("\tay\t" + String(leftShoeAccel->getLinY()));
  Serial.print("\taz\t" + String(leftShoeAccel->getLinZ()));

  Serial.print("\ty\t" + String(leftShoeAccel->getYaw()));
  Serial.print("\tr\t" + String(leftShoeAccel->getRoll()));
  Serial.print("\tp\t" + String(leftShoeAccel->getPitch()));

  Serial.print("\tweight:");
  Serial.print("\ts\t" + String(leftSideFoot->readRaw()));
  Serial.println("\tb\t" + String(leftBackFoot->readRaw()));
}

void printRawValues()
{
  Serial.println("(\\/)*===*(\\/)");
  printRawChest();
  printRawRightShoe();
  printRawLeftShoe();
}
