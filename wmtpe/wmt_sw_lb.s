	.cpu	cortex-a9
	.fpu	neon

	.text
	.align	2
	.global	wmt_sw_lb_Neon
	.type	wmt_sw_lb_Neon, %function
	@
	@	void wmt_sw_lb(uint8_t *src, uint8_t *dst, int width, int height, int stride)
	@

	@
	@	Constraints: width = 16x, where x > 0
	@
wmt_sw_lb_Neon:
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
	cmp		r8, #0						@	h =? 0
	sub		r4, r0, ip					@	pre = src - stride

	moveq	r4, r0
	add		r9, r8, #1					@	h + 1

	add		r5, r0, ip					@	nxt = src + stride
	cmp		r9, r3						@	h+1 =? height

	lsr		r7, r2, #4					@	w = width >> 4
	moveq	r5, r0

2:
	vld1.8	{q9}, [r0, :128]!			@	load 16byte from src

	vld1.8	{q8}, [r4, :128]!			@	load 16byte from pre

	vld1.8	{q10}, [r5, :128]!			@	load 16byte from nxt
	
	vshll.u8	q0, d18, #1				@	first half src[16] << 1

	vshll.u8	q1, d19, #1				@	last half src[16] << 1

	vaddl.u8	q2, d16, d20

	vaddl.u8	q3, d17, d21
	
	vadd.i16	q0, q0, q2

	vadd.i16	q1, q1, q3

	vshrn.i16	d0, q0, #2

	vshrn.i16	d1, q1, #2
		subs	r7, r7, #1

	vst1.8	{q0}, [r1, :128]!
		bne	2b

	add		r8, r8, #1
	add		r0, r0, r6					@	src += off

	cmp		r8, r3
	add		r1, r1, r6					@	dst += off

	blt		1b

	pop		{r4-r9}
	bx		lr
	.size	wmt_sw_lb_Neon, .-wmt_sw_lb_Neon
