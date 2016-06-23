#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <sstream>

using namespace std;

#define UEVENT_BUFFER_SIZE      2048
#define DEV_HUB_PREFIX_SIZE		1024
#define	DEV_HUB_PREFIX_CONF	    "usb_hub_prefix.conf"

#define DEFAULT_SPEED           9600
#define DEFAULT_DATABITS        8
#define DEFAULT_PARITY          'N'
#define DEFAULT_STOPBITS        1


//! 函数功能：string to int
//! nScale表示string是何进制
int str2int( const string &str, int nScale = 10);

#endif // _COMMON_H_