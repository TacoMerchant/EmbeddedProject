;****************************************************************************
; debug_w_9registers.s                                                      *
; Runs on MSP432E401Y                                                       *
;   Input: a pointer passed in R0 to an array of nine 32 bit integers      *
;   Output: places the 9 values into registers R1 to R9 and then goes       *
;           into a hard loop doing nothing.  Does not save registers        *
;           before using them. Run with a debugger to look at               *
;           register values.                                                *
;   To use: Put this prototype in the C file where you wish to use it:      *
;               void Load_9registers(uint32_t data[]);                      *
;           Create an array for data:                                       *
;               uint32_t data[9];                                           *
;           In your C program place data into the array.                    *
;               data[0] = X;      / Where X and Y are variables with data   *
;               data[1] = Y;      /   you wish to observe                   *
;                 etc...                                                    *
;           And then call this routine:                                     *
;               Load_9registers(data);                                      *
;           Start your program in debug mode and set a break point on the B *
;           instruction in this assembly code.  If your program is well     *
;           behaved it will run to the break point at which time registers  *
;           R1 to R9, as seen with the debugger, will show the data.        *
;           You can modify this code if needed.                             *
;  5/28/18  L.Aamodt                                                        *
;****************************************************************************

        AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
        REQUIRE8
        PRESERVE8

		EXPORT Load_9registers
			
Load_9registers
        CPSID   I				; Disable interrupts

		LDR		r1,[r0]			; Place data into registers R1 to R8
		LDR		r2,[r0,#4]
		LDR		r3,[r0,#8]
		LDR		r4,[r0,#12]
		LDR		r5,[r0,#16]
		LDR		r6,[r0,#20]
		LDR		r7,[r0,#24]
		LDR		r8,[r0,#28]
		LDR		r9,[r0,#32]
		B		.				; Endless loop here, branch to itself
		
    ALIGN
    END
