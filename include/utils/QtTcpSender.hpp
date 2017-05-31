//
// Created by andreanistico on 30/05/17.
//

#ifndef WAVERECORDER_TCPSENDER_HPP
#define WAVERECORDER_TCPSENDER_HPP

#include <netinet/in.h>
#include <iostream>
#include "QtCore/qcoreapplication.h"
#include "QtNetwork/qtcpsocket.h"


class QtTcpSender{
    typedef std::vector<double> vectd_t;
public:
    QtTcpSender(QString _ip, quint16 _port) : sck(new QTcpSocket(NULL)), _connected(false),
                                                     _ip(_ip), _port(_port), _refMarsh{0}, _canSend(false)
    {}

    virtual ~QtTcpSender() {
        delete sck;

    }

    bool connect(int timeout=5){
        sck->connectToHost(_ip, _port);
        _connected = sck->waitForConnected(1000 * timeout);
        if(!_connected)
            std::cerr << "Unable to connect to: "<<_ip.toStdString()<< "|"<<_port<<std::endl;
        return _connected;
    }
    bool isConnected(){
        return _connected;
    }
    //Get Set
    vectd_t& reference(vectd_t *r=nullptr) {
        if(r == nullptr)
            return _ref;

        _ref.clear();
        _ref.assign(r->begin(),r->end());
        std::cout << _ref[2] << std::endl;
        return _ref;
    }
    void sendOverTcp(){
        // Sending new data
        setMarshed();
        if(_canSend) {
            _canSend = false;
            QByteArray rawData;
            rawData = QByteArray::fromRawData(reinterpret_cast<const char *>(_refMarsh), sizeof(double) * 6);
            std::cout << "Raw data to send: " << rawData.size() << " bytes\n";
            sck->write(rawData, sizeof(double) * 6);
            sck->waitForBytesWritten();
            _canSend = true;
        }
    }



private:
    QTcpSocket *sck;
    bool _connected;
    bool _canSend;
    double _refMarsh[6];
    vectd_t _ref;
    QString _ip;
    quint16 _port;

    //Marshalling
    double HostToNetD(double in)
    {
        int tempLo;
        int tempHi;
        double swapped;
        unsigned char *ptr;
        int *T;
        int a;

        T = &a;

        T = (int*)"\01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

        if (*T == 1)
        {

            ptr = (unsigned char *) &in;

            memcpy((unsigned char *) &tempHi, (unsigned char *) ptr, sizeof(int));
            tempHi = htonl(tempHi);
            memcpy((unsigned char *) &tempLo, (unsigned char *) (ptr + sizeof(int)), sizeof(int));
            tempLo = htonl(tempLo);

            ptr = (unsigned char *) &swapped;
            memcpy((unsigned char *) ptr, (unsigned char *) &tempLo, sizeof(int));
            memcpy((unsigned char *) (ptr + sizeof(int)), (unsigned char *) &tempHi, sizeof(int));

        }
        else
        {
            swapped = in;
        }

        return swapped;
    }
    void setMarshed(){
        if(_ref.size() == 6){
            int c = 0;
            for (vectd_t::iterator i = _ref.begin(); i < _ref.end(); ++i) {
                _refMarsh[c++] = HostToNetD(*i);

            }
            _canSend = true;
        } else {
            std::cout << "Ref vector is empty" << std::endl;
            _canSend = false;
        }
    }
};

#endif //WAVERECORDER_TCPSENDER_HPP
