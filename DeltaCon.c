#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "DeltaK.h"
#include "DeltaCon.h"


#define COM_HOME 0x01 
#define COM_STOP 0x02
#define COM_COORD 0x04


void DeltaControl(char DeltaFrame[],char *ConReplay){

     vfnDelta(DeltaFrame[1],DeltaFrame[2],DeltaFrame[3],DeltaFrame[4]);
     
     switch(DeltaFrame[0]){
	
	case COM_HOME:
		strcpy(ConReplay, "Point reached effector in home\r\n");
	     break;	

	case COM_STOP:
		strcpy(ConReplay, "PWM modules are off\r\n");
             break;

	case COM_COORD:
		strcpy(ConReplay, "Point reached\r\n");
	     break;

	default :
             break;
     }
	
}
