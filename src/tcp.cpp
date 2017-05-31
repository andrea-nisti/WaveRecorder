#include <iostream>
#include "QtCore/qcoreapplication.h"
#include "QtNetwork/qtcpsocket.h"
#include <arpa/inet.h>
#include <cstring>

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

int main(int argc, char *argv[])
{

    //QCoreApplication a(argc, argv);

    //return a.exec();

    QTcpSocket *sck = new QTcpSocket(NULL);

    sck->connectToHost("192.168.1.160", 7007);

    if (sck->waitForConnected(5000)) {
        double vRefs[6];

       // while (true) {
            std::cout<<"Enter new refs: ";
            std::cin>>vRefs[0]>>vRefs[1]>>vRefs[2]>>vRefs[3]>>vRefs[4]>>vRefs[5];

            // Marshalling data
            for (int i = 0; i < 6; i++) {
                vRefs[i] = HostToNetD(vRefs[i]);
            }

            // Sending new data
            QByteArray rawData;
            rawData = QByteArray::fromRawData(reinterpret_cast<const char*>(vRefs), sizeof(double) * 6);
            std::cout<<"Raw data to send: "<<rawData.size()<<" bytes\n";
            sck->write(rawData, sizeof(double) * 6);
            sck->waitForBytesWritten();
        //}
    } else{
        std::cout << "connection timeout" << std::endl;
    }

    delete sck;

    return 0;
}
