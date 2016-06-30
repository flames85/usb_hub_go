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
            return -2; // fail
        }
    }
}

int getDevName(int nSeq, char* pDevName, int maxLen)
{
    if(NULL == g_monitor)
    {
        printf("error: monitor nil\n");
        return -1;
    }
    string devName;
    if ( !g_monitor->getUsbHubTunnelName(nSeq, devName))
    {
        return -2;
    }
    strncpy(pDevName, devName.c_str(), maxLen);
    return 1;
}

PyObject * getExistUsbSeqArray()
{
    PyObject *retval = NULL;
    if(NULL == g_monitor)
    {
        printf("error: monitor nil\n");
        return retval;
    }

    vector<int> seqArray;
    if(!g_monitor->getExistSeqArray(seqArray))
    {
        return retval;
    }

    for(int index = 0; index < seqArray.size(); ++index)
    {
        if(NULL == retval)
        {
            retval = (PyObject*)Py_BuildValue("[i]", seqArray[index]);
        }
        PyModule_AddIntConstant(retval, "[i]", seqArray[index]);
    }

    return retval;
}

static PyObject* libUsbHubGo_getDevName(PyObject *self, PyObject *args)
{
    int nSeq;
    PyObject* retval = NULL;

    do{
        // args
        if (!PyArg_ParseTuple(args, "i", &nSeq))
        {
            retval = (PyObject*)Py_BuildValue("s", "");
            break;
        }
        // run c function
        char pDevName[64] = {0};
        if(getDevName(nSeq, pDevName, 64) < 0)
        {
            printf("not found!\n\n");
            retval = (PyObject*)Py_BuildValue("s", "");
            break;
        }
        // result
        retval = (PyObject*)Py_BuildValue("s", pDevName);

    } while(0);

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


static PyObject* libUsbHubGo_getExistUsbSeqArray(PyObject *self, PyObject *args)
{
    PyObject* retval = getExistUsbSeqArray();
    if(NULL == retval)
    {
        retval = (PyObject*)Py_BuildValue("i", 0);
    }
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
