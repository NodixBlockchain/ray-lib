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
	unsigned   int	* childs;
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
	unsigned int			padd_to_16;
}bounding_boxf_t;

typedef struct
{
	bounding_boxf_t			AABOX;
	vec3f_t					center;
	vec3f_t					size;
	planef_t				planes[6];
	float					*matrix;
	float					*norm_mat;
	float					*inv_matrix;
	float					*inv_norm_mat;
	materialf_t				material;
	bounding_boxf_t			CUBE;
}cubef_t;

typedef struct
{
	bounding_boxf_t				AABOX;
	vec3f_t						center;
	float						radius;
	float						sq_radius;

	float						*inv_matrix;
	float						*inv_norm_mat;
	float						*matrix;
	float						*norm_mat;
	
	float						padding[2];
	materialf_t					material;
}spheref_t;

typedef struct
{
	bounding_boxf_t	AABOX;
	vec3f_t			center;
	float			half_height;
	
	float			*inv_matrix;
	float			*inv_norm_mat;
	float			*matrix;
	float			*norm_mat;

	float			radiussq;
	float			radius;
	materialf_t		material;
	
}cylf_t;

typedef int		intersect_ray_boxf_func			( const vec3f_t r_origin, const vec3f_t r_inv_dir, float *parameters, float *t);
typedef			intersect_ray_boxf_func			*intersect_ray_boxf_func_ptr;

typedef	int		intersect_ray_boxesf_func		(const vec3f_t ray_o,const vec3f_t ray_dir,unsigned int ray_sign,const box_testf_t *boxes,unsigned int n_boxes,unsigned int cast,unsigned int *bound_ids,float *min_max);
typedef			intersect_ray_boxesf_func		*intersect_ray_boxesf_func_ptr;

#ifdef __cplusplus 
extern "C" {
#endif
/*
int		pointInCubef			(vec3f_t vec,cubef_t *cube,vec3f_t ormVec,float *uv_coord);
int		pointInSpheref			(float *vec,spheref_t *sphere,float *normVec);
float	point_in_cyl_test_c		(const float *pt1, const float *pt2, float lengthsq, float radius_sq, const float *testpt,float *normVec,float *uv_coord  );
void	getCubeUvCoords			(cubef_t *cube,int col,float *vec,float *uv_coord);
void	computeCubePlanesf		(cubef_t *c,float aX,float aY,float aZ,float *minv,float *maxv);
*/




int		 rayIntersectCube		(const cubef_t *cube,const rayf_t *ray,float *out_dist,vec3f_t outPos);
int		 intersectRayCyl		(const rayf_t *ray,const cylf_t *cyl,float *tVals,vec3f_t outPos);
int		 Sphere_intersect		(const rayf_t *ray,const spheref_t *sphere,float* t,float *t2,vec3f_t outPos);

void	 init_rayf				(rayf_t *ray,vec3f_t o,vec3f_t d);
void	 alloc_bboxf			(bounding_boxf_t *box);
void	 init_bboxf				(bounding_boxf_t *box,unsigned int id,vec3f_t min,vec3f_t max);
void	 init_cylf				(cylf_t	*cyl,vec3f_t base,float height,float radius,unsigned int id);

void	 remove_vec3			(unsigned int num);
float	*new_vec3				();


void	dllexport init_vec_funcsf	();
void	dllexport get_cube_norm			(unsigned int which,vec3f_t out);

extern intersect_ray_boxf_func_ptr		intersect_ray_boxf;
extern intersect_ray_boxesf_func_ptr	intersect_ray_boxesf;

#ifdef __cplusplus 
}
#endif




