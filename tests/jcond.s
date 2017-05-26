jmp #5
print ^cond_jump_insts_ok
halt
print ^cond_jump_insts_ng
halt
sub $1 $2 #3
jz  #3 #3
jge #3 #3
jgt #3 #3
sub $2 $2 #3
jlt #3 #3
jnz #3 #3
jgt #3 #3
sub $2 $1 #3
jlt #3 #3
jle #3 #3
jz  #3 #3
sub $1 $2 #3
jle #3 #20
jmp #3
jlt #3 #22
jmp #3
sub $2 $2 #3
jz  #3 #25
jmp #3
sub $2 $1 #3
jnz #3 #28
jmp #3
sub $1 $2 #3
jnz #3 #1
jmp #3
