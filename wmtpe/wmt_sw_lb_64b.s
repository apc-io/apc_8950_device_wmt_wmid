	.cpu	cortex-a9
	.fpu	neon

	.text
	.align	2
	.global	wmt_sw_lb_Neon_64byte_block
	.type	wmt_sw_lb_Neon_64byte_block, %function
	@
	@	void wmt_sw_lb(uint8_t *src, uint8_t *dst, int width, int height, int stride)
	@

	@
	@	Constraints: width = 64x, where x > 0
	@
wmt_sw_lb_Neon_64byte_block:
	cmp		r3, #0
	ldr		ip, [sp]					@	ip = stride

	bxeq	lr
	push	{r4-r9}

	sub		r6, ip, r2					@	off = stride - width
	mov		r8, #0						@	reset h

	@	r0 = src, r1 = dst, r2 = width, r3 = height
	@	ip = stride
	@
	@	r4 = pre, r5 = nxt, r6 = off, r7 = w, r8 = h
	@
1:
	pld		[r0]
	pld		[r0, #32]

	cmp		r8, #0						@	h =? 0
	sub		r4, r0, ip					@	pre = src - stride

	moveq	r4, r0
	add		r9, r8, #1					@	h + 1

	pld		[r4]
	pld		[r4, #32]

	add		r5, r0, ip					@	nxt = src + stride
	cmp		r9, r3						@	h+1 =? height

	lsr		r7, r2, #6					@	w = width >> 6
	moveq	r5, r0

	pld		[r5]
	pld		[r5, #32]

2:
	vld1.8	{q10, q11}, [r0,:256]!		@	load first 32byte from src
	pld		[r0, #32]
	pld		[r0, #64]

	vld1.8	{q8, q9}, [r4,:256]!		@	load first 32byte from pre
	pld		[r4, #32]
	pld		[r4, #64]

	vld1.8	{q12, q13}, [r5,:256]!		@	load first 32byte from nxt
	pld		[r5, #32]
	pld		[r5, #64]
	
	vshll.u8	q0, d20, #1				@	src[0~7] << 1
	vshll.u8	q1, d21, #1				@	src[8~15] << 1
	vshll.u8	q2, d22, #1				@	src[16~23] << 1
	vshll.u8	q3, d23, #1				@	src[24~31] << 1

	vaddw.u8	q0, q0, d16
	vaddw.u8	q1, q1, d17
	vaddw.u8	q2, q2, d18
	vaddw.u8	q3, q3, d19

	vaddw.u8	q0, q0, d24
	vaddw.u8	q1, q1, d25
	vaddw.u8	q2, q2, d26
	vaddw.u8	q3, q3, d27

	vld1.8	{q10, q11}, [r0,:256]!		@	load last 32byte from src

	vld1.8	{q8, q9}, [r4,:256]!		@	load last 32byte from pre

	vld1.8	{q12, q13}, [r5,:256]!		@	load last 32byte from nxt

	vshrn.i16	d28, q0, #2
	vshrn.i16	d29, q1, #2
	vshrn.i16	d30, q2, #2
	vshrn.i16	d31, q3, #2

	vshll.u8	q0, d20, #1				@	src[32~39] << 1
	vshll.u8	q1, d21, #1				@	src[40~47] << 1
	vshll.u8	q2, d22, #1				@	src[48~55] << 1
	vshll.u8	q3, d23, #1				@	src[56~63] << 1

	vaddw.u8	q0, q0, d16
	vaddw.u8	q1, q1, d17
	vaddw.u8	q2, q2, d18
	vaddw.u8	q3, q3, d19
	
	vaddw.u8	q0, q0, d24
	vaddw.u8	q1, q1, d25
	vaddw.u8	q2, q2, d26
	vaddw.u8	q3, q3, d27

	vshrn.i16	d0, q0, #2
	vshrn.i16	d1, q1, #2
	vshrn.i16	d2, q2, #2
	vshrn.i16	d3, q3, #2

	vst1.8	{q14, q15}, [r1,:256]!
		subs	r7, r7, #1
	vst1.8	{q0, q1}, [r1,:256]!
		bne	2b

	add		r8, r8, #1
	add		r0, r0, r6					@	src += off

	cmp		r8, r3
	add		r1, r1, r6					@	dst += off

	blt		1b

	pop		{r4-r9}
	bx		lr
	.size	wmt_sw_lb_Neon_64byte_block, .-wmt_sw_lb_Neon_64byte_block
