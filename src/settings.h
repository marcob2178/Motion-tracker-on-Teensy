#ifndef SETTINGS_H
#define SETTINGS_H

/*
All this section is related to algorithm and code tuning, 
Please follow all instructions written near to the some setting or option.
*/

//========================================================================
//  JOYSTICK
//========================================================================
#define TYPE_PS4 1
#define TYPE_STEAM_KNUCKLES 2

#define TYPE_OF_CONTROOLER TYPE_STEAM_KNUCKLES
//calibration values for detecting center and limit values of the joystick's cross
#define RIGHT_HORIZONT_MIN 36
#define RIGHT_HORIZONT_MAX 243
#define RIGHT_HORIZONT_MIDDLE 127
#define RIGHT_VERTICAL_MIN 10
#define RIGHT_VERTICAL_MAX 243
#define RIGHT_VERTICAL_MIDDLE 127

#define LEFT_HORIZONT_MIN 3
#define LEFT_HORIZONT_MAX 252
#define LEFT_HORIZONT_MIDDLE 127
#define LEFT_VERTICAL_MIN 3
#define LEFT_VERTICAL_MAX 252
#define LEFT_VERTICAL_MIDDLE 127

//========================================================================
//  ALGORITHM SETTINGS
//========================================================================

//====================================
//  axes setting
#define INVERTED -1
#define NORMAL 1

//  global, takes an effect to all axes settings 
#define ALL_AXE_Y_INVERTED          NORMAL
#define ALL_AXE_X_INVERTED          NORMAL

// separate
#define BENDING_X_INVERTED          INVERTED
#define BENDING_Y_INVERTED          NORMAL

#define WALKING_Y_INVERTED          INVERTED

#define CRUISE_Y_INVERTED           INVERTED

#define WEIGHT_SIDE_X_INVERTED      INVERTED

#define WEIGHT_BACK_Y_INVERTED      NORMAL

//====================================
//  main settings
#define CALCULATING_PERIOD 10 //  DANGEROUS TO CHANGE!!! that's period of the all measuring
#define FEET_ANGLE 10         //  minimum angle of step detecting

//====================================
//  calibrating time
#define CALIBRATING_DURATION 1500
#define CALIBRATING_PERIOD 50

//====================================
//  jump
#define MINIMUM_JUMP_POWER 10

//====================================
//  running setting
#define MINIMUM_VALUE_FOR_RUNNING 110

//====================================
//  chest bending algorithm, no negative values, use absolute. Takes an effect to USUAL BENDING algorithm!!

#define CHEST_MINIMUM_BENDING 5

#define CHEST_FORWARD_MIN 5  // degrees
#define CHEST_FORWARD_MAX 25 // degrees

#define CHEST_BACKWARD_MIN 5  // degrees
#define CHEST_BACKWARD_MAX 15 // degrees

#define CHEST_LEFT_MIN 5  // degrees
#define CHEST_LEFT_MAX 25 // degrees

#define CHEST_RIGHT_MIN 5  // degrees
#define CHEST_RIGHT_MAX 25 // degrees

//  integration with running
#define ENABLE_RUNNING_BENDING_ALG 0 // 1 for enable, 0 for

#if ENABLE_RUNNING_BENDING_ALG == 1
#define RUNNING_BENDING_COEFF 0.5f // more value - more sensitivity while running
#else
#define RUNNING_BENDING_COEFF 1
#endif
//====================================
//  walking algorithm tunning
//  tunable parameters - all defined for range 0-100
#define WALK_SENSITIVITY 25       //  physical movement intensity to game walking speed \
                                            //  higher value - less motion required to move faster
#define RESPONSIVENESS_COEFF 40   //  how fast changes in motion would affect game walking speed
#define DEAD_ZONE 20              //  minimal non-zero motion would result in this speed, if dead_zone = 30 then \
                                            //  walking speed of 5 would result in game action of 35
#define HYSTERESYS_COEFFICIENT 20 //  higher value results in more constant speed, but low sensitivity \
                                            //  for speed change. Lower value results in high sensitivity to speed change, but unstable speed
#define USE_DIGIPOT_REMAP 0       //  1 for on, 0 for off
#define DIGIPOT_RESISTANCE_KOHM 5 //  resistance of installed digipot

//====================================
//  cruise control
#define CRUISE_MIN_ANGLE 10
#define CRUISE_MAX_ANGLE 40
#define CRUISE_MIN_JOYSTICK_VALUE 25
#define CRUISE_MAX_JOYSTICK_VALUE 100

#define REQURIED_TIME 500

//====================================
//  weight sensors
#define MIN_BACKSTEP_POWER 200
#define MIN_SIDESTEP_POWER 200

//  side weight sensors, same for left and right
#define RIGHT_BACK_MIN_POWER 200
#define RIGHT_BACK_MAX_POWER 550
#define LEFT_BACK_MIN_POWER 200
#define LEFT_BACK_MAX_POWER 550
//  back weight sensors
#define RIGHT_SIDE_MIN_POWER 200
#define RIGHT_SIDE_MAX_POWER 550
#define LEFT_SIDE_MIN_POWER 200
#define LEFT_SIDE_MAX_POWER 550

//========================================================================
//  Pinout for TEENSY (do not edit without serious purpose)
//========================================================================

#define LEFT_BUTTON_PIN 4
#define RIGHT_BUTTON_PIN 7
#define CHEST_ACCEL_INT_PIN 8
#define POT_0_CS 6
#define POT_1_CS 5
#define Sensor_BR A1
#define Sensor_SR A7
#define Sensor_BL A0
#define Sensor_SL A6

//========================================================================
//  INTERFACES SETTING
//========================================================================

#define RIGHT_ACCEL_TWI_ADRESS 0x28
#define LEFT_ACCEL_TWI_ADRESS 0x29
#define CHEST_ACCEL_TWI_ADRESS 0x28

#define CUSTON_UART_SPEED 2000000
#define CUSTOM_WIRE_SPEED 400000
#define CUSTOM_WIRE1_SPEED 400000
#define CUSTOM_SPI_CLOCK_DIVIDER SPI_CLOCK_DIV16

#endif

//OLD CODES (press F to pay respect)

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
