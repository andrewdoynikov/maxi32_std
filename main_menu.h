//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
void ac_wait(void)
{
lcd_load_defchar();
while (ac_wait_cnt > 0)
{
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW2 + 3); lcd_puts(msg[MSG_AC]); print_dec(ac_wait_cnt,2,'0'); lcd_puts(msg[MSG_SEC]);
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW2); lcd_puts(msg[MSG_AC]); print_dec(ac_wait_cnt,2,'0'); lcd_puts(msg[MSG_SEC]);
#endif
}
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW2 + 3); lcd_puts(msg[MSG_AC]); print_dec(0, 2, '0'); lcd_puts(msg[MSG_SEC]);
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW2); lcd_puts(msg[MSG_AC]); print_dec(0, 2, '0'); lcd_puts(msg[MSG_SEC]);
#endif
}
//************************************************************************************************
unsigned char run_standby(unsigned char key)
{
switch (key) // switch
{
case KBD_LONDNESS:
  l_londness = !l_londness;
  send_tda_freq();
  eeprom_write_byte((uint8_t*)TDA7313_ADDR_LOUD, l_londness);
  break;
case KBD_STANDBY:
  light_on();
  ac_wait_off();
  standby_out_on();
  ac_wait_cnt = ac_wait_sec;
  lcd_clear();
  ac_wait();
  ac_wait_on();
  // Step do up
  mmenu = MM_MAIN;
  mmenu_cursor = 0, mmenu_top = 0;
  lcd_main_menu();
  step_tda7313_vol(STEP_UP);
  l_standby = 0;
  led_standby_off();
  return 1;
  break;
default:
  return 0;
  break;
}
return 1;
}
//************************************************************************************************
unsigned char run_mute(unsigned char key)
{
switch (key) // switch
{
case KBD_LONDNESS:
  light_on();
  l_londness = !l_londness;
  _delay_ms(200);
  eeprom_write_byte((uint8_t*)TDA7313_ADDR_LOUD, l_londness);
  beep();
  return 0;
  break;
case KBD_STANDBY:
  // Step do down
  light_on();
  lcd_clear();
  ac_wait_cnt = ac_wait_sec;
  ac_wait();
  ac_wait_off();
  standby_out_off();
  l_standby = 1;
  l_mute = 0;
  led_standby_on();
  break;
case KBD_MUTE:
  light_on();
  lcd_load_defchar();
  lcd_clear();
  lcd_main_menu();
  step_tda7313_vol(STEP_UP);
  l_mute = 0;
  return 2;
  break;
default:
  return 0;
  break;
}
return 1;
}
//************************************************************************************************
unsigned char run_nokey(unsigned char key)
{
if ((key >= KBD_1) && (key <= KBD_9) && (n_select == MSG_SELECT_3))
{
  n_chanel = (n_bank * 10) + (key - KBD_EXT_KEY_START);
  if (n_chanel == 0) { n_bank = 0; n_chanel = 1; }
  frequency = freq_fm[n_chanel - 1];
  send_tda_freq();
  save_chanel();
  save_bank();
  _delay_ms(200);
  return 0;
}
switch (key) // switch
{
case KBD_DOWN:
  if (n_select == MSG_SELECT_3)
  {
    if (n_chanel < MAX_CHANEL) { n_chanel++; } else { n_chanel = 1; }
    frequency = freq_fm[n_chanel - 1];
    send_tda_freq();
    save_chanel();
    _delay_ms(300);
  }
  return 0;
  break;
case KBD_UP:
  if (n_select == MSG_SELECT_3)
  {
    if (n_chanel > 1) { n_chanel--; } else { n_chanel = MAX_CHANEL; }
    frequency = freq_fm[n_chanel - 1];
    send_tda_freq();
    save_chanel();
    _delay_ms(300);
  }
  return 0;
  break;
case KBD_BANK:
  if (n_select == MSG_SELECT_3)
  {
    n_bank = !n_bank;
    if (n_chanel > 10) { n_chanel = n_chanel - 10; }
    n_chanel = (n_bank * 10) + n_chanel;
    frequency = freq_fm[n_chanel - 1];
    send_tda_freq();
    save_chanel();
    save_bank();
  }
  return 0;
  break;
case KBD_LONDNESS:
  l_londness = !l_londness;
  send_tda_freq();
  eeprom_write_byte((uint8_t*)TDA7313_ADDR_LOUD, l_londness);
  return 0;
  break;
default:
  return 1;
  break;
}
return 1;
}
//************************************************************************************************
unsigned char run_mainmenu(unsigned char key)
{
unsigned char i = 0;
lcd_main_menu();
if ((key >= KBD_1) && (key <= KBD_9))
{
  n_chanel = (n_bank * 10) + (key - KBD_EXT_KEY_START);
  if (n_chanel == 0) { n_bank = 0; n_chanel = 1; }
  frequency = freq_fm[n_chanel - 1];
  send_tda_freq();
  save_chanel();
  save_bank();
  _delay_ms(200);
  return 1;
}
switch (key) // switch
{
case KBD_BANK:
  n_bank = !n_bank;
  if (n_chanel > 10) { n_chanel = n_chanel - 10; }
  n_chanel = (n_bank * 10) + n_chanel;
  frequency = freq_fm[n_chanel - 1];
  send_tda_freq();
  save_bank();
  break;
case KBD_STANDBY:
  // Step do down
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
  break;
case KBD_MUTE:
  lcd_main_menu();
  step_tda7313_vol(STEP_DOWN);
  l_mute = 1;
  send_tda7313_mute();
  break;
case KBD_DOWN:
  mmenu_cursor++;
  if (mmenu_cursor > 3) { mmenu_cursor = 3; mmenu_top++; }
  if (mmenu_top > (MMENU_MAX - 3)) { mmenu_top = MMENU_MAX - 3; }
  break;
case KBD_UP:
  if (mmenu_cursor > 0) { mmenu_cursor--; } else { mmenu_cursor = 0; mmenu_top--; }
  if (mmenu_top < 0) { mmenu_top = 0; }
  break;
case KBD_SELECT:
  n_select++;
  if (n_select > SELECT_MAX) { n_select = 0; }
  lcd_clear();
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW2 + 5); lcd_puts(msg[MSG_SELECT]); lcd_puts(select[n_select]);
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW2 + 2); lcd_puts(msg[MSG_SELECT]); lcd_puts(select[n_select]);
#endif
  send_tda_freq();
  main_delay(3);
  eeprom_write_byte((uint8_t*)TDA7313_ADDR_SELECT, n_select);
  mmenu = MM_MAIN; mmenu_cursor = 0; mmenu_top = 0;
  lcd_main_menu();
  break;
case KBD_LEFT:
  i = mmenu_top + mmenu_cursor;
  if (tda7313_param[n_select][i] > 0) { tda7313_param[n_select][i]--; }
  send_tda_freq();
  eeprom_write_tda7313_cursor(i);
  break;
case KBD_RIGHT:
  i = mmenu_top + mmenu_cursor;
  if (tda7313_param[n_select][i] < tda7313_max[i]) { tda7313_param[n_select][i]++; }
  send_tda_freq();
  eeprom_write_tda7313_cursor(i);
  break;
case KBD_SET:
  if (n_select == MSG_SELECT_3)
  {
    lcd_clear();
    mmenu = MM_FMRADIO;
    lcd_main_menu();
  }
  break;
case KBD_MENU:
  omenu_cursor = 0, omenu_top = 0;
  lcd_clear();
  mmenu = MM_OPTMENU;
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW2 + 5); lcd_puts(msg[MSG_OMENU]);
  lcd_com(LCD_ROW3 + 5); lcd_puts(msg[MSG_OMENU1]);
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW2 + 3); lcd_puts(msg[MSG_OMENU]);
  lcd_com(LCD_ROW3 + 3); lcd_puts(msg[MSG_OMENU1]);
#endif
  main_delay(2);
  lcd_clear();
  lcd_option_menu();
  return 1;
  break;
case KBD_LONDNESS:
  l_londness = !l_londness;
  send_tda_freq();
  eeprom_write_byte((uint8_t*)TDA7313_ADDR_LOUD, l_londness);
  break;
case KBD_BASS_UP:
  if (tda7313_param[n_select][MM_BASS] > 0) { tda7313_param[n_select][MM_BASS]--; }
  send_tda_freq();
  eeprom_write_tda7313_cursor(MM_BASS);
  break;
case KBD_BASS_DOWN:
  if (tda7313_param[n_select][MM_BASS] < tda7313_max[MM_BASS]) { tda7313_param[n_select][MM_BASS]++; }
  send_tda_freq();
  eeprom_write_tda7313_cursor(MM_BASS);
  break;
case KBD_TREBLE_UP:
  if (tda7313_param[n_select][MM_TREBLE] > 0) { tda7313_param[n_select][MM_TREBLE]--; }
  send_tda_freq();
  eeprom_write_tda7313_cursor(MM_TREBLE);
  break;
case KBD_TREBLE_DOWN:
  if (tda7313_param[n_select][MM_TREBLE] < tda7313_max[MM_TREBLE]) { tda7313_param[n_select][MM_TREBLE]++; }
  send_tda7313();
  eeprom_write_tda7313_cursor(MM_TREBLE);
  break;
default:
  return 0;
  break;
}
lcd_main_menu();
return 1;
}
//************************************************************************************************
void lcd_main_menu(void)
{
unsigned char i, n, p;
if (mmenu == MM_MAIN)
{
  n = 4 + mmenu_top;
for (i = mmenu_top; i < n; i++) {
  lcd_com(menu_rows[i - mmenu_top]);
  if ((mmenu_cursor + mmenu_top) == i) lcd_dat(MENU_CURSOR); else lcd_dat(' ');
  lcd_puts(mainmenu[i]);
  p = tda7313_param[n_select][i];
#if (LCD_TYPE == LCD20x4)
  lcdprogress_bar(p, tda7313_max[i], 10);
  lcd_dat(' '); print_dec(p, 2,'0'); 
  lcd_com(LCD_ROW1 + 19);
  if (mmenu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 19);
  if (mmenu_top < (MMENU_MAX - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
#if (LCD_TYPE == LCD16x4)
  lcdprogress_bar(p, tda7313_max[i], 6);
  lcd_dat(' '); print_dec(p, 2,'0');
  lcd_com(LCD_ROW1 + 15);
  if (mmenu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 15);
  if (mmenu_top < (MMENU_MAX - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
  }
} else {
  p = tda7313_param[n_select][MM_VOLUME];
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW1 + 1); lcd_puts(msg[MSG_CH]); print_dec(n_chanel,2,'0'); 
  lcd_com(LCD_ROW2 + 1); lcd_freq(frequency); lcd_puts(msg[MSG_MHZ]);
  lcd_com(LCD_ROW4 + 1); lcdprogress_bar(p, tda7313_max[MM_VOLUME], 16);
  lcd_dat(' '); print_dec(p, 2,'0');
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW1 + 1); lcd_puts(msg[MSG_CH]); print_dec(n_chanel,2,'0'); 
  lcd_com(LCD_ROW2 + 1); lcd_freq(frequency); lcd_puts(msg[MSG_MHZ]);
  lcd_com(LCD_ROW4 + 1); lcdprogress_bar(p, tda7313_max[MM_VOLUME], 12);
  lcd_dat(' '); print_dec(p, 2,'0');
#endif
}
}
//************************************************************************************************
unsigned char run_fmradio(unsigned char key)
{
if ((key >= KBD_1) && (key <= KBD_9))
{
  n_chanel = (n_bank * 10) + (key - KBD_EXT_KEY_START);
  if (n_chanel == 0) { n_bank = 0; n_chanel = 1; }
  frequency = freq_fm[n_chanel - 1];
  send_tda_freq();
  save_chanel();
  save_bank();
  lcd_main_menu();
  return 1;
}
switch (key) // switch
{
case KBD_BANK:
  n_bank = !n_bank;
  if (n_chanel > 10) { n_chanel = n_chanel - 10; }
  n_chanel = (n_bank * 10) + n_chanel;
  frequency = freq_fm[n_chanel - 1];
  send_tda_freq();
  save_bank();
  lcd_main_menu();
  break;
case KBD_DOWN:
  if (n_chanel < MAX_CHANEL) { n_chanel++; } else { n_chanel = 1; }
  frequency = freq_fm[n_chanel - 1];
  send_tda_freq();
  save_chanel();
  lcd_main_menu();
  return 2;
  break;
case KBD_UP:
  if (n_chanel > 1) { n_chanel--; } else { n_chanel = MAX_CHANEL; }
  frequency = freq_fm[n_chanel - 1];
  send_tda_freq();
  save_chanel();
  lcd_main_menu();
  return 2;
  break;
case KBD_LEFT:
  if (tda7313_param[MSG_SELECT_3][MM_VOLUME] > 0) { tda7313_param[MSG_SELECT_3][MM_VOLUME]--; }
  send_tda_freq();
  eeprom_write_tda7313_cursor(MM_VOLUME);
  break;
case KBD_RIGHT:
  if (tda7313_param[MSG_SELECT_3][MM_VOLUME] < tda7313_max[MM_VOLUME]) { tda7313_param[MSG_SELECT_3][MM_VOLUME]++; }
  send_tda_freq();
  eeprom_write_tda7313_cursor(MM_VOLUME);
  break;
case KBD_SET:
  mmenu = MM_MAIN; mmenu_cursor = 0; mmenu_top = 0;
  break;
default:
  return 0;
  break;
}
lcd_main_menu();
return 1;
}
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
