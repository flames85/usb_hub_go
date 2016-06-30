#ifndef _LIB_H_
#define _LIB_H_
#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

    // python template function
    void initlibUsbHubGo();
    static PyObject* libUsbHubGo_goStudy(PyObject *self, PyObject *args);
    static PyObject* libUsbHubGo_goMonitor(PyObject *self, PyObject *args);
    static PyObject* libUsbHubGo_getDevName(PyObject *self, PyObject *args);
    static PyObject* libUsbHubGo_getExistUsbSeqArray(PyObject *self, PyObject *args);

    // python API with description
    static PyMethodDef libUsbHubGoMethods[] =
    {
            { "goStudy", libUsbHubGo_goStudy, METH_VARARGS,
                    "Go study mode and blocked.\n\t eg. libUsbHubGo.goStudy()" },
            { "goMonitor", libUsbHubGo_goMonitor, METH_VARARGS,
                    "Go monitor mode in child thread\n\t eg. libUsbHubGo.goMonitor()"},
            { "getDevName", libUsbHubGo_getDevName, METH_VARARGS,
                    "return the dev-name[Python Type:str] of usb-hub (like ttyUSBx) with port sequence.\n\t eg. libUsbHubGo.getDevName(3)" },
            { "getExistUsbSeqArray", libUsbHubGo_getExistUsbSeqArray, METH_VARARGS,
                    ".\n\t eg. portArray = libUsbHubGo.getExistUsbSeqArray()" },

            { NULL, NULL, 0, NULL },
    };

#ifdef __cplusplus
}
#endif


// C++ function
void goStudy();
int goMonitor();
int getDevName(int nSeq, char* pDevName, int maxLen = 64);
PyObject* getExistUsbSeqArray();

#endif // _LIB_H_