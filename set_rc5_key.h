//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
unsigned char edit_rc5_key(void)
{
unsigned char l_flag = 1;
rc5_menu_cursor = 0; rc5_menu_top = 0;
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
lcd_clear();
lcd_rc5_key();
l_no_rc5 = 0;
rc5_code = 0xFF;
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
        if (rc5_menu_cursor > 0) { rc5_menu_cursor--; } else { rc5_menu_cursor = 0; rc5_menu_top--; }
        if (rc5_menu_top < 0) { rc5_menu_top = 0; }
		rc5_code = 0xFF;
      break;
      case KBD_DOWN:
        rc5_menu_cursor++;
        if (rc5_menu_cursor > 3) { rc5_menu_cursor = 3; rc5_menu_top++; }
        if (rc5_menu_top > (RC5_MENU_MAX - 3)) { rc5_menu_top = RC5_MENU_MAX - 3; }
		rc5_code = 0xFF;
      break;
      case KBD_SET:
//	    if (is_rc5_dubl())
//		{
//		  lcd_rc5_dubl();
//		  lcd_rc5_key();
//		} else {
          key_rc5[rc5_menu_cursor + rc5_menu_top] = rc5_code;
		  rc5_code = 0xFF;
//		}
      break;
      case KBD_MENU:
        l_flag = 0;
	  // save EEPROM All
	    save_rc5key();
      break;
    }
	beep();
    kbd_key = KBD_NOKEY; rc5.flip = -1;
    kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
  }
  lcd_rc5_key();
}
save_rc5key();
l_no_rc5 = 1;
lcd_clear();
lcd_option_menu();
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
return 1;
}
//************************************************************************************************
void lcd_rc5_key(void)
{
unsigned char i, n;
n = 4 + rc5_menu_top;
for (i = rc5_menu_top; i < n; i++) {
  lcd_com(menu_rows[i - rc5_menu_top]);
  if ((rc5_menu_cursor + rc5_menu_top) == i) { lcd_dat(MENU_CURSOR); } else { lcd_dat(' '); }
#if (LCD_TYPE == LCD20x4)
  lcd_puts(rc5_menu[i]); lcd_space(2); print_dec(key_rc5[i],2,'0');
  if ((rc5_menu_cursor + rc5_menu_top) == i)
  {
    lcd_dat(':');
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT))
    {
      lcd_space(2);
    } else {
      if (rc5_code == 0xFF) { lcd_dat('?'); lcd_dat('?'); } else { print_dec(rc5_code,2,'0'); }
    }
  } else {
    lcd_space(3);
  }
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_puts(rc5_menu[i]); print_dec(key_rc5[i],2,'0');
  if ((rc5_menu_cursor + rc5_menu_top) == i)
  {
    lcd_dat(':');
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT))
    {
      lcd_space(2);
    } else {
      if (rc5_code == 0xFF) { lcd_dat('?'); lcd_dat('?'); } else { print_dec(rc5_code,2,'0'); }
    }
  } else {
    lcd_space(3);
  }
#endif
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW1 + 19);
  if (rc5_menu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 19);
  if (rc5_menu_top < (RC5_MENU_MAX - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW1 + 15);
  if (rc5_menu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 15);
  if (rc5_menu_top < (RC5_MENU_MAX - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
}
}
//************************************************************************************************
unsigned char is_rc5_dubl(void)
{
unsigned char i1,i2, k, k1, ret = 0;
for(i1 = 0; i1 < RC5_MENU_MAX + 1; i1++)
{
  k = key_rc5[i1];
  for(i2 = 0; i2 < RC5_MENU_MAX + 1; i2++)
  {
    k1 = key_rc5[i2];
	if (i1 != i2) {
	  if (k == k1) { ret = 1; break; i1 = RC5_MENU_MAX + 1; }
	}
  }
}
return ret;
}
//************************************************************************************************
void lcd_rc5_dubl(void)
{
lcd_clear();
beep();
#if (LCD_TYPE == LCD20x4)
lcd_com(LCD_ROW2 + 1); lcd_puts(msg[MSG_RC5_DUBLE]);
#endif
#if (LCD_TYPE == LCD16x4)
lcd_com(LCD_ROW2); lcd_puts(msg[MSG_RC5_DUBLE]);
#endif
main_delay(8);
lcd_clear();
}
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
