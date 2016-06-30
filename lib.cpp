#include <stdio.h>
#include <iostream>
#include <string.h>
#include "usb_hub_monitor.h"
#include "usb_hub_study.h"
#include "usb_hub_test.h"
#include "lib.h"


static UsbHubMonitor *g_monitor = NULL;

char g_devNameBuf[64];

void goStudy()
{
    UsbHubStudy study;
    study.go();
}

int goMonitor()
{
    if(NULL == g_monitor)
    {
        g_monitor = new UsbHubMonitor;
        // monitor hub change
        if(0 != g_monitor->go())
        {
            return -1; // fail
        }
    }
}

char* getDevName(int nSeq)
{
    if(NULL == g_monitor)
    {
        printf("error: monitor nil\n");
        return NULL;
    }
    string devName;
    if ( !g_monitor->getUsbHubTunnelName(nSeq, devName))
    {
        return NULL;
    }
    strncpy(g_devNameBuf, devName.c_str(), sizeof(g_devNameBuf));
    return g_devNameBuf;
}

static PyObject* libUsbHubGo_getDevName(PyObject *self, PyObject *args)
{
    int nSeq;
    // args
    if (!PyArg_ParseTuple(args, "i", &nSeq))
        return NULL;
    // run c function
    char * devName = getDevName(nSeq);
    // result
    PyObject* retval = (PyObject*)Py_BuildValue("s", devName);
    return retval;
}

static PyObject* libUsbHubGo_goStudy(PyObject *self, PyObject *args)
{
    // run c function
    goStudy();
    return NULL;
}

static PyObject* libUsbHubGo_goMonitor(PyObject *self, PyObject *args)
{
    // run c function
    int ret = goMonitor();
    // result
    PyObject* retval = (PyObject*)Py_BuildValue("i", ret);
    return retval;
}


void initlibUsbHubGo()
{
    Py_InitModule("libUsbHubGo", libUsbHubGoMethods);
}

//int goTest() {
//    __pid_t nChildPID = 0;
//
//    string devName; // like ttyUSB0
//    int nSeq = 1; // if you have 8 usb in one hub. it will be 1,2,3,4,5,6,7,8
//
//    char in[100] = {0};
//    while(1)
//    {
//        // keyboard input cycle start
//        memset(in, 0x0, sizeof(in));
//        cout << "please choose usb-seq> ";
//        while (fgets(in, 100, stdin)) {
//
//            string strParamer = in;
//            if(0 == strParamer.compare(0, 4, "quit") || 0 == strParamer.compare(0, 6, "quit()"))
//            {
//                printf("bye!\n");
//                return 0;
//            }
//
//            nSeq = atoi(in);
//            if(nSeq <= 0)
//            {
//                cout << "please choose usb-seq> ";
//                continue;
//            }
//            // seq if exists?
//            if ( !go_monitor->getUsbHubTunnelName(nSeq, devName)) {
//                printf("\n\t[%d] usb-seq[%d] not exists\n\n", getpid(), nSeq);
//                cout << "please choose usb-seq> ";
//                continue;
//            }
//            break;
//        }
//        // keyboard input cycle end
//
//        // create child process
//        nChildPID = fork();
//        if (nChildPID < 0) {
//            printf("error in fork!\n"); // error
//        }
//        else if (nChildPID == 0) // child process[for test]
//        {
//            do {
//                printf("[*%d] test dev-name[%s]\n", getpid(), devName.c_str());
//
//                UsbHubTest test;
//                test.go(devName);
//
//                test.send("Hello", 5);
//                printf("[*%d] send test msg Hello\n", getpid());
//
//                char buff[12] = {0};
//                test.recv(buff, 5);
//                printf("[*%d] recv test msg[%s]\n", getpid(), buff);
//
//                sleep(100);
//                printf("[*%d] test finished\n", getpid());
//            } while(0);
//            exit(0);
//        }
//        else // main process for keyboard
//        {
//            printf("[%d] create child-PID %d, test dev-name[%s]\n", getpid(), nChildPID, devName.c_str());
//            monitor->setUsbHubTunnelPid(nSeq, nChildPID);
//        }
//        sleep(1);
//    }
//    return 0;
//
//}
//
//int main(int argc,char **argv)
//{
//#ifdef _DEBUG_
//    printf("# debug mode\n");
//#endif
//
//    // study mode
//    if(argc > 1)
//    {
//        goStudy();
//        return 0;
//    }
//
//    return goTest();
//}
