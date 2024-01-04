typedef struct
{
 
    vec3i_t		origin;
    vec3f_t		direction;
    vec3i_t		inv_direction;
    int			sign[3];
	int			o_sign01;
}rayi_t;

typedef struct
{
	int norm[3];
	int det;
}planei_t;

typedef struct
{
	unsigned int			cast_id;
	vec3i_t					*parameters;
	vec3i_t					*o_params;
	int						*oo_params_1;
	int						*oo_params_2;
}bounding_boxi_t;

typedef struct
{
	int						center[3];
	int						size[3];
	float					vecs[6][4][3];
	planei_t				planes[6];
	bounding_boxi_t			AABOX;
}cubei_t;

typedef struct
{
int							center[3];
int							radius;
int							sq_radius;
bounding_boxi_t				AABOX;
}spherei_t;

typedef void	copy_vec3i_t (p_vec3i_t d_vec,p_vec3i_t s_vec);
typedef			copy_vec3i_t *p_copy_vec3i_t;

typedef int		intersect_ray_box_t			( cp_vec3i_t r_origin, cp_vec3i_t r_inv_dir, int *parameters_1, int *parameters_2, int *t);
typedef			intersect_ray_box_t			*p_intersect_ray_box_t;



#ifdef __cplusplus 
extern "C" {
#endif
void	copy_vec3i_c		(p_vec3i_t d_vec,p_vec3i_t s_vec);
void	alloc_bbox			(bounding_boxi_t *box);
void	init_bbox			(bounding_boxi_t *box,unsigned int id,vec3i_t min,vec3i_t max);
void	init_ray			(rayi_t *ray,vec3i_t o,vec3f_t d);
void	getsubS				(int *v1,int *v2,int *out);
int		dotS				(int *vec1,int *vec2);
void	crossproduct		(int *A, int *B,int *Out);
void	computeCubePlanes	(cubei_t *c,float a);
int		pointInCube			(int *vec,cubei_t *cube,float *normVec,float *uv_coord);
int		pointInSphere		(int *vec,spherei_t *sphere,float *normVec);
void	castRay				(const rayi_t *ray,unsigned char *pix,int cast,rayi_t *reflectRay);



dllexport void		init_geometry	();

extern  p_intersect_ray_box_t	intersect_ray_box;

extern void	dllexport init_vec_funcs();
#ifdef __cplusplus 
}
#endif