//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
unsigned char edit_ext_key(void)
{
unsigned char l_flag = 1;
ext_menu_cursor = 0; ext_menu_top = 0;
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
lcd_clear();
lcd_ext_key();
l_no_ext = 1;
ext_code = 999;
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
        if (ext_menu_cursor > 0) { ext_menu_cursor--; } else { ext_menu_cursor = 0; ext_menu_top--; }
        if (ext_menu_top < 0) { ext_menu_top = 0; }
		ext_code = 999;
      break;
      case KBD_DOWN:
        ext_menu_cursor++;
        if (ext_menu_cursor > 3) { ext_menu_cursor = 3; ext_menu_top++; }
        if (ext_menu_top > (EXT_MENU_MAX - 3)) { ext_menu_top = EXT_MENU_MAX - 3; }
		ext_code = 999;
      break;
      case KBD_SET:
        extkey_volts[ext_menu_cursor + ext_menu_top] = ext_code;
		ext_code = 999;
      break;
      case KBD_MENU:
        l_flag = 0;
	  // save EEPROM All
        save_extkey_volt_all();
      break;
    }
	beep();
    kbd_key = KBD_NOKEY; rc5.flip = -1;
    kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
  }
  lcd_ext_key();
}
//save_extkey_volt_all();
l_no_ext = 0;
lcd_clear();
lcd_option_menu();
kbd_key = KBD_NOKEY; rc5.flip = -1;
kbd_nokey_flag = 0; kbd_nokey_cnt = 0;
return 1;
}
//************************************************************************************************
void lcd_ext_key(void)
{
unsigned char i, n;
n = 4 + ext_menu_top;
for (i = ext_menu_top; i < n; i++) {
  lcd_com(menu_rows[i - ext_menu_top]);
  if ((ext_menu_cursor + ext_menu_top) == i) { lcd_dat(MENU_CURSOR); } else { lcd_dat(' '); }
#if (LCD_TYPE == LCD20x4)
  if (i == EXT_MENU_MAX)
  {
    lcd_puts(ext_menu[i]);  lcd_space(6);
  } else {
    lcd_puts(msg[MSG_KNOPKA]); print_dec(i + 1,1,'0'); lcd_space(3);
  }
  print_dec(extkey_volts[i],3,'0');  
  if ((ext_menu_cursor + ext_menu_top) == i)
  {
    lcd_dat(':');
    if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT))
    {
      lcd_space(3);
    } else {
      if (ext_code == 999) { lcd_dat('?'); lcd_dat('?'); lcd_dat('?'); } else { print_dec(ext_code,3,'0'); }
    }
  } else {
    lcd_space(4);
  }
#endif
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW1 + 19);
  if (ext_menu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 19);
  if (ext_menu_top < (EXT_MENU_MAX - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW1 + 15);
  if (ext_menu_top > 0) { lcd_dat(MENU_UP); } else { lcd_dat(' '); }
  lcd_com(LCD_ROW4 + 15);
  if (ext_menu_top < (EXT_MENU_MAX - 3)) { lcd_dat(MENU_DOWN); } else { lcd_dat(' '); }
#endif
}
}
