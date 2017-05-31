//
// Created by andreanistico on 29/05/17.
//

#include <iostream>
#define strtk_no_tr1_or_boost
#include "strtk/strtk.hpp"
#include "utils/TimeHelpers.hpp"
#include "utils/QtTcpSender.hpp"

typedef std::vector<std::string> stringv_t;
typedef std::vector<double> doublev_t;

bool readPathInString(std::string, stringv_t&);
double string2Double(std::string);

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
    Spinner sp;
    TimeManager tm;
    sp.setRate(10);

    //Some time helpers
    int l = 0;
    tm.updateTimer();

    //Prepare socket
    QtTcpSender sock("192.168.1.160", 7007);
    sock.connect();
    if(sock.isConnected()){
        std::cout << "sending def position" << std::endl;
        doublev_t* defPosition = new doublev_t{0, -1, 0.93, 0, 0, 0};
        sock.reference(defPosition);
        sock.sendOverTcp();
        usleep((useconds_t)(5 / MICRO2SECS));
        delete defPosition;
    }
    std::cout << "starting" << std::endl;
    tm.updateTimer();
    long int start = tm._actualTime;
    while (/*sp.ok() && l<path.size() && */sock.isConnected()){

/*
        std::cout<<"Enter new refs: ";
        std::cin>>sock.reference()[0]>>sock.reference()[1]>>sock.reference()[2]>>sock.reference()[3]>>sock.reference()[4]>>sock.reference()[5];
*/
        double a = -1 + 0.5*sin(0.7 * (tm._actualTime - start)*tm.toSecs());
        sock.reference()[1] = a;
        sock.reference()[2] = -a-0.08;
        sock.sendOverTcp();



        /*
        double h = path[l++];
        //Send via socket
        std::cout << h << std::endl;
        */

        sock.sendOverTcp();
        tm.updateTimer();
    }
    tm.updateTimer();
    std::cout << "It took "<< tm._dt<<" seconds!" << std::endl;


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
double string2Double(std::string str){

    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    double value = atof(cstr);
    delete (cstr);
    return value;

}