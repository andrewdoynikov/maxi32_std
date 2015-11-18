//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
void main_delay(unsigned char i)
{
unsigned char n;
for (n = 0; n < i; n++) {
  _delay_ms(250);
}
}
//************************************************************************************************
void beep(void)
{
PORT_BEEP &= ~(_BV(PIN_BEEP));
_delay_ms(BEEP);
PORT_BEEP |= _BV(PIN_BEEP);
}
//************************************************************************************************
void flash(void)
{
PORT_LED_RC5 &= ~(_BV(LED_RC5));
_delay_ms(5);
PORT_LED_RC5 |= _BV(LED_RC5);
}
//************************************************************************************************
void print_dec(unsigned int x, unsigned char n, unsigned char fillch)
{
unsigned char i;
unsigned char s[10];
for (i = 0; i < n; i++) {
  s[n - i - 1] = '0' + (x % 10);
  x /= 10;
}
for (i=0; i<(n - 1); i++) {
  if (s[i] == '0') s[i] = fillch; else break;
}
  for (i=0; i<n; i++) lcd_dat(s[i]);
}
//************************************************************************************************
void print_hex(unsigned char i)
{
unsigned char hi,lo;
hi = i & 0xF0;
hi = hi >> 4;
hi = hi + '0';
if (hi >'9') hi = hi + 7;
lo = (i & 0x0F) + '0';
if (lo > '9') lo = lo + 7;
lcd_dat(hi); lcd_dat(lo);
}
//************************************************************************************************
void print_temperatura(unsigned char nd)
{
switch (nd) // switch
{ 
  case 1:
  if (temperature1 == NO_DS_FOPUND_TEMP)
  {
    lcd_dat('-');lcd_dat('-');lcd_dat('-'); 
  } else {
    if (temperature1 < 0) { lcd_dat('-'); } else { lcd_dat('+'); }
    print_dec(temperature1,2,'0'); 
  }
  break;
  case 2:
  if (temperature2 == NO_DS_FOPUND_TEMP)
  {
    lcd_dat('-');lcd_dat('-');lcd_dat('-');
  } else {
    if (temperature2 < 0) { lcd_dat('-'); } else { lcd_dat('+'); }
    print_dec(temperature2,2,'0');
  }
  break;
}
}
//************************************************************************************************
#if (LCD_TYPE == LCD20x4)
void lcd_bigchar(unsigned char col, unsigned char x)
{
unsigned char i;
i = x * 4;
lcd_com(LCD_ROW2 + col);
lcd_dat(bignumchars1[i]); lcd_dat(bignumchars1[i + 1]); lcd_dat(bignumchars1[i + 2]); lcd_dat(bignumchars1[i + 3]);
lcd_com(LCD_ROW3 + col);
lcd_dat(bignumchars2[i]); lcd_dat(bignumchars2[i + 1]); lcd_dat(bignumchars2[i + 2]); lcd_dat(bignumchars2[i + 3]);
}
#endif
#if (LCD_TYPE == LCD16x4)
void lcd_bigchar(unsigned char col, unsigned char x)
{
unsigned char i;
i = x * 3;
lcd_com(LCD_ROW2 + col);
lcd_dat(bignumchars1[i]); lcd_dat(bignumchars1[i + 1]); lcd_dat(bignumchars1[i + 2]);
lcd_com(LCD_ROW3 + col);
lcd_dat(bignumchars2[i]); lcd_dat(bignumchars2[i + 1]); lcd_dat(bignumchars2[i + 2]);
}
#endif
//************************************************************************************************
#if (LCD_TYPE == LCD20x4)
void lcd_bigtime(void)
{
unsigned char h,m,s;
lcd_com(LCD_ROW2 + 19); lcd_dat(0x20);
lcd_com(LCD_ROW3 + 19); lcd_dat(0x20);
rtc_get_time(&h,&m,&s);
if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT))
{ 
  lcd_com(LCD_ROW2 + 9); lcd_dat(0x04); lcd_dat(0x20);
  lcd_com(LCD_ROW3 + 9); lcd_dat(0x04); lcd_dat(0x20);
} else {
  lcd_com(LCD_ROW2 + 9); lcd_dat(0x20); lcd_dat(0x20);
  lcd_com(LCD_ROW3 + 9); lcd_dat(0x20); lcd_dat(0x20);
}
lcd_bigchar(1, h / 10); lcd_bigchar(5, h % 10); lcd_bigchar(11, m / 10); lcd_bigchar(15, m % 10);
}
#endif
//************************************************************************************************
#if (LCD_TYPE == LCD16x4)
void lcd_bigtime(void)
{
unsigned char h,m,s;
rtc_get_time(&h,&m,&s);
lcd_com(LCD_ROW2 + 3); lcd_dat(0x20);
lcd_com(LCD_ROW3 + 3); lcd_dat(0x20);
lcd_com(LCD_ROW2 + 7); lcd_dat(0x20);
lcd_com(LCD_ROW3 + 7); lcd_dat(0x20);
lcd_com(LCD_ROW2 + 12); lcd_dat(0x20);
lcd_com(LCD_ROW3 + 12); lcd_dat(0x20);
if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT))
{ 
  lcd_com(LCD_ROW2 + 8); lcd_dat(0x04);
  lcd_com(LCD_ROW3 + 8); lcd_dat(0x04);
} else {
  lcd_com(LCD_ROW2 + 8); lcd_dat(0x20);
  lcd_com(LCD_ROW3 + 8); lcd_dat(0x20);
}
lcd_bigchar(0, h / 10); lcd_bigchar(4, h % 10); lcd_bigchar(9, m / 10); lcd_bigchar(13, m % 10);
}
#endif
//************************************************************************************************
void lcd_info(void)
{
unsigned char d,m,y;
rtc_get_date(&d,&m,&y);
lcd_bigtime();
#if (LCD_TYPE == LCD20x4)
if (!l_standby)
{
  lcd_com(LCD_ROW4); print_temperatura(1); 
  lcd_com(LCD_ROW4 + 17); print_temperatura(2); 
  lcd_com(LCD_ROW4 + 5); print_dec(d,2,'0'); lcd_dat('/'); print_dec(m,2,'0'); lcd_dat('/'); print_dec(y,2,'0');
  lcd_dat(' '); lcd_puts(dw[dayofweek(d,m,y)]);
  lcd_com(LCD_ROW1 + 18); if (l_londness) { lcd_dat('*'); } else { lcd_dat(' '); }
  if (alarm_param[ALARM_ON_OFF])
  {
    if (alarm_posle_flag)
	{
	  if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { lcd_com(LCD_ROW1 + 19); lcd_dat(0xED); } else { lcd_dat(' '); } 
	} else {
      lcd_com(LCD_ROW1 + 19); lcd_dat(0xED); 
    }
  } else { lcd_dat(' '); }
} else {
  lcd_com(LCD_ROW4 + 4); print_dec(d,2,'0'); lcd_dat('/'); print_dec(m,2,'0'); lcd_dat('/'); print_dec(20,2,'0'); print_dec(y,2,'0');
  lcd_dat(' '); lcd_puts(dw[dayofweek(d,m,y)]);
  if (alarm_param[ALARM_ON_OFF])
  {
    if (alarm_posle_flag)
	{
	  if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { lcd_com(LCD_ROW1 + 19); lcd_dat(0xED); } else { lcd_dat(' '); } 
	} else {
      lcd_com(LCD_ROW1 + 19); lcd_dat(0xED); 
    }
  } else { lcd_com(LCD_ROW1 + 19); lcd_dat(' '); }
}
#endif
#if (LCD_TYPE == LCD16x4)
if (!l_standby)
{
  lcd_com(LCD_ROW4); print_temperatura(1); 
  lcd_com(LCD_ROW4 + 13); print_temperatura(2); 
  lcd_com(LCD_ROW4 + 4); print_dec(d,2,'0'); lcd_dat('/'); print_dec(m,2,'0'); lcd_dat('/'); print_dec(y,2,'0');
  lcd_com(LCD_ROW1 + 14); if (l_londness) { lcd_dat('*'); } else { lcd_dat(' '); }
  if (alarm_param[ALARM_ON_OFF])
  {
    if (alarm_posle_flag)
	{
	  if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { lcd_com(LCD_ROW1 + 15); lcd_dat(0xED); } else { lcd_dat(' '); } 
	} else {
      lcd_com(LCD_ROW1 + 15); lcd_dat(0xED); 
    }
  } else { lcd_com(LCD_ROW1 + 15); lcd_dat(' '); }
} else {
  lcd_com(LCD_ROW4 + 3); print_dec(d,2,'0'); lcd_dat('/'); print_dec(m,2,'0'); lcd_dat('/'); print_dec(y,2,'0');
  lcd_dat(' '); lcd_puts(dw[dayofweek(d,m,y)]);
  if (alarm_param[ALARM_ON_OFF])
  {
    if (alarm_posle_flag)
	{
	  if (bit_is_clear(DS1307_PORT_OUT, DS1307_PIN_OUT)) { lcd_com(LCD_ROW1 + 15); lcd_dat(0xED); } else { lcd_dat(' '); } 
	} else {
      lcd_com(LCD_ROW1 + 15); lcd_dat(0xED); 
    }
  } else { lcd_dat(' '); }
}
#endif
if (((l_mute) || (kbd_nokey_flag == 1)) && (!l_standby))
{
  lcd_com(LCD_ROW1);
  if (l_mute) 
  {
    if (bit_is_set(DS1307_PORT_OUT, DS1307_PIN_OUT))
	{ 
	  lcd_space(3);
	} else { lcd_puts(select[n_select]); }
  } else {
    lcd_puts(select[n_select]);
  }
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
}
}
//************************************************************************************************

