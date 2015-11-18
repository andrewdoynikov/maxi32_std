//*****************************************************************************
// Project MAXI32  
// Author: Andrew Doynikov
// 08.03.2011	
//*****************************************************************************
// LCD FUNCTIONS HEADER
//*****************************************************************************
#ifndef _LCD_LIB_H
#define _LCD_LIB_H

#define  LCD16x4      0
#define  LCD20x4      1
#define  LNG_ENG      0
#define  LNG_RUS      1
//#define  LCD_TYPE     LCD16x4
#define  LCD_TYPE     LCD20x4
#define  LNG_TYPE     LNG_RUS
//#define  LNG_TYPE     LNG_ENG
//#define  TEST_TYPE	  1 // (эмуляция 16х4 на 20х4)	
#define  TEST_TYPE  0 // действительно 16х4	
//************************************************************************************************
#if (LCD_TYPE == LCD20x4)
#define LCD_ROW1				0x80
#define LCD_ROW2				0xC0
#define LCD_ROW3				0x94
#define LCD_ROW4				0xD4
#define CNT_BLANK               20
#define LCD_DISP_LENGTH         20
#endif
#if (LCD_TYPE == LCD16x4)
#define LCD_ROW1				0x80
#define LCD_ROW2				0xC0
#if (TEST_TYPE == 1)
#define LCD_ROW3				0x94
#define LCD_ROW4				0xD4
#endif
#if (TEST_TYPE == 0)
#define LCD_ROW3				0x90
#define LCD_ROW4				0xD0
#endif
#define CNT_BLANK               16
#define LCD_DISP_LENGTH         16
#endif
//*****************************************************************************
// define macros
//= Править здесь =============================================================
#define LCD_PORT  PORTA             // port for LCD
#define LCD_PORTU PORTA             // port for LCD
#define LCD_RS    PA5               // pin RS for LCD
#define LCD_EN    PA4               // pin EN for LCD
#define LCD_TIME  1                 // wait for LCD
//=============================================================================
#define LCD_CLR                 0	// clear display
#define LCD_HOME                1	// return to home position
#define LCD_ENTRY_MODE          2	// set entry mode
#define LCD_ENTRY_INC           1	// increment
#define LCD_ENTRY_SHIFT         0	// shift
#define LCD_ON_CTRL             3	// turn lcd/cursor on
#define LCD_ON_DISPLAY          2	// turn display on
#define LCD_ON_CURSOR           1	// turn cursor on
#define LCD_ON_BLINK            0	// blinking cursor
#define LCD_MOVE                4	// move cursor/display
#define LCD_MOVE_DISP           3	// move display (0-> move cursor)
#define LCD_MOVE_RIGHT          2	// move right (0-> left)
#define LCD_FUNCTION            5	// function set
#define LCD_CGRAM               6	// set CG RAM address
#define LCD_DDRAM               7	// set DD RAM address
#define LCD_LINES			    2	// visible lines
#define LCD_LINE_LENGTH		    16	// line length (in characters)
#define PROGRESSPIXELS_PER_CHAR	6
// Functions prototype
void lcd(unsigned char p);
void lcd_init(void);
void lcd_com(unsigned char p);
void lcd_dat(unsigned char p);
void lcd_puts(unsigned char *data);
void lcd_clear(void);
void lcd_home(void);
void lcd_cursor_ON(void); 
void lcd_cursor_ON_blink(void);
void lcd_cursor_OFF(void);
void lcd_definechar(const uint8_t *pc,uint8_t char_code);
void lcd_space(unsigned char n_sp);
void lcd_char(unsigned char n_sp);
void lcdprogress_bar(uint8_t progress, uint8_t maxprogress, uint8_t length);
void lcd_load_defchar(void);
void lcd_load_bignum(void);
void lcd_gotoxy(unsigned char x, unsigned char y);
//=============================================================================
//=============================================================================
#endif // _LCD_LIB_H

