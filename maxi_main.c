//************************************************************************************************
// Project MAXI32 TIMER SERVICE  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "intsoft.h"
#include "bits_macros.h"
#include "timer_service.h"
#include "i2c.h"
#include "rc5.h"
#include "lcd_lib.h"
#include "onewire.h"
#include "ds18x20.h"
#include "maxi32.h"
//************************************************************************************************
#if (LNG_TYPE == LNG_ENG)
#include "english.h"
#endif
#if (LNG_TYPE == LNG_RUS)
#include "english.h"
//#include "russian.h"
#endif
//************************************************************************************************
#include "lm7001.h"
#include "tda7313.h"
#include "ds1307.h"
#include "keyboard.h"
#include "encoder.h"
#include "adc_keyboard.h"
#include "functions.h"
#include "main_menu.h"
#include "option_menu.h"
#include "set_fm_station.h"
#include "set_rc5_key.h"
#include "set_ext_key.h"
#include "set_alarm.h"
#include "eeprom.h"
//************************************************************************************************
void init_port(void)
{
PORTA = 0xC0; DDRA = 0x3F;
PORTB = 0xF3; DDRB = 0x0F;
PORTD = 0xFD; DDRD = 0x82;
PORTC = 0x0F; DDRC = 0xF3;  
// Timer0 initialization
TCCR0 = 0x00; TCNT0 = 0x00; OCR0 = 0x00;
// Timer1 initialization
TCCR1A = 0x00; TCCR1B = 0x00; TCNT1H = 0x00; TCNT1L = 0x00;
ICR1H = 0x00; ICR1L = 0x00;
OCR1AH = 0x00; OCR1AL = 0x00; OCR1BH = 0x00; OCR1BL = 0x00;
// Timer2 initialization
ASSR = 0x00; TCCR2 = 0x00; TCNT2 = 0x00; OCR2 = 0x00;
GICR |= 0x40; MCUCR = 0x02; MCUCSR = 0x00; GIFR = 0x40; // INT0 Filing
// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;
// Analog Comparator initialization
ACSR=0x80; SFIOR=0x00;
}
//************************************************************************************************
void init_main(void)
{
lcd_load_defchar();  // Знакогенератор по дефолту
send_tda7313_mute(); // Выключаем звук
led_standby_on(); 	 // Светодиод STANDBY включен
ac_wait_off();		 // Колонки выключены
standby_out_off();	 // Усилок выключен
ds_temp_off();		 // Вентилятор обдува выключен
// Выводим логотип
#if (LCD_TYPE == LCD20x4)
lcd_com(LCD_ROW1 + 2); lcd_puts(logo[LOGO_1]);
lcd_com(LCD_ROW2 + 2); lcd_puts(logo[LOGO_2]);
lcd_com(LCD_ROW4 + 2); lcd_puts(logo[LOGO_3]);
ds_count = search_ow_devices();
lcd_com(LCD_ROW4 + 11); print_dec(ds_count,2,'0');
#endif
#if (LCD_TYPE == LCD16x4)
lcd_com(LCD_ROW1); lcd_puts(logo[LOGO_1]);
lcd_com(LCD_ROW2); lcd_puts(logo[LOGO_2]);
lcd_com(LCD_ROW4); lcd_puts(logo[LOGO_3]);
ds_count = search_ow_devices();
lcd_com(LCD_ROW4 + 9); print_dec(ds_count,2,'0');
#endif
}
//************************************************************************************************
void init_hardware(void)
{
// I2C init
i2c_init();
// Lcd init
lcd_init();
// RTC init
rtc_init();
// Encoder init
encoder_init();
// ADC init
adc_init();
// Rc5 init
rc5_init(rc5_system);
lcd_clear();
}
//************************************************************************************************
void scan_nokey(void)
{
if (l_alarm > 0)
{
  l_alarm--; 
}
if ((light_cnt < kbd_nokey) && (!light_flag))
{
  light_cnt++;
} else {
  light_flag = 1;
}
if (ac_wait_cnt > 0)
{
  ac_wait_cnt--; 
}
if ((!l_standby) && (!l_mute))
{
  if (kbd_nokey_cnt < kbd_nokey)
  {
    kbd_nokey_cnt++;
    kbd_nokey_flag = 0;
  } else {
    kbd_nokey_flag = 1;
  }
}
}
//************************************************************************************************
void scan_ds18x20(void)
{
if (ds_cnt > 0)
{
  ds_cnt--;
} else {  
  switch (ds_state) // switch
  {
  case 0: // Поиск датчиков
    ds_count = search_ow_devices();
    if (ds_count == 0) // switch
    {
 	  temperature1 = NO_DS_FOPUND_TEMP; temperature2 = NO_DS_FOPUND_TEMP; 
      ds_state = 0;
	  ds_temp_off();
      ds_cnt = 20;
    } else {
      DS18x20_StartMeasure();
      ds_state = 1;
      ds_cnt = 10;
	}
  break;
  case 1: 
    DS18x20_ReadData(owDevicesIDs[0], data1);
    temperature1 = DS18x20_ConvertToThemperature(owDevicesIDs[0][0],data1);
	if (ds_count == 1)
	{
 	  temperature2 = NO_DS_FOPUND_TEMP;
      if (temperature1 >= ds_temp_max) { ds_temp_on(); }
	  if (temperature1 < (ds_temp_max - 3)) { ds_temp_off(); }
	}
	if (ds_count == 2)
	{
      DS18x20_ReadData(owDevicesIDs[1], data2);
      temperature2 = DS18x20_ConvertToThemperature(owDevicesIDs[1][0],data2);
      if ((temperature1 >= ds_temp_max) || (temperature2 >= ds_temp_max)) { ds_temp_on(); }
	  if ((temperature1 < (ds_temp_max - 3)) && (temperature2 < (ds_temp_max - 3))) { ds_temp_off(); }
	}
    ds_state = 0;
    ds_cnt = 1;
  break;
  }
}
}
//************************************************************************************************
unsigned char search_ow_devices(void) // поиск всех устройств на шине
{ 
unsigned char	i;
unsigned char	id[OW_ROMCODE_SIZE];
unsigned char	diff, sensors_count;
sensors_count = 0;
for ( diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && sensors_count < MAXDEVICES ; )
{ 
  OW_FindROM( &diff, &id[0] );
  if( diff == OW_PRESENCE_ERR ) break;
  if( diff == OW_DATA_ERR )	break;
  for (i = 0;i < OW_ROMCODE_SIZE; i++) 	owDevicesIDs[sensors_count][i] = id[i];
  sensors_count++;
}
return sensors_count;
}
//************************************************************************************************
void check_light(void) 
{
if (light_flag)
{
  if (light_on_off != 2) { light_off(); }
}
}
//************************************************************************************************
//************************************************************************************************
int main(void)
{
  unsigned char ret = 0, i;
  load_param_all();
  standby_out_off();
  eeprom_read_tda7313_all();
  load_chanel_freq_all();
  load_rc5key();
  load_extkey_volt_all();
  load_alarm_eeprom();
  init_port(); 			// инициализация портов
  init_hardware();		// инициализируем перефирию
  light_on();			// включ подсветку
  init_main();	// logo и загрузка данных из еепром
  //инициализируем программные таймеры
  CreateTimer(&ds18x20_timer, 0, 100, ACTIVE, scan_ds18x20);
  CreateTimer(&kbd_timer, 1, 10, ACTIVE, scan_kbd);
  CreateTimer(&adc_timer, 2, 100, ACTIVE, scan_adckbd);
  CreateTimer(&encoder_timer, 3, 2, ACTIVE, scan_encoder);
  CreateTimer(&rc5_timer, 4, 5, ACTIVE, scan_rc5);
  CreateTimer(&nokey_timer, 5, 1000, ACTIVE, scan_nokey);
  RunTimerService(); // запуск системного таймера
  for (i = 0;i < LOGO_TIME; i++) { ext_key_no = extkey_volt; _delay_ms(250); } // Напряжение если не нажато доп клавиатура
  lcd_clear();
  beep();
  while(1)
  {
    //============================================================================================
    if (l_standby)
	{
      if (light_on_off == 2) { light_stb(); } else { light_on(); }
	  light_cnt = 0;
      light_flag = 0;
	  lcd_clear();
      lcd_load_bignum();
      while(1)
      {
        if (check_alarm() == 1) { beep(); break; }
        check_light();
	    lcd_info();
        if (get_key())
		{
          if (run_standby(kbd_key)) { beep(); break; }
          kbd_nokey_cnt++;
          kbd_nokey_flag = 0;
          kbd_key = KBD_NOKEY; rc5.flip = -1; 
		}
		_delay_ms(200);
	  }
      light_on();
      light_flag = 1;
      mmenu = MM_MAIN;
      kbd_nokey_flag = 0;
      kbd_nokey_cnt = 0;
      kbd_key = KBD_NOKEY; rc5.flip = -1; 
      lcd_load_defchar();
      lcd_main_menu();
      send_tda_freq();
	}
    //============================================================================================
    if (l_mute)
	{
      light_on();
	  light_cnt = 0;
      light_flag = 0;
	  lcd_clear();
      lcd_load_bignum();
      while(1)
      {
        if (check_alarm() == 1) { beep(); break; }
        check_light();
	    lcd_info();
        if ( get_key() ) {
		  ret = run_mute(kbd_key);
          if (ret > 0) { beep(); break; }
          kbd_nokey_cnt = 0;
          kbd_nokey_flag = 0;
          kbd_key = KBD_NOKEY; rc5.flip = -1; 
        } 
		_delay_ms(20);
	  }
      light_on();
      light_flag = 1;
      mmenu = MM_MAIN;
      kbd_nokey_flag = 0;
      kbd_nokey_cnt = 0;
      kbd_key = KBD_NOKEY; rc5.flip = -1; 
      lcd_load_defchar();
      if (ret == 2) { lcd_main_menu(); }
	}
    //============================================================================================
    if (kbd_nokey_flag)
	{
      light_on();
	  light_cnt = 0;
      light_flag = 0;
	  lcd_clear();
      lcd_load_bignum();
      while(1)
      {
        if (check_alarm() == 1) { break; }
        check_light();
	    lcd_info();
        if (get_key())
		{
		  if (run_nokey(kbd_key) == 1)
		  {
		    break;
		  } else {
	        lcd_info();
		    beep();
            lcd_com(LCD_ROW1 + 4);
            #if (LCD_TYPE == LCD20x4)
            if (n_select == MSG_SELECT_3) {
              lcd_freq(frequency); lcd_puts(msg[MSG_MHZ]);
            } else { lcd_space(9); }
            #endif
            #if (LCD_TYPE == LCD16x4)
            if (n_select == MSG_SELECT_3)
            { 
              lcd_freq(frequency); lcd_puts(msg[MSG_MHZ]);
            } else { lcd_space(6); }
            #endif
            rc5.flip = -1; 
            kbd_nokey_flag = 0;
            kbd_nokey_cnt = 0;
			kbd_key = KBD_NOKEY;
		  }
		}
	  }
      mmenu = MM_MAIN;
      light_on();
      light_flag = 1;
      rc5.flip = -1; 
      kbd_nokey_flag = 0;
      kbd_nokey_cnt = 0;
      lcd_load_defchar();
	}
    check_alarm();
    switch (mmenu) // switch
    {
      case MM_OPTMENU:
        lcd_option_menu();
      break;
    }
	if ( get_key() )
    {
      switch (mmenu) // switch
      {
      case MM_FMRADIO:
	    ret = run_fmradio(kbd_key);
        if ( ret == 2) { beep(); _delay_ms(500); }
        if (ret == 1) { beep(); }
        if (ret == 0)
		{
          if (run_mainmenu(kbd_key)) { beep(); }
		}
        kbd_key = KBD_NOKEY; rc5.flip = -1; 
      break;
      case MM_OPTMENU:
        if (run_option_menu(kbd_key) == 1) { beep(); }
        kbd_key = KBD_NOKEY; rc5.flip = -1; 
      break;
      case MM_MAIN:
        if (run_mainmenu(kbd_key)) { beep(); }
        kbd_key = KBD_NOKEY; rc5.flip = -1; 
      break;
      }
    }
  }
  return 0;
}
//************************************************************************************************

