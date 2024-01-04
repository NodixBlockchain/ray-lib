
#include	"../libC/libc_math.h"
#include	"../libC/libc_file.h"
#include	"../libC/cpu_infos.h"
#include	"string.h"
#include	"vec_types.h"
#include	"ray_float.h"
#include	"ray_int.h"
#include	"mat3.h"
#include	"geom_f.h"


const unsigned int cubemap_right= 0;
const unsigned int cubemap_left	= 1;
const unsigned int cubemap_up	= 2;
const unsigned int cubemap_down	= 3;
const unsigned int cubemap_forward= 4;
const unsigned int cubemap_backward=5;



void *operator new (size_t size)
{
	return aligned_alloc(size);
}


void getTexPix (image_t *texture,float *uv_coord,unsigned char *pix)
{
	int texX,texY;

	if(texture->pixels==NULL)return;

	texX=uv_coord[0]*512.0f;
	texY=uv_coord[1]*512.0f;

	if(texX<0)return;
	if(texY<0)return;

	if(texX>512)return;
	if(texY>512)return;

	pix[0]=texture->pixels[(texY*512+texX)*3+2];
	pix[1]=texture->pixels[(texY*512+texX)*3+1];
	pix[2]=texture->pixels[(texY*512+texX)*3+0];
}

void getTexPix_32 (const image_t *texture,float *uv_coord,int *pix)
{
	int texX;

	if(texture->pixels==NULL)return;
		
	texX=((unsigned int)(uv_coord[1]*texture->h))*(texture->w*3)+((unsigned int)(uv_coord[0]*texture->w))*(3);

	if(texX<0)texX=0;

	pix[0]=texture->pixels[texX+2];
	pix[1]=texture->pixels[texX+1];
	pix[2]=texture->pixels[texX+0];
	


}

void getTexPix_32uc (const image_t *texture,float *uv_coord,unsigned char *pix)
{
	int texX;

	if(texture->pixels==NULL)return;

	
	texX=((unsigned int)(uv_coord[1]*texture->h))*(texture->w*3)+((unsigned int)(uv_coord[0]*texture->w))*(3);
	pix[0]=texture->pixels[texX+2];
	pix[1]=texture->pixels[texX+1];
	pix[2]=texture->pixels[texX+0];
	



}

int readEnvCubemapPixf(const rayf_t *myRay,float *uv)
{
 

	if ((libc_fabsf(myRay->direction[0]) >= libc_fabsf(myRay->direction[1])) && (libc_fabsf(myRay->direction[0]) >= libc_fabsf(myRay->direction[2])))
    {
        if (myRay->direction[0] > 0.0f)
        {
            //currentColor = cm->texture + cubemap_right * cm->sizeX * cm->sizeY;
            uv[0]=	1.0f - (myRay->direction[2] / myRay->direction[0]+ 1.0f) * 0.5f;
			uv[1]=	(myRay->direction[1] / myRay->direction[0]+ 1.0f) * 0.5f;
			return cubemap_right;
            // cm->sizeX, cm->sizeY
        }
        else if (myRay->direction[0] < 0.0f)
        {
            //currentColor = cm->texture + cubemap_left * cm->sizeX * cm->sizeY;
			uv[0]= 1.0f - (myRay->direction[2] / myRay->direction[0]+ 1.0f) * 0.5f;
			uv[1]= 1.0f - (myRay->direction[1] / myRay->direction[0] + 1.0f) * 0.5f;	
			return cubemap_left;
            //cm->sizeX, cm->sizeY);
        }
    }
    else if ((libc_fabsf(myRay->direction[1]) >= libc_fabsf(myRay->direction[0])) && (libc_fabsf(myRay->direction[1]) >= libc_fabsf(myRay->direction[2])))
    {
        if (myRay->direction[1] > 0.0f)
        {
			//currentColor = cm->texture + cubemap_up * cm->sizeX * cm->sizeY;
			uv[0]= (myRay->direction[0] / myRay->direction[1] + 1.0f) * 0.5f;
			uv[1]= 1.0f - (myRay->direction[2]/ myRay->direction[1] + 1.0f) * 0.5f;
			return cubemap_up;
        }
        else if (myRay->direction[1] < 0.0f)
        {
			//currentColor = cm->texture + cubemap_down * cm->sizeX * cm->sizeY;
			uv[0]= 1.0f - (myRay->direction[0] / myRay->direction[1] + 1.0f) * 0.5f;
			uv[1]= (myRay->direction[2]/myRay->direction[1] + 1.0f) * 0.5f;
			return cubemap_down;
        }
    }
    else if ((libc_fabsf(myRay->direction[2]) >= libc_fabsf(myRay->direction[0])) && (libc_fabsf(myRay->direction[2]) >= libc_fabsf(myRay->direction[1])))
    {
        if (myRay->direction[2] > 0.0f)
        {
			//currentColor = cm->texture + cubemap_forward * cm->sizeX * cm->sizeY;
			uv[0]= (myRay->direction[0] / myRay->direction[2] + 1.0f) * 0.5f;
			uv[1]= (myRay->direction[1]/myRay->direction[2] + 1.0f) * 0.5f;
			return cubemap_forward;

        }
        else if (myRay->direction[2] < 0.0f)
        {
            //currentColor = cm->texture + cubemap_backward * cm->sizeX * cm->sizeY;
			uv[0]= (myRay->direction[0] / myRay->direction[2] + 1.0f) * 0.5f;
			uv[1]= 1.0f - (myRay->direction[1] /myRay->direction[2]+1) * 0.5f;
			return cubemap_backward;
        }
    }

    return -1;
}



void alloc_bboxf		(bounding_boxf_t *box)
{
	if(box==NULL)return;
	box->parameters		=	(vec3f_t *)aligned_alloc(2*sizeof(vec3f_t));
	box->_parameters	=	(float *)aligned_alloc	(8*4*sizeof(vec3f_t));
}


void init_cube(cubef_t *c,float *minv,float *maxv)
{
	DECLARE_ALIGNED_VEC3_ARRAY(Point,8);
	DECLARE_ALIGNED_VEC3_ARRAY(c_Point,8);
	
	DECLARE_ALIGNED_VEC3(Min);
	DECLARE_ALIGNED_VEC3(Max);
	int			n;
	int			box_ids[24];





	set_vec4i(&box_ids[0*4],0,1,2,3);
	set_vec4i(&box_ids[1*4],1,5,6,2);
	set_vec4i(&box_ids[2*4],5,4,7,6);
	set_vec4i(&box_ids[3*4],4,0,3,7);
	set_vec4i(&box_ids[4*4],3,2,6,7);
	set_vec4i(&box_ids[5*4],1,0,4,5);
	

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


	minv[0]=10000;
	minv[1]=10000;
	minv[2]=10000;

	maxv[0]=-10000;
	maxv[1]=-10000;
	maxv[2]=-10000;

	

 

	n=0;
	while(n<8)
	{
		mul_vec3x3_o (Point[n],c->matrix,c_Point[n]);

		c_Point[n][0]+=c->center[0];
		c_Point[n][1]+=c->center[1];
		c_Point[n][2]+=c->center[2];

		if(c_Point[n][0]<minv[0])
			minv[0]=c_Point[n][0];
			
		if(c_Point[n][0]>maxv[0])
			maxv[0]=c_Point[n][0];

		if(c_Point[n][1]<minv[1])
			minv[1]=c_Point[n][1];

		if(c_Point[n][1]>maxv[1])
			maxv[1]=c_Point[n][1];

		if(c_Point[n][2]<minv[2])
			minv[2]=c_Point[n][2];
			
		if(c_Point[n][2]>maxv[2])
			maxv[2]=c_Point[n][2];
		n++;
	}
	
	Min[0]=-c->size[0];
	Min[1]=-c->size[1];
	Min[2]=-c->size[2];

	Max[0]=+c->size[0];
	Max[1]=+c->size[1];
	Max[2]=+c->size[2];

	
	init_bboxf(&c->CUBE,0xFF,Min,Max);
	
	n=0;
	while(n<6)
	{
		DECLARE_ALIGNED_VEC3(d1);
		DECLARE_ALIGNED_VEC3(d2);

		sub_vec3			(c_Point[box_ids[n*4+1]],c_Point[box_ids[n*4+0]],d1);
		sub_vec3			(c_Point[box_ids[n*4+2]],c_Point[box_ids[n*4+0]],d2);
		cross_vec3			(d1,d2,c->planes[n].norm);
		normalize_vec3		(c->planes[n].norm);

		c->planes[n].det	= -dot_vec3(c->planes[n].norm,c_Point[box_ids[n*4+0]]);

		n++;
	}

}
void init_cylf		(cylf_t	*cyl,vec3f_t center,float half_height,float radius,unsigned int id)
{
	DECLARE_ALIGNED_VEC3		(minf);
	DECLARE_ALIGNED_VEC3		(maxf);
	DECLARE_ALIGNED_VEC3		(dvec);
	DECLARE_ALIGNED_VEC3		(pt1);
	DECLARE_ALIGNED_VEC3		(pt2);
	DECLARE_ALIGNED_VEC3		(k);

	
	cyl->half_height	=	half_height;

	cyl->radius			=	radius;
	cyl->radiussq		=	radius*radius;
	
	copy_vec3			(cyl->center,center);
	


		

	set_vec3	(pt1	, 0	, -half_height	,0);
	set_vec3	(pt2	, 0	,  half_height	,0);
	
	mul_vec3x3	(pt1,cyl->matrix);
	mul_vec3x3	(pt2,cyl->matrix);

	sub_vec3	(pt2,pt1,dvec);

	dvec[0]	=dvec[0]*dvec[0];
	dvec[1]	=dvec[1]*dvec[1];
	dvec[2]	=dvec[2]*dvec[2];

	k[0]	=libc_sqrtf((dvec[1]+dvec[2])/(dvec[0]+dvec[1]+dvec[2]));
	k[1]	=libc_sqrtf((dvec[0]+dvec[2])/(dvec[0]+dvec[1]+dvec[2]));
	k[2]	=libc_sqrtf((dvec[0]+dvec[1])/(dvec[0]+dvec[1]+dvec[2]));

	scale_vec3	(k,radius);

	minf[0]	=	min(pt1[0],pt2[0]);
	minf[1]	=	min(pt1[1],pt2[1]);
	minf[2]	=	min(pt1[2],pt2[2]);

	maxf[0]	=	max(pt1[0],pt2[0]);
	maxf[1]	=	max(pt1[1],pt2[1]);
	maxf[2]	=	max(pt1[2],pt2[2]);

	sub_vec3	(minf,k,minf);
	add_vec3	(maxf,k,maxf);

	add_vec3	(maxf,cyl->center,maxf);
	add_vec3	(minf,cyl->center,minf);

	

	init_bboxf	(&cyl->AABOX,id,minf,maxf);
}
unsigned int create_id_var(unsigned int type,unsigned int idx)
{
	return (((type&0xFF)<<24)|(idx&0x00FFFFFF));
}

unsigned int get_id_var_type(unsigned int id_var)
{
	return ((id_var>>24)&0xFF);
}

unsigned int get_id_var_idx(unsigned int id_var)
{
	return (id_var&0x00FFFFFF);
}

CScene::CScene()
{
	inv_matrice			= (float*)aligned_alloc(sizeof(mat3x3f_t));
	inv_matrice_normales= (float*)aligned_alloc(sizeof(mat3x3f_t));
	camPos				= (float*)aligned_alloc(sizeof(vec3f_t));

	getCPUInfos			(&cpu_infos);



	SpherefList			=	new SpherefList_t();
	CubefList			=	new CubefList_t();
	CylfList			=	new CylfList_t();
	HalofList			=	new SpherefList_t();

	lightPos[0]			=	-100;
	lightPos[1]			=	-100;
	lightPos[2]			=	0;

	camPos[0]			=	0;
	camPos[1]			=	300;
	camPos[2]			=	-800;

	
	camAngle[0]			=	PIf/2.0;
	camAngle[1]			=	PIf;
	camAngle[2]			=	0;


	camScale[0]			=	1.0;
	camScale[1]			=	1.0;
	camScale[2]			=	1.0;

	start_time			=	0;
	cur_time			=	0;


	n_boxf				=	0;
	box_testf			=	(box_testf_t	*)aligned_alloc(128*sizeof(box_testf_t));

	boxData				=	(float	*)aligned_alloc(2048*4*sizeof(float));
	

	

	rays				=	NULL;
	raysf				=	NULL;
	targetImage.pixels=	NULL;



}

void CScene::Resize(unsigned int w,unsigned int h)
{
	__m128					align;
	vec3f_t					of;
	vec3f_t					d;
	int						n;
	unsigned int			pX,pY;
	vec3i_t					o;

	if(rays)aligned_free(rays);
	if(raysf)aligned_free(raysf);
	if(targetImage.pixels)aligned_free(targetImage.pixels);

	align					=	_mm_setzero_ps();
	targetImage.w			=	w;
	targetImage.h			=	h;
	targetImage.pixels		=	(unsigned char	*)aligned_alloc(w*h*3);

	rays					=	(rayi_t *)aligned_alloc(w*h*sizeof(rayi_t));
	raysf					=	(rayf_t *)aligned_alloc(w*h*sizeof(rayf_t));

	n						=	0;
	pY						=	0;

	while(pY<h)
	{
		pX		=	0;
		while(pX<w)
		{
			float ox,oy,oz;

			ox			=	w/2.0f;
			oy			=	h/2.0f;
			oz			=	-800.0f;
			
			o[0]		=	pX;
			o[1]		=	pY;
			o[2]		=	0;
			
			of[0]		=	((pX-ox)/w)*100.0f;
			of[1]		=	((pY-oy)/h)*100.0f;
			of[2]		=	0;
			
			d[0]		=	(pX	-ox);
			d[1]		=	(pY	-oy);
			d[2]		=	(0.0f-oz);

			normalize_vec3	(d);
			init_ray		(&rays[n],o,d);
			init_rayf		(&raysf[n],of,d);

			pX++;
			n++;
		}
		pY++;
	}
}

spheref_t *CScene::getSpheref(unsigned int id)
{
	return SpherefList->getItem(id);

}


void CScene::read_texture(char *file,int n)
{
	void			*v_file_data;
	unsigned char	*file_data;



	if(!libc_readfile	(file,&v_file_data))
	{
		textures[n].w=0;
		textures[n].h=0;
		textures[n].size=0;
		textures[n].pixels=NULL;
		return;
	}

	file_data			=(unsigned char	*)v_file_data;
	textures[n].w		=512;
	textures[n].h		=512;
	textures[n].size	=512*512*3;
	textures[n].pixels	=(unsigned char *)aligned_alloc(textures[n].size);

	memcpy(textures[n].pixels,&file_data[54],textures[n].size);
	aligned_free		(file_data);



}



spheref_t *CScene::newSpheref(float centerX,float centerY,float centerZ,float radius)
{
	vec3f_t		minf,maxf;
	spheref_t	*newSphere;
	unsigned int id_var;
	
	id_var					=	create_id_var(1,SpherefList->num_item);
	newSphere				=	SpherefList->newItem(0);



			
	newSphere->center[0]	=	centerX;
	newSphere->center[1]	=	centerY;
	newSphere->center[2]	=	centerZ;
	newSphere->radius		=	radius;
	newSphere->sq_radius	=	newSphere->radius*newSphere->radius;

	minf[0]					=	newSphere->center[0]-newSphere->radius;
	minf[1]					=	newSphere->center[1]-newSphere->radius;
	minf[2]					=	newSphere->center[2]-newSphere->radius;

	maxf[0]					=	newSphere->center[0]+newSphere->radius;
	maxf[1]					=	newSphere->center[1]+newSphere->radius;
	maxf[2]					=	newSphere->center[2]+newSphere->radius;

	

	alloc_bboxf				(&newSphere->AABOX);
	init_bboxf				(&newSphere->AABOX,id_var,minf,maxf);

	newSphere->material.normals = NULL;
	newSphere->material.nid		=	0;

	

	return newSphere;
}

cubef_t *CScene::newCubef(float centerX,float centerY,float centerZ,float sizeX,float sizeY,float sizeZ)
{
	DECLARE_ALIGNED_VEC3(minv);
	DECLARE_ALIGNED_VEC3(maxv);
	cubef_t		*newCube;
	unsigned int id_var;

	id_var						=	create_id_var(2,CubefList->num_item);
	newCube						=	CubefList->newItem(0);
	
	newCube->center[0]			=	centerX;
	newCube->center[1]			=	centerY;
	newCube->center[2]			=	centerZ;

	newCube->size[0]			=	sizeX;
	newCube->size[1]			=	sizeY;
	newCube->size[2]			=	sizeZ;



	alloc_bboxf				(&newCube->AABOX);
	alloc_bboxf				(&newCube->CUBE);

	init_cube				(newCube,minv,maxv);
	init_bboxf				(&newCube->AABOX,id_var,minv,maxv);

	newCube->material.normals = NULL;
	newCube->material.nid	  =0;

	
	
	return newCube;
}

cylf_t *CScene::newCylinderf(float centerX,float centerY,float centerZ,float half_height,float radius)
{
	DECLARE_ALIGNED_VEC3(center);
	cylf_t				*newCyl;
	unsigned int		id_var;

	id_var				=	create_id_var(3,CylfList->num_item);
	newCyl				=	CylfList->newItem(0);

	center[0]				=	centerX;
	center[1]				=	centerY;
	center[2]				=	centerZ;

	
	alloc_bboxf			(&newCyl->AABOX);
	init_cylf			(newCyl,center,half_height,radius,id_var);

	newCyl->material.normals = NULL;
	newCyl->material.nid	  =0;
	
	
	return newCyl;
}
spheref_t *CScene::newHalof(float centerX,float centerY,float centerZ,float radius)
{
	DECLARE_ALIGNED_VEC3(minf);
	DECLARE_ALIGNED_VEC3(maxf);
	spheref_t	*newSphere;
	unsigned int id_var;
	
	id_var					=	create_id_var(4,HalofList->num_item);
	newSphere				=	HalofList->newItem(0);

		
	newSphere->center[0]	=	centerX;
	newSphere->center[1]	=	centerY;
	newSphere->center[2]	=	centerZ;
	newSphere->radius		=	radius;
	newSphere->sq_radius	=	newSphere->radius*newSphere->radius;

	minf[0]					=	newSphere->center[0]-newSphere->radius;
	minf[1]					=	newSphere->center[1]-newSphere->radius;
	minf[2]					=	newSphere->center[2]-newSphere->radius;

	maxf[0]					=	newSphere->center[0]+newSphere->radius;
	maxf[1]					=	newSphere->center[1]+newSphere->radius;
	maxf[2]					=	newSphere->center[2]+newSphere->radius;

	alloc_bboxf				(&newSphere->AABOX);
	init_bboxf				(&newSphere->AABOX,id_var,minf,maxf);

	newSphere->material.normals = NULL;
	newSphere->material.nid		=0;

	return newSphere;
}



void CScene::setCubeMaterialf(cubef_t *cube,int tex_idx,unsigned int tid,float refract,float reflect)
{
	if(tid>0)
	{
		cube->material.texture				=	&textures[tex_idx];
		cube->material.tid					=	tid;
	}
	else
	{
		cube->material.texture				=	NULL;
		cube->material.tid					=	0;

	}
	cube->material.refract				=	refract;
	cube->material.reflect				=	reflect;
	cube->material.refract_density		=	1.4;

}

void CScene::setCubeNormalMap(cubef_t *cube,int tex_idx,unsigned int tid)
{
	if(tid>0)
	{
		cube->material.normals				=	&textures[tex_idx];
		cube->material.nid					=	tid;
	}
	else
	{
		cube->material.normals				=	NULL;
		cube->material.nid					=	0;

	}

}



void CScene::setSphereMaterialf(spheref_t *sphere,int tex_idx,unsigned int tid,float refract,float reflect)
{
	sphere->material.texture				=	&textures[tex_idx];
	sphere->material.tid					=	tid;

	sphere->material.refract				=	refract;
	sphere->material.reflect				=	reflect;
	sphere->material.refract_density		=	1.8;

}

void CScene::setSphereNormalMap(spheref_t *sphere,int tex_idx,unsigned int tid)
{
	if(tid>0)
	{
		sphere->material.normals				=	&textures[tex_idx];
		sphere->material.nid					=	tid;
	}
	else
	{
		sphere->material.normals				=	NULL;
		sphere->material.nid					=	0;

	}
}

void CScene::setCylMaterialf(cylf_t *cyl,int tex_idx,unsigned int tid,float refract,float reflect)
{
	cyl->material.texture		=	&textures[tex_idx];

	cyl->material.tid					=	tid;
	cyl->material.refract				=	refract;
	cyl->material.reflect				=	reflect;
	cyl->material.refract_density		=	1.8;

}


void CScene::setCylNormalMap(cylf_t *cyl,int tex_idx,unsigned int nid)
{
	if(nid>0)
	{
		cyl->material.normals				=	&textures[tex_idx];
		cyl->material.nid					=	nid;
	}
	else
	{
		cyl->material.normals				=	NULL;
		cyl->material.nid					=	0;

	}
}

void CScene::setSphereCenter_matf (spheref_t	*sphere,float cX,float cY,float cZ,mat3x3f_t m)
{
	DECLARE_ALIGNED_VEC3(minf);
	DECLARE_ALIGNED_VEC3(maxf);
	CMat3 <float,cmat_funcs>   mat,norm_mat;

	memcpy(mat.m,m,sizeof(mat3x3f_t));

	norm_mat				=mat.getNormalMat();
		
	copy_mat3x3				(sphere->matrix	,mat.m);
	copy_mat3x3				(sphere->norm_mat	,norm_mat.m);

	sphere->center[0]	=cX;
	sphere->center[1]	=cY;
	sphere->center[2]	=cZ;

	minf[0]				=sphere->center[0]-sphere->radius;
	minf[1]				=sphere->center[1]-sphere->radius;
	minf[2]				=sphere->center[2]-sphere->radius;

	maxf[0]				=sphere->center[0]+sphere->radius;
	maxf[1]				=sphere->center[1]+sphere->radius;
	maxf[2]				=sphere->center[2]+sphere->radius;

	init_bboxf			(&sphere->AABOX,sphere->AABOX.cast_id,minf,maxf);

}


void CScene::setSphereCenterf (spheref_t	*sphere,float cX,float cY,float cZ)
{
	DECLARE_ALIGNED_VEC3(minf);
	DECLARE_ALIGNED_VEC3(maxf);
	CMat3 <float,cmat_funcs>   mat,norm_mat;

	mat.identity();
	norm_mat				=mat.getNormalMat();
		
	copy_mat3x3				(sphere->matrix	,mat.m);
	copy_mat3x3				(sphere->norm_mat	,norm_mat.m);

	sphere->center[0]	=cX;
	sphere->center[1]	=cY;
	sphere->center[2]	=cZ;

	minf[0]				=sphere->center[0]-sphere->radius;
	minf[1]				=sphere->center[1]-sphere->radius;
	minf[2]				=sphere->center[2]-sphere->radius;

	maxf[0]				=sphere->center[0]+sphere->radius;
	maxf[1]				=sphere->center[1]+sphere->radius;
	maxf[2]				=sphere->center[2]+sphere->radius;

	init_bboxf			(&sphere->AABOX,sphere->AABOX.cast_id,minf,maxf);

}


void CScene::setCubeCenter_matf (cubef_t	*newCube,float cX,float cY,float cZ,mat3x3f_t m)
{
	DECLARE_ALIGNED_VEC3(minf);
	DECLARE_ALIGNED_VEC3(maxf);
	CMat3 <float,cmat_funcs>   mat,norm_mat;



	memcpy(mat.m,m,sizeof(mat3x3f_t));

	norm_mat				=mat.getNormalMat();
		
	copy_mat3x3				(newCube->matrix	,mat.m);
	copy_mat3x3				(newCube->norm_mat	,norm_mat.m);

	newCube->center[0]	=cX;
	newCube->center[1]	=cY;
	newCube->center[2]	=cZ;

	init_cube				(newCube,minf,maxf);
	init_bboxf				(&newCube->AABOX,newCube->AABOX.cast_id,minf,maxf);
}

void CScene::setCubeCenterf (cubef_t	*newCube,float cX,float cY,float cZ,float aX,float aY,float aZ)
{
	DECLARE_ALIGNED_VEC3(minf);
	DECLARE_ALIGNED_VEC3(maxf);
	CMat3 <float,cmat_funcs>   mat,norm_mat;

	newCube->center[0]	=cX;
	newCube->center[1]	=cY;
	newCube->center[2]	=cZ;

	mat.rotateEuler		(aX,aY,aZ);
	norm_mat=mat.getNormalMat();
		
	copy_mat3x3				(newCube->matrix	,mat.m);
	copy_mat3x3				(newCube->norm_mat,norm_mat.m);

	init_cube				(newCube,minf,maxf);
	init_bboxf				(&newCube->AABOX,newCube->AABOX.cast_id,minf,maxf);
}


void CScene::setCylCenter_matf (cylf_t	*cyl,float cX,float cY,float cZ,mat3x3f_t m)
{
	DECLARE_ALIGNED_VEC3(minf);
	DECLARE_ALIGNED_VEC3(maxf);
	DECLARE_ALIGNED_VEC3(dvec);
	DECLARE_ALIGNED_VEC3(pt1);
	DECLARE_ALIGNED_VEC3(pt2);
	DECLARE_ALIGNED_VEC3(k);
	CMat3 <float,cmat_funcs>	mat,norm_mat;
	
	set_vec3		(cyl->center,cX,cY,cZ);
	

	copy_mat3x3			(mat.m,m);
	norm_mat			=mat.getNormalMat();
	
	copy_mat3x3			(cyl->norm_mat	,norm_mat.m);
	copy_mat3x3			(cyl->matrix	,mat.m);



	
	set_vec3	(pt1, 0	, -cyl->half_height	,0);
	set_vec3	(pt2, 0	, cyl->half_height	,0);
	
	mul_vec3x3	(pt1,cyl->matrix);
	mul_vec3x3	(pt2,cyl->matrix);

	sub_vec3	(pt2,pt1,dvec);

	dvec[0]	=dvec[0]*dvec[0];
	dvec[1]	=dvec[1]*dvec[1];
	dvec[2]	=dvec[2]*dvec[2];

	k[0]	=libc_sqrtf((dvec[1]+dvec[2])/(dvec[0]+dvec[1]+dvec[2]));
	k[1]	=libc_sqrtf((dvec[0]+dvec[2])/(dvec[0]+dvec[1]+dvec[2]));
	k[2]	=libc_sqrtf((dvec[0]+dvec[1])/(dvec[0]+dvec[1]+dvec[2]));

	scale_vec3	(k,cyl->radius);

	minf[0]	=	min(pt1[0],pt2[0]);
	minf[1]	=	min(pt1[1],pt2[1]);
	minf[2]	=	min(pt1[2],pt2[2]);

	maxf[0]	=	max(pt1[0],pt2[0]);
	maxf[1]	=	max(pt1[1],pt2[1]);
	maxf[2]	=	max(pt1[2],pt2[2]);

	sub_vec3	(minf,k,minf);
	add_vec3	(maxf,k,maxf);

	add_vec3	(maxf,cyl->center,maxf);
	add_vec3	(minf,cyl->center,minf);

	init_bboxf	(&cyl->AABOX,cyl->AABOX.cast_id,minf,maxf);	
	
	
}

void CScene::setCylCenterf (cylf_t	*cyl,float cX,float cY,float cZ,float aX,float aY,float aZ)
{
	DECLARE_ALIGNED_VEC3(minf);
	DECLARE_ALIGNED_VEC3(maxf);
	DECLARE_ALIGNED_VEC3(dvec);
	DECLARE_ALIGNED_VEC3(pt1);
	DECLARE_ALIGNED_VEC3(pt2);
	DECLARE_ALIGNED_VEC3(k);
	CMat3 <float,cmat_funcs>	mat,norm_mat;
	float		angles[3];

	set_vec3		(cyl->center,cX,cY,cZ);
	
	angles[0]	=	aX;
	angles[1]	=	aY;
	angles[2]	=	aZ;



	identity_mat3x3		(cyl->matrix);
	mat.rotateEuler		(angles[0],angles[1],angles[2]);
	norm_mat			=mat.getNormalMat();
		
	
	copy_mat3x3			(cyl->norm_mat	,norm_mat.m);
	copy_mat3x3			(cyl->matrix	,mat.m);



	
	set_vec3	(pt1, 0	, -cyl->half_height	,0);
	set_vec3	(pt2, 0	, cyl->half_height	,0);
	
	mul_vec3x3	(pt1,cyl->matrix);
	mul_vec3x3	(pt2,cyl->matrix);

	sub_vec3	(pt2,pt1,dvec);

	dvec[0]	=dvec[0]*dvec[0];
	dvec[1]	=dvec[1]*dvec[1];
	dvec[2]	=dvec[2]*dvec[2];

	k[0]	=libc_sqrtf((dvec[1]+dvec[2])/(dvec[0]+dvec[1]+dvec[2]));
	k[1]	=libc_sqrtf((dvec[0]+dvec[2])/(dvec[0]+dvec[1]+dvec[2]));
	k[2]	=libc_sqrtf((dvec[0]+dvec[1])/(dvec[0]+dvec[1]+dvec[2]));

	scale_vec3	(k,cyl->radius);

	minf[0]	=	min(pt1[0],pt2[0]);
	minf[1]	=	min(pt1[1],pt2[1]);
	minf[2]	=	min(pt1[2],pt2[2]);

	maxf[0]	=	max(pt1[0],pt2[0]);
	maxf[1]	=	max(pt1[1],pt2[1]);
	maxf[2]	=	max(pt1[2],pt2[2]);

	sub_vec3	(minf,k,minf);
	add_vec3	(maxf,k,maxf);

	add_vec3	(maxf,cyl->center,maxf);
	add_vec3	(minf,cyl->center,minf);

	init_bboxf	(&cyl->AABOX,cyl->AABOX.cast_id,minf,maxf);	
	
	
}




void CScene::add_boxf(bounding_boxf_t	*box)
{
	box_testf[n_boxf]._parameters	=	box->_parameters;
	box_testf[n_boxf].id			=	box->cast_id;
	n_boxf++;
}

void CScene::compute_camera()
{
	int n;
	/*
	matrice.identity			();
	matrice.rotate				(camAngle[0],0.0f					,1.0f	,0.0f);
	matrice.rotate				(camAngle[1],libc_cosf(camAngle[0])	,0.0f	,libc_sinf(camAngle[0]));
	*/
	matrice_normales		=	matrice.getNormalMat();
	
	
	matrice.inverseTo			(inv_matrice);
	matrice_normales.inverseTo	(inv_matrice_normales);


	
	n=0;
	while(n<CubefList->num_item)
	{
		cubef_t	*cube;

		cube				=  CubefList->getItemAt(n);
		inverse_mat3x3		(cube->matrix			,cube->inv_matrix);
		inverse_mat3x3		(cube->norm_mat			,cube->inv_norm_mat);


		n++;
	}

	n=0;
	while(n<SpherefList->num_item)
	{
		spheref_t	*sphere;

		sphere				=   SpherefList->getItemAt(n);
		inverse_mat3x3	(sphere->norm_mat	,sphere->inv_norm_mat);
		inverse_mat3x3	(sphere->matrix		,sphere->inv_matrix);
		n++;
	}

	n=0;
	while(n<CylfList->num_item)
	{
		cylf_t		*cyl;
		cyl					=	CylfList->getItemAt(n);
		inverse_mat3x3	(cyl->norm_mat		,cyl->inv_norm_mat);
		inverse_mat3x3	(cyl->matrix		,cyl->inv_matrix);
		n++;
	}


	n=0;
	while(n<HalofList->num_item)
	{
		spheref_t	*sphere;

		sphere				=	HalofList->getItemAt(n);
		inverse_mat3x3	(sphere->norm_mat		,sphere->inv_norm_mat);
		inverse_mat3x3	(sphere->matrix		,sphere->inv_matrix);
		n++;
	}


}

void CScene::doCasts(rayf_t *new_ray_ptr,thread_info_t *thread,int sX,int sY,int eX,int eY)
{
		unsigned int	pX,pY;
		unsigned int	n;
		unsigned char   *pData;
		
		pY = sY;
		while(pY<eY)
		{
			pX		=	sX;
			n		=	pY*targetImage.w+pX;
			pData	=	&targetImage.pixels[n*3];
			while(pX<eX)
			{
				mul_vec3x3_o		(raysf[n].origin		,inv_matrice			,thread->trans_ray_origin);
				add_vec3			(thread->trans_ray_origin,camPos,thread->trans_ray_origin);
				mul_vec3x3_o		(raysf[n].direction		,inv_matrice_normales	,thread->trans_ray_direction);
				
				init_rayf			(new_ray_ptr,thread->trans_ray_origin,thread->trans_ray_direction);
				castRayf			(new_ray_ptr,pData,0,thread);

				n++;
				
				pData+=3;
				pX++;
			}
			pY++;
		}
}

void CScene::getCameraInvMat_ogl(float *m)
{
	get_mat3x3gl(inv_matrice,m);
}
void CScene::getCameraInvNormMat_ogl(float *m)
{
	get_mat3x3gl(inv_matrice_normales,m);
}
int CScene::getGeomData()
{
	int		dataStart;
	int		n;
	dataStart=0;
	n=0;
	while(n<n_boxf)
	{
		boxData[dataStart+0]	=	box_testf[n]._parameters[0];
		boxData[dataStart+1]	=	box_testf[n]._parameters[1];
		boxData[dataStart+2]	=	box_testf[n]._parameters[4];
							
		boxData[dataStart+3]	=	box_testf[n]._parameters[2];
		boxData[dataStart+4]	=	box_testf[n]._parameters[3];
		boxData[dataStart+5]	=	box_testf[n]._parameters[5];

		dataStart				+= 6;

		boxData[dataStart+0]	=	get_id_var_type	(box_testf[n].id);
		boxData[dataStart+1]	=	get_id_var_idx	(box_testf[n].id);

		dataStart				+= 3;

		n++;
	}
	
	n=0;
	while(n<CubefList->num_item)
	{
		cubef_t	*cube;

		cube		=	CubefList->getItemAt(n);

		boxData[dataStart+0]	=	cube->center[0];
		boxData[dataStart+1]	=	cube->center[1];
		boxData[dataStart+2]	=	cube->center[2];
							
		boxData[dataStart+3]	=	cube->size[0];
		boxData[dataStart+4]	=	cube->size[1];
		boxData[dataStart+5]	=	cube->size[2];

		dataStart				+= 6;

		boxData[dataStart+0]	=	cube->AABOX.cast_id;
		boxData[dataStart+1]	=	cube->material.tid;
		boxData[dataStart+2]	=	cube->material.nid;

		dataStart				+= 3;

		boxData[dataStart+0]	=	cube->material.reflect;
		boxData[dataStart+1]	=	cube->material.refract;
		boxData[dataStart+2]	=	cube->material.refract_density;

		dataStart				+= 3;

		toOpenGL3x3		(cube->matrix,&boxData[dataStart+0]);
		dataStart		+= 9;
	
		n++;
	}

	n=0;
	while(n<SpherefList->num_item)
	{
		spheref_t	*sphere;

		sphere					=	SpherefList->getItemAt(n);

		boxData[dataStart+0]	=	sphere->center[0];
		boxData[dataStart+1]	=	sphere->center[1];
		boxData[dataStart+2]	=	sphere->center[2];
		dataStart				+= 3;

		boxData[dataStart+0]	=	sphere->sq_radius;
		dataStart				+= 3;
							
		boxData[dataStart+0]	=	sphere->AABOX.cast_id;
		boxData[dataStart+1]	=	sphere->material.tid;
		boxData[dataStart+2]	=	sphere->material.nid;
		
		dataStart				+= 3;

		boxData[dataStart+0]	=	sphere->material.reflect;
		boxData[dataStart+1]	=	sphere->material.refract;
		boxData[dataStart+2]	=	sphere->material.refract_density;
		dataStart				+= 3;

		toOpenGL3x3		(sphere->inv_matrix	,&boxData[dataStart]);
		dataStart	+= 9;

		toOpenGL3x3		(sphere->matrix		,&boxData[dataStart]);
		dataStart	+= 9;
		n++;
	}

	n=0;
	while(n<CylfList->num_item)
	{
		cylf_t	*cyl;

		cyl						=	CylfList->getItemAt(n);

		boxData[dataStart+0]	=	cyl->center[0];
		boxData[dataStart+1]	=	cyl->center[1];
		boxData[dataStart+2]	=	cyl->center[2];

		dataStart				+= 3;
						
		boxData[dataStart+0]	=	cyl->radiussq;
		boxData[dataStart+1]	=	cyl->half_height;
		dataStart				+= 3;

		boxData[dataStart+0]	=	cyl->AABOX.cast_id;
		boxData[dataStart+1]	=	cyl->material.tid;
		boxData[dataStart+2]	=	cyl->material.nid;
		dataStart				+= 3;
		
		boxData[dataStart+0]	=	cyl->material.reflect;
		boxData[dataStart+1]	=	cyl->material.refract;
		boxData[dataStart+2]	=	cyl->material.refract_density;
		dataStart				+= 3;

		toOpenGL3x3		(cyl->inv_matrix	,&boxData[dataStart+0]);
		dataStart	+= 9;

		toOpenGL3x3		(cyl->inv_norm_mat	,&boxData[dataStart+0]);
		dataStart	+= 9;

		toOpenGL3x3		(cyl->norm_mat		,&boxData[dataStart+0]);
		dataStart	+= 9;

		n++;
	}
	n=0;
	while(n<HalofList->num_item)
	{
		spheref_t	*sphere;

		sphere					=	HalofList->getItemAt(n);

		boxData[dataStart+0]	=	sphere->center[0];
		boxData[dataStart+1]	=	sphere->center[1];
		boxData[dataStart+2]	=	sphere->center[2];
		dataStart				+= 3;

		boxData[dataStart+0]	=	sphere->sq_radius;
		dataStart				+= 3;
		dataStart				+= 6;//skip material
		toOpenGL3x3				(sphere->inv_matrix	,&boxData[dataStart]);
		dataStart				+= 9;
							

		n++;
	}

	return dataStart;
}


int CScene::castRayToLightf	( const rayf_t *ray	,unsigned int cast)
{
	unsigned int	bound_ids[128];
	float			min_max[2];
	unsigned int	intersect;	
	int				n;

	
	intersect=intersect_ray_boxesf(ray->f_origin,ray->f_inv_direction,ray->o_sign01,box_testf,n_boxf,cast,bound_ids,min_max);
	if(!intersect)return 1;

	n		=0;
	while(bound_ids[n]!=0)
	{
		float			sphere_dist;
		float			cube_dist;
		unsigned int	item_type;
		unsigned int	item_idx;

		item_type	=get_id_var_type(bound_ids[n]);
		item_idx	=get_id_var_idx	(bound_ids[n]);
		intersect	=0;

		switch(item_type)
		{
			case 1:	
				intersect=Sphere_intersect(ray,&SpherefList->listPtr[item_idx],&sphere_dist,NULL,NULL);
			break;
			case 2:	
				intersect=rayIntersectCube(&CubefList->listPtr[item_idx],ray,&cube_dist,NULL);
			break;
			case 3:
				float cyldist[2];
				intersect=intersectRayCyl(ray,&CylfList->listPtr[item_idx],cyldist,NULL);
			break;
		}
		if(intersect)
			return 0;
		n++;
	}
	return 1;
}


void CScene::castRayf ( const rayf_t *new_ray	,unsigned char *pix,int cast,thread_info_t *thread)
{
	vec3<vec3SSE>	lightVec;
	vec3<vec3SSE>	rayPos;
	vec3<vec3SSE>	outPos;
	vec3<vec3SSE>	normVec;

	unsigned int	bound_ids[128];
	int				pixNew[4];
	int				pixTex[4];
	float			min_max[2];
	float			uv_coord[2];
	float			env_uv[2];
	unsigned char	newPix[4];
	unsigned char	pixNorm[4];
	unsigned char	env_pix[4];
	float			dist;
	float			dot;
	float			shadow;
	unsigned int	intersect;	
	int				n;
	int				env_map;
	int				bbox_id;
	unsigned int	col;
	materialf_t		*material;
	spheref_t		*sphere_ptr;
	cubef_t			*cube_ptr;
	cylf_t			*cyl_ptr;
	float			ddlight;
	float			sphere_dist,sphere_dist2;


	min_max[0]		=	 100000000000000000000.0f;
	min_max[1]		=	-100000000000000000000.0f;
	

	pix[0]		=	0;
	pix[1]		=	0;
	pix[2]		=	0;

	intersect=intersect_ray_boxesf(new_ray->f_origin,new_ray->f_inv_direction,new_ray->o_sign01,box_testf,n_boxf,cast,bound_ids,min_max);
	
	if(!intersect)
		goto no_inter;
	

	dist		=	1000000000000000.0f;
	sphere_ptr	=	NULL;
	cube_ptr	=	NULL;
	cyl_ptr		=	NULL;
	bbox_id		=	0;
	n		=0;
	col		=0;
	while(bound_ids[n]!=0)
	{
		spheref_t		*sphere;
		cubef_t			*cube;
		cylf_t			*cyl;
		float			sphere_dist,sphere_dist2;
		float			cube_dist;
		float			cyl_dist;
		unsigned int	item_type;
		unsigned int	item_idx;

		item_type	=get_id_var_type(bound_ids[n]);
		item_idx	=get_id_var_idx	(bound_ids[n]);

		switch(item_type)
		{
			case 1:	
				sphere	=	&SpherefList->listPtr[item_idx];
				if(Sphere_intersect(new_ray,sphere,&sphere_dist,&sphere_dist2,thread->tmpNorm))
				{
					if(sphere_dist<dist)
					{
						sphere_ptr	=sphere;
						cube_ptr	=NULL;
						cyl_ptr		=NULL;
						dist		=sphere_dist;
						bbox_id		=bound_ids[n];
						outPos		=thread->tmpNorm;
						col=1;
					}
					
				}
			break;
			case 2:	
				cube	=	&CubefList->listPtr[item_idx];
				if(rayIntersectCube(cube,new_ray,&cube_dist,thread->tmpNorm))
				{
					if(cube_dist<dist)
					{
						cube_ptr			=	cube;
						sphere_ptr			=	NULL;
						cyl_ptr				=	NULL;
						dist				=	cube_dist;
						bbox_id				=	bound_ids[n];

						outPos				=	thread->tmpNorm;
						col					=	1;
					}
				}
			break;
			case 3:
				float cyldist[2];

				cyl	=	&CylfList->listPtr[item_idx];
				if(intersectRayCyl(new_ray,cyl,cyldist,thread->tmpNorm))
				{
					if(cyldist[0]>0)
					{
						cyl_dist=cyldist[0];
						if(cyl_dist<dist)
						{
							cyl_ptr				=	cyl;
							cube_ptr			=	NULL;
							sphere_ptr			=	NULL;
							
							dist				=	cyl_dist;
							bbox_id				=	bound_ids[n];
							col					=	1;
							outPos				=	thread->tmpNorm;
						}
					}
				}
			break;
		}
		n++;
	}

	if(col==0)goto no_inter;
	rayPos.v[0]			=	new_ray->origin[0]+new_ray->direction[0]*dist;
	rayPos.v[1]			=	new_ray->origin[1]+new_ray->direction[1]*dist;
	rayPos.v[2]			=	new_ray->origin[2]+new_ray->direction[2]*dist;
			
	if(sphere_ptr!=NULL)
	{
		material		=&sphere_ptr->material;

		//sub_vec3		(outPos.v,sphere_ptr->center,normVec.v);
		copy_vec3		(normVec.v,outPos.v);
		normalize_vec3	(normVec.v);

		uv_coord[0]	=	libc_asinf(normVec[0])/PIf + 0.5f;
		uv_coord[1]	=	libc_asinf(normVec[1])/PIf + 0.5f;

		normVec		*=   sphere_ptr->norm_mat;
	}
	else if(cyl_ptr!=NULL)
	{
		DECLARE_ALIGNED_VEC3(A);
		DECLARE_ALIGNED_VEC3(C);
		DECLARE_ALIGNED_VEC3(V);
		DECLARE_ALIGNED_VEC3(P);

		material			=	&cyl_ptr->material;

		C[0]				=	0.0f;
		C[1]				=	0.0f;
		C[2]				=	0.0f;

		A[0]				=	0.0f;
		A[1]				=	1.0f;
		A[2]				=	0.0f;

		//V = X - C
		sub_vec3			(outPos.v	,C,V);
		project_vec3		(V			,A,P);

		sub_vec3			(V,P,normVec.v); // Project V onto A
		normalize_vec3		(normVec.v);

		uv_coord[0]		=	libc_asinf(normVec.v[0])/PIf + 0.5f;
		uv_coord[1]		=	length_vec3(P)/(cyl_ptr->half_height);
		if(uv_coord[1]>1.0)
			uv_coord[1]=1.0;

		normVec			*=   cyl_ptr->norm_mat;
	}
	else if(cube_ptr!=NULL)
	{
		float distPlane;
		
		material	= &cube_ptr->material;
		n			= 0;
		while(n<6)
		{
			distPlane		=	dot_vec3(cube_ptr->planes[n].norm,rayPos.v)+cube_ptr->planes[n].det;
			if(libc_fabsf(distPlane)<1.0)
			{
				normVec	= cube_ptr->planes[n].norm;
				switch(n)
				{
					case 2:
					case 0:
						uv_coord[0]		=	(outPos.v[0]+(cube_ptr->size[0]))/(cube_ptr->size[0]*2.0f);
						uv_coord[1]		=	(outPos.v[1]+(cube_ptr->size[1]))/(cube_ptr->size[1]*2.0f);
					break;
					case 3:
					case 1:
						//	cube_normz[1]=vec3(1.0,0.0,0.0);
						uv_coord[0]		=	(outPos.v[1]+(cube_ptr->size[1]))/(cube_ptr->size[1]*2.0f);
						uv_coord[1]		=	(outPos.v[2]+(cube_ptr->size[2]))/(cube_ptr->size[2]*2.0f);
					break;		
					case 4:
					case 5:
						//	cube_normz[4]=vec3(0.0,1.0,0.0);
						uv_coord[0]		=	(outPos.v[0]+(cube_ptr->size[0]))/(cube_ptr->size[0]*2.0f);
						uv_coord[1]		=	(outPos.v[2]+(cube_ptr->size[2]))/(cube_ptr->size[2]*2.0f);
					break;
				}
				break;
			}
			n++;
		}
		
	}
	
	sub_vec3			(lightPos,rayPos.v,lightVec.v);
	lightVec.lengthTo1	();

	shadow=1.0f;

	if(cast==0)
	{
		init_rayf	(thread->reflectRayf,rayPos.v,lightVec.v);
		if(!castRayToLightf(thread->reflectRayf,bbox_id))
		{
			shadow=0.8f;
		}
	}


	if((material->texture!=NULL)&&(material->texture->pixels!=NULL))
	{
		getTexPix_32(material->texture,uv_coord,pixTex);
	}
	else
	{
		pixTex[0]=255;
		pixTex[1]=255;
		pixTex[2]=255;
	}


	if(material->normals!=NULL)
	{
		vec3<vec3SSE>			tNorm;
		vec3<vec3SSE>			tangent,nt;
		vec3<vec3SSE>			binormal,nb;
		vec3<vec3SSE>			test_norm;
		mat3x3f_t				mat;
		float					dist,plane_det;
		
		getTexPix_32uc(material->normals,uv_coord,pixNorm);
		tNorm[0]= (pixNorm[0]-128.0);
		tNorm[1]= (pixNorm[1]-128.0);
		tNorm[2]= (pixNorm[2]-128.0);

		
		tNorm			*=	0.5f;							//scale

		plane_det		=	-(normVec^rayPos);				//dot
		test_norm		=	rayPos;

		test_norm.v[0]	=	test_norm.v[0]+1.0;
		dist			=	(test_norm^normVec)+plane_det;
		tangent			=	test_norm-normVec*dist;			//sub scaled
		tangent.lengthTo1();
		binormal		=	normVec|tangent;				//normalize + cross
		binormal.lengthTo1();
										
		tangent.out_to(&mat[0]);
		binormal.out_to(&mat[4]);
		normVec.out_to(&mat[8]);
	
		tNorm		*=mat;									//3x3 matrix mul
		normVec		=tNorm.getlengthTo1();
	}
	
	
	
	dot	=	libc_fabsf(normVec^lightVec);
	//if(dot<0)dot=0;
	

	if(cast==0)
	{
		DECLARE_ALIGNED_VEC3(Reflect);
		pixNew[0]=0;
		pixNew[1]=0;
		pixNew[2]=0;

		
		if(material->reflect>0.0f)
		{
			reflect_vec3	(new_ray->direction,normVec.v,Reflect);
			init_rayf		(thread->reflectRayf,rayPos.v,Reflect);
			castRayf		(thread->reflectRayf,newPix,bbox_id,thread);

			pixNew[0]=pixNew[0]*(1.0-material->reflect)+newPix[0]*material->reflect;
			pixNew[1]=pixNew[1]*(1.0-material->reflect)+newPix[1]*material->reflect;
			pixNew[2]=pixNew[2]*(1.0-material->reflect)+newPix[2]*material->reflect;
		}

		if(material->refract>0.0f)
		{
			refract_vec3	(normVec.v,new_ray->direction,1.0,material->refract_density,Reflect);
			init_rayf		(thread->reflectRayf,rayPos.v,Reflect);
			castRayf		(thread->reflectRayf,newPix,bbox_id,thread);
			
			pixNew[0]=pixNew[0]*(1.0-material->refract)+newPix[0]*material->refract;
			pixNew[1]=pixNew[1]*(1.0-material->refract)+newPix[1]*material->refract;
			pixNew[2]=pixNew[2]*(1.0-material->refract)+newPix[2]*material->refract;
	
		}
	}

	

	ddlight			=0.0;

	n=0;
	while(n<HalofList->num_item)
	{
		spheref_t		*spheref;
		spheref			=HalofList->getItemAt(n);
		if(Sphere_intersect(new_ray,spheref,&sphere_dist,&sphere_dist2,NULL))
		{
			float volume;
			if(sphere_dist<dist)
			{
				volume		=	libc_fabsf(sphere_dist2-sphere_dist);
				ddlight		=	volume/150.0f;//powf(volume/100.0,100.0);

				if(ddlight<0.0)
					ddlight=0.0;
						
				if(ddlight>1.0)
					ddlight=1.0;
			}
		}
		n++;
	}
	
	pixNew[0]=	(pixTex[0]*shadow*dot+pixNew[0]);
	pixNew[1]=	(pixTex[1]*shadow*dot+pixNew[1])+ddlight*128;
	pixNew[2]=	(pixTex[2]*shadow*dot+pixNew[2]);



	if(pixNew[0]>255)pixNew[0]=255;
	if(pixNew[1]>255)pixNew[1]=255;
	if(pixNew[2]>255)pixNew[2]=255;

	if(pixNew[0]<0)pixNew[0]=0;
	if(pixNew[1]<0)pixNew[1]=0;
	if(pixNew[2]<0)pixNew[2]=0;
	
	pix[0]=	pixNew[0];
	pix[1]=	pixNew[1];
	pix[2]=	pixNew[2];


	return;
	
	no_inter:
	

	

	ddlight			=0.0;
	
	n				=0;
	while(n<HalofList->num_item)
	{
		spheref_t		*spheref;
		spheref			=HalofList->getItemAt(n);
		if(Sphere_intersect(new_ray,spheref,&sphere_dist,&sphere_dist2,NULL))
		{
			float volume;
	
			volume		=	libc_fabsf(sphere_dist2-sphere_dist);
			ddlight		=	volume/150.0f;//powf(volume/100.0,100.0);

			if(ddlight<0.0)
				ddlight=0.0;
						
			if(ddlight>1.0)
				ddlight=1.0;
		}
		n++;
	}
	

	env_map	=	readEnvCubemapPixf(new_ray,env_uv);
	if(env_map>=0)
	{
		getTexPix		(&textures[env_map],env_uv,env_pix);

		pix[0]			=(pix[0]+env_pix[0])*0.5f;
		pix[1]			=(pix[1]+env_pix[1])*0.5f+ddlight*128;
		pix[2]			=(pix[2]+env_pix[2])*0.5f;
	}
}

void get_bbox_params(unsigned int ray_sign,bounding_boxf_t *box,float *parameters)
{
	switch(ray_sign)
	{
		case 0:
			//sign0 = 0,sign1 = 0,sign2 = 0
			parameters[0+0]=box->parameters[0][0];
			parameters[0+1]=box->parameters[0][1];
			parameters[0+2]=box->parameters[1][0];
			parameters[0+3]=box->parameters[1][1];
			
			parameters[4+0]=box->parameters[0][2];
			parameters[4+1]=box->parameters[1][2];
			parameters[4+2]=box->parameters[0][2];
			parameters[4+3]=box->parameters[1][2];
		break;
		case 1:
			//sign0 = 1,sign1 = 0,sign2 = 0
			parameters[0+0]=box->parameters[1][0];
			parameters[0+1]=box->parameters[0][1];
			parameters[0+2]=box->parameters[0][0];
			parameters[0+3]=box->parameters[1][1];

			parameters[4+0]=box->parameters[0][2];
			parameters[4+1]=box->parameters[1][2];
			parameters[4+2]=box->parameters[0][2];
			parameters[4+3]=box->parameters[1][2];
		break;
		case 2:
			//sign0 = 0,sign1 = 1,sign2 = 0
			parameters[0+0]=box->parameters[0][0];
			parameters[0+1]=box->parameters[1][1];
			parameters[0+2]=box->parameters[1][0];
			parameters[0+3]=box->parameters[0][1];

			parameters[4+0]=box->parameters[0][2];
			parameters[4+1]=box->parameters[1][2];
			parameters[4+2]=box->parameters[0][2];
			parameters[4+3]=box->parameters[1][2];
		break;
		case 3:
			//sign0 = 1,sign1 = 1,sign2 = 0
			parameters[0+0]=box->parameters[1][0];
			parameters[0+1]=box->parameters[1][1];
			parameters[0+2]=box->parameters[0][0];
			parameters[0+3]=box->parameters[0][1];

			parameters[4+0]=box->parameters[0][2];
			parameters[4+1]=box->parameters[1][2];
			parameters[4+2]=box->parameters[0][2];
			parameters[4+3]=box->parameters[1][2];
		break;
		case 4:
			//sign0 = 0,sign1 = 0,sign2 = 1
			parameters[0+0]=box->parameters[0][0];
			parameters[0+1]=box->parameters[0][1];
			parameters[0+2]=box->parameters[1][0];
			parameters[0+3]=box->parameters[1][1];

			parameters[4+0]=box->parameters[1][2];
			parameters[4+1]=box->parameters[0][2];
			parameters[4+2]=box->parameters[1][2];
			parameters[4+3]=box->parameters[0][2];
		break;
		case 5:
			//sign0 = 1,sign1 = 0,sign2 = 1
			parameters[0+0]=box->parameters[1][0];
			parameters[0+1]=box->parameters[0][1];
			parameters[0+2]=box->parameters[0][0];
			parameters[0+3]=box->parameters[1][1];

			parameters[4+0]=box->parameters[1][2];
			parameters[4+1]=box->parameters[0][2];
			parameters[4+2]=box->parameters[1][2];
			parameters[4+3]=box->parameters[0][2];
		break;
		case 6:
			//sign0 = 0,sign1 = 1,sign2 = 1
			parameters[0+0]=box->parameters[0][0];
			parameters[0+1]=box->parameters[1][1];
			parameters[0+2]=box->parameters[1][0];
			parameters[0+3]=box->parameters[0][1];

			parameters[4+0]=box->parameters[1][2];
			parameters[4+1]=box->parameters[0][2];
			parameters[4+2]=box->parameters[1][2];
			parameters[4+3]=box->parameters[0][2];
		break;
		case 7:
			//sign0 = 1,sign1 = 1,sign2 = 1
			parameters[0+0]=box->parameters[1][0];
			parameters[0+1]=box->parameters[1][1];
			parameters[0+2]=box->parameters[0][0];
			parameters[0+3]=box->parameters[0][1];

			parameters[4+0]=box->parameters[1][2];
			parameters[4+1]=box->parameters[0][2];
			parameters[4+2]=box->parameters[1][2];
			parameters[4+3]=box->parameters[0][2];
		break;
	}
}

void init_bboxf		(bounding_boxf_t *box,unsigned int id,vec3f_t min,vec3f_t max)
{
	copy_vec3			(box->parameters[0],min);
	copy_vec3			(box->parameters[1],max);

	box->cast_id	=	id;

	//sign0 = 0,sign1 = 0,sign2 = 0
	box->_parameters[0+0]=box->parameters[0][0];
	box->_parameters[0+1]=box->parameters[0][1];
	box->_parameters[0+2]=box->parameters[1][0];
	box->_parameters[0+3]=box->parameters[1][1];
	
	box->_parameters[4+0]=box->parameters[0][2];
	box->_parameters[4+1]=box->parameters[1][2];
	box->_parameters[4+2]=box->parameters[0][2];
	box->_parameters[4+3]=box->parameters[1][2];

	//sign0 = 1,sign1 = 0,sign2 = 0
	box->_parameters[8+0]=box->parameters[1][0];
	box->_parameters[8+1]=box->parameters[0][1];
	box->_parameters[8+2]=box->parameters[0][0];
	box->_parameters[8+3]=box->parameters[1][1];

	box->_parameters[12+0]=box->parameters[0][2];
	box->_parameters[12+1]=box->parameters[1][2];
	box->_parameters[12+2]=box->parameters[0][2];
	box->_parameters[12+3]=box->parameters[1][2];

	//sign0 = 0,sign1 = 1,sign2 = 0
	box->_parameters[16+0]=box->parameters[0][0];
	box->_parameters[16+1]=box->parameters[1][1];
	box->_parameters[16+2]=box->parameters[1][0];
	box->_parameters[16+3]=box->parameters[0][1];

	box->_parameters[20+0]=box->parameters[0][2];
	box->_parameters[20+1]=box->parameters[1][2];
	box->_parameters[20+2]=box->parameters[0][2];
	box->_parameters[20+3]=box->parameters[1][2];

	//sign0 = 1,sign1 = 1,sign2 = 0
	box->_parameters[24+0]=box->parameters[1][0];
	box->_parameters[24+1]=box->parameters[1][1];
	box->_parameters[24+2]=box->parameters[0][0];
	box->_parameters[24+3]=box->parameters[0][1];

	box->_parameters[28+0]=box->parameters[0][2];
	box->_parameters[28+1]=box->parameters[1][2];
	box->_parameters[28+2]=box->parameters[0][2];
	box->_parameters[28+3]=box->parameters[1][2];


	//sign0 = 0,sign1 = 0,sign2 = 1
	box->_parameters[32+0]=box->parameters[0][0];
	box->_parameters[32+1]=box->parameters[0][1];
	box->_parameters[32+2]=box->parameters[1][0];
	box->_parameters[32+3]=box->parameters[1][1];

	box->_parameters[36+0]=box->parameters[1][2];
	box->_parameters[36+1]=box->parameters[0][2];
	box->_parameters[36+2]=box->parameters[1][2];
	box->_parameters[36+3]=box->parameters[0][2];


	//sign0 = 1,sign1 = 0,sign2 = 1
	box->_parameters[40+0]=box->parameters[1][0];
	box->_parameters[40+1]=box->parameters[0][1];
	box->_parameters[40+2]=box->parameters[0][0];
	box->_parameters[40+3]=box->parameters[1][1];

	box->_parameters[44+0]=box->parameters[1][2];
	box->_parameters[44+1]=box->parameters[0][2];
	box->_parameters[44+2]=box->parameters[1][2];
	box->_parameters[44+3]=box->parameters[0][2];

	//sign0 = 0,sign1 = 1,sign2 = 1
	box->_parameters[48+0]=box->parameters[0][0];
	box->_parameters[48+1]=box->parameters[1][1];
	box->_parameters[48+2]=box->parameters[1][0];
	box->_parameters[48+3]=box->parameters[0][1];

	box->_parameters[52+0]=box->parameters[1][2];
	box->_parameters[52+1]=box->parameters[0][2];
	box->_parameters[52+2]=box->parameters[1][2];
	box->_parameters[52+3]=box->parameters[0][2];

	//sign0 = 1,sign1 = 1,sign2 = 1
	box->_parameters[56+0]=box->parameters[1][0];
	box->_parameters[56+1]=box->parameters[1][1];
	box->_parameters[56+2]=box->parameters[0][0];
	box->_parameters[56+3]=box->parameters[0][1];

	box->_parameters[60+0]=box->parameters[1][2];
	box->_parameters[60+1]=box->parameters[0][2];
	box->_parameters[60+2]=box->parameters[1][2];
	box->_parameters[60+3]=box->parameters[0][2];

}



/*

void getPix	 (float *uv_coord,unsigned char *pix)
{
	if(fabs(uv_coord[0])<0.5)
	{
		if(fabs(uv_coord[1])<0.5)
		{
			pix[0]=128;
			pix[1]=0;
			pix[2]=0;
		}
		else
		{
			pix[0]=0;
			pix[1]=128;
			pix[2]=0;
		}
	}
	else
	{
		if(fabs(uv_coord[1])>
			0.5)
		{
			pix[0]=128;
			pix[1]=0;
			pix[2]=0;
		}
		else
		{
			pix[0]=0;
			pix[1]=128;
			pix[2]=0;
		}

	}
}
void castRay(const rayi_t *ray,unsigned char *pix,int cast,rayi_t *reflectRay)
{
	vec3i_t			o;
	vec3f_t			d;
	vec3i_t			i_dir;

	vec3f_t			normVec;
	vec3f_t			lightVec;
	vec3f_t			Reflect;
	vec3f_t			Refract;
	
	float			uv_coord[2];
	unsigned char	newPix[3];
	int				dist;
	int				min,max;
	int				c_minmax[4];
	int				idx;
	int				intersect;
	float			dot;
	int				rayPos[3];
	

	dist		=	0;
	pix[0]		=	0;
	pix[1]		=	0;
	pix[2]		=	0;
	
	intersect	=	0;
	min			=	100000;
	max			=	-100000;


	if((ray->direction[0]!=0)&&(ray->direction[1]!=0))
	{

		if(cast!=3)
		{
			if(intersect_ray_box(ray->origin,ray->inv_direction,&ground->AABOX.oo_params_1[ray->o_sign01],&ground->AABOX.oo_params_2[ray->sign[2]*4],c_minmax))
			{
				if(c_minmax[0]>0)
				{
					if(c_minmax[0]<min)
						min=c_minmax[0];

					if(c_minmax[1]>max)
						max=c_minmax[1];
			
					intersect=1;
				}
			}
		}

		
		if(cast!=2)
		{
			if(intersect_ray_box(ray->origin,ray->inv_direction,&cube->AABOX.oo_params_1[ray->o_sign01],&cube->AABOX.oo_params_2[ray->sign[2]*4],c_minmax))
			{
				if(c_minmax[0]>0)
				{
					if(c_minmax[0]<min)
						min=c_minmax[0];

					if(c_minmax[1]>max)
						max=c_minmax[1];
			
					intersect=1;
				}
			}
		}
	
		if(cast!=1)
		{
			if(intersect_ray_box(ray->origin,ray->inv_direction,&sphere->AABOX.oo_params_1[ray->o_sign01],&sphere->AABOX.oo_params_2[ray->sign[2]*4],c_minmax))
			{
				if(c_minmax[0]>0)
				{
					if(c_minmax[0]<min)
						min=c_minmax[0];

					if(c_minmax[1]>max)
						max=c_minmax[1];
			
					intersect=1;
				}
			}
		}
		if(!intersect)return;
	}
	else
	{
		min=0;
		max=10000;
	}
	if(min<0)return;

	i_dir[0]	=	ray->direction[0]*PREC_MUL;
	i_dir[1]	=	ray->direction[1]*PREC_MUL;
	i_dir[2]	=	ray->direction[2]*PREC_MUL;
	
	rayPos[0]	=	ray->origin[0]*PREC_MUL+i_dir[0]*min;
	rayPos[1]	=	ray->origin[1]*PREC_MUL+i_dir[1]*min;
	rayPos[2]	=	ray->origin[2]*PREC_MUL+i_dir[2]*min;
	

	i_dir[0]	=	i_dir[0]*2;
	i_dir[1]	=	i_dir[1]*2;
	i_dir[2]	=	i_dir[2]*2;


	dist		=	min;
	
	while(dist<max)
	{	
		
		if(cast!=3)
		{
			if(pointInCube(rayPos,ground,normVec,uv_coord))
			{


				newPix[0]		=	0;
				newPix[1]		=	0;
				newPix[2]		=	0;
				if(cast==0)
				{
					reflect_vec3		(ray->direction,normVec,Reflect);
					normalize_vec3		(Reflect);


					o[0]=rayPos[0]>>PREC_SHIFT;
					o[1]=rayPos[1]>>PREC_SHIFT;
					o[2]=rayPos[2]>>PREC_SHIFT;

					init_ray	(reflectRay,o,Reflect);

					castRay		(reflectRay,newPix,3,NULL);
				}

				lightVec[0]	=Scene->lightPos[0]-(rayPos[0]>>PREC_SHIFT);
				lightVec[1]	=Scene->lightPos[1]-(rayPos[1]>>PREC_SHIFT);
				lightVec[2]	=Scene->lightPos[2]-(rayPos[2]>>PREC_SHIFT);

				normalize_vec3	(lightVec);

				dot=dot_vec3	(normVec,lightVec);
				if(dot<0)dot=0;


				getPix(uv_coord,pix);
			
				pix[0]+=newPix[0]*dot;
				pix[1]+=newPix[1]*dot;
				pix[2]+=newPix[2]*dot;

				return;
			}
		}
			
		
		if(cast!=2)
		{
			if(pointInCube(rayPos,cube,normVec,uv_coord))
			{

				newPix[0]		=	0;
				newPix[1]		=	0;
				newPix[2]		=	0;

				if(cast==0)
				{
					reflect_vec3		(ray->direction,normVec,Reflect);
					normalize_vec3		(Reflect);

					o[0]=rayPos[0]>>PREC_SHIFT;
					o[1]=rayPos[1]>>PREC_SHIFT;
					o[2]=rayPos[2]>>PREC_SHIFT;

					init_ray	(reflectRay,o,Reflect);

					castRay		(reflectRay,newPix,2,NULL);
				}


			
				lightVec[0]	=Scene->lightPos[0]-(rayPos[0]>>PREC_SHIFT);
				lightVec[1]	=Scene->lightPos[1]-(rayPos[1]>>PREC_SHIFT);
				lightVec[2]	=Scene->lightPos[2]-(rayPos[2]>>PREC_SHIFT);

				normalize_vec3	(lightVec);

				dot=dot_vec3	(normVec,lightVec);
				if(dot<0)dot=0;
			
				getPix(uv_coord,pix);
			
				pix[0]+=newPix[0]*dot;
				pix[1]+=newPix[1]*dot;
				pix[2]+=newPix[2]*dot;
				return;
			}
		}
		
		

		
		if(cast!=1)
		{
			if(pointInSphere(rayPos,sphere,normVec))
			{
				newPix[0]		=	0;
				newPix[1]		=	0;
				newPix[2]		=	0;

				if(cast==0)
				{
					
					reflect_vec3		(ray->direction,normVec,Reflect);
					normalize_vec3		(Reflect);

					o[0]=rayPos[0]>>PREC_SHIFT;
					o[1]=rayPos[1]>>PREC_SHIFT;
					o[2]=rayPos[2]>>PREC_SHIFT;

					init_ray	(reflectRay,o,Reflect);

					castRay		(reflectRay,newPix,1,NULL);
				}


				lightVec[0]	=Scene->lightPos[0]-(rayPos[0]>>PREC_SHIFT);
				lightVec[1]	=Scene->lightPos[1]-(rayPos[1]>>PREC_SHIFT);
				lightVec[2]	=Scene->lightPos[2]-(rayPos[2]>>PREC_SHIFT);

				normalize_vec3	(lightVec);

				dot=dot_vec3	(normVec,lightVec);
				if(dot<0)dot=0;
			
				//dot=1;

				pix[0]=(newPix[0]/2)*dot+newPix[0]/2;
				pix[1]=(newPix[1]/2)*dot+newPix[1]/2;
				pix[2]=(newPix[2]/2)*dot+newPix[2]/2;
				return;
			}
		}
		

		rayPos[0]=rayPos[0]+i_dir[0];
		rayPos[1]=rayPos[1]+i_dir[1];
		rayPos[2]=rayPos[2]+i_dir[2];
		dist=dist+2;
	}
}
*/


