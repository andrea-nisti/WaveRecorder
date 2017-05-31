#include <iostream>
#include <sstream>
#include "WaveGen.h"
#include <chrono>
#include "lcm/lcm-cpp.hpp"
#include "geometry/pose.hpp"
#include "utils/TimeHelpers.hpp"
#include "utils/SimpleLog.hpp"

int main(int argc, char** argv){

    TimeManager timer;
    Spinner spinner;
    WaveGen w;
    double maxw = 3;
    double minw = 0.5;
    double U    = 5;

    w.init(5,maxw,minw,U);
    w.info();

    spinner.setRate(20);
    lcm::LCM handler;
    geometry::pose platPose;



    for (int j = 1; j < 4; ++j) {
        Logger log;
        std::ostringstream ss;
        ss <<"../log/"<< j <<"_waveLog_" <<minw<<"_"<<maxw<<".txt";
        log.useFile(ss.str());

        w.init(5,maxw,minw,U);
        timer.updateTimer();
        long int start = timer._actualTime;
        double max = 0;
        double min = 0;
        while((timer._actualTime - start) * MILLI2SECS < (60 * 4) && spinner.ok() ) {

            timer.updateTimer();
            double h = w.generateWaveHeightAtPoint(0,0,timer._actualTime * MILLI2SECS);

            platPose.position[2] = h;
            handler.publish("platRob",&platPose);


            if(h>max) max = h;
            if(h<min) min = h;
            //Log h
            log<<h<<" ";
        }
        log<<std::endl;
        //log info
        std::string s = w.info().str();
        log << s;
        log <<"Min and Max h: " <<std::endl<<min << " " << max <<std::endl;
    }

	return 0;
}


