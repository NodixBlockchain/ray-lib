

section .code

global _has_cpuid
global _get_cpu_feat

;Note that prior to use the CPUID instruction, you should also make sure the processor supports it by testing the 'ID' bit (0x200000) in eflags 
;(modifiable only when the CPUID instruction is supported. For systems that don't support CPUID, writing a '1' at that place will have no effect). 

_has_cpuid:
	push ecx
	pushfd ; get
	pop eax
	mov ecx, eax ; save 
	xor eax, 0x200000 ; flip
	push eax ; set
	popfd
	pushfd ; and test
	pop eax
	xor eax, ecx ; mask changed bits
	shr eax, 21 ; move bit 21 to bit 0
	and eax, 1 ; and mask others
	push ecx
	popfd ; restore original flags
	pop ecx
ret

_get_cpu_feat:

	mov eax , [esp+4]
	pusha
	
		mov edi , eax
		mov eax	, 1
		cpuid
				
		mov [edi]    , eax
		mov [edi+4]  , ebx
		mov [edi+8]  , ecx
		mov [edi+12] , edx
	
	popa

ret