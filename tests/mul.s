; Test all possible combinations of multipliacation instructions  
mul $3 $2 #1        ; multiply: (3)*(2)
print #1
mul $3 $-2 #2       ; multiply: (3)*(-2)
print #2
mul $-2 $3 #3       ; multiply: (-2)*(3)
print #3
mul $-2 $-2 #4      ; multiply: (-2)*(-2)
print #4
mul $1 #1 #5        ; multiply: (1)*(6)
print #5
mul $-3 #1 #6       ; multiply: (-3)*(6)
print #6
mul #2 $-3 #7       ; multiply: (-6)*(-3)
print #7
mul #2 #3 #8        ; multiply: (-6)*(-6)
print #8
mul #3 #4 #9        ; multiply: (-6)*(4)
print #9
mul #8 #9 #10       ; multiply: (36)*(-24)
print #10
