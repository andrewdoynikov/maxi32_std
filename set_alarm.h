//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
void edit_alarm(void)
{
unsigned char l_flag = 1;
alarm_menu_cursor = 0; alarm_menu_top = 0;
blink_flag = BLINK_OFF;
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
lcd_clear();
lcd_alarm();
kbd_type = KBD_ONE_PRESS;
while(l_flag)
{
  if (kbd_nokey_flag) { l_flag = 0; }
  if ( get_key() )
  {
    switch (kbd_key) // switch
    {
      case KBD_STANDBY:
        l_flag = 0;
      break;
      case KBD_UP:
		if (alarm_param[ALARM_ON_OFF])
		{
	    blink_flag = BLINK_OFF;
          if (alarm_menu_cursor > 0) { alarm_menu_cursor--; } else { alarm_menu_cursor = 0; alarm_menu_top--; }
          if (alarm_menu_top < 0) { alarm_menu_top = 0; }
        } 
      break;
      case KBD_DOWN:
		if (alarm_param[ALARM_ON_OFF])
		{
 	      blink_flag = BLINK_OFF;
          alarm_menu_cursor++;
          if (alarm_menu_cursor > 3) { alarm_menu_cursor = 3; alarm_menu_top++; }
          if (alarm_menu_top > (AMENU_MAX - 3)) { alarm_menu_top = AMENU_MAX - 3; }
        }
      break;
      case KBD_LEFT:
        if (blink_flag)
		{
          run_edit_proc_alarm(alarm_menu_cursor + alarm_menu_top);
        }
      break;
      case KBD_RIGHT:
        if (blink_flag)
		{
          run_edit_proc_alarm(alarm_menu_cursor + alarm_menu_top);
        }
      break;
      case KBD_SET:
        blink_flag = !blink_flag;
		if (blink_flag == BLINK_OFF)
		{
		  // save EEPROM
          save_alarm_eeprom();
		} else {
          if ((alarm_menu_cursor + alarm_menu_top) == SET_ATIME) { run_edit_proc_alarm(alarm_menu_cursor + alarm_menu_top); }
        }
      break;
      case KBD_MENU:
        l_flag = 0;
	  // save EEPROM All
        save_alarm_eeprom();
      break;
    }
	beep();
    kbd_key = KBD_NOKEY; rc5.flip = -1;
    kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
  }
  lcd_alarm();
}
kbd_type = KBD_AUTO_PRESS;
lcd_clear();
lcd_option_menu();
blink_flag = BLINK_OFF;
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
}
//************************************************************************************************
void lcd_alarm(void)
{
unsigned char i, n, k;
n = 4 + alarm_menu_top;
for (i = alarm_menu_top; i < n; i++)
{
  k = alarm_menu_cursor + alarm_menu_top;
  lcd_com(menu_rows[i - alarm_menu_top]);
  if ((alarm_menu_cursor + alarm_menu_top) == i) lcd_dat(MENU_CURSOR); else lcd_dat(' ');
  lcd_puts(alarmmenu[i]);
  if ( (i != SET_ATIME) && ( k == i) && (blink_flag == BLINK_ON))
  {
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { lcd_space(8); } else { lcd_option_alarm(i); }
  } else {
    lcd_option_alarm(i);
  }
}
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW1 + 19);
  if (alarm_menu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 19);
  if (alarm_menu_top < (AMENU_MAX - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW1 + 15);
  if (alarm_menu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 15);
  if (alarm_menu_top < (AMENU_MAX - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
}
//************************************************************************************************
void run_edit_proc_alarm(unsigned char n_proc)
{
switch (n_proc) // switch
{
  case  SET_ALARM_ON_OFF:
    alarm_param[ALARM_ON_OFF] = edit_option_log(alarm_param[ALARM_ON_OFF]);
  break;
  case  SET_ATIME:
    beep();
    edit_time_alarm(menu_rows[alarm_menu_cursor]);
  break;
  case SET_ATYPE:
    alarm_param[ALARM_TYPE] = edit_option_log(alarm_param[ALARM_TYPE]);
  break;
//  case SET_AINPUT:
//    alarm_param[ALARM_INPUT] = edit_option_num(alarm_param[ALARM_INPUT], 0, SELECT_MAX);
//  break;
  case SET_AMIN:
    alarm_param[ALARM_MINUT] = edit_option_num(alarm_param[ALARM_MINUT], ALARM_MIN, ALARM_MAX);
  break;
//  case SET_APOSLE:
//    alarm_param[ALARM_POSLE] = edit_option_log(alarm_param[ALARM_POSLE]);
//  break;
  case SET_ABUD:
    alarm_param[ALARM_BUD] = edit_option_log(alarm_param[ALARM_BUD]);
  break;
}
}
//************************************************************************************************
void lcd_option_alarm(unsigned char n_param)
{
switch (n_param) // switch
{
  case SET_ATIME:
    print_dec(alarm_param[ALARM_H],2,'0'); lcd_dat(':'); print_dec(alarm_param[ALARM_M],2,'0'); lcd_dat(':'); print_dec(0,2,'0');
  break;
  case SET_ALARM_ON_OFF:
    if (alarm_param[ALARM_ON_OFF]) { lcd_puts(msg[MSG_ON]); } else { lcd_puts(msg[MSG_OFF]); }; lcd_space(4);
  break;
  case SET_ATYPE:
    if (alarm_param[ALARM_TYPE] == 0) { lcd_puts(msg[MSG_PISK]); } else { lcd_puts(msg[MSG_IN]); }; lcd_space(4);
  break;
//  case SET_AINPUT:
//    lcd_puts(select[alarm_param[ALARM_INPUT]]); lcd_space(5);
//  break;
  case SET_AMIN:
    print_dec(alarm_param[ALARM_MINUT],2,' '); lcd_space(6);
  break;
//  case SET_APOSLE:
//    lcd_dat(' ');
//    if (alarm_param[ALARM_POSLE] == 0) { lcd_puts(msg[MSG_NET]); } else { lcd_puts(msg[MSG_DA]); }; lcd_space(4);
//  break;
  case SET_ABUD:
    lcd_dat(' ');
    if (alarm_param[ALARM_BUD] == 0) { lcd_puts(msg[MSG_NET]); } else { lcd_puts(msg[MSG_DA]); }; lcd_space(4);
  break;
  default:
    lcd_space(8);
  break;
}
}
//************************************************************************************************
void edit_time_alarm(unsigned char row)
{
unsigned char n_edit, l_flag = 1;
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
n_edit = 0;
kbd_type = KBD_ONE_PRESS;
while(l_flag)
{
  if (kbd_nokey_flag) { l_flag = 0; }
  if ( get_key() )
  {
    switch (kbd_key) // switch
    {
      case KBD_LEFT: case KBD_UP:
        switch (n_edit) // switch
        { 
          case DEF_TIME_H:
          if (alarm_param[ALARM_H] > 0) { alarm_param[ALARM_H]--; } else { alarm_param[ALARM_H] = 23; }
          break;
          case DEF_TIME_M:
          if (alarm_param[ALARM_M] > 0) { alarm_param[ALARM_M]--; } else { alarm_param[ALARM_M] = 59; }
          break;
        }
      break;
      case KBD_RIGHT: case KBD_DOWN:
        switch (n_edit) // switch
        { 
          case DEF_TIME_H:
          if (alarm_param[ALARM_H] < 23) { alarm_param[ALARM_H]++; } else { alarm_param[ALARM_H] = 0; }
          break;
          case DEF_TIME_M:
          if (alarm_param[ALARM_M] < 59) { alarm_param[ALARM_M]++; } else { alarm_param[ALARM_M] = 0; }
          break;
        }
      break;
      case KBD_SET:
        n_edit++;
        if (n_edit > 1) { l_flag = 0; }
      break;
      case KBD_MENU:
        n_edit = 0; l_flag = 0;
      break;
    }
	beep();
    kbd_key = KBD_NOKEY; rc5.flip = -1;
    kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
  }
#if (LCD_TYPE == LCD20x4)
  lcd_com(row + 11);
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(row + 7);
#endif
  switch (n_edit) // switch
  { 
    case DEF_TIME_H:
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { print_dec(alarm_param[ALARM_H],2,'0'); } else { lcd_space(2); }
    lcd_dat(':'); print_dec(alarm_param[ALARM_M],2,'0'); 
    break;
    case DEF_TIME_M:
    print_dec(alarm_param[ALARM_H],2,'0'); lcd_dat(':'); 
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { print_dec(alarm_param[ALARM_M],2,'0'); } else { lcd_space(2); }
    break;
  }
  lcd_dat(':'); print_dec(0,2,'0');
}
l_alarm = 0;
kbd_type = KBD_AUTO_PRESS;
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
}
//************************************************************************************************
unsigned char check_alarm(void)
{
unsigned char h,m,s, l_flag = 1, sec = 60, blink = 0;
rtc_get_time(&h,&m,&s);
if ((alarm_posle_flag == 1) && (l_alarm > 0))
{
  if (!kbd_nokey_flag) { lcd_clear(); }
  lcd_load_bignum();
  l_flag = 1;
  while ((l_flag) && (l_alarm > 0))
  {
    lcd_info();
//	lcd_com(LCD_ROW1); lcd_puts(select[n_select]);
    if (get_key())
    {
      lcd_info();
	  beep();
      kbd_nokey_flag = 1;
	  l_alarm = 60 - s + 2;
      alarm_regim = 0; 
      kbd_key = KBD_NOKEY; rc5.flip = -1;
	  break;
    }
  }
  alarm_posle_flag = 0;
  if (alarm_param[ALARM_BUD])
  {
    alarm_param[ALARM_ON_OFF] = 0; 
    eeprom_write_byte((uint8_t*)PARAM_ALARM_ON_OFF, 0);
  }
  if (alarm_regim == 1) { lcd_load_defchar(); run_mainmenu(KBD_STANDBY); return 0; }
  if (alarm_regim == 2) { lcd_load_defchar(); run_mainmenu(KBD_MUTE); return 0; }
  if (alarm_regim == 0) { if (kbd_nokey_flag) { lcd_load_bignum(); return 0; } else { lcd_load_defchar(); return 1; } }
} else {
if ((alarm_param[ALARM_ON_OFF]) && (l_alarm == 0))
{
  if ((alarm_param[ALARM_H] == h) && (alarm_param[ALARM_M] == m))
  {
    if (alarm_param[ALARM_TYPE] == 0)
	{
      if ((!l_standby) && (!l_mute)) { send_tda7313_mute(); }
      sec = 0;
	  while ((l_flag) && (alarm_param[ALARM_H] == h) && (alarm_param[ALARM_M] == m))
	  {
		alarm_posle_flag = 1;
        rtc_get_time(&h,&m,&s);
		if (sec != s) { sec = s; blink = !blink; }
        if ((l_standby) || (l_mute) || (kbd_nokey_flag))
		{ 
		  lcd_info(); 
		}
        if (blink) { PORT_BEEP &= ~(_BV(PIN_BEEP)); } else { PORT_BEEP |= _BV(PIN_BEEP); }
	    if (get_key())
		{
	      l_alarm = 60 - s + 2;
          kbd_key = KBD_NOKEY; rc5.flip = -1;
		  l_flag = 0;
		}    
	  }
	  alarm_posle_flag = 0;
      PORT_BEEP |= _BV(PIN_BEEP);
	  if (alarm_param[ALARM_BUD])
	  {
	    alarm_param[ALARM_ON_OFF] = 0; 
        eeprom_write_byte((uint8_t*)PARAM_ALARM_ON_OFF, 0);
      }
      if ((!l_standby) && (!l_mute)) { send_tda_freq(); }
	} else {
//	  if (alarm_param[ALARM_POSLE] == 1)
//	  {
	    l_alarm = alarm_param[ALARM_MINUT] * 60;
		alarm_posle_flag = 1;
//	  }
//      n_select = alarm_param[ALARM_INPUT];
      n_select = MSG_SELECT_3;
      eeprom_write_byte((uint8_t*)TDA7313_ADDR_SELECT, n_select);
	  if (l_standby) { run_standby(KBD_STANDBY); alarm_regim = 1; return 1; }
	  if (l_mute) { run_mute(KBD_MUTE); alarm_regim = 2; return 1; }
	  if (kbd_nokey_flag) { alarm_regim = 0; lcd_info(); }
      if ((!l_standby) && (!l_mute)) { send_tda_freq(); }
	}
  }
}
}
return 0;
}

//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
