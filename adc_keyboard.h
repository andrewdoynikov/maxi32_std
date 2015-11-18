//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
#ifndef ADC_KBD_H
#define ADC_KBD_H
//************************************************************************************************
#define start_conv_adc() ADCSRA |= (1<<ADSC) 
#define val_in_param(val, val_min, val_max)  (((val >= val_min) && ((val <= val_max))) == 1)
//************************************************************************************************
// Defines ADC
#define ADC_VREF_TYPE_INT  0xC0   // внутренний
#define ADC_VREF_TYPE_AVCC 0x40   // AVCC
#define ADC_VREF_TYPE_AREF 0x00   // AREF
//************************************************************************************************
volatile unsigned long extkey_volt;
volatile unsigned adc_state = 0;
#define MENU_SETEXTKEY_MAX	9
unsigned long extkey_volts[] = { 211,339,432,502,558,603,640,671,697,23 };
unsigned char extkey_kbd[] = { KBD_1,KBD_2,KBD_3,KBD_4,KBD_5,KBD_6,KBD_7,KBD_8,KBD_9,KBD_BANK };
//************************************************************************************************
void adc_init(void);
void scan_adckbd(void);
unsigned char get_extkey_kbd(unsigned long key);
//************************************************************************************************
void adc_init(void)
{
ADMUX = (0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0);
ADCSRA = (1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
//************************************************************************************************
void scan_adckbd(void)
{
if (kbd_key == KBD_NOKEY)
{
  switch (adc_state) // switch
  {
  case 0: // Запуск преобразования
  start_conv_adc();
  adc_state = 2;
  break;
  case 1:
  if (kbd_key != KBD_NOKEY)
  {
    kbd_nokey_flag = 0;
    kbd_nokey_cnt = 0;
  }
  adc_state = 0;
  break;
  }
}
}
//************************************************************************************************
ISR(SIG_ADC)
{
if (adc_state == 2)
{
  extkey_volt = ADCW;
  if (l_no_ext)
  {
    if (extkey_volt < ext_key_no - 5)
    {
      ext_code = extkey_volt;
	  beep();
      kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
    }
  } else {
    kbd_key = get_extkey_kbd(extkey_volt);
  }
  adc_state = 1;
}
}
//************************************************************************************************
unsigned char get_extkey_kbd(unsigned long key)
{
unsigned char ret = KBD_NOKEY, i;
unsigned long v;
for(i = 0; i < MENU_SETEXTKEY_MAX + 1; i++)
{
  v = extkey_volts[i];
  if (val_in_param(key, v - 10, v + 10))
  { 
    ret = extkey_kbd[i];
	break;
  }
}
return ret;
}
//************************************************************************************************

#endif
