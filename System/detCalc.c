#include "detCalc.h"

void pidCalc(PIDgroup* G,int16_t* output)
{
  int16_t Outpid,SetPoint,NextPoint;                                        //当前误差
	int16_t pid[6];
	int8_t cur;
	
	SetPoint=G->current;
	NextPoint=G->target;
	int16_t LastError=G->LastError;
	int16_t PrevError=G->PrevError;
	int16_t IntegralError=G->IntegralError;
	
  int16_t iError=NextPoint-SetPoint;                                //增量计算
	if(G->circleCalc&&iError>180)iError-=360;
	if(G->circleCalc&&iError<-180)iError+=360;
	*output=G->P*iError+G->I*LastError+G->D*(PrevError-LastError);
	
  G->PrevError=LastError;                                      //存储误差，用于下次计算
  G->LastError=iError;
  G->IntegralError+=iError;

}
void DrivePidCalc(PIDgroup* RG,PIDgroup* LG,int16_t* RS,int16_t* LS){
	int16_t tmp;
	pidCalc(RG,&tmp);
	*RS=BASIC_SPEEDR+tmp;
	pidCalc(LG,&tmp);
	*LS=BASIC_SPEEDL+tmp;
	
}