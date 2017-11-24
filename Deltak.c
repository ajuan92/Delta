#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <pwm.h>

#define RAD_TO_DEG (180/3.14159265)
#define DELTA_LA 12 /*Constante Del Robot Base superior*/
#define DELTA_LB 3  /*Constante Del Robot Base inferior*/
#define DELTA_L1 15 /*Constantes del robot*/
#define DELTA_L2 30 /*Constantes del robot*/
#define COORD_LIMIT 3

void vfnDelta (int8_t bCoord_x, int8_t bCoord_y,int8_t bCoord_z );
int16_t vfnDelD (float abP[], float abA[]);

int main(int argc, char *argv[]) {

	if (argc == 4){
	vfnDelta(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]));
	}else{
	printf("Faltan argumentos");
	}
	
	return 0;
}

void vfnDelta (int8_t bCoord_x, int8_t bCoord_y,int8_t bCoord_z )
{


		int8_t bIndex;
		
		int Degv;
		int DC;

		char bT1Status = 0;
		char bT2Status = 0;
		char bT3Status = 0;

		float bOx1 = 0;
	        float bOy1 = 0;
		float bOz1 = DELTA_LA;

		float bOx2 = (0.86602*DELTA_LA);
		float bOy2 = 0;
		float bOz2 = -(DELTA_LA/2);

		float bOx3 = -(0.86602*DELTA_LA);
		float bOy3 = 0;
		float bOz3 = -(DELTA_LA/2);

		float abO1[] = {bOx1,bOy1,bOz1};
		float abO2[] = {bOx2,bOy2,bOz2};
		float abO3[] = {bOx3,bOy3,bOz3};
		
		
		printf("O1 = %f, %f, %f \r\n",abO1[0],abO1[1],abO1[2]);
		printf("O2 = %f, %f, %f \r\n",abO2[0],abO2[1],abO2[2]);
		printf("O3 = %f, %f, %f \r\n",abO3[0],abO3[1],abO3[2]);
		
		
		float abP[] = {bCoord_x,(bCoord_y - 12),bCoord_z};

		float bBx1 = 0;
		float bBy1 = 0;
		float bBz1 = DELTA_LB;

		float bBx2 = (0.86602*DELTA_LB);
		float bBy2 = 0;
		float bBz2 = -1.5;//-DELTA_LB/2;

		float bBx3 = -((0.86602*DELTA_LB));
		float bBy3 = 0;
		float bBz3 = -1.5;//-DELTA_LB/2;

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

		int16_t bT1 = vfnDelD(abP1, abO1);
		int16_t bT2 = vfnDelD(abP2, abO2);
		int16_t bT3 = vfnDelD(abP3, abO3);
		
		printf("P1 = %f, %f, %f \r\n",abP1[0],abP1[1],abP1[2]);
		printf("P2 = %f, %f, %f \r\n",abP2[0],abP2[1],abP2[2]);
		printf("P3 = %f, %f, %f \r\n",abP3[0],abP3[1],abP3[2]);
		
		for(Degv =-48, DC = 7;Degv <= 48 , DC <=39; DC+=2,Degv+=6){
	
			if((bT1 <= Degv)&& (bT1Status == 0)){
			   bT1=DC;
			   bT1Status = 1;		   
			}
			

			if((bT2 <= Degv)&& (bT2Status == 0)){
			   bT2=DC;
			   bT2Status = 1;		   
			}
			
			if((bT3 <= Degv)&& (bT3Status == 0)){
			   bT3=DC;
			   bT3Status = 1;		   
			}
		}		
	 printf("bT1 = %d bT2 = %d bT3 = %d \r\n",bT1,bT2,bT3);

	 vfnPwm(0,180,bT1,0);
	 vfnPwm(0,180,bT2,0);
	 vfnPwm(0,180,bT3,0);
				
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
	//printf("R = %f \r\n" ,bR);
	
	float C0 = (pow(DELTA_L2,2)-pow(DELTA_L1,2)-pow(bR,2))/(-2*(DELTA_L1)*(DELTA_L2));
//	printf("C0 = %f \r\n" ,C0);
	float C = (acos(C0))*(RAD_TO_DEG);
	//printf("C = %f \r\n" ,C);

	float y = sqrt((pow(abA[0],2))+(pow(abA[1],2))+(pow(abA[2],2)));
	float x = ((abA[0]*abp[0])+(abA[1]*abp[1])+(abA[2]*abp[2]));
	float w = bR*y;
	float S0 = (acos(x/w)*RAD_TO_DEG);
//	printf("S0 = %f \r\n" ,S0);
	float Teta = S0-C;
	int16_t wTeta = Teta*1000;
	
	printf("T = %f \r\n" ,Teta);
	
	return wTeta;
}

