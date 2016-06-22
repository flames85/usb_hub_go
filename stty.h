/*串口设备无论是在工控领域，还是在嵌入式设备领域，应用都非常广泛。而串口编程也就显得必不可少。
  偶然的一次机会，需要使用串口，而且操作系统还要求是Linux，因此，趁着这次机会，综合别人的代码，
  进行了一次整理和封装。具体的封装格式为C代码，这样做是为了很好的移植性，使它可以在C和C++环境下，
  都可以编译和使用。代码的头文件如下： */

///////////////////////////////////////////////////////////////////////////////

//filename:stty.h

#ifndef __STTY_H__
#define __STTY_H__

//包含头文件

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>
//

// 串口设备信息结构
typedef struct tty_info_t
{
        int fd; // 串口设备ID
        pthread_mutex_t mt; // 线程同步互斥对象
        char name[24]; // 串口设备名称，例："/dev/ttyS0"
        struct termios ntm; // 新的串口设备选项
        struct termios otm; // 旧的串口设备选项

} TTY_INFO;
//

// 串口操作函数

#ifdef __cplusplus    //__cplusplus是cpp中自定义的一个宏
extern "C" {          //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的
#endif

TTY_INFO *readyTTY(const char* name);
int setTTYSpeed(TTY_INFO *ptty, int speed);
int setTTYParity(TTY_INFO *ptty,int databits,int parity,int stopbits);
int cleanTTY(TTY_INFO *ptty);
int sendnTTY(TTY_INFO *ptty,char *pbuf,int size);
int recvnTTY(TTY_INFO *ptty,char *pbuf,int size);
int lockTTY(TTY_INFO *ptty);
int unlockTTY(TTY_INFO *ptty);

int init_hotplug_sock(void);


#ifdef __cplusplus
}
#endif

#endif // __STTY_H__
