//************************************************************************************************
// Project MAXI32 TIMER SERVICE  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
#ifndef BITS_MACROS_
#define BITS_MACROS_
//************************************************************************************************
#define Bit(bit)  (1<<(bit))
#define ClearBit(reg, bit)       reg &= (~(1<<(bit)))
#define SetBit(reg, bit)          reg |= (1<<(bit))	
#define SetBitVal(reg, bit, val) do { if ((val & 1) == 0) reg &= (~(1<<(bit))); else reg |= (1<<(bit));} while(0)
#define BitIsClear(reg, bit)    ((reg & (1<<(bit))) == 0)
#define BitIsSet(reg, bit)       ((reg & (1<<(bit))) != 0)
#define InvBit(reg, bit)	  reg ^= (1<<(bit))
//************************************************************************************************
#endif // BITS_MACROS_



