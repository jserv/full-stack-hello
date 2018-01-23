jmp $5                      ; keep pass/fail at fixed location
print "cond_jump_insts_ok"   ; all conditional jump instructions pass this test
halt
print "cond_jump_insts_fail"; any fail
halt
sub $0 $1 #1                ; operand -1
add $0 $0 #2                ; operand 0
add $0 $1 #3                ; operand 1
jlt #1 $10                  ; jlt should only branch with a negative operand
jmp $3
jlt #2 $3
jlt #3 $3
jle #1 $14                  ; jle should only branch with a zero or negative operand
jmp $3
jle #2 $16
jmp $3
jle #3 $3
jz  #1 $3                   ; jz should only branch with a zero operand
jz  #2 $20
jmp $3
jz  #3 $3
jge #1 $3                   ; jge should only branch with a zero or positive operand
jge #2 $24
jmp $3
jge #3 $26
jmp $3
jgt #1 $3                   ; jgt should only branch with a positive operand
jgt #2 $3
jgt #3 $30
jmp $3
jnz #1 $32                  ; jnz should only branch with a negative or positive operand
jmp $3
jnz #2 $3
jnz #3 $1
jmp $3
