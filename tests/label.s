call :E
print $8
halt

A:
print $1
jge $5 :C

B:
print $2

D:
print $4
jgt $0 :A
jlt $-5 :G

F:
print $6
jmp :B

C:
jle $1 :G
print $3
jz $0 :F

E:
print $5
jnz $4 :A

G:
print $7
ret

