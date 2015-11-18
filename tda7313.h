//*****************************************************************************
// Project MAXI32  
// Author: Andrew Doynikov
// 08.03.2011	
//*****************************************************************************
#define TDA7313_ADDR        0x88
#define TDA7313_VOLUME_REG  0x00
#define TDA7313_ATT_LR_REG  0xC0
#define TDA7313_ATT_RR_REG  0xE0
#define TDA7313_ATT_LF_REG  0x80
#define TDA7313_ATT_RF_REG  0xA0
#define TDA7313_SWITCH_REG  0x40
#define TDA7313_BASS_REG    0x60
#define TDA7313_TREBLE_REG  0x70
#define TDA7313_MUTE_LR     0xDF
#define TDA7313_MUTE_RR     0xFF
#define TDA7313_MUTE_LF     0x9F
#define TDA7313_MUTE_RF     0xBF
#define TDA7313_ATT_CH_MAX    31
#define TDA7313_SEL_1       0x00
#define TDA7313_SEL_2       0x01
#define TDA7313_SEL_3       0x02
#define TDA7313_SET_ALL     0xFF
#define TDA7313_LONDNESS_BIT   2
//*****************************************************************************
#define MAX_VOLUME       31
#define MAX_BASS         15
#define MAX_TREBLE       15
#define MAX_BALANS       15
#define PARAM_TDA_MAX     8
unsigned char tda7313_param[3][8] = { {24,3,8,8,31,31,31,31}, {24,3,8,8,31,31,31,31}, {24,3,8,8,31,31,31,31} };
unsigned char tda7313_max[] = { 31,3,15,15,31,31,31,31 };
unsigned char tda7313_bass_treb[] = { 0,1,2,3,4,5,6,7,15,14,13,12,11,10,9,8 };
unsigned char tda7313_attens[] = { 0x18,0x10,0x08,0x00 };
//*****************************************************************************
void send_tda7313_mute(void)
{
i2c_start(); i2c_write(TDA7313_ADDR); i2c_write(TDA7313_MUTE_LF); i2c_stop(); // Mute LF
i2c_start(); i2c_write(TDA7313_ADDR); i2c_write(TDA7313_MUTE_RF); i2c_stop(); // Mute RF
i2c_start(); i2c_write(TDA7313_ADDR); i2c_write(TDA7313_MUTE_LR); i2c_stop(); // Mute LR
i2c_start(); i2c_write(TDA7313_ADDR); i2c_write(TDA7313_MUTE_RR); i2c_stop(); // Mute RR
}
//*****************************************************************************
void send_tda7313(void)
{
unsigned char sw_reg = TDA7313_SWITCH_REG, k;
// Volume
k = tda7313_param[n_select][MM_VOLUME] - 1;
i2c_start(); i2c_write(TDA7313_ADDR);
i2c_write(TDA7313_VOLUME_REG + (0x3F - (k + k)));
i2c_stop();
// Attens
if (k > 3) { k = 3; }
k = tda7313_param[n_select][MM_ATTENS];
sw_reg = sw_reg + n_select + tda7313_attens[k];
if (l_londness) sw_reg &= ~(_BV(2)); else sw_reg |= _BV(2);
i2c_start(); i2c_write(TDA7313_ADDR); i2c_write(sw_reg);
i2c_stop();
// Attens Front Left
k = tda7313_param[n_select][MM_ATTENS_FL];
i2c_start(); i2c_write(TDA7313_ADDR);
i2c_write(TDA7313_ATT_LF_REG + (TDA7313_ATT_CH_MAX - k));
i2c_stop();
// Attens Front Right
k = tda7313_param[n_select][MM_ATTENS_FR];
i2c_start(); i2c_write(TDA7313_ADDR);
i2c_write(TDA7313_ATT_RF_REG + (TDA7313_ATT_CH_MAX - k));
i2c_stop();
// Attens Rear Left
k = tda7313_param[n_select][MM_ATTENS_RL];
i2c_start(); i2c_write(TDA7313_ADDR);
i2c_write(TDA7313_ATT_LR_REG + (TDA7313_ATT_CH_MAX - k));
i2c_stop();
// Attens Rear Right
k = tda7313_param[n_select][MM_ATTENS_RR];
i2c_start(); i2c_write(TDA7313_ADDR);
i2c_write(TDA7313_ATT_RR_REG + (TDA7313_ATT_CH_MAX - k));
i2c_stop();
// Bass
k = tda7313_param[n_select][MM_BASS];
i2c_start(); i2c_write(TDA7313_ADDR);
i2c_write(TDA7313_BASS_REG + tda7313_bass_treb[k]);
i2c_stop();
// Treble
k = tda7313_param[n_select][MM_TREBLE];
i2c_start(); i2c_write(TDA7313_ADDR);
i2c_write(TDA7313_TREBLE_REG + tda7313_bass_treb[k]);
i2c_stop();
}
//*****************************************************************************
void send_tda_freq(void)
{
if (n_select == MSG_SELECT_3)
{
  frequency = freq_fm[n_chanel - 1];
  if ((frequency > MINFREQ) && (frequency < MAXFREQ))
  { 
    send_lm7001(frequency);
    send_tda7313();
  } else {
    send_tda7313_mute();
  }
} else {
  send_tda7313();
}
}
//*****************************************************************************
void step_tda7313_vol(unsigned char n_up_down)
{
unsigned char n;
if (n_up_down) {
// Vol step +
  save_volume = tda7313_param[n_select][MM_VOLUME];
#if (LCD_TYPE == LCD20x4)
  lcd_com(LCD_ROW3 + 1); lcdprogress_bar(0, save_volume, 18);
#endif
#if (LCD_TYPE == LCD16x4)
  lcd_com(LCD_ROW3 + 1); lcdprogress_bar(0, save_volume, 14);
#endif
  for(n = 1; n <= save_volume; n++)
  {
    tda7313_param[n_select][0] = n;
	send_tda_freq();
    lcd_main_menu();
	_delay_ms(1); 
  }
  send_tda7313();
} else {
// Vol step -
  save_volume = tda7313_param[n_select][MM_VOLUME];
  for(n = save_volume; n > 0; n--)
  {
    tda7313_param[n_select][0] = n;
	send_tda_freq();
    lcd_main_menu();
	_delay_ms(1); 
  }
  lcd_com(LCD_ROW3 + 1); lcdprogress_bar(0, save_volume, 18);
  send_tda7313_mute();
  tda7313_param[n_select][MM_VOLUME] = save_volume;
}
}
//*****************************************************************************
//*****************************************************************************

