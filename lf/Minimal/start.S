.section .text
.globl _start
.type _start,@function

_start:
  // set stack pointer
  li sp, 0x20001000

  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop

  // set argc, argv
  // li a0, 0x0
  // li a1, 0x0

  // jump to main
  call main
