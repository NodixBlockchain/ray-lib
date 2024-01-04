#include	"../libC/libc_math.h"
#include	"../libC/cpu_infos.h"

#include	"../libray/vec_types.h"
#include	"../libray/ray_int.h"
#include	"../libray/ray_float.h"
#include	"../libray/mat3.h"
#include	"../libray/geom_f.h"

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glext_4.h>		// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <time.h>		// Header File For Windows
#include <btBulletDynamicsCommon.h>		// Header File For Windows

#include "physics.h"
#include "ogl_render.h"





unsigned int		cur_tex;
POINT				Curs;
POINT				LastCurs;
unsigned int		lastCursSet;
	// Génération d'une texture CubeMap


bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default




cubef_t			*cubef;
cubef_t			*groundf;
spheref_t		*spheref;
cylf_t			*cylf;
btRigidBody		*boneBody=NULL;

GLuint			glTexId;
GLuint			glNormId;
GLuint			texId;
GLuint			cube_map_texture_ID;
GLuint			boxDataTexId;

thread_info_t	thread[16];
unsigned int	n_threads;


CScene	 *Scene;
CPhysics *Physic;

float motorAngle;


double gettime()
{
	return GetTickCount();
}


DWORD WINAPI doCasts(LPVOID tp)
{
	thread_info_t	*t;
	
	t			=	(thread_info_t	*)tp;
	

	while(t->threadSync!=0)
	{
		WaitForSingleObject		(t->EventStart,INFINITE);
		Scene->doCasts			(t->new_ray,t,t->sX,t->sY,t->eX,t->eY);
		SetEvent				(t->EventFinish);
	}
	return NULL;
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{

	unsigned int n,slice_thread;
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}
	// Reset The Modelview Matrix

	
	Scene->Resize			(width,height);

	slice_thread		=Scene->targetImage.h/n_threads;
	n			=0;
	while(n<n_threads)
	{
		WaitForSingleObject		(thread[n].EventFinish,1000);
		thread[n].sX				=	0;
		thread[n].eX				=	Scene->targetImage.w;

		thread[n].sY				=	n*slice_thread;
		thread[n].eY				=	(n+1)*slice_thread;


		
		n++;
	}
	
}


int InitGL()										// All Setup For OpenGL Goes Here
{

	ReSizeGLScene				(COGLRenderer::winW, COGLRenderer::winH);
	lastCursSet						=0;

	COGLRenderer::loadFullShader	("GLSL_Vert.c","GLSL_Frag.c");


	glShadeModel	(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor	(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth	(1.0f);									// Depth Buffer Setup
	glEnable		(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc		(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint			(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	//target texture for software rendering
	texId				=	COGLRenderer::createTexture(1024,1024,GL_RGB,GL_FLOAT,NULL);

	if(COGLRenderer::ProgramObject)
	{
		//texture for shader geometry infos
		boxDataTexId		=	COGLRenderer::createTexture(2048,0,GL_RGB,GL_FLOAT,Scene->boxData);

		//cube map texture
		cube_map_texture_ID	=	COGLRenderer::createCubeMap (512,512,GL_RGB,GL_UNSIGNED_BYTE,&Scene->textures[0]);
		
		//other textures
		unsigned char *glTex;
		glTex=(unsigned char *)aligned_alloc(1024*1024*3);
		COGLRenderer::imageToOpenGL	(&Scene->textures[6],glTex,1024,0	,0);
		COGLRenderer::imageToOpenGL	(&Scene->textures[7],glTex,1024,512	,0);
		COGLRenderer::imageToOpenGL	(&Scene->textures[8],glTex,1024,0	,512);
		glTexId=COGLRenderer::createTexture(1024,1024,GL_RGB,GL_UNSIGNED_BYTE,glTex);

		COGLRenderer::imageToOpenGL				(&Scene->textures[9],glTex,1024,0	,512);
		glNormId=COGLRenderer::createTexture	(1024,1024,GL_RGB,GL_UNSIGNED_BYTE,glTex);


		aligned_free(glTex);
	}

	motorAngle	=	0;

	groundf	=	Scene->newCubef		(480,-150,400,1000,50,1000);
	Scene->add_boxf					(&groundf->AABOX);
	Scene->setCubeMaterialf			(groundf,7	,2,0.0,0.3);
	

	cubef	=	Scene->newCubef		(480+250,-50,200,50,50,50);
	Scene->add_boxf					(&cubef->AABOX);
	Scene->setCubeMaterialf			(cubef	,6	,1,0.0,0.0);
	Physic->add_cube						(cubef	,btScalar(0.0f));

	cubef	=	Scene->newCubef		(480,10,200,300,10,50);
	Scene->add_boxf					(&cubef->AABOX);
	Scene->setCubeMaterialf			(cubef	,6	,1,0.0,0.0);
	Physic->add_cube						(cubef	,btScalar(0.0f));
	
	cubef	=	Scene->newCubef		(480-250,-50,200,50,50,50);
	Scene->add_boxf					(&cubef->AABOX);
	Scene->setCubeMaterialf			(cubef	,6	,1,0.0,0.0);
	Physic->add_cube						(cubef	,btScalar(0.0f));
	
	spheref	=	Scene->newSpheref	(480-250,30,200,10.0);
	Scene->add_boxf					(&spheref->AABOX);
	Scene->setSphereMaterialf		(spheref	,NULL			,0,0.8,0.2);
	Physic->add_sphere						(spheref	,btScalar(10.0f));

	spheref	=	Scene->newSpheref	(480-150,70,160,50.0);
	Scene->add_boxf					(&spheref->AABOX);
	Scene->setSphereMaterialf		(spheref	,8			,3,0.8,0.5);
	Scene->setSphereNormalMap		(spheref	,9,3);
	Physic->add_sphere						(spheref	,btScalar(10.0f));
	
	spheref	=	Scene->newSpheref	(680+150,50,160,30.0);
	Scene->add_boxf					(&spheref->AABOX);
	Scene->setSphereMaterialf		(spheref	,NULL			,0,0.8,1.0);
	Physic->add_sphere						(spheref	,btScalar(15.0f));

	spheref	=	Scene->newSpheref	(480+250,30,200,10.0);
	Scene->add_boxf					(&spheref->AABOX);
	Scene->setSphereMaterialf		(spheref	,NULL			,0,0.8,0.2);
	Physic->add_sphere						(spheref	,btScalar(10.0f));

	cubef	=	Scene->newCubef		(480-50,80,160,20,60,20);
	Scene->add_boxf					(&cubef->AABOX);
	Scene->setCubeMaterialf			(cubef	,6	,1,0.5,0.2);
	Physic->add_cube						(cubef	,btScalar(10.0f));



	cylf	=	Scene->newCylinderf	(480,-150,500,250,150.0);
	Scene->add_boxf					(&cylf->AABOX);
	Scene->setCylMaterialf			(cylf	,8	,3,0.0,0.5);
	Scene->setCylNormalMap			(cylf	,9	,3);
	Physic->add_cylinder					(cylf,15);


	cylf	=	Scene->newCylinderf	(80,-150,500,250,150.0);
	Scene->add_boxf					(&cylf->AABOX);
	Scene->setCylMaterialf			(cylf	,8	,3,0.0,0.66);
	Physic->add_cylinder					(cylf,15);
	

	cylf	=	Scene->newCylinderf	(880,-150,500,250,150.0);
	Scene->add_boxf					(&cylf->AABOX);
	Scene->setCylMaterialf			(cylf	,8	,3,0.5,0.5);
	Physic->add_cylinder					(cylf,15);
	

	//---
	cubef	=	Scene->newCubef		(580,800,200,60,30,20);
	Scene->add_boxf					(&cubef->AABOX);
	Scene->setCubeMaterialf			(cubef	,6	,1,0.0,0.1);
	Physic->add_cube						(cubef	,btScalar(10.0f));

	cubef	=	Scene->newCubef		(180,800,200,60,20,60);
	Scene->add_boxf					(&cubef->AABOX);
	Scene->setCubeMaterialf			(cubef	,6	,1,0.0,0.1);
	Physic->add_cube						(cubef	,btScalar(10.0f));

	cubef	=	Scene->newCubef		(380,800,200,60,60,30);
	Scene->add_boxf					(&cubef->AABOX);
	Scene->setCubeMaterialf			(cubef	,6	,1,0.0,0.1);
	Physic->add_cube						(cubef	,btScalar(10.0f));


	//---
	cubef	=	Scene->newCubef		(80,800,500,80,70,90);
	Scene->add_boxf					(&cubef->AABOX);
	Scene->setCubeMaterialf			(cubef	,6	,1,0.0,0.1);
	Physic->add_cube						(cubef	,btScalar(160.0f));

	cubef	=	Scene->newCubef		(480,1000,500,60,90,50);
	Scene->add_boxf					(&cubef->AABOX);
	Scene->setCubeMaterialf			(cubef	,6	,1,0.0,0.1);
	Physic->add_cube						(cubef	,btScalar(160.0f));

	cubef	=	Scene->newCubef		(880,1480,500,70,60,80);
	Scene->add_boxf					(&cubef->AABOX);
	Scene->setCubeMaterialf			(cubef	,6	,1,0.0,0.1);
	Physic->add_cube						(cubef	,btScalar(160.0f));


	spheref	=	Scene->newHalof		(480-150,240,800,150);
	Scene->setSphereMaterialf		(spheref,0,0,0.1,0.1);



	Physic->add_cube						(groundf,btScalar(.0f));
	

	
	boneBody=Physic->add_bones				(btVector3(0,800,0),Scene);

	
	

	return TRUE;										// Initialization Went OK
}



int DrawGLScene()									// Here's Where We Do All The Drawing
{
	int i;
	vec3f_t upVec={0.0,1.0,0.0};
	vec3f_t	origin;

		
	if(Scene->start_time==0)
	{
		Scene->start_time=gettime();
		Scene->cur_time=0;
	}
	else
	{
		Scene->cur_time=gettime()-Scene->start_time;
	}
	//cur_time=frame*100;

	Physic->syncScene		(Scene);
	
	

	Scene->lightPos[0]		=400.0f;
	Scene->lightPos[1]		=400.0f;
	Scene->lightPos[2]		=0.0f;

	
	spheref					=Scene->HalofList->getItemAt(0);
	Scene->setSphereCenterf	(spheref	,Scene->lightPos[0],Scene->lightPos[1],Scene->lightPos[2]);
	

		
	if(Physic->getBonePos(origin))
	{
		mat3x3_lookat(Scene->camPos,origin,upVec,Scene->matrice.m);
	}

	Scene->compute_camera	();

	
	if(COGLRenderer::ProgramObject)
	{
		COGLRenderer::copyBoxesToTex	(Scene,boxDataTexId);
		COGLRenderer::bindTextures		(cube_map_texture_ID,boxDataTexId,glTexId,glNormId);
	}
	else
	{
		HANDLE		Events[16];
		int			n;
		n=0;
		while(n<n_threads)
		{
			Events[n]	=thread[n].EventFinish;
			SetEvent	(thread[n].EventStart);
			n++;
		}
		WaitForMultipleObjects		(n_threads,Events,TRUE,INFINITE);
		COGLRenderer::loadTexData	(texId,Scene->targetImage.w,Scene->targetImage.h,GL_RGB,GL_UNSIGNED_BYTE,Scene->targetImage.pixels);
	}

	COGLRenderer::RenderQuad		(texId);
	COGLRenderer::Swap			();				// Swap Buffers (Double Buffering)
	Scene->frame++;

	
	
	return TRUE;										// Everything Went OK
}
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}
		case WM_MOUSEMOVE:
						
			GetCursorPos		(&Curs);



		break;
		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			COGLRenderer::resizeGL		(LOWORD(lParam),HIWORD(lParam));
			ReSizeGLScene				(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


								
void *operator new (size_t size)
{
	return aligned_alloc(size);
}						

void operator delete (void  *ptr)
{
	return aligned_free(ptr);
}
/*
extern "C"
{
	int _fltused=1;
}

int WINAPI WinMainCRTStartup(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State

*/

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;						// Bool Variable To Exit Loop
	unsigned int n;

	
	init_vec_funcs			();
	init_vec_funcsf			();
	init_mat3_fncs			();
	init_geometry			();
	

	Scene				=	new CScene();
	Physic				=	new CPhysics();

	Physic->initPhysic		();

	Scene->read_texture("textures/RIGHT.bmp",0);
	Scene->read_texture("textures/LEFT.bmp",1);
	Scene->read_texture("textures/DOWN.bmp",2);
	Scene->read_texture("textures/UP.bmp",3);
	Scene->read_texture("textures/FOR.bmp",4);
	Scene->read_texture("textures/BACK.bmp",5);
	Scene->read_texture("textures/WOOD.bmp",6);
	Scene->read_texture("textures/MARBLE.bmp",7);
	Scene->read_texture("textures/METAL.bmp",8);
	Scene->read_texture("textures/METAL_NRM.bmp",9);
	
	

	n=0;
	while(n<16)
	{
		thread[n].threadSync		 =	3;
		thread[n].reflectRay		 =	(rayi_t *)aligned_alloc(sizeof(rayi_t));
		thread[n].reflectRayf		 =  (rayf_t *)aligned_alloc(sizeof(rayf_t));
		thread[n].trans_ray_origin	 = (float*)aligned_alloc(sizeof(vec3f_t));
		thread[n].trans_ray_direction= (float*)aligned_alloc(sizeof(vec3f_t));
		thread[n].tmpNorm			 = (float*)aligned_alloc(sizeof(vec3f_t));
		thread[n].new_ray			 =	(rayf_t			*)aligned_alloc(sizeof(rayf_t));

		thread[n].EventStart		=	CreateEvent(NULL,  FALSE,FALSE,NULL);
		thread[n].EventFinish		=	CreateEvent(NULL,  FALSE,FALSE,NULL);
		
		ResetEvent					(thread[n].EventStart);
		SetEvent					(thread[n].EventFinish);
		n++;
	}
	n_threads	=	8;
	n=0;
	while(n<n_threads)
	{
		thread[n].TH				=	CreateThread(NULL,1000,doCasts,(LPVOID )&thread[n],0,&thread[n].TID);
		SetThreadPriority	(thread[n].TH,THREAD_PRIORITY_TIME_CRITICAL);
		n++;
	}


	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		COGLRenderer::fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!COGLRenderer::CreateGLWindow("NeHe's OpenGL Framework",640,480,16,COGLRenderer::fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}
	
	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		COGLRenderer::KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	
	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				float		angle;
				
				btVector3	new_vel;
				btVector3	forward (0.0f ,0.0f	, 1.0f);
				btVector3	backward(0.0f ,0.0f	,-1.0f);
				vec3f_t		angle_mat;
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					
				}
				
				
				
			

				if(keys[VK_DOWN])
				{
					new_vel=boneBody->getWorldTransform().getBasis()*forward;
					new_vel[1]=0;
					boneBody->setLinearVelocity	(new_vel*-400.0f);
				}

				if(keys[VK_UP])
				{
					new_vel=boneBody->getWorldTransform().getBasis()*forward;
					new_vel[1]=0;
					boneBody->setLinearVelocity	(new_vel*400.0f);

				}

				if(keys[VK_RIGHT])
				{
					boneBody->setAngularVelocity(btVector3(0,20.0,0));
					boneBody->setDamping(0,0.9);
				}

				if(keys[VK_LEFT])
				{
					boneBody->setAngularVelocity(btVector3(0,-20.0,0));
					boneBody->setDamping(0,0.9);
				}

				

				
				//add_vec3	(Scene->camPos,new_vel,Scene->camPos);
				
				
				/*
				if(lastCursSet==1)
				{
					Scene->camAngle[0]+=(Curs.x-LastCurs.x)/50.0f;
					Scene->camAngle[1]+=(Curs.y-LastCurs.y)/50.0f;
					Scene->camAngle[2] =0;
				}
				lastCursSet		= 1;
				LastCurs.x		= Curs.x;
				LastCurs.y		= Curs.y;
				*/

			}



			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				COGLRenderer::KillGLWindow();						// Kill Our Current Window
				COGLRenderer::fullscreen=!COGLRenderer::fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!COGLRenderer::CreateGLWindow("NeHe's OpenGL Framework",640,480,16,COGLRenderer::fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	COGLRenderer::KillGLWindow	();					// Kill The Window
	ExitProcess					(msg.wParam);
	
	
	return (msg.wParam);							// Exit The Program


}

//



