#include <stdio.h>
#include <string.h>
#include "usb_hub_monitor.h"
#include "usb_hub_study.h"

int main(int argc,char **argv)
{
#ifdef _DEBUG_
    printf("# debug mode\n");
#endif

    if(argc > 1)
    {
        UsbHubStudy study;
        study.go();
        return 0;
    }

    UsbHubMonitor monitor;
    monitor.go();

    int nRet = 0;

    int idx = 0;
    char buff[6];
    while(1)
    {
        memset(buff, 0x0, sizeof(buff));
        // send
        nRet = monitor.sendUsbHubTunnel(1, "Hello", 5, 9600, 8, 'N', 1);
        printf("(%d)send[Hello]\n", idx++);
        // recv
        nRet = monitor.recvUsbHubTunnel(1, buff, 5);
        sleep(1);
        printf("(%d)recv[%s]\n", idx++, buff);
    }
}
