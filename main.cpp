#include <stdio.h>
#include <iostream>
#include <string.h>
//#include <sys/types.h>
//#include <sys/wait.h>
#include "usb_hub_monitor.h"
#include "usb_hub_study.h"
#include "usb_hub_test.h"
#include "common.h"

int main(int argc,char **argv)
{
#ifdef _DEBUG_
    printf("# debug mode\n");
#endif

    // study mode
    if(argc > 1)
    {
        UsbHubStudy study;
        study.go();
        return 0;
    }

    // monitor hub change
    UsbHubMonitor monitor;
    if(0 != monitor.go())
    {
        return -1; // fail
    }

//    sleep(3);
    __pid_t nChildPID = 0;

    string devName; // like ttyUSB0
    int nSeq = 1; // if you have 8 usb in one hub. it will be 1,2,3,4,5,6,7,8

    char in[100] = {0};
    while(1)
    {
        // keyboard input cycle start
        memset(in, 0x0, sizeof(in));
        cout << "please choose usb-seq> ";
        while (fgets(in, 100, stdin)) {
            nSeq = atoi(in);
            // seq if exists?
            if (nSeq <= 0 || !monitor.getUsbHubTunnelName(nSeq, devName)) {
                printf("\t[%d] usb-seq[%d] not exists\n", getpid(), nSeq);
                cout << "please choose usb-seq> ";
                continue;
            }
            break;
        }
        // keyboard input cycle end

        // create child process
        nChildPID = fork();
        if (nChildPID < 0) {
            printf("error in fork!\n"); // error
        }
        else if (nChildPID == 0) // child process[for test]
        {
            do {
                printf("[*%d] test dev-name[%s]\n", getpid(), devName.c_str());

                UsbHubTest test;
                test.go(devName);

                printf("[*%d] going to test[%s]\n", getpid(), devName.c_str());

                test.send("Hello", 5);

                printf("[*%d] send test msg Hello\n", getpid());

                char buff[12] = {0};
                test.recv(buff, 5);

                printf("[*%d] recv test msg[%s]\n", getpid(), buff);

                sleep(100);

                printf("[*%d]exit\n", getpid());

            } while(0);
            exit(0);
        }
        else // main process for keyboard
        {
            printf("[%d] create child-PID %d, test dev-name[%s]\n", getpid(), nChildPID, devName.c_str());
            monitor.setUsbHubTunnelPid(nSeq, nChildPID);
        }
        sleep(1);
    }
}
