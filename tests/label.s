; Test for label
; Output 10! result
add $0 $1 #0    ; result
add $0 $10 #1   ; start from 10
go:
mul #0 #1 #0    ; result = result + n
sub #1 $1 #1    ; n = n - 1
jnz #1 :go      ; jump to go if #1 not zero

jmp :output       ; jump to output
add $0 $100 #1    ; should not be executed, #1 remained 0
; print the result
output:
print "10! ="
print #0        ; check result is 3628800
print #1        ; check #1 is 0
