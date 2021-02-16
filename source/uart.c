#include <termios.h>      /*终端参数*/
#include <string.h>
#include "my_log.h"
#include "my_types.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define 	COMM_NOPARITY		0//校验
#define		COMM_ODDPARITY		1
#define 	COMM_EVENPARITY		2

#define     COMM_ONESTOPBIT		0
#define     COMM_TWOSTOPBITS	1

#define CTX_MAGIC  0xEF
#define CTX_ON   (_IO(CTX_MAGIC, 0x11))
#define CTX_OFF  (_IO(CTX_MAGIC, 0x12))

typedef struct com_attr       /*我自己定义的串口属性结构*/
{
    unsigned int baudrate;    /*波特率*/
    unsigned char databits;    /*数据位*/
    unsigned char stopbits;    /*停止位*/
    unsigned char parity;    /*校验位*/
}com_attr;

int set_com_attr(int fd, com_attr *attr)
{
    struct termios opt;
    memset(&opt, 0, sizeof(struct termios));
    tcgetattr(fd, &opt);
    cfmakeraw(&opt);

    /*******************波特率********************/
    MY_LOG("set baudrate %d\n", attr->baudrate);
    switch (attr->baudrate)
    {
        case 50:
            cfsetispeed(&opt, B50);
            cfsetospeed(&opt, B50);
            break;
        case 75:
            cfsetispeed(&opt, B75);
            cfsetospeed(&opt, B75);
            break;
        case 110:
            cfsetispeed(&opt, B110);
            cfsetospeed(&opt, B110);
            break;
        case 134:
            cfsetispeed(&opt, B134);
            cfsetospeed(&opt, B134);
            break;
        case 150:
            cfsetispeed(&opt, B150);
            cfsetospeed(&opt, B150);
            break;
        case 200:
            cfsetispeed(&opt, B200);
            cfsetospeed(&opt, B200);
            break;
        case 300:
            cfsetispeed(&opt, B300);
            cfsetospeed(&opt, B300);
            break;
        case 600:
            cfsetispeed(&opt, B600);
            cfsetospeed(&opt, B600);
            break;
        case 1200:
            cfsetispeed(&opt, B1200);
            cfsetospeed(&opt, B1200);
            break;
        case 1800:
            cfsetispeed(&opt, B1800);
            cfsetospeed(&opt, B1800);
            break;
        case 2400:
            cfsetispeed(&opt, B2400);
            cfsetospeed(&opt, B2400);
            break;
        case 4800:
            cfsetispeed(&opt, B4800);
            cfsetospeed(&opt, B4800);
            break;
        case 9600:
            cfsetispeed(&opt, B9600);
            cfsetospeed(&opt, B9600);
            break;
        case 19200:
            cfsetispeed(&opt, B19200);
            cfsetospeed(&opt, B19200);
            break;
        case 38400:
            cfsetispeed(&opt, B38400);
            cfsetospeed(&opt, B38400);
            break;
        case 57600:
            cfsetispeed(&opt, B57600);
            cfsetospeed(&opt, B57600);
            break;
        case 115200:
            cfsetispeed(&opt, B115200);
            cfsetospeed(&opt, B115200);
            break;
        case 230400:
            cfsetispeed(&opt, B230400);
            cfsetospeed(&opt, B230400);
            break;
        case 460800:
            cfsetispeed(&opt, B460800);
            cfsetospeed(&opt, B460800);
            break;
        case 500000:
            cfsetispeed(&opt, B500000);
            cfsetospeed(&opt, B500000);
            break;
        case 576000:
            cfsetispeed(&opt, B576000);
            cfsetospeed(&opt, B576000);
            break;
        case 921600:
            cfsetispeed(&opt, B921600);
            cfsetospeed(&opt, B921600);
            break;
        case 1000000:
            cfsetispeed(&opt, B1000000);
            cfsetospeed(&opt, B1000000);
            break;
        case 1152000:
            cfsetispeed(&opt, B1152000);
            cfsetospeed(&opt, B1152000);
            break;
        case 1500000:
            cfsetispeed(&opt, B1500000);
            cfsetospeed(&opt, B1500000);
            break;
        case 2000000:
            cfsetispeed(&opt, B2000000);
            cfsetospeed(&opt, B2000000);
            break;
        case 2500000:
            cfsetispeed(&opt, B2500000);
            cfsetospeed(&opt, B2500000);
            break;
        case 3000000:
            cfsetispeed(&opt, B3000000);
            cfsetospeed(&opt, B3000000);
            break;
        case 3500000:
            cfsetispeed(&opt, B3500000);
            cfsetospeed(&opt, B3500000);
            break;
        case 4000000:
            cfsetispeed(&opt, B4000000);
            cfsetospeed(&opt, B4000000);
            break;
        default:
            MY_LOG("unsupported baudrate %d\n", attr->baudrate);
            return MY_FALSE;
            break;
    }

    /************************校验位************************/
    switch (attr->parity)
    {
        case COMM_NOPARITY://校验
            opt.c_cflag &= ~PARENB;
            opt.c_iflag &= ~INPCK;
            break;
        case COMM_ODDPARITY:
            opt.c_cflag |= PARENB;
            opt.c_cflag |= PARODD;
            opt.c_iflag |= INPCK;
            break;
        case COMM_EVENPARITY:
            opt.c_cflag |= PARENB;
            opt.c_cflag &= ~PARODD;
            opt.c_iflag |= INPCK;
            break;
        default:
            MY_LOG("unsupported parity %d\n", attr->parity);
            return MY_FALSE;
            break;
    }
            opt.c_cflag &= ~CSIZE;       /*无论设置多少校验位都需要的*/

    /*******************数据位*****************/
    switch (attr->databits)
    {
        case 5:
            opt.c_cflag |= CS5;
            break;
        case 6:
            opt.c_cflag |= CS6;
            break;
        case 7:
            opt.c_cflag |= CS7;
            break;
        case 8:
            opt.c_cflag |= CS8;
            break;
        default:
            MY_LOG("unsupported data bits %d\n", attr->databits);
            return MY_FALSE;
            break;
    }
    opt.c_cflag &= ~CSTOPB;

    /*******************停止位***************/
    switch (attr->stopbits)
    {
        case COMM_ONESTOPBIT:
            opt.c_cflag &= ~CSTOPB;
            break;

        case COMM_TWOSTOPBITS:
            opt.c_cflag |= CSTOPB;
            break;
        default:
            MY_LOG("unsupported stop bits %d\n", attr->stopbits);
            return MY_FALSE;
            break;
    }


    /*等待时间,阻塞模式下设置的*/
    //opt.c_cc[VTIME]   = 0;    /*设置超时时间*/
    //opt.c_cc[VMIN]    = 1;

    opt.c_iflag &= ~(ICRNL | INLCR);
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);/*关闭软件流控(一般都是关闭软硬流控，我也不知道为啥)*/


    tcflush(fd, TCIOFLUSH);        //刷清缓冲区
    if (tcsetattr(fd, TCSANOW, &opt) < 0)
    {
        MY_LOG("tcsetattr faild\n");
        return MY_FALSE;
    }
    return MY_TRUE;
}

int uartWrite(unsigned char *pdata,unsigned char lenth)
{
/*	u8 i=0;
	for(i=0;i<lenth;i++){
		MY_LOG("%02x ",pdata[i]);
	}
	pdata[pdata[1]+2] = 0;
	for(i=1;i<=pdata[1]+1;i++){//bo 01 01 02
		pdata[pdata[1]+2] += pdata[i];
	}
	ioctl(fioctl,CTX_ON);
	MY_LOG("write %x:   %d\n",pdata[0],lenth);
	write(fuart,pdata,lenth);
	usleep(12500);
	ioctl(fioctl,CTX_OFF);
	return 0;
	*/
}

