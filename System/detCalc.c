#include "detCalc.h"



void PIDgroupInit(){
	//巡线PID
	GreyLeft.target=0;
	GreyRight.target=0;
	GreyLeft.P=-16;
	GreyLeft.I=0;
	GreyLeft.D=0;
	
	GreyRight.P=16;
	GreyRight.I=0;
	GreyRight.D=0;
	
	GreyLeft.IntegralError=0;
	GreyLeft.LastError=0;
	GreyLeft.PrevError=0;
	GreyLeft.current=0;
	
	GreyRight.IntegralError=0;
	GreyRight.LastError=0;
	GreyRight.PrevError=0;
	GreyRight.current=0;
	
	//角度PID
	AngleLeft.circleCalc=1;
	AngleRight.circleCalc=1;
	
	AngleLeft.P=-0.1;
	AngleLeft.I=0;
	AngleLeft.D=0;
	
	AngleRight.P=0.1;
	AngleRight.I=0;
	AngleRight.D=0;
	
	AngleLeft.IntegralError=0;
	AngleLeft.LastError=0;
	AngleLeft.PrevError=0;
	AngleLeft.current=0;
	
	AngleRight.IntegralError=0;
	AngleRight.LastError=0;
	AngleRight.PrevError=0;
	AngleRight.current=0;
	
}

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
	
  int16_t iError=NextPoint-SetPoint;   	//增量计算
	if(G->circleCalc)CirCal(&iError);
	*output=G->P*iError+G->I*LastError+G->D*(PrevError-LastError);
	
  G->PrevError=LastError;                                      //存储误差，用于下次计算
  G->LastError=iError;
  G->IntegralError+=iError;

}

void CirCal(int16_t *Ang){
	if(*Ang>180)*Ang-=360;
	if(*Ang<-180)*Ang+=360;
}
void CirCalF(float *Ang){
	if(*Ang>180)*Ang-=360;
	if(*Ang<-180)*Ang+=360;
}

void DrivePidCalc(PIDgroup* RG,PIDgroup* LG,int16_t* RS,int16_t* LS){
	int16_t tmp;
	pidCalc(RG,&tmp);
	*RS=BASIC_SPEEDR+tmp;
	pidCalc(LG,&tmp);
	*LS=BASIC_SPEEDL+tmp;
	
}
 