//
// Created by andreanistico on 29/05/17.
//

#include <iostream>
#define strtk_no_tr1_or_boost
#include "strtk/strtk.hpp"
#include "utils/TimeHelpers.hpp"
#include "utils/QtTcpSender.hpp"
#define MIN_ALPHA 0.05
#define MAX_ALPHA 1.0
#define J_OFF -0.08
#define L_ARM 0.7
typedef std::vector<std::string> stringv_t;
typedef std::vector<double> doublev_t;

bool readPathInString(std::string, stringv_t&);
void readMinMax(std::string name, double& min, double& max);
double string2Double(std::string);
bool calculateAlpha(double&,double,double);

int main(int argc, char** argv){
    const std::string logPath = "../log/";
    std::string fd = "1";

    //Manage args
    if (argc > 1)
       fd=argv[1];
    std::cout << "opening: " + logPath +fd+".txt"<< std::endl;

    //Store path values in path vector as doubles
    stringv_t ret;
    doublev_t path;
    if(readPathInString(logPath+fd+".txt",ret)){
        for (stringv_t::iterator it=ret.begin(); it!=ret.end(); ++it) {
            path.push_back(string2Double(*it));
        }
    }

    //Playback, paths were recorded at 20Hz
    Spinner sp(7);
    TimeManager tm;

    //Some time helpers
    tm.updateTimer();

    //Prepare socket
    QtTcpSender sock("192.168.0.160", 7007);
    sock.connect();
    if(sock.isConnected()){
        std::cout << "sending def position" << std::endl;
        doublev_t* defPosition = new doublev_t{2.9, -1, 0.93, 0, 0, 0};
        sock.reference(defPosition);
        sock.sendOverTcp();
        usleep((useconds_t)(5 / MICRO2SECS));
        delete defPosition;
    }

    //Calculate wave parameters
    double min,max;
    readMinMax(logPath+fd+".txt",min,max);

    //Remark: straight robot position is -1.5 and flat is 0. We consider 1.5 and 0, after we negate it for simplicity
    double minh = L_ARM * std::sin(MIN_ALPHA);
    double h0 = fabs(min) + minh;
    int l = 0;


    std::cout << "starting" << std::endl;
    tm.updateTimer();
    bool sent = true;
    while (sp.ok() && l<path.size() && sock.isConnected()){
        /*
        std::cout<<"Enter new refs: ";
        std::cin>>sock.reference()[0]>>sock.reference()[1]>>sock.reference()[2]>>sock.reference()[3]>>sock.reference()[4]>>sock.reference()[5];
         double a = -1 + 0.5*sin(0.7 * (tm._actualTime - start)*tm.toSecs());
*/
        double a;
        if(calculateAlpha(a,path[l++],h0)){

            sock.reference()[1] = a;
            sock.reference()[2] = -a + J_OFF;
            sock.sendOverTcp();
            std::cout << a << std::endl;
        }

        tm.updateTimer();
    }

    return 0;
}

bool readPathInString(std::string name, stringv_t& path){

    std::ifstream myfile (name);
    if (myfile.is_open()) {

        //Build tokenized vector line by line
        std::string line;
        getline (myfile,line);

        std::vector<std::string> str_vector;
        strtk::parse(line," ",path);

        myfile.close();
        return true;
    }
    else{

        std::cout << "Unable to open file" << std::endl;
        return false;

    }

}
void readMinMax(std::string name, double& min, double& max){

    bool done = false;
    bool maxFound = false;
    bool minFound = false;
    std::ifstream myfile (name);
    std::string line;
    while (!done){

        getline (myfile,line);

        if(line == "Max:"){
            getline (myfile,line);
            max=string2Double(line);
            maxFound = true;

        }
        if(line == "Min:"){
            getline (myfile,line);
            min=string2Double(line);
            minFound = true;
        }

        done = minFound && maxFound;

    }
    std::cout << min << std::endl;
    std::cout << max << std::endl;

}
double string2Double(std::string str){

    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    double value = atof(cstr);
    delete (cstr);
    return value;

}

bool calculateAlpha(double& a,double h ,double h0){

    double arg = (h+h0)/L_ARM;
    if(arg > 1) arg  = 1;
    if(arg < -1) arg = -1;

    a = std::asin(arg);
    if(a<0) return false;
    //Clamp between min and max
    if(a >= MAX_ALPHA) a = MAX_ALPHA;
    if(a <= MIN_ALPHA) a = MIN_ALPHA;

    a=-a;
    return true;

}
