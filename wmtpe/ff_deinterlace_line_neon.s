# 1 "ff_deinterlace_line_neon.s1"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "ff_deinterlace_line_neon.s1"
 .cpu cortex-a9
 .fpu neon
 .text
 .align 2
 .global ff_deinterlace_line_neon
 .type ff_deinterlace_line_neon, %function
 @
 @ void ff_deinterlace_line_neon(uint8_t *dst, const uint8_t *lum_m4, const uint8_t *lum_m3, const uint8_t *lum_m2,
 @ const uint8_t *lum_m1, const uint8_t *lum, int size)
 @
ff_deinterlace_line_neon:
 ldr ip, [sp, #8] @ ip = size

 push {r4-r5}

 cmp ip, #16 @ >= 16pixel?

 ldrd r4, [sp, #8] @ r4 = lum_m1, r5 = lum
 blo 15f

 @ r0 = dst, r1 = lum_m4, r2 = lum_m3, r3 = lum_m2, r4 = lum_m1, r5 = lum, ip = size
16:
 vld1.8 {q11}, [r4]! @ *lum_m1
 @ ~1cycle stall
 vld1.8 {q9}, [r2]! @ *lum_m3
 @ ~1cycle stall
 vld1.8 {q8}, [r1]! @ *lum_m4
 @ ~1cycle stall
 vld1.8 {q12}, [r5]! @ *lum
 @ ~1cycle stall
 vld1.8 {q10}, [r3]! @ *lum_m2
 @ ~1cycle stall

 vaddl.u8 q0, d18, d22
 vaddl.u8 q1, d19, d23 @ (short)(*lum_m1 + *lum_m3)
 vaddl.u8 q2, d16, d24
 vaddl.u8 q3, d17, d25 @ (short)(*lum_m4 + *lum)
 vshll.u8 q14, d20, #1
 vshll.u8 q15, d21, #1 @ (short)(*lum_m2 << 1)
 vshl.i16 q0, q0, #2
 vshl.i16 q1, q1, #2 @ (short)((*lum_m1 + *lum_m3) << 2)
 vsub.i16 q2, q14, q2
 vsub.i16 q3, q15, q3 @ (short)((*lum_m2 << 1) - (*lum_m4 + *lum))
 vadd.i16 q2, q0, q2
 vadd.i16 q3, q1, q3 @ (short)((*lum_m2 << 1) + ((*lum_m1 + *lum_m3) << 2) - (*lum_m4 + *lum))
 @ 1cycle stall
 vqshrun.s16 d0, q2, #3
 vqshrun.s16 d1, q3, #3 @ saturate result from short to unsigned byte
  subs ip, ip, #16

 @ 2cycle stall
 vst1.8 {q0}, [r0]!
  bhs 16b

 adds ip, ip, #16 @ compensate pre-subtract #16

 popeq {r4-r5}
 bxeq lr

15:
 tst ip, #8

 beq 7f
8:
 vld1.8 {d22}, [r4]! @ *lum_m1
 @ ~1cycle stall
 vld1.8 {d18}, [r2]! @ *lum_m3
 @ ~1cycle stall
 vld1.8 {d16}, [r1]! @ *lum_m4
 @ ~1cycle stall
 vld1.8 {d24}, [r5]! @ *lum
 @ ~1cycle stall
 vld1.8 {d20}, [r3]! @ *lum_m2
 @ ~1cycle stall

 vaddl.u8 q0, d18, d22 @ (short)(*lum_m1 + *lum_m3)
 vaddl.u8 q2, d16, d24 @ (short)(*lum_m4 + *lum)
 vshll.u8 q14, d20, #1 @ (short)(*lum_m2 << 1)
 vshl.i16 q0, q0, #2 @ (short)((*lum_m1 + *lum_m3) << 2)
 vsub.i16 q2, q14, q2 @ (short)((*lum_m2 << 1) - (*lum_m4 + *lum))
 @ 1cycle stall
 vadd.i16 q2, q0, q2 @ (short)((*lum_m2 << 1) + ((*lum_m1 + *lum_m3) << 2) - (*lum_m4 + *lum))
 @ 2cycle stall
 vqshrun.s16 d0, q2, #3 @ saturate result from short to unsigned byte

 @ 3cycle stall
 vst1.8 {d0}, [r0]!

7:
 tst ip, #7

 beq 0f

 vld1.8 {d22}, [r4]! @ *lum_m1
 @ ~1cycle stall
 vld1.8 {d18}, [r2]! @ *lum_m3
 @ ~1cycle stall
 vld1.8 {d16}, [r1]! @ *lum_m4
 @ ~1cycle stall
 vld1.8 {d24}, [r5]! @ *lum
 @ ~1cycle stall
 vld1.8 {d20}, [r3]! @ *lum_m2
 @ ~1cycle stall

 vaddl.u8 q0, d18, d22 @ (short)(*lum_m1 + *lum_m3)
 vaddl.u8 q2, d16, d24 @ (short)(*lum_m4 + *lum)
 vshll.u8 q14, d20, #1 @ (short)(*lum_m2 << 1)
 vshl.i16 q0, q0, #2 @ (short)((*lum_m1 + *lum_m3) << 2)
 vsub.i16 q2, q14, q2 @ (short)((*lum_m2 << 1) - (*lum_m4 + *lum))
 @ 1cycle stall
 vadd.i16 q2, q0, q2 @ (short)((*lum_m2 << 1) + ((*lum_m1 + *lum_m3) << 2) - (*lum_m4 + *lum))
 @ 2cycle stall
 vqshrun.s16 d0, q2, #3 @ saturate result from short to unsigned byte

 @ 3cycle stall
 vmov r2, r3, d0
4:
 tst ip, #4

 strne r2, [r0], #4
 movne r3, r2

2:
 tst ip, #2

 strneh r2, [r0], #2
 lsrne r2, r2, #16

1:
 tst ip, #1

 strneb r2, [r0], #1

0:
 pop {r4-r5}
 bx lr
 .size ff_deinterlace_line_neon, .-ff_deinterlace_line_neon
