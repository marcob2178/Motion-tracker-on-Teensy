#ifndef SETTINGS_H
#define SETTINGS_H

//========================================================================
//  Pinout
//========================================================================

#define LEFT_BUTTON_PIN 4
#define RIGHT_BUTTON_PIN 7
#define RIGHT_ACCEL_TWI_ADRESS 0x28
#define LEFT_ACCEL_TWI_ADRESS 0x29
#define CHEST_ACCEL_INT_PIN 8
#define POT_0_CS 6
#define POT_1_CS 5
#define Sensor_BR A1
#define Sensor_SR A7
#define Sensor_BL A0
#define Sensor_SL A6

//========================================================================
//  JOYSTICK
//========================================================================

//calibration values
#define HORIZONT_MIN 36
#define HORIZONT_MAX 243
#define HORIZONT_MIDDLE 127
#define VERTICAL_MIN 10
#define VERTICAL_MAX 243
#define VERTICAL_MIDDLE 127

//========================================================================
//  THRESHOLDS for moving recognition 
//========================================================================

#define CALCULATING_PERIOD 10
#define FEET_ANGLE 10

//no negative values, use absolute
#define CHEST_FORWARD_MIN 5
#define CHEST_FORWARD_MAX 25

#define CHEST_BACKWARD_MIN 5
#define CHEST_BACKWARD_MAX 25

#define CHEST_LEFT_MIN 5
#define CHEST_LEFT_MAX 25

#define CHEST_RIGHT_MIN 5
#define CHEST_RIGHT_MAX 25

#endif




//OLD CODES




//=======================================================================================================================
// #define TYPE_NOT_WALKING 0
// #define TYPE_WALKING 1
// #define TYPE_RUNNING 2

// int walkType = TYPE_NOT_WALKING;
// int prevRightTime = 0;
// int prevLeftTime = 0;
// bool lastLeft = false;
// bool lastRight = false;
// int steps = 0;
// int step_timer = 0;
// bool isWalk = false;
// bool stepChanged = false;
// int lastStepAccel = 0;
// int prevAccel = 0;

// long timeBetweenSteps = 0;
// long stepsTimer = 0;

// void translateWalkingWithTimings()
// {

//   bool curLeft = leftFoot->isWalking();
//   bool curRight = rightFoot->isWalking();

//   if ((!curLeft && lastLeft) || (!curRight && lastRight))
//   {
//     stepChanged = true;
//     lastStepAccel = prevAccel;
//     prevAccel = 0;

//     // if (lastRight)
//     //   timeBetweenSteps = prevRightTime;
//     // else if (lastLeft)
//     //   timeBetweenSteps = prevLeftTime;

//     if (isWalk)
//     {

//       timeBetweenSteps = millis() - stepsTimer;
//       if (timeBetweenSteps > 250)
//         walkType = TYPE_WALKING;
//       else if (timeBetweenSteps <= 250)
//         walkType = TYPE_RUNNING;
//     }

//     stepsTimer = millis();
//   }

//   if (curRight || curLeft)
//   {
//     //=============================
//     //count steps
//     stepChanged = false;
//     if ((curLeft && !lastLeft) || (curRight && !lastRight))
//       steps++;

//     prevRightTime = rightFoot->getStepTime();
//     prevLeftTime = leftFoot->getStepTime();

//     step_timer = 0;
//   }

//   isWalk = steps > 1;

//   //=============================
//   if ((steps > 0) && stepChanged)
//   {
//     left_y = lastStepAccel;
//     ychanged = true;
//     step_timer += CALCULATING_PERIOD;
//     if (step_timer > 1000)
//     {
//       step_timer = 0;
//       steps = 0;
//       stepChanged = false;
//       timeBetweenSteps = 0;
//       walkType = TYPE_NOT_WALKING;
//     }
//   }

//   lastLeft = curLeft;
//   lastRight = curRight;

//   //=============================

//   // 1) is activatedby walking only
//   // 2) is activated by walking and chest bent. and the more I bent the chest, the more or less the + moves.
//   // 3) is activated by running nad maybe a fixed bigget angle of bent of the chest?

//   //=============================

//   if (isWalk)
//   {
//     if (timeBetweenSteps > 0 && timeBetweenSteps <= 400)
//       left_y = 110;
//     else if (timeBetweenSteps > 400 && timeBetweenSteps <= 600)
//       left_y = 90;
//     else if (timeBetweenSteps > 600 && timeBetweenSteps <= 800)
//       left_y = 60;
//     else if (timeBetweenSteps > 800 && timeBetweenSteps < 1000)
//       left_y = 30;

//     ychanged = true;

//     // switch (walkType)
//     // {
//     // case TYPE_WALKING:
//     //   if (chestAccel->getRoll() < -8)
//     //   {
//     //     left_y = ((chestAccel->getRoll()) * -3);
//     //     ychanged = true;
//     //   }
//     //   else
//     //   {
//     //     left_y = 30;
//     //     ychanged = true;
//     //   }
//     //   break;

//     // case TYPE_RUNNING:

//     //   left_y = 110;
//     //   ychanged = true;
//     //   break;
//     // }
//   }

//   // Serial.print(rightFoot->getStepTime() + String("\t"));
//   // Serial.print(rightFoot->getRawPower() + String("\t"));
//   // Serial.print(leftFoot->getStepTime() + String("\t"));
//   // Serial.print(leftFoot->getRawPower() + String("\t"));
//   // Serial.print(chestAccel->getRoll() + String("\t"));

//   // Serial.print(isWalk + String("\t"));
//   // Serial.print(walkType + String("\t"));
//   // Serial.print(left_y + String("\t"));
//   // Serial.println(timeBetweenSteps);

//   // Serial.print(steps + String("\t"));
//   // Serial.print(stepChanged + String("\t"));
//   // Serial.print(step_timer + String("\t"));
//   // Serial.print(prevAccel + String("\t"));
//   // Serial.println(lastStepAccel + String("\t"));
// }

//=======================================================================================================================
// #define TYPE_NOT_WALKING 0
// #define TYPE_WALKING 1
// #define TYPE_RUNNING 2

// int walkType = TYPE_NOT_WALKING;
// double prevRightPower = 0;
// double prevLeftPower = 0;
// bool lastLeft = false;
// bool lastRight = false;
// int steps = 0;
// int step_timer = 0;
// bool isWalk = false;
// bool stepChanged = false;
// int lastStepAccel = 0;

// void translateWalkingWithAcceleration()
// {

//   bool curLeft = leftFoot->isWalking();
//   bool curRight = rightFoot->isWalking();

//   if ((!curLeft && lastLeft) || (!curRight && lastRight))
//   {
//     if (lastRight)
//       lastStepAccel = prevRightPower;
//     else if (lastLeft)
//       lastStepAccel = prevLeftPower;

//     prevRightPower = 0;
//     prevLeftPower = 0;

//     // lastStepAccel = left_y;
//     //ychanged = true;

//     stepChanged = true;
//   }

//   if (curRight || curLeft)
//   {
//     //=============================
//     //count steps
//     stepChanged = false;
//     if ((curLeft && !lastLeft) || (curRight && !lastRight))
//       steps++;

//     prevRightPower = rightFoot->getDistance() * 100 > prevRightPower ? rightFoot->getDistance() * 100 : prevRightPower;
//     prevLeftPower = leftFoot->getDistance() * 100 > prevLeftPower ? leftFoot->getDistance() * 100 : prevLeftPower;

//     step_timer = 0;
//   }

//   isWalk = steps > 1;

//   //=============================
//   if ((steps > 0))
//   {
//     left_y = lastStepAccel;
//     ychanged = true;
//     step_timer += CALCULATING_PERIOD;
//     if (step_timer > 1000)
//     {
//       step_timer = 0;
//       steps = 0;
//       stepChanged = false;
//       walkType = TYPE_NOT_WALKING;
//     }
//   }

//   lastLeft = curLeft;
//   lastRight = curRight;

//   //=============================

//   // Serial.print(rightFoot->getStepTime() + String("\t"));
//   // Serial.print(rightFoot->getRawPower() + String("\t"));
//   // Serial.print(prevRightPower + String("\t"));
//   // Serial.print(leftFoot->getStepTime() + String("\t"));
//   // Serial.print(leftFoot->getRawPower() + String("\t"));
//   // Serial.print(prevLeftPower + String("\t"));

//   // Serial.print(isWalk + String("\t"));

//   // Serial.print(isWalk + String("\t"));
//   // Serial.print(walkType + String("\t"));
//   // Serial.print(left_y + String("\t"));
//   // Serial.print(timeBetweenSteps);

//   // Serial.print(steps + String("\t"));
//   // Serial.print(stepChanged + String("\t"));
//   // Serial.print(step_timer + String("\t"));
//   // Serial.print(prevAccel + String("\t"));
//   // Serial.print(lastStepAccel + String("\t"));
//   //
//   // Serial.println();
// }
//=======================================================================================================================
