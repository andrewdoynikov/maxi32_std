//*****************************************************************************
// Project MAXI32  
// Author: Andrew Doynikov
// 08.03.2011	
//*****************************************************************************
void send_lm7001(unsigned long freq);
void lcd_freq(unsigned long freq);
void print_freq(unsigned long freq);
void send_lm7001_byte(unsigned char byte);
//*****************************************************************************
#define PORT_CE    	         PORTC
#define PIN_CE		         PC7
#define PORT_CL    	         PORTC
#define PIN_CL		         PC6
#define PORT_DATA  	         PORTC
#define PIN_DATA	         PC5
#define HiBYTE(w) (uint8_t)((w >> 8) & 0x00ff)
#define LoBYTE(w) (uint8_t)((w >> 0) & 0x00ff)
#define MINFREQ		     8750		
#define MAXFREQ		    10800		
#define DEFFREQ		    10000		
#define IF		         1070		
#define STEP50	            5	    
#define MAXBANK		        1
#define MAX_CHANEL         19
#define lm7001_en_on() PORT_CE |= _BV(PIN_CE);
#define lm7001_en_off() PORT_CE &= ~(_BV(PIN_CE));
#define lm7001_data_0() PORT_DATA &= ~(_BV(PIN_DATA));
#define lm7001_data_1() PORT_DATA |= _BV(PIN_DATA);
#define lm7001_clk_0() PORT_CL &= ~(_BV(PIN_CL));
#define lm7001_clk_1() PORT_CL |= _BV(PIN_CL);
//*****************************************************************************
unsigned char freq_lo, freq_hi;
unsigned long frequency = 10120, freq_step = STEP50, tmp_frequency;
volatile unsigned char n_chanel = 1, tmp_chanel;
unsigned char n_bank;
//*****************************************************************************
void send_lm7001(unsigned long freq)
{
unsigned long freqs, freq_get;
unsigned char l,h;
freqs = (freq + IF);
//freqs = (freq - IF);
freq_get = freqs / freq_step;
h = HiBYTE(freq_get);
l = LoBYTE(freq_get);
lm7001_en_on(); _delay_ms(1);
send_lm7001_byte(l);
send_lm7001_byte(h);
send_lm7001_byte(0xC0); 
lm7001_en_off(); _delay_ms(1);
if ((freq < MINFREQ) || (freq > MAXFREQ))
{
  send_tda7313_mute();
} else { 
  send_tda7313();
}
}
//*****************************************************************************
void lcd_freq(unsigned long freq)
{
if ((freq < MINFREQ) || (freq > MAXFREQ))
{
  lcd_puts(msg[MSG_FREQ]);
//  send_tda7313_mute();
} else { 
#if (LCD_TYPE == LCD20x4)
  print_freq(freq); 
#endif
#if (LCD_TYPE == LCD16x4)
  print_freq(freq); 
#endif
}
}
//*****************************************************************************
void print_freq(unsigned long freq)
{
unsigned char i;
unsigned char s[6];
for (i = 0; i < 5; i++) {
  s[5 - i - 1] = '0' + (freq % 10);
  freq /= 10;
}
if (s[0] == '0') { s[0] = ' '; }
for (i = 0; i < 5; i++) {
  if (i == 3) { lcd_dat('.'); }
  lcd_dat(s[i]);
}
}
//*****************************************************************************
void send_lm7001_byte(unsigned char byte)
{
unsigned char i;
for(i = 0; i < 8; i++)
{
  if (byte & 0x01) {
    lm7001_data_1();
  } else {
    lm7001_data_0();
  }
  _delay_us(10);
  lm7001_clk_1(); _delay_us(10); lm7001_clk_0(); _delay_us(10);
  byte >>= 1;
}
}
//*****************************************************************************
