class cmat_funcs_d 
{
public:
	static identity_mat3x3d_func_ptr		identity_mat3x3;
	static determinant_mat3x3d_func_ptr		determinant_mat3x3;
	static inverse_mat3x3d_func_ptr			inverse_mat3x3;

	static copy_mat3x3d_func_ptr			copy_mat3x3;

	static mul_mat3x3d_func_ptr				mul_mat3x3;
	static mul_mat3x3d_func_ptr				mul_mat3x3d;
	
 	static mul_mat3x3d_o_func_ptr			mul_mat3x3_o;
	
	static mul_mat3x3_revd_func_ptr			mul_mat3x3_rev;
	static mul_mat3x3_revd_func_ptr			mul_mat3x3_revd;
	static mul_mat3x3_revd_o_func_ptr		mul_mat3x3_rev_o;

	static double_func_ptr					sin;
	static double_func_ptr					cos;
	static double_func_ptr					sqrt;


};



class cmat_funcs
{
public:
	static identity_mat3x3_func_ptr			identity_mat3x3;
	static determinant_mat3x3_func_ptr		determinant_mat3x3;
	static inverse_mat3x3_func_ptr			inverse_mat3x3;

	static copy_mat3x3_func_ptr				copy_mat3x3;

	static mul_mat3x3_func_ptr				mul_mat3x3;
	static mul_mat3x3f2d_func_ptr			mul_mat3x3d;
	static mul_mat3x3_o_func_ptr			mul_mat3x3_o;
	
	static mul_mat3x3_rev_func_ptr			mul_mat3x3_rev;
	static mul_mat3x3_revf2d_func_ptr		mul_mat3x3_revd;
	static mul_mat3x3_rev_o_func_ptr		mul_mat3x3_rev_o;

	static float_func_ptr					sin;
	static float_func_ptr					cos;
	static float_func_ptr					sqrt;

};




template <typename m_type,typename funcs_type>
class CMat3
{
	

public:
	CMat3()
	{
		identity();
	}
	void identity()
	{
		funcs_type::identity_mat3x3(m);
	}

	void create_rotation_mat(m_type angle,m_type aX,m_type aY,m_type aZ)
	{
	   m_type  x,y,z;
	   m_type  xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c, s, c;
	   int    optimized;

	   x	=	aX;
	   y	=	aY;
	   z	=	aZ;

	   s 	=	funcs_type::sin( angle );
	   c 	=	funcs_type::cos( angle );

	   identity();


	   optimized = 0;

	   #define M(row,col)  m[col*4+row]
		   if (x == 0.0) {
			  if (y == 0.0) {
				 if (z != 0.0) {
					optimized = 1;
					/* rotate only around z-axis */
					M(0,0) = c;
					M(1,1) = c;
					if (z < 0.0) {
					   M(0,1) = s;
					   M(1,0) = -s;
					}
					else {
					   M(0,1) = -s;
					   M(1,0) = s;
					}
				 }
			  }
			  else if (z == 0.0) {
				 optimized = 1;
				 /* rotate only around y-axis */
				 M(0,0) = c;
				 M(2,2) = c;
				 if (y < 0.0) {
					M(0,2) = -s;
					M(2,0) = s;
				 }
				 else {
					M(0,2) = s;
					M(2,0) = -s;
				 }
			  }
		   }
		   else if (y == 0.0) {
			  if (z == 0.0) {
				 optimized = 1;
				 /* rotate only around x-axis */
				 M(1,1) = c;
				 M(2,2) = c;
				 if (x < 0.0) {
					M(1,2) = s;
					M(2,1) = -s;
				 }
				 else {
					M(1,2) = -s;
					M(2,1) = s;
				 }
			  }
		   }

		   if (!optimized) {
			  const m_type mag = funcs_type::sqrt(x * x + y * y + z * z);

			  if (mag <= 1.0e-4) {
				 /* no rotation, leave mat as-is */
				 return;
			  }

			  x /= mag;
			  y /= mag;
			  z /= mag;


			  /*
			   *     Arbitrary axis rotation matrix.
			   *
			   *  This is composed of 5 matrices, Rz, Ry, T, Ry', Rz', multiplied
			   *  like so:  Rz * Ry * T * Ry' * Rz'.  T is the final rotation
			   *  (which is about the X-axis), and the two composite transforms
			   *  Ry' * Rz' and Rz * Ry are (respectively) the rotations necessary
			   *  from the arbitrary axis to the X-axis then back.  They are
			   *  all elementary rotations.
			   *
			   *  Rz' is a rotation about the Z-axis, to bring the axis vector
			   *  into the x-z plane.  Then Ry' is applied, rotating about the
			   *  Y-axis to bring the axis vector parallel with the X-axis.  The
			   *  rotation about the X-axis is then performed.  Ry and Rz are
			   *  simply the respective inverse transforms to bring the arbitrary
			   *  axis back to it's original orientation.  The first transforms
			   *  Rz' and Ry' are considered inverses, since the data from the
			   *  arbitrary axis gives you info on how to get to it, not how
			   *  to get away from it, and an inverse must be applied.
			   *
			   *  The basic calculation used is to recognize that the arbitrary
			   *  axis vector (x, y, z), since it is of unit length, actually
			   *  represents the sines and cosines of the angles to rotate the
			   *  X-axis to the same orientation, with theta being the angle about
			   *  Z and phi the angle about Y (in the order described above)
			   *  as follows:
			   *
			   *  cos ( theta ) = x / sqrt ( 1 - z^2 )
			   *  sin ( theta ) = y / sqrt ( 1 - z^2 )
			   *
			   *  cos ( phi ) = sqrt ( 1 - z^2 )
			   *  sin ( phi ) = z
			   *
			   *  Note that cos ( phi ) can further be inserted to the above
			   *  formulas:
			   *
			   *  cos ( theta ) = x / cos ( phi )
			   *  sin ( theta ) = y / sin ( phi )
			   *
			   *  ...etc.  Because of those relations and the standard trigonometric
			   *  relations, it is pssible to reduce the transforms down to what
			   *  is used below.  It may be that any primary axis chosen will give the
			   *  same results (modulo a sign convention) using thie method.
			   *
			   *  Particularly nice is to notice that all divisions that might
			   *  have caused trouble when parallel to certain planes or
			   *  axis go away with care paid to reducing the expressions.
			   *  After checking, it does perform correctly under all cases, since
			   *  in all the cases of division where the denominator would have
			   *  been zero, the numerator would have been zero as well, giving
			   *  the expected result.
			   */

			  xx = x * x;
			  yy = y * y;
			  zz = z * z;
			  xy = x * y;
			  yz = y * z;
			  zx = z * x;
			  xs = x * s;
			  ys = y * s;
			  zs = z * s;
			  one_c = 1.0 - c;

			  /* We already hold the identity-matrix so we can skip some statements */
			  M(0,0) = (one_c * xx) + c;
			  M(0,1) = (one_c * xy) - zs;
			  M(0,2) = (one_c * zx) + ys;
		      

			  M(1,0) = (one_c * xy) + zs;
			  M(1,1) = (one_c * yy) + c;
			  M(1,2) = (one_c * yz) - xs;
		      

			  M(2,0) = (one_c * zx) - ys;
			  M(2,1) = (one_c * yz) + xs;
			  M(2,2) = (one_c * zz) + c;
		   
		   }
		#undef M
	}

	void rotate(m_type angle,m_type aX,m_type aY,m_type aZ)
	{
	   CMat3<double,cmat_funcs_d> rot_mat;

	   rot_mat.create_rotation_mat	(angle,aX,aY,aZ);
	   this->transform				(rot_mat.m);
	}

	void rotateEuler(m_type aX,m_type aY,m_type aZ)
	{
	   CMat3<double,cmat_funcs_d>	rot_mat;
	   double			hbp[3];
	   double			axis[3];
	   double			angle;

	   hbp[0]	=	aX;
	   hbp[1]	=	aY;
	   hbp[2]	=	aZ;

	   angle	=	GetAngleAxis	(hbp,axis,1);

	   rot_mat.create_rotation_mat	(angle,axis[0],axis[1],axis[2]);
	   funcs_type::mul_mat3x3d		(m,rot_mat.m);
	   //this->transform				(rot_mat.m);
	}

	m_type &operator[](int idx)
	{
		return m[idx];
	}
	
	void transform (mat3x3f_t op)
	{
		funcs_type::mul_mat3x3_rev(m,op);
	}

	void transform (mat3x3d_t op)
	{
		funcs_type::mul_mat3x3_revd(m,op);
	}

	m_type determinant()
	{
		return funcs_type::determinant_mat3x3(m);
	}

	int inverseTo(mat3x3f_t out)
	{
		return funcs_type::inverse_mat3x3(m,out);
	}

	int inverseTo(mat3x3d_t out)
	{
		return funcs_type::inverse_mat3x3(m,out);
	}

	CMat3 <m_type,funcs_type>operator*(mat3x3f_t op)
	{
		CMat3 <m_type,funcs_type> ret_m;
		mul_mat3x3_o(this->m,op,ret_m.m);
		return ret_m;
	}

	CMat3 <m_type,funcs_type>operator*=(mat3x3f_t op)
	{
		CMat3 <m_type,funcs_type> ret_m;
		mul_mat3x3_o(this->m,op,ret_m.m);
		return ret_m;
	}



	CMat3 <m_type,funcs_type>operator*(mat3x3d_t op)
	{
		CMat3 <m_type,funcs_type> ret_m;
		funcs_type::mul_mat3x3_o(this->m,op,ret_m.m);
		return ret_m;
	}

	CMat3 <m_type,funcs_type>operator*=(mat3x3d_t op)
	{
		CMat3 <m_type,funcs_type> ret_m;
		funcs_type::mul_mat3x3_o(this->m,op,ret_m.m);
		return ret_m;
	}

	CMat3 <m_type,funcs_type>operator*(const CMat3 <m_type,funcs_type> &op)
	{
		CMat3 <m_type,funcs_type> ret_m;
		funcs_type::mul_mat3x3_o(this->m,op.m,ret_m.m);
		return ret_m;
	}

	CMat3 <m_type,funcs_type>operator*=(const CMat3 <m_type,funcs_type> &op)
	{
		funcs_type::mul_mat3x3(this.m,op.m);
		return *this;
	}

	void transpose()
	{
		int			i,j;
		m_type		temp;
		for ( i=0; i<3; i++)
		{ 
			for ( j=0; j<3; j++)
			{
			   if(i>j)
				{
					temp     = m[4*i+j];
					m[4*i+j] = m[4*j+i];
					m[4*j+i] = temp;
				}
			}
		}
	}

	CMat3 <m_type,funcs_type> getNormalMat ()const
	{
		CMat3 <m_type,funcs_type> ret;

		//inverseTo		(ret.m);
		//ret.transpose	();
		funcs_type::copy_mat3x3	(ret.m,m);

		return ret;
	}

	m_type		m[16];
	
};

typedef CMat3 <float,cmat_funcs>	CMat3f;
typedef CMat3 <double,cmat_funcs_d> CMat3d;

class vec3SSE
{
public:
	vec3f_t v;

protected:
	
	__forceinline  vec3SSE & normalize()
	{
		normalize_vec3(v);
		return (*this);
	}

	__forceinline  vec3SSE &copy(const vec3SSE & v2)
	{
		copy_vec3	(v,v2.v);
		return (*this);
	}
	__forceinline  vec3SSE &copy(const vec3f_t v2)
	{
		copy_vec3	(v,v2);
		return (*this);
	}

	 __forceinline  vec3SSE &zero()
	{
		zero_vec3(v);
		return (*this);
	}
	__forceinline   vec3SSE &mul(const vec3SSE & v2)
	{
		mul_vec3(v,v2.v);
		return (*this);
	}	
	__forceinline   vec3SSE &scale(float d)
	{
		scale_vec3(v,d);
		return (*this);
	}
	__forceinline   vec3SSE &mul_mat3x3(const mat3x3f_t m)
	{
		mul_vec3x3(v,m);
		return (*this);
	}
	__forceinline  const vec3SSE &copy_to(vec3f_t v2)const
	{
		copy_vec3	(v2,v);
		return (*this);
	}
	__forceinline  vec3SSE get_normalized()const
	{
		vec3SSE		Out;
		normalize_vec3_o(v,Out.v);
		return Out;
	}
	__forceinline   vec3SSE add(const vec3SSE & v2)const
	{
		vec3SSE		Out;
		add_vec3(v,v2.v,Out.v);
		return Out;
	}

	__forceinline   vec3SSE scale_o(float d)const
	{
		vec3SSE		Out;
		scale_vec3_o(v,d,Out.v);
		return Out;
	}
	__forceinline   vec3SSE sub		(const vec3SSE &v2)const
	{
		vec3SSE		Out;
		sub_vec3	(v,v2.v,Out.v);
		return		Out;
	}

	__forceinline   vec3SSE sub		(const vec3f_t &v2)const
	{
		vec3SSE		Out;
		sub_vec3	(v,v2,Out.v);
		return		Out;
	}

	__forceinline   vec3SSE sub_scale_o(const vec3SSE & dr,float d)const
	{
		vec3SSE		Out;
		sub_scale_vec3_o(v,dr.v,d,Out.v);
		return Out;
	}
	__forceinline  vec3SSE cross(const vec3SSE &v2) const
	{
		vec3SSE		Out;
		cross_vec3(v,v2.v,Out.v);
		return Out;
	}
	__forceinline   vec3SSE mul_mat3x3_o(const mat3x3f_t m)const
	{
		vec3SSE  Out;
		mul_vec3x3_o(v,m,Out.v);
		return Out;
	}

	__forceinline   float dot(const vec3SSE & vec2)const
	{
		return dot_vec3(v,vec2.v);
	}
	__forceinline   float dot(const vec3f_t vec2)const
	{
		return dot_vec3(v,vec2);
	}
	__forceinline   float dot_self()const
	{
		return dot_self_vec3(v);
	}
	__forceinline   float sq_dist(const vec3SSE & vec2)const
	{
		return  sq_dist_vec3(v,vec2.v);
	}
	__forceinline   float dist(vec3SSE & vec2)const
	{
		return  dist_vec3(v,vec2.v);
	}
	__forceinline   float length()const
	{
		return libc_sqrtf(dot_self());
	}
};

template <typename m_type>
class vec3:public m_type
{
public:
	vec3()
	{
		zero();
	}
	vec3(float x,float y,float z)
	{
		v[0]=x;
		v[1]=y;
		v[2]=z;
	}
	//copy
	__forceinline  m_type &operator=(const m_type &in)
	{
		return this->copy(in);
	}
	__forceinline m_type &operator=(const vec3f_t in)
	{
		return this->copy(in);
	}
	__forceinline float &operator[](int idx) 
	{
		return v[idx];
	}
	__forceinline const m_type &out_to(vec3f_t out)const
	{
		return this->copy_to(out);
	}

	//Matrices operator on self
	__forceinline m_type &operator*=(const CMat3f &op)
	{
		return this->mul_mat3x3(op.m);
	}
	__forceinline  m_type &operator*=(const mat3x3f_t op)
	{
		return this->mul_mat3x3(op);
	}

	//scale self
	__forceinline  m_type &operator*=(float d)
	{
		return this->scale(d);
	}
	//normalize self
	__forceinline void lengthTo1()
	{
		this->normalize();
	}

	//const Matrix operator
	__forceinline m_type operator*(const CMat3f &op)const
	{
		return this->mul_mat3x3_o(op.m,out.v);
	}
	__forceinline m_type operator*(const mat3x3f_t op)const
	{
		return this->mul_mat3x3_o(op);
	}

	//const normalize
	__forceinline  m_type getlengthTo1()const
	{
		return this->get_normalized();
	}

	//scale
	__forceinline  m_type operator*(float d)const
	{
		return this->scale_o(d);
	}
	//subtract
	__forceinline  m_type operator-(const m_type &vec)const
	{
		return this->sub(vec);
	}

	//subtract
	__forceinline  m_type operator-(const vec3f_t &vec)const
	{
		return this->sub(vec);
	}

	//corss product
	__forceinline  m_type operator|(const m_type &vec)const
	{
		return this->cross(vec);
	}

	//dot product
	__forceinline  float operator^(const m_type &vec)const
	{
		return this->dot(vec);
	}
	//addittion
	__forceinline m_type operator+(const m_type &vec)const
	{
		return this->add(vec);
	}
};

