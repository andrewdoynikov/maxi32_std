//************************************************************************************************
// Project MAXI32 TIMER SERVICE  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
#ifndef TIMER_SERVICE_H
#define TIMER_SERVICE_H
//************************************************************************************************
#include <avr/interrupt.h>
//************************************************************************************************
// datatype definitions macros
typedef unsigned char  u08;
typedef   signed char  s08;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned long  u32;
typedef   signed long  s32;
//************************************************************************************************
#define TIMER_ISR	TIMER2_COMP_vect
//************************************************************************************************
#define Prescaler	  		64
#define	TimerDivider  		(F_CPU / Prescaler / 1000)		// 1 mS
#define HI(x) ((x) >> 8)
#define LO(x) ((x) & 0xFF)
//************************************************************************************************
//���������� ��������
#define MAX_TIMERS  8
//��������� ������� - ������������, ��������, ������������
enum StateTimer {IDLE, ACTIVE, DONE};
//��������� ������������ �������
typedef struct {
  unsigned int time;        //����� ������������
  unsigned int period;      //������ ����������
  enum StateTimer state;    //������� ��������� 
  void (*pFunc)(void);      //��������� �� �������
} SoftTimer;
//************************************************************************************************
extern void CreateTimer(SoftTimer *CurSoftTimer, unsigned int time, unsigned int period, enum StateTimer state, void (*pFunc)(void));
extern void CheckTimer(void);
extern void SetTimerParam(SoftTimer *CurSoftTimer, unsigned int time, unsigned int period, enum StateTimer state);
extern void RunTimerService(void);
//************************************************************************************************
#endif

