
#include <xmmintrin.h>
#define dllexport __declspec(dllexport) 

#define PIf 3.1415926535897932384626433832795f
#define PId 3.1415926535897932384626433832795

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define POSITIVE_INFINITY 3.402823466e+38F
#define NEGATIVE_INFINITY -POSITIVE_INFINITY 

typedef float float_func (float a);
typedef double double_func (double a);

typedef float_func  *float_func_ptr;
typedef double_func *double_func_ptr;



#ifdef __cplusplus 
extern "C" {
#endif

extern    float libc_atanf(float a);
extern	  float libc_sinf(float a);
extern	  float libc_cosf(float a);

extern	 double libc_atand(double a);


extern	 double libc_sind(double a);
extern	 double libc_cosd(double a);
extern	 double  libc_sqrtd(double a);

static __inline	float  libc_sqrtf(float a)
{
	float o;
	_mm_store_ss(&o,_mm_sqrt_ss(_mm_load_ss(&a)));
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
  if (A==1.0f)   return PIf/2.0f;
  if (A==-1.0f)return PIf/-2.0f;
  return libc_atanf(A/libc_sqrtf(1.0f-A*A));
}


static __inline float libc_acosf ( float A ){
  if (A>1.0f||A<-1.0f) return 0.0f;
  if (A==0.0f)		   return PIf/2.0f;
	A=libc_atanf(libc_sqrtf(1.0f-A*A)/A);
  if (A<0.0f)A+=PIf;
  return A;
}


static __inline double libc_acosd ( double A ){
  if (A>1.0||A<-1.0) return 0.0;
  if (A==0.0)		 return PId/2.0;
  
  A=libc_atand(libc_sqrtd(1.0-A*A)/A);
  if (A<0.0)A+=PId;
  return A;
}

static __inline double libc_asind  ( double A ){
  
  if (A>1.0||A<-1.0)return 0.0;
  if (A==1.0)   return PId/2.0;
  if (A==-1.0)return PId/-2.0;
  return libc_atand(A/libc_sqrtd(1.0f-A*A));
}



#ifdef __cplusplus 
}
#endif