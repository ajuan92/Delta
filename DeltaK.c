#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "pwm.h"
#include "DeltaK.h"


#define SIN_120 0.866025403
#define RAD_TO_DEG (180/3.14159265)
#define DELTA_LA 5 /*Constante Del Robot Base superior*/
#define DELTA_LB 2  /*Constante Del Robot Base inferior*/
#define DELTA_L1 12 /*Constantes del robot*/
#define DELTA_L2 22 /*Constantes del robot*/
#define COORD_LIMIT 3

void vfnDelta (int8_t bCoord_x, int8_t bCoord_y,int8_t bCoord_z, int8_t enable);
int16_t vfnDelD (float abP[], float abA[]);
/*
int main(int argc, char *argv[]) {

	if (argc == 5){
	vfnDelta(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
	}else{
	printf("Faltan argumentos\r\n");
	}
	
	return 0;
}
*/
void vfnDelta (int8_t bCoord_x, int8_t bCoord_y,int8_t bCoord_z, int8_t enable )
{


		int8_t bIndex;
		
		int Degv;
		int DC;

		int8_t bT1Status = 0;
		int8_t bT2Status = 0;
		int8_t bT3Status = 0;

		float bOx1 = 0;
	        float bOy1 = 0;
		float bOz1 = DELTA_LA;

		float bOx2 = (SIN_120*DELTA_LA);
		float bOy2 = 0;
		float bOz2 = -(DELTA_LA/2);

		float bOx3 = -(SIN_120*DELTA_LA);
		float bOy3 = 0;
		float bOz3 = -(DELTA_LA/2);

		float abO1[] = {bOx1,bOy1,bOz1};
		float abO2[] = {bOx2,bOy2,bOz2};
		float abO3[] = {bOx3,bOy3,bOz3};
		
/*		
		printf("O1 = %f, %f, %f \r\n",abO1[0],abO1[1],abO1[2]);
		printf("O2 = %f, %f, %f \r\n",abO2[0],abO2[1],abO2[2]);
		printf("O3 = %f, %f, %f \r\n",abO3[0],abO3[1],abO3[2]);
		
*/		
		float abP[] = {bCoord_x,(bCoord_y - 15),bCoord_z};

		float bBx1 = 0;
		float bBy1 = 0;
		float bBz1 = DELTA_LB;

		float bBx2 = (SIN_120*DELTA_LB);
		float bBy2 = 0;
		float bBz2 = -DELTA_LB/2;

		float bBx3 = -((SIN_120*DELTA_LB));
		float bBy3 = 0;
		float bBz3 = -DELTA_LB/2;

		float abB1[] = {bBx1,bBy1,bBz1};
		float abB2[] = {bBx2,bBy2,bBz2};
		float abB3[] = {bBx3,bBy3,bBz3};

		float abP1[COORD_LIMIT];
		float abP2[COORD_LIMIT];
		float abP3[COORD_LIMIT];
		
		

		for(bIndex = 0; bIndex < COORD_LIMIT; bIndex++)
		{
			abP1[bIndex] = abB1[bIndex] + abP[bIndex];
			abP2[bIndex] = abB2[bIndex] + abP[bIndex];
			abP3[bIndex] = abB3[bIndex] + abP[bIndex];
		}

		int16_t bT1 = (int16_t)vfnDelD(abP1, abO1);
		int16_t bT2 = (int16_t)vfnDelD(abP2, abO2);
		int16_t bT3 = (int16_t)vfnDelD(abP3, abO3);
/*		
		printf("P1 = %f, %f, %f \r\n",abP1[0],abP1[1],abP1[2]);
		printf("P2 = %f, %f, %f \r\n",abP2[0],abP2[1],abP2[2]);
		printf("P3 = %f, %f, %f \r\n",abP3[0],abP3[1],abP3[2]);
*/		
		for(Degv =-48, DC = 7;Degv <= 48; DC+=2,Degv+=6){
	
			
			if((bT1 <= Degv)&&(bT1Status == 0)){
			   if(Degv <= 39){
			      bT1=DC;
			   }else{
			      bT1=39;
			   }
				
			   bT1Status = 1;
			}
			
			if((bT2 <= Degv)&&(bT2Status == 0)){
			   if(Degv <= 39){
                              bT2=DC;
                           }else{
                              bT2=39;
                           }

                           bT2Status = 1;
			}
			
			if((bT3 <= Degv)&&(bT3Status == 0)){
           		   if(Degv <= 39){
                              bT3=DC;
                           }else{
                              bT3=39;
                           }

                           bT3Status = 1;

			}					
			
		}		
	 printf("bT1 = %d bT2 = %d bT3 = %d \r\n",bT1,bT2,bT3);

	 vfnPwm(0,180,bT1,enable);
	 vfnPwm(1,180,bT2,enable);
	 vfnPwm(2,180,bT3,enable);
				
	 bT1Status = 0;
	 bT2Status = 0;
	 bT3Status = 0;			

	
}

int16_t vfnDelD (float abP[], float abA[])
{
	uint8_t bIndex;
	
	float abp[COORD_LIMIT];
	
	for(bIndex = 0;bIndex < COORD_LIMIT; bIndex++)
	abp[bIndex] = abP[bIndex]-abA[bIndex];

    float bR = sqrt((pow(abp[0],2))+(pow(abp[1],2))+(pow(abp[2],2)));
	
	float C0 = (pow(DELTA_L2,2)-pow(DELTA_L1,2)-pow(bR,2))/(-2*(DELTA_L1)*(DELTA_L2));
	float C = (acos(C0))*(RAD_TO_DEG);

	float y = sqrt((pow(abA[0],2))+(pow(abA[1],2))+(pow(abA[2],2)));
	float x = ((abA[0]*abp[0])+(abA[1]*abp[1])+(abA[2]*abp[2]));
	float w = bR*y;
	float S0 = (acos(x/w)*RAD_TO_DEG);
	float Teta = S0-C;
	int16_t wTeta = Teta;
/*	
	printf("T = %f \r\n" ,Teta);
*/	
	return wTeta;
}

