//
// Created by andreanistico on 26/05/17.
//

#ifndef WAVERECORDER_SIMPLELOG_HPP
#define WAVERECORDER_SIMPLELOG_HPP
#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>

class Logger {

    std::ostream *m_out; // use pointer so you can change it at any point
    bool          m_owner;
public:
    // constructor is trivial (and ommited)
    Logger(){
        m_out= &std::cout; // defaults to stdout
        m_owner = false;
    }
    virtual ~Logger()
    {
        if (m_owner)
            delete m_out;
    }
    void setStream( std::ostream* stream, bool owner )
    {
        if(m_owner)
            delete m_out;
        m_out = stream;
        m_owner = owner;
    }
    template<typename T>
    Logger& operator << ( T& object)
    {
        if(!m_out)
            throw std::runtime_error("No stream set for Logger class");
        (*m_out) << object;
        m_out->flush();
        return *this;
    }
    //Manages  logger << std::endl
    Logger &operator <<(std::ostream& (*os)(std::ostream&))
    {
        if(!m_out)
            throw std::runtime_error("No stream set for Logger class");
        *m_out << os;
        m_out->flush();
        return *this;
    }

    void useFile(std::string filename) {

        setStream(new std::ofstream(filename),true);

        /*
        if(m_owner)
            delete m_out;

        m_out=new std::ofstream(filename);
        m_owner=true;
         */
    }
};

#endif //WAVERECORDER_SIMPLELOG_HPP
