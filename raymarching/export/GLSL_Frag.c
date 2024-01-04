uniform vec3	lightPos;

uniform	mat3x3	cameraInvNormal;
uniform	mat3x3	cameraInvMat;
uniform	vec3	cameraPos;

uniform	float	winW;
uniform	float	winH;

//general box infos
uniform vec3	cube_normz[6];
int				box_dets[6];

//axis aligned bounding box
uniform int numboxes;

//cubes
uniform int num_cubes;

//sphere
uniform int num_spheres;

//cylinder
uniform int num_cyls;

//halos
uniform int num_halos;

float			PI		= 3.14159265358979323846264;
float			invRes	= 1.0/2048.0;
/*
uniform vec3	minboxData[32];

uniform vec3	cube_sizes[16];
uniform float	cube_ids[8];
uniform float	cube_tex[8];
uniform mat3x3	cube_mat[8];

uniform float	sphere_ids[8];
uniform vec3	sphere_centers[8];
uniform float	sphere_sqradius[8];


uniform float	cyl_ids[8];
uniform vec3	cyl_pt1[8];
uniform vec3	cyl_pt2[8];
uniform float	cyl_radius[8];
*/

float			D3_EPSILON;

uniform			samplerCube cubemap;
uniform			sampler2D	texture1;
uniform			sampler2D	texture2;
uniform			sampler1D	boxDataTex;

vec3			sp_center;
vec3 refract(vec3 incident,vec3 normal,float n1, float n2)
{
	vec3			outVec;
	float n			=  n1 / n2;
	float cosI		= dot(normal, incident);
	float  sinT2	= n * n * (1.0 - cosI * cosI);
	if (sinT2 > 1.0)
	{
		return vec3(0,0,0);
	}

	outVec	=	n*incident-(n+sqrt(1.0-sinT2))*normal;
	
	return outVec;
}

int getItemId(int type,int idx)
{
	return ((type<<24)|idx);
}

int getItemType(int item_id)
{
	return (item_id>>24);
}


int getItemIdx(int item_id)
{
	return (item_id&0x00FFFFFF);
}

int getItemInfos(int i,out int item_type,out int item_idx, out int item_ID )
{
	item_type	  = int (texture1D(boxDataTex,float((i*3)+2)*invRes).r);
	item_idx	  = int (texture1D(boxDataTex,float((i*3)+2)*invRes).g);
	item_ID		  = getItemId(item_type,item_idx);
	return 1;
}
int getboxMinMax(int i,out vec3 min,out vec3 max)
{
	min		  =	texture1D(boxDataTex,float((i*3)+0)*invRes).rgb;
	max		  =	texture1D(boxDataTex,float((i*3)+1)*invRes).rgb;
	return 1;
}

int getSphereInfos(int item_idx,out vec3 sphere_center,out float sphere_radius,out mat3x3 sphere_inv_mat,out mat3x3 sphere_mat)
{
	float			startPixU;

	startPixU		=	float(numboxes*3+num_cubes*7+(item_idx*10));

	sphere_center	=	texture1D(boxDataTex,(startPixU+0.0)*invRes).rgb;
	sphere_radius	=	texture1D(boxDataTex,(startPixU+1.0)*invRes).r;

	sphere_inv_mat	=	mat3x3(texture1D(boxDataTex,(startPixU+4.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+ 5.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+ 6.0)*invRes).rgb);
	sphere_mat		=	mat3x3(texture1D(boxDataTex,(startPixU+7.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+8.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+9.0)*invRes).rgb);
	return 1;
}

int getCubeInfos(int item_idx,out vec3 cube_center,out vec3 cube_size,out mat3x3 cube_mat)
{
	float			startPixU;

	startPixU	=	float(numboxes*3+(item_idx*7));

	cube_center	=	texture1D(boxDataTex,(startPixU+0.0)*invRes).rgb;
	cube_size	=	texture1D(boxDataTex,(startPixU+1.0)*invRes).rgb;
	cube_mat	=	mat3x3	 (texture1D(boxDataTex,(startPixU+4.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+5.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+6.0)*invRes).rgb);

	return 1;
}

int getCylInfos (int item_idx,out vec3 cyl_center,out float cyl_radius, out float cyl_half_height,out mat3x3 cyl_inv_mat,out mat3x3 cyl_inv_norm_mat,out mat3x3 cyl_norm_mat)
{
	float			startPixU;
	startPixU		=	float(numboxes*3+num_cubes*7+num_spheres*10+(item_idx*13));

	cyl_center			=	texture1D(boxDataTex,(startPixU+0.0)*invRes).rgb;
	cyl_radius			=	texture1D(boxDataTex,(startPixU+1.0)*invRes).r;
	cyl_half_height		=	texture1D(boxDataTex,(startPixU+1.0)*invRes).g;

	cyl_inv_mat		=	mat3x3(texture1D(boxDataTex,(startPixU+ 4.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+ 5.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+ 6.0)*invRes).rgb);
	cyl_inv_norm_mat=	mat3x3(texture1D(boxDataTex,(startPixU+ 7.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+ 8.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+ 9.0)*invRes).rgb);
	cyl_norm_mat	=	mat3x3(texture1D(boxDataTex,(startPixU+10.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+11.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+12.0)*invRes).rgb);

	return 1;
}


vec3 getMatInfos(int item_ID)
{
	float	startPixU;
	int		item_idx,item_type;
	vec3	outMat;

	item_type	=	getItemType		(item_ID);
	item_idx	=	getItemIdx		(item_ID);

	switch(item_type)
	{
		case 1:
			startPixU		=	float(numboxes*3+num_cubes*7+(item_idx*10));
		break;
		case 2:
			startPixU		=	float(numboxes*3+(item_idx*7));
		break;
		case 3:
			startPixU		=	float(numboxes*3+num_cubes*7+num_spheres*10+(item_idx*13));
		break;

	}
	outMat	=	texture1D(boxDataTex,(startPixU+3.0)*invRes).rgb;
	return outMat;

}


vec3 project(vec3 A, vec3 B)
 {
	float dot1,dot2;
	float coef;

	dot1=dot(A,B);
	dot2=dot(B,B);
	coef=dot1/dot2;

	return(B*coef);
 }

int intersect_ray_box (vec3 min,vec3 max,vec3 ray_origin,vec3 ray_inv_direction,out vec2 t)
{
  float tmin, tmax, tymin, tymax, tzmin, tzmax;
  
  if(ray_inv_direction[0]>0.0)
  {
	tmin = ((min[0] - ray_origin[0]) * ray_inv_direction[0]);
	tmax = ((max[0] - ray_origin[0]) * ray_inv_direction[0]);
  }
  else
  {
	tmin = ((max[0]  - ray_origin[0]) * ray_inv_direction[0]);
	tmax = ((min[0]  - ray_origin[0]) * ray_inv_direction[0]);
  }

  if(ray_inv_direction[1]>0.0)
  {
	tymin = ((min[1] - ray_origin[1])   * ray_inv_direction[1]);
	tymax  =((max[1] - ray_origin[1]) * ray_inv_direction[1]);
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

  if(ray_inv_direction[2]>0.0)
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

	

int rayIntersectCube(vec3 center,vec3 size,vec3 raypos ,vec3 direction,mat3x3 mat,out vec3 NormVec,out vec2 uv_coord,out int plane_id,out float out_dist)
{
	int			n;
	int			col,ret;
	float		dist;
	vec3		raydir;
	vec3		rayori;
	vec3		rayidir;
	vec2		minmax;
	vec3		vec;
					
	dist		=	10000000.0;
	raypos		=	(raypos-center)	*	mat;
	raydir		=	direction		*	mat;
	rayidir		=	1.0/raydir;
	ret			=	0;
	
	if(intersect_ray_box	( -size,size,raypos,rayidir,minmax)==1)
	{
		float	distPlane,det_c;
		int		det;

		if(minmax[0]>0.0)
		{
			dist	=	minmax[0];
			vec		=	raypos+raydir*dist;

			for(n=0;n<6;n++)
			{
				switch(n)
				{
					case 0:det_c =	 size[2]; break;
					case 1:det_c =	 size[0]; break;
					case 2:det_c =	 size[2]; break;
					case 3:det_c =	 size[0]; break;
					case 4:det_c =	 size[1]; break;
					case 5:det_c =	 size[1]; break;
				}
				
				distPlane		=	dot(cube_normz[n],vec)-det_c;
				if(abs(distPlane)<5.0)
				{
					plane_id		=	n;
					NormVec			=	cube_normz[plane_id];
					out_dist		=	dist;
					ret				=	1;
				}
			}
		}
	}

	if(ret==1)
	{
		vec3	total;
		total=(vec+size)/(size*2.0);
		switch(plane_id)
		{
				//cube_normz[0]=vec3(0.0,0.0,1.0);
			case 0:
			case 2:
				uv_coord[0]		=	total[0];
				uv_coord[1]		=	total[1];
				
			break;
			case 1:
			case 3:
				//	cube_normz[1]=vec3(1.0,0.0,0.0);
				uv_coord[0]		=	total[1];
				uv_coord[1]		=	total[2];
			break;
			case 4:
			case 5:
				//	cube_normz[4]=vec3(0.0,1.0,0.0);
				uv_coord[0]		=	total[0];
				uv_coord[1]		=	total[2];
			break;
		}
	}
	return ret;
}



int Sphere_intersect(vec3 sphere_center,float sq_radius,mat3x3 sphere_inv_mat,vec3 raypos ,vec3 direction,out float d0,out float d1,out vec3 sphere_norm,out vec2 sphere_uv)
{
    //Compute A, B and C coefficients
	vec3 outPos,raydir,rayidir;
	float a;// = dot(direction, direction);
    float b;// = 2.0 * dot(direction,l);
    float c;// = dot(sphere_center,sphere_center) + dot(raypos,raypos) -2.0*dot(sphere_center,raypos) - sq_radius;
	
	raypos		=	(raypos-sphere_center)*sphere_inv_mat;//	*	mat;
	raydir		=	direction*sphere_inv_mat;//		*	mat;
	rayidir		=	1.0/raydir;

	a 			= dot(raydir, raydir);
    b 			= 2.0 * dot(raydir,raypos);
    c 			= dot(raypos,raypos) - sq_radius;

	
    //Compute A, B and C coefficients




    //Find discriminant
    float disc = b * b - 4.0 * a * c;
    
    // if discriminant is negative there are no real roots, so return 
    // false as ray misses sphere
    if (disc < 0.0)
        return 0;

    // compute q as described above
    float distSqrt = sqrt(disc);
    float q;
    if (b < 0.0)
        q = (-b - distSqrt)/(2.0*a);
    else
        q = (-b + distSqrt)/(2.0*a);


    // compute t0 and t1
    float t0 = q / a;
    float t1 = c / q;

    // make sure t0 is smaller than t1
    if (t0 > t1)
    {
        // if t0 is bigger than t1 swap them around
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // if t1 is less than zero, the object is in the ray's negative direction
    // and consequently the ray misses the sphere
    if (t1 < 0.0)
        return 0;
		

    // if t0 is less than zero, the intersection point is at t1
    if (t0 < 0.0)
    {
		d0=t1;
		d1=t0;
    }
    // else the intersection point is at t0
    else
    {
		d0=t0;
		d1=t1;
        
    }
	outPos			=	raypos+raydir*d0;
	sphere_norm		=	normalize	(outPos);

	sphere_uv[0]	=	asin(sphere_norm[0])/(PI) + 0.5;
	sphere_uv[1]	=	asin(sphere_norm[1])/(PI) + 0.5;

	return 1;
}


 int intcyl(vec3 raybase,vec3  raydir, float radius, out vec2 tVals)
 {
     int	 hit; // True if ray intersects cyl
     vec3    RC; // Ray base to cylinder base
     vec3    n, O;
	 float   d; // Shortest distance between the ray and the cylinder
     float	 t, s; // Distances along the ray
     float	 ln;


	 RC	  = raybase;
	
	 n	 = vec3(-raydir[2],0.0,raydir[0]);
     
     // Ray is parallel to the cylinder's axis.
     if ((ln = length(n)) == 0.0)
     {
		 d		=	dot(RC,RC);
         
         tVals[0] = -10000000000000000.0;
         tVals[1] = 10000000000000000.0;
         // True if ray is in cylinder.
		 if(d <= (radius*radius))
			return 1;
		 else
			 return 0;
     }
	 n*=1.0/ln;

     d	 = abs(dot(RC,n)); // Shortest distance.

	 if(d <= (radius))
		hit = 1;
	 else
		 hit = 0;

     // If ray hits cylinder.
     if (hit==1)
     {
		 O[0] = -RC[2];
		 O[1] =  0.0;
		 O[2] =  RC[0];

         t		  = -dot(O,n) / ln;

		 O[0] = -n[2];
		 O[1] =  0.0;
		 O[2] =  n[0];

		 O		  = normalize	(O);
         
         s		  = abs (sqrt(radius  - d * d)/dot(raydir,O));
         tVals[0] = t - s; // Entering distance.
         tVals[1] = t + s; // Exiting distance.
     }

     return hit;
 }

 int clipcyl(vec3 raybase,vec3 raydir,float half_height, inout vec2 tVals)
{
  float dc, dwb, dwt, tb, tt;
  float _in, _out; // Object intersection distances.

  _in  = tVals[0];
  _out = tVals[1];

  dc  = raydir[1];
  dwb = raybase[1] + half_height;
  dwt = raybase[1] - half_height;

  // Ray is parallel to the cylinder end-caps.
  if (dc == 0.0)
  {
      if (dwb <= 0.0)
          return 0;
      if (dwt >= 0.0)
          return 0;
  }
  else
  {
      // Intersect the ray with the bottom end-cap.
      tb = -dwb / dc;
      // Intersect the ray with the top end-cap.
      tt = -dwt / dc;

      // Bottom is near cap, top is far cap.
      if (dc >= 0.0)
      {
          if (tb > _out)
              return 0;
          if (tt < _in)
              return 0;
          if (tb > _in && tb < _out)
              _in = tb;
          if (tt > _in && tt < _out)
              _out = tt;
      }
      // Bottom is far cap, top is near cap.
      else
      {
          if (tb < _in)
              return 0;
          if (tt > _out)
              return 0;
          if (tb > _in && tb < _out)
              _out = tb;
          if (tt > _in && tt < _out)
              _in = tt;
      }
  }

  tVals[0] = _in;
  tVals[1] = _out;
  if(_in < _out)
	return 1;
  else
	return 0;
 }

 int intersectRayCyl(	vec3 ray_origin,vec3 ray_direction,vec3 cyl_center,float cyl_half_height,float cyl_radius,
						mat3x3 cyl_inv_matrix,mat3x3 cyl_inv_norm_mat,
						out vec2 tVals,out vec3 cylNorm,out vec2 uv_coord)
{
	vec3	raypos,raydir;
	vec3	A,C,V,P;

	raypos   = ray_origin	-cyl_center;
	raypos	*= cyl_inv_matrix;

	raydir   =ray_direction*cyl_inv_norm_mat;


	if (intcyl(raypos, raydir, cyl_radius, tVals)==0)
        return 0;

	
	
    if (clipcyl(raypos, raydir, cyl_half_height, tVals)==0)
        return 0;
	
	
	raypos			=	raypos+raydir*tVals[0];

	C[0]			=	0.0;
	C[1]			=	0.0;
	C[2]			=	0.0;

	A[0]			=	0.0;
	A[1]			=	1.0;
	A[2]			=	0.0;

	//V = X - C
	V				=	raypos-C;
	P				=	project		(V,A);
	cylNorm			=	normalize	(V-P);

	uv_coord[0]		=	asin(cylNorm[0])/PI + 0.5;
	uv_coord[1]		=	length(P)/cyl_half_height;

	return 1;

}





int castRaytoLight(vec3 ray_origin,vec3 lightPos,int CastId)
{
	vec2			minmax;
	vec3			ray_direction;
	vec3			ray_inv_direction;
	int				planeId;
	vec3			cubeNorm;
	int				intersect;


	ray_direction			=	normalize(lightPos-ray_origin);
	ray_inv_direction		=	1.0/ray_direction;
	

	intersect				=	0;

	for(int i=0;i<numboxes;i++)
	{
		int  item_idx,item_type,item_ID;

		getItemInfos	(i,item_type,item_idx,item_ID);

		if(CastId!=item_ID)
		{
			vec3	 min,max;
			getboxMinMax	(i,min,max);
			
			if(intersect_ray_box(min,max,ray_origin,ray_inv_direction,minmax)==1)
			{
				float			cube_dist;
				float			sphere_dist1,sphere_dist2;
				vec2			sphere_uv;
				vec3			sphere_norm;
				float			cyl_dist;
				float			startPixU;
				int				n;
				vec3			mat_1,mat_2,mat_3;


				if(minmax[1]>0.0)
				{
					switch(item_type)
					{
						case 1:
							vec3	sphere_center;
							float	sphere_radius;
							mat3x3		sphere_inv_mat;
							mat3x3		sphere_mat;
														
							getSphereInfos		(item_idx,sphere_center,sphere_radius,sphere_inv_mat,sphere_mat);
							intersect		=	Sphere_intersect	(sphere_center,sphere_radius,sphere_inv_mat,ray_origin,ray_direction,sphere_dist1,sphere_dist2,sphere_norm,sphere_uv);

						break;
						case 2:
							vec2	uv_coord;
							vec3	cube_center,cube_size;
							mat3x3	cube_mat;

							getCubeInfos		(item_idx,cube_center,cube_size,cube_mat);

							
							intersect		=	rayIntersectCube	(cube_center,cube_size,ray_origin,ray_direction,cube_mat,cubeNorm,uv_coord,planeId,cube_dist);
						break;
						case 3:
							vec3		cyl_center;
							vec3		cyl_norm;
							vec2		cyl_uv;
							vec2		cyl_dist;
							float		cyl_half_height;
							float		cyl_radius;
							mat3x3		cyl_inv_mat;
							mat3x3		cyl_inv_norm_mat;
							mat3x3		cyl_norm_mat;

							getCylInfos	  (item_idx,cyl_center,cyl_radius,cyl_half_height,cyl_inv_mat,cyl_inv_norm_mat,cyl_norm_mat);

							intersect	=	intersectRayCyl	(ray_origin,ray_direction,cyl_center,cyl_half_height,cyl_radius,cyl_inv_mat,cyl_inv_norm_mat,cyl_dist,cyl_norm,cyl_uv);
						break;
					}
					if(intersect==1)
						return 0;
				}
			}
		}
	}
	

	return 1;
}




int castRay(vec3 ray_origin,vec3 ray_direction,vec3 ray_inv_direction,out vec4 pix,out float ddlight,out vec3 vecPos,out vec3 normVec,inout int CastId )
{
	float			dist_final;
	vec2			minmax;
	float			lightDot;
	int				planeId;
	vec3			cubeNorm;
	int				inCastId;
	int				tid;
	int				nid;
	vec2			uv_coord;
	vec3			Reflect;
	int				col;

	inCastId	=   CastId;
	col			=	0;
	dist_final	=	 1000000000000000.0;

	for(int i=0;i<numboxes;i++)
	{
		int item_type;
		int item_idx;
		int item_ID;
		
		getItemInfos	(i,item_type,item_idx,item_ID);

		if(inCastId!=item_ID)
		{
			vec3	min,max;

			getboxMinMax(i,min,max);

			if(intersect_ray_box(min,max,ray_origin,ray_inv_direction,minmax)==1)
			{
				float	startPixU;
				vec3	mat_1,mat_2,mat_3;
				float	cube_dist;
				float	sphere_dist1,sphere_dist2;
				float	cyl_dist;
				int		n;
				
				
				if(minmax[1]>0.0)
				{
					switch(item_type)
					{
						case 1:
							int			sid;
							vec3		sphere_center;
							vec2		sphere_uv;
							float		sphere_radius;
							vec3		sphere_norm;
							mat3x3		sphere_inv_mat;
							mat3x3		sphere_mat;
														
													
							getSphereInfos		(item_idx,sphere_center,sphere_radius,sphere_inv_mat,sphere_mat);
							sp_center		=	sphere_center;

							if(Sphere_intersect	(sphere_center,sphere_radius,sphere_inv_mat,ray_origin,ray_direction,sphere_dist1,sphere_dist2,sphere_norm,sphere_uv)==1)
							{
								if(sphere_dist1<dist_final)
								{
									startPixU		=	float(numboxes*3+num_cubes*7+(item_idx*10));

									dist_final		=	sphere_dist1;
									vecPos			=	ray_origin+ray_direction*dist_final;

									col				=	1;
									CastId			=  item_ID;
									
									tid				=	int(texture1D(boxDataTex,(startPixU+2.0)*invRes).g);
									nid				=	int(texture1D(boxDataTex,(startPixU+2.0)*invRes).b);

									normVec			=	sphere_norm*sphere_mat;
									uv_coord		=	sphere_uv;
								}
							}
						break;
						case 2:
							vec3	cube_center,cube_size;
							vec2	cube_uv;
							mat3x3	cube_mat;

							getCubeInfos	(item_idx,cube_center,cube_size,cube_mat);
							
							if(rayIntersectCube	(cube_center,cube_size,ray_origin,ray_direction,cube_mat,cubeNorm,cube_uv,planeId,cube_dist)==1)
							{
								if(cube_dist<dist_final)
								{

									startPixU		=	float(numboxes*3+(item_idx*7));
						

									dist_final		=	cube_dist;
									vecPos			=	ray_origin+ray_direction*dist_final;
									normVec			=	normalize(cubeNorm)*cube_mat;
									col				=	1;
									CastId			=  item_ID;
									
									tid				=	int(texture1D(boxDataTex,(startPixU+2.0)*invRes).g);
									nid				=	int(texture1D(boxDataTex,(startPixU+2.0)*invRes).b);
									uv_coord		=	cube_uv;
								}
							}
						break;
						case 3:
							vec3		cyl_center;
							vec3		cyl_norm;
							vec2		cyl_uv;
							vec2		cyl_dist;
							float		cyl_half_height;
							float		cyl_radius;
							mat3x3		cyl_inv_mat;
							mat3x3		cyl_inv_norm_mat;
							mat3x3		cyl_norm_mat;


							getCylInfos	  (item_idx,cyl_center,cyl_radius,cyl_half_height,cyl_inv_mat,cyl_inv_norm_mat,cyl_norm_mat);
							
							if(intersectRayCyl	(ray_origin,ray_direction,cyl_center,cyl_half_height,cyl_radius,cyl_inv_mat,cyl_inv_norm_mat,cyl_dist,cyl_norm,cyl_uv)==1)
							{
								if(cyl_dist[0]<dist_final)
								{
									startPixU		=	float(numboxes*3+num_cubes*7+num_spheres*10+(item_idx*13));
									/*
									float		cyl_refract;
									float		cyl_reflect;
									cyl_refract		=	texture1D(boxDataTex,(startPixU+3.0)*invRes).r;
									cyl_reflect		=	texture1D(boxDataTex,(startPixU+3.0)*invRes).g;
									*/

									dist_final		=	cyl_dist[0];
									vecPos			=	ray_origin+ray_direction*dist_final;
									tid				=	int(texture1D(boxDataTex,(startPixU+2.0)*invRes).g);
									nid				=	int(texture1D(boxDataTex,(startPixU+2.0)*invRes).b);
									col				=	1;
									CastId			=  item_ID;
									
									normVec			=	cyl_norm*cyl_norm_mat;
									uv_coord		=	cyl_uv;
								}
							}
						break;
					}
				}
			}
		}
	}



	
	ddlight	=0.0;
	/*
	for(int halo_idx=0;halo_idx<num_halos;halo_idx++)
	*/
	{
		mat3x3		sphere_inv_mat;
		vec3		sphere_center;
		vec3		sphere_norm;
		vec2		sphere_uv;
		float		startPixU;
		float		sphere_radius;
		float		sphere_dist1,sphere_dist2;
		int			halo_idx;

		halo_idx		=	0;
		
		startPixU		=	float(numboxes*3+num_cubes*7+num_spheres*10+num_cyls*13+halo_idx*10);

		sphere_center	=		texture1D(boxDataTex,(startPixU+0.0)*invRes).rgb;
		sphere_radius	=		texture1D(boxDataTex,(startPixU+1.0)*invRes).r;
		sphere_inv_mat	=		mat3x3(texture1D(boxDataTex,(startPixU+4.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+ 5.0)*invRes).rgb,texture1D(boxDataTex,(startPixU+ 6.0)*invRes).rgb);
		
		
				
		if(Sphere_intersect(sphere_center,sphere_radius,sphere_inv_mat,ray_origin,ray_direction,sphere_dist1,sphere_dist2,sphere_norm,sphere_uv)==1)
		{
			float volume;
			if(sphere_dist1<dist_final)
			{
				volume		=	abs(sphere_dist2-sphere_dist1);
				ddlight		=	volume/150.0f;//powf(volume/100.0,100.0);

				if(ddlight<0.0)
					ddlight=0.0;
						
				if(ddlight>1.0)
					ddlight=1.0;
			}
		}
	}
	

	if(col==1)
	{
		vec3			lightVec;
		vec4			pixNorm;
		vec2			t_uv_coord;
		
		switch(tid)
		{
			case 1:
				t_uv_coord[0]=uv_coord[0]/2.0;
				t_uv_coord[1]=uv_coord[1]/2.0;
				pix			=   texture2D		(texture1,t_uv_coord);
			break;
			case 2:
				t_uv_coord[0]=uv_coord[0]/2.0+0.5;
				t_uv_coord[1]=uv_coord[1]/2.0;
				pix			=   texture2D		(texture1,t_uv_coord);
			break;
			case 3:
				t_uv_coord[0]=uv_coord[0]/2.0;
				t_uv_coord[1]=uv_coord[1]/2.0+0.5;
				pix			=   texture2D		(texture1,t_uv_coord);
			break;
			case 4:
				t_uv_coord[0]=uv_coord[0]/2.0+0.5;
				t_uv_coord[1]=uv_coord[1]/2.0+0.5;
				pix			=   texture2D		(texture1,t_uv_coord);
			break;
			default:pix			=   vec4			(1.0,1.0,1.0,1.0);break;
		}


		if(nid!=0)
		{
			vec3		axis_1,axis_2;
			vec3		newNorm,tNorm;
			vec2		n_uv_coord;
			vec3		c1,c2;
			vec3		binormal,tangent;
			vec3  	    test_norm;
			float		dist,plane_det;
			mat3x3		mat;

			switch(nid)
			{
				case 1:
					n_uv_coord[0]=uv_coord[0]/2.0;
					n_uv_coord[1]=uv_coord[1]/2.0;
					pixNorm			=   texture2D		(texture2,n_uv_coord);
				break;

				case 2:
					n_uv_coord[0]=uv_coord[0]/2.0+0.5;
					n_uv_coord[1]=uv_coord[1]/2.0;
					pixNorm			=   texture2D		(texture2,n_uv_coord);
				break;
				case 3:
					n_uv_coord[0]=uv_coord[0]/2.0;
					n_uv_coord[1]=uv_coord[1]/2.0+0.5;
					pixNorm			=   texture2D		(texture2,n_uv_coord);
				break;
				case 4:
					n_uv_coord[0]=uv_coord[0]/2.0+0.5;
					n_uv_coord[1]=uv_coord[1]/2.0+0.5;
					pixNorm			=   texture2D		(texture2,n_uv_coord);
				break;
				
			}
			tNorm[0]= ((pixNorm[0]-0.5))*2.0;
			tNorm[1]= ((pixNorm[1]-0.5))*2.0;
			tNorm[2]= ((pixNorm[2]-0.5))*2.0;
			tNorm	=	normalize(tNorm);



			plane_det		= -dot(normVec,vecPos);
			test_norm		=	vecPos;

			test_norm[0]	=	test_norm[0]+1.0;
			dist			=	dot(test_norm,normVec)+plane_det;

			if(abs(dist)<0.1)
			{
				test_norm		=	vecPos;
				test_norm[1]	=	test_norm[1]+1.0;
				dist			=	dot(test_norm,normVec)+plane_det;
			}

			tangent[0]		=	test_norm[0]-normVec[0]*dist;
			tangent[1]		=	test_norm[1]-normVec[1]*dist;
			tangent[2]		=	test_norm[2]-normVec[2]*dist;

			tangent			=	normalize(tangent);
			binormal	=	cross		(normVec, tangent ); 
			binormal	=	normalize	(binormal);

			mat			=	mat3x3(vec3(tangent[0],binormal[0],normVec[0]),vec3(tangent[1],binormal[1],normVec[1]),vec3(tangent[2],binormal[2],normVec[2]));
			newNorm		=	tNorm*mat;
			normVec		=	newNorm;
		
		}
		lightVec		=	lightPos-vecPos;
		lightVec		=	normalize	(lightVec);
		lightDot		=	abs(dot			(normVec,lightVec));
		//if(lightDot<0.0)lightDot=0.0;
		
		pix			*= lightDot;
	}
	else
	{
		pix			=	vec4	(textureCube(cubemap,ray_direction));
	}

	
	return col;
}

void main()
{
	vec4			pix;
	vec4			halopix;
	vec3			ray_origin;
	vec3			origin;
	vec3			ray_direction;
	vec3			ray_inv_direction;
	vec3			outPos;

	vec3			normVec;
	vec3			raypos,raydir;

	int				CastId;
	int				origId;
	int				n;


	
	box_dets[0]	=2;
	box_dets[1]	=0;
	box_dets[2]	=2;
	box_dets[3]	=0;
	box_dets[4]	=1;
	box_dets[5]	=1;
	
	/*
	box_ids[0]		=	ivec4(0,1,2,3);
	box_ids[1]		=	ivec4(1,5,6,2);
	box_ids[2]		=	ivec4(5,4,7,6);
	box_ids[3]		=	ivec4(4,0,3,7);
	box_ids[4]		=	ivec4(3,2,6,7);
	box_ids[5]		=	ivec4(1,0,4,5);

	cube_normz[0]	=	vec3(0.0,0.0,1.0);
	cube_normz[1]	=	vec3(1.0,0.0,0.0);
	cube_normz[2]	=	vec3(0.0,0.0,-1.0);
	cube_normz[3]	=	vec3(-1.0,0.0,0.0);
	cube_normz[4]	=	vec3(0.0,1.0,0.0);
	cube_normz[5]	=	vec3(0.0,-1.0,0.0);
	*/

	//winW		   = 1024.0;
	//winH		   = 768.0;


	origin[0]		=winW/2.0;
	origin[1]		=winH/2.0;
	origin[2]		=400.0;

	raypos[0]		=-((float(gl_FragCoord.x)-origin[0])/winW)*100.0;
	raypos[1]		=((float(gl_FragCoord.y)-origin[1])/winH)*100.0;
	raypos[2]		=0.0;

	raydir[0]		=-(float(gl_FragCoord.x)-origin[0]);
	raydir[1]		=float(gl_FragCoord.y)-origin[1];
	raydir[2]		=0.0-origin[2];
	
	ray_origin			= (raypos*cameraInvMat)+cameraPos;
	ray_direction		= normalize(raydir)*cameraInvNormal;

	ray_inv_direction	= 1.0/ray_direction;
	
	D3_EPSILON		=	0.000000000000001;
	origId			=	-1;

	float ddl;
	
	
	ddl=0.0;

	if(castRay(ray_origin,ray_direction,ray_inv_direction,pix,ddl,outPos,normVec,origId)==1)
	{
		vec3		reflectedVec,refractedVec;
		vec3		invReflect,invRefract;
		
		vec3		newOut,newNorm;
		vec4		reflPix,refrPix;
		vec4		reflHaloPix,refrHaloPix;
		vec4		newPix;
		vec4		pixNew;
		
		float		reflect_fac,refract_fac;
		float		shadow;
		float		light;
		float		len;
		float		nddl;
		vec3		Material;


		Material	=	getMatInfos(origId);

		
		reflect_fac	=Material.r;
		refract_fac	=Material.g;

		pixNew			=	vec4(0.0,0.0,0.0,1.0);
		
		reflectedVec	=	reflect			(ray_direction,normVec);
		refractedVec	=	refract			(ray_direction,normVec,1.0,0.8);
		
		invReflect		=	1.0/reflectedVec;
		invRefract		=	1.0/refractedVec;
		
		CastId			=	origId;
		castRay		(outPos,reflectedVec,invReflect,reflPix ,nddl,newOut,newNorm,CastId);
		reflHaloPix		=	vec4(0.0,nddl,0.0,0.0);
		
		
		CastId			=	origId;
		castRay		(outPos,refractedVec,invRefract,refrPix ,nddl,newOut,newNorm,CastId);
		refrHaloPix		=	vec4(0.0,nddl,0.0,0.0);		
		
		
		pixNew			=	mix		(pix,refrPix,refract_fac);
		pixNew			=	pixNew+reflPix*reflect_fac;
		
		shadow			=	1.0;

		if(castRaytoLight(outPos,lightPos,origId)==0)
			shadow*=0.5;

		pix			=pixNew*(shadow);
	}
	halopix	=	vec4(0.0,ddl,0.0,0.0);
	pix		+=	halopix;
	
    gl_FragColor = pix;
}