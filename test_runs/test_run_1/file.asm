opening: .string "Heya"
var1: .data 4  , +21 , 	-23
var2: .data -3

;mov
MAIN: mov #-1 ,	var1
mov #-2 ,	r7
mov var1 ,	var1
mov var1 ,	r7
mov r5 ,	var1
mov r6 ,	r5

;cmp
cmp #+3	,	#-4
cmp #+5	,	var2
cmp #+6	,	r3
cmp var2,	#-7
cmp var2,	var2
cmp var2,	r3
cmp r3	,	#8
cmp r3	,	var2
cmp r3	,	r3

;add
add #-3 ,	var4
add #-4 ,	r4
add var2 ,	var1
add var2 ,	r4
add r2 ,	var4
add r3 ,	r4

var3: .string "F"
.entry var1
.entry var3
.extern var4


;sub
sub #-32 ,	var3
sub #+12 ,	r5
sub var1 ,	var1
sub var3 ,	r6
sub r5 ,	var3
sub r4 ,	r0

;lea
lea var4 ,	var3
lea var2 ,	r5

;clr
clr var4
clr r6

;not
not var2
not r3

;inc
inc var1
inc r1

;dec
dec var3
dec r7

;jmp
jmp MAIN
jmp %MAIN

;bne
bne MAIN
bne %MAIN

;jsr
jsr MAIN
jsr %MAIN

;red
red var3
red r2

;print
print #-63
print var1
print r4

;rts
rts

;stop
stop

