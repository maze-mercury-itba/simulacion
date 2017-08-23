#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#define F_MAX_AMOUNT_OF_DATA 100
#define F_OK 1

//errors
#define F_ERROR_WAYD	-2   //WAYD for What Are You Doing
#define F_ERROR_CANTOPEN	-1
//end of errors

int16_t F_Startup (char * filename);

double F_getSensorType (uint16_t sensornumber);

double F_getSensorXPos (uint16_t sensornumber);

double F_getSensorYPos (uint16_t sensornumber);

double F_getSensorAngle (uint16_t sensornumber);

uint16_t F_getBasicInfo (uint16_t order);

#endif

/*
This code is designe for speed performance and doesn't care about memory at all
please be aware of adjust the MAX_AMOUNT_OF_DATA to the limit! to get better memory performance
some use of dynamic memory may came in future updates


--HOW TO USE:

1st: call F_Startup (filename);
		filename should have the path of the .txt file

2nd and last: get the info you need!
	this is fast, you can use it without needing to save it in local variables!!

*/