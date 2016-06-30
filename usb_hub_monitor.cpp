#include <sstream>
#include <string.h>
#include <sys/socket.h>
#include <signal.h>
#include <wait.h>
#include "usb_hub_monitor.h"

UsbHubMonitor::UsbHubMonitor():
		m_strHubPrefix(NULL), m_id(0)
{
	pthread_mutex_init(&m_mutex, NULL);
}
UsbHubMonitor::~UsbHubMonitor()
{
	if(m_strHubPrefix)
	{
		delete m_strHubPrefix;
		m_strHubPrefix = NULL;
	}
}

void * UsbHubMonitor::threadProc(void* self)
{

	UsbHubMonitor *_self = (UsbHubMonitor*)self;
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
		printf("\n\t[%d] debug:recv:%s\n", getpid(), buf);
#endif
		string strBuf = buf;
		// 解析出开头是add, remove, 末尾有tty
		if(0 == strBuf.find("add@") && strBuf.find("tty/") != std::string::npos)
		{
#ifdef _DEBUG_
			printf("\n\t[%d] debug:add:%s\n", getpid(), strBuf.c_str());
#endif
			strBuf = strBuf.substr(4);
			_self->work(strBuf, TYPE_ADD);
		}
		else if(0 == strBuf.find("remove@") && strBuf.find("tty/") != std::string::npos)
		{
#ifdef _DEBUG_
			printf("\n\t[%d] debug:remove:%s\n", getpid(), strBuf.c_str());
#endif
			strBuf = strBuf.substr(7);
			_self->work(strBuf, TYPE_REMOVE);
		}
	}

}

int UsbHubMonitor::go()
{
//	printf("# monitor mode\n");

	FILE *fp = NULL;
	if( (fp = fopen(DEV_HUB_PREFIX_CONF, "rb") ) == NULL )
	{
		printf("# open conf[%s] fail, please run study mode first!\n", DEV_HUB_PREFIX_CONF);
		printf("# usage.\n\tusb_hub_monitor study\n\n");
		return -1;
	}

	char cHubPrefix[DEV_HUB_PREFIX_SIZE] = {0};
	memset(cHubPrefix, 0x0, sizeof(cHubPrefix));
	if(fread(cHubPrefix, sizeof(char), DEV_HUB_PREFIX_SIZE, fp) <=0 )
	{
		printf("# read conf[%s] fail, please run study mode first!\n", DEV_HUB_PREFIX_CONF);
		return -2;
	}
//	printf("# get dev hub prefix[%s]\n", cHubPrefix);

	m_strHubPrefix = new string(cHubPrefix);

	fclose(fp);

	pthread_create(&m_id, NULL, threadProc, this);

	return 0;
}

bool UsbHubMonitor::setUsbHubTunnelPid(int nSeq, __pid_t pid)
{
	pthread_mutex_lock(&m_mutex);
	bool bSet = true;
	do{
		if(m_usbSeqMap.find(nSeq) == m_usbSeqMap.end())
		{
			printf("\n\t[%d] usb-seq[%d] not exists\n", getpid(), nSeq);
			bSet = false;
			break;
		}
		UsbHubTunnelInfo &tunnelInfo = m_usbSeqMap.at(nSeq);
		if(-1 != tunnelInfo.pid)
		{
			kill(tunnelInfo.pid, 9);
			waitpid(tunnelInfo.pid, NULL, 0);
		}
		m_usbSeqMap[nSeq].pid = pid;

	} while(0);

	pthread_mutex_unlock(&m_mutex);

	return bSet;
}
bool UsbHubMonitor::getUsbHubTunnelName(int nSeq, string &devName)
{
	pthread_mutex_lock(&m_mutex);
	bool bGet = true;
	do{
		if(m_usbSeqMap.find(nSeq) == m_usbSeqMap.end())
		{
			bGet = false;
			break;
		}
		devName = m_usbSeqMap[nSeq].devName;

	} while(0);

	pthread_mutex_unlock(&m_mutex);

	return bGet;
}

bool UsbHubMonitor::getExistSeqArray(vector<int> &seqArray)
{
	bool bExists = false;
	map<int, UsbHubTunnelInfo>::const_iterator it = m_usbSeqMap.begin();
	for(; it != m_usbSeqMap.end(); ++it)
	{
		const int nSeq = it->first;
		seqArray.push_back(nSeq);
		bExists = true;
	}
	return bExists;
}

void UsbHubMonitor::addUsbHubTunnel(int nSeq, const string &devName)
{
	UsbHubTunnelInfo tunnelInfo;
	pthread_mutex_lock(&m_mutex);
	tunnelInfo.devName = devName;
	m_usbSeqMap[nSeq] = tunnelInfo;
	pthread_mutex_unlock(&m_mutex);
}

bool UsbHubMonitor::removeUsbHubTunnel(int nSeq)
{
	bool bSuccess = true;
	pthread_mutex_lock(&m_mutex);

	do{

		if(m_usbSeqMap.find(nSeq) == m_usbSeqMap.end())
		{
			bSuccess = false;
			break;
		}
		UsbHubTunnelInfo &tunnelInfo = m_usbSeqMap.at(nSeq);

		if(-1 != tunnelInfo.pid)
		{
			printf("\n\t[%d] kill pid[%d]\n", getpid(), tunnelInfo.pid);
			kill(tunnelInfo.pid, 9);
			waitpid(tunnelInfo.pid, NULL, 0);
		}

		m_usbSeqMap.erase(nSeq);

		printf("\n\t[%d] erase [%d]\n", getpid(), nSeq);

	} while(0);
	pthread_mutex_unlock(&m_mutex);

	return bSuccess;
}




void UsbHubMonitor::work(const string &strBuf, CHANGE_TYPE type)
{
	string devName;
	string usbSeq;

	size_t nPos = strBuf.find(*m_strHubPrefix);
	if(nPos == string::npos)
	{
		return ;
	}

	size_t nStart = nPos + m_strHubPrefix->size();

	nPos = strBuf.find("/", nStart);

	if(nPos == string::npos)
	{
		return ;
	}

	size_t nLen = nPos - nStart;

	usbSeq = strBuf.substr(nStart, nLen);

	nPos = strBuf.find_last_of("/");
	if(nPos == string::npos)
	{
		return ;
	}

	devName = strBuf.substr(nPos+1);


	int nSeq = str2int(usbSeq);

	if(nSeq >= 0)
	{
		if(TYPE_ADD == type)
		{
			addUsbHubTunnel(nSeq, devName);
//			printf("\n\t[%d] add: usb-no[%d] usb-name[%s]\n", getpid(), nSeq, devName.c_str());
		}
		else if(TYPE_REMOVE == type)
		{
			removeUsbHubTunnel(nSeq);
//			printf("\n\t[%d] remove: usb-no[%d] usb-name[%s]\n", getpid(), nSeq, devName.c_str());
		}
		else {
//			printf("\n\t[%d] unknow type[%d]", getpid(), type);
		}
	}
	else
	{
		printf("\n\t[%d] error usb-no[%s]", getpid(), usbSeq.c_str());
	}
}


