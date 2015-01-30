/* Optimized memcpy implementation for PowerPC64/POWER8.
   Copyright (C) 2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <sysdep.h>

/* __ptr_t [r3] memcpy (__ptr_t dst [r3], __ptr_t src [r4], size_t len [r5]);
   Returns 'dst'.  */

	.machine power8
EALIGN (memcpy, 5, 0)
	cmpldi 7,5,15
	bgt 7,.L2
	rldicl. 9,5,0,63
	mr 9,3
	beq 0,.L3
	lbz 10,0(4)
	addi 9,3,1
	addi 4,4,1
	stb 10,0(3)
.L3:
	rldicl. 10,5,63,63
	beq 0,.L4
	lhz 10,0(4)
	addi 9,9,2
	addi 4,4,2
	sth 10,-2(9)
.L4:
	rldicl. 10,5,62,63
	beq 0,.L5
	lwz 10,0(4)
	addi 9,9,4
	addi 4,4,4
	stw 10,-4(9)
.L5:
	rldicl. 10,5,61,63
	beqlr 0
	ld 10,0(4)
	std 10,0(9)
	blr
	.p2align 4,,15
.L2:
	cmpldi 7,5,32
	ble 7,.L35
	cmpldi 7,5,64
	ble 7,.L36
	cmpldi 7,5,512
	std 26,-48(1)
	std 27,-40(1)
	std 28,-32(1)
	std 29,-24(1)
	std 30,-16(1)
	std 31,-8(1)
	bgt 7,.L10
	cmpldi 7,5,255
	mr 9,3
	bgt 7,.L37
.L11:
	cmpldi 7,5,127
	bgt 7,.L38
.L12:
	cmpldi 7,5,63
	bgt 7,.L39
.L13:
	cmpldi 7,5,32
	bgt 7,.L40
	cmpdi 7,5,0
	bne 7,.L41
.L7:
	ld 26,-48(1)
	ld 27,-40(1)
	ld 28,-32(1)
	ld 29,-24(1)
	ld 30,-16(1)
	ld 31,-8(1)
	blr
	.p2align 4,,15
.L35:
	lxvw4x 32,0,4
	addi 5,5,-16
	stxvw4x 32,0,3
	lxvw4x 32,4,5
	stxvw4x 32,3,5
	blr
	.p2align 4,,15
.L41:
	addi 5,5,-32
	li 10,16
	add 8,4,5
	lxvw4x 33,4,5
	lxvw4x 32,10,8
	add 8,9,5
	stxvw4x 33,9,5
	stxvw4x 32,10,8
	b .L7
	.p2align 4,,15
.L39:
	li 10,16
	lxvw4x 44,0,4
	addi 7,4,32
	addi 8,9,32
	addi 5,5,-64
	lxvw4x 32,10,4
	addi 4,4,64
	stxvw4x 44,0,9
	stxvw4x 32,10,9
	addi 9,9,64
	lxvw4x 32,0,7
	lxvw4x 33,10,7
	stxvw4x 33,10,8
	stxvw4x 32,0,8
	b .L13
	.p2align 4,,15
.L38:
	li 10,16
	lxvw4x 40,0,4
	addi 30,4,32
	addi 31,9,32
	addi 11,4,64
	lxvw4x 32,10,4
	addi 6,9,64
	addi 7,4,96
	addi 8,9,96
	addi 5,5,-128
	addi 4,4,128
	stxvw4x 40,0,9
	stxvw4x 32,10,9
	addi 9,9,128
	lxvw4x 32,0,30
	lxvw4x 44,10,30
	stxvw4x 44,10,31
	stxvw4x 32,0,31
	lxvw4x 32,0,11
	lxvw4x 33,10,11
	stxvw4x 33,10,6
	stxvw4x 32,0,6
	lxvw4x 32,0,7
	lxvw4x 40,10,7
	stxvw4x 40,10,8
	stxvw4x 32,0,8
	b .L12
	.p2align 4,,15
.L37:
	li 10,16
	lxvw4x 40,0,4
	addi 7,4,32
	addi 8,3,32
	addi 9,4,64
	lxvw4x 32,10,4
	addi 26,3,64
	addi 12,4,96
	addi 27,3,96
	addi 28,4,128
	addi 29,3,128
	addi 30,4,160
	stxvw4x 40,0,3
	addi 31,3,160
	addi 11,4,192
	addi 6,3,192
	addi 5,5,-256
	stxvw4x 32,10,3
	lxvw4x 33,0,7
	lxvw4x 32,10,7
	addi 7,4,224
	addi 4,4,256
	stxvw4x 33,0,8
	stxvw4x 32,10,8
	addi 8,3,224
	lxvw4x 33,0,9
	lxvw4x 32,10,9
	addi 9,3,256
	stxvw4x 33,0,26
	stxvw4x 32,10,26
	lxvw4x 32,0,12
	lxvw4x 44,10,12
	stxvw4x 44,10,27
	stxvw4x 32,0,27
	lxvw4x 32,0,28
	lxvw4x 33,10,28
	stxvw4x 33,10,29
	stxvw4x 32,0,29
	lxvw4x 32,0,30
	lxvw4x 40,10,30
	stxvw4x 40,10,31
	stxvw4x 32,0,31
	lxvw4x 32,0,11
	lxvw4x 44,10,11
	stxvw4x 44,10,6
	stxvw4x 32,0,6
	lxvw4x 32,0,7
	lxvw4x 33,10,7
	stxvw4x 33,10,8
	stxvw4x 32,0,8
	b .L11
	.p2align 4,,15
.L10:
	rlwinm 9,3,0,27,31
	li 10,16
	lxvw4x 44,0,4
	subfic 9,9,32
	li 12,16
	extsw 9,9
	lxvw4x 32,10,4
	li 0,32
	subf 28,9,5
	add 27,4,9
	addi 8,28,-128
	add 31,3,9
	stxvw4x 44,0,3
	srdi 8,8,7
	mr 9,27
	addi 8,8,1
	li 11,48
	stxvw4x 32,10,3
	li 4,64
	mr 10,31
	mtctr 8
	li 5,80
	li 6,96
	li 7,112
	.p2align 4,,15
.L15:
	lxvw4x 41,12,9
	lxvw4x 42,9,0
	li 29,16
	li 30,32
	lxvw4x 43,11,9
	lxvw4x 44,4,9
	li 8,48
	lxvw4x 45,5,9
	lxvw4x 33,6,9
	lxvw4x 32,7,9
	lxvw4x 40,0,9
	addi 9,9,128
	stxvw4x 40,0,10
	stxvw4x 41,12,10
	stxvw4x 42,10,0
	stxvw4x 43,11,10
	stxvw4x 44,4,10
	stxvw4x 45,5,10
	stxvw4x 33,6,10
	stxvw4x 32,7,10
	addi 10,10,128
	bdnz .L15
	rldicl 5,28,0,57
	cmpldi 7,5,63
	subf 10,5,28
	extsw 10,10
	add 4,27,10
	add 9,31,10
	ble 7,.L13
	lxvw4x 45,29,4
	lxvw4x 33,30,4
	rldicl 28,28,0,58
	lxvw4x 32,8,4
	lxvw4x 44,27,10
	subf 5,28,5
	rldicl 7,5,0,32
	mr 5,28
	add 4,4,7
	stxvw4x 44,31,10
	stxvw4x 45,29,9
	stxvw4x 33,30,9
	stxvw4x 32,8,9
	add 9,9,7
	b .L13
	.p2align 4,,15
.L36:
	li 9,16
	lxvw4x 33,0,4
	addi 5,5,-32
	add 8,4,5
	add 10,3,5
	lxvw4x 32,9,4
	stxvw4x 33,0,3
	stxvw4x 32,9,3
	lxvw4x 33,4,5
	lxvw4x 32,9,8
	stxvw4x 33,3,5
	stxvw4x 32,9,10
	blr
	.p2align 4,,15
.L40:
	li 10,16
	lxvw4x 40,0,4
	addi 5,5,-32
	add 7,4,5
	add 8,9,5
	lxvw4x 32,10,4
	stxvw4x 40,0,9
	stxvw4x 32,10,9
	lxvw4x 33,4,5
	lxvw4x 32,10,7
	stxvw4x 33,9,5
	stxvw4x 32,10,8
	b .L7
END_GEN_TB (memcpy,TB_TOCLESS)
libc_hidden_builtin_def (memcpy)
