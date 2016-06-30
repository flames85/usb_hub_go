#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>


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

//! 函数功能：int to string
//! nScale表示num是什么进制
//! nWidth表示生成的字符串的宽度（不足0的在前面以0填充）
const string int2str( int num, int nScale = 10, int nWidth = 0);

#endif // _COMMON_H_