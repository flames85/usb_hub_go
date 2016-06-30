
#include <Python.h>

extern "C" {

void goStudy();

    int goMonitor();
    char* getDevName(int nSeq);

    void initlibUsbHubGo();
    static PyObject* libUsbHubGo_getDevName(PyObject *self, PyObject *args);
    static PyObject* libUsbHubGo_goStudy(PyObject *self, PyObject *args);
    static PyObject* libUsbHubGo_goMonitor(PyObject *self, PyObject *args);

    static PyMethodDef libUsbHubGoMethods[] =
    {
            { "getDevName", libUsbHubGo_getDevName, METH_VARARGS },
            { "goStudy", libUsbHubGo_goStudy, METH_VARARGS },
            { "goMonitor", libUsbHubGo_goMonitor, METH_VARARGS },
            { NULL, NULL },
    };


}