#ifndef _TMC5160_H_
#define _TMC5160_H_

#define R_SENSE 0.11f // Match to your driver
                      // SilentStepStick series use 0.11
                      // UltiMachine Einsy and Archim2 boards use 0.2
                      // Panucatt BSD2660 uses 0.1
                      // Watterott TMC5160 uses 0.075
#define PIN_CONTACT_LOGIC 0
class TMC5160
{
private:
    int _uSwitchInitPin;
    int _uSwitchfinPin;
    int _dirPin;
    int _enPin;
    int _stepPin;

private:
    double frequency;
    double radianConversion;
    double timeFormatFactor;
    int amplitude;
    int microStepsFactor;

    int rounds;

    uint32_t currentPosition;
    uint32_t totalLength;

    uint32_t stepCount; // desired
    uint32_t tempCnt;
    uint32_t timeStamp;
    uint32_t initTime;
    int timeCalculation;

    bool dirForward;
    bool cond;
    bool inited;
    bool _logic;

private:
    TMC5160Stepper *_tmcDriver;

public:
    TMC5160(TMC5160Stepper *tmc, int microSwitchInitPin, int microSwitchfinalPin, int directionPin, int enablePin, int stepPin, bool logic);
    void begin();
    void reset_values();

    uint32_t get_total_length();
    uint32_t get_current_position();
    int get_microstepping_factor();
    double get_frequency();
    uint32_t get_remaining_step_count();
    bool get_direction();
    int get_remaining_rounds();

    void set_frequency(double freq);
    void set_amplitude(int amp);
    void set_microstepping_factor(int uStepFct);
    void set_step_count(uint32_t stpCnt);
    void set_direction(bool fwd);
    void set_rounds(int rnd);

    void calibrate_zero();
    void calibrate_get_length(int speed);

    void config(double f, int A, uint32_t cnt, bool dir, int rn);
    void run();

private:
    int _time_calculator();
};

#endif
