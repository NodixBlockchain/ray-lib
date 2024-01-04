%define PREC_SHIFT 8

section .code

global _intersect_ray_box_mmx

_intersect_ray_box_mmx :

	mov ecx,esp
	
	;mov eax,[esp+4]
	;mov [r_sign],eax

	;mov eax,[esp+8]
	;mov [r_origin],eax
	
	;mov eax,[esp+12]
	;mov [r_inv_dir],eax
	
	;mov eax,[esp+16]
	;mov [o_params],eax
	
	;mov eax,[esp+20]
	;mov [t0],eax	

	;mov eax,[esp+24]
	;mov [t1],eax	
	
	pusha

	mov eax,[ecx+12]
		
	MOVAPS xmm0,[eax]

	
	mov eax,[ecx+4]
	PSHUFD  xmm1,[eax],01010000b
	PSUBD   xmm0,xmm1

	mov eax,[ecx+8]
	PSHUFD	xmm3,[eax],01010000b

	
	movshdup xmm1,xmm0				;xmm1 = v1,v1,v3,v3
	pmuludq  xmm0,xmm3           	;xmm0 = v0*inv_dir[0],v2*inv_dir[1]
	pmuludq  xmm1,xmm3           	;xmm1 = v1*inv_dir[0],v3*inv_dir[1]
	
	movaps   xmm5,xmm0				;xmm5 = v0*inv_dir[0],v2*inv_dir[1]
	PSHUFD   xmm6,xmm1,01001110b	;xmm6 = v3*inv_dir[0],v1*inv_dir[1]
	
	PCMPGTD		xmm5,xmm6
	PMOVMSKB	eax,xmm5
	test		eax,eax
	jnz test_ok
		popa
		mov eax,0
		ret
	test_ok:
	
	
	mov esi,[ecx+16]
	
		
	
	movd eax,xmm1	;eax = v1
	movd ebx,xmm6	;ebx = v3
	
	cmp ebx,eax
	jg _c1
		sar ebx,PREC_SHIFT
		mov [esi+4],ebx
	jmp _c2
	_c1:
		sar eax,PREC_SHIFT
		mov [esi+4],eax
	_c2:
	
	movd	eax,xmm0	;eax = v0
	shufps  xmm0,xmm0	,10101010b 	;xmm0 = i*v0,i*v1,i*v2,i*v3
	movd	ebx,xmm0	;eax = v2
	
	cmp eax,ebx
	jl _c3
		sar eax,PREC_SHIFT
		mov [esi],eax
	jmp _c4
	_c3:
		sar ebx,PREC_SHIFT
		mov [esi],ebx
	_c4:
	

	
	mov edx,[ecx+20]

	MOVAPS xmm0,[edx]


	mov eax,[ecx+4]
	PSHUFD  xmm1,[eax],10101010b
	PSUBD   xmm0,xmm1

	mov eax,[ecx+8]
	PSHUFD	xmm3,[eax],10101010b
	
	pmuludq  xmm0,xmm3           	;xmm1 = v0*inv_dir[0],v2*inv_dir[1]
			
	shufps   xmm0,xmm0,10001000b 	;xmm0 = i*v0,i*v2,i*v1,i*v3
	PSRAD   xmm0,PREC_SHIFT
	
	movd	ebx,xmm0	
	shufps  xmm0,xmm0	,01010101b 	
	movd	eax,xmm0

	cmp [esi],eax
	jle _test1_ok
		popa
		mov eax,0
		ret
	_test1_ok:

	cmp [esi+4],ebx
	jg _test2_ok
		popa
		mov eax,0
		ret
	_test2_ok:

	
	cmp ebx,[esi]
	jl __c1
		mov [esi],ebx
	__c1:
	
	cmp eax,[esi+4]
	jg __c2
		mov [esi+4],eax
	__c2:

	popa	
	mov eax,1
ret


