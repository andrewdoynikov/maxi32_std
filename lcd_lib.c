//*****************************************************************************
// Project MAXI32  
// Author: Andrew Doynikov
// 15.08.2012	
//*****************************************************************************
// LCD FUNCTIONS
//*****************************************************************************
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <inttypes.h>
#include "lcd_lib.h"
//************************************************************************************************
// win1251 - LCD translation table ///////////
const unsigned char TransTable[] =
{ 'A',0xA0,'B',0xA1,0xE0,'E',0xA3,0xA4,0xA5,0xA6,'K',0xA7,'M','H','O',0xA8,'P','C','T',0xA9,0xAA,'X',0xE1,0xAB,
0xAC,0xD9,0xDA,0xAE,'b',0xAF,0xB0,0xB1,'a',0xB2,0xB3,0xB4,0xE3,'e',0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,'o',0xBE,
'p','c',0xBF,'y',0xE4,'x',0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7 };
//************************************************************************************************
const uint8_t LcdCustomChar[] PROGMEM = //define 8 custom LCD chars
{
	0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, // 0. 0/5 full progress block
	0x10, 0x10, 0x10, 0x15, 0x10, 0x10, 0x10, 0x00, // 1. 1/5 full progress block
	0x18, 0x18, 0x18, 0x1D, 0x18, 0x18, 0x18, 0x00, // 2. 2/5 full progress block
	0x1C, 0x1C, 0x1C, 0x1D, 0x1C, 0x1C, 0x1C, 0x00, // 3. 3/5 full progress block
	0x1E, 0x1E, 0x1E, 0x1F, 0x1E, 0x1E, 0x1E, 0x00, // 4. 4/5 full progress block
	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // 5. 5/5 full progress block
	0x0E, 0x15, 0x15, 0x17, 0x11, 0x11, 0x0E, 0x00, // 6. clock
	0x03, 0x05, 0x19, 0x11, 0x19, 0x05, 0x03, 0x00  // 5. dinamik fill
};
//=============================================================================
const uint8_t LcdBigNum[] PROGMEM = //define 8 custom LCD chars
{
	0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,
	0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,
	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
#if (LCD_TYPE == LCD20x4)
	0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x1C, 0x00,
#endif
#if (LCD_TYPE == LCD16x4)
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00,
#endif
	0x1F, 0x1B, 0x13, 0x1B, 0x1B, 0x1B, 0x1F, 0x00,
	0x1F, 0x11, 0x1D, 0x11, 0x17, 0x11, 0x1F, 0x00,
	0x1F, 0x11, 0x1D, 0x11, 0x1D, 0x11, 0x1F, 0x00
};
//=============================================================================
void lcd(unsigned char p)
{ 
  LCD_PORT &= 0xF0; LCD_PORT |= (p >> 4);
  _delay_us(100);
  LCD_PORT &= ~_BV(LCD_EN);
  _delay_us(100);
  LCD_PORT |= _BV(LCD_EN);
  LCD_PORT &= 0xF0; LCD_PORT |= (p & 0x0F);
  _delay_us(100);
  LCD_PORT &= ~_BV(LCD_EN);
  _delay_us(100);
}                
//=============================================================================
void lcd_com(unsigned char p)
{
  LCD_PORT &= ~_BV(LCD_RS);
  LCD_PORT |= _BV(LCD_EN);
  lcd(p);
}
//=============================================================================
void lcd_dat(unsigned char p)
{
  LCD_PORT |= _BV(LCD_RS) | _BV(LCD_EN);
  if ((p > 0x80) && (p != 0xED)) {
    p = TransTable[(p & 0x7F) - 0x40];
  }
  lcd(p);
}
//=============================================================================
void lcd_init(void)
{
unsigned char ch = 0, chn = 0;
_delay_ms(200);
lcd_com(0x33);
_delay_ms(20);
lcd_com(0x32);
lcd_com(0x28);
lcd_com(0x08);
lcd_com(0x01);
_delay_ms(50);
lcd_com(0x06);
lcd_com(0x0C);
while(ch < 64)
{
  lcd_definechar((LcdCustomChar + ch),chn++);
  _delay_ms(1);
  ch = ch + 8;
}
lcd_clear();
}
//=============================================================================
void lcd_load_bignum(void)
{
unsigned char ch = 0, chn = 0;
while(ch < 64)
{
  lcd_definechar((LcdBigNum + ch),chn++);
  _delay_ms(1);
  ch = ch + 8;
}
}
//=============================================================================
void lcd_load_defchar(void)
{
unsigned char ch = 0, chn = 0;
while(ch < 64)
{
  lcd_definechar((LcdCustomChar + ch),chn++);
  _delay_ms(1);
  ch = ch + 8;
}
}
//=============================================================================
void lcd_puts(unsigned char *data)
{
  while(*data) lcd_dat(*(data++));
}
//=============================================================================
void lcd_clear(void)
{
  lcd_com(1<<LCD_CLR);
  _delay_ms(200);
}
//=============================================================================
void lcd_definechar(const uint8_t *pc,uint8_t char_code)
{
  uint8_t a, pcc;
  uint16_t i;
  a = (char_code << 3) | 0x40;
  for (i = 0; i < 8; i++) {
    pcc = pgm_read_byte(&pc[i]);
	lcd_com(a++); lcd_dat(pcc);
  }
}
//=============================================================================
void lcdprogress_bar(uint8_t progress, uint8_t maxprogress, uint8_t length)
{
uint8_t i;
uint16_t pixelprogress;
uint8_t c;
pixelprogress = ((progress * (length * PROGRESSPIXELS_PER_CHAR)) / maxprogress);
for(i=0; i<length; i++)
{
  if( ((i * (uint16_t)PROGRESSPIXELS_PER_CHAR) + 5) > pixelprogress )
  {
    if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)) > pixelprogress )
    {
      c = 0;
    } else {
      c = pixelprogress % PROGRESSPIXELS_PER_CHAR;
    }
  } else {
    c = 5;
  }
  lcd_dat(c);
}

}
//=============================================================================
void lcd_space(unsigned char n_sp)
{
unsigned char i;
for (i = 1; i <= n_sp; i++) {
  lcd_dat(' ');
}
}
//=============================================================================
void lcd_char(unsigned char n_sp)
{
unsigned char i;
for (i = 1; i <= n_sp; i++) {
  lcd_dat(0xFF);
}
}
//=============================================================================
void lcd_gotoxy(unsigned char x, unsigned char y)
{
unsigned char i;
if (x > LCD_DISP_LENGTH) { x = LCD_DISP_LENGTH; }
switch (y)
{
  case 0: i = LCD_ROW1; break;
  case 1: i = LCD_ROW1; break;
  case 2: i = LCD_ROW2; break;
  case 3: i = LCD_ROW3; break;
  case 4: i = LCD_ROW4; break;
  default: i = LCD_ROW1;break;
}
lcd_com(i + x);
}
//************************************************************************************************
//=============================================================================
// END
//=============================================================================
