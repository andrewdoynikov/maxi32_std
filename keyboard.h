//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//*****************************************************************************
// KBD procedures
void scan_kbd(void);
unsigned char debounce_sw_standby(void);
unsigned char debounce_sw_menu(void);
unsigned char debounce_sw_set(void);
unsigned char debounce_sw_select(void);
unsigned char debounce_sw_up(void);
unsigned char debounce_sw_down(void);
unsigned char debounce_sw_left(void);
unsigned char debounce_sw_right(void);
//*****************************************************************************
void scan_kbd(void)
{
unsigned char key = KBD_NOKEY;
if (kbd_key == KBD_NOKEY)
{
  if (debounce_sw_standby() == 1) { key = KBD_STANDBY; }
  if (debounce_sw_menu() == 1) { key = KBD_MENU; }
  if (debounce_sw_set() == 1) { key = KBD_SET; }
  if (debounce_sw_select() == 1) { key = KBD_SELECT; }
  if (debounce_sw_up() == 1) { key = KBD_UP; }
  if (debounce_sw_down() == 1) { key = KBD_DOWN; }
  if (kbd_type == KBD_ONE_PRESS)
  {
    if (debounce_sw_left() == 1) { key = KBD_LEFT; }
    if (debounce_sw_right() == 1) { key = KBD_RIGHT; }
  } else {
    if (debounce_sw_left() >= 1) { key = KBD_LEFT; }
    if (debounce_sw_right() >= 1) { key = KBD_RIGHT; }
  }
  if (key != KBD_NOKEY)
  {
    kbd_key = key;
    kbd_nokey_flag = 0;
    kbd_nokey_cnt = 0;
  }
}
}
//*****************************************************************************
unsigned char debounce_sw_standby(void)
{
static uint16_t state1 = 0; //holds present state
state1 = (state1 << 1) | (! bit_is_clear(PORT_STANDBY, PIN_STANDBY)) | 0xE000;
if (state1 == 0xF000) return 1;
return 0;
}
//*****************************************************************************
unsigned char debounce_sw_left(void)
{
static uint16_t state2 = 0; //holds present state
state2 = (state2 << 1) | (! bit_is_clear(PORT_LEFT, PIN_LEFT)) | 0xE000;
if (state2 == 0xF000) return 1;
if (state2 == 0xE000) return 2;
return 0;
}
//*****************************************************************************
unsigned char debounce_sw_right(void)
{
static uint16_t state3 = 0; //holds present state
state3 = (state3 << 1) | (! bit_is_clear(PORT_RIGHT, PIN_RIGHT)) | 0xE000;
if (state3 == 0xF000) return 1;
if (state3 == 0xE000) return 2;
return 0;
}
//*****************************************************************************
unsigned char debounce_sw_set(void)
{
static uint16_t state4 = 0; //holds present state
state4 = (state4 << 1) | (! bit_is_clear(PORT_SET, PIN_SET)) | 0xE000;
if (state4 == 0xF000) return 1;
return 0;
}
//*****************************************************************************
unsigned char debounce_sw_menu(void)
{
static uint16_t state5 = 0; //holds present state
state5 = (state5 << 1) | (! bit_is_clear(PORT_MENU, PIN_MENU)) | 0xE000;
if (state5 == 0xF000) return 1;
return 0;
}
//*****************************************************************************
unsigned char debounce_sw_select(void)
{
static uint16_t state6 = 0; //holds present state
state6 = (state6 << 1) | (! bit_is_clear(PORT_SELECT, PIN_SELECT)) | 0xE000;
if (state6 == 0xF000) return 1;
return 0;
}
//*****************************************************************************
unsigned char debounce_sw_up(void)
{
static uint16_t state7 = 0; //holds present state
state7 = (state7 << 1) | (! bit_is_clear(PORT_UP, PIN_UP)) | 0xE000;
if (state7 == 0xF000) return 1;
if (state7 == 0xE000) return 2;
return 0;
}
//*****************************************************************************
unsigned char debounce_sw_down(void)
{
static uint16_t state8 = 0; //holds present state
state8 = (state8 << 1) | (! bit_is_clear(PORT_DOWN, PIN_DOWN)) | 0xE000;
if (state8 == 0xF000) return 1;
if (state8 == 0xE000) return 2;
return 0;
}
//*****************************************************************************
void scan_rc5(void)
{
if (kbd_key == KBD_NOKEY)
{
  if (rc5.flip != -1)
  {
	if (l_no_rc5)
	{
	  kbd_key = rc5_to_kbd(rc5.code);
    } else {
      rc5_code = rc5.code;
	  kbd_key = KBD_RC5;
	}
	if (kbd_key != KBD_NOKEY)
	{
	  flash();
    } else {
	  rc5.flip = -1;
	}
    kbd_nokey_flag = 0;
    kbd_nokey_cnt = 0;
  }
}
}
//************************************************************************************************
unsigned char rc5_to_kbd(unsigned char rc5)
{
unsigned char i, ret;
ret = KBD_NOKEY;
for(i = 0; i < RC5_MENU_MAX + 1; i++)
{
  if (rc5 == key_rc5[i]) { ret = kbdkey_rc5[i]; break; }
}
return ret;
}
//************************************************************************************************
unsigned char get_key(void)
{
if (kbd_key != KBD_NOKEY) { return 1; } else { return 0; }
}
//************************************************************************************************

