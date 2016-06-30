#ifndef _USB_HUB_MONITOR_H
#define _USB_HUB_MONITOR_H

#include <string>
#include <map>
#include <vector>
#include "stty.h"
#include "common.h"

using namespace std;

enum CHANGE_TYPE
{
    TYPE_ADD = 0,
    TYPE_REMOVE = 1
};


struct UsbHubTunnelInfo
{
    __pid_t pid;
    string devName;
    UsbHubTunnelInfo()
    {
        pid = -1;
        devName.clear();
    }
};

class UsbHubMonitor
{
public:

    UsbHubMonitor();
    virtual ~UsbHubMonitor();

    int go();

    void addUsbHubTunnel(int nSeq, const string &devName);
    bool removeUsbHubTunnel(int nSeq);
    bool getUsbHubTunnelName(int nSeq, string &devName);
    bool setUsbHubTunnelPid(int nSeq, __pid_t pid);
    int dumpUsbSeqMapForJson(string &json);

private:

    void work(const string &strBuf, CHANGE_TYPE type);
    static void * threadProc(void* self);

private:
    pthread_mutex_t                m_mutex;
    map<int, UsbHubTunnelInfo>     m_usbSeqMap;
    string 	                      *m_strHubPrefix;
    pthread_t                      m_id;
};




#endif // _USB_HUB_MONITOR_H