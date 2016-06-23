//
// Created by root on 16-6-23.
//

#ifndef USB_HUB_GO_USB_HUB_TEST_H
#define USB_HUB_GO_USB_HUB_TEST_H

#include <string>
#include "common.h"
#include "stty.h"

using namespace std;

class UsbHubTest
{
public:

    UsbHubTest();

    ~UsbHubTest();


    int go(const string &devName,
           int speed = DEFAULT_SPEED,
           int databits = DEFAULT_DATABITS ,
           int parity = DEFAULT_PARITY,
           int stopbits = DEFAULT_STOPBITS);

    int send(char *pbuf, int size);

    int recv(char *pbuf, int size);

private:

    bool updateUsbHubTunnelAttrubute(int speed, int databits, int parity, int stopbits);


private:
    TTY_INFO                       *m_ptty;
};


#endif //USB_HUB_GO_USB_HUB_TEST_H
