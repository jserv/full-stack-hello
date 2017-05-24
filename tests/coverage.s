; FIXME: all instructions should be used for coverage test.
add $40 $2 #1
add #1 $8 #2
print #1
print #2
jmp #6
sub #2 $16 #3		; skipped because previous jump
add #2 $100 #3		; to be executed
print #3
