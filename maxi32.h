//************************************************************************************************
// Project MAXI32 TIMER SERVICE  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
#define  LNG_ENG      0
#define  LNG_RUS      1
#define  LNG_TYPE     LNG_RUS
#define  LOGO_TIME	  4	
//************************************************************************************************
//объявляем программные таймеры
SoftTimer kbd_timer;
SoftTimer adc_timer;
SoftTimer encoder_timer;
SoftTimer rc5_timer;
SoftTimer nokey_timer;
SoftTimer ds18x20_timer;
void lcd_bigtime(void);
void lcd_info(void);
void lcd_bigchar(unsigned char col, unsigned char x);
//************************************************************************************************
//определяем прототипы функции
//************************************************************************************************
void init_port(void);
void init_hardware(void);
void init_main(void);
void main_delay(unsigned char i);
void beep(void);
void flash(void);
void print_dec(unsigned int x, unsigned char n, unsigned char fillch);
void print_hex(unsigned char i);
void scan_nokey(void);
void scan_ds18x20(void);
int dayofweek(int day, int month, int year);
//************************************************************************************************
#define KBD_NOKEY        	0xFF
#define KBD_ONE_PRESS 	    1
#define KBD_AUTO_PRESS  	0
// KBD defines Key code
#define KBD_STANDBY    	 	10
#define KBD_MENU   	     	11
#define KBD_SET        	 	12
#define KBD_SELECT     	 	13
#define KBD_UP     	     	14
#define KBD_DOWN      	 	15
#define KBD_LEFT       	 	16
#define KBD_RIGHT      	 	17
#define KBD_1		     	21
#define KBD_2		     	22
#define KBD_3		     	23
#define KBD_4	    	 	24
#define KBD_5	  	     	25
#define KBD_6		     	26
#define KBD_7		     	27
#define KBD_8		     	28
#define KBD_9		     	29
#define KBD_BANK	     	30
#define KBD_BASS_UP    	 	40
#define KBD_BASS_DOWN  	 	41
#define KBD_TREBLE_UP    	42
#define KBD_TREBLE_DOWN  	43
#define KBD_LONDNESS     	44
#define KBD_MUTE         	45
#define KBD_EXT_KEY_START 	20
#define KBD_NOKEY_TIME_MIN 	5
#define KBD_NOKEY_TIME_MAX 	60
#define KBD_NOKEY_TIME_DEF 	15
#define KBD_RC5			 	250
#define KBD_EXT_KEY		 	251
//************************************************************************************************
// Key port define
#define PORT_STANDBY   	  PINB
#define PIN_STANDBY       PB5
#define PORT_MENU         PINB
#define PIN_MENU          PB7
#define PORT_SET          PINB
#define PIN_SET           PB6
#define PORT_SELECT       PINC
#define PIN_SELECT        PC3
#define PORT_UP           PIND
#define PIN_UP            PD4
#define PORT_DOWN         PIND
#define PIN_DOWN          PD5
#define PORT_LEFT         PINC
#define PIN_LEFT          PC2
#define PORT_RIGHT        PIND
#define PIN_RIGHT         PD6
//************************************************************************************************
#define BEEPLONG  		  50
#define BEEP       		  20
#define PORT_BEEP	      PORTD
#define PIN_BEEP	      PD7
#define PORT_LED_RC5   	  PORTB
#define LED_RC5		      PB1
#define PORT_LED_STANDBY  PORTB
#define LED_STANDBY	      PB0
#define PORT_STANDBY_OUT  PORTB
#define PIN_STANDBY_OUT   PB2
#define PORT_LIGHT_OUT    PORTC
#define PIN_LIGHT_OUT     PC4
#define PORT_AC_WAIT      PORTB
#define PIN_AC_WAIT       PB3
#define PORT_DS_OUT       PORTD
#define PIN_DS_OUT        PD1
//************************************************************************************************
#define DS_MAX_TEMP     	75
#define DS_MIN_TEMP     	35
#define NO_DS_FOPUND_TEMP   0xFF
//************************************************************************************************
#if (LCD_TYPE == LCD20x4)
char bignumchars1[] = {0x03,0x00,0x03,0x20, 0x00,0x03,0x20,0x20, 0x02,0x02,0x03,0x20, 0x00,0x02,0x03,0x20, 0x03,0x01,0x03,0x20,
                       0x03,0x02,0x02,0x20, 0x03,0x02,0x02,0x20, 0x00,0x00,0x03,0x20, 0x03,0x02,0x03,0x20, 0x03,0x02,0x03,0x20};
char bignumchars2[] = {0x03,0x01,0x03,0x20, 0x01,0x03,0x01,0x20, 0x03,0x01,0x01,0x20, 0x01,0x01,0x03,0x20, 0x20,0x20,0x03,0x20,
                       0x01,0x01,0x03,0x20, 0x03,0x01,0x03,0x20, 0x20,0x20,0x03,0x20, 0x03,0x01,0x03,0x20, 0x01,0x01,0x03,0x20};
#endif
#if (LCD_TYPE == LCD16x4)
char bignumchars1[] = {0x03,0x00,0x03, 0x00,0x03,0x20, 0x02,0x02,0x03, 0x00,0x02,0x03, 0x03,0x01,0x03,
                       0x03,0x02,0x02, 0x03,0x02,0x02, 0x00,0x00,0x03, 0x03,0x02,0x03, 0x03,0x02,0x03};
char bignumchars2[] = {0x03,0x01,0x03, 0x01,0x03,0x01, 0x03,0x01,0x01, 0x01,0x01,0x03, 0x20,0x20,0x03,
                       0x01,0x01,0x03, 0x03,0x01,0x03, 0x20,0x20,0x03, 0x03,0x01,0x03, 0x01,0x01,0x03};
#endif
//************************************************************************************************
unsigned long freq_fm[] = {10120,10000,10040,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0};
//************************************************************************************************
unsigned char kbdkey_rc5[] = {
KBD_STANDBY, KBD_SET, KBD_MUTE, KBD_UP, KBD_DOWN, KBD_LEFT, KBD_RIGHT, KBD_MENU, KBD_LONDNESS, KBD_SELECT,
KBD_1, KBD_2, KBD_3, KBD_4, KBD_5, KBD_6, KBD_7, KBD_8, KBD_9, KBD_BANK, KBD_BASS_UP, KBD_BASS_DOWN,
KBD_TREBLE_UP, KBD_TREBLE_DOWN };
unsigned char key_rc5[] = { 12, 59, 13, 32, 33, 17, 16, 48, 44, 14, 1, 2, 3, 4, 5, 6, 7, 8, 9, 55, 44, 54, 43, 24 };
unsigned char rc5_to_kbd(unsigned char rc5);
unsigned char n_select = 2, save_volume;
volatile unsigned char alarm_param[] = {0,1,2,0,2,4,0,0};
volatile unsigned char alarm_posle_flag = 0;
volatile unsigned char l_alarm = 0;
volatile unsigned char alarm_regim = 0;
//************************************************************************************************
// Макросы функций
#define light_on()  PORT_LIGHT_OUT |= _BV(PIN_LIGHT_OUT); light_cnt = 0;
#define light_off() if (light_on_off) { PORT_LIGHT_OUT |= _BV(PIN_LIGHT_OUT); } else { PORT_LIGHT_OUT &= ~(_BV(PIN_LIGHT_OUT)); }  light_cnt = 0;
#define light_stb() PORT_LIGHT_OUT &= ~(_BV(PIN_LIGHT_OUT)); 
#define led_standby_on()  PORT_LED_STANDBY &= ~(_BV(LED_STANDBY));
#define led_standby_off() PORT_LED_STANDBY |= _BV(LED_STANDBY);
#define ac_wait_on()  PORT_AC_WAIT |= _BV(PIN_AC_WAIT); _delay_ms(100);
#define ac_wait_off() PORT_AC_WAIT &= ~(_BV(PIN_AC_WAIT)); _delay_ms(100);
#define standby_out_off() if (standby_out) { PORT_STANDBY_OUT |= _BV(PIN_STANDBY_OUT); } else { PORT_STANDBY_OUT &= ~(_BV(PIN_STANDBY_OUT)); }
#define standby_out_on()  if (standby_out) { PORT_STANDBY_OUT &= ~(_BV(PIN_STANDBY_OUT)); } else { PORT_STANDBY_OUT |= _BV(PIN_STANDBY_OUT); }
#define ds_temp_on()  PORT_DS_OUT |= _BV(PIN_DS_OUT);
#define ds_temp_off() PORT_DS_OUT &= ~(_BV(PIN_DS_OUT));
#define AC_WAIT_MIN	1
#define AC_WAIT_MAX 10

//************************************************************************************************
// Глобальные переменные
volatile unsigned char kbd_key = KBD_NOKEY, kbd_nokey_cnt = 0, kbd_nokey = 10, kbd_nokey_flag = 0;
unsigned char menu_rows[] = { LCD_ROW1,LCD_ROW2,LCD_ROW3,LCD_ROW4 };
unsigned char rc5_system = 0xFF;
volatile unsigned char kbd_type = KBD_AUTO_PRESS;
volatile unsigned char ac_wait_sec = 2, ac_wait_cnt = 0;
volatile unsigned char l_standby = 1, l_mute = 0, l_londness = 0, standby_out = 0;
volatile unsigned char ds_state = 0, ds_count = 0, ds_temp_max = DS_MAX_TEMP;
volatile unsigned int ds_cnt = 0;
volatile unsigned char light_on_off = 0, light_flag = 1;
signed int temperature1 = 0, temperature2 = 0;
unsigned char data1[8], data2[8];
unsigned char owDevicesIDs[MAXDEVICES][8];
unsigned char search_ow_devices(void); 

//************************************************************************************************
//************************************************************************************************
void send_tda7313(void);
void send_tda7313_mute(void);
void send_tda_freq(void);
void step_tda7313_vol(unsigned char n_up_down);
unsigned char check_alarm(void);
//*****************************************************************************
#define STEP_UP		1
#define STEP_DOWN	0
#define MENU_CURSOR '>'
//************************************************************************************************
#define MENU_UP     0xD9
#define MENU_DOWN   0xDA
//************************************************************************************************
void ac_wait(void);
unsigned char run_standby(unsigned char key);
unsigned char run_mute(unsigned char key);
unsigned char run_fmradio(unsigned char key);
unsigned char run_mainmenu(unsigned char key);
unsigned char run_nokey(unsigned char key);
void lcd_main_menu(void);
unsigned char run_option_menu(unsigned char key);
void lcd_option_menu(void);
void lcd_option(unsigned char noption);
//************************************************************************************************
unsigned char is_rc5_dubl(void);
void lcd_rc5_dubl(void);
//************************************************************************************************
// Main menu
#define BLINK_ON	 1
#define BLINK_OFF	 0
#define DEF_TIME_H   0
#define DEF_TIME_M   1
#define DEF_DATE_D   0
#define DEF_DATE_M   1
#define DEF_DATE_Y   2
volatile unsigned char blink_flag = BLINK_OFF;
signed char mmenu, mmenu_cursor = 0, mmenu_top = 0;
signed char omenu, omenu_cursor = 0, omenu_top = 0;
signed char fm_menu_cursor = 0, fm_menu_top = 0;
signed char rc5_menu_cursor = 0, rc5_menu_top = 0;
unsigned char rc5_code = 0xFF, l_no_rc5 = 1;
signed char ext_menu_cursor = 0, ext_menu_top = 0;
volatile unsigned long ext_code = 999, ext_key_no = 0;
volatile unsigned char l_no_ext = 0;
void run_edit_procedure(unsigned char nopt);
unsigned char edit_option_num(unsigned char val, unsigned char min, unsigned char max);
unsigned char edit_option_log(unsigned char val);
void edit_time(unsigned char row);
void edit_time_alarm(unsigned char row);
unsigned char edit_date(unsigned char row);
void save_eeprom_param(unsigned char param);
unsigned char edit_fm_station(void);
void lcd_fm_station(void);
unsigned char edit_rc5_key(void);
void lcd_rc5_key(void);
unsigned char edit_ext_key(void);
void lcd_ext_key(void);
volatile unsigned char light_cnt = 0;
void check_light(void);
void lcd_alarm(void);
void edit_alarm(void);
void save_alarm_eeprom(void);
void load_alarm_eeprom(void);
signed char alarm_menu_cursor = 0, alarm_menu_top = 0;
void run_edit_proc_alarm(unsigned char n_proc);
void lcd_option_alarm(unsigned char n_param);
//************************************************************************************************
// EEprom
//************************************************************************************************
// TDA7313
#define TDA7313_ADDR_START_1   0x10
#define TDA7313_ADDR_START_2   0x20
#define TDA7313_ADDR_START_3   0x30
#define TDA7313_ADDR_SELECT    0x3C
#define TDA7313_ADDR_SAVE      0x3D
#define TDA7313_ADDR_LOUD      0x3E
void eeprom_write_tda7313(unsigned char nsel);
void eeprom_read_tda7313(unsigned char nsel);
void eeprom_write_tda7313_all(void);
void eeprom_read_tda7313_all(void);
void eeprom_write_tda7313_cursor(unsigned char param);
//************************************************************************************************
#define LM7001_ADDR_START      0x40
#define LM7001_ADDR_CHANEL     0x62
#define LM7001_ADDR_BANK       0x63
//*****************************************************************************
#define save_chanel() eeprom_write_byte((uint8_t*)LM7001_ADDR_CHANEL, n_chanel);
#define load_chanel() n_chanel = eeprom_read_byte((uint8_t*)LM7001_ADDR_CHANEL);
#define save_bank() eeprom_write_byte((uint8_t*)LM7001_ADDR_BANK, n_bank);
#define load_bank() n_bank = eeprom_read_byte((uint8_t*)LM7001_ADDR_BANK);
void save_chanel_freq(unsigned long freqs,unsigned char chl);
void load_chanel_freq(unsigned char chl);
void load_chanel_freq_all(void);
void save_chanel_freq_all(void);
//************************************************************************************************
#define RC5_DEFINE          1
#define RC5_ADDR_START      0x70
#define RC5_PARAM_SAVE     0x6F
//*****************************************************************************
void save_rc5key(void);
void load_rc5key(void);
//************************************************************************************************
#define EXTKEY_ADDR_START   0x200
#define EXTKEY_ADDR_SAVE    0x9F
//*****************************************************************************
void save_extkey_volt(unsigned long volt,unsigned char key);
void load_extkey_volt(unsigned char key);
void load_extkey_volt_all(void);
void save_extkey_volt_all(void);
//************************************************************************************************
#define AC_WAIT_DEF     1
#define RC5_SYSTEM_MIN  0
#define RC5_SYSTEM_MAX  128
#define RC5_SYSTEM_DEF  0xFF
//*****************************************************************************
#define PARAM_ADDR_START      0x110
#define PARAM_ADDR_MAX_TEMP   PARAM_ADDR_START
#define PARAM_ADDR_NOKEY_TIME PARAM_ADDR_START + 1
#define PARAM_ADDR_STB_OUT    PARAM_ADDR_START + 2
#define PARAM_ADDR_AC_WAIT    PARAM_ADDR_START + 3
#define PARAM_ADDR_RC5_SYSTEM PARAM_ADDR_START + 4
#define PARAM_ADDR_LIGHT_LCD  PARAM_ADDR_START + 5
//*****************************************************************************
void load_param_all(void);
void save_param_all(void);
//************************************************************************************************
#define save_max_temp() 	eeprom_write_byte((uint8_t*)PARAM_ADDR_MAX_TEMP, ds_temp_max)
#define save_nokey()		eeprom_write_byte((uint8_t*)PARAM_ADDR_NOKEY_TIME, kbd_nokey);
#define save_stb_out()		eeprom_write_byte((uint8_t*)PARAM_ADDR_STB_OUT, standby_out);
#define save_ac_wait()		eeprom_write_byte((uint8_t*)PARAM_ADDR_AC_WAIT, ac_wait_sec);
#define save_rc5_system()	eeprom_write_byte((uint8_t*)PARAM_ADDR_RC5_SYSTEM, rc5_system);
#define save_light()		eeprom_write_byte((uint8_t*)PARAM_ADDR_LIGHT_LCD, light_on_off);
//************************************************************************************************
#define PARAM_ADDR_ALARM    0x120
#define PARAM_ALARM_ON_OFF  PARAM_ADDR_ALARM
#define PARAM_ALARM_H       PARAM_ADDR_ALARM + 1
#define PARAM_ALARM_M       PARAM_ADDR_ALARM + 2
#define PARAM_ALARM_TYPE    PARAM_ADDR_ALARM + 3
#define PARAM_ALARM_INPUT   PARAM_ADDR_ALARM + 4
#define PARAM_ALARM_MINUT   PARAM_ADDR_ALARM + 5
#define PARAM_ALARM_POSLE   PARAM_ADDR_ALARM + 6
#define PARAM_ALARM_BUD     PARAM_ADDR_ALARM + 7
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************



