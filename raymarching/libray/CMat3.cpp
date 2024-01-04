//psikounov geometry analytique
#include "../libC/libc_math.h"
#include "vec_types.h"
#include "mat3.h"



copy_mat3x3d_func_ptr			cmat_funcs_d::copy_mat3x3			=	NULL;

mul_mat3x3d_func_ptr			cmat_funcs_d::mul_mat3x3			=	NULL;
mul_mat3x3d_o_func_ptr			cmat_funcs_d::mul_mat3x3_o			=	NULL;
mul_mat3x3_revd_func_ptr		cmat_funcs_d::mul_mat3x3d			=	NULL;

mul_mat3x3_revd_func_ptr		cmat_funcs_d::mul_mat3x3_rev		=	NULL;
mul_mat3x3_revd_func_ptr		cmat_funcs_d::mul_mat3x3_revd		=	NULL;
mul_mat3x3_revd_o_func_ptr		cmat_funcs_d::mul_mat3x3_rev_o		=	NULL;

identity_mat3x3d_func_ptr		cmat_funcs_d::identity_mat3x3		=	NULL;
determinant_mat3x3d_func_ptr	cmat_funcs_d::determinant_mat3x3	=	NULL;
inverse_mat3x3d_func_ptr		cmat_funcs_d::inverse_mat3x3		=	NULL;

double_func_ptr					cmat_funcs_d::sin					=	NULL;
double_func_ptr					cmat_funcs_d::cos					=	NULL;
double_func_ptr					cmat_funcs_d::sqrt					=	NULL;


copy_mat3x3_func_ptr			cmat_funcs::copy_mat3x3				=	NULL;
mul_mat3x3_func_ptr				cmat_funcs::mul_mat3x3				=	NULL;
mul_mat3x3f2d_func_ptr			cmat_funcs::mul_mat3x3d				=	NULL;
mul_mat3x3_o_func_ptr			cmat_funcs::mul_mat3x3_o			=	NULL;

mul_mat3x3_rev_func_ptr			cmat_funcs::mul_mat3x3_rev			=	NULL;
mul_mat3x3_revf2d_func_ptr		cmat_funcs::mul_mat3x3_revd			=	NULL;
mul_mat3x3_rev_o_func_ptr		cmat_funcs::mul_mat3x3_rev_o		=	NULL;

 identity_mat3x3_func_ptr		cmat_funcs::identity_mat3x3			=	NULL;
determinant_mat3x3_func_ptr		cmat_funcs::determinant_mat3x3		=	NULL;
inverse_mat3x3_func_ptr			cmat_funcs::inverse_mat3x3			=	NULL;

float_func_ptr					cmat_funcs::sin						=	NULL;
float_func_ptr					cmat_funcs::cos						=	NULL;
float_func_ptr					cmat_funcs::sqrt					=	NULL;


void blabla()
{
	CMat3 <float,cmat_funcs>		testf,normf;
	CMat3 <double,cmat_funcs_d>		testd,normd;
	mat3x3f_t		matf;
	mat3x3d_t		matd;
	
	testf	=	testf*matf;
	testf   *=  matf;
	
	testf.rotate(45.0f,0.0f,1.0f,0.0f);

	normf	=	testf.getNormalMat();
	
	testd	=	testd*matd;
	testd   *=  matd;


	testd.rotate				(45.0f,0.0f,1.0f,0.0f);
	normd=testd.getNormalMat	();
	
}


extern "C"
{
	void init_mat3_fncs()
	{
		init_mat3_fncs_c();

		cmat_funcs_d::copy_mat3x3			=	copy_mat3x3d;
		cmat_funcs_d::mul_mat3x3			=	mul_mat3x3d;
		cmat_funcs_d::mul_mat3x3_o			=	mul_mat3x3d_o;
		cmat_funcs_d::mul_mat3x3d			=	mul_mat3x3d;

		cmat_funcs_d::mul_mat3x3_rev		=	mul_mat3x3_revd;
		cmat_funcs_d::mul_mat3x3_revd		=	mul_mat3x3_revd;
		cmat_funcs_d::mul_mat3x3_rev_o		=	mul_mat3x3_revd_o;


		cmat_funcs_d::identity_mat3x3		=	identity_mat3x3d;
		cmat_funcs_d::determinant_mat3x3	=	determinant_mat3x3d;
		cmat_funcs_d::inverse_mat3x3		=	inverse_mat3x3d;

		cmat_funcs_d::sin					=	libc_sind;
		cmat_funcs_d::cos					=	libc_cosd;
		cmat_funcs_d::sqrt					=	libc_sqrtd;


		cmat_funcs::copy_mat3x3				=	copy_mat3x3;
		cmat_funcs::mul_mat3x3				=	mul_mat3x3;
		cmat_funcs::mul_mat3x3d				=	mul_mat3x3f2d;
		cmat_funcs::mul_mat3x3_o			=	mul_mat3x3_o;

		cmat_funcs::mul_mat3x3_rev			=	mul_mat3x3_rev;
		cmat_funcs::mul_mat3x3_revd			=	mul_mat3x3_revf2d;
		cmat_funcs::mul_mat3x3_rev_o		=	mul_mat3x3_rev_o;

		cmat_funcs::identity_mat3x3			=	identity_mat3x3;
		cmat_funcs::determinant_mat3x3		=	determinant_mat3x3;
		cmat_funcs::inverse_mat3x3			=	inverse_mat3x3;

		cmat_funcs::sin						=	libc_sinf;
		cmat_funcs::cos						=	libc_cosf;
		cmat_funcs::sqrt					=	libc_sqrtf;

	}
}
