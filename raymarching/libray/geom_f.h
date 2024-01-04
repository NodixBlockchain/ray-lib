

template  <typename item_type>
class itemList
{
public:
	itemList()
	{
		num_item_alloc	=	32;
		num_item		=	0;
		listPtr			=	(item_type *)aligned_alloc(sizeof(item_type)*num_item_alloc);
	}
	int addItem(item_type *item,unsigned int id)
	{
		listPtr[num_item]=item;
		num_item++;
	}
	item_type *newItem(unsigned int id)
	{
		item_type	*ptr;

		ptr		=	&listPtr[num_item];
		ptr->matrix			=	(float *)aligned_alloc(sizeof(mat3x3f_t));
		ptr->inv_matrix		=	(float *)aligned_alloc(sizeof(mat3x3f_t));
		ptr->norm_mat		=	(float *)aligned_alloc(sizeof(mat3x3f_t));
		ptr->inv_norm_mat	=	(float *)aligned_alloc(sizeof(mat3x3f_t));


		identity_mat3x3			(ptr->matrix);
		identity_mat3x3			(ptr->norm_mat);
		num_item++;

		return ptr;
	}
	item_type	*getItemAt(unsigned int n)
	{
		if(n>=num_item)return NULL;
		return &listPtr[n];
	}

	item_type	*getItem(unsigned int id)
	{
		int n;
		n=0;
		while(n<num_item)
		{
			if(listPtr[n].AABOX.cast_id==id)
				return &listPtr[n];
			n++;
		}
		return NULL;
	}
	item_type		*listPtr;
	unsigned int	num_item;
	unsigned int	num_item_alloc;
};

typedef itemList<spheref_t>		SpherefList_t;
typedef itemList<cylf_t>		CylfList_t;
typedef itemList<cubef_t>		CubefList_t;

typedef itemList<spherei_t>		SphereList_t;
//typedef itemList<cyli_t>		CylList_t;
typedef itemList<cubei_t>		CubeList_t;

typedef struct
{
	unsigned int sX;
	unsigned int eX;
	unsigned int sY;
	unsigned int eY;
	unsigned long TID;
	void		*TH;
	rayi_t		*reflectRay;
	rayf_t		*reflectRayf;
	float		*trans_ray_origin;
	float		*trans_ray_direction;
	float		*tmpNorm;
	rayf_t		*new_ray;
	unsigned int volatile threadSync;
	void		*EventStart;
	void		*EventFinish;
}thread_info_t;


class dllexport CScene
{	
public:
	CScene();
	void Resize(unsigned int w,unsigned int h);

	spheref_t *			getSpheref				(unsigned int id);
	spheref_t *			newSpheref				(float centerX,float centerY,float centerZ,float radius);
	spheref_t *			newHalof				(float centerX,float centerY,float centerZ,float radius);


	cubef_t *			newCubef				(float centerX,float centerY,float centerZ,float sizeX,float sizeY,float sizeZ);
	cylf_t	*			newCylinderf			(float centerX,float centerY,float centerZ,float half_height,float radius);

	void 				setSphereMaterialf		(spheref_t *sphere,int tex_idx,unsigned int tid,float refract,float reflect);
	void 				setSphereCenter_matf	(spheref_t	*sphere,float cX,float cY,float cZ,mat3x3f_t mat);
	void				setSphereCenterf		(spheref_t	*sphere,float cX,float cY,float cZ);
	void				setSphereNormalMap		(spheref_t *sphere,int tex_idx,unsigned int tid);


	void 				setCubeMaterialf		(cubef_t *cube,int tex_idx,unsigned int tid,float refract,float reflect);
	void 				setCubeCenterf			(cubef_t	*newCube,float cX,float cY,float cZ,float aX,float aY,float aZ);
	void				setCubeCenter_matf		(cubef_t	*newCube,float cX,float cY,float cZ,mat3x3f_t mat);
	void				setCubeNormalMap		(cubef_t *cube,int tex_idx,unsigned int tid);	
	
	void 				setCylMaterialf			(cylf_t *cyl,int tex_idx,unsigned int tid,float refract,float reflect);
	void				setCylCenterf			(cylf_t	*cyl,float cX,float cY,float cZ,float aX,float aY,float aZ);
	void				setCylCenter_matf		(cylf_t	*cyl,float cX,float cY,float cZ,mat3x3f_t m);
	void				setCylNormalMap			(cylf_t *cyl,int tex_idx,unsigned int tid);
	void				compute_camera			();
	int	 				getGeomData				();
	void				getCameraInvMat_ogl		(float *m);
	void				getCameraInvNormMat_ogl	(float *m);
	int					castRayToLightf			( const rayf_t *ray	,unsigned int cast);
	void				castRayf				( const rayf_t *ray	,unsigned char *pix,int cast,thread_info_t *thread);
	void				doCasts					(rayf_t *new_ray_ptr,thread_info_t *thread,int sX,int sY,int eX,int eY);
	

	void				add_boxf				(bounding_boxf_t	*box);

	void				read_texture			(char *file,int n);

	vec3f_t				camScale,camAngle;
	vec3f_t				lightPos;

	rayf_t				*raysf;
	rayi_t				*rays;

	image_t				textures[32];
	

	double				start_time;
	double				cur_time;

	cpu_infos_t			cpu_infos;
	

	SpherefList_t		*SpherefList;
	SpherefList_t		*HalofList;
	CylfList_t			*CylfList;
	CubefList_t			*CubefList;

	int					frame;
	image_t				targetImage;

	float				*boxData;
	box_testf_t			*box_testf;
	unsigned int		n_boxf;

	CMat3 <float,cmat_funcs> matrice;
	CMat3 <float,cmat_funcs> matrice_normales;

	float				*inv_matrice;
	float				*inv_matrice_normales;
	float				*camPos;

};

