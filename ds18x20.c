#include "ds18x20.h"

char DS18x20_StartMeasure(void)
{
	//Reset, skip ROM and start temperature conversion
	if (!OW_Reset()) return 0;
	OW_WriteByte(OW_CMD_SKIPROM);
	OW_WriteByte(THERM_CMD_CONVERTTEMP);
	return 1;
}

char DS18x20_StartMeasureAddressed(unsigned char* rom)
{
	//Reset and start temperature conversion
	if (!OW_Reset()) return 0;
	OW_MatchROM(rom);
	OW_WriteByte(THERM_CMD_CONVERTTEMP);
	return 1;
}

#ifdef DS18X20_CHECK_CRC
#define CRC8INIT	0x00
#define CRC8POLY	0x18              //0X18 = X^8+X^5+X^4+X^0

unsigned char crc8(unsigned char *data_in, unsigned int number_of_bytes_to_read )
{
	unsigned char	crc;
	unsigned int	loop_count;
	unsigned char	bit_counter;
	unsigned char	data;
	unsigned char	feedback_bit;

	crc = CRC8INIT;
	
	for (loop_count = 0; loop_count != number_of_bytes_to_read; loop_count++)
	{ 
		data = data_in[loop_count];

		bit_counter = 8;
		do { 
			feedback_bit = (crc ^ data) & 0x01;
			if (feedback_bit==0x01) crc = crc ^ CRC8POLY;

			crc = (crc >> 1) & 0x7F;
			if (feedback_bit==0x01) crc = crc | 0x80;

			data = data >> 1;
			bit_counter--;
		}
		while (bit_counter > 0);
	}
	return crc;
}
#endif 

char DS18x20_ReadData(unsigned char *rom, unsigned char *buffer)
{
	//Reset, skip ROM and send command to read Scratchpad
	if (!OW_Reset()) return 0;
	if (rom[0]==0)	OW_WriteByte(OW_CMD_SKIPROM);
	else OW_MatchROM(rom);
	OW_WriteByte(THERM_CMD_RSCRATCHPAD);
	
#ifdef DS18X20_CHECK_CRC
	unsigned char	buff[9] = {1,2,3,4,5,6,7,8,9};
	for (unsigned char i=0; i<9; i++) buff[i] = OW_ReadByte();
	buffer[0] = buff[0]; buffer[1] = buff[1];
	buffer[2] = buff[2]; buffer[3] = buff[3];
	buffer[4] = buff[4]; buffer[5] = buff[5];
	buffer[6] = buff[6]; buffer[7] = buff[7];
	if (crc8(buff, 9)) return 0;	// если контрольная сумма не совпала, возвращаем ошибку
#else 
	//Read Scratchpad (only 2 first bytes)
	buffer[0] = OW_ReadByte(); // Read TL
	buffer[1] = OW_ReadByte(); // Read TH	
#endif

	return 1;
}

/*signed int DS18x20_ConvertToThemperature(unsigned char famely_ds, unsigned char* data)
{
unsigned int tmp = 0;
unsigned char scratchpad0, scratchpad1;
signed int temp = 0, sign_bit;
scratchpad0 = data[0];
scratchpad1 = data[1];
if ((scratchpad1 & 128) == 0)
{
  sign_bit = 0;
} else {
  sign_bit = 1;
  tmp = ((unsigned int)scratchpad1 << 8) | scratchpad0;
  tmp = ~tmp + 1;
  scratchpad0 = tmp;
  scratchpad1 = tmp >> 8;  
}
if (famely_ds == 0x28) // 18B20
{
//  temp = ((scratchpad0 >> 4) | ((scratchpad1 & 7) << 4));
  tmp = ((unsigned int)scratchpad1 << 8) | scratchpad0;
  temp = tmp / 16;
} else {
  tmp = ((unsigned int)scratchpad1 << 8) | scratchpad0;
  temp =  tmp / 2;
}
if (sign_bit) { temp = -temp; }
return temp;
}
*/

signed int DS18x20_ConvertToThemperature(unsigned char famely_ds, unsigned char* data)
{
signed int temp = 0, sign_bit = 0;
int meas;
meas = data[0];  // LSB
meas |= ( (uint16_t)data[1] ) << 8; // MSB
if (famely_ds == 0x10)
{
  meas &= (uint16_t) 0xfffe;    // Discard LSB, needed for later extended precicion calc
  meas <<= 3;                   // Convert to 12-bit, now degrees are in 1/16 degrees units
  meas += (16 - data[6]); // Add the compensation and remember to subtract 0.25 degree (4/16)
}
if ( meas & 0x8000 )  {
  sign_bit = 1;      // mark negative
  meas ^= 0xffff;  // convert to positive => (twos complement)++
  meas++;
} else {
  sign_bit = 0;
}
temp  = (uint8_t)(meas >> 4); 
if (sign_bit) { temp = -temp; }
return temp;
}


