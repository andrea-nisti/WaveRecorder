//
// Created by andreanistico on 31/01/17.
//

#include "WaveGen.h"

void WaveGen::init(int n, double maxw, double minw, double U) {

    _n = n;
    _minW = minw;
    _maxW = maxw;
    _U = U;

    //Init streams
    _phiSS.str("");
    _omSS.str("");
    _phiSS << "Phases:"<<std::endl;
    _omSS  << "Frequencies:"<<std::endl;
    //Start random engine
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_real_distribution<> distr(0, 2 * M_PI); // define the range

    if (_n >0) deltaW = (_maxW - _minW) / _n;
    else return;

    _w_vect.clear();
    _phi_vect.clear();
    //Fill frequency vector
    for (int i = 0; i < _n; ++i) {
        double t = _minW + i * deltaW;
        _w_vect.push_back(t);
        _omSS << t << " ";
        _omSS.flush();

    }
    //Fill phi vector
    for(int i=0; i<_n; ++i) {
        _phi_vect.push_back((float) distr(eng));
        _phiSS << _phi_vect[i] << " ";
        _phiSS.flush();
    }
    std::cout << _omSS.str() << std::endl;
    std::cout << _phiSS.str() << std::endl;

}

double WaveGen::S(double U, double w) {

    double A = (0.78 / pow(w,5));
    //if (U  == 0) return A;

    double t = A * (exp(-(0.74 * pow(9.81,4)) / (pow(U * w,4))));

    return t;

}

double WaveGen::eps(double w) {

    double t;

    t = sqrt(2 * S(_U,w) * deltaW);

    return t;
}

double WaveGen::generateWaveHeightAtPoint(double x, double y, double t, double beta) {

    double h = 0;
    //fetch values

    for (int i = 0; i < _n; ++i) {

        double wi  = _w_vect[i];
        double phi = _phi_vect[i];
        double ki  = pow(wi,2) / 9.81;
        double A   = eps(wi);

        h = h + A * cos(ki * x * cos(beta) + ki * y * sin(beta) - wi * t + phi);

    }

    return h;
}

std::ostringstream WaveGen::info() {

    std::ostringstream os;

    //Return a stream, why not.
    os <<"N:" <<_n << std::endl;
    os <<"deltaw:" <<deltaW << std::endl;
    os <<"maxW:" <<_maxW << std::endl;
    os <<"minW:" <<_minW << std::endl;
    os <<"U:"    <<_U << std::endl;
    os << _omSS.str()<<std::endl;
    os << _phiSS.str()<<std::endl;

    std::cout << os.str() << std::endl;
    return os;

}

WaveGen::WaveGen(): _ssinit(false) {}
























