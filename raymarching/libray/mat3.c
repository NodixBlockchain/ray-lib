//psikounov geometry analytique
#include "..\libC\libc_math.h"
#include "vec_types.h"

#ifndef _DEBUG
//int _fltused=1;
#endif

#define EulFrmS	     0
#define EulFrmR	     1
#define EulFrm(ord)  ((unsigned)(ord)&1)
#define EulRepNo     0
#define EulRepYes    1
#define EulRep(ord)  (((unsigned)(ord)>>1)&1)
#define EulParEven   0
#define EulParOdd    1
#define EulPar(ord)  (((unsigned)(ord)>>2)&1)
#define EulSafe	     "\000\001\002\000"
#define EulNext	     "\001\002\000\001"
#define EulAxI(ord)  ((int)(EulSafe[(((unsigned)(ord)>>3)&3)]))
#define EulAxJ(ord)  ((int)(EulNext[EulAxI(ord)+(EulPar(ord)==EulParOdd)]))
#define EulAxK(ord)  ((int)(EulNext[EulAxI(ord)+(EulPar(ord)!=EulParOdd)]))
#define EulAxH(ord)  ((EulRep(ord)==EulRepNo)?EulAxK(ord):EulAxI(ord))
    /* EulGetOrd unpacks all useful information about order simultaneously. */
#define EulGetOrd(ord,i,j,k,h,n,s,f) {unsigned o=ord;f=o&1;o>>1;s=o&1;o>>1;\
    n=o&1;o>>1;i=EulSafe[o&3];j=EulNext[i+n];k=EulNext[i+1-n];h=s?k:i;}
    /* EulOrd creates an order value between 0 and 23 from 4-tuple choices. */
#define EulOrd(i,p,r,f)	   (((((((i)<<1)+(p))<<1)+(r))<<1)+(f))
    /* Static axes */
#define EulOrdXYZs    EulOrd(X,EulParEven,EulRepNo,EulFrmS)
#define EulOrdXYXs    EulOrd(X,EulParEven,EulRepYes,EulFrmS)
#define EulOrdXZYs    EulOrd(X,EulParOdd,EulRepNo,EulFrmS)
#define EulOrdXZXs    EulOrd(X,EulParOdd,EulRepYes,EulFrmS)
#define EulOrdYZXs    EulOrd(Y,EulParEven,EulRepNo,EulFrmS)
#define EulOrdYZYs    EulOrd(Y,EulParEven,EulRepYes,EulFrmS)
#define EulOrdYXZs    EulOrd(Y,EulParOdd,EulRepNo,EulFrmS)
#define EulOrdYXYs    EulOrd(Y,EulParOdd,EulRepYes,EulFrmS)
#define EulOrdZXYs    EulOrd(Z,EulParEven,EulRepNo,EulFrmS)
#define EulOrdZXZs    EulOrd(Z,EulParEven,EulRepYes,EulFrmS)
#define EulOrdZYXs    EulOrd(Z,EulParOdd,EulRepNo,EulFrmS)
#define EulOrdZYZs    EulOrd(Z,EulParOdd,EulRepYes,EulFrmS)
    /* Rotating axes */
#define EulOrdZYXr    EulOrd(X,EulParEven,EulRepNo,EulFrmR)
#define EulOrdXYXr    EulOrd(X,EulParEven,EulRepYes,EulFrmR)
#define EulOrdYZXr    EulOrd(X,EulParOdd,EulRepNo,EulFrmR)
#define EulOrdXZXr    EulOrd(X,EulParOdd,EulRepYes,EulFrmR)
#define EulOrdXZYr    EulOrd(Y,EulParEven,EulRepNo,EulFrmR)
#define EulOrdYZYr    EulOrd(Y,EulParEven,EulRepYes,EulFrmR)
#define EulOrdZXYr    EulOrd(Y,EulParOdd,EulRepNo,EulFrmR)
#define EulOrdYXYr    EulOrd(Y,EulParOdd,EulRepYes,EulFrmR)
#define EulOrdYXZr    EulOrd(Z,EulParEven,EulRepNo,EulFrmR)
#define EulOrdZXZr    EulOrd(Z,EulParEven,EulRepYes,EulFrmR)
#define EulOrdXYZr    EulOrd(Z,EulParOdd,EulRepNo,EulFrmR)
#define EulOrdZYZr    EulOrd(Z,EulParOdd,EulRepYes,EulFrmR)

#define EulHPB		  EulOrdYXZr
#define	RIGHT_HANDZ(z)		(-z)

//float matrixes
identity_mat3x3_func_ptr			identity_mat3x3;
determinant_mat3x3_func_ptr			determinant_mat3x3;
inverse_mat3x3_func_ptr				inverse_mat3x3;

mul_mat3x3_func_ptr					mul_mat3x3;		
mul_mat3x3_o_func_ptr				mul_mat3x3_o;		
mul_mat3x3_revf2d_func_ptr			mul_mat3x3f2d;	
mul_mat3x3_rev_func_ptr				mul_mat3x3_rev;
mul_mat3x3_rev_o_func_ptr			mul_mat3x3_rev_o;
mul_mat3x3_revf2d_func_ptr			mul_mat3x3_revf2d;

get_mat3x3gl_func_ptr				get_mat3x3gl;		



//double matrixes
identity_mat3x3d_func_ptr			identity_mat3x3d;	
determinant_mat3x3d_func_ptr		determinant_mat3x3d;
inverse_mat3x3d_func_ptr			inverse_mat3x3d;

mul_mat3x3d_func_ptr				mul_mat3x3d;		
mul_mat3x3d_o_func_ptr				mul_mat3x3d_o;		
mul_mat3x3_revd_func_ptr			mul_mat3x3_revd;
mul_mat3x3_revd_o_func_ptr			mul_mat3x3_revd_o;



#define M(col,row) m[col*4+row]


static double RADperDEG = 3.1415926535897932384626433832795f/180.0;	

EulerAngles Eul_(float ai, float aj, float ah, int order)
{
    EulerAngles ea;
    ea.x = ai; ea.y = aj; ea.z = ah;
    ea.w = order;
    return (ea);
}
/* Construct quaternion from Euler angles (in radians). */
Quat Eul_ToQuat(EulerAngles ea)
{
    Quat qu;
    double a[3], ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
    int i,j,k,h,n,s,f;
    EulGetOrd(ea.w,i,j,k,h,n,s,f);
    if (f==EulFrmR) {float t = ea.x; ea.x = ea.z; ea.z = t;}
    if (n==EulParOdd) ea.y = -ea.y;
    ti = ea.x*0.5; tj = ea.y*0.5; th = ea.z*0.5;
    ci = libc_cosd(ti);  cj = libc_cosd(tj);  ch = libc_cosd(th);
    si = libc_sind(ti);  sj = libc_sind(tj);  sh = libc_sind(th);
    cc = ci*ch; cs = ci*sh; sc = si*ch; ss = si*sh;
    if (s==EulRepYes) {
	a[i] = cj*(cs + sc);	/* Could speed up with */
	a[j] = sj*(cc + ss);	/* trig identities. */
	a[k] = sj*(cs - sc);
	qu.w = cj*(cc - ss);
    } else {
	a[i] = cj*sc - sj*cs;
	a[j] = cj*ss + sj*cc;
	a[k] = cj*cs - sj*sc;
	qu.w = cj*cc + sj*ss;
    }
    if (n==EulParOdd) a[j] = -a[j];
    qu.x = a[X]; qu.y = a[Y]; qu.z = a[Z];
    return (qu);
}
#if 0
/* Construct matrix from Euler angles (in radians). */
void Eul_ToHMatrix(EulerAngles ea, HMatrix M)
{
    double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
    int i,j,k,h,n,s,f;
    EulGetOrd(ea.w,i,j,k,h,n,s,f);
    if (f==EulFrmR) {float t = ea.x; ea.x = ea.z; ea.z = t;}
    if (n==EulParOdd) {ea.x = -ea.x; ea.y = -ea.y; ea.z = -ea.z;}
    ti = ea.x;	  tj = ea.y;	th = ea.z;
    ci = libc_cosd(ti); cj = libc_cosd(tj); ch = libc_cosd(th);
    si = libc_sind(ti); sj = libc_sind(tj); sh = libc_sind(th);
    cc = ci*ch; cs = ci*sh; sc = si*ch; ss = si*sh;
    if (s==EulRepYes) {
	M[i][i] = cj;	  M[i][j] =  sj*si;    M[i][k] =  sj*ci;
	M[j][i] = sj*sh;  M[j][j] = -cj*ss+cc; M[j][k] = -cj*cs-sc;
	M[k][i] = -sj*ch; M[k][j] =  cj*sc+cs; M[k][k] =  cj*cc-ss;
    } else {
	M[i][i] = cj*ch; M[i][j] = sj*sc-cs; M[i][k] = sj*cc+ss;
	M[j][i] = cj*sh; M[j][j] = sj*ss+cc; M[j][k] = sj*cs-sc;
	M[k][i] = -sj;	 M[k][j] = cj*si;    M[k][k] = cj*ci;
    }
    M[W][X]=M[W][Y]=M[W][Z]=M[X][W]=M[Y][W]=M[Z][W]=0.0; M[W][W]=1.0;
}




/* Convert matrix to Euler angles (in radians). */
EulerAngles Eul_FromHMatrix(HMatrix M, int order)
{
    EulerAngles ea;
    int i,j,k,h,n,s,f;
    EulGetOrd(order,i,j,k,h,n,s,f);
    if (s==EulRepYes) {
	double sy = sqrt(M[i][j]*M[i][j] + M[i][k]*M[i][k]);
	if (sy > 16*FLT_EPSILON) {
	    ea.x = atan2(M[i][j], M[i][k]);
	    ea.y = atan2(sy, M[i][i]);
	    ea.z = atan2(M[j][i], -M[k][i]);
	} else {
	    ea.x = atan2(-M[j][k], M[j][j]);
	    ea.y = atan2(sy, M[i][i]);
	    ea.z = 0;
	}
    } else {
	double cy = sqrt(M[i][i]*M[i][i] + M[j][i]*M[j][i]);
	if (cy > 16*FLT_EPSILON) {
	    ea.x = atan2(M[k][j], M[k][k]);
	    ea.y = atan2(-M[k][i], cy);
	    ea.z = atan2(M[j][i], M[i][i]);
	} else {
	    ea.x = atan2(-M[j][k], M[j][j]);
	    ea.y = atan2(-M[k][i], cy);
	    ea.z = 0;
	}
    }
    if (n==EulParOdd) {ea.x = -ea.x; ea.y = - ea.y; ea.z = -ea.z;}
    if (f==EulFrmR) {float t = ea.x; ea.x = ea.z; ea.z = t;}
    ea.w = order;
    return (ea);
}

/* Convert quaternion to Euler angles (in radians). */
EulerAngles Eul_FromQuat(Quat q, int order)
{
    HMatrix M;
    double Nq = q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
    double s = (Nq > 0.0) ? (2.0 / Nq) : 0.0;
    double xs = q.x*s,	  ys = q.y*s,	 zs = q.z*s;
    double wx = q.w*xs,	  wy = q.w*ys,	 wz = q.w*zs;
    double xx = q.x*xs,	  xy = q.x*ys,	 xz = q.x*zs;
    double yy = q.y*ys,	  yz = q.y*zs,	 zz = q.z*zs;
    M[X][X] = 1.0 - (yy + zz); M[X][Y] = xy - wz; M[X][Z] = xz + wy;
    M[Y][X] = xy + wz; M[Y][Y] = 1.0 - (xx + zz); M[Y][Z] = yz - wx;
    M[Z][X] = xz - wy; M[Z][Y] = yz + wx; M[Z][Z] = 1.0 - (xx + yy);
    M[W][X]=M[W][Y]=M[W][Z]=M[X][W]=M[Y][W]=M[Z][W]=0.0; M[W][W]=1.0;
    return (Eul_FromHMatrix(M, order));
}
#endif

/* Fill v[] with axis (from origin), return angle for quaternion style rotation */
double  GetAngleAxis(double *hpb,double *v,unsigned int mod)
{
	Quat out;
	double	ang=0,s;
	EulerAngles inAngs;
	inAngs.x = (float)hpb[0];
	inAngs.y = (float)hpb[1];
	inAngs.z = (float)hpb[2];
	
	if(mod==0)
		inAngs.w = EulOrdYXZs;
	else if(mod==1)
		inAngs.w = EulOrdYXZr;


	out = Eul_ToQuat(inAngs);
	ang = libc_acosd(out.w);
	s	= libc_sind	(ang);
	ang *= 2.0/RADperDEG;
	
	if(libc_fabsf(s)>EPSILON)		 /* Is this valid???!!! */
	{
		v[0] = out.x/s;
		v[1] = out.y/s;
		v[2] = out.z/s;
	}
	else 
		v[0] = v[1] = v[2] = 0.0;
	
	return(ang);
}

void copy_mat3x3(mat3x3f_t m,const mat3x3f_t op)
{
	int n=12;
	while(n--){m[n]=op[n];}
}


void mat3x3_lookat( const vec3f_t eyePosition, const vec3f_t lookAt, const vec3f_t upVector,mat3x3f_t  rotmat)
{
vec3f_t e, l, u;
vec3f_t forward, side, up;

copy_vec3(e ,eyePosition);
copy_vec3(l , lookAt);
copy_vec3(u ,upVector);

// make sure up is normalized
copy_vec3(up ,upVector);
normalize_vec3(up);

// forward vector
sub_vec3(l,e,forward);
normalize_vec3(forward);

// side vector
cross_vec3(forward, up ,side);
normalize_vec3(side);

rotmat[0] = side[0];
rotmat[4] = side[1];
rotmat[8] = side[2];

rotmat[1] = up[0];
rotmat[5] = up[1];
rotmat[9] = up[2];

rotmat[2] = -forward[0];
rotmat[6] = -forward[1];
rotmat[10] = -forward[2];


}
float determinant_mat3x3_c(const mat3x3f_t m)

{
	/*
	The inverse of a 3x3 matrix:

	| a00 a01 a02 |-1             |   a22a11-a21a12  -(a22a01-a21a02)   a12a01-a11a02  |
	| a10 a11 a12 |    =  1/DET * | -(a22a10-a20a12)   a22a00-a20a02  -(a12a00-a10a02) |
	| a20 a21 a22 |               |   a21a10-a20a11  -(a21a00-a20a01)   a11a00-a10a01  |

	with DET  =  a00(a22a11-a21a12)-a10(a22a01-a21a02)+a20(a12a01-a11a02)
	*/
	//return (M(0,0)*(M(2,2)*M(1,1)-M(2,1)*M(1,2))-M(1,0)*(M(2,2)*M(0,1)-M(2,1)*M(0,2))+M(2,0)*(M(1,2)*M(0,1)-M(1,1)*M(0,2)));
	return (M(0,0)*(M(2,2)*M(1,1)-M(2,1)*M(1,2))-M(1,0)*(M(2,2)*M(0,1)-M(2,1)*M(0,2))+M(2,0)*(M(1,2)*M(0,1)-M(1,1)*M(0,2)));
}


double determinant_mat3x3d_c(const mat3x3d_t m)

{
	/*
	The inverse of a 3x3 matrix:

	| a00 a01 a02 |-1             |   a22a11-a21a12  -(a22a01-a21a02)   a12a01-a11a02  |
	| a10 a11 a12 |    =  1/DET * | -(a22a10-a20a12)   a22a00-a20a02  -(a12a00-a10a02) |
	| a20 a21 a22 |               |   a21a10-a20a11  -(a21a00-a20a01)   a11a00-a10a01  |

	with DET  =  a00(a22a11-a21a12)-a10(a22a01-a21a02)+a20(a12a01-a11a02)
	*/
	//return (M(0,0)*(M(2,2)*M(1,1)-M(2,1)*M(1,2))-M(1,0)*(M(2,2)*M(0,1)-M(2,1)*M(0,2))+M(2,0)*(M(1,2)*M(0,1)-M(1,1)*M(0,2)));
	return (M(0,0)*(M(2,2)*M(1,1)-M(2,1)*M(1,2))-M(1,0)*(M(2,2)*M(0,1)-M(2,1)*M(0,2))+M(2,0)*(M(1,2)*M(0,1)-M(1,1)*M(0,2)));
}

int inverse_mat3x3_c(const mat3x3f_t m,mat3x3f_t out)
{
	float det;
	
	det		=	 determinant_mat3x3(m);
	if(det==0)
		return 0;

	det		=	1.0f/det;

	out[0]	=	 (M(1,1)*M(2,2)-M(2,1)*M(1,2))*det;
	out[1]	=	-(M(0,1)*M(2,2)-M(0,2)*M(2,1))*det;
	out[2]	=	 (M(0,1)*M(1,2)-M(0,2)*M(1,1))*det;

	out[4]	=	-(M(1,0)*M(2,2)-M(1,2)*M(2,0))*det;
	out[5]	=	 (M(0,0)*M(2,2)-M(0,2)*M(2,0))*det;
	out[6]	=	-(M(0,0)*M(1,2)-M(1,0)*M(0,2))*det;

	out[8]	=	 (M(1,0)*M(2,1)-M(2,0)*M(1,1))*det;
	out[9]	=	-(M(0,0)*M(2,1)-M(2,0)*M(0,1))*det;
	out[10]	=	 (M(0,0)*M(1,1)-M(1,0)*M(0,1))*det;


	/*
	out[0]	=	 (M(1,1)*M(2,2)-M(2,1)*M(1,2))*det;
	out[1]	=	-(M(0,1)*M(2,2)-M(0,2)*M(2,1))*det;
	out[2]	=	 (M(0,1)*M(1,2)-M(0,2)*M(1,1))*det;

	out[3]	=	-(M(1,0)*M(2,2)-M(1,2)*M(2,0))*det;
	out[4]	=	 (M(0,0)*M(2,2)-M(0,2)*M(2,0))*det;
	out[5]	=	-(M(0,0)*M(1,2)-M(1,0)*M(0,2))*det;

	out[6]	=	 (M(1,0)*M(2,1)-M(2,0)*M(1,1))*det;
	out[7]	=	-(M(0,0)*M(2,1)-M(2,0)*M(0,1))*det;
	out[8]	=	 (M(0,0)*M(1,1)-M(1,0)*M(0,1))*det;
	*/


	return 1;

}

int inverse_mat3x3d_c(const mat3x3d_t m,mat3x3d_t out)
{
	double det;
	
	det		=	 determinant_mat3x3d(m);
	if(det==0)
		return 0;

	det		=	1.0f/det;


	out[0]		=	 (M(1,1)*M(2,2)-M(2,1)*M(1,2))*det;
	out[1]		=	-(M(0,1)*M(2,2)-M(0,2)*M(2,1))*det;
	out[2]		=	 (M(0,1)*M(1,2)-M(0,2)*M(1,1))*det;
	
	out[4]		=	-(M(1,0)*M(2,2)-M(1,2)*M(2,0))*det;
	out[5]		=	 (M(0,0)*M(2,2)-M(0,2)*M(2,0))*det;
	out[6]		=	-(M(0,0)*M(1,2)-M(1,0)*M(0,2))*det;
	
	out[8]		=	 (M(1,0)*M(2,1)-M(2,0)*M(1,1))*det;
	out[9]		=	-(M(0,0)*M(2,1)-M(2,0)*M(0,1))*det;
	out[10]		=	 (M(0,0)*M(1,1)-M(1,0)*M(0,1))*det;



	return 1;
}

void toOpenGL3x3(float *inMat,float *outMat)
{
	int i,j;
	for ( i=0; i<3; i++)
	{ 
		for ( j=0; j<3; j++)
		{
			outMat[3*i+j] = inMat[4*j+i];
		}
	}
}


void transpose_mat3x3(const mat3x3f_t m,mat3x3f_t out)
{
	int			i,j;
	
	for ( i=0; i<3; i++)
	{ 
		for ( j=0; j<3; j++)
		{
			out[4*i+j] = m[4*j+i];
		}
	}
}

void copy_mat3x3d(mat3x3d_t m,const mat3x3d_t op)
{
	int n=12;
	while(n--){m[n]=op[n];}
}

dllexport void identity_mat3x3_c(mat3x3f_t m)
{

	M(0,0)	= M(1,1)  = M(2,2)	 = 1.0;
	M(0,1)	= M(0,2)  = M(1,0)	 =
	M(1,2)	= M(2,0)  = M(2,1)	 = 0.0;

}
void get_mat3x3gl_c(const mat3x3f_t m,float glMat[9])
{
	glMat[0]=m[0];
	glMat[1]=m[1];
	glMat[2]=m[2];
	glMat[3]=m[4];
	glMat[4]=m[5];
	glMat[5]=m[6];
	glMat[6]=m[8];
	glMat[7]=m[9];
	glMat[8]=m[10];
}
void identity_mat3x3d_c(mat3x3d_t m)
{
	M(0,0)	= M(1,1)  = M(2,2)	 = 1.0;
	M(0,1)	= M(0,2)  = M(1,0)	 =
	M(1,2)	= M(2,0)  = M(2,1)	 = 0.0;
}

void mul_mat3x3f2d_c(mat3x3f_t m,const mat3x3d_t op)
{
	float	m0,m1,m2;

	m0=M(0,0);
	m1=M(0,1);
	m2=M(0,2);

	M(0,0) = m0 *op[0] + m1*op[4]  + m2 *op[8] ;
	M(0,1) = m0 *op[1] + m1*op[5]  + m2 *op[9] ;
	M(0,2) = m0 *op[2] + m1*op[6]  + m2 *op[10];

	m0=M(1,0);
	m1=M(1,1);
	m2=M(1,2);

	M(1,0) = m0 *op[0] + m1*op[4]  + m2 *op[8] ;
	M(1,1) = m0 *op[1] + m1*op[5]  + m2 *op[9] ;
	M(1,2) = m0 *op[2] + m1*op[6]  + m2 *op[10];


	m0=M(2,0);
	m1=M(2,1);
	m2=M(2,2);

	M(2,0) = m0 *op[0] + m1*op[4]  + m2 *op[8] ;
	M(2,1) = m0 *op[1] + m1*op[5]  + m2 *op[9] ;
	M(2,2) = m0 *op[2] + m1*op[6]  + m2 *op[10];
}

void mul_mat3x3_c(mat3x3f_t m,const mat3x3f_t op)
{
	float	m0,m1,m2;

	m0=M(0,0);
	m1=M(0,1);
	m2=M(0,2);

	M(0,0) = m0 *op[0] + m1*op[4]  + m2 *op[8] ;
	M(0,1) = m0 *op[1] + m1*op[5]  + m2 *op[9] ;
	M(0,2) = m0 *op[2] + m1*op[6]  + m2 *op[10];

	m0=M(1,0);
	m1=M(1,1);
	m2=M(1,2);

	M(1,0) = m0 *op[0] + m1*op[4]  + m2 *op[8] ;
	M(1,1) = m0 *op[1] + m1*op[5]  + m2 *op[9] ;
	M(1,2) = m0 *op[2] + m1*op[6]  + m2 *op[10];


	m0=M(2,0);
	m1=M(2,1);
	m2=M(2,2);

	M(2,0) = m0 *op[0] + m1*op[4]  + m2 *op[8] ;
	M(2,1) = m0 *op[1] + m1*op[5]  + m2 *op[9] ;
	M(2,2) = m0 *op[2] + m1*op[6]  + m2 *op[10];
}



void mul_mat3x3_rev_c(mat3x3f_t m,const mat3x3f_t op)
{
	float		op0,op1,op2;
	mat3x3f_t	temp;

	op0=op[0];
	op1=op[1];
	op2=op[2];

	temp[0] = op0 *M(0,0) + op1*M(1,0)  + op2 *M(2,0);
	temp[1] = op0 *M(0,1) + op1*M(1,1)  + op2 *M(2,1);
	temp[2] = op0 *M(0,2) + op1*M(1,2)  + op2 *M(2,2);

	op0=op[4];
	op1=op[5];
	op2=op[6];

	temp[4] = op0 *M(0,0) + op1*M(1,0)  + op2 *M(2,0);
	temp[5] = op0 *M(0,1) + op1*M(1,1)  + op2 *M(2,1);
	temp[6] = op0 *M(0,2) + op1*M(1,2)  + op2 *M(2,2);

	op0=op[8];
	op1=op[9];
	op2=op[10];

	temp[8]  = op0 *M(0,0) + op1*M(1,0)  + op2  *M(2,0);
	temp[9]  = op0 *M(0,1) + op1*M(1,1)  + op2  *M(2,1);
	temp[10] = op0 *M(0,2) + op1*M(1,2)  + op2  *M(2,2);

	copy_mat3x3(m,temp);
}


void mul_mat3x3_rev_o_c(const mat3x3f_t m,const mat3x3f_t op,mat3x3f_t out)
{
	copy_mat3x3			(out,m);
	mul_mat3x3_rev_c	(out,op);

}
void mul_mat3x3_o_c(const mat3x3f_t m,const mat3x3f_t op,mat3x3f_t out)
{
	copy_mat3x3			(out,m);
	mul_mat3x3_c		(out,op);
}


void mul_mat3x3d_c(mat3x3d_t m,const mat3x3d_t op)
{
	double	m0,m1,m2;

	m0=M(0,0);
	m1=M(0,1);
	m2=M(0,2);

	M(0,0) = m0 *op[0] + m1*op[4]  + m2 *op[8] ;
	M(0,1) = m0 *op[1] + m1*op[5]  + m2 *op[9] ;
	M(0,2) = m0 *op[2] + m1*op[6]  + m2 *op[10];

	m0=M(1,0);
	m1=M(1,1);
	m2=M(1,2);

	M(1,0) = m0 *op[0] + m1*op[4]  + m2 *op[8] ;
	M(1,1) = m0 *op[1] + m1*op[5]  + m2 *op[9] ;
	M(1,2) = m0 *op[2] + m1*op[6]  + m2 *op[10];


	m0=M(2,0);
	m1=M(2,1);
	m2=M(2,2);

	M(2,0) = m0 *op[0] + m1*op[4]  + m2 *op[8] ;
	M(2,1) = m0 *op[1] + m1*op[5]  + m2 *op[9] ;
	M(2,2) = m0 *op[2] + m1*op[6]  + m2 *op[10];
}
void mul_mat3x3_revf2d_c(mat3x3f_t m,const mat3x3d_t op)
{
	float		op0,op1,op2;
	mat3x3f_t	temp;

	op0=op[0];
	op1=op[1];
	op2=op[2];

	temp[0] = op0 *M(0,0) + op1*M(1,0)  + op2 *M(2,0);
	temp[1] = op0 *M(0,1) + op1*M(1,1)  + op2 *M(2,1);
	temp[2] = op0 *M(0,2) + op1*M(1,2)  + op2 *M(2,2);

	op0=op[4];
	op1=op[5];
	op2=op[6];

	temp[4] = op0 *M(0,0) + op1*M(1,0)  + op2 *M(2,0);
	temp[5] = op0 *M(0,1) + op1*M(1,1)  + op2 *M(2,1);
	temp[6] = op0 *M(0,2) + op1*M(1,2)  + op2 *M(2,2);

	op0=op[8];
	op1=op[9];
	op2=op[10];

	temp[8]  = op0 *M(0,0) + op1*M(1,0)  + op2 *M(2,0);
	temp[9]  = op0 *M(0,1) + op1*M(1,1)  + op2 *M(2,1);
	temp[10] = op0 *M(0,2) + op1*M(1,2)  + op2 *M(2,2);

	copy_mat3x3(m,temp);
}

void mul_mat3x3_revd_c(mat3x3d_t m,const mat3x3d_t op)
{
	double		op0,op1,op2;
	mat3x3d_t	temp;

	op0=op[0];
	op1=op[1];
	op2=op[2];

	temp[0] = op0 *M(0,0) + op1*M(1,0)  + op2 *M(2,0);
	temp[1] = op0 *M(0,1) + op1*M(1,1)  + op2 *M(2,1);
	temp[2] = op0 *M(0,2) + op1*M(1,2)  + op2 *M(2,2);

	op0=op[4];
	op1=op[5];
	op2=op[6];

	temp[4] = op0 *M(0,0) + op1*M(1,0)  + op2 *M(2,0);
	temp[5] = op0 *M(0,1) + op1*M(1,1)  + op2 *M(2,1);
	temp[6] = op0 *M(0,2) + op1*M(1,2)  + op2 *M(2,2);

	op0=op[8];
	op1=op[9];
	op2=op[10];

	temp[8]  = op0 *M(0,0) + op1*M(1,0)  + op2 *M(2,0);
	temp[9]  = op0 *M(0,1) + op1*M(1,1)  + op2 *M(2,1);
	temp[10] = op0 *M(0,2) + op1*M(1,2)  + op2 *M(2,2);

	copy_mat3x3d(m,temp);
}


void mul_mat3x3_revd_o_c(const mat3x3d_t m,const mat3x3d_t op,mat3x3d_t out)
{
	copy_mat3x3d	(out,op);
	mul_mat3x3d		(out,m);
}

void mul_mat3x3d_o_c(const mat3x3d_t m,const mat3x3d_t op,mat3x3d_t out)
{
	copy_mat3x3d	(out,m);
	mul_mat3x3d		(out,op);
}








void rotate_X_3f(vec3f_t v,float aD){
	
	float a;
	float vnew[3];

	a=(aD*PIf)/180.0f;

	vnew[1] = libc_cosf(a) * v[1] - libc_sinf(a) * v[1];
	vnew[2] = libc_sinf(a) * v[2] + libc_cosf(a) * v[2];

	v[1]	= vnew[1];
	v[2]	= vnew[2];
    
}

void rotate_Y_3f(vec3f_t v,float aD){
	
	float a;
	float vnew[3];

	a=(aD*PIf)/180.0f;

	vnew[0] = libc_cosf(a) * v[0] - libc_sinf(a) * v[2];
	vnew[2] = libc_sinf(a) * v[0] + libc_cosf(a) * v[2];

	v[0]	= vnew[0];
	v[2]	= vnew[2];
    
} 

void rotate_Z_3f(vec3f_t v,float aD){
	
	float a;
	float vnew[3];

	a		=(aD*PIf)/180.0f;

	vnew[0] = libc_cosf(a) * v[0] - libc_sinf(a) * v[1];
	vnew[1] = libc_sinf(a) * v[0] + libc_cosf(a) * v[1];
	
	v[0]	= vnew[0];
	v[1]	= vnew[1];
   
} 


 void project_vec3(vec3f_t A, vec3f_t B, vec3f_t out)
 {
	float dot1,dot2;
	float coef;

	dot1=dot_vec3(A,B);
	dot2=dot_vec3(B,B);
	coef=dot1/dot2;
	out[0]=B[0]*(coef);
	out[1]=B[1]*(coef);
	out[2]=B[2]*(coef);

 }


void set_vec4i			(int *vec,int a,int b,int c,int d)
{
	vec[0]=a;
	vec[1]=b;
	vec[2]=c;
	vec[3]=d;

}

void set_vec3			(vec3f_t vec,float a,float b,float c)
{
	vec[0]=a;
	vec[1]=b;
	vec[2]=c;
}





int refract_vec3(const vec3f_t normal, const vec3f_t incident,double n1, double n2,vec3f_t out)
{
	const double n = n1 / n2;
	const double cosI = dot_vec3(normal, incident);
	const double sinT2 = n * n * (1.0 - cosI * cosI);
	if (sinT2 > 1.0)
	{
		return 0;
	}

	out[0]=n*incident[0]-(n+libc_sqrtd(1.0-sinT2))*normal[0];
	out[1]=n*incident[1]-(n+libc_sqrtd(1.0-sinT2))*normal[1];
	out[2]=n*incident[2]-(n+libc_sqrtd(1.0-sinT2))*normal[2];

	
	return 1;
}


void reflect_vec3(const vec3f_t v,const vec3f_t n,vec3f_t r)
{
	float dot;

	dot	=	dot_vec3(v,n);

	r[0]=-(2*dot*n[0]-v[0]);
	r[1]=-(2*dot*n[1]-v[1]);
	r[2]=-(2*dot*n[2]-v[2]);


	//R = 2*(V dot N)*N - V
}







void init_mat3_fncs_c()
{
mul_mat3x3			=	mul_mat3x3_c;
mul_mat3x3d			=	mul_mat3x3d_c;

get_mat3x3gl		=	get_mat3x3gl_c;

mul_mat3x3_o		=	mul_mat3x3_o_c;
mul_mat3x3d_o		=	mul_mat3x3d_o_c;
mul_mat3x3f2d		=	mul_mat3x3f2d_c;

mul_mat3x3_rev		=	mul_mat3x3_rev_c;
mul_mat3x3_rev_o	=	mul_mat3x3_rev_o_c;
mul_mat3x3_revf2d	=	mul_mat3x3_revf2d_c;

mul_mat3x3_revd		=	mul_mat3x3_revd_c;
mul_mat3x3_revd_o	=	mul_mat3x3_revd_o_c;

identity_mat3x3		=	identity_mat3x3_c;
determinant_mat3x3	=	determinant_mat3x3_c;
inverse_mat3x3		=	inverse_mat3x3_c;

identity_mat3x3d	=	identity_mat3x3d_c;	
determinant_mat3x3d	=	determinant_mat3x3d_c;
inverse_mat3x3d		=	inverse_mat3x3d_c;


}

#if 0
/***********************/
/*   Useful Routines   */
/***********************/

/* return roots of ax^2+bx+c */
/* stable algebra derived from Numerical Recipes by Press et al.*/
int quadraticRoots(a, b, c, roots)
double a, b, c, *roots;
{
double d, q;
int count = 0;
	d = (b*b)-(4*a*c);
	if (d < 0.0) { *roots = *(roots+1) = 0.0;  return(0); }
	q =  -0.5 * (b + (SGN(b)*sqrt(d)));
	if (a != 0.0)  { *roots++ = q/a; count++; }
	if (q != 0.0) { *roots++ = c/q; count++; }
	return(count);
	}


/* generic 1d regula-falsi step.  f is function to evaluate */
/* interval known to contain root is given in left, right */
/* returns new estimate */
double RegulaFalsi(f, left, right)
double (*f)(), left, right;
{
double d = (*f)(right) - (*f)(left);
	if (d != 0.0) return (right - (*f)(right)*(right-left)/d);
	return((left+right)/2.0);
	}

/* generic 1d Newton-Raphson step. f is function, df is derivative */
/* x is current best guess for root location. Returns new estimate */
double NewtonRaphson(f, df, x)
double (*f)(), (*df)(), x;
{
double d = (*df)(x);
	if (d != 0.0) return (x-((*f)(x)/d));
	return(x-1.0);
	}


/* hybrid 1d Newton-Raphson/Regula Falsi root finder. */
/* input function f and its derivative df, an interval */
/* left, right known to contain the root, and an error tolerance */
/* Based on Blinn */
double findroot(left, right, tolerance, f, df)
double left, right, tolerance;
double (*f)(), (*df)();
{
double newx = left;
	while (ABS((*f)(newx)) > tolerance) {
		newx = NewtonRaphson(f, df, newx);
		if (newx < left || newx > right) 
			newx = RegulaFalsi(f, left, right);
		if ((*f)(newx) * (*f)(left) <= 0.0) right = newx;  
			else left = newx;
		}
	return(newx);
	} 
#endif