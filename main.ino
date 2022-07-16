/*
 * This is an example of the sinusoidal movement of the motors
 *
 * The frequency must remain below the capability of the processor, for higher Frequencies, more powerful MCU should be used.
 *
 * ***************************************************************************************************************************
 * Author: Majid Abdi
 *
 * This code is free to use for all purposes with referencing the author(s)
 * ***************************************************************************************************************************
 * To use this code, an instance of TMC5160Stepper should be created and passed to TMC5160 object for sinusoidal movements.
 * The setup would be like the following:
 *
 * TMC5160Stepper *driver = new TMC5160Stepper(TMC_CS_PIN, R_SENSE);
 * TMC5160 sinusoidal_test = TMC5160 (TMC5160Stepper *tmc, int microSwitchInitPin, int microSwitchfinalPin, int directionPin, int enablePin, int stepPin, bool logic);
 *
 * TMC5160Stepper *tmc  -> driver
 * microSwitchInitPin -> The pin related to initial position for calibration
 * microSwitchfinalPin  -> The pin related to final position for calibration
 * directionPin -> The pin related to motor direction (connected to driver)
 * enablePin  -> enable pin of the driver
 * stepPin  -> step pin of the driver
 * logic  -> high or low logic of the switches (low active or high active)
 *
 * In the setup part
 * SPI.begin();
 * sinusoidal_test.begin();
 * sinusoidal_test.config(double f, int A, uint32_t cnt, bool dir, int rn);
 *
 *
 *  f -> frequency of the motor
 *  A -> amplitude of the delay for the motor
 *  cnt -> step count for the motor
 *  dir -> start direction of motor
 *  rn -> repeat time for the motor
 *
 * At the end, in the loop:
 * the sinusoidal_test.run();
 * should be called repeatedly
 */
/*
******************************************************************************************************************************
*********************************************AN EXAMPLE FOR THREE MOTORS******************************************************
******************************************************************************************************************************
*/
//#include <TMCStepper.h>
//#include "pin_names.h"
//#include "TMC5160.h"
//
//#define MOTOR1_STEP_PIN 4
//#define MOTOR1_DIR_PIN 5
//#define MOTOR1_EN_PIN (int)A7
//#define MOTOR1_BACKCAL_PIN (int)A6
//#define MOTOR1_FORCAL_PIN (int)A1
//#define MOTOR1_CS_PIN 10
//
//#define MOTOR2_STEP_PIN 2
//#define MOTOR2_DIR_PIN 3
//#define MOTOR2_EN_PIN (int)A7
//#define MOTOR2_BACKCAL_PIN (int)A3
//#define MOTOR2_FORCAL_PIN (int)A4
//#define MOTOR2_CS_PIN 9
//
//#define MOTOR3_STEP_PIN 6
//#define MOTOR3_DIR_PIN 7
//#define MOTOR3_EN_PIN (int)A7
//#define MOTOR3_BACKCAL_PIN (int)A2
//#define MOTOR3_FORCAL_PIN (int)A5
//#define MOTOR3_CS_PIN 8
//
//
// TMC5160Stepper *driver1 = new TMC5160Stepper(MOTOR1_CS_PIN, R_SENSE);
// TMC5160Stepper *driver2 = new TMC5160Stepper(MOTOR2_CS_PIN, R_SENSE);
// TMC5160Stepper *driver3 = new TMC5160Stepper(MOTOR3_CS_PIN, R_SENSE);
// TMC5160 motor1(driver1, MOTOR1_FORCAL_PIN, MOTOR1_BACKCAL_PIN, MOTOR1_DIR_PIN, MOTOR1_EN_PIN, MOTOR1_STEP_PIN, false);
// TMC5160 motor2(driver2, MOTOR2_FORCAL_PIN, MOTOR2_BACKCAL_PIN, MOTOR2_DIR_PIN, MOTOR1_EN_PIN, MOTOR2_STEP_PIN, false);
// TMC5160 motor3(driver3, MOTOR3_FORCAL_PIN, MOTOR3_BACKCAL_PIN, MOTOR3_DIR_PIN, MOTOR1_EN_PIN, MOTOR3_STEP_PIN, false);
// void setup() {
//  SPI.begin();
//  motor1.begin();
//  motor1.config(20, 2000, 10000, true, 2);
//  motor2.begin();
//  motor2.config(15, 500, 50000, true, 2);
//  motor3.begin();
//  motor3.config(30, 2000, 10000, true, 2);
//
//}
//
// void loop() {
//  // put your main code here, to run repeatedly:
//  //motor1.run();
//  //motor2.run();
//  //motor3.run();
//}
/*
******************************************************************************************************************************
***************************************************END OF THE EXAMPLE*********************************************************
******************************************************************************************************************************
*/

#include <TMCStepper.h>
#include "pin_names.h"
#include "TMC5160.h"

TMC5160Stepper *driver = new TMC5160Stepper(53, R_SENSE);
TMC5160Stepper *driver1 = new TMC5160Stepper(10, R_SENSE);
TMC5160Stepper *driver2 = new TMC5160Stepper(4, R_SENSE);
TMC5160 tst(driver, 28, 26, 8, 7, 9, false);
TMC5160 tst1(driver1, 22, 32, 11, 7, 12, false);
TMC5160 tst2(driver2, 30, 24, 5, 7, 6, false);
void setup()
{
  SPI.begin();
  tst.begin();
  tst.config(5, 500, 55000, true, 2);
  tst2.begin();
  tst2.config(10, 500, 55000, true, 2);
  tst1.begin();
  tst1.config(15, 500, 55000, true, 2);
}

void loop()
{
  tst.run();  // motor 2
  tst1.run(); // motor 3
  tst2.run(); // motor 1
}
