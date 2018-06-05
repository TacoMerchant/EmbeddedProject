////****************************************************************************
//// UARTTestMain.c
//// Runs on MSP432E401Y
//// Used to test the UART.c driver
////
//// Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
////    You may use, edit, run or distribute this file
////    as long as the above copyright notice remains
////
////  Modified  L.Aamodt  5/1/18
////****************************************************************************

//// U0Rx (VCP receive) connected to PA0
//// U0Tx (VCP transmit) connected to PA1
//// Note: Connected LaunchPad JP4 and JP5 inserted parallel with long side of board.

//#include <stdint.h>
//#include "PLL.h"
//#include "UART.h"

////---------------------OutCRLF---------------------
//// Output a CR,LF to UART to go to a new line
//// Input: none
//// Output: none
//void OutCRLF(void){
//  //UART_OutChar(CR);
//	UART_OutCommand(0xFE);
//	UART_OutCommand(0x01);
//	
//	//UART_OutChar(LF);
////	UART_OutCommand(0xFE);
////	UART_OutCommand(0x0A);
//}

//int main(void){
////  char i;
////  char string[20];  // global to assist in debugging
////  uint32_t n;

//  PLL_Init();               // set system clock to 120 MHz
//  UART_Init();              // initialize UART
////  OutCRLF();              // CRLF doesn't work well with LCD display
////  for(i='A'; i<='Z'; i=i+1){// print the uppercase alphabet
////    UART_OutChar(i);
////  }
////   OutCRLF();
////  UART_OutChar(' ');
////  for(i='a'; i<='z'; i=i+1){// print the lowercase alphabet
////    UART_OutChar(i);
////  }
////   OutCRLF();
//  UART_OutChar('-');
//  UART_OutChar('-');
//  UART_OutChar('>');
//	
////	OutCRLF();
////	UART_OutCommand(0xFE); 			///////////////////temp
////	UART_OutChar('A'); 					//////////////////temp
//  while(1){
////                   Put code here for repeating I/O		
////    UART_OutChar('U');		// uncomment this line to send out character U
//		
////  The following 8 statements prompt for, read in and display a char string
////    UART_OutString("InString: ");
////    UART_InString(string,19);
////    UART_OutString(" OutString="); UART_OutString(string);
////
////		OutCRLF();

////    UART_OutString("InUDec: ");  n=UART_InUDec();
////    UART_OutString(" OutUDec="); UART_OutUDec(n); OutCRLF();

////    UART_OutString("InUHex: ");  n=UART_InUHex();
////    UART_OutString(" OutUHex="); UART_OutUHex(n); OutCRLF();

//  }
//}
