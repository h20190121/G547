#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>

#define MAJOR_NUM 100


#define ioctl_selectchannel _IOW(MAJOR_NUM, 0,int)

#define ioctl_selectallignment _IOW(MAJOR_NUM, 1, char*)

#define DEVICE_FILE_NAME "/dev/adc8_dev"

#endif
