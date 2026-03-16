.text
# Longer mixed program (~120+ instructions) to stress arithmetic, memory, control

start:
        addi x1, x0, 0        # i
        addi x2, x0, 1        # one
        addi x3, x0, 32       # outer limit
        addi x4, x0, 0        # outer sum

        lui  x5, 0x10010      # base high for array A
        addi x5, x5, 0
        lui  x6, 0x10020      # base high for array B
        addi x6, x6, 0

outer_loop:
        add  x7, x1, x2       # t = i + 1
        sll  x8, x7, x2       # t2 = t << 1
        add  x4, x4, x8       # outer_sum += t2

        sll  x9, x1, x2       # idx = i << 1
        add  x10, x5, x9      # addrA
        add  x11, x6, x9      # addrB

        sw   x7, 0(x10)       # A[i] = t
        sw   x8, 0(x11)       # B[i] = t2

        addi x1, x1, 1
        blt  x1, x3, outer_loop

        # inner loops: compute dot product of A and B into x20
        addi x12, x0, 0       # i = 0
        addi x20, x0, 0       # dot = 0

dot_loop:
        sll  x9, x12, x2      # idx = i << 1
        add  x10, x5, x9      # addrA
        add  x11, x6, x9      # addrB
        lw   x13, 0(x10)      # A[i]
        lw   x14, 0(x11)      # B[i]

        add  x15, x13, x14
        sub  x16, x14, x13
        and  x17, x15, x16
        or   x18, x15, x16
        xor  x19, x17, x18

        add  x20, x20, x19    # accumulate

        addi x12, x12, 1
        blt  x12, x3, dot_loop

        # second phase: walk backwards with branches and jumps
        addi x21, x3, -1      # i = limit - 1
        addi x22, x0, 0       # acc2 = 0

back_loop:
        sll  x9, x21, x2
        add  x10, x5, x9
        lw   x23, 0(x10)

        add  x22, x22, x23
        addi x21, x21, -1
        bge  x21, x0, back_loop

        # small branch/jump maze
        add  x24, x20, x22
        addi x25, x0, 1000
        blt  x24, x25, path_small
        jal  x0, path_big

path_small:
        addi x26, x0, 1
        add  x27, x26, x24
        beq  x27, x24, ps_eq
        jal  x0, ps_end
ps_eq:
        addi x28, x0, 2
ps_end:
        jal  x0, merge

path_big:
        addi x26, x0, -1
        add  x27, x26, x24
        bne  x27, x24, pb_ne
        jal  x0, pb_end
pb_ne:
        addi x28, x0, 3
pb_end:
        jal  x0, merge

merge:
        add  x29, x24, x28

        # Another loop with conditional branches
        addi x30, x0, 0       # k = 0
        addi x31, x0, 0       # acc3 = 0

loop2:
        add  x31, x31, x30
        addi x30, x30, 1
        blt  x30, x3, loop2

        # Final combination into x10..x15 for inspection
        add  x10, x20, x22
        add  x11, x29, x31
        sub  x12, x10, x11
        and  x13, x10, x11
        or   x14, x10, x11
        xor  x15, x10, x11

end:
        add  x16, x10, x11

