#include	"../libC/libc_math.h"
#include	"vec_types.h"
#include	"ray_int.h"
#include	"ray_float.h"
#define NULL 0x00000000

spherei_t		*Spheres;
unsigned int	n_spheres;
unsigned int	n_spheres_alloc;


cubei_t			*Cubes;
unsigned int	n_cubes;
unsigned int	n_cubes_alloc;
void			computeCubePlanes(cubei_t *c,float a);

p_copy_vec3i_t			copy_vec3i;
p_intersect_ray_box_t	intersect_ray_box;

extern int intersect_ray_box_mmx	( cp_vec3i_t r_origin, cp_vec3i_t r_inv_dir, int *parameters_1, int *parameters_2, int *t);

void init_vec_funcs()
{
	copy_vec3i			=	copy_vec3i_c;
	intersect_ray_box	=	intersect_ray_box_mmx;
}


void init_geometry()
{
	n_spheres_alloc		=	16;
	n_spheres			=	0;
	Spheres				=	(spherei_t *)aligned_alloc(n_spheres_alloc*sizeof(spherei_t));


	n_cubes_alloc		=	16;
	n_cubes				=	0;
	Cubes				=	(cubei_t *)aligned_alloc(n_cubes_alloc*sizeof(cubei_t));

}

void setCubeCenter (cubei_t	*cube,int cX,int cY,int cZ,int aX,int aY,int aZ)
{
	cube->center[0]	=cX;
	cube->center[1]	=cY;
	cube->center[2]	=cZ;
	computeCubePlanes	(cube,aX);

}

spherei_t *newSphere(int centerX,int centerY,int centerZ,int radius)
{
	vec3i_t					min,max;
	spherei_t			*newSphere;

	newSphere				=	&Spheres[n_spheres];

	alloc_bbox				(&newSphere->AABOX);

	newSphere->center[0]		=	centerX;
	newSphere->center[1]		=	centerY;
	newSphere->center[2]		=	centerZ;
	newSphere->radius			=	radius;
	newSphere->sq_radius		=	newSphere->radius*newSphere->radius;

	min[0]					=	newSphere->center[0]-newSphere->radius;
	min[1]					=	newSphere->center[1]-newSphere->radius;
	min[2]					=	newSphere->center[2]-newSphere->radius;

	max[0]					=	newSphere->center[0]+newSphere->radius;
	max[1]					=	newSphere->center[1]+newSphere->radius;
	max[2]					=	newSphere->center[2]+newSphere->radius;

	alloc_bbox			(&newSphere->AABOX);
	init_bbox			(&newSphere->AABOX,n_spheres+1,min,max);


	n_spheres++;

	return newSphere;

}

void setSphereCenter (spherei_t	*sphere,int cX,int cY,int cZ)
{
	vec3i_t		min,max;

	sphere->center[0]	=cX;
	sphere->center[1]	==cY;
	sphere->center[2]	=cZ;

	min[0]				=sphere->center[0]-sphere->radius;
	min[1]				=sphere->center[1]-sphere->radius;
	min[2]				=sphere->center[2]-sphere->radius;

	max[0]				=sphere->center[0]+sphere->radius;
	max[1]				=sphere->center[1]+sphere->radius;
	max[2]				=sphere->center[2]+sphere->radius;

	init_bbox			(&sphere->AABOX,1,min,max);

}

cubei_t *newCube(int centerX,int centerY,int centerZ,int sizeX,int sizeY,int sizeZ)
{
	cubei_t		*newCube;

	newCube					=	&Cubes[n_cubes];

	newCube->center[0]			=	centerX;
	newCube->center[1]			=	centerY;
	newCube->center[2]			=	centerZ;

	newCube->size[0]			=	100;
	newCube->size[1]			=	100;
	newCube->size[2]			=	100;

	alloc_bbox				(&newCube->AABOX);
	computeCubePlanes		(newCube,0);

	n_cubes++;

	return newCube;
}


void copy_vec3i_c		(p_vec3i_t d_vec,p_vec3i_t s_vec)
{
	d_vec[0]=s_vec[0];
	d_vec[1]=s_vec[1];
	d_vec[2]=s_vec[2];
}

void alloc_bbox		(bounding_boxi_t *box)
{
	box->parameters		=	(vec3i_t *)aligned_alloc(2*sizeof(vec3i_t));
	box->o_params		=	(vec3i_t *)aligned_alloc(2*sizeof(vec3i_t));
	box->oo_params_1	=	(int *)aligned_alloc(4*4*sizeof(int));
	box->oo_params_2	=	(int *)aligned_alloc(4*4*sizeof(int));
}


void init_bbox		(bounding_boxi_t *box,unsigned int id,vec3i_t min,vec3i_t max)
{
	copy_vec3i			(box->parameters[0],min);
	copy_vec3i			(box->parameters[1],max);

	
	box->oo_params_1[0+0]=box->parameters[0][0];
	box->oo_params_1[0+1]=box->parameters[1][0];
	box->oo_params_1[0+2]=box->parameters[0][1];
	box->oo_params_1[0+3]=box->parameters[1][1];
	box->oo_params_1[0+4]=box->parameters[0][2];
	box->oo_params_1[0+5]=box->parameters[1][2];

	box->oo_params_1[8+0]=box->parameters[1][0];
	box->oo_params_1[8+1]=box->parameters[0][0];
	box->oo_params_1[8+2]=box->parameters[0][1];
	box->oo_params_1[8+3]=box->parameters[1][1];
	box->oo_params_1[8+4]=box->parameters[0][2];
	box->oo_params_1[8+5]=box->parameters[1][2];

	box->oo_params_1[16+0]=box->parameters[0][0];
	box->oo_params_1[16+1]=box->parameters[1][0];
	box->oo_params_1[16+2]=box->parameters[1][1];
	box->oo_params_1[16+3]=box->parameters[0][1];
	box->oo_params_1[16+4]=box->parameters[0][2];
	box->oo_params_1[16+5]=box->parameters[1][2];


	box->oo_params_1[24+0]=box->parameters[1][0];
	box->oo_params_1[24+1]=box->parameters[0][0];
	box->oo_params_1[24+2]=box->parameters[1][1];
	box->oo_params_1[24+3]=box->parameters[0][1];
	box->oo_params_1[24+4]=box->parameters[0][2];
	box->oo_params_1[24+5]=box->parameters[1][2];



	box->o_params[0][0]=box->parameters[0][0];
	box->o_params[0][1]=box->parameters[1][0];
	box->o_params[0][2]=box->parameters[0][1];
	box->o_params[0][3]=box->parameters[1][1];

	box->o_params[1][0]=box->parameters[0][2];
	box->o_params[1][1]=box->parameters[1][2];
}

void init_ray(rayi_t *ray,vec3i_t o,vec3f_t d)
{

	  copy_vec3i	(ray->origin,o);

	  ray->direction[0]			=	d[0];
	  ray->direction[1]			=	d[1];
	  ray->direction[2]			=	d[2];

      ray->inv_direction[0]		=	PREC_MUL/ray->direction[0];
	  ray->inv_direction[1]		=	PREC_MUL/ray->direction[1];
	  ray->inv_direction[2]		=	PREC_MUL/ray->direction[2];

	  ray->sign[0]				= 	(ray->inv_direction[0] < 0);
      ray->sign[1]				= 	(ray->inv_direction[1] < 0);
      ray->sign[2]				= 	(ray->inv_direction[2] < 0);

	  ray->o_sign01				=	(ray->sign[0]|(ray->sign[1]<<1))*8;

}


void getsubS(int *v1,int *v2,int *out)
{
	out[0]=v1[0]-v2[0];
	out[1]=v1[1]-v2[1];
	out[2]=v1[2]-v2[2];
}

int dotS(int *vec1,int *vec2)
{
	return (vec1[0]*vec2[0]+vec1[1]*vec2[1]+vec1[2]*vec2[2]);
}


void crossproduct(int *A, int *B,int *Out)
{
    Out[0] = (A[1]*B[2])-(B[1]*A[2]);
    Out[1] = (B[0]*A[2])-(A[0]*B[2]);
    Out[2] = (A[0]*B[1])-(A[1]*B[0]);
    return;
}

void calculatePlane( float *v1,float *v2,float *v3,planei_t *plane )
{
	int d1[3],d2[3];

	d1[0]=v2[0]-v1[0];
	d1[1]=v2[1]-v1[1];
	d1[2]=v2[2]-v1[2];

	d2[0]=v3[0]-v1[0];
	d2[1]=v3[1]-v1[1];
	d2[2]=v3[2]-v1[2];

	
	crossproduct	(d1,d2,plane->norm);
	

	plane->det		= -(plane->norm[0]*v1[0]+plane->norm[1]*v1[1]+plane->norm[2]*v1[2]);

	// Get Shortened Names For The Vertices Of The Face

/*a = (By-Ay)(Cz-Az)-(Cy-Ay)(Bz-Az)
b = (Bz-Az)(Cx-Ax)-(Cz-Az)(Bx-Ax)
c = (Bx-Ax)(Cy-Ay)-(Cx-Ax)(By-Ay)
d = -(aAx+bAy+cAz).
*/
}

void computeCubePlanes(cubei_t *c,float a)
{
	int			n,i;
	float		Point[8][3];
	vec3i_t		min,max;
	float		minv[3],maxv[3];

/*	if(c->AABOX	!= NULL)
		delete c->AABOX;
*/
	Point[0][0]=-c->size[0];
	Point[0][1]=-c->size[1];
	Point[0][2]=c->size[2];

	Point[1][0]=c->size[0];
	Point[1][1]=-c->size[1];
	Point[1][2]=c->size[2];

	Point[2][0]=c->size[0];
	Point[2][1]=c->size[1];
	Point[2][2]=c->size[2];

	Point[3][0]=-c->size[0];
	Point[3][1]=c->size[1];
	Point[3][2]=c->size[2];

	Point[4][0]=-c->size[0];
	Point[4][1]=-c->size[1];
	Point[4][2]=-c->size[2];

	Point[5][0]=c->size[0];
	Point[5][1]=-c->size[1];
	Point[5][2]=-c->size[2];

	Point[6][0]=c->size[0];
	Point[6][1]=c->size[1];
	Point[6][2]=-c->size[2];

	Point[7][0]=-c->size[0];
	Point[7][1]=c->size[1];
	Point[7][2]=-c->size[2];


	copy_vec3(c->vecs[0][0],Point[0]);
	copy_vec3(c->vecs[0][1],Point[1]);
	copy_vec3(c->vecs[0][2],Point[2]);
	copy_vec3(c->vecs[0][3],Point[3]);

	copy_vec3(c->vecs[1][0],Point[1]);
	copy_vec3(c->vecs[1][1],Point[5]);
	copy_vec3(c->vecs[1][2],Point[6]);
	copy_vec3(c->vecs[1][3],Point[2]);
	 
	copy_vec3(c->vecs[2][0],Point[5]);
	copy_vec3(c->vecs[2][1],Point[4]);
	copy_vec3(c->vecs[2][2],Point[7]);
	copy_vec3(c->vecs[2][3],Point[6]);

	copy_vec3(c->vecs[3][0],Point[4]);
	copy_vec3(c->vecs[3][1],Point[0]);
	copy_vec3(c->vecs[3][2],Point[3]);
	copy_vec3(c->vecs[3][3],Point[7]);
	
	copy_vec3(c->vecs[4][0],Point[3]);
	copy_vec3(c->vecs[4][1],Point[2]);
	copy_vec3(c->vecs[4][2],Point[6]);
	copy_vec3(c->vecs[4][3],Point[7]);
 
	copy_vec3(c->vecs[5][0],Point[1]);
	copy_vec3(c->vecs[5][1],Point[0]);
	copy_vec3(c->vecs[5][2],Point[4]);
	copy_vec3(c->vecs[5][3],Point[5]);


	minv[0]=10000;
	minv[1]=10000;
	minv[2]=10000;

	maxv[0]=-10000;
	maxv[1]=-10000;
	maxv[2]=-10000;
 
	n=0;
	while(n<6)
	{
		
		for(i=0;i<4;i++){
			rotate_Z_3f(c->vecs[n][i],a);
			rotate_Y_3f(c->vecs[n][i],a);
			c->vecs[n][i][0]+=c->center[0];
			c->vecs[n][i][1]+=c->center[1];
			c->vecs[n][i][2]+=c->center[2];

			if(c->vecs[n][i][0]<minv[0])
				minv[0]=c->vecs[n][i][0];
			
			if(c->vecs[n][i][0]>maxv[0])
				maxv[0]=c->vecs[n][i][0];

			if(c->vecs[n][i][1]<minv[1])
				minv[1]=c->vecs[n][i][1];

			if(c->vecs[n][i][1]>maxv[1])
				maxv[1]=c->vecs[n][i][1];

			if(c->vecs[n][i][2]<minv[2])
				minv[2]=c->vecs[n][i][2];
			
			if(c->vecs[n][i][2]>maxv[2])
				maxv[2]=c->vecs[n][i][2];
		}

		calculatePlane(c->vecs[n][0],c->vecs[n][1],c->vecs[n][2],&c->planes[n]);


		n++;
	}

	min[0]		=	minv[0];
	min[1]		=	minv[1];
	min[2]		=	minv[2];

	max[0]		=	maxv[0];
	max[1]		=	maxv[1];
	max[2]		=	maxv[2];


	init_bbox	(&c->AABOX,1,min,max);
}



int pointInCube(int *vec,cubei_t *cube,float *normVec,float *uv_coord)
{
	int		n;
	int		col;
	int		min_dist,dist;
	
	

	min_dist	=	1000000000;
	n			=	0;
	

	while(n<6)
	{
		dist=(cube->planes[n].norm[0]*(vec[0]>>PREC_SHIFT)+cube->planes[n].norm[1]*(vec[1]>>PREC_SHIFT)+cube->planes[n].norm[2]*(vec[2]>>PREC_SHIFT)+cube->planes[n].det);
		if(dist>0)
			return 0;

		dist=abs(dist);

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
	normalize_vec3	(normVec);
	
	//uv_coord[0]		=0;
	//uv_coord[1]		=0;


	
	if(uv_coord!=NULL)
	{
		float AP[3];
		float CP[3];
		float AB[3];
		float AC[3];
		float N[3];
		float v_num;
		float u_num;
		float denom;
		float ABxAP[3];
		float CAxCP[3];
		float plane_point[3];
		plane_point[0]	=	(vec[0]>>PREC_SHIFT);
		plane_point[1]	=	(vec[1]>>PREC_SHIFT);
		plane_point[2]	=	(vec[2]>>PREC_SHIFT);
		
		AP[0]		=	plane_point[0]-cube->vecs[col][0][0];
		AP[1]		=	plane_point[1]-cube->vecs[col][0][1];
		AP[2]		=	plane_point[2]-cube->vecs[col][0][2];

		CP[0]		=	plane_point[0]-cube->vecs[col][3][0];
		CP[1]		=	plane_point[1]-cube->vecs[col][3][1];
		CP[2]		=	plane_point[2]-cube->vecs[col][3][2];

		AB[0]		=	(cube->vecs[col][1][0]-cube->vecs[col][0][0]);
		AB[1]		=	(cube->vecs[col][1][1]-cube->vecs[col][0][1]);
		AB[2]		=	(cube->vecs[col][1][2]-cube->vecs[col][0][2]);

		AC[0]		=	(cube->vecs[col][3][0]-cube->vecs[col][0][0]);
		AC[1]		=	(cube->vecs[col][3][1]-cube->vecs[col][0][1]);
		AC[2]		=	(cube->vecs[col][3][2]-cube->vecs[col][0][2]);


		cross_vec3(AB, AC,N);

		cross_vec3(AB, AP,ABxAP);
		v_num	=  dot_vec3(N, ABxAP);

		cross_vec3(CP, AC,CAxCP);
		u_num = dot_vec3(N, CAxCP);

		denom = dot_vec3(N, N); 

		

		uv_coord[0]		=	u_num/denom;
		uv_coord[1]		=	v_num/denom;
	}
	
	
	return 1;
}



int pointInSphere(int *vec,spherei_t *sphere,float *normVec)
{
	int	 l;
	int	 distVec[3];
	int  r_vec[3];

	r_vec[0]=vec[0]>>PREC_SHIFT;
	r_vec[1]=vec[1]>>PREC_SHIFT;
	r_vec[2]=vec[2]>>PREC_SHIFT;

	getsubS(r_vec,sphere->center,distVec);
	l=distVec[0]*distVec[0]+distVec[1]*distVec[1]+distVec[2]*distVec[2];

	if(l>sphere->sq_radius)
		return 0;

	normVec[0]	=distVec[0];
	normVec[1]	=distVec[1];
	normVec[2]	=distVec[2];

	normalize_vec3	(normVec);
	return 1;
}

int intersect_ray_box_c	(cp_vec3i_t r_origin,cp_vec3i_t r_inv_dir,int *parameters_1,int *parameters_2,int *t)
{
return 0;
}
/*
int intersect_ray_box_c	(p_vec3i_t r_sign,p_vec3i_t r_origin,p_vec3i_t r_inv_dir,vec3i_t *parameters,int *t0,int *t1)
{
  int tmin, tmax, tymin, tymax, tzmin, tzmax;

  tmin = ((parameters[r_sign[0]][0] - r_origin[0]) * r_inv_dir[0])>>PREC_SHIFT;
  tmax = ((parameters[1-r_sign[0]][0] - r_origin[0]) * r_inv_dir[0])>>PREC_SHIFT;


  tymin = ((parameters[r_sign[1]][1] - r_origin[1]) * r_inv_dir[1])>>PREC_SHIFT;
  tymax = ((parameters[1-r_sign[1]][1] - r_origin[1]) * r_inv_dir[1])>>PREC_SHIFT;

  if ( (tmin > tymax) || (tymin > tmax) ) 
    return false;

  if (tymin > tmin)
    tmin = tymin;

  if (tymax < tmax)
    tmax = tymax;

  tzmin = ((parameters[r_sign[2]][2] - r_origin[2]) * r_inv_dir[2])>>PREC_SHIFT;
  tzmax = ((parameters[1-r_sign[2]][2] - r_origin[2]) * r_inv_dir[2])>>PREC_SHIFT;

  if ( (tmin > tzmax) || (tzmin > tmax) ) 
    return false;
  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;

  *t0=tmin;
  *t1=tmax;

  return true;
}
*/

