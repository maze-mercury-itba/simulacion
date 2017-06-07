#include <stdio.h>
#include <stdint.h>
#include "FileHandler.h"


int main (void)
{
	F_Startup("holapaola.txt");
	printf("%u\n",F_getBasicInfo(1) );

}