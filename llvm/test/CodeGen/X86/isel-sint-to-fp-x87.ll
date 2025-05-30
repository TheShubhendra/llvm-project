; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py UTC_ARGS: --version 5
; NOTE: Fast Isel is not added because it does not support x87 stores.

; RUN: llc < %s -mtriple=x86_64-- -mattr=+x87,-sse,-sse2 -global-isel=0 | FileCheck %s --check-prefixes X64,SDAG-X64
; RUN: llc < %s -mtriple=x86_64-- -mattr=+x87,-sse,-sse2 -global-isel -global-isel-abort=2 | FileCheck %s --check-prefixes X64,GISEL-X64
; RUN: llc < %s -mtriple=i686-- -mattr=+x87,-sse,-sse2 -global-isel=0 | FileCheck %s --check-prefixes X86,SDAG-X86
; RUN: llc < %s -mtriple=i686-- -mattr=+x87,-sse,-sse2 -global-isel -global-isel-abort=2 | FileCheck %s --check-prefixes X86,GISEL-X86

define void @test_int8_to_float(i8 %x, ptr %p) nounwind {
; X64-LABEL: test_int8_to_float:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstps (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int8_to_float:
; X86:       # %bb.0: # %entry
; X86-NEXT:    pushl %eax
; X86-NEXT:    movsbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    filds {{[0-9]+}}(%esp)
; X86-NEXT:    fstps (%ecx)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
entry:
  %conv = sitofp i8 %x to float
  store float %conv, ptr %p, align 4
  ret void
}

define void @test_int16_to_float(i16 %x, ptr %p) nounwind {
; X64-LABEL: test_int16_to_float:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movw %di, -{{[0-9]+}}(%rsp)
; X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstps (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int16_to_float:
; X86:       # %bb.0: # %entry
; X86-NEXT:    pushl %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movw %cx, {{[0-9]+}}(%esp)
; X86-NEXT:    filds {{[0-9]+}}(%esp)
; X86-NEXT:    fstps (%eax)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
entry:
  %conv = sitofp i16 %x to float
  store float %conv, ptr %p, align 4
  ret void
}

define void @test_int32_to_float(i32 %x, ptr %p) nounwind {
; X64-LABEL: test_int32_to_float:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movl %edi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildl -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstps (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int32_to_float:
; X86:       # %bb.0: # %entry
; X86-NEXT:    pushl %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movl %ecx, (%esp)
; X86-NEXT:    fildl (%esp)
; X86-NEXT:    fstps (%eax)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
entry:
  %conv = sitofp i32 %x to float
  store float %conv, ptr %p, align 4
  ret void
}

define void @test_int64_to_float(i64 %x, ptr %p) nounwind {
; X64-LABEL: test_int64_to_float:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movq %rdi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildll -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstps (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int64_to_float:
; X86:       # %bb.0: # %entry
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    fildll {{[0-9]+}}(%esp)
; X86-NEXT:    fstps (%eax)
; X86-NEXT:    retl
entry:
  %conv = sitofp i64 %x to float
  store float %conv, ptr %p, align 4
  ret void
}

define x86_fp80 @test_int8to_longdouble(i8 %a) nounwind {
; X64-LABEL: test_int8to_longdouble:
; X64:       # %bb.0:
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; X64-NEXT:    retq
;
; X86-LABEL: test_int8to_longdouble:
; X86:       # %bb.0:
; X86-NEXT:    pushl %eax
; X86-NEXT:    movsbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    filds {{[0-9]+}}(%esp)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
  %conv = sitofp i8 %a to x86_fp80
  ret x86_fp80 %conv
}

define x86_fp80 @test_int16_to_longdouble(i16 %a) nounwind {
; X64-LABEL: test_int16_to_longdouble:
; X64:       # %bb.0:
; X64-NEXT:    movw %di, -{{[0-9]+}}(%rsp)
; X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; X64-NEXT:    retq
;
; X86-LABEL: test_int16_to_longdouble:
; X86:       # %bb.0:
; X86-NEXT:    pushl %eax
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    filds {{[0-9]+}}(%esp)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
  %conv = sitofp i16 %a to x86_fp80
  ret x86_fp80 %conv
}

define x86_fp80 @test_int32_to_longdouble(i32 %a) nounwind {
; X64-LABEL: test_int32_to_longdouble:
; X64:       # %bb.0:
; X64-NEXT:    movl %edi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildl -{{[0-9]+}}(%rsp)
; X64-NEXT:    retq
;
; X86-LABEL: test_int32_to_longdouble:
; X86:       # %bb.0:
; X86-NEXT:    pushl %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl %eax, (%esp)
; X86-NEXT:    fildl (%esp)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
  %conv = sitofp i32 %a to x86_fp80
  ret x86_fp80 %conv
}

define x86_fp80 @test_int64_to_longdouble(i64 %a, ptr %p) nounwind {
; X64-LABEL: test_int64_to_longdouble:
; X64:       # %bb.0:
; X64-NEXT:    movq %rdi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildll -{{[0-9]+}}(%rsp)
; X64-NEXT:    retq
;
; X86-LABEL: test_int64_to_longdouble:
; X86:       # %bb.0:
; X86-NEXT:    fildll {{[0-9]+}}(%esp)
; X86-NEXT:    retl
  %conv = sitofp i64 %a to x86_fp80
  ret x86_fp80 %conv
}


define void @test_int8to_double(i8 %x, ptr %p) nounwind {
; X64-LABEL: test_int8to_double:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstpl (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int8to_double:
; X86:       # %bb.0: # %entry
; X86-NEXT:    pushl %eax
; X86-NEXT:    movsbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    filds {{[0-9]+}}(%esp)
; X86-NEXT:    fstpl (%ecx)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
entry:
  %conv = sitofp i8 %x to double
  store double %conv, ptr %p, align 4
  ret void
}

define void @test_int16_to_double(i16 %x, ptr %p) nounwind {
; X64-LABEL: test_int16_to_double:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movw %di, -{{[0-9]+}}(%rsp)
; X64-NEXT:    filds -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstpl (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int16_to_double:
; X86:       # %bb.0: # %entry
; X86-NEXT:    pushl %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movw %cx, {{[0-9]+}}(%esp)
; X86-NEXT:    filds {{[0-9]+}}(%esp)
; X86-NEXT:    fstpl (%eax)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
entry:
  %conv = sitofp i16 %x to double
  store double %conv, ptr %p, align 4
  ret void
}

define void @test_int32_to_double(i32 %x, ptr %p) nounwind {
; X64-LABEL: test_int32_to_double:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movl %edi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildl -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstpl (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int32_to_double:
; X86:       # %bb.0: # %entry
; X86-NEXT:    pushl %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movl %ecx, (%esp)
; X86-NEXT:    fildl (%esp)
; X86-NEXT:    fstpl (%eax)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
entry:
  %conv = sitofp i32 %x to double
  store double %conv, ptr %p, align 4
  ret void
}

define void @test_int64_to_double(i64 %x, ptr %p) nounwind {
; X64-LABEL: test_int64_to_double:
; X64:       # %bb.0: # %entry
; X64-NEXT:    movq %rdi, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fildll -{{[0-9]+}}(%rsp)
; X64-NEXT:    fstpl (%rsi)
; X64-NEXT:    retq
;
; X86-LABEL: test_int64_to_double:
; X86:       # %bb.0: # %entry
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    fildll {{[0-9]+}}(%esp)
; X86-NEXT:    fstpl (%eax)
; X86-NEXT:    retl
entry:
  %conv = sitofp i64 %x to double
  store double %conv, ptr %p, align 4
  ret void
}
;; NOTE: These prefixes are unused and the list is autogenerated. Do not add tests below this line:
; GISEL-X86: {{.*}}
; GISEL-X64: {{.*}}
; SDAG-X86: {{.*}}
; SDAG-X64: {{.*}}
