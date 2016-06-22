#include <sstream>
#include <string.h>
#include <sys/socket.h>
#include "usb_hub_monitor.h"
#include "common.h"


UsbHubMonitor::UsbHubMonitor():
		m_strHubPrefix(NULL), m_id(0)
{

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
			_self->work(strBuf, TYPE_ADD);
		}
		else if(0 == strBuf.find("remove@") && strBuf.find("tty/") != std::string::npos)
		{
#ifdef _DEBUG_
			printf("debug:remove:%s\n", strBuf.c_str());
#endif
			strBuf = strBuf.substr(7);
			_self->work(strBuf, TYPE_REMOVE);
		}
	}
}

int UsbHubMonitor::go()
{
	printf("# test mode\n");

	FILE *fp = NULL;
	if( (fp = fopen(DEV_HUB_PREFIX_CONF, "rb") ) == NULL )
	{
		printf("open conf[%s] fail, please run init mode first!\n", DEV_HUB_PREFIX_CONF);
		printf("usage.\n\tusb_hub_monitor init\n\n");
		return -1;
	}

	char cHubPrefix[DEV_HUB_PREFIX_SIZE] = {0};
	memset(cHubPrefix, 0x0, sizeof(cHubPrefix));
	if(fread(cHubPrefix, sizeof(char), DEV_HUB_PREFIX_SIZE, fp) <=0 )
	{
		printf("read conf[%s] fail, please run init mode first!\n", DEV_HUB_PREFIX_CONF);
		return -2;
	}
	printf("get dev hub prefix[%s]\n", cHubPrefix);

	m_strHubPrefix = new string(cHubPrefix);

	fclose(fp);

	pthread_create(&m_id, NULL, threadProc, this);

	return 0;
}



void UsbHubMonitor::addUsbHubTunnel(int nSeq, UsbHubTunnelInfo *tunnelInfo)
{
	pthread_mutex_lock(&m_mutex);
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

		UsbHubTunnelInfo *tunnelInfo = m_usbSeqMap[nSeq];

		if(tunnelInfo->ptty)
		{
			cleanTTY(tunnelInfo->ptty);
		}

		delete tunnelInfo;

		m_usbSeqMap.erase(nSeq);

	} while(0);
	pthread_mutex_unlock(&m_mutex);

	return bSuccess;
}


bool UsbHubMonitor::updateUsbHubTunnelAttrubute(UsbHubTunnelInfo *tunnelInfo, int speed, int databits, int parity, int stopbits)
{
	// set speed
	if(setTTYSpeed(tunnelInfo->ptty, speed)>0)
	{
		printf("setTTYSpeed() error\n");
		return false;
	}
	if (setTTYParity(tunnelInfo->ptty, databits, parity, stopbits) > 0)
	{
		printf("setTTYParity() error\n");
		return false;
	}
	return true;
}

int UsbHubMonitor::sendUsbHubTunnel(int nSeq, char *pbuf, int size, int speed, int databits, int parity, int stopbits)
{
	int nRet = 0;
	pthread_mutex_lock(&m_mutex);
	do{
		if(m_usbSeqMap.find(nSeq) == m_usbSeqMap.end())
		{
			nRet = -1;
			break;
		}
		UsbHubTunnelInfo *tunnelInfo = m_usbSeqMap[nSeq];
		if(NULL == tunnelInfo)
		{
			printf("usb-seq[%d] not exists\n", nSeq);
			nRet = -2;
			break;
		}

		// create bby fd
		if(NULL == tunnelInfo->ptty)
		{
			tunnelInfo->ptty = readyTTY(tunnelInfo->devName.c_str());
			if(tunnelInfo->ptty == NULL)
			{
				printf("readyTTY(%s) error\n", tunnelInfo->devName.c_str());
				nRet = -3;
				break;
			}
		}

		if(!updateUsbHubTunnelAttrubute(tunnelInfo, speed, databits, parity, stopbits))
		{
			nRet = -4;
			break;
		}

		nRet = sendnTTY(tunnelInfo->ptty, pbuf, size);

	} while(0);
	pthread_mutex_unlock(&m_mutex);
	return nRet;

}
int UsbHubMonitor::sendUsbHubTunnel(int nSeq, char *pbuf, int size)
{
	int nRet = 0;
	pthread_mutex_lock(&m_mutex);
	do{
		if(m_usbSeqMap.find(nSeq) == m_usbSeqMap.end())
		{
			nRet = -1;
			break;
		}
		UsbHubTunnelInfo *tunnelInfo = m_usbSeqMap[nSeq];
		if(NULL == tunnelInfo)
		{
			printf("usb-seq[%d] not exists\n", nSeq);
			nRet = -2;
			break;
		}

		// create bby fd
		if(NULL == tunnelInfo->ptty)
		{
			tunnelInfo->ptty = readyTTY(tunnelInfo->devName.c_str());
			if(tunnelInfo->ptty == NULL)
			{
				printf("readyTTY(%s) error\n", tunnelInfo->devName.c_str());
				nRet = -3;
				break;
			}
		}

		if(!updateUsbHubTunnelAttrubute(tunnelInfo, DEFAULT_SPEED, DEFAULT_DATABITS, DEFAULT_PARITY, DEFAULT_STOPBITs))
		{
			nRet = -4;
			break;
		}

		nRet = sendnTTY(tunnelInfo->ptty, pbuf, size);

	} while(0);
	pthread_mutex_unlock(&m_mutex);
	return nRet;
}

int UsbHubMonitor::recvUsbHubTunnel(int nSeq, char *pbuf, int size, int speed, int databits, int parity, int stopbits)
{
	int nRet = 0;
	pthread_mutex_lock(&m_mutex);
	do {

		if(m_usbSeqMap.find(nSeq) == m_usbSeqMap.end())
		{
			nRet = -1;
			break;
		}
		UsbHubTunnelInfo *tunnelInfo = m_usbSeqMap[nSeq];
		if(NULL == tunnelInfo)
		{
			printf("usb-seq[%d] not exists\n", nSeq);
			nRet = -2;
			break;
		}

		// create bby fd
		if(NULL == tunnelInfo->ptty)
		{
			tunnelInfo->ptty = readyTTY(tunnelInfo->devName.c_str());
			if(tunnelInfo->ptty == NULL)
			{
				printf("readyTTY(%s) error\n", tunnelInfo->devName.c_str());
				nRet = -3;
				break;
			}
		}

		if(!updateUsbHubTunnelAttrubute(tunnelInfo, speed, databits, parity, stopbits))
		{
			nRet = -4;
			break;
		}

		nRet = recvnTTY(tunnelInfo->ptty, pbuf, size);
	} while(0);
	pthread_mutex_unlock(&m_mutex);

	return nRet;

}
int UsbHubMonitor::recvUsbHubTunnel(int nSeq, char *pbuf, int size)
{
	int nRet = 0;
	pthread_mutex_lock(&m_mutex);
	do {

		if(m_usbSeqMap.find(nSeq) == m_usbSeqMap.end())
		{
			nRet = -1;
			break;
		}
		UsbHubTunnelInfo *tunnelInfo = m_usbSeqMap[nSeq];
		if(NULL == tunnelInfo)
		{
			printf("usb-seq[%d] not exists\n", nSeq);
			nRet = -2;
			break;
		}

		// create bby fd
		if(NULL == tunnelInfo->ptty)
		{
			tunnelInfo->ptty = readyTTY(tunnelInfo->devName.c_str());
			if(tunnelInfo->ptty == NULL)
			{
				printf("readyTTY(%s) error\n", tunnelInfo->devName.c_str());
				nRet = -3;
				break;
			}
		}

		if(!updateUsbHubTunnelAttrubute(tunnelInfo, DEFAULT_SPEED, DEFAULT_DATABITS, DEFAULT_PARITY, DEFAULT_STOPBITs))
		{
			nRet = -4;
			break;
		}

		nRet = recvnTTY(tunnelInfo->ptty, pbuf, size);
	} while(0);
	pthread_mutex_unlock(&m_mutex);

	return nRet;
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
			UsbHubTunnelInfo *tunnelInfo = new UsbHubTunnelInfo();
			tunnelInfo->devName = devName;
			addUsbHubTunnel(nSeq, tunnelInfo);
			printf("# add: usb-no[%d] usb-name[%s]\n", nSeq, devName.c_str());
		}
		else if(TYPE_REMOVE == type)
		{
			removeUsbHubTunnel(nSeq);
			printf("# remove: usb-no[%d] usb-name[%s]\n", nSeq, devName.c_str());
		}
		else {
			printf("# unknow type[%d]", type);
		}
	}
	else
	{
		printf("# error usb-no[%s]", usbSeq.c_str());
	}
}


//! 函数功能：string to int
//! nScale表示string是何进制
int UsbHubMonitor::str2int( const string &str, int nScale)
{
	stringstream ss( str);
	int num;
	switch( nScale)
	{
		case 16:
			if(( ss >> hex >> num). fail())
			{
				// ERROR
			}
			break;
		case 10:
			if(( ss >> dec >> num). fail())
			{
				//ERROR
			}
			break;
		case 8:
			if(( ss >> oct >> num). fail())
			{
				//ERROR
			}
			break;
		default:
			// ERROR
			break;
	}
	return num;
}