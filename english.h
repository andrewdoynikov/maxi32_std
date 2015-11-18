//************************************************************************************************
// Project MAXI32 GRTOS  
// Author: Andrew Doynikov
// 15.08.2012	
//************************************************************************************************
// ENG LANG FILE
//************************************************************************************************
unsigned char *dw[] = {"Mo","Tu","We","Th","Fr","Sa","Su"};
unsigned char *logo[] = {
//012345678901234567890
 "Design : MAXI-32",
 "ver... : FINAL 4",
 "FOUND  :    SENS"
};
#define LOGO_1     0
#define LOGO_2     1
#define LOGO_3     2
//************************************************************************************************
#if (LCD_TYPE == LCD20x4)
unsigned char *msg[] = {
//012345678901234567890
 "OFF      ",
 "ACOUSTIC ",
 " SEC",
 "ON  ",
 "OFF ",
 "SOUND OFF    ",
 "---.--",
 "MHz",
 "INSTALLAT",
 "PARAMETERS",
 "LOG : ",
 "STATION: ",
 "AUTO",
 "STATION:",
 "BUTTON ",
 " REQUIRED ",
 "REBOOT      ",
 "LOG ",
 "PEEP",
 "PfP",
 "YES",
 "NO ",
 "RC5 BUTTON IS ALREAD"
};
#endif
#if (LCD_TYPE == LCD16x4)
unsigned char *msg[] = {
//012345678901234567890
 "OFF      ",
 "ACOUSTIC ",
 " SEC",
 "ON ",
 "OFF",
 "SOUND OF",
 "---.--",
 "MHz",
 "INSTALLAT",
 "PARAMETERS",
 "LOG : ",
 "STATION: ",
 "AWT",
 "STAN:",
 "BUTT ",
 " REQUIRED ",
 "REBOOT      ",
 "LOG ",
 "PEEP",
 "PfP",
 "YES",
 "NO ",
 "BUTTON IS ALREA"
};
#endif
#define MSG_STANDBY		0
#define MSG_AC		 	1
#define MSG_SEC		 	2
#define MSG_ON		 	3
#define MSG_OFF		 	4
#define MSG_MUTE	 	5
#define MSG_FREQ	 	6
#define MSG_MHZ		 	7
#define MSG_OMENU	 	8
#define MSG_OMENU1	 	9
#define MSG_SELECT	 	10
#define MSG_CH	 		11
#define MSG_AVTO 		12
#define MSG_STAN 		13
#define MSG_KNOPKA 		14
#define MSG_PEREZ1 		15
#define MSG_PEREZ2 		16
#define MSG_IN  		17
#define MSG_PISK  		18
#define MSG_000  		19
#define MSG_DA  		20
#define MSG_NET  		21
#define MSG_RC5_DUBLE   22
//************************************************************************************************
#if (LCD_TYPE == LCD20x4)
#define AMENU_MAX 4
unsigned char *alarmmenu[] = {
//0123456789ABCDEF0123
 "BUDILNIK :",
 "TIME     :",  	  	
 "TIP BUD. :",
// "LOG      :",
 "DURATION.:",
// "AFTER OFF:",
 "BUDIL OFF:"
};
#endif
//************************************************************************************************
#if (LCD_TYPE == LCD16x4)
#define AMENU_MAX 4
unsigned char *alarmmenu[] = {
//0123456789ABCDEF0123
 "BUDIL:",
 "TIME :",  	  	
 "TYPE :",
// "LOG  :",
 "DLIT.:",
// "P.OFF:",
 "B.OFF:"
};
#endif
//************************************************************************************************
#define SET_ALARM_ON_OFF 0
#define SET_ATIME        1
#define SET_ATYPE        2
//#define SET_AINPUT       3
#define SET_AMIN         3
//#define SET_APOSLE       5
#define SET_ABUD         4
#define ALARM_ON_OFF     0
#define ALARM_H          1
#define ALARM_M          2
#define ALARM_TYPE       3
#define ALARM_INPUT      4
#define ALARM_MINUT      5
#define ALARM_POSLE      6
#define ALARM_BUD        7
#define ALARM_MIN        1
#define ALARM_MAX        4
//************************************************************************************************
#define SELECT_MAX     2
unsigned char *select[] = {
"PC ",
"DVD",
"FM "
};
#define MSG_SELECT_1   0
#define MSG_SELECT_2   1
#define MSG_SELECT_3   2
//************************************************************************************************
#define MMENU_MAX 7
unsigned char *mainmenu[] = {
//012345678901234567890
 "VOL :", // 1
 "USIL:", // 2
 "LF  :", // 3
 "HF  :", // 4
 "FR L:", // 5
 "FR R:", // 6
 "TL L:", // 7
 "TL R:"  // 8
};
#define MM_VOLUME      0
#define MM_ATTENS      1
#define MM_BASS        2
#define MM_TREBLE      3
#define MM_ATTENS_FL   4
#define MM_ATTENS_FR   5
#define MM_ATTENS_RL   6
#define MM_ATTENS_RR   7
#define MM_MAIN        8
#define MM_FMRADIO     9
#define MM_OPTMENU     10
//************************************************************************************************
#if (LCD_TYPE == LCD20x4)
#define OMENU_MAX 11
unsigned char *optionmenu[] = {
//0123456789ABCDEF0123
 "TIME : ",  	  	
 "DATA : ",  		
 "ADM. ALARM > ",    
 "FM STATION > ",		
 "RC5 KEYS   > ", 
 "EXT. KEY.  > ", 
 "MAX. TEMP .: ", 
 "WAIT TIME  : ", 
 "OFF   STB  : ", 
 "DELAY AS   : ", 
 "REMOTE COD : ", 
 "LIGHT      : " 
};
#endif
//************************************************************************************************
#if (LCD_TYPE == LCD16x4)
#define OMENU_MAX 11
unsigned char *optionmenu[] = {
//01234567890123456789
 "TIME :",  	  	
 "DATA :",  		
 "ADM.ALARM >",    
 "FM STAT.  >",		
 "RC5 BUTT. >", 
 "EXT BUTT. >", 
 "MAX. TEMP. ", 
 "WAIT TIME :", 
 "OFF   STB :", 
 "DELAY AS  :", 
 "CODE RC5  :", 
 "LIGHT     :" 
};
#endif
//************************************************************************************************
#define SET_TIME        0
#define SET_DATE        1
#define SET_ALARM       2
#define SET_FM_CHANEL   3
#define SET_RC5_KEY     4
#define SET_EXT_KEY     5
#define SET_MAX_TEMP    6
#define SET_NOKEY       7
#define SET_STB_OUT     8
#define SET_AC_WAIT     9
#define SET_RC5_SYSTEM  10
#define SET_LIGHT_LCD   11
//************************************************************************************************
#if (LCD_TYPE == LCD20x4)
#define RC5_MENU_MAX 23
unsigned char *rc5_menu[] = {
//0123456789ABCDEF0123
//**************00:??*
 "TURNING   :",  	  	
 "CHOICE    :",    
 "SOUND OFF :",  		
 "UP        :",    
 "DOWN      :",		
 "LEFT      :", 
 "RIGHT     :", 
 "MENU      :", 
 "LOUDNESS  :", 
 "INPUT SEL.:", 
 "BUTTON 1  :", 
 "BUTTON 2  :", 
 "BUTTON 3  :", 
 "BUTTON 4  :", 
 "BUTTON 5  :", 
 "BUTTON 6  :", 
 "BUTTON 7  :", 
 "BUTTON 8  :", 
 "BUTTON 9  :", 
 "BANK STAN :", 
 "LF -      :", 
 "LF +      :", 
 "HF -      :", 
 "HF +      :" 
};
#endif
//************************************************************************************************
#if (LCD_TYPE == LCD16x4)
#define RC5_MENU_MAX 23
unsigned char *rc5_menu[] = {
//0123456789ABCDEF0123
//**********00:??*
 "TURNING :",  	  	
 "CHOICE  :",    
 "SOUND OF:",  		
 "UP      :",    
 "DOWN    :",		
 "LEFT    :", 
 "RIGHT   :", 
 "MENU    :", 
 "LOUDNESS:", 
 "INP. SEL:", 
 "BUTTON 1:", 
 "BUTTON 2:", 
 "BUTTON 3:", 
 "BUTTON 4:", 
 "BUTTON 5:", 
 "BUTTON 6:", 
 "BUTTON 7:", 
 "BUTTON 8:", 
 "BUTTON 9:", 
 "BANK ST.:", 
 "LF -    :", 
 "LF +    :", 
 "HF -    :", 
 "HF +    :" 
};
#endif
//************************************************************************************************
#define EXT_MENU_MAX 9
unsigned char *ext_menu[] = {
//0123456789ABCDEF0123
//BUTTON*1****000:???*
//BUTT*1**000:???*
 "1",  	  	
 "2",  	  	
 "3",  	  	
 "4",  	  	
 "5",  	  	 
 "6",  	  	
 "7",  	  	
 "8",  	  	
 "9",  	  	
 "BANK ",  	  	
};
//************************************************************************************************



