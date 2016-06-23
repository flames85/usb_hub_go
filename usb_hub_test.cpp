//
// Created by root on 16-6-23.
//

#include "usb_hub_test.h"


UsbHubTest::UsbHubTest()
{

}

UsbHubTest::~UsbHubTest()
{


}

int UsbHubTest::go(const string &devName,
                   int speed,
                   int databits,
                   int parity,
                   int stopbits)
{
    m_ptty = readyTTY(devName.c_str());
    if(m_ptty == NULL)
    {
        printf("readyTTY(%s) error\n", devName.c_str());
        return -1;
    }

    if(!updateUsbHubTunnelAttrubute(speed, databits, parity, stopbits))
    {
        return -2;
    }
    return 0;
}

int UsbHubTest::send(char *pbuf, int size)
{
    return sendnTTY(m_ptty, pbuf, size);
}


int UsbHubTest::recv(char *pbuf, int size)
{
    return recvnTTY(m_ptty, pbuf, size);;
}


bool UsbHubTest::updateUsbHubTunnelAttrubute(int speed, int databits, int parity, int stopbits)
{
    // set speed
    if(setTTYSpeed(m_ptty, speed)>0)
    {
        printf("setTTYSpeed() error\n");
        return false;
    }
    if (setTTYParity(m_ptty, databits, parity, stopbits) > 0)
    {
        printf("setTTYParity() error\n");
        return false;
    }
    return true;
}
