#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include<stdlib.h>

#define PERIOD "/sys/class/pwm/pwmchipX/pwm0/period"
#define DUTY_CYCLE "/sys/class/pwm/pwmchipX/pwm0/duty_cycle"
#define ENABLE "/sys/class/pwm/pwmchipX/pwm0/enable"
#define ASCII_CONV(x) x+48



void vfnPwm(char PwmNum,int period, int dutyCycle, int enable);

int main (int ArgC, char *ArgV[]){

    if(ArgC != 5)
    {
        printf("error: missing Args\n\r");
        exit(0);
    }

 vfnPwm(atoi(ArgV[1]),atoi(ArgV[2]),atoi(ArgV[3]),atoi(ArgV[4]));

return 0;
}

void vfnPwm(char PwmNum, int period, int dutyCycle, int enable){

int File;
int WriteS;
char bWriteStr[15];
char bPathIndex = 0;

char baPathPeriod[] = PERIOD;
char baPathDutyCycle[] = DUTY_CYCLE;
char baPathEnable[] = ENABLE;

     while(baPathDutyCycle[bPathIndex] != '\0' ){
	
     	if(baPathDutyCycle[bPathIndex]  == 'X'){

 		 baPathPeriod[bPathIndex] = ASCII_CONV(PwmNum);
	 	 baPathDutyCycle[bPathIndex] = ASCII_CONV(PwmNum);
		 baPathEnable[bPathIndex] = ASCII_CONV(PwmNum);

	}

	bPathIndex++;

     }  


/*Set period*/

    File = open(baPathPeriod, O_TRUNC | O_WRONLY);
    if(File == -1)
    {  
        printf("error: file doesn't exist\n\r");
	printf("Remember to export the pwm module\n\r");
	printf("use the follow command from the root");
	printf(" echo 0 > /sys/class/pwm/pwmchipX/export\n\r");

        exit(0);
    }
   
   memset(bWriteStr,'\0',15);
   sprintf(bWriteStr, "%d", period);
   WriteS = write(File, bWriteStr, sizeof(bWriteStr));
 
    if(WriteS  == -1)
    {  
        printf("error: Imposible to write\n\r");
        exit(0);
    }

    close(File);

/*Set Duty cycle*/

    File = open(baPathDutyCycle, O_TRUNC | O_WRONLY);
    if(File == -1)
    {  
        printf("error: file doesn't exist\n\r");
        exit(0);
    }
   
   memset(bWriteStr,'\0',15);
   sprintf(bWriteStr, "%d", dutyCycle);
   WriteS = write(File, bWriteStr, sizeof(bWriteStr));
 
    if(WriteS  == -1)
    {  
        printf("error: Imposible to write\n\r");
        exit(0);
    }

    close(File);

/*Set enable*/
    File = open(baPathEnable, O_TRUNC | O_WRONLY);
    if(File == -1)
    {  
        printf("error: file doesn't exist\n\r");
        exit(0);
    }
   
   memset(bWriteStr,'\0',15);
   sprintf(bWriteStr, "%d", enable);
   WriteS = write(File, bWriteStr, sizeof(bWriteStr));
 
    if(WriteS  == -1)
    {  
        printf("error: Imposible to write\n\r");
        exit(0);
    }

    close(File);
}
