//************************************************************************************************
// Project MAXI32 TIMER SERVICE  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
#include <avr/io.h>
#include "timer_service.h"
//************************************************************************************************
unsigned char AmountTimers = 0;
SoftTimer* SoftTimers[MAX_TIMERS];
//************************************************************************************************
// ������ ���������� �������
inline void RunTimerService(void)
{
TCCR2 = 1<<WGM21|4<<CS20; 				// Freq = CK/64 - ���������� ����� � ������������
								        // ��������� ����� ���������� �������� ���������
TCNT2 = 0;								// ���������� ��������� �������� ���������
OCR2  = LO(TimerDivider); 				// ���������� �������� � ������� ���������
TIMSK = 0<<TOIE0|1<<OCF2;		        // ��������� ���������� RTOS - ������ ��
sei();
}
//************************************************************************************************
ISR(TIMER_ISR)
{
CheckTimer();
}
//************************************************************************************************
void CreateTimer(SoftTimer *CurSoftTimer, unsigned int time, unsigned int period , enum StateTimer state, void (*pFunc)(void)){
SoftTimers[AmountTimers] = CurSoftTimer;
CurSoftTimer->time = time;
CurSoftTimer->period = period;
CurSoftTimer->state = state;
CurSoftTimer->pFunc = pFunc;
AmountTimers++;
}
//************************************************************************************************
void CheckTimer(void)
{
for(unsigned char i = 0; i < AmountTimers; i++)
{ 
  if (SoftTimers[i]->state == ACTIVE)
  {
    if (SoftTimers[i]->time == 0)
	{
      SoftTimers[i]->pFunc();
      if (SoftTimers[i]->period != 0) SoftTimers[i]->time = (SoftTimers[i]->period-1); else SoftTimers[i]->state = DONE;
    } else (SoftTimers[i]->time)--;
  }
}
}
//************************************************************************************************
void SetTimerParam(SoftTimer *CurSoftTimer, unsigned int time, unsigned int period, enum StateTimer state)
{
CurSoftTimer->time = time;
CurSoftTimer->period = period;
CurSoftTimer->state = state;
}
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
