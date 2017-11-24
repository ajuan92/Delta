#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <pwm.h>

#define PERIOD "/sys/class/pwm/pwmchipX/pwm0/period"
#define DUTY_CYCLE "/sys/class/pwm/pwmchipX/pwm0/duty_cycle"
#define ENABLE "/sys/class/pwm/pwmchipX/pwm0/enable"
#define ASCII_CONV(x) x+48

#define DC_VAL(x,y) x*y/100
#define PERIOD_VAL(x) 1000000000/x
#define NANO_2_MILI(x) x/1000000
#define MILI_2_NANO(x) x*1000000
#define DEGREE(x,y) 100*(1+x/120)/NANO_2_MILI(y)


void vfnPwm(char PwmNum,const int period, const int dutyCycle, int enable);
/*
int main (int ArgC, char *ArgV[]){

    if(ArgC != 5)
    {
        printf("error: missing Args\n\r");
        exit(0);
    }

 vfnPwm(atoi(ArgV[1]),atoi(ArgV[2]),atoi(ArgV[3]),atoi(ArgV[4]));

return 0;
}
*/
void vfnPwm(char PwmNum, const int period, const int dutyCycle, int enable){

int File;
int WriteS;
char bWriteStr[15];
char bPathIndex = 0;
int DutyC;
int PeriodVal;

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
   
   PeriodVal = 1000000000/period;
   printf("period = %d\r\n",PeriodVal);
  
   memset(bWriteStr,'\0',15);
   sprintf(bWriteStr, "%d", PeriodVal);
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
  
 //   DutyC = dutyCycle*10000;
   DutyC = (dutyCycle*PeriodVal)/100;   
// DutyC = (1000*(100000*(1000+(dutyCycle*1000))/120000))/20;
   printf("dc = %d \r\n",DutyC);

   memset(bWriteStr,'\0',15);
   sprintf(bWriteStr, "%d",(int)DutyC);
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
    if(enable){
    	printf("PWM%d On\n\r",PwmNum);
    }else{
       	printf("PWM%d Off\n\r",PwmNum);
    }
    close(File);
}
