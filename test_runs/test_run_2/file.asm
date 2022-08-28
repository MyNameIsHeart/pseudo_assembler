; file name: file.asm
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: print #48
 lea W, r5
 inc r5
 bne END
 mov r3, K
 sub r1, r4
 cmp K, #-6
 bne %END
 dec W
.entry MAIN
 jmp %LOOP
 add L3, L3
END: stop
STR: .string "asdf"
LIST: .data 6, -9
 .data -100
K: .data 31
.extern L3
