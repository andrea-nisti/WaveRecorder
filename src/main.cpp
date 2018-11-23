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
    Spinner spinner(20);
    WaveGen w;
    double maxw = 1.3;
    double minw = 0.2;
    double U    = 6;

    spinner.suppressWarnings();
    lcm::LCM handler;
    geometry::pose platPose;

    int n;
    int ns;
    int secs;
    std::cout << "How many file should I generate?" << std::endl;
    std::cin >> n;
    std::cout << "For how much time (seconds)?" << std::endl;
    std::cin >> secs;
    std::cout << "Max freq: " << std::endl;
    std::cin >> maxw;
    std::cout << "Min freq: " << std::endl;
    std::cin >> minw;
    std::cout << "Wind speed: " << std::endl;
    std::cin >> U;
    std::cout << "Finally, how many sinusoids?: " << std::endl;
    std::cin >> ns;




    for (int j = 0; j < n; ++j) {
        Logger log;
        std::ostringstream ss;
        ss <<"../log/"<< j <<"_waveLog_" <<minw<<"_"<<maxw<<".txt";
        log.useFile(ss.str());

        w.init(ns,maxw,minw,U);
        timer.updateTimer();
        long int start = timer._actualTime;
        double max = 0;
        double min = 0;
        while((timer._actualTime - start) * MILLI2SECS < (secs) && spinner.ok() ) {

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

        log <<"Max:" <<std::endl<<max <<std::endl;
        log <<"Min:" <<std::endl<<min <<std::endl;

    }

	return 0;
}


