#include <linux/ioctl.h>

#define MAGIC_LED 'H'
#define LED_ON 	_IO(MAGIC_LED,0)
#define LED_OFF _IO(MAGIC_LED,1)
