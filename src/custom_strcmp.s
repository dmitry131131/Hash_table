global _Z13strcmp_customDv4_xS_

section .text

_Z13strcmp_customDv4_xS_:
        push    rbp                         
        mov     rbp, rsp

        vpcmpeqb        ymm0, ymm1, ymm0
        vpmovmskb       eax, ymm0
        not     eax

        pop rbp
        ret