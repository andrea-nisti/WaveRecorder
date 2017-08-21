//
// Created by andreanistico on 26/05/17.
//

#ifndef WAVERECORDER_TIMEHELPERS_HPP
#define WAVERECORDER_TIMEHELPERS_HPP

#define  MILLI2SECS 0.001
#define  MICRO2SECS 0.000001
#define  NANO2SECS  0.000000001

#include <iostream>
#include <chrono>
#include <unistd.h>

class TimeManager {

    //Define the timer resolution
    enum unit {

        MILLI, //0
        MICRO, //1
        NANO   //2

    };

public:

    TimeManager() : _actualTime(0),_prevTime(0), _dt(0)
    {
        useMilli();
    }

    long int _actualTime;
    long int _prevTime;
    double _dt;

    void setUnit(unit un){
        _un = un;
    }
    unit getUnit(){
        return _un;
    }

    inline long int getTimeMilliSecond() {

        return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);

    }
    inline long int getTimeNanoSecond() {

        return std::chrono::system_clock::now().time_since_epoch() / std::chrono::nanoseconds(1);

    }
    inline long int getTimeMicroSecond() {

        return std::chrono::system_clock::now().time_since_epoch() / std::chrono::microseconds(1);

    }

    void useMilli(){
        _un=MILLI;
        _updateFunPtr = &TimeManager::milupdate;
        _toSecs = 0.001;
    }
    void useMicro(){
        _un=MICRO;
        _updateFunPtr = &TimeManager::micupdate;
        _toSecs = 0.000001;
    }
    void useNano(){
        _un=NANO;
        _updateFunPtr = &TimeManager::nupdate;
        _toSecs = 0.000000001;
    }
    void updateTimer(){
        (this->*_updateFunPtr)();
    }
    double toSecs(){
        return _toSecs;
    }

protected:

    unit _un;
    double _toSecs;
    void (TimeManager:: *_updateFunPtr)(void);

    void milupdate() {

        _actualTime = getTimeMilliSecond();
        double dt = _prevTime != 0 ? (_actualTime - _prevTime) * (double)MILLI2SECS : 0.0;
        _prevTime = _actualTime;
        _dt = dt;

    }
    void nupdate() {

        _actualTime = getTimeNanoSecond();
        double dt = _prevTime != 0 ? (_actualTime - _prevTime) * (double)NANO2SECS : 0.0;
        _prevTime = _actualTime;
        _dt = dt;
    }
    void micupdate() {

        _actualTime = getTimeMicroSecond();
        double dt = _prevTime != 0 ? (_actualTime - _prevTime) * (double)MICRO2SECS : 0.0;
        _prevTime = _actualTime;
        _dt = dt;

    }

};
//Spin a loop at a desired rate
class Spinner : public TimeManager{


public:
    Spinner(int r) : TimeManager(), _rate(r),_firstrun(true),_noW(false),_deadlineNotRespected(false)
    {
        useNano();
    }

    void setRate(int hz){
        if (hz > 0)
            _rate = hz;
    }
    void suppressWarnings(){
        _noW = true;
    }
    void enableWarnings(){
        _noW = false;
    }

    //Implements the waiting logic
    bool ok(){
        double T;
        if(_rate == 0){
            std::cout << "Rate is zero, exit.." << std::endl;
            return false;
        }
        else{

            T=1/(double)_rate;

            if(_firstrun){
                _firstrun = false;
                reset();
                updateTimer();
                return true;
            }

            //Read time when executing ok() and calculate dt
            updateTimer();

            if(_dt <= T){

                //Sleep time in seconds
                double sleepTime = (T - _dt);
                usleep((__useconds_t)(sleepTime / MICRO2SECS));

            }else{
                _deadlineNotRespected = true;
                //Your loop is to slow
                std::cout << "WARNING: your loop takes too long. Decrease your rate value" << std::endl;

                //Exit
                return _noW;
            }
        }
        //Start readings
        reset();
        updateTimer();
        return true;
    }

private:
    int _rate;
    bool _firstrun;
    bool _noW;
    bool _deadlineNotRespected;
    void reset(){
        _prevTime = _actualTime;
    }
};

class Duration : TimeManager{

public:
    //Duration starts when created
    Duration(int d) : TimeManager(), _duration(d){
        updateTimer();
        _start = _actualTime;
    }
    bool isExpired(){
        updateTimer();
        return ((_actualTime - _start) * (this->_toSecs)) >= (_duration);
    }

private:
    //duration in seconds
    int _duration;
    long int _start;
};

#endif //WAVERECORDER_TIMEHELPERS_HPP
