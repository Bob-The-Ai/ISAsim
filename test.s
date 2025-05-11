ldi s1, 0xdead
sll s2, s1, 16
ldi s1, 0xbeef
add s1, s2, s1
ldi sf, 0x0001
brc s1, 0, 2
stw sf, s1

ldi s1, 0x1
sll s1, s1, 16
ldi s2, 0x0011
add s1, s1, s2
# s1 ready
ldi s2, 0xa
sll s2, s2, 16
ldi s3, 0x00a4
add s2, s2, s3
# s2 ready
add s2, s2, s1
stw sf, s2

ldi s1, 1
neg s1, s1
ldi s2, 15
stw s2, s2
add s2, s2, s1
tst s3, s2
brc s3, 0x80, -4

hlt
