#ifndef _USB_HUB_STUDY_H
#define _USB_HUB_STUDY_H

#include <string>


using namespace std;

class UsbHubStudy
{
public:

    UsbHubStudy();
    ~UsbHubStudy();

    void go();

private:
    void work(const string &strBuf);

private:

    string 	            *m_strHubPrefix;
};


#endif