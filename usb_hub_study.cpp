#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>

#include "common.h"
#include "usb_hub_study.h"
#include "stty.h"


UsbHubStudy::UsbHubStudy() : m_strHubPrefix(NULL)
{

}
UsbHubStudy::~UsbHubStudy()
{
    if(m_strHubPrefix)
    {
        delete m_strHubPrefix;
        m_strHubPrefix = NULL;
    }
}

void UsbHubStudy::go() {

    printf("# study mode\n");
    printf("请将USB存储设备插拔同一个Hub的不同USB口, 然后退出.\n\n");

    // 读取消息数组
    char buf[UEVENT_BUFFER_SIZE * 2] = {0};
    // 串口socket
    int hotplug_sock = init_hotplug_sock();
    while(1)
    {
        // 清空
        memset(buf, 0x0, sizeof(buf));
        // 阻塞读取
        recv(hotplug_sock, &buf, sizeof(buf), 0);
#ifdef _DEBUG_
        printf("debug:recv:%s\n", buf);
#endif
        string strBuf = buf;
        // 解析出开头是add, remove, 末尾有tty
        if(0 == strBuf.find("add@") && strBuf.find("tty/") != std::string::npos)
        {
#ifdef _DEBUG_
            printf("debug:add:%s\n", strBuf.c_str());
#endif
            strBuf = strBuf.substr(4);
            work(strBuf);
        }
        else if(0 == strBuf.find("remove@") && strBuf.find("tty/") != std::string::npos)
        {
#ifdef _DEBUG_
            printf("debug:remove:%s\n", strBuf.c_str());
#endif
            strBuf = strBuf.substr(7);
            work(strBuf);
        }
    }
}


void UsbHubStudy::work(const string &strBuf)
{
    if(NULL == m_strHubPrefix)
    {
        m_strHubPrefix = new string(strBuf);
    }

    bool bSet = false;
    for(size_t index = 0; index < m_strHubPrefix->size() && index < strBuf.size(); index ++)
    {
        if(m_strHubPrefix->at(index) != strBuf.at(index))
        {
            *m_strHubPrefix = m_strHubPrefix->substr(0, index);
            bSet = true;
            break;
        }
    }

    if(bSet)
    {
        FILE *fp = NULL;
#ifdef _DEBUG_
        printf("# dev hub prefix is[%s] so far\n", m_strHubPrefix->c_str());
#endif
        if( (fp = fopen(DEV_HUB_PREFIX_CONF, "wb") ) == NULL )
        {
            printf("# open conf[%s] fail. 可能权限不足!\n", DEV_HUB_PREFIX_CONF);
            return;
        }
        fwrite(m_strHubPrefix->c_str(), sizeof(char), m_strHubPrefix->size(), fp);
        fclose(fp);
        printf("# Study ok! Bye\n");
        exit(0);
    }
    else
    {
        printf("# Please try another USB slot\n");
    }



}