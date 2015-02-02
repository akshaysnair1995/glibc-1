/* Optimized memmove implementation for PowerPC64/POWER8.
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


/* void* [r3] memmove (void *dest [r3], const void *src [r4], size_t len [r5])

   This optimization check if memory 'dest'  overlaps with 'src'. If it does
   not then it calls an optimized memcpy call (similar to memcpy for POWER7,
   embedded here to gain some cycles).
   If source and destiny overlaps, a optimized backwards memcpy is used
   instead.  */

	.machine power8
EALIGN (memmove, 5, 0)
	CALL_MCOUNT 3

L(_memmove):
	subf 9,4,3
	std 24,-64(1)
	std 25,-56(1)
	cmpld 7,9,5
	std 26,-48(1)
	std 27,-40(1)
	std 28,-32(1)
	std 29,-24(1)
	std 30,-16(1)
	std 31,-8(1)
	bge 7,.L98
	cmpldi 7,5,15
	add 4,4,5
	add 9,3,5
	ble 7,.L23
	cmpldi 7,5,512
	bgt 7,.L27
	cmpldi 7,5,255
	bgt 7,.L99
.L28:
	cmpldi 7,5,127
	bgt 7,.L100
.L29:
	cmpldi 7,5,63
	bgt 7,.L101
.L30:
	cmpldi 7,5,31
	bgt 7,.L102
.L31:
	cmpldi 7,5,15
	bgt 7,.L103
.L23:
	rldicl. 6,5,0,63
	beq 0,.L24
	lbz 10,-1(4)
	addi 9,9,-1
	addi 4,4,-1
	stb 10,0(9)
.L24:
	rldicl. 8,5,63,63
	beq 0,.L25
	lhz 10,-2(4)
	addi 9,9,-2
	addi 4,4,-2
	sth 10,0(9)
.L25:
	rldicl. 10,5,62,63
	beq 0,.L26
	lwz 10,-4(4)
	addi 9,9,-4
	addi 4,4,-4
	stw 10,0(9)
.L26:
	rldicl. 6,5,61,63
	beq 0,.L9
	ld 10,-8(4)
	std 10,-8(9)
.L9:
	ld 24,-64(1)
	ld 25,-56(1)
	ld 26,-48(1)
	ld 27,-40(1)
	ld 28,-32(1)
	ld 29,-24(1)
	ld 30,-16(1)
	ld 31,-8(1)
	blr
	.p2align 4,,15
.L98:
	cmpldi 7,5,15
	ble 7,.L104
	cmpldi 7,5,512
	bgt 7,.L10
	cmpldi 7,5,255
	mr 9,3
	bgt 7,.L105
.L11:
	cmpldi 7,5,127
	bgt 7,.L106
.L12:
	cmpldi 7,5,63
	bgt 7,.L107
.L13:
	cmpldi 7,5,31
	bgt 7,.L108
.L14:
	cmpldi 7,5,15
	bgt 7,.L109
.L4:
	rldicl. 6,5,0,63
	beq 0,.L5
	lbz 10,0(4)
	addi 9,9,1
	addi 4,4,1
	stb 10,-1(9)
.L5:
	rldicl. 8,5,63,63
	beq 0,.L6
	lhz 10,0(4)
	addi 9,9,2
	addi 4,4,2
	sth 10,-2(9)
.L6:
	rldicl. 10,5,62,63
	beq 0,.L7
	lwz 10,0(4)
	addi 9,9,4
	addi 4,4,4
	stw 10,-4(9)
.L7:
	rldicl. 6,5,61,63
	beq 0,.L9
	ld 10,0(4)
	ld 24,-64(1)
	ld 25,-56(1)
	ld 26,-48(1)
	ld 27,-40(1)
	ld 28,-32(1)
	ld 29,-24(1)
	ld 30,-16(1)
	ld 31,-8(1)
	std 10,0(9)
	blr
	.p2align 4,,15
.L104:
	mr 9,3
	b .L4
	.p2align 4,,15
.L103:
	li 10,-16
	addi 5,5,-16
	cmpldi 7,5,16
	lxvw4x 32,10,4
	addi 4,4,-16
	stxvw4x 32,10,9
	addi 9,9,-16
	bne 7,.L23
	lxvw4x 33,10,4
	stxvw4x 33,10,9
	b .L9
	.p2align 4,,15
.L101:
	li 8,-16
	li 10,-32
	addi 6,4,-32
	addi 7,9,-32
	lxvw4x 32,10,4
	lxvw4x 33,8,4
	addi 5,5,-64
	addi 4,4,-64
	stxvw4x 33,8,9
	stxvw4x 32,10,9
	addi 9,9,-64
	lxvw4x 32,8,6
	lxvw4x 40,10,6
	stxvw4x 40,10,7
	stxvw4x 32,8,7
	b .L30
	.p2align 4,,15
.L100:
	li 8,-16
	li 10,-32
	addi 29,4,-32
	addi 30,9,-32
	lxvw4x 32,10,4
	lxvw4x 44,8,4
	addi 31,4,-64
	addi 11,9,-64
	addi 6,4,-96
	addi 7,9,-96
	addi 5,5,-128
	addi 4,4,-128
	stxvw4x 44,8,9
	stxvw4x 32,10,9
	addi 9,9,-128
	lxvw4x 32,8,29
	lxvw4x 33,10,29
	stxvw4x 33,10,30
	stxvw4x 32,8,30
	lxvw4x 32,8,31
	lxvw4x 40,10,31
	stxvw4x 40,10,11
	stxvw4x 32,8,11
	lxvw4x 32,8,6
	lxvw4x 44,10,6
	stxvw4x 44,10,7
	stxvw4x 32,8,7
	b .L29
	.p2align 4,,15
.L99:
	li 8,-16
	li 10,-32
	addi 6,4,-32
	addi 7,9,-32
	lxvw4x 32,10,4
	lxvw4x 33,8,4
	addi 24,4,-64
	addi 25,9,-64
	addi 26,4,-96
	addi 12,9,-96
	addi 27,4,-128
	addi 28,9,-128
	addi 29,4,-160
	addi 30,9,-160
	addi 31,4,-192
	addi 11,9,-192
	stxvw4x 33,8,9
	addi 5,5,-256
	stxvw4x 32,10,9
	lxvw4x 32,8,6
	lxvw4x 40,10,6
	addi 6,4,-224
	addi 4,4,-256
	stxvw4x 40,10,7
	stxvw4x 32,8,7
	addi 7,9,-224
	addi 9,9,-256
	lxvw4x 32,8,24
	lxvw4x 44,10,24
	stxvw4x 44,10,25
	stxvw4x 32,8,25
	lxvw4x 32,8,26
	lxvw4x 33,10,26
	stxvw4x 33,10,12
	stxvw4x 32,8,12
	lxvw4x 32,8,27
	lxvw4x 40,10,27
	stxvw4x 40,10,28
	stxvw4x 32,8,28
	lxvw4x 32,8,29
	lxvw4x 44,10,29
	stxvw4x 44,10,30
	stxvw4x 32,8,30
	lxvw4x 32,8,31
	lxvw4x 33,10,31
	stxvw4x 33,10,11
	stxvw4x 32,8,11
	lxvw4x 32,8,6
	lxvw4x 40,10,6
	stxvw4x 40,10,7
	stxvw4x 32,8,7
	b .L28
	.p2align 4,,15
.L27:
	extsw 10,9
	rldicl. 6,10,0,63
	rldicl 29,10,0,59
	subf 28,29,5
	bne 0,.L110
	mr 7,4
	mr 8,9
.L32:
	rldicl. 6,10,63,63
	beq 0,.L33
	lhz 6,-2(7)
	addi 8,8,-2
	addi 7,7,-2
	sth 6,0(8)
.L33:
	rldicl. 6,10,62,63
	beq 0,.L34
	lwz 6,-4(7)
	addi 8,8,-4
	addi 7,7,-4
	stw 6,0(8)
.L34:
	rldicl. 6,10,61,63
	beq 0,.L35
	ld 6,-8(7)
	addi 8,8,-8
	addi 7,7,-8
	std 6,0(8)
.L35:
	rldicl. 6,10,60,63
	beq 0,.L36
	ld 10,-8(7)
	std 10,-8(8)
	ld 10,-16(7)
	std 10,-16(8)
.L36:
	addi 8,28,-128
	neg 29,29
	srdi 8,8,7
	add 4,4,29
	addi 8,8,1
	add 29,9,29
	mtctr 8
	mr 10,29
	mr 9,4
	li 30,-16
	li 31,-32
	li 12,-48
	li 0,-64
	li 11,-80
	li 5,-96
	li 6,-112
	li 7,-128
	.p2align 4,,15
.L37:
	lxvw4x 41,31,9
	lxvw4x 42,12,9
	lxvw4x 43,9,0
	lxvw4x 44,11,9
	lxvw4x 45,5,9
	lxvw4x 33,6,9
	lxvw4x 32,7,9
	lxvw4x 40,30,9
	addi 9,9,-128
	stxvw4x 40,30,10
	stxvw4x 41,31,10
	stxvw4x 42,12,10
	stxvw4x 43,10,0
	stxvw4x 44,11,10
	stxvw4x 45,5,10
	stxvw4x 33,6,10
	stxvw4x 32,7,10
	addi 10,10,-128
	bdnz .L37
	rldicl 5,28,0,57
	cmpldi 7,5,63
	subf 9,5,28
	extsw 9,9
	neg 9,9
	add 4,4,9
	add 9,29,9
	ble 7,.L30
	addi 7,5,-64
	mr 8,9
	srdi 7,7,6
	mr 10,4
	addi 7,7,1
	li 31,-16
	mtctr 7
	li 0,-32
	li 11,-48
	li 6,-64
	.p2align 4,,15
.L38:
	lxvw4x 45,31,10
	lxvw4x 33,10,0
	lxvw4x 32,11,10
	lxvw4x 44,6,10
	addi 10,10,-64
	stxvw4x 44,6,8
	stxvw4x 45,31,8
	stxvw4x 33,8,0
	stxvw4x 32,11,8
	addi 8,8,-64
	bdnz .L38
	rldicl 28,28,0,58
	subf 10,28,5
	mr 5,28
	rldicl 10,10,0,32
	neg 10,10
	add 4,4,10
	add 9,9,10
	b .L30
	.p2align 4,,15
.L102:
	li 8,-16
	li 10,-32
	addi 5,5,-32
	lxvw4x 32,8,4
	lxvw4x 44,10,4
	addi 4,4,-32
	stxvw4x 44,10,9
	stxvw4x 32,8,9
	addi 9,9,-32
	b .L31
	.p2align 4,,15
.L10:
	rlwinm 9,3,0,27,31
	subfic 9,9,32
	rldicl. 6,9,0,63
	extsw 7,9
	subf 12,7,5
	bne 0,.L111
	mr 8,4
	mr 10,3
.L15:
	rldicl. 6,9,63,63
	beq 0,.L16
	lhz 6,0(8)
	addi 10,10,2
	addi 8,8,2
	sth 6,-2(10)
.L16:
	rldicl. 6,9,62,63
	beq 0,.L17
	lwz 6,0(8)
	addi 10,10,4
	addi 8,8,4
	stw 6,-4(10)
.L17:
	rldicl. 6,9,61,63
	beq 0,.L18
	ld 6,0(8)
	addi 10,10,8
	addi 8,8,8
	std 6,-8(10)
.L18:
	rldicl. 6,9,60,63
	beq 0,.L19
	ld 6,0(8)
	addi 10,10,16
	addi 8,8,16
	std 6,-16(10)
	ld 6,-8(8)
	std 6,-8(10)
.L19:
	rldicl. 6,9,59,63
	beq 0,.L20
	li 9,16
	lxvw4x 32,0,8
	lxvw4x 33,9,8
	stxvw4x 33,9,10
	stxvw4x 32,0,10
.L20:
	addi 8,12,-128
	add 27,4,7
	srdi 8,8,7
	add 28,3,7
	addi 8,8,1
	mr 10,28
	mtctr 8
	mr 9,27
	li 31,16
	li 0,32
	li 11,48
	li 4,64
	li 5,80
	li 6,96
	li 7,112
	.p2align 4,,15
.L21:
	lxvw4x 41,31,9
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
	stxvw4x 41,31,10
	stxvw4x 42,10,0
	stxvw4x 43,11,10
	stxvw4x 44,4,10
	stxvw4x 45,5,10
	stxvw4x 33,6,10
	stxvw4x 32,7,10
	addi 10,10,128
	bdnz .L21
	rldicl 5,12,0,57
	cmpldi 7,5,63
	subf 10,5,12
	extsw 10,10
	add 4,27,10
	add 9,28,10
	ble 7,.L13
	lxvw4x 45,29,4
	lxvw4x 33,30,4
	rldicl 12,12,0,58
	lxvw4x 32,8,4
	lxvw4x 44,27,10
	subf 5,12,5
	rldicl 7,5,0,32
	mr 5,12
	add 4,4,7
	stxvw4x 44,28,10
	stxvw4x 45,29,9
	stxvw4x 33,30,9
	stxvw4x 32,8,9
	add 9,9,7
	b .L13
	.p2align 4,,15
.L109:
	lxvw4x 40,0,4
	addi 5,5,-16
	addi 4,4,16
	cmpldi 7,5,16
	stxvw4x 40,0,9
	addi 9,9,16
	bne 7,.L4
	lxvw4x 44,0,4
	stxvw4x 44,0,9
	b .L9
	.p2align 4,,15
.L108:
	li 10,16
	lxvw4x 32,0,4
	addi 5,5,-32
	lxvw4x 33,10,4
	addi 4,4,32
	stxvw4x 33,10,9
	stxvw4x 32,0,9
	addi 9,9,32
	b .L14
	.p2align 4,,15
.L110:
	lbz 6,-1(4)
	addi 7,4,-1
	addi 8,9,-1
	stb 6,-1(9)
	b .L32
	.p2align 4,,15
.L111:
	lbz 6,0(4)
	addi 8,4,1
	addi 10,3,1
	stb 6,0(3)
	b .L15
	.p2align 4,,15
.L105:
	li 10,16
	lxvw4x 33,0,4
	addi 7,4,32
	addi 8,3,32
	addi 9,4,64
	lxvw4x 32,10,4
	addi 25,3,64
	addi 26,4,96
	addi 27,3,96
	addi 28,4,128
	addi 29,3,128
	addi 30,4,160
	stxvw4x 33,0,3
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
	stxvw4x 33,0,25
	stxvw4x 32,10,25
	lxvw4x 32,0,26
	lxvw4x 40,10,26
	stxvw4x 40,10,27
	stxvw4x 32,0,27
	lxvw4x 32,0,28
	lxvw4x 44,10,28
	stxvw4x 44,10,29
	stxvw4x 32,0,29
	lxvw4x 32,0,30
	lxvw4x 33,10,30
	stxvw4x 33,10,31
	stxvw4x 32,0,31
	lxvw4x 32,0,11
	lxvw4x 40,10,11
	stxvw4x 40,10,6
	stxvw4x 32,0,6
	lxvw4x 32,0,7
	lxvw4x 44,10,7
	stxvw4x 44,10,8
	stxvw4x 32,0,8
	b .L11
	.p2align 4,,15
.L107:
	li 10,16
	lxvw4x 40,0,4
	addi 7,4,32
	addi 8,9,32
	addi 5,5,-64
	lxvw4x 32,10,4
	addi 4,4,64
	stxvw4x 40,0,9
	stxvw4x 32,10,9
	addi 9,9,64
	lxvw4x 32,0,7
	lxvw4x 44,10,7
	stxvw4x 44,10,8
	stxvw4x 32,0,8
	b .L13
	.p2align 4,,15
.L106:
	li 10,16
	lxvw4x 33,0,4
	addi 30,4,32
	addi 31,9,32
	addi 11,4,64
	lxvw4x 32,10,4
	addi 6,9,64
	addi 7,4,96
	addi 8,9,96
	addi 5,5,-128
	addi 4,4,128
	stxvw4x 33,0,9
	stxvw4x 32,10,9
	addi 9,9,128
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
	b .L12
END_GEN_TB (memmove, TB_TOCLESS)
libc_hidden_builtin_def (memmove)


/* void bcopy(const void *src [r3], void *dest [r4], size_t n [r5])
   Implemented in this file to avoid linker create a stub function call
   in the branch to '_memmove'.  */
ENTRY (bcopy)
	mr	r6,r3
	mr	r3,r4
	mr	r4,r6
	b	L(_memmove)
END (bcopy)
