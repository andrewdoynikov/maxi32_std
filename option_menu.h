//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
unsigned char run_option_menu(unsigned char key)
{
switch (key) // switch
{
case KBD_STANDBY:
  mmenu_cursor = 0, mmenu_top = 0;
  mmenu = MM_MAIN;
  lcd_main_menu();
  eeprom_write_tda7313_all();
  step_tda7313_vol(STEP_DOWN);
  lcd_clear();
  ac_wait_cnt = ac_wait_sec;
  ac_wait();
  ac_wait_off();
  standby_out_off();
  l_standby = 1;
  led_standby_on();
  return 1;
  break;
case KBD_DOWN:
  blink_flag = BLINK_OFF;
  omenu_cursor++;
  if (omenu_cursor > 3) { omenu_cursor = 3; omenu_top++; }
  if (omenu_top > (OMENU_MAX - 3)) { omenu_top = OMENU_MAX - 3; }
  break;
case KBD_UP:
  blink_flag = BLINK_OFF;
  if (omenu_cursor > 0) { omenu_cursor--; } else { omenu_cursor = 0; omenu_top--; }
  if (omenu_top < 0) { omenu_top = 0; }
  break;
case KBD_LEFT:
  if (blink_flag == BLINK_ON)
  {
    run_edit_procedure(omenu_cursor + omenu_top);
  }
  break;
case KBD_RIGHT:
  if (blink_flag == BLINK_ON)
  {
    run_edit_procedure(omenu_cursor + omenu_top);
  }
  break;
case KBD_SET:
  beep();
  if ((omenu_cursor + omenu_top) > SET_EXT_KEY)
  {
    blink_flag = !blink_flag;
	if (blink_flag == BLINK_OFF)
	{
	  // Save EEPROM param omenu_cursor + omenu_top
	  save_eeprom_param(omenu_cursor + omenu_top);
    } else {
      run_edit_procedure(omenu_cursor + omenu_top);
    }
  } else {
    run_edit_procedure(omenu_cursor + omenu_top);
  }
  lcd_option_menu();
  return 0;
  break;
case KBD_MENU:
  mmenu_cursor = 0, mmenu_top = 0;
  lcd_clear();
  mmenu = MM_MAIN;
  lcd_main_menu();
  save_param_all();
  return 1;
  break;
default:
  return 0;
  break;
}
lcd_option_menu();
return 1;
}
//************************************************************************************************
void lcd_option_menu(void)
{
unsigned char i, n, k;
n = 4 + omenu_top;
for (i = omenu_top; i < n; i++)
{
  k = omenu_cursor + omenu_top;
  lcd_com(menu_rows[i - omenu_top]);
  if ((omenu_cursor + omenu_top) == i) lcd_dat(MENU_CURSOR); else lcd_dat(' ');
  lcd_puts(optionmenu[i]);
  if ((i > SET_EXT_KEY) && ( k == i) && (blink_flag == BLINK_ON))
  {
#if (LCD_TYPE == LCD20x4)
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { lcd_space(6); } else { lcd_option(i); }
#endif
#if (LCD_TYPE == LCD16x4)
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { lcd_space(3); } else { lcd_option(i); }
#endif
  } else {
    lcd_option(i);
  }
}
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW1 + 19);
  if (omenu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 19);
  if (omenu_top < (OMENU_MAX - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW1 + 15);
  if (omenu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 15);
  if (omenu_top < (OMENU_MAX - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
}
//************************************************************************************************
#if (LCD_TYPE == LCD20x4)
void lcd_option(unsigned char noption)
{
switch (noption) // switch
{
  case  SET_TIME:
    rtc_print_time_read(); lcd_space(2);
  break;
  case  SET_DATE:
    rtc_print_date_read();
  break;
  case  SET_MAX_TEMP:
    print_dec(ds_temp_max, 3, ' '); lcd_dat('C'); 
  break;
  case  SET_NOKEY:
    print_dec(kbd_nokey, 3, ' '); lcd_dat(' ');
  break;
  case  SET_STB_OUT:
    if (standby_out) { print_dec(1, 3, ' '); } else { print_dec(0, 3, ' '); } lcd_dat(' ');
  break;
  case  SET_AC_WAIT:
    print_dec(ac_wait_sec, 3, ' '); lcd_dat(' ');
  break;
  case  SET_RC5_SYSTEM:
    print_dec(rc5_system, 3, '0'); lcd_dat(' ');
  break;
  case  SET_LIGHT_LCD:
    switch (light_on_off)
	{
	  case 0:
	    lcd_puts(msg[MSG_AVTO]);
	  break;
	  case 1:
	    lcd_puts(msg[MSG_ON]);
	  break;
	  case 2:
	    lcd_puts(msg[MSG_000]);
	  break;
	}
  break;
  default:
    lcd_space(4);
  break;
}
}
#endif
//************************************************************************************************
#if (LCD_TYPE == LCD16x4)
void lcd_option(unsigned char noption)
{
switch (noption) // switch
{
  case  SET_TIME:
    rtc_print_time_read();
  break;
  case  SET_DATE:
    rtc_print_date_read();
  break;
  case  SET_MAX_TEMP:
    print_dec(ds_temp_max, 2, ' '); lcd_dat('C');
  break;
  case  SET_NOKEY:
    print_dec(kbd_nokey, 2, ' '); lcd_dat(' ');
  break;
  case  SET_STB_OUT:
    if (standby_out) { print_dec(1, 2, ' '); } else { print_dec(0, 2, ' '); } lcd_dat(' ');
  break;
  case  SET_AC_WAIT:
    print_dec(ac_wait_sec, 2, ' '); lcd_dat(' ');
  break;
  case  SET_RC5_SYSTEM:
    print_dec(rc5_system, 3, ' ');
  break;
  case  SET_LIGHT_LCD:
    switch (light_on_off)
	{
	  case 0:
	    lcd_puts(msg[MSG_AVTO]);
	  break;
	  case 1:
	    lcd_puts(msg[MSG_ON]);
	  break;
	  case 2:
	    lcd_puts(msg[MSG_000]);
	  break;
	}
  break;
  default:
    lcd_space(3);
  break;
}
}
#endif
//************************************************************************************************
void run_edit_procedure(unsigned char nopt)
{
switch (nopt) // switch
{
  case  SET_TIME:
    edit_time(menu_rows[omenu_cursor]);
  break;
  case  SET_DATE:
    edit_date(menu_rows[omenu_cursor]);
  break;
  case  SET_FM_CHANEL:
    edit_fm_station();
  break;
  case  SET_ALARM:
    edit_alarm();
  break;
  case  SET_EXT_KEY:
    edit_ext_key();
  break;
  case  SET_RC5_KEY:
    edit_rc5_key();
  break;
  case  SET_MAX_TEMP:
    ds_temp_max = edit_option_num(ds_temp_max, DS_MIN_TEMP, DS_MAX_TEMP);
  break;
  case  SET_NOKEY:
    kbd_nokey = edit_option_num(kbd_nokey, KBD_NOKEY_TIME_MIN, KBD_NOKEY_TIME_MAX);
  break;
  case  SET_STB_OUT:
    standby_out = edit_option_log(standby_out);
  break;
  case  SET_AC_WAIT:
    ac_wait_sec = edit_option_num(ac_wait_sec, AC_WAIT_MIN, AC_WAIT_MAX);
  break;
  case  SET_RC5_SYSTEM:
    rc5_system = edit_option_num(rc5_system, 0, 255);
  break;
  case  SET_LIGHT_LCD:
    light_on_off = edit_option_num(light_on_off,0,2);
  break;
}
}
//************************************************************************************************
void save_eeprom_param(unsigned char param)
{
switch (param) // switch
{
  case  SET_MAX_TEMP:
	save_max_temp();
  break;
  case  SET_NOKEY:
	save_nokey();
  break;
  case  SET_STB_OUT:
	save_stb_out();
  break;
  case  SET_AC_WAIT:
	save_ac_wait();
  break;
  case  SET_RC5_SYSTEM:
	save_rc5_system();
    lcd_clear();
#if (LCD_TYPE == LCD20x4)
    lcd_com(LCD_ROW2 + 4); lcd_puts(msg[MSG_PEREZ1]);
    lcd_com(LCD_ROW3 + 4); lcd_puts(msg[MSG_PEREZ2]);
#endif
#if (LCD_TYPE == LCD16x4)
    lcd_com(LCD_ROW2 + 2); lcd_puts(msg[MSG_PEREZ1]);
    lcd_com(LCD_ROW3 + 2); lcd_puts(msg[MSG_PEREZ2]);
#endif
  main_delay(4);
  lcd_clear();
  lcd_option_menu();
  break;
  case  SET_LIGHT_LCD:
	save_light();
  break;
}
}
//************************************************************************************************
unsigned char edit_option_num(unsigned char val, unsigned char min, unsigned char max)
{
switch (kbd_key) // switch
{
  case KBD_LEFT:
  if (val > min) { val--; } else { val = max; }
  break;
  case KBD_RIGHT:
  if (val < max) { val++; } else { val = min; }
  break;
}
return val;
}
//************************************************************************************************
unsigned char edit_option_log(unsigned char val)
{
switch (kbd_key) // switch
{
  case KBD_LEFT:
  val = !val;
  break;
  case KBD_RIGHT:
  val = !val;
  break;
}
_delay_ms(300);
return val;
}
//************************************************************************************************
void edit_time(unsigned char row)
{
unsigned char n_edit, h,m,s, l_flag = 1;
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
n_edit = 0;
rtc_get_time(&h,&m,&s);
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
          if (h > 0) { h--; } else { h = 23; }
          break;
          case DEF_TIME_M:
          if (m > 0) { m--; } else { m = 59; }
          break;
        }
      break;
      case KBD_RIGHT: case KBD_DOWN:
        switch (n_edit) // switch
        { 
          case DEF_TIME_H:
          if (h < 23) { h++; } else { h = 0; }
          break;
          case DEF_TIME_M:
          if (m < 59) { m++; } else { m = 0; }
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
  lcd_com(row + 8);
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(row + 7);
#endif
  switch (n_edit) // switch
  { 
    case DEF_TIME_H:
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { print_dec(h,2,'0'); } else { lcd_space(2); }
    lcd_dat(':'); print_dec(m,2,'0'); 
    break;
    case DEF_TIME_M:
    print_dec(h,2,'0'); lcd_dat(':'); 
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { print_dec(m,2,'0'); } else { lcd_space(2); }
    break;
  }
  lcd_dat(':'); print_dec(s,2,'0');
}
kbd_type = KBD_AUTO_PRESS;
rtc_set_time(h, m, 0);
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
}
//************************************************************************************************
unsigned char edit_date(unsigned char row)
{
unsigned char n_edit, d,m,y, l_flag = 1;
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
n_edit = 0;
rtc_get_date(&d,&m,&y);
kbd_type = KBD_ONE_PRESS;
while(l_flag)
{
  if (kbd_nokey_flag) { l_flag = 0;  }
  if ( get_key() )
  {
    switch (kbd_key) // switch
    {
      case KBD_LEFT: case KBD_UP:
        switch (n_edit) // switch
        { 
          case DEF_DATE_D:
          if (d > 1) { d--; } else { d = 31; }
          break;
          case DEF_DATE_M:
          if (m > 1) { m--; } else { m = 12; }
          break;
          case DEF_DATE_Y:
          if (y > 10) { y--; } else { y = 10; }
          break;
        }
      break;
      case KBD_RIGHT: case KBD_DOWN:
        switch (n_edit) // switch
        { 
          case DEF_DATE_D:
          if (d < 31) { d++; } else { d = 1; }
          break;
          case DEF_DATE_M:
          if (m < 12) { m++; } else { m = 1; }
          break;
          case DEF_DATE_Y:
          y++;
          break;
        }
      break;
      case KBD_SET:
        n_edit++;
        if (n_edit > 2) { l_flag = 0; }
      break;
      case KBD_MENU:
        n_edit = 0; l_flag = 0;
      break;
    }
    kbd_key = KBD_NOKEY; rc5.flip = -1; beep();
    kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
  }
#if (LCD_TYPE == LCD20x4)
  lcd_com(row + 8);
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(row + 7);
#endif
  switch (n_edit) // switch
  { 
    case DEF_DATE_D:
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { print_dec(d,2,'0'); } else { lcd_space(2); }
    lcd_dat('/'); print_dec(m,2,'0'); 
#if (LCD_TYPE == LCD20x4)
    lcd_dat('/'); print_dec(20,2,'0'); print_dec(y,2,'0');
#endif
#if (LCD_TYPE == LCD16x4)
    lcd_dat('/'); print_dec(y,2,'0');
#endif
    break;
    case DEF_DATE_M:
    print_dec(d,2,'0'); lcd_dat('/');
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { print_dec(m,2,'0'); } else { lcd_space(2); }
#if (LCD_TYPE == LCD20x4)
    lcd_dat('/'); print_dec(20,2,'0'); print_dec(y,2,'0');
#endif
#if (LCD_TYPE == LCD16x4)
    lcd_dat('/'); print_dec(y,2,'0');
#endif
    break;
    case DEF_DATE_Y:
#if (LCD_TYPE == LCD20x4)
    print_dec(d,2,'0'); lcd_dat('/'); print_dec(m,2,'0'); lcd_dat('/');
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { print_dec(20,2,'0'); print_dec(y,2,'0'); } else { lcd_space(4); }
#endif
#if (LCD_TYPE == LCD16x4)
    print_dec(d,2,'0'); lcd_dat('/'); print_dec(m,2,'0'); lcd_dat('/');
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { print_dec(y,2,'0'); } else { lcd_space(2); }
#endif
    break;
  }
}
kbd_type = KBD_AUTO_PRESS;
rtc_set_date(d, m, y);
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
return 0;
}
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
