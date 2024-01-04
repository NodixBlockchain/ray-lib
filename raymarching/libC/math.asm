
section .code

global _libc_sinf
global _libc_cosf
global _libc_atanf

global _libc_sind
global _libc_cosd
global _libc_sqrtd
global _libc_atand



;----------------
;float
;----------------

_libc_sinf:
	fld dword [esp+4]
	fsin
ret

_libc_cosf:
	fld dword [esp+4]
	fcos
ret

_libc_atanf:
	fld dword [esp+4]
    fld1 	
	fpatan
ret


_libc_sqrtd:
	fld qword [esp+4]
	fsqrt
ret

;----------------
;double
;----------------


_libc_sind:
	fld qword [esp+4]
	fsin
ret

_libc_cosd:
	fld qword [esp+4]
	fcos
ret


_libc_atand:
	fld qword [esp+4]
    fld1 	
	fpatan
ret

