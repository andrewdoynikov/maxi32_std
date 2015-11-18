//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
// RTC defines
#define DS1307_ADDR_READ  0xD1 
#define DS1307_ADDR_WRITE 0xD0 
#define DS1307_CONTROL    0x07
#define DS1307_DATE       0x04
#define CH                0x07
#define DS1307_PORT_OUT   PINA
#define DS1307_PIN_OUT    PA6
// RTC function
void rtc_init(void);
void rtc_get_time(unsigned char *hour,unsigned char *min,unsigned char *sec);
void rtc_set_time(unsigned char hour, unsigned char min, unsigned char sec);
void rtc_print_time(unsigned char hour, unsigned char min, unsigned char sec);
void rtc_print_time_read(void);
void rtc_get_date(unsigned char *date,unsigned char *month,unsigned char *year);
void rtc_set_date(unsigned char date,unsigned char month,unsigned char year);
unsigned char bcd2bin(unsigned char bcd);
unsigned char bin2bcd(unsigned char bin);
void rtc_print_time(unsigned char hour, unsigned char min, unsigned char sec);
void rtc_print_date(unsigned char date,unsigned char month,unsigned char year);
void rtc_print_time_read(void);
void rtc_print_date_read(void);
//*****************************************************************************
void rtc_init(void)
{
i2c_start();
i2c_write(DS1307_ADDR_WRITE);
i2c_write(0x00);
i2c_stop();
i2c_start();
i2c_write(DS1307_ADDR_WRITE);
i2c_write(DS1307_CONTROL);
i2c_write(0x90);
i2c_stop();
}
//*****************************************************************************
void rtc_set_time(unsigned char hour, unsigned char min, unsigned char sec)
{
  i2c_start();
  i2c_write(DS1307_ADDR_WRITE);
  i2c_write(0);
  i2c_write(bin2bcd(sec));
  i2c_write(bin2bcd(min));
  i2c_write(bin2bcd(hour));
  i2c_stop();
}
//*****************************************************************************
void rtc_get_time(unsigned char *hour,unsigned char *min,unsigned char *sec)
{
i2c_start();
i2c_write(DS1307_ADDR_WRITE);
i2c_write(0);
i2c_start();
i2c_write(DS1307_ADDR_READ);
*sec = bcd2bin(i2c_read(I2C_ASK));
*min = bcd2bin(i2c_read(I2C_ASK));
*hour = bcd2bin(i2c_read(I2C_NOASK));
i2c_stop();
}
//*****************************************************************************
void rtc_set_date(unsigned char date,unsigned char month,unsigned char year)
{
i2c_start();
i2c_write(DS1307_ADDR_WRITE);
i2c_write(DS1307_DATE);
i2c_write(bin2bcd(date));
i2c_write(bin2bcd(month));
i2c_write(bin2bcd(year));
i2c_stop();
}
//*****************************************************************************
void rtc_get_date(unsigned char *date,unsigned char *month, unsigned char *year)
{
i2c_start();
i2c_write(DS1307_ADDR_WRITE);
i2c_write(DS1307_DATE);
i2c_start();
i2c_write(DS1307_ADDR_READ);
*date = bcd2bin(i2c_read(I2C_ASK));
*month = bcd2bin(i2c_read(I2C_ASK));
*year = bcd2bin(i2c_read(I2C_NOASK));
i2c_stop();
}
//*****************************************************************************
unsigned char bcd2bin(unsigned char bcd)
{
unsigned char Temp = bcd & 0x0F; 
while (bcd >= 0x10) 
{ 
  Temp += 10; 
  bcd -= 0x10; 
} 
return Temp; 
}
//*****************************************************************************
unsigned char bin2bcd(unsigned char bin)
{
unsigned Temp = 0; 
while(bin > 9) 
{ 
  Temp += 0x10; 
  bin -= 10; 
} 
return Temp + bin; 
}
//*****************************************************************************
void rtc_print_time(unsigned char hour, unsigned char min, unsigned char sec)
{
print_dec(hour,2,'0'); lcd_dat(':'); print_dec(min,2,'0'); lcd_dat(':'); print_dec(sec,2,'0');
}
//*****************************************************************************
void rtc_print_date(unsigned char date,unsigned char month,unsigned char year)
{
#if (LCD_TYPE == LCD20x4)
print_dec(date,2,'0'); lcd_dat('/'); print_dec(month,2,'0'); lcd_dat('/'); print_dec(20,2,'0'); print_dec(year,2,'0');
#endif
#if (LCD_TYPE == LCD16x4)
print_dec(date,2,'0'); lcd_dat('/'); print_dec(month,2,'0'); lcd_dat('/'); print_dec(year,2,'0');
#endif
}
//*****************************************************************************
void rtc_print_time_read(void)
{
unsigned char h,m,s;
rtc_get_time(&h,&m,&s);
rtc_print_time(h,m,s);
}
//*****************************************************************************
void rtc_print_date_read(void)
{
unsigned char d,m,y;
rtc_get_date(&d,&m,&y);
rtc_print_date(d,m,y);
}
//*****************************************************************************
int dayofweek(int day, int month, int year)
{
  if (month < 3) {
    month += 12;
    year--;
  }
  return ( (13 * month + 3) / 5 + day + year + year / 4 - year / 100 + year / 400) % 7;
}
//*****************************************************************************
