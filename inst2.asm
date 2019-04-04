j label
lw $4, 0($1)
add $3, $4, $8
addi $1, $2, 100
mult $2, $3
mfhi $1
sra $3, $2, 10
slti $1, $3, 1023
lw $1, 100($3)
