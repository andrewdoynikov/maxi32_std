//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
#include <avr/io.h>
#include <inttypes.h>
//************************************************************************************************
void eeprom_write_tda7313(unsigned char nsel)
{
unsigned char i;
for(i = 0; i < 8; i++)
{
  switch (nsel) // switch
  {
  case MSG_SELECT_1:
    eeprom_write_byte((uint8_t*)TDA7313_ADDR_START_1 + i, tda7313_param[0][i]);
  break;
  case MSG_SELECT_2:
    eeprom_write_byte((uint8_t*)TDA7313_ADDR_START_2 + i, tda7313_param[1][i]);
  break;
  case MSG_SELECT_3:
    eeprom_write_byte((uint8_t*)TDA7313_ADDR_START_3 + i, tda7313_param[2][i]);
  break;
  }
}
eeprom_write_byte((uint8_t*)TDA7313_ADDR_SELECT, n_select);
eeprom_write_byte((uint8_t*)TDA7313_ADDR_LOUD, l_londness);
}
//************************************************************************************************
void eeprom_read_tda7313(unsigned char nsel)
{
unsigned char i, k = 0;
for(i = 0; i < 8; i++)
{
  switch (nsel) // switch
  {
  case MSG_SELECT_1:
  k = eeprom_read_byte((uint8_t*)TDA7313_ADDR_START_1 + i);
  break;
  case MSG_SELECT_2:
  k = eeprom_read_byte((uint8_t*)TDA7313_ADDR_START_2 + i);
  break;
  case MSG_SELECT_3:
  k = eeprom_read_byte((uint8_t*)TDA7313_ADDR_START_3 + i);
  break;
  }
  if (k <= tda7313_max[i]) {
    tda7313_param[nsel][i] = k;
  }
}
k = eeprom_read_byte((uint8_t*)TDA7313_ADDR_SELECT);
if (k < 4) { n_select = k; } else { n_select = 0; }
k = eeprom_read_byte((uint8_t*)TDA7313_ADDR_LOUD);
if (k < 2) { l_londness = k; } else { l_londness = 0; }
}
//************************************************************************************************
void eeprom_write_tda7313_all(void)
{
eeprom_write_tda7313(0);
eeprom_write_tda7313(1);
eeprom_write_tda7313(2);
}
//************************************************************************************************
void eeprom_read_tda7313_all(void)
{
eeprom_read_tda7313(0);
eeprom_read_tda7313(1);
eeprom_read_tda7313(2);
}
//************************************************************************************************
void eeprom_write_tda7313_cursor(unsigned char param)
{
eeprom_write_byte((uint8_t*)TDA7313_ADDR_START_1 + (n_select * 0x10) + param, tda7313_param[n_select][param]);
}
//************************************************************************************************
void save_chanel_freq(unsigned long freqs,unsigned char chl)
{
unsigned char frq_lo,frq_hi;
frq_hi = HiBYTE(freqs);
frq_lo = LoBYTE(freqs);
eeprom_write_byte((uint8_t*)LM7001_ADDR_START + (chl - 1) * 2, frq_hi);
eeprom_write_byte((uint8_t*)LM7001_ADDR_START + (chl - 1) * 2 + 1, frq_lo);
}
//*****************************************************************************
void load_chanel_freq(unsigned char chl)
{
unsigned char frq_lo,frq_hi;
unsigned long frq = 0;
frq_hi = eeprom_read_byte((uint8_t*)LM7001_ADDR_START + (chl - 1) * 2);
frq_lo = eeprom_read_byte((uint8_t*)LM7001_ADDR_START + (chl - 1) * 2 + 1);
frq = frq_hi; frq = (frq << 8); frq = frq + frq_lo;
freq_fm[chl - 1] = frq;
}
//*****************************************************************************
void load_chanel_freq_all(void)
{
unsigned char i;
for(i = 1; i < MAX_CHANEL + 1; i++)
{
  load_chanel_freq(i);
}
i = eeprom_read_byte((uint8_t*)LM7001_ADDR_CHANEL);
if ((i == 0) || (i > MAX_CHANEL)) { n_chanel = 1; } else { n_chanel = i; }
i = eeprom_read_byte((uint8_t*)LM7001_ADDR_BANK);
if ((i == 0) || (i > MAXBANK)) { n_bank = 0; } else { n_bank = i; }
frequency = freq_fm[n_chanel - 1];
}
//*****************************************************************************
void save_chanel_freq_all(void)
{
unsigned char i;
for(i = 0; i < MAX_CHANEL; i++)
{
  save_chanel_freq(freq_fm[i], i + 1);
}
}
//*****************************************************************************
void save_rc5key(void)
{
unsigned char i;
for(i = 0; i < RC5_MENU_MAX + 1; i++)
{
  eeprom_write_byte((uint8_t*)RC5_ADDR_START + i, key_rc5[i]);
}    
//eeprom_write_byte((uint8_t*)RC5_PARAM_SAVE,'T');
}
//*****************************************************************************
void load_rc5key(void)
{
unsigned char i;
//p = eeprom_read_byte((uint8_t*)RC5_PARAM_SAVE);
//if (p == 'T')
//{
  for(i = 0; i < RC5_MENU_MAX + 1; i++)
  {
    key_rc5[i] = eeprom_read_byte((uint8_t*)RC5_ADDR_START + i);
  }    
//}
}
//*****************************************************************************
void save_extkey_volt(unsigned long volt, unsigned char key)
{
unsigned char volt_lo,volt_hi;
volt_hi = HiBYTE(volt);
volt_lo = LoBYTE(volt);
eeprom_write_byte((uint8_t*)EXTKEY_ADDR_START + key * 2, volt_hi);
eeprom_write_byte((uint8_t*)EXTKEY_ADDR_START + key * 2 + 1, volt_lo);
}
//*****************************************************************************
void load_extkey_volt(unsigned char key)
{
unsigned char volt_lo,volt_hi;
unsigned long volt = 0;
volt_hi = eeprom_read_byte((uint8_t*)EXTKEY_ADDR_START + key * 2);
volt_lo = eeprom_read_byte((uint8_t*)EXTKEY_ADDR_START + key * 2 + 1);
volt = volt_hi; volt = (volt << 8); volt = volt + volt_lo;
extkey_volts[key] = volt;
}
//*****************************************************************************
void load_extkey_volt_all(void)
{
unsigned char i;
//p = eeprom_read_byte((uint8_t*)EXTKEY_ADDR_SAVE);
//if (p == 'T')
//{
  for(i = 0; i < EXT_MENU_MAX + 1; i++)
  {
    load_extkey_volt(i);
  }    
//}
}
//*****************************************************************************
void save_extkey_volt_all(void)
{
unsigned char i;
for(i = 0; i < EXT_MENU_MAX + 1; i++)
{
  save_extkey_volt(extkey_volts[i], i );
}    
//eeprom_write_byte((uint8_t*)EXTKEY_ADDR_SAVE, 'T');
}
//************************************************************************************************
void load_param_all(void)
{
unsigned char k;
k = eeprom_read_byte((uint8_t*)PARAM_ADDR_MAX_TEMP);
if ((k < DS_MIN_TEMP) || (k > DS_MAX_TEMP)) { ds_temp_max = DS_MIN_TEMP; } else { ds_temp_max = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ADDR_NOKEY_TIME);
if ((k < KBD_NOKEY_TIME_MIN) || (k > KBD_NOKEY_TIME_MAX)) { kbd_nokey = KBD_NOKEY_TIME_DEF; } else { kbd_nokey = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ADDR_STB_OUT);
if (k > 1) { standby_out = 1; } else { standby_out =  k; } 
k = eeprom_read_byte((uint8_t*)PARAM_ADDR_AC_WAIT);
if ((k < AC_WAIT_MIN) || (k > AC_WAIT_MAX)) { ac_wait_sec = AC_WAIT_DEF; } else { ac_wait_sec = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ADDR_RC5_SYSTEM);
if ((k < RC5_SYSTEM_MIN) || (k > RC5_SYSTEM_MAX)) { rc5_system = RC5_SYSTEM_DEF; } else { rc5_system = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ADDR_LIGHT_LCD);
if (k > 2) { light_on_off = 1; } else { light_on_off = k; }
}

//*****************************************************************************
void save_param_all(void)
{
eeprom_write_byte((uint8_t*)PARAM_ADDR_MAX_TEMP, ds_temp_max);
eeprom_write_byte((uint8_t*)PARAM_ADDR_NOKEY_TIME, kbd_nokey);
eeprom_write_byte((uint8_t*)PARAM_ADDR_STB_OUT, standby_out);
eeprom_write_byte((uint8_t*)PARAM_ADDR_AC_WAIT, ac_wait_sec);
eeprom_write_byte((uint8_t*)PARAM_ADDR_RC5_SYSTEM, rc5_system);
eeprom_write_byte((uint8_t*)PARAM_ADDR_LIGHT_LCD, light_on_off);
}
//************************************************************************************************
void save_alarm_eeprom(void)
{
eeprom_write_byte((uint8_t*)PARAM_ALARM_ON_OFF, alarm_param[ALARM_ON_OFF]);
eeprom_write_byte((uint8_t*)PARAM_ALARM_H, alarm_param[ALARM_H]);
eeprom_write_byte((uint8_t*)PARAM_ALARM_M, alarm_param[ALARM_M]);
eeprom_write_byte((uint8_t*)PARAM_ALARM_TYPE, alarm_param[ALARM_TYPE]);
eeprom_write_byte((uint8_t*)PARAM_ALARM_INPUT, alarm_param[ALARM_INPUT]);
eeprom_write_byte((uint8_t*)PARAM_ALARM_MINUT, alarm_param[ALARM_MINUT]);
eeprom_write_byte((uint8_t*)PARAM_ALARM_POSLE, alarm_param[ALARM_POSLE]);
eeprom_write_byte((uint8_t*)PARAM_ALARM_BUD, alarm_param[ALARM_BUD]);
}
//************************************************************************************************
void load_alarm_eeprom(void)
{
unsigned char k;
k = eeprom_read_byte((uint8_t*)PARAM_ALARM_ON_OFF);
if (k > 1) { alarm_param[ALARM_ON_OFF] = 0; } else { alarm_param[ALARM_ON_OFF] = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ALARM_H);
if (k > 23) { alarm_param[ALARM_H] = 0; } else { alarm_param[ALARM_H] = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ALARM_M);
if (k > 59) { alarm_param[ALARM_M] = 0; } else { alarm_param[ALARM_M] = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ALARM_TYPE);
if (k > 1) { alarm_param[ALARM_TYPE] = 0; } else { alarm_param[ALARM_TYPE] = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ALARM_INPUT);
if (k > SELECT_MAX) { alarm_param[ALARM_INPUT] = 0; } else { alarm_param[ALARM_INPUT] = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ALARM_MINUT);
if (k > 4) { alarm_param[ALARM_MINUT] = 1; } else { alarm_param[ALARM_MINUT] = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ALARM_POSLE);
if (k > 1) { alarm_param[ALARM_POSLE] = 0; } else { alarm_param[ALARM_POSLE] = k; }
k = eeprom_read_byte((uint8_t*)PARAM_ALARM_BUD);
if (k > 1) { alarm_param[ALARM_BUD] = 0; } else { alarm_param[ALARM_BUD] = k; }
}
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************
