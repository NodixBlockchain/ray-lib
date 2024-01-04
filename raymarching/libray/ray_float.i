#line 1 ".\\ray_float.c"
#line 1 ".\\../libC/libc_math.h"

#line 1 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"








 





 







 
#line 1 "C:\\Program Files\\Intel\\Compiler\\11.1\\054\\include\\mmintrin.h"









 


 















typedef union __declspec(align(8)) {__int64 __m;} __m64;
#line 46 "C:\\Program Files\\Intel\\Compiler\\11.1\\054\\include\\mmintrin.h"

 
extern void  __cdecl _m_empty(void);
extern __m64 __cdecl _m_from_int(int i);
extern int   __cdecl _m_to_int(__m64 m);
extern __m64 __cdecl _m_packsswb(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_packssdw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_packuswb(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_punpckhbw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_punpckhwd(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_punpckhdq(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_punpcklbw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_punpcklwd(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_punpckldq(__m64 m1, __m64 m2);

 
extern __m64 __cdecl _m_paddb(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_paddw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_paddd(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_paddsb(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_paddsw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_paddusb(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_paddusw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_psubb(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_psubw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_psubd(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_psubsb(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_psubsw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_psubusb(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_psubusw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_pmaddwd(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_pmulhw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_pmullw(__m64 m1, __m64 m2);

 
extern __m64 __cdecl _m_psllw(__m64 m, __m64 count);
extern __m64 __cdecl _m_psllwi(__m64 m, int count);
extern __m64 __cdecl _m_pslld(__m64 m, __m64 count);
extern __m64 __cdecl _m_pslldi(__m64 m, int count);
extern __m64 __cdecl _m_psllq(__m64 m, __m64 count);
extern __m64 __cdecl _m_psllqi(__m64 m, int count);
extern __m64 __cdecl _m_psraw(__m64 m, __m64 count);
extern __m64 __cdecl _m_psrawi(__m64 m, int count);
extern __m64 __cdecl _m_psrad(__m64 m, __m64 count);
extern __m64 __cdecl _m_psradi(__m64 m, int count);
extern __m64 __cdecl _m_psrlw(__m64 m, __m64 count);
extern __m64 __cdecl _m_psrlwi(__m64 m, int count);
extern __m64 __cdecl _m_psrld(__m64 m, __m64 count);
extern __m64 __cdecl _m_psrldi(__m64 m, int count);
extern __m64 __cdecl _m_psrlq(__m64 m, __m64 count);
extern __m64 __cdecl _m_psrlqi(__m64 m, int count);

 
extern __m64 __cdecl _m_pand(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_pandn(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_por(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_pxor(__m64 m1, __m64 m2);

 
extern __m64 __cdecl _m_pcmpeqb(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_pcmpeqw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_pcmpeqd(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_pcmpgtb(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_pcmpgtw(__m64 m1, __m64 m2);
extern __m64 __cdecl _m_pcmpgtd(__m64 m1, __m64 m2);

 
extern __m64 __cdecl _mm_setzero_si64();
extern __m64 __cdecl _mm_set_pi32(int i1, int i0);
extern __m64 __cdecl _mm_set_pi16(short s3, short s2, short s1, short s0);
extern __m64 __cdecl _mm_set_pi8(char b7, char b6, char b5, char b4,
	  	                 char b3, char b2, char b1, char b0);
extern __m64 __cdecl _mm_set1_pi32(int i);
extern __m64 __cdecl _mm_set1_pi16(short s);
extern __m64 __cdecl _mm_set1_pi8(char b);
extern __m64 __cdecl _mm_setr_pi32(int i1, int i0);
extern __m64 __cdecl _mm_setr_pi16(short s3, short s2, short s1, short s0);
extern __m64 __cdecl _mm_setr_pi8(char b7, char b6, char b5, char b4,
  		                  char b3, char b2, char b1, char b0);






 
#line 189 "C:\\Program Files\\Intel\\Compiler\\11.1\\054\\include\\mmintrin.h"






#line 25 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"

#line 32 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"

#line 39 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"

#line 46 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"
   typedef union  __declspec(align(16)) { float f[4]; } __m128;







 
#line 1 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\malloc.h"












 


#pragma once





#line 1 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"










 

 

  
#line 23 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"






#line 38 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"












 


#line 61 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"







#line 1 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\sal.h"












 










































































































































































































 

#pragma once



#line 232 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\sal.h"








 





 

#line 486 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\sal.h"









 

#line 688 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\sal.h"









 

#line 711 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\sal.h"


    



#line 724 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\sal.h"






#line 69 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"



#line 79 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"



#line 95 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"





#line 1 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtassem.h"










 

#pragma once





#line 26 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtassem.h"





#line 101 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"



#line 111 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' "                    "name='" "Microsoft.VC80" ".CRT' "                      "version='" "8.0.50727.4053" "' "                                  "processorArchitecture='x86' "                                          "publicKeyToken='" "1fc8b3b9a1e18e3b" "'\"")








#line 137 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 155 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"









#pragma pack(push,8)


#line 1 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\vadefs.h"










 


#pragma once














 


#pragma pack(push,8)







#line 47 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\vadefs.h"





typedef  unsigned int   uintptr_t;








typedef char *  va_list;










#line 86 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\vadefs.h"

#line 100 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\vadefs.h"







#line 145 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\vadefs.h"






#pragma pack(pop)


#line 168 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"





 











#line 192 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"


 
#line 202 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 

#line 212 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 

#line 227 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 





 












#line 254 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 262 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 270 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 278 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 290 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 298 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 






 


 


 




 




 






 
 




#line 342 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 
 




#line 356 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 
 






#line 376 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 
 




#line 397 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 


 

 
 




#line 416 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"


 
#line 427 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 
#line 438 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"



   
#line 449 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 463 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 
 




#line 477 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"





typedef  unsigned int   size_t;






typedef size_t rsize_t;








typedef  int            intptr_t;




#line 511 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"





typedef  int            ptrdiff_t;





typedef unsigned short wchar_t;




typedef unsigned short wint_t;
typedef unsigned short wctype_t;



#line 540 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 551 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"



 

__declspec(deprecated("This name was supported during some Whidbey pre-releases. Instead, use the standard name errno_t.")) typedef int errcode;




typedef int errno_t;



typedef  long __time32_t;    





typedef __int64 __time64_t;      








typedef __time64_t time_t;       




#line 593 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 601 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 609 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 
















#line 638 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

 




#line 651 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"




 




 
#line 807 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"

#line 820 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"





































































#line 1889 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"


















































































































































struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct * pthreadlocinfo;
typedef struct threadmbcinfostruct * pthreadmbcinfo;
struct __lc_time_data;

typedef struct localeinfo_struct
{
    pthreadlocinfo locinfo;
    pthreadmbcinfo mbcinfo;
} _locale_tstruct, *_locale_t;


typedef struct tagLC_ID {
        unsigned short wLanguage;
        unsigned short wCountry;
        unsigned short wCodePage;
} LC_ID, *LPLC_ID;




typedef struct threadlocaleinfostruct {
        int refcount;
        unsigned int lc_codepage;
        unsigned int lc_collate_cp;
        unsigned long lc_handle[6];  
        LC_ID lc_id[6];
        struct {
            char *locale;
            wchar_t *wlocale;
            int *refcount;
            int *wrefcount;
        } lc_category[6];
        int lc_clike;
        int mb_cur_max;
        int * lconv_intl_refcount;
        int * lconv_num_refcount;
        int * lconv_mon_refcount;
        struct lconv * lconv;
        int * ctype1_refcount;
        unsigned short * ctype1;
        const unsigned short * pctype;
        const unsigned char * pclmap;
        const unsigned char * pcumap;
        struct __lc_time_data * lc_time_curr;
} threadlocinfo;

























#line 2113 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\crtdefs.h"






#pragma pack(pop)


#line 23 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\malloc.h"





 
#pragma pack(push,8)






 







 





 

#line 60 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\malloc.h"


typedef struct _heapinfo {
        int * _pentry;
        size_t _size;
        int _useflag;
        } _HEAPINFO;



 


extern __declspec(deprecated("This function or variable may be unsafe. Consider using " "_get_amblksiz" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) unsigned int _amblksiz;








 

#line 114 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\malloc.h"



__declspec(dllimport)   __declspec(noalias) __declspec(restrict)      void * __cdecl calloc( size_t _NumOfElements,  size_t _SizeOfElements);
__declspec(dllimport)                     __declspec(noalias)                                                                             void   __cdecl free( void * _Memory);
__declspec(dllimport)   __declspec(noalias) __declspec(restrict)                                void * __cdecl malloc( size_t _Size);
__declspec(dllimport)                     __declspec(noalias) __declspec(restrict)                             void * __cdecl realloc( void * _Memory,  size_t _NewSize);
__declspec(dllimport)                     __declspec(noalias) __declspec(restrict)                         void * __cdecl _recalloc( void * _Memory,  size_t _Count,  size_t _Size);
__declspec(dllimport)                     __declspec(noalias)                                                                             void   __cdecl _aligned_free( void * _Memory);
__declspec(dllimport)                     __declspec(noalias) __declspec(restrict)                                void * __cdecl _aligned_malloc( size_t _Size,  size_t _Alignment);
__declspec(dllimport)                     __declspec(noalias) __declspec(restrict)                                void * __cdecl _aligned_offset_malloc( size_t _Size,  size_t _Alignment,  size_t _Offset);
__declspec(dllimport)                     __declspec(noalias) __declspec(restrict)                                void * __cdecl _aligned_realloc( void * _Memory,  size_t _Size,  size_t _Alignment);
__declspec(dllimport)                     __declspec(noalias) __declspec(restrict)                         void * __cdecl _aligned_recalloc( void * _Memory,  size_t _Count,  size_t _Size,  size_t _Alignment);
__declspec(dllimport)                     __declspec(noalias) __declspec(restrict)                                void * __cdecl _aligned_offset_realloc( void * _Memory,  size_t _Size,  size_t _Alignment,  size_t _Offset);
__declspec(dllimport)                     __declspec(noalias) __declspec(restrict)                         void * __cdecl _aligned_offset_recalloc( void * _Memory,  size_t _Count,  size_t _Size,  size_t _Alignment,  size_t _Offset);
__declspec(dllimport)                                                                                                size_t __cdecl _aligned_msize( void * _Memory,  size_t _Alignment,  size_t _Offset);


#line 148 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\malloc.h"

__declspec(dllimport) int     __cdecl _resetstkoflw (void);



__declspec(dllimport) unsigned long __cdecl _set_malloc_crt_max_wait( unsigned long _NewValue);



#line 163 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\malloc.h"

__declspec(dllimport)   void *  __cdecl _expand( void * _Memory,  size_t _NewSize);
__declspec(dllimport)  size_t  __cdecl _msize( void * _Memory);






  void *          __cdecl _alloca( size_t _Size);
__declspec(dllimport)  size_t  __cdecl _get_sbh_threshold(void);
__declspec(dllimport) int     __cdecl _set_sbh_threshold( size_t _NewValue);
__declspec(dllimport) errno_t __cdecl _set_amblksiz( size_t _Value);
__declspec(dllimport) errno_t __cdecl _get_amblksiz( size_t * _Value);
__declspec(dllimport)  int     __cdecl _heapadd( void * _Memory,  size_t _Size);
__declspec(dllimport)  int     __cdecl _heapchk(void);
__declspec(dllimport)  int     __cdecl _heapmin(void);
__declspec(dllimport) int     __cdecl _heapset( unsigned int _Fill);
__declspec(dllimport)  int     __cdecl _heapwalk( _HEAPINFO * _EntryInfo);
__declspec(dllimport) size_t  __cdecl _heapused(size_t * _Used, size_t * _Commit);

__declspec(dllimport) intptr_t __cdecl _get_heap_handle(void);





#line 197 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\malloc.h"

typedef char __static_assert_t[ (sizeof(unsigned int) <= 8) ];


__inline void *_MarkAllocaS(  void *_Ptr, unsigned int _Marker)
{
    if (_Ptr)
    {
        *((unsigned int*)_Ptr) = _Marker;
        _Ptr = (char*)_Ptr + 8;
    }
    return _Ptr;
}


#line 227 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\malloc.h"





 


__declspec(noalias) __inline void __cdecl _freea( void * _Memory)
{
    unsigned int _Marker;
    if (_Memory)
    {
        _Memory = (char*)_Memory - 8;
        _Marker = *(unsigned int *)_Memory;
        if (_Marker == 0xDDDD)
        {
            free(_Memory);
        }
#line 252 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\malloc.h"
    }
}




 





#line 282 "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\include\\malloc.h"







#pragma pack(pop)


#line 57 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"













 
 
 
 
 
 
 
 
 
 




 
 
 
 
 
 
 
 
 
 
#line 107 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"


 





 
#line 123 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"

#line 131 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"































 
 
 



 

extern __m128 __cdecl _mm_add_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_add_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_sub_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_sub_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_mul_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_mul_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_div_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_div_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_sqrt_ss(__m128 a);
extern __m128 __cdecl _mm_sqrt_ps(__m128 a);
extern __m128 __cdecl _mm_rcp_ss(__m128 a);
extern __m128 __cdecl _mm_rcp_ps(__m128 a);
extern __m128 __cdecl _mm_rsqrt_ss(__m128 a);
extern __m128 __cdecl _mm_rsqrt_ps(__m128 a);
extern __m128 __cdecl _mm_min_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_min_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_max_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_max_ps(__m128 a, __m128 b);



 

extern __m128 __cdecl _mm_and_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_andnot_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_or_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_xor_ps(__m128 a, __m128 b);



 

extern __m128 __cdecl _mm_cmpeq_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpeq_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmplt_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmplt_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmple_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmple_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpgt_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpgt_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpge_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpge_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpneq_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpneq_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpnlt_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpnlt_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpnle_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpnle_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpngt_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpngt_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpnge_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpnge_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpord_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpord_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpunord_ss(__m128 a, __m128 b);
extern __m128 __cdecl _mm_cmpunord_ps(__m128 a, __m128 b);
extern int __cdecl _mm_comieq_ss(__m128 a, __m128 b);
extern int __cdecl _mm_comilt_ss(__m128 a, __m128 b);
extern int __cdecl _mm_comile_ss(__m128 a, __m128 b);
extern int __cdecl _mm_comigt_ss(__m128 a, __m128 b);
extern int __cdecl _mm_comige_ss(__m128 a, __m128 b);
extern int __cdecl _mm_comineq_ss(__m128 a, __m128 b);
extern int __cdecl _mm_ucomieq_ss(__m128 a, __m128 b);
extern int __cdecl _mm_ucomilt_ss(__m128 a, __m128 b);
extern int __cdecl _mm_ucomile_ss(__m128 a, __m128 b);
extern int __cdecl _mm_ucomigt_ss(__m128 a, __m128 b);
extern int __cdecl _mm_ucomige_ss(__m128 a, __m128 b);
extern int __cdecl _mm_ucomineq_ss(__m128 a, __m128 b);



 

extern int    __cdecl _mm_cvt_ss2si(__m128 a);
extern __m64  __cdecl _mm_cvt_ps2pi(__m128 a);
extern int    __cdecl _mm_cvtt_ss2si(__m128 a);
extern __m64  __cdecl _mm_cvtt_ps2pi(__m128 a);
extern __m128 __cdecl _mm_cvt_si2ss(__m128, int);
extern __m128 __cdecl _mm_cvt_pi2ps(__m128, __m64);



 

extern __m128 __cdecl _mm_shuffle_ps(__m128 a, __m128 b, unsigned int imm8);
extern __m128 __cdecl _mm_unpackhi_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_unpacklo_ps(__m128 a, __m128 b);
extern __m128 __cdecl _mm_loadh_pi(__m128, __m64 const*);
extern __m128 __cdecl _mm_movehl_ps(__m128, __m128);
extern __m128 __cdecl _mm_movelh_ps(__m128, __m128);
extern void   __cdecl _mm_storeh_pi(__m64 *, __m128);
extern __m128 __cdecl _mm_loadl_pi(__m128, __m64 const*);
extern void   __cdecl _mm_storel_pi(__m64 *, __m128);
extern int    __cdecl _mm_movemask_ps(__m128 a);




 
extern int   __cdecl _m_pextrw(__m64, int);
extern __m64 __cdecl _m_pinsrw(__m64, int, int);
extern __m64 __cdecl _m_pmaxsw(__m64, __m64);
extern __m64 __cdecl _m_pmaxub(__m64, __m64);
extern __m64 __cdecl _m_pminsw(__m64, __m64);
extern __m64 __cdecl _m_pminub(__m64, __m64);
extern int   __cdecl _m_pmovmskb(__m64);
extern __m64 __cdecl _m_pmulhuw(__m64, __m64);
extern __m64 __cdecl _m_pshufw(__m64, int);
extern void  __cdecl _m_maskmovq(__m64, __m64, char *);
extern __m64 __cdecl _m_pavgb(__m64, __m64);
extern __m64 __cdecl _m_pavgw(__m64, __m64);
extern __m64 __cdecl _m_psadbw(__m64, __m64);




 

extern __m128 __cdecl _mm_set_ss(float a);
extern __m128 __cdecl _mm_set_ps1(float a);
extern __m128 __cdecl _mm_set_ps(float a, float b, float c, float d);
extern __m128 __cdecl _mm_setr_ps(float a, float b, float c, float d);
extern __m128 __cdecl _mm_setzero_ps(void);
extern __m128 __cdecl _mm_load_ss(float const*a);
extern __m128 __cdecl _mm_load_ps1(float const*a);
extern __m128 __cdecl _mm_load_ps(float const*a);
extern __m128 __cdecl _mm_loadr_ps(float const*a);
extern __m128 __cdecl _mm_loadu_ps(float const*a);
extern void __cdecl _mm_store_ss(float *v, __m128 a);
extern void __cdecl _mm_store_ps1(float *v, __m128 a);
extern void __cdecl _mm_store_ps(float *v, __m128 a);
extern void __cdecl _mm_storer_ps(float *v, __m128 a);
extern void __cdecl _mm_storeu_ps(float *v, __m128 a);
extern void __cdecl _mm_prefetch(char const*a, int sel);
extern void __cdecl _mm_stream_pi(__m64 *, __m64);
extern void __cdecl _mm_stream_ps(float *, __m128);
extern __m128 __cdecl _mm_move_ss(__m128 a, __m128 b);
extern float  __cdecl _mm_cvtss_f32(__m128 a);

extern void __cdecl _mm_sfence(void);
extern unsigned int __cdecl _mm_getcsr(void);
extern void __cdecl _mm_setcsr(unsigned int);



 






 
#line 345 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"


 
 
 

 
 
 
 
 
 
 
 
#line 368 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"


 
 
 
 
 
 
 
 
#line 385 "C:\\PROGRA~1\\Intel\\Compiler\\11.1\\054\\include\\ia32\\xmmintrin.h"

 
 
 
 
 
 
 
 





 
 
 
 
 
 
 
 





 
 
 
 
 
 
 
 
 




 
 
 
 
 
 
 
 




 
 
 
 
 
 
 
 
 











 




#line 3 ".\\../libC/libc_math.h"
















typedef float float_func (float a);
typedef double double_func (double a);

typedef float_func  *float_func_ptr;
typedef double_func *double_func_ptr;







extern    float libc_atanf(float a);
extern	  float libc_sinf(float a);
extern	  float libc_cosf(float a);

extern	 double libc_atand(double a);


extern	 double libc_sind(double a);
extern	 double libc_cosd(double a);
extern	 double  libc_sqrtd(double a);

static __inline		float  libc_sqrtf(float a)
{
	__m128 sqrt;
	float o;
	sqrt = _mm_sqrt_ss(_mm_load_ss(&a));
	_mm_store_ss(&o,sqrt);
	return o;

}


static __inline float libc_fabsf(float a)
{	
	unsigned int t;
	t=*(((unsigned int *)(&a)))&0x7FFFFFFF;
	return *((float *)(&t));
}

static __inline float libc_asinf  ( float A ){
  
  if (A>1.0f||A<-1.0f)return 0.0f;
  if (A==1.0f)   return 3.1415926535897932384626433832795f/2.0f;
  if (A==-1.0f)return 3.1415926535897932384626433832795f/-2.0f;
  return libc_atanf(A/libc_sqrtf(1.0f-A*A));
}


static __inline float libc_acosf ( float A ){
  if (A>1.0f||A<-1.0f) return 0.0f;
  if (A==0.0f)		   return 3.1415926535897932384626433832795f/2.0f;
	A=libc_atanf(libc_sqrtf(1.0f-A*A)/A);
  if (A<0.0f)A+=3.1415926535897932384626433832795f;
  return A;
}


static __inline double libc_acosd ( double A ){
  if (A>1.0||A<-1.0) return 0.0;
  if (A==0.0)		 return 3.1415926535897932384626433832795/2.0;
  
  A=libc_atand(libc_sqrtd(1.0-A*A)/A);
  if (A<0.0)A+=3.1415926535897932384626433832795;
  return A;
}

static __inline double libc_asind  ( double A ){
  
  if (A>1.0||A<-1.0)return 0.0;
  if (A==1.0)   return 3.1415926535897932384626433832795/2.0;
  if (A==-1.0)return 3.1415926535897932384626433832795/-2.0;
  return libc_atand(A/libc_sqrtd(1.0f-A*A));
}



#line 2 ".\\ray_float.c"
#line 1 ".\\vec_types.h"










typedef float  mat3x3f_t[16];
typedef double mat3x3d_t[16];

typedef int			vec3i_t[4];
typedef int			*p_vec3i_t;
typedef const int	*cp_vec3i_t;

typedef float		vec3f_t[4];
typedef float		*p_vec3f_t;
typedef const float	*cp_vec3f_t;

typedef double		vec3d_t[4];

typedef int			 vec4i_t[4];
typedef int			*p_vec4i_t;



typedef struct {float x, y, z, w;} Quat;  
enum QuatPart {X, Y, Z, W};
typedef float HMatrix[4][4];  
typedef Quat EulerAngles;     

#line 49 ".\\vec_types.h"
     


     

     
#line 67 ".\\vec_types.h"
     
#line 80 ".\\vec_types.h"







typedef void 	copy_mat3x3_func			(mat3x3f_t m,const mat3x3f_t src);
typedef void 	copy_mat3x3d_func			(mat3x3d_t m,const mat3x3d_t src);

typedef void 	mul_mat3x3_func				(mat3x3f_t m,const mat3x3f_t op);
typedef void 	mul_mat3x3d_func			(mat3x3d_t m,const mat3x3d_t op);

typedef void 	mul_mat3x3_o_func			(const mat3x3f_t m,const mat3x3f_t op,mat3x3f_t out);
typedef void 	mul_mat3x3d_o_func			(const mat3x3d_t m,const mat3x3d_t op,mat3x3d_t out);
typedef void 	mul_mat3x3f2d_func			(mat3x3f_t m,const mat3x3d_t op);

typedef void	identity_mat3x3_func		(mat3x3f_t m);
typedef float	determinant_mat3x3_func		(const mat3x3f_t m);
typedef int		inverse_mat3x3_func			(const mat3x3f_t m,mat3x3f_t out);

typedef void 	mul_mat3x3_rev_func			(mat3x3f_t m,const mat3x3f_t op);
typedef void 	mul_mat3x3_rev_o_func		(const mat3x3f_t m,const mat3x3f_t op,mat3x3f_t out);
typedef void 	mul_mat3x3_revf2d_func		(mat3x3f_t m,const mat3x3d_t op);

typedef void 	mul_mat3x3_revd_func		(mat3x3d_t m,const mat3x3d_t op);
typedef void 	mul_mat3x3_revd_o_func		(const mat3x3d_t m,const mat3x3d_t op,mat3x3d_t out);

typedef void	identity_mat3x3d_func		(mat3x3d_t m);
typedef double	determinant_mat3x3d_func	(const mat3x3d_t m);
typedef int		inverse_mat3x3d_func		(const mat3x3d_t m,mat3x3d_t out);

typedef 		copy_mat3x3d_func			*copy_mat3x3d_func_ptr;
typedef 		copy_mat3x3_func			*copy_mat3x3_func_ptr;

typedef 		mul_mat3x3_func				*mul_mat3x3_func_ptr;
typedef 		mul_mat3x3d_func			*mul_mat3x3d_func_ptr;	
									 	
typedef 		mul_mat3x3_o_func			*mul_mat3x3_o_func_ptr;		
typedef 		mul_mat3x3d_o_func			*mul_mat3x3d_o_func_ptr;	
typedef 		mul_mat3x3f2d_func			*mul_mat3x3f2d_func_ptr;

typedef 		identity_mat3x3_func		*identity_mat3x3_func_ptr;
typedef 		determinant_mat3x3_func		*determinant_mat3x3_func_ptr;
typedef 		inverse_mat3x3_func			*inverse_mat3x3_func_ptr;
									 
typedef 		mul_mat3x3_rev_func			*mul_mat3x3_rev_func_ptr;
typedef 		mul_mat3x3_rev_o_func		*mul_mat3x3_rev_o_func_ptr;
typedef 		mul_mat3x3_revf2d_func		*mul_mat3x3_revf2d_func_ptr;
									 
typedef 		mul_mat3x3_revd_func		*mul_mat3x3_revd_func_ptr;
typedef 		mul_mat3x3_revd_o_func		*mul_mat3x3_revd_o_func_ptr;

typedef 		identity_mat3x3d_func		*identity_mat3x3d_func_ptr;
typedef 		determinant_mat3x3d_func	*determinant_mat3x3d_func_ptr;
typedef 		inverse_mat3x3d_func		*inverse_mat3x3d_func_ptr;




void *aligned_alloc(unsigned int size);
void  aligned_free(void	 *a_ptr);

EulerAngles Eul_(float ai, float aj, float ah, int order);

Quat		Eul_ToQuat					(EulerAngles ea);
void		Eul_ToHMatrix				(EulerAngles ea, HMatrix M);
EulerAngles Eul_FromHMatrix				(HMatrix M, int order);
EulerAngles Eul_FromQuat				(Quat q, int order);
double  GetAngleAxis					(double *hpb,double *v,unsigned int mod);

void    set_vec4i						(int *vec,int a,int b,int c,int d);

void   set_vec3							(vec3f_t vec,float a,float b,float c);
void   copy_vec3						(vec3f_t v1,const vec3f_t v2);
void   sub_vec3							(const vec3f_t v1,const vec3f_t v2,vec3f_t out);
void   add_vec3							(const vec3f_t v1,const vec3f_t v2,vec3f_t out);
void   mul_vec3							(vec3f_t v1,const vec3f_t v2);

void   mul_vec4							( vec3f_t v1,const vec3f_t v2);
void   sub_vec4							(const vec3f_t v1,const vec3f_t v2,vec3f_t out);




float  dot_vec3							(const vec3f_t vec1,const vec3f_t vec2);
float  length_vec3						(vec3f_t vec);
double length_vec3d						(vec3d_t vec);
void   scale_vec3						(vec3f_t vec,float d);
void   scale_vec3_o						(const vec3f_t vec,float d,vec3f_t out);
void   normalize_vec3					(vec3f_t vec);
float  dist_vec3						(vec3f_t vec1,vec3f_t vec2);
float  sq_dist_vec3						(vec3f_t vec1,vec3f_t vec2);


void   rotate_X_3f						(vec3f_t v,float aD);
void   rotate_Y_3f						(vec3f_t v,float aD);
void   rotate_Z_3f						(vec3f_t v,float aD);
void   project_vec3						(vec3f_t A, vec3f_t B, vec3f_t out);
void   cross_vec3						(const vec3f_t A,const vec3f_t B,vec3f_t Out);
void   reflect_vec3						(const vec3f_t v,const vec3f_t n,vec3f_t r);
int	   refract_vec3						(const vec3f_t normal, const vec3f_t incident,double n1, double n2,vec3f_t out);

void copy_mat3x3						(mat3x3f_t m,const mat3x3f_t op);
void copy_mat3x3d						(mat3x3d_t m,const mat3x3d_t op);
void transpose_mat3x3					(const mat3x3f_t m, mat3x3f_t out);
void toOpenGL3x3						(float *inMat,float *outMat);
									 
extern identity_mat3x3_func_ptr			identity_mat3x3;
extern determinant_mat3x3_func_ptr		determinant_mat3x3;
extern inverse_mat3x3_func_ptr			inverse_mat3x3;

extern identity_mat3x3d_func_ptr		identity_mat3x3d;
extern determinant_mat3x3d_func_ptr		determinant_mat3x3d;
extern inverse_mat3x3d_func_ptr			inverse_mat3x3d;

extern mul_mat3x3_func_ptr				mul_mat3x3;
extern mul_mat3x3d_func_ptr				mul_mat3x3d;
									
extern mul_mat3x3_o_func_ptr			mul_mat3x3_o;
extern mul_mat3x3d_o_func_ptr			mul_mat3x3d_o;
extern mul_mat3x3f2d_func_ptr			mul_mat3x3f2d;

extern mul_mat3x3_rev_func_ptr			mul_mat3x3_rev;
extern mul_mat3x3_rev_o_func_ptr		mul_mat3x3_rev_o;
extern mul_mat3x3_revf2d_func_ptr		mul_mat3x3_revf2d;
									
extern mul_mat3x3_revd_func_ptr			mul_mat3x3_revd;
extern mul_mat3x3_revd_o_func_ptr		mul_mat3x3_revd_o;

void init_mat3_fncs_c					();
void __declspec(dllexport) init_mat3_fncs			();

void mul_vec3x3							(vec3f_t v,const mat3x3f_t m);
void mul_vec3x3_o						(const vec3f_t v,const mat3x3f_t m,vec3f_t out);

static __inline void sub_vec3			(const vec3f_t v1,const vec3f_t v2,vec3f_t out)
{
	




 

	out[0]=v1[0]-v2[0];
	out[1]=v1[1]-v2[1];
	out[2]=v1[2]-v2[2];

}



static __inline void mul_vec3x3(vec3f_t v,const mat3x3f_t m)
{

	__m128 v_row,vec;
	__m128 mul,m_row;

	vec		= _mm_load_ps(v);
		
	v_row	= _mm_shuffle_ps(vec,vec,0x00);
	m_row	= _mm_load_ps(&m[0]);
	mul		= _mm_mul_ps(v_row,m_row);

	v_row	= _mm_shuffle_ps(vec,vec,0x55);
	m_row	= _mm_load_ps(&m[4]);
	mul		= _mm_add_ps(mul,_mm_mul_ps (v_row,m_row));

	v_row	= _mm_shuffle_ps(vec,vec,0xAA);
	m_row	= _mm_load_ps(&m[8]);
	mul		= _mm_add_ps(mul,_mm_mul_ps (v_row,m_row));

	_mm_store_ps(v,mul);










 
}


static __inline void mul_vec3x3_o(const vec3f_t v,const mat3x3f_t m,vec3f_t out)
{
	__m128 v_row,vec;
	__m128 mul,m_row;

	vec		= _mm_load_ps(v);
		
	v_row	= _mm_shuffle_ps(vec,vec,0x00);
	m_row	=_mm_load_ps(&m[0]);
	mul		= _mm_mul_ps(v_row,m_row);

	v_row	= _mm_shuffle_ps(vec,vec,0x55);
	m_row	= _mm_load_ps(&m[4]);
	mul		= _mm_add_ps(mul,_mm_mul_ps (v_row,m_row));

	v_row	= _mm_shuffle_ps(vec,vec,0xAA);
	m_row	= _mm_load_ps(&m[8]);
	mul		= _mm_add_ps(mul,_mm_mul_ps (v_row,m_row));

	_mm_store_ps(out,mul);
	









 
}





#line 3 ".\\ray_float.c"
#line 1 ".\\ray_float.h"
typedef struct
{
 
    vec3f_t		origin;
	float		f_origin[8];
	float		f_inv_direction[8];
    vec3f_t		direction;
    vec3f_t		inv_direction;
    int			sign[3];
	int			o_sign01;
}rayf_t;

typedef struct
{
	unsigned   int	id;
	float			*_parameters;
	unsigned   int	idx;
	int				idxx;
}box_testf_t;


typedef struct
{
	unsigned char	*pixels;
	unsigned int	w;
	unsigned int	h;
	unsigned int	size;

}image_t;

typedef struct
{
	unsigned int	tid;
	unsigned int	nid;
	const image_t	*texture;
	const image_t	*normals;
	float			refract;
	float			reflect;
	double			refract_density;
}materialf_t;

typedef struct
{
	float norm[3];
	float det;
}planef_t;

typedef struct
{
	unsigned int			cast_id;
	vec3f_t					*parameters;
	float					*_parameters;
}bounding_boxf_t;

typedef struct
{
	materialf_t				material;
	float					center[3];
	float					size[3];
	vec3f_t					*Point;
	float					vecs[6][4][3];
	planef_t				planes[6];
	
	float					*inv_matrix;
	float					*inv_norm_mat;
	float					*matrix;
	float					*norm_mat;
	
	bounding_boxf_t			CUBE;
	bounding_boxf_t			AABOX;
}cubef_t;

typedef struct
{
	materialf_t					material;
	float						center[3];
	float						radius;
	float						sq_radius;
	bounding_boxf_t				AABOX;
}spheref_t;

typedef struct
{
	materialf_t		material;
	vec3f_t			base;
	float			height;
	
	float			*inv_matrix;
	float			*inv_norm_mat;
	float			*matrix;
	float			*norm_mat;

	float			radiussq;
	float			radius;
	float			lengthsq;
	bounding_boxf_t	AABOX;
}cylf_t;

typedef int		intersect_ray_boxf_func			( const vec3f_t r_origin, const vec3f_t r_inv_dir, float *parameters, float *t);
typedef			intersect_ray_boxf_func			*intersect_ray_boxf_func_ptr;

typedef	int		intersect_ray_boxesf_func		(const vec3f_t ray_o,const vec3f_t ray_dir,unsigned int ray_sign,const box_testf_t *boxes,unsigned int n_boxes,unsigned int cast,unsigned int *bound_ids,float *min_max);
typedef			intersect_ray_boxesf_func		*intersect_ray_boxesf_func_ptr;




int		pointInCubef			(vec3f_t vec,cubef_t *cube,vec3f_t ormVec,float *uv_coord);
int		pointInSpheref			(float *vec,spheref_t *sphere,float *normVec);
float	point_in_cyl_test_c		(const float *pt1, const float *pt2, float lengthsq, float radius_sq, const float *testpt,float *normVec,float *uv_coord  );

void	getCubeUvCoords			(cubef_t *cube,int col,float *vec,float *uv_coord);
void	computeCubePlanesf		(cubef_t *c,float aX,float aY,float aZ,float *minv,float *maxv);


extern intersect_ray_boxf_func_ptr		intersect_ray_boxf;
extern intersect_ray_boxesf_func_ptr	intersect_ray_boxesf;


int		 rayIntersectCube		(const cubef_t *cube,const rayf_t *ray,float *out_dist,vec3f_t normVec,unsigned int *planeId,float *uv_coord);
int		 intersectRayCyl		(const rayf_t *ray,const cylf_t *cyl,float *tVals,vec3f_t cylNorm,float *uv_coord);
int		 Sphere_intersect		(const spheref_t *sphere,const vec3f_t raypos ,const vec3f_t direction,float* t,float *t2);

void	 init_rayf				(rayf_t *ray,vec3f_t o,vec3f_t d);
void	 alloc_bboxf			(bounding_boxf_t *box);
void	 init_bboxf				(bounding_boxf_t *box,unsigned int id,vec3f_t min,vec3f_t max);
void	 init_cylf				(cylf_t	*cyl,vec3f_t base,float height,float radius,unsigned int id);

void	 remove_vec3			(unsigned int num);
float	*new_vec3				();


void	__declspec(dllexport) init_vec_funcsf	();
void	__declspec(dllexport) get_cube_norm			(unsigned int which,vec3f_t out);








#line 4 ".\\ray_float.c"


 vec3f_t				cube_norm[6];


intersect_ray_boxf_func_ptr		intersect_ray_boxf;
intersect_ray_boxesf_func_ptr	intersect_ray_boxesf;

extern int  intersect_ray_boxesf_sse(const vec3f_t ray_o,const vec3f_t ray_dir,unsigned int ray_sign,const box_testf_t *boxes,unsigned int n_boxes,unsigned int cast,unsigned int *bound_ids,float *min_max);
extern int  intersect_ray_boxf_sse	(const vec3f_t r_origin,const vec3f_t r_inv_dir,float *parameters_1,float *out);

void	get_cube_norm					(unsigned int which,vec3f_t out)
{
	copy_vec3(out,cube_norm[which]);
}

int intersect_ray_boxf_c	(const vec3f_t r_origin,const vec3f_t r_inv_dir,float *parameters,float *out)
{
  vec3f_t	v_xy;
  float		v_z[2];

  sub_vec4	(parameters,r_origin,v_xy);
  mul_vec4	(v_xy,r_inv_dir);
    
  if ( (v_xy[0] > v_xy[3]) || (v_xy[1]>v_xy[2] ) ) 
    return 0;
  
  v_xy[0] = (((v_xy[0]) > (v_xy[1])) ? (v_xy[0]) : (v_xy[1]));
  v_xy[1] = (((v_xy[2]) < (v_xy[3])) ? (v_xy[2]) : (v_xy[3]));
  
  v_z[0]  = (parameters[4]-r_origin[4])*r_inv_dir[4];
  v_z[1]  = (parameters[5]-r_origin[5])*r_inv_dir[5];

  if ( (v_xy[0]  > v_z[1]) || ( v_z[0] > v_xy[1]) ) 
    return 0;

  out[0]= (((v_xy[0]) > (v_z[0])) ? (v_xy[0]) : (v_z[0]));
  out[1]= (((v_xy[1]) < (v_z[1])) ? (v_xy[1]) : (v_z[1]));
  return 1;

}
int intersect_ray_boxesf_c(const vec3f_t ray_o,const vec3f_t ray_dir,unsigned int ray_sign,const box_testf_t *boxes,unsigned int n_boxes,unsigned int cast,unsigned int *bound_ids,float *min_max)
{
	unsigned int	intersect;
	unsigned int	n;
	float			c_minmax[4];

	intersect	=	0;
	min_max[0]		=	100000;
	min_max[1]		=	-100000;

	n			=	0;
	while(n<n_boxes)
	{
		if(cast!=boxes[n].id)
		{
			if(intersect_ray_boxf_c(ray_o,ray_dir,&boxes[n]._parameters[ray_sign],c_minmax))
			{
				if(c_minmax[0]<min_max[0])
					min_max[0]=c_minmax[0];

				if(c_minmax[1]>min_max[1])
					min_max[1]=c_minmax[1];
				intersect=1;
		
				(*bound_ids++)=boxes[n].id;
			}
		}
		n++;
	}
	(*bound_ids)=0;

	return intersect;

}


float			*mem_pool;
float			*mem_pool_ptr;

void init_mem_pool(unsigned int num)
{
	mem_pool		=	(float *)aligned_alloc(num*sizeof(vec3f_t));
	mem_pool_ptr	=	mem_pool;
}

float *new_vec3()
{
	float *ret_ptr;
	ret_ptr			=	mem_pool_ptr;
	mem_pool_ptr	+=  4;
	return ret_ptr;
}

void remove_vec3(unsigned int num)
{
	mem_pool_ptr	-=  (4*num);
}


void init_vec_funcsf()
{
	intersect_ray_boxf		=	intersect_ray_boxf_sse;
	intersect_ray_boxesf	=	intersect_ray_boxesf_sse;
	
	init_mem_pool			(1024);


	set_vec3(cube_norm[0],0,0,1);
	set_vec3(cube_norm[1],1,0,0);
	set_vec3(cube_norm[2],0,0,-1);
	set_vec3(cube_norm[3],-1,0,0);
	set_vec3(cube_norm[4],0,1,0);
	set_vec3(cube_norm[5],0,-1,0);
}




void init_rayf(rayf_t *ray,vec3f_t o,vec3f_t d)
{

	  copy_vec3					(ray->origin,o);

	  ray->direction[0]				=	d[0];
	  ray->direction[1]				=	d[1];
	  ray->direction[2]				=	d[2];

      ray->inv_direction[0]			=	1.0f/ray->direction[0];
	  ray->inv_direction[1]			=	1.0f/ray->direction[1];
	  ray->inv_direction[2]			=	1.0f/ray->direction[2];

	  ray->sign[0]					= 	(ray->inv_direction[0] < 0);
      ray->sign[1]					= 	(ray->inv_direction[1] < 0);
      ray->sign[2]					= 	(ray->inv_direction[2] < 0);

	  ray->f_origin[0]				=	ray->origin[0];
	  ray->f_origin[1]				=	ray->origin[1];
	  ray->f_origin[2]				=	ray->origin[0];
	  ray->f_origin[3]				=	ray->origin[1];
	  
	  ray->f_origin[4]				=	ray->origin[2];
	  ray->f_origin[5]				=	ray->origin[2];
	  ray->f_origin[6]				=	ray->origin[2];
	  ray->f_origin[7]				=	ray->origin[2];


	  ray->f_inv_direction[0]		=	ray->inv_direction[0];
	  ray->f_inv_direction[1]		=	ray->inv_direction[1];
	  ray->f_inv_direction[2]		=	ray->inv_direction[0];
	  ray->f_inv_direction[3]		=	ray->inv_direction[1];

	  ray->f_inv_direction[4]		=	ray->inv_direction[2];
	  ray->f_inv_direction[5]		=	ray->inv_direction[2];
	  ray->f_inv_direction[6]		=	ray->inv_direction[2];
	  ray->f_inv_direction[7]		=	ray->inv_direction[2];

	  ray->o_sign01					=	(ray->sign[0]|(ray->sign[1]<<1)|(ray->sign[2]<<2))*8;

}




float point_in_cyl_test_c( const float *pt1, const float *pt2, float lengthsq, float radius_sq, const float *testpt,float *normVec,float *uv_coord  )
{
	float dx, dy, dz;	
	float pdx, pdy, pdz;	
	float dot, dsq;

	dx = pt2[0] - pt1[0];	
	dy = pt2[1] - pt1[1];     
	dz = pt2[2] - pt1[2];

	pdx = testpt[0] - pt1[0];		
	pdy = testpt[1] - pt1[1];
	pdz = testpt[2] - pt1[2];

	
	

	dot = pdx * dx + pdy * dy + pdz * dz;

	
	
	

	if( dot < 0.0f || dot > lengthsq )
	{
		return( -1.0f );
	}
	else 
	{
		
		
		
		
		
		
		
		
		
		

		

		dsq = (pdx*pdx + pdy*pdy + pdz*pdz) - dot*dot/lengthsq;

		if( dsq > radius_sq )
		{
			return( -1.0f );
		}
		else
		{
			vec3f_t X,A,C,V,P;

			C[0]=(pt2[0]+pt1[0])/2.0f;
			C[1]=(pt2[1]+pt1[1])/2.0f;
			C[2]=(pt2[2]+pt1[2])/2.0f;

			copy_vec3	(X,testpt);
			sub_vec3	(pt2,pt1,A);
			normalize_vec3	(A);

			
			sub_vec3		(X,C,V);
			project_vec3	(V,A,P);

			sub_vec3	(V,P,normVec); 
			normalize_vec3(normVec);
			

			return( dsq );		
		}
	}
}



int pointInCubef(float *vec,cubef_t *cube,float *normVec,float *uv_coord)
{
	int		n;
	int		col;
	float	min_dist,dist;
	
	

	min_dist	=	1000000000;
	n			=	0;
	
	while(n<6)
	{
		dist=cube->planes[n].norm[0]*vec[0]+cube->planes[n].norm[1]*vec[1]+cube->planes[n].norm[2]*vec[2]+cube->planes[n].det;
		if(dist>0)
			return 0;

		dist=libc_fabsf(dist);

		if(dist<min_dist)
		{
			min_dist=dist;
			col     =n;
		}
		n++;
	}
	normVec[0]		=	cube->planes[col].norm[0];
	normVec[1]		=	cube->planes[col].norm[1];
	normVec[2]		=	cube->planes[col].norm[2];

	if(uv_coord!=0x00000000)
	{
		getCubeUvCoords(cube,col,vec,uv_coord);
		
	}
	return 1;
}


int pointInSpheref(float *vec,spheref_t *sphere,float *normVec)
{
	float	l,sql;
	float	 distVec[3];


	sub_vec3(vec,sphere->center,distVec);
	l=dot_vec3(distVec,distVec);

	if(l>sphere->sq_radius)
		return 0;

	sql			=1.0f/libc_sqrtf(l);
	normVec[0]	=distVec[0]*sql;
	normVec[1]	=distVec[1]*sql;
	normVec[2]	=distVec[2]*sql;

	return 1;
}


int rayIntersectsTriangle(const float *p, const float *d,const float *v0, const float *v1, const float *v2,float *t) 
{

	float e1[3],e2[3],h[3],s[3],q[3];
	float a,f,u,v;
	
	sub_vec3(v1,v0,e1);
	sub_vec3(v2,v0,e2);

	cross_vec3(d,e2,h);
	a = dot_vec3(e1,h);

	if (a > -0.00001 && a < 0.00001)
		return(0);

	f = 1/a;
	sub_vec3(p,v0,s);
	u = f * (dot_vec3(s,h));

	if (u < 0.0 || u > 1.0)
		return(0);

	cross_vec3(s,e1,q);
	v = f * dot_vec3(d,q);

	if (v < 0.0 || u + v > 1.0)
		return(0);

	
	
	*t = f * dot_vec3(e2,q);

	if (*t > 0.00001) 
		return(1);

	else 
		 
		 return (0);
}


int intersect_ray_box_soft (float *ray_origin,float *ray_inv_direction,float *min,float *max,float *t)
{
  float tmin, tmax, tymin, tymax, tzmin, tzmax;
    
  if(ray_inv_direction[0]>0)
  {
	tmin = ((min[0] - ray_origin[0]) * ray_inv_direction[0]);
	tmax = ((max[0] - ray_origin[0]) * ray_inv_direction[0]);
  }
  else
  {
	tmin = ((max[0]  - ray_origin[0]) * ray_inv_direction[0]);
	tmax = ((min[0]  - ray_origin[0]) * ray_inv_direction[0]);
  }

  if(ray_inv_direction[1]>0)
  {
	tymin = ((min[1] - ray_origin[1])   * ray_inv_direction[1]);
	tymax  =  ((max[1] - ray_origin[1]) * ray_inv_direction[1]);
  }
  else
  {
	tymin = ((max[1]  - ray_origin[1]) * ray_inv_direction[1]);
	tymax = ((min[1]  - ray_origin[1]) * ray_inv_direction[1]);
  }
  
 
  if ( (tmin > tymax) || (tymin > tmax) ) 
    return 0;

  if (tymin > tmin)
    tmin = tymin;

  if (tymax < tmax)
    tmax = tymax;

  if(ray_inv_direction[2]>0)
  {
	tzmin = ((min[2] - ray_origin[2]) * ray_inv_direction[2]);
	tzmax = ((max[2] - ray_origin[2]) * ray_inv_direction[2]);
  }
  else
  {
	tzmin = ((max[2]  - ray_origin[2]) * ray_inv_direction[2]);
	tzmax = ((min[2]  - ray_origin[2]) * ray_inv_direction[2]);
  }  

  if ( (tmin > tzmax) || (tzmin > tmax) ) 
    return 0;
  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;

  t[0]=tmin;
  t[1]=tmax;
  
  return 1;
}



int rayIntersectCube(const cubef_t *cube,const rayf_t *ray,float *out_dist,float *normVec,unsigned int *plane_id,float *uv_coord)
{
	int			n,col,ret;
	float		minmax[2];
	float		dist;
	float _raydir[8];float *raydir=(float *)((((unsigned int )(_raydir))&0xFFFFFFF0)+16);;
	float _rayori[8];float *rayori=(float *)((((unsigned int )(_rayori))&0xFFFFFFF0)+16);;
	float _rayidir[8];float *rayidir=(float *)((((unsigned int )(_rayidir))&0xFFFFFFF0)+16);;
	float _raypos[8];float *raypos=(float *)((((unsigned int )(_raypos))&0xFFFFFFF0)+16);;
	float _vec[8];float *vec=(float *)((((unsigned int )(_vec))&0xFFFFFFF0)+16);;
	





 

	(*out_dist)=10000000.0;
	ret=0;
	mul_vec3x3_o		(ray->direction	,cube->inv_norm_mat	,raydir);

	sub_vec3			(ray->origin	,cube->center		,rayori);
	mul_vec3x3_o		(rayori			,cube->inv_matrix	,raypos);

	
	rayidir[0]=	1.0f/raydir[0];
	rayidir[1]=	1.0f/raydir[1];
	rayidir[2]=	1.0f/raydir[2];

	if(intersect_ray_box_soft	( raypos,rayidir,cube->CUBE.parameters[0],cube->CUBE.parameters[1],minmax))
	{
		float distPlane;
		

		if(minmax[0]>0)
		{
			dist=minmax[0];

			vec[0]=raypos[0]+raydir[0]*dist;
			vec[1]=raypos[1]+raydir[1]*dist;
			vec[2]=raypos[2]+raydir[2]*dist;
			n	=0;
			while((ret==0)&&(n<6))
			{
				distPlane		=	dot_vec3(cube_norm[n],vec)+cube->planes[n].det;
				if(libc_fabsf(distPlane)<1.0)
				{
					col     =n;
					ret		=1;
					*out_dist=dist;
				}
				n++;
			}
		}
	}
	
	if(ret==1)
	{
		*plane_id		=	col;
		
		copy_vec3	(normVec,cube_norm[col]);

		if(uv_coord!=0x00000000)
		{
			switch(col)
			{
					
				case 0:
					uv_coord[0]		=	(vec[0]+(cube->size[0]))/(cube->size[0]*2.0f);
					uv_coord[1]		=	(vec[1]+(cube->size[1]))/(cube->size[1]*2.0f);
					
				break;
				case 1:
					
					uv_coord[0]		=	(vec[1]+(cube->size[1]))/(cube->size[1]*2.0f);
					uv_coord[1]		=	(vec[2]+(cube->size[2]))/(cube->size[2]*2.0f);
				break;		
				case 2:
					
					uv_coord[0]		=	(vec[0]+(cube->size[0]))/(cube->size[0]*2.0f);
					uv_coord[1]		=	(vec[1]+(cube->size[1]))/(cube->size[1]*2.0f);
				break;
				case 3:
					
					uv_coord[0]		=	(vec[1]+(cube->size[1]))/(cube->size[1]*2.0f);
					uv_coord[1]		=	(vec[2]+(cube->size[2]))/(cube->size[2]*2.0f);
				break;
				case 4:
					
					uv_coord[0]		=	(vec[0]+(cube->size[0]))/(cube->size[0]*2.0f);
					uv_coord[1]		=	(vec[2]+(cube->size[2]))/(cube->size[2]*2.0f);
				break;
				case 5:
					
					uv_coord[0]		=	(vec[0]+(cube->size[0]))/(cube->size[0]*2.0f);
					uv_coord[1]		=	(vec[2]+(cube->size[2]))/(cube->size[2]*2.0f);
				break;
			}
		}
	}

	return ret;

		
}




#line 834 ".\\ray_float.c"


 int intcyl(const vec3f_t raybase, const vec3f_t raydir, float radius, float *tVals)
 {
     int	 hit; 
     float   d; 
     float	 t, s; 
     vec3f_t n, O;
     float	 ln;

	 
	 

	 n[0] = -raydir[2];
	 n[1] =  0.0;
     n[2] =  raydir[0];
     
     
     if ((ln = length_vec3(n)) == 0.0)
     {
		 d	=	dot_vec3(raybase,raybase);
         
         tVals[0] = -3.402823466e+38F;
         tVals[1] = 3.402823466e+38F;
         
         return (d <= (radius*radius));
     }
	 scale_vec3(n,1.0f/ln);

     d	 = libc_fabsf(dot_vec3(raybase,n)); 
     hit = (d <= radius);

     
     if (hit)
     {
		 O[0] = -raybase[2];
		 O[1] =  0.0;
		 O[2] =  raybase[0];

         t		  = -dot_vec3	(O,n) / ln;

		 O[0] = -n[2];
		 O[1] =  0.0;
		 O[2] =  n[0];

		 normalize_vec3	(O);
         
         s		  = libc_fabsf (libc_sqrtf(radius - d * d)/dot_vec3(raydir,O));
         tVals[0] = t - s; 
         tVals[1] = t + s; 
     }

     return hit;
 }

 int clipcyl(const vec3f_t raybase, const vec3f_t raydir,float height, float *tVals)
{
  float dc, dwb, dwt, tb, tt;
  float in, out; 

  in  = tVals[0];
  out = tVals[1];

  dc  = raydir[1];
  dwb = raybase[1] ;
  dwt = raybase[1] - height;

  
  if (dc == 0.0)
  {
      if (dwb <= 0.0)
          return 0;
      if (dwt >= 0.0)
          return 0;
  }
  else
  {
      
      tb = -dwb / dc;
      
      tt = -dwt / dc;

      
      if (dc >= 0.0)
      {
          if (tb > out)
              return 0;
          if (tt < in)
              return 0;
          if (tb > in && tb < out)
              in = tb;
          if (tt > in && tt < out)
              out = tt;
      }
      
      else
      {
          if (tb < in)
              return 0;
          if (tt > out)
              return 0;
          if (tb > in && tb < out)
              out = tb;
          if (tt > in && tt < out)
              in = tt;
      }
  }

  tVals[0] = in;
  tVals[1] = out;
  return (in < out);
 }

 
int intersectRayCyl(const rayf_t *ray,const cylf_t *cyl,float *tVals,vec3f_t cylNorm,float *uv_coord)
{
	
	float			tworays[8+16];
	float			*tworays_a;
	float			*raypos;
	float			*raydir;

	tworays_a		=(((unsigned int)(&tworays[0]))&0xFFFFFFF0)+16;
	
	raypos	=	tworays_a;
	raydir	=	&tworays_a[4];
	

	sub_vec3	(ray->origin	,cyl->base			,raypos);
	mul_vec3x3	(raypos			,cyl->inv_matrix);
	mul_vec3x3_o(ray->direction	,cyl->inv_norm_mat	,raydir);
	

	if (!intcyl(raypos, raydir, cyl->radiussq, tVals))
	{
		
        return 0;
	}

    if (!clipcyl(raypos, raydir, cyl->height, tVals))
	{
		
        return 0;
	}

	if(cylNorm!=0x00000000)
	{
		vec3f_t		A,C,V,P;

		raypos[0]			=	raypos[0]+raydir[0]*tVals[0];
		raypos[1]			=	raypos[1]+raydir[1]*tVals[0];
		raypos[2]			=	raypos[2]+raydir[2]*tVals[0];

		C[0]				=	0.0f;
		C[1]				=	cyl->height/2.0f;
		C[2]				=	0.0f;

		A[0]				=	0.0f;
		A[1]				=	1.0f;
		A[2]				=	0.0f;

		
		sub_vec3			(raypos	,C,V);
		project_vec3		(V		,A,P);

		sub_vec3			(V,P,cylNorm); 
		normalize_vec3		(cylNorm);

		uv_coord[0]			=	libc_asinf(cylNorm[0])/3.1415926535897932384626433832795f + 0.5f;
		uv_coord[1]			=	length_vec3(P)/cyl->height;

		mul_vec3x3		(cylNorm,cyl->norm_mat);
	}

	
	return 1;
}


int	Sphere_intersect(const spheref_t *sphere,const vec3f_t raypos ,const vec3f_t direction,float* t,float *t2)
{
	float l[3];
	float a,b,c,disc;
	float distSqrt,q;
	float t0;
	float t1 ;

	sub_vec3	(raypos,sphere->center,l);
    
	a = dot_vec3(direction, direction);
    b = 2 * dot_vec3(direction,l);
    c = dot_vec3(sphere->center,sphere->center) + dot_vec3(raypos,raypos) -2*dot_vec3(sphere->center,raypos) - sphere->sq_radius;

    
    disc = b * b - 4 * a * c;
    
    
    
    if (disc < 0)
        return 0;

    
    distSqrt = libc_sqrtf(disc);

    if (b < 0)
        q = (-b - distSqrt)/(2.0f*a);
    else
        q = (-b + distSqrt)/(2.0f*a);

    
    t0 = q / a;
    t1 = c / q;

    
    if (t0 > t1)
    {
        
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    
    
    if (t1 < 0)
        return 0;
		

    
    if (t0 < 0)
    {
        *t = t1;
		if(t2)
			*t2=t0;
    }
    
    else
    {
        *t = t0;

		if(t2)
			*t2=t1;
	}


	return 1;
}
