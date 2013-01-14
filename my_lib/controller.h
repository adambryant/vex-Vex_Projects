// controller.h

#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_

#include "../base_lib/vex.h"

typedef struct
{
	char ch1;
	char ch2;
	char ch3;
	char ch4;
	char ch5;
	char ch6;
} Controller;

void registerController( Controller *cont );
void readController();

#endif
