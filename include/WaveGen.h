//
// Created by andreanistico on 31/01/17.
//

#ifndef LANDINGPLAT_WAVEGEN_H
#define LANDINGPLAT_WAVEGEN_H

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>

class WaveGen {

private:
    int _n;     //number of sampled frequencies
    double _maxW;
    double _minW;
    double deltaW;
    double _U; //wind speed
    double S(double U , double w);
    double eps(double w);

    //Helper streams for info
    std::ostringstream _phiSS;
    std::ostringstream _omSS;
    bool _ssinit;

public:
    std::ostringstream info();
    //sampled frequencies
    std::vector < double > _w_vect;
    //random phases
    std::vector <double> _phi_vect;
    void init(int n, double maxw, double minw, double U);
    double generateWaveHeightAtPoint(double x, double y, double t, double beta = 0);
    WaveGen();

};


#endif //LANDINGPLAT_WAVEGEN_H
