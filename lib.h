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
    static PyObject* libUsbHubGo_getUsbDevName(PyObject *self, PyObject *args);
    static PyObject* libUsbHubGo_dumpUsbSeqMapForJson(PyObject *self, PyObject *args);

    // python API with description
    static PyMethodDef libUsbHubGoMethods[] =
    {
            { "goStudy", libUsbHubGo_goStudy, METH_VARARGS,
                    "Go study mode and blocked.\n\te.g. libUsbHubGo.goStudy()" },
            { "goMonitor", libUsbHubGo_goMonitor, METH_VARARGS,
                    "Go monitor mode in child thread.\n\te.g. libUsbHubGo.goMonitor()"},
            { "getUsbDevName", libUsbHubGo_getUsbDevName, METH_VARARGS,
                    "return the usb-dev-name[Python Type:str] of usb-hub (like ttyUSBx) with port sequence.\n\te.g. libUsbHubGo.getUsbDevName(3)" },
            { "dumpUsbSeqMapForJson", libUsbHubGo_dumpUsbSeqMapForJson, METH_VARARGS,
                    "return json string with all usb-seq for key usb-dev-name for value.\n\te.g. json = libUsbHubGo.dumpUsbSeqMapForJson()" },

            { NULL, NULL, 0, NULL },
    };

#ifdef __cplusplus
}
#endif


// C++ function
void goStudy();
int goMonitor();
int getUsbDevName(int nSeq, char* pDevName, int maxLen = 64);
int dumpUsbSeqMapForJson(char* pJson, int maxLen = 1024);

#endif // _LIB_H_
