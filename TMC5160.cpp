/*
 * This file is related to the drivers for the stepper motor for the sensor fusion project
 *
 * Author: Majid Abdi
 */
#include <TMCStepper.h>
#include "Arduino.h"
#include "pin_names.h"
#include "TMC5160.h"

/*
 * This is the cunstructor for this object
 *
 * Inp:
 *      A reference (pointer) to TMC5160 object
 */
TMC5160::TMC5160(TMC5160Stepper *tmc, int microSwitchInitPin, int microSwitchfinalPin, int directionPin, int enablePin, int stepPin, bool logic) : _tmcDriver(tmc), _uSwitchInitPin(microSwitchInitPin), _uSwitchfinPin(microSwitchfinalPin), _dirPin(directionPin), _enPin(enablePin), _stepPin(stepPin), _logic(logic)
{
    timeFormatFactor = 300.0;
    radianConversion = 3.14 / 180.0;
    microStepsFactor = 32;
    dirForward = true;
    inited = false;
}

/*
 * This function will start the action! This function must be called after
 * SPI.begin() function
 *
 */
void TMC5160::begin()
{
    pinMode(_uSwitchfinPin, INPUT);
    pinMode(_uSwitchInitPin, INPUT);
    pinMode(_enPin, OUTPUT);
    pinMode(_stepPin, OUTPUT);
    pinMode(_dirPin, OUTPUT);
    digitalWrite(_enPin, LOW); // Enable driver in hardware
    digitalWrite(_dirPin, dirForward);
    _tmcDriver->begin();                      //  SPI: Init CS pins and possible SW SPI pins
    _tmcDriver->toff(5);                      // Enables driver in software
    _tmcDriver->rms_current(600);             // Set motor RMS current
    _tmcDriver->microsteps(microStepsFactor); // Set microsteps to 1/16th
    _tmcDriver->en_pwm_mode(true);            // Toggle stealthChop on TMC2130/2160/5130/5160
    _tmcDriver->pwm_autoscale(true);          // Needed for stealthChop

    calibrate_get_length(40);
}

/*
 * This function reset the values to the default
 *
 */
void TMC5160::reset_values()
{
    timeFormatFactor = 300;
    radianConversion = 3.14 / 180;
    microStepsFactor = 32;
    dirForward = true;
    calibrate_get_length(400);
}

/*
 * This function will return total length between microswitches
 *
 * o/p: Stepcount between microswitches
 */
uint32_t TMC5160::get_total_length()
{
    return totalLength;
}

/*
 * This function returns current position based on the refrence which was set at time of calibration
 *
 * o/p: Stepcounts from origin
 */
uint32_t TMC5160::get_current_position()
{
    return currentPosition;
}

/*
 * This function will return teh microstepping factor set by user
 *
 * o/p: Microstepping factor
 */
int TMC5160::get_microstepping_factor()
{
    return microStepsFactor;
}

/*
 * This function will get the frequency set by user
 *
 * o/p: frequency defined by user
 */
double TMC5160::get_frequency()
{
    return frequency;
}

/*
 * This function will return remaining step counts set by user
 *
 * o/p: remaining step counts
 */
uint32_t TMC5160::get_remaining_step_count()
{
    if (rounds <= 1)
        return stepCount;
    else
        return (rounds - 1) * tempCnt + stepCount;
}

/*
 * This function will return the current direction
 *
 * o/p: direction
 */
bool TMC5160::get_direction()
{
    return dirForward;
}

/*
 * This function will get the remaining rounds for the sinusoidal
 *
 * o/p: remaining rounds
 */
int TMC5160::get_remaining_rounds()
{
    return rounds;
}

/*
 * This function will set the desired frequency for the sinusoidal
 *
 * i/p: desired frequency
 */
void TMC5160::set_frequency(double freq)
{
    frequency = freq;
}

/*
 * This function will set the amplitude of the sinusoidal
 *
 * i/p: sinusoidal amplitude
 */
void TMC5160::set_amplitude(int amp)
{
    amplitude = amp;
}

/*
 * This function will set the microstepping factor for the driver
 *
 * i/p: microstepping factor
 */
void TMC5160::set_microstepping_factor(int uStepFct)
{
    microStepsFactor = uStepFct;
    _tmcDriver->microsteps(microStepsFactor);
}

/*
 * This function will set the desired stepcount
 *
 * i/p: step count
 */
void TMC5160::set_step_count(uint32_t stpCnt)
{
    stepCount = stpCnt;
    tempCnt = stepCount;
}

/*
 * This function will set the desired direction
 *
 * i/p: true-> forward
 *      false-> backward
 */
void TMC5160::set_direction(bool fwd)
{
    dirForward = fwd;
    digitalWrite(_dirPin, dirForward);
}

/*
 * This function will set the desired rounds
 *
 * i/p: desired rounds
 */
void TMC5160::set_rounds(int rnd)
{
    rounds = rnd;
}

/*
 * This function will return position to initial position
 *
 *
 */
void TMC5160::calibrate_zero()
{
    set_direction(false);
    while (digitalRead(_uSwitchInitPin) != _logic)
    {
        delayMicroseconds(400);
        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(400);
        digitalWrite(_stepPin, LOW);
    }
    set_direction(true);
}

/*
 * This function will calibrate the motors and also gets the total length
 *
 * i/p: desired speed of calibration (not less than 400)
 */
void TMC5160::calibrate_get_length(int speed)
{
    uint32_t stepLength1 = 0;
    uint32_t stepLength2 = 0;
    set_direction(false);
    while (digitalRead(_uSwitchInitPin) != _logic)
    {
        delayMicroseconds(speed);
        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(_stepPin, LOW);
    }
    set_direction(true);
    while (digitalRead(_uSwitchfinPin) != _logic)
    {
        stepLength1++;
        delayMicroseconds(speed);
        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(_stepPin, LOW);
    }
    set_direction(false);
    while (digitalRead(_uSwitchInitPin) != _logic)
    {
        stepLength2++;
        delayMicroseconds(speed);
        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(_stepPin, LOW);
    }
    set_direction(true);
    totalLength = (stepLength1 + stepLength2) / 2;
}

/*
 * This function will configure the parameters before each run
 *
 * i/p: frequency (double)
 *      amplitude (integer)
 *      step count in each round (uint32_t)
 *      direction (bool)
 *      number of rounds (int)
 */
void TMC5160::config(double f, int A, uint32_t cnt, bool dir, int rn)
{
    Serial.begin(115200);
    inited = false;
    set_frequency(f);
    set_amplitude(A);
    set_step_count(cnt);
    set_direction(dir);
    set_rounds(rn);
}

/*
 * Thid function will run the motor and needs to be called repeatedly
 *
 * i/p: current time
 */
void TMC5160::run()
{
    if (!inited)
    {
        inited = true;
        initTime = micros();
        timeStamp = initTime;
    }
    if (stepCount == 0)
    {
        if (rounds > 1)
        {
            stepCount = tempCnt;
            set_direction(!get_direction());
            rounds--;
        }
        if (!rounds)
            inited = false;
    }
    else
    {
        if (cond && micros() - timeStamp > timeCalculation / 2)
        {
            digitalWrite(_stepPin, HIGH);
            cond = false;
        }
        else if (!cond && micros() - timeStamp > timeCalculation)
        {
            timeStamp = micros();
            digitalWrite(_stepPin, LOW);
            cond = true;
            _time_calculator();
            stepCount--;
            timeCalculation += 150;
            timeCalculation += amplitude;
        }
    }
}

/*
 *  This function will calculate the time delay between steps;
 *
 *
 */
int TMC5160::_time_calculator()
{
    timeCalculation = int(amplitude * sin(frequency * radianConversion * (millis() - initTime) / timeFormatFactor));
    return timeCalculation;
}
