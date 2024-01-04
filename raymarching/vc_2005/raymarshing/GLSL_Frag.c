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
uniform int		numboxes;

//cubes
uniform int		num_cubes;

//sphere
uniform int		num_spheres;

//cylinder
uniform int		num_cyls;

float			PI = 3.14159265358979323846264;

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

int				boundIds[16];
int				n_bounds;
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
				det				=	box_dets[n];
				det_c			=	-size[det];
				distPlane		=	dot(cube_normz[n],vec)-det_c;
				if(abs(distPlane)<1.0)
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



float Sphere_intersect(vec3 sphere_center,float sq_radius,vec3 raypos ,vec3 direction)
{
	vec3 l;

	l=raypos-sphere_center;
    //Compute A, B and C coefficients
	float a = dot(direction, direction);
    float b = 2.0 * dot(direction,l);
    float c = dot(sphere_center,sphere_center) + dot(raypos,raypos) -2.0*dot(sphere_center,raypos) - sq_radius;

    //Find discriminant
    float disc = b * b - 4.0 * a * c;
    
    // if discriminant is negative there are no real roots, so return 
    // false as ray misses sphere
    if (disc < 0.0)
        return -1.0;

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
        return -1.0;
		

    // if t0 is less than zero, the intersection point is at t1
    if (t0 < 0.0)
    {
        return t1;
    }
    // else the intersection point is at t0
    else
    {
        return t0;
    }

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
         
         s		  = abs (sqrt(radius * radius - d * d)/dot(raydir,O));
         tVals[0] = t - s; // Entering distance.
         tVals[1] = t + s; // Exiting distance.
     }

     return hit;
 }

 int clipcyl(vec3 raybase,vec3 raydir,float height, out vec2 tVals)
{
  float dc, dwb, dwt, tb, tt;
  float _in, _out; // Object intersection distances.

  _in  = tVals[0];
  _out = tVals[1];

  dc  = raydir[1];
  dwb = raybase[1] ;
  dwt = raybase[1] - height;

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

 int intersectRayCyl(vec3 ray_origin,vec3 ray_direction,vec3 cyl_base,float cyl_height,float cyl_radius,
					mat3x3 cyl_inv_matrix,mat3x3 cyl_inv_norm_mat,
					out vec2 tVals,out vec3 cylNorm,out vec2 uv_coord)
{
	vec3	raypos,raydir;
	vec3	A,C,V,P;

	raypos   = ray_origin	-cyl_base;
	raypos	*= cyl_inv_matrix;

	raydir   =ray_direction*cyl_inv_norm_mat;

	if (intcyl(raypos, raydir, cyl_radius, tVals)==0)
        return 0;

    if (clipcyl(raypos, raydir, cyl_height, tVals)==0)
        return 0;

	
	raypos			=	raypos+raydir*tVals[0];

	C[0]			=	0.0;
	C[1]			=	cyl_height/2.0;
	C[2]			=	0.0;

	A[0]			=	0.0;
	A[1]			=	1.0;
	A[2]			=	0.0;

	//V = X - C
	V				=	raypos-C;
	P				=	project		(V,A);
	cylNorm			=	normalize	(V-P);

	uv_coord[0]		=	asin(cylNorm[0])/PI + 0.5;
	uv_coord[1]		=	length(P)/cyl_height;

	return 1;

}


void setBoundId	(int n,int i)
{
	switch(n)
	{
		case 0:boundIds[0]=i;break;
		case 1:boundIds[1]=i;break;
		case 2:boundIds[2]=i;break;
		case 3:boundIds[3]=i;break;
		case 4:boundIds[4]=i;break;
		case 5:boundIds[5]=i;break;
		case 6:boundIds[6]=i;break;
		case 7:boundIds[7]=i;break;
		case 8:boundIds[8]=i;break;
		case 9:boundIds[9]=i;break;
		case 10:boundIds[10]=i;break;
		case 11:boundIds[11]=i;break;
		case 12:boundIds[12]=i;break;
		case 13:boundIds[13]=i;break;
		case 14:boundIds[14]=i;break;
		case 15:boundIds[15]=i;break;
	}	
}

int getBoundId	(int n)
{
	int bid;
	switch(n)
	{
		case 0:bid=boundIds[0];break;
		case 1:bid=boundIds[1];break;
		case 2:bid=boundIds[2];break;
		case 3:bid=boundIds[3];break;
		case 4:bid=boundIds[4];break;
		case 5:bid=boundIds[5];break;
		case 6:bid=boundIds[6];break;
		case 7:bid=boundIds[7];break;
		case 8:bid=boundIds[8];break;
		case 9:bid=boundIds[9];break;
		case 10:bid=boundIds[10];break;
		case 11:bid=boundIds[11];break;
		case 12:bid=boundIds[12];break;
		case 13:bid=boundIds[13];break;
		case 15:bid=boundIds[15];break;
	}

	return bid;
}


int castRaytoLight(vec3 ray_origin,vec3 lightPos,int CastId)
{
	vec2			minmax;
	vec3			ray_direction;
	vec3			ray_inv_direction;
	
	int				planeId;
	vec3			cubeNorm;
	int				intersect;
	int				boundIds[16];
	int				n_bounds;
	int				col;

	ray_direction			=	normalize(lightPos-ray_origin);
	ray_inv_direction		=	1.0/ray_direction;
	

	intersect				=	0;
	n_bounds				=	0;

	for(int i=0;i<numboxes;i++)
	{
		if(CastId!=i)
		{
			vec3	min,max;
			vec4	p1,p2;
			float	startPixU;
			float	endPixU;
			float	invRes;

			invRes	  = 1.0/1024.0;
			startPixU =	float((i*2)+0)*invRes;
			endPixU =	float((i*2)+1)*invRes;

			min		  =	texture1D(boxDataTex,startPixU).rgb;
			max		  =	texture1D(boxDataTex,endPixU).rgb;

			if(intersect_ray_box(min,max,ray_origin,ray_inv_direction,minmax)==1)
			{
				if(minmax[1]>0.0)
				{
					intersect		=1;
					setBoundId		(n_bounds,i);
					n_bounds++;
				}
				
			}
		}
	}
	
	col		=0;
	CastId	=-1;

	
	if(intersect==1)
	{
		float	cube_dist;
		float	sphere_dist;
		float	cyl_dist;
		int		n;
		int		bid;
		n=0;
		while(n<n_bounds)
		{

			bid	=	getBoundId(n);


			for(int cube_idx=0;cube_idx<num_cubes;cube_idx++)
			{
				vec2	uv_coord;
				int		cid;
				vec3	cube_center,cube_size;
				float	startPixU;
				float	invRes,cube_tex_id;
				vec3	mat_1,mat_2,mat_3;
				mat3x3	cube_mat;

				invRes		=	1.0/1024.0;
				startPixU	=	float(numboxes*2+(cube_idx*6));


				cube_center	=	texture1D(boxDataTex,(startPixU+0.0)*invRes).rgb;
				cube_size	=	texture1D(boxDataTex,(startPixU+1.0)*invRes).rgb;
				cid			=	int(texture1D(boxDataTex,(startPixU+2.0)*invRes).r);
				cube_tex_id	=	texture1D(boxDataTex,(startPixU+2.0)*invRes).g;

				mat_1		=	texture1D(boxDataTex,(startPixU+3.0)*invRes).rgb;
				mat_2		=	texture1D(boxDataTex,(startPixU+4.0)*invRes).rgb;
				mat_3		=	texture1D(boxDataTex,(startPixU+5.0)*invRes).rgb;

				cube_mat	=	mat3x3(mat_1,mat_2,mat_3);


				//cid		=	int(cube_ids[cube_idx]);
				if(cid==bid)
				{
					cube_dist				=	1000000000000000.0;
					if(rayIntersectCube	(cube_center,cube_size,ray_origin,ray_direction,cube_mat,cubeNorm,uv_coord,planeId,cube_dist)==1)
					{
						return 1;
					}
				}
			}
			
			for(int cyl_idx=0;cyl_idx<num_cyls;cyl_idx++)
			{
				int			cid;
				vec3		cyl_base;
				vec3		cyl_norm;
				vec2		cyl_uv;
				vec2		cyl_dist;
				float		cyl_height;
				float		startPixU;
				float		cyl_radius;
				float		invRes;
				vec3		mat_1,mat_2,mat_3;
				mat3x3		cyl_inv_mat;
				mat3x3		cyl_inv_norm_mat;
				mat3x3		cyl_norm_mat;


								invRes			=	1.0/1024.0;
				startPixU		=	float(numboxes*2+num_cubes*6+num_spheres*2+(cyl_idx*12));

				cyl_base		=		texture1D(boxDataTex,(startPixU+0.0)*invRes).rgb;
				cid			    =	int(texture1D(boxDataTex,(startPixU+1.0)*invRes).r);
				cyl_radius		=		texture1D(boxDataTex,(startPixU+1.0)*invRes).g;
				cyl_height		=		texture1D(boxDataTex,(startPixU+1.0)*invRes).b;

				mat_1			=	texture1D(boxDataTex,(startPixU+3.0)*invRes).rgb;
				mat_2			=	texture1D(boxDataTex,(startPixU+4.0)*invRes).rgb;
				mat_3			=	texture1D(boxDataTex,(startPixU+5.0)*invRes).rgb;
				cyl_inv_mat		=	mat3x3(mat_1,mat_2,mat_3);

				mat_1			=	texture1D(boxDataTex,(startPixU+6.0)*invRes).rgb;
				mat_2			=	texture1D(boxDataTex,(startPixU+7.0)*invRes).rgb;
				mat_3			=	texture1D(boxDataTex,(startPixU+8.0)*invRes).rgb;
				cyl_inv_norm_mat=	mat3x3(mat_1,mat_2,mat_3);

				mat_1			=	texture1D(boxDataTex,(startPixU+8.0)*invRes).rgb;
				mat_2			=	texture1D(boxDataTex,(startPixU+10.0)*invRes).rgb;
				mat_3			=	texture1D(boxDataTex,(startPixU+11.0)*invRes).rgb;
				cyl_norm_mat	=	mat3x3(mat_1,mat_2,mat_3);

				if(cid==bid)
				{
					if(intersectRayCyl	(ray_origin,ray_direction,cyl_base,cyl_height,cyl_radius,cyl_inv_mat,cyl_inv_norm_mat,cyl_dist,cyl_norm,cyl_uv)==1)
					{
						return 1;
					}
				}
			}

			for(int sphere_idx=0;sphere_idx<num_spheres;sphere_idx++)
			{
				int		sid;
				vec3	sphere_center;
				float	startPixU;
				float	sphere_radius;
				float	invRes;

				invRes			=	1.0/1024.0;
				startPixU		=	float(numboxes*2+num_cubes*6+(sphere_idx*2));

				sphere_center	=		texture1D(boxDataTex,(startPixU+0.0)*invRes).rgb;
				sid			    =	int(texture1D(boxDataTex,(startPixU+1.0)*invRes).r);
				sphere_radius	=		texture1D(boxDataTex,(startPixU+1.0)*invRes).g;

				
				if(sid==bid)
				{
					sphere_dist	= Sphere_intersect	(sphere_center,sphere_radius,ray_origin,ray_direction);
					if(sphere_dist!=-1.0)
					{
						sphere_dist=sphere_dist;
						return 1;
					}
				}
			}
			n++;
		}
	}


	return 0;
}




int castRay(vec3 ray_origin,vec3 ray_direction,vec3 ray_inv_direction,out vec4 pix,out vec3 vecPos,out vec3 normVec,inout int CastId )
{
	float			dist_final;
	vec2			minmax;
	float			lightDot;
	int				planeId;
	vec3			cubeNorm;
	int				intersect;
	int				tid;
	vec2			uv_coord;
	vec3			Reflect;
	int				col;

	intersect				=	0;
	n_bounds				=	0;

	for(int i=0;i<numboxes;i++)
	{
		if(CastId!=i)
		{
			vec3	min,max;
			vec4	p1,p2;
			float	startPixU;
			float	endPixU;
			float	invRes;

			invRes	  = 1.0/1024.0;
			startPixU =	float((i*2)+0)*invRes;
			endPixU =	float((i*2)+1)*invRes;

			min		  =	texture1D(boxDataTex,startPixU).rgb;
			max		  =	texture1D(boxDataTex,endPixU).rgb;

			if(intersect_ray_box(min,max,ray_origin,ray_inv_direction,minmax)==1)
			{
				//if((minmax[0]>0.0)&&(minmax[0]<minmax[1]))
				if(minmax[1]>0.0)
				{
					intersect			=1;
					setBoundId		(n_bounds,i);
					n_bounds++;
				}
			}
		}
	}
	
	col		=0;
	CastId	=-1;

	
	if(intersect==1)
	{
		float	cube_dist;
		float	sphere_dist;
		float	cyl_dist;
		int		n;
		int		bid;
		n=0;

		dist_final				=	 1000000000000000.0;


		while(n<n_bounds)
		{
			bid	=	getBoundId(n);

			for(int cube_idx=0;cube_idx<num_cubes;cube_idx++)
			{
				int		cid;
				vec3	cube_center,cube_size;
				vec2	cube_uv;
				float	startPixU;
				float	invRes;
				float	cube_tex_id;
				vec3	mat_1,mat_2,mat_3;
				mat3x3	cube_mat;


				invRes		=	1.0/1024.0;
				startPixU	=	float(numboxes*2+(cube_idx*6));


				cube_center	=	texture1D(boxDataTex,(startPixU+0.0)*invRes).rgb;
				cube_size	=	texture1D(boxDataTex,(startPixU+1.0)*invRes).rgb;
				cid			=	int(texture1D(boxDataTex,(startPixU+2.0)*invRes).r);
				cube_tex_id	=	texture1D(boxDataTex,(startPixU+2.0)*invRes).g;

				mat_1		=	texture1D(boxDataTex,(startPixU+3.0)*invRes).rgb;
				mat_2		=	texture1D(boxDataTex,(startPixU+4.0)*invRes).rgb;
				mat_3		=	texture1D(boxDataTex,(startPixU+5.0)*invRes).rgb;

				cube_mat	=	mat3x3(mat_1,mat_2,mat_3);

				if(cid==bid)
				{
					
					//cube_dist				=	minmax[0];
					
					cube_dist				=	1000000000000000.0;
					if(rayIntersectCube	(cube_center,cube_size,ray_origin,ray_direction,cube_mat,cubeNorm,cube_uv,planeId,cube_dist)==1)
					{
						if(cube_dist<dist_final)
						{
							dist_final		=	cube_dist;
							vecPos			=	ray_origin+ray_direction*dist_final;
							normVec			=	normalize(cubeNorm)*cube_mat;
							col				=	1;
							CastId			=	bid;
							tid				=	int(cube_tex_id);
							uv_coord		=	cube_uv;
						}
					}
				}
			}

			
			for(int cyl_idx=0;cyl_idx<num_cyls;cyl_idx++)
			{
				int			cid;
				vec3		cyl_base;
				vec3		cyl_norm;
				vec2		cyl_uv;
				vec2		cyl_dist;
				float		cyl_refract;
				float		cyl_reflect;
				float		cyl_tid;
				float		cyl_height;
				float		startPixU;
				float		cyl_radius;
				float		invRes;
				vec3		mat_1,mat_2,mat_3;
				mat3x3		cyl_inv_mat;
				mat3x3		cyl_inv_norm_mat;
				mat3x3		cyl_norm_mat;


				invRes			=	1.0/1024.0;
				startPixU		=	float(numboxes*2+num_cubes*6+num_spheres*2+(cyl_idx*12));

				cyl_base		=		texture1D(boxDataTex,(startPixU+0.0)*invRes).rgb;
				cid			    =	int(texture1D(boxDataTex,(startPixU+1.0)*invRes).r);
				cyl_radius		=		texture1D(boxDataTex,(startPixU+1.0)*invRes).g;
				cyl_height		=		texture1D(boxDataTex,(startPixU+1.0)*invRes).b;

				cyl_tid		    =	   (texture1D(boxDataTex,(startPixU+2.0)*invRes).r);
				cyl_refract		=		texture1D(boxDataTex,(startPixU+2.0)*invRes).g;
				cyl_reflect		=		texture1D(boxDataTex,(startPixU+2.0)*invRes).b;

				

				mat_1			=	texture1D(boxDataTex,(startPixU+3.0)*invRes).rgb;
				mat_2			=	texture1D(boxDataTex,(startPixU+4.0)*invRes).rgb;
				mat_3			=	texture1D(boxDataTex,(startPixU+5.0)*invRes).rgb;
				cyl_inv_mat		=	mat3x3(mat_1,mat_2,mat_3);

				mat_1			=	texture1D(boxDataTex,(startPixU+6.0)*invRes).rgb;
				mat_2			=	texture1D(boxDataTex,(startPixU+7.0)*invRes).rgb;
				mat_3			=	texture1D(boxDataTex,(startPixU+8.0)*invRes).rgb;
				cyl_inv_norm_mat=	mat3x3(mat_1,mat_2,mat_3);

				mat_1			=	texture1D(boxDataTex,(startPixU+9.0)*invRes).rgb;
				mat_2			=	texture1D(boxDataTex,(startPixU+10.0)*invRes).rgb;
				mat_3			=	texture1D(boxDataTex,(startPixU+11.0)*invRes).rgb;
				cyl_norm_mat	=	mat3x3(mat_1,mat_2,mat_3);

				
				if(cid==bid)
				{

					if(intersectRayCyl	(ray_origin,ray_direction,cyl_base,cyl_height,cyl_radius,cyl_inv_mat,cyl_inv_norm_mat,cyl_dist,cyl_norm,cyl_uv)==1)
					{
						if(cyl_dist[0]<dist_final)
						{
							
							dist_final		=	cyl_dist[0];
							vecPos			=	ray_origin+ray_direction*dist_final;
							tid				=	int(cyl_tid);
							col				=	1;
							CastId			=	bid;
							normVec			=	cyl_norm*cyl_norm_mat;
							uv_coord		=	cyl_uv;
						}
					}
				}
			}

			for(int sphere_idx=0;sphere_idx<num_spheres;sphere_idx++)
			{
				int			sid;
				vec3		sphere_center;
				float		startPixU;
				float		sphere_radius;
				float		invRes;

				invRes			=	1.0/1024.0;
				startPixU		=	float(numboxes*2+num_cubes*6+(sphere_idx*2));

				sphere_center	=		texture1D(boxDataTex,(startPixU+0.0)*invRes).rgb;
				sid			    =	int(texture1D(boxDataTex,(startPixU+1.0)*invRes).r);
				sphere_radius	=		texture1D(boxDataTex,(startPixU+1.0)*invRes).g;
				
				sp_center		=	sphere_center;
				
				if(sid==bid)
				{
					sphere_dist	= Sphere_intersect	(sphere_center,sphere_radius,ray_origin,ray_direction);
					if(sphere_dist!=-1.0)
					{
						sphere_dist=sphere_dist;
						
						if(sphere_dist<dist_final)
						{
							dist_final		=	sphere_dist;
							col				=	1;
							CastId			=	bid;
							tid				=	0;

							vecPos			=	ray_origin+ray_direction*dist_final;
							normVec			=	vecPos-sphere_center;
							normVec			=	normalize		(normVec);
						}
						
					}
				}
			}
			n++;
		}
	}

	if(col==1)
	{
		vec3			lightVec;
		switch(tid)
		{
			case 1:
				uv_coord[0]=uv_coord[0]/2.0;
				uv_coord[1]=uv_coord[1]/2.0;
				pix			=   texture2D		(texture1,uv_coord);
			break;
			case 2:
				uv_coord[0]=uv_coord[0]/2.0+0.5;
				uv_coord[1]=uv_coord[1]/2.0;
				pix			=   texture2D		(texture1,uv_coord);
			break;
			case 3:
				uv_coord[0]=uv_coord[0]/2.0;
				uv_coord[1]=uv_coord[1]/2.0+0.5;
				pix			=   texture2D		(texture1,uv_coord);
			break;

			default:pix			=   vec4			(1.0,1.0,1.0,1.0);break;
		}

		
		lightVec		=	lightPos-vecPos;
		lightVec		=	normalize	(lightVec);
		lightDot		=	dot			(normVec,lightVec);
		if(lightDot<0.0)lightDot=0.0;
		
		pix			*=lightDot;
		
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

	raypos[0]		=((float(gl_FragCoord.x)-winW/2.0)/winW)*100.0;
	raypos[1]		=((float(gl_FragCoord.y)-winH/2.0)/winH)*100.0;
	raypos[2]		=0.0;

	raydir[0]		=float(gl_FragCoord.x)-origin[0];
	raydir[1]		=float(gl_FragCoord.y)-origin[1];
	raydir[2]		=0.0-origin[2];
	
	ray_origin			= (raypos*cameraInvMat)+cameraPos;
	ray_direction		= normalize(raydir)*cameraInvNormal;

	ray_inv_direction	= 1.0/ray_direction;
	
	D3_EPSILON		=	0.000000000000001;
	origId			=	255;

	if(castRay(ray_origin,ray_direction,ray_inv_direction,pix,outPos,normVec,origId)==1)
	{
		vec3		reflectedVec,refractedVec;
		vec3		invReflect,invRefract;
		
		vec3		newOut,newNorm;
		vec4		reflPix,refrPix;
		vec4		newPix;
		vec4		pixNew;
		
		float		reflect_fac,refract_fac;
		float		shadow;
		float		light;
		float		len;
		
		reflect_fac	=0.2;
		refract_fac	=0.4;


		reflectedVec	=	reflect			(ray_direction,normVec);
		refractedVec	=	refract			(ray_direction,normVec,1.0,0.8);
		
		invReflect		=	1.0/reflectedVec;
		invRefract		=	1.0/refractedVec;

		
		CastId			=	origId;
		castRay		(outPos,reflectedVec,invReflect,reflPix ,newOut,newNorm,CastId);

		
		CastId			=	origId;
		castRay		(outPos,refractedVec,invRefract,refrPix ,newOut,newNorm,CastId);
		
		pixNew		=vec4(0.0,0.0,0.0,1.0);

		pixNew[0]	=pixNew[0]*(1.0-reflect_fac)+reflPix[0]*reflect_fac;
		pixNew[1]	=pixNew[1]*(1.0-reflect_fac)+reflPix[1]*reflect_fac;
		pixNew[2]	=pixNew[2]*(1.0-reflect_fac)+reflPix[2]*reflect_fac;

		pixNew[0]	=pixNew[0]*(1.0-refract_fac)+refrPix[0]*refract_fac;
		pixNew[1]	=pixNew[1]*(1.0-refract_fac)+refrPix[1]*refract_fac;
		pixNew[2]	=pixNew[2]*(1.0-refract_fac)+refrPix[2]*refract_fac;		

		shadow		=1.0;

		if(castRaytoLight(outPos,lightPos,origId)==1)
			shadow*=0.5;


		pix			=pix*(shadow)+pixNew;

	}
	

    gl_FragColor = pix;


}


