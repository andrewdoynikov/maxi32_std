//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
#ifndef ENCODER_H
#define ENCODER_H
//************************************************************************************************
#include <avr/io.h>
//************************************************************************************************
#define ENCODER_PIN_A    	 PD0 
#define ENCODER_PIN_B    	 PD3 
#define ENCODER_PIN_PORT 	 PIND 
#define ENCODER_PORT 	     PORTD 
#define ENCODER_DDR_PORT 	 DDRD 
#define ENCODER_SCAN_TIME 	 2		// 2 ms 
//************************************************************************************************
//это для наглядности кода
#define ENC_MASK       3
#define ENC_RIGHT_MASK 210
#define ENC_LEFT_MASK  225
#define ENC_RIGHT KBD_RIGHT 
#define ENC_LEFT  KBD_LEFT 
//************************************************************************************************
void encoder_init(void);
void scan_encoder(void);
//************************************************************************************************
void encoder_init(void)
{
ClearBit(ENCODER_DDR_PORT, ENCODER_PIN_A); //вход
ClearBit(ENCODER_DDR_PORT, ENCODER_PIN_B); //вход
SetBit(ENCODER_PORT, ENCODER_PIN_A);	   //вкл подтягивающий резистор
SetBit(ENCODER_PORT, ENCODER_PIN_B);	   //вкл подтягивающий резистор
}
//************************************************************************************************
void scan_encoder(void)
{
static unsigned char stateEnc; 	//хранит последовательность состояний энкодера
unsigned char tmp;  
unsigned char currentState = 0;
if (kbd_key == KBD_NOKEY)
{
  //проверяем состояние выводов микроконтроллера
  if ((ENCODER_PIN_PORT & (1 << ENCODER_PIN_A)) != 0) { SetBit(currentState,0); }
  if ((ENCODER_PIN_PORT & (1 << ENCODER_PIN_B)) != 0) { SetBit(currentState,1); }
  //если равно предыдущему, то выходим
  tmp = stateEnc;
  if (currentState == (tmp & ENC_MASK)) 
  {
    return;
  }
  //если не равно, то сдвигаем и сохраняем в озу
  tmp = (tmp << 2) | currentState;
  stateEnc = tmp;
  //сравниваем получившуюся последовательность
  if (tmp == ENC_LEFT_MASK) { kbd_key = ENC_LEFT; }
  if (tmp == ENC_RIGHT_MASK) { kbd_key = ENC_RIGHT; }	 
  if (kbd_key != KBD_NOKEY)
  {
    kbd_nokey_flag = 0;
    kbd_nokey_cnt = 0;
  }
}
}
//************************************************************************************************
#endif
