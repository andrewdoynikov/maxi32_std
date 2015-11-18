//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
unsigned char edit_fm_station(void)
{
unsigned char l_flag = 1, sel;
fm_menu_cursor = 0; fm_menu_top = 0;
blink_flag = BLINK_OFF;
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
sel = n_select;
n_select = MSG_SELECT_3;
lcd_clear();
lcd_fm_station();
send_tda_freq();
send_lm7001(freq_fm[0]);
//kbd_type = KBD_ONE_PRESS;
while(l_flag)
{
  if (kbd_nokey_flag) { l_flag = 0; }
  if ( get_key() )
  {
    switch (kbd_key) // switch
    {
      case KBD_STANDBY:
	    freq_fm[fm_menu_cursor + fm_menu_top] = 0;
        save_chanel_freq(0, fm_menu_cursor + fm_menu_top);
      break;
      case KBD_UP:
	    blink_flag = BLINK_OFF;
        if (fm_menu_cursor > 0) { fm_menu_cursor--; } else { fm_menu_cursor = 0; fm_menu_top--; }
        if (fm_menu_top < 0) { fm_menu_top = 0; }
      break;
      case KBD_DOWN:
	    blink_flag = BLINK_OFF;
        fm_menu_cursor++;
        if (fm_menu_cursor > 3) { fm_menu_cursor = 3; fm_menu_top++; }
        if (fm_menu_top > (MAX_CHANEL - 3)) { fm_menu_top = MAX_CHANEL - 3; }
      break;
      case KBD_LEFT:
        if (blink_flag)
		{
          if (freq_fm[fm_menu_cursor + fm_menu_top] > MINFREQ) { freq_fm[fm_menu_cursor + fm_menu_top] -= freq_step; }
          if (freq_fm[fm_menu_cursor + fm_menu_top] < MINFREQ) { freq_fm[fm_menu_cursor + fm_menu_top] = MINFREQ; }
        }
      break;
      case KBD_RIGHT:
        if (blink_flag)
		{
          if (freq_fm[fm_menu_cursor + fm_menu_top] < MAXFREQ) { freq_fm[fm_menu_cursor + fm_menu_top] += freq_step; }
          if (freq_fm[fm_menu_cursor + fm_menu_top] > MAXFREQ) { freq_fm[fm_menu_cursor + fm_menu_top] = MAXFREQ; }
        }
      break;
      case KBD_SET:
        blink_flag = !blink_flag;
		if (blink_flag == BLINK_OFF)
		{
		  // save EEPROM
          save_chanel_freq_all();
		} else {
          if ((freq_fm[fm_menu_cursor + fm_menu_top] < MINFREQ) || (freq_fm[fm_menu_cursor + fm_menu_top] > MAXFREQ))
          {
		    freq_fm[fm_menu_cursor + fm_menu_top] = DEFFREQ;
		  }
		}
      break;
      case KBD_MENU:
        l_flag = 0;
	  // save EEPROM All
      save_chanel_freq_all();
      break;
    }
    send_lm7001(freq_fm[fm_menu_cursor + fm_menu_top]);
	beep();
	if ((kbd_key == KBD_LEFT) || (kbd_key == KBD_RIGHT)) { _delay_ms(200); }
    kbd_key = KBD_NOKEY; rc5.flip = -1;
    kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
  }
  lcd_fm_station();
}
//save_chanel_freq_all();
kbd_type = KBD_AUTO_PRESS;
lcd_clear();
lcd_option_menu();
n_select = sel;
send_tda_freq();
send_lm7001(freq_fm[n_chanel - 1]);
blink_flag = BLINK_OFF;
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
return 1;
}
//************************************************************************************************
void lcd_fm_station(void)
{
unsigned char i, n;
unsigned long f;
n = 4 + fm_menu_top;
for (i = fm_menu_top; i < n; i++) {
  f = freq_fm[i];
  lcd_com(menu_rows[i - fm_menu_top]);
  if ((fm_menu_cursor + fm_menu_top) == i) { lcd_dat(MENU_CURSOR); } else { lcd_dat(' '); }
  lcd_puts(msg[MSG_STAN]); print_dec(i + 1,2,'0'); lcd_dat(' '); 
  if (((fm_menu_cursor + fm_menu_top) == i) && (blink_flag == BLINK_ON))
  {
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { lcd_space(6); } else { lcd_freq(f); }
  } else {
    lcd_freq(f);
  }
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW1 + 19);
  if (fm_menu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 19);
  if (fm_menu_top < (MAX_CHANEL - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW1 + 15);
  if (fm_menu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 15);
  if (fm_menu_top < (MAX_CHANEL - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
}
}
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
