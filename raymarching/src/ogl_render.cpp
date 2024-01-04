
#include	"../libC/libc_math.h"
#include	"../libC/libc_file.h"
#include	"../libC/cpu_infos.h"
#include	"../libray/vec_types.h"
#include	"../libray/ray_float.h"
#include	"../libray/ray_int.h"
#include	"../libray/mat3.h"
#include	"../libray/geom_f.h"


#include	<windows.h>	
#include	<gl\gl.h>	
#include	<gl\glext_4.h>
#include	"ogl_render.h"

GLenum cube_map_target[6] = {           
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB
};


GLuint		COGLRenderer::vertexShaderObject;
GLuint		COGLRenderer::fragmentShaderObject;
GLuint		COGLRenderer::ProgramObject;
GLcharARB  *COGLRenderer::VertexShaderSource;
GLcharARB  *COGLRenderer::FragmentShaderSource;
int			COGLRenderer::vlength;
int			COGLRenderer::flength;
unsigned int	COGLRenderer::winH;
unsigned int	COGLRenderer::winW;

bool		COGLRenderer::fullscreen=false;
HDC			COGLRenderer::hDC		=NULL;		// Private GDI Device Context
HGLRC		COGLRenderer::hRC		=NULL;		// Permanent Rendering Context
HWND		COGLRenderer::hWnd		=NULL;		// Holds Our Window Handle
HINSTANCE	COGLRenderer::hInstance	=NULL;		// Holds The Instance Of The Application

 PFNGLCREATEPROGRAMOBJECTARBPROC  		COGLRenderer::glCreateProgramObjectARB;
 PFNGLDELETEOBJECTARBPROC         		COGLRenderer::glDeleteObjectARB;
 PFNGLGETPROGRAMIVPROC					COGLRenderer::glGetProgramiv;
 PFNGLGETPROGRAMBINARYPROC				COGLRenderer::glGetProgramBinary;
 PFNGLPROGRAMBINARYPROC					COGLRenderer::glProgramBinary;
 PFNGLPROGRAMPARAMETERIARBPROC			COGLRenderer::glProgramParameteriARB;

 PFNGLUSEPROGRAMOBJECTARBPROC     		COGLRenderer::glUseProgramObjectARB;
 PFNGLCREATESHADEROBJECTARBPROC   		COGLRenderer::glCreateShaderObjectARB;
 PFNGLSHADERSOURCEARBPROC         		COGLRenderer::glShaderSourceARB;
 PFNGLCOMPILESHADERARBPROC        		COGLRenderer::glCompileShaderARB;
 PFNGLGETOBJECTPARAMETERIVARBPROC 		COGLRenderer::glGetObjectParameterivARB;
 PFNGLATTACHOBJECTARBPROC         		COGLRenderer::glAttachObjectARB;
 PFNGLGETINFOLOGARBPROC           		COGLRenderer::glGetInfoLogARB;
 PFNGLLINKPROGRAMARBPROC          		COGLRenderer::glLinkProgramARB;
 PFNGLGETUNIFORMLOCATIONARBPROC   		COGLRenderer::glGetUniformLocationARB ;
 PFNGLUNIFORM4FARBPROC            		COGLRenderer::glUniform4fARB;
 PFNGLUNIFORM3FARBPROC            		COGLRenderer::glUniform3fARB;
 PFNGLUNIFORM3FVARBPROC           		COGLRenderer::glUniform3fvARB;
 PFNGLUNIFORM1FARBPROC            		COGLRenderer::glUniform1fARB;
 PFNGLUNIFORM1IARBPROC            		COGLRenderer::glUniform1iARB;
 PFNGLUNIFORM1IVARBPROC           		COGLRenderer::glUniform1ivARB;
 PFNGLUNIFORM1FVARBPROC           		COGLRenderer::glUniform1fvARB;
 PFNGLGETSHADERIVPROC			 		COGLRenderer::glGetShaderiv;
 PFNGLGETSHADERINFOLOGPROC		 		COGLRenderer::glGetShaderInfoLog;
 PFNGLUNIFORMMATRIX3FVPROC		 		COGLRenderer::glUniformMatrix3fv;
 PFNGLUNIFORMMATRIX4FVPROC		 		COGLRenderer::glUniformMatrix4fv;
 PFNGLGETATTRIBLOCATIONARBPROC			COGLRenderer::glGetAttribLocationARB;
 PFNGLVERTEXATTRIB3FARBPROC				COGLRenderer::glVertexAttrib3fARB;
 PFNGLVERTEXATTRIBPOINTERARBPROC		COGLRenderer::glVertexAttribPointerARB;
 PFNGLENABLEVERTEXATTRIBARRAYARBPROC	COGLRenderer::glEnableVertexAttribArrayARB ;
 PFNGLDISABLEVERTEXATTRIBARRAYARBPROC	COGLRenderer::glDisableVertexAttribArrayARB;
 PFNGLACTIVETEXTUREARBPROC				COGLRenderer::glActiveTextureARB;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc


GLvoid COGLRenderer::KillGLWindow()								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL COGLRenderer::CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	
	resizeGL		(width,height);					// Set Up Our Perspective GL Screen
	initExtensions	();


	return TRUE;									// Success
}


void COGLRenderer::Swap()
{
	SwapBuffers(hDC);
}
  int  COGLRenderer::initExtensions()
  {
 	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
 	glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");
 	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
 	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
 	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB"); 
 	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB"); 
 	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB"); 
 	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB"); 
 	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
 	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");

	glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	 	
 	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
 	glUniform4fARB  = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
 	glUniform3fARB  = (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
	glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC)wglGetProcAddress("glUniform3fvARB");

 	glUniform1fARB = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
 	glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC)wglGetProcAddress("glUniform1ivARB");
	glUniform1fvARB= (PFNGLUNIFORM1FVARBPROC)wglGetProcAddress("glUniform1fvARB");
	glUniformMatrix3fv	=(PFNGLUNIFORMMATRIX3FVPROC	)wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv	=(PFNGLUNIFORMMATRIX4FVPROC	)wglGetProcAddress("glUniformMatrix4fv");
	
	
	
 
 	glGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC)wglGetProcAddress("glGetAttribLocationARB");
 	glVertexAttrib3fARB = (PFNGLVERTEXATTRIB3FARBPROC)wglGetProcAddress("glVertexAttrib3fARB");
 	
 	glVertexAttribPointerARB=(PFNGLVERTEXATTRIBPOINTERARBPROC)wglGetProcAddress("glVertexAttribPointerARB");
 	glEnableVertexAttribArrayARB=(PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glEnableVertexAttribArrayARB");
 	glDisableVertexAttribArrayARB=( PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glDisableVertexAttribArrayARB");

 	glGetShaderiv				=(PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
 	glGetShaderInfoLog			=(PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");

	glGetProgramiv				=(PFNGLGETPROGRAMIVPROC	)wglGetProcAddress("glGetProgramiv");

	glGetProgramBinary			=(PFNGLGETPROGRAMBINARYPROC)wglGetProcAddress("glGetProgramBinary");
	glProgramBinary				=(PFNGLPROGRAMBINARYPROC)wglGetProcAddress("glProgramBinary");
	glProgramParameteriARB		=(PFNGLPROGRAMPARAMETERIARBPROC)wglGetProcAddress("glProgramParameteriARB");

	ProgramObject		=0;


	return 1;
 }

  int  COGLRenderer::resizeGL(int width,int height)
  {
  	glViewport(0,0,width,height);						// Reset The Current Viewport
	winW	=	width;
	winH	=	height;

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	glOrtho			(-1,1,-1,1,0,1000);
	glMatrixMode	(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity	();	
	return 1;
  }


  int  COGLRenderer::imageToOpenGL(image_t *tex,unsigned char *glTex,int texRes,int ofsX,int ofsY)
 {
	int ppX,ppY;	
	unsigned char *texPix;

	texPix=tex->pixels;
		
	ppY=0;
	while(ppY<512)
	{
		ppX=0;
		while(ppX<512*3)
		{
			int pp,pp2;
			
			pp	=((ppY+ofsY)*texRes*3+ppX+ofsX*3);
			pp2	=((511-ppY)*512*3)+ppX;

			glTex[pp+0]=texPix[pp2+2];
			glTex[pp+1]=texPix[pp2+1];
			glTex[pp+2]=texPix[pp2+0];
			ppX+=3;
		}
		ppY++;
	}

	return 0;
}
int COGLRenderer::createCubeMap(unsigned int w,unsigned int h,unsigned int pixType,unsigned int dataType,image_t *images)
{
	unsigned char *glTex;
	unsigned int cube_map_texture_ID;

	glTex=NULL;

	if((pixType==GL_RGB)&&(dataType==GL_UNSIGNED_BYTE))
	{
		glTex		=(unsigned char *)aligned_alloc(w*h*3);
	}

	if(glTex==NULL)
		return 0;
	
	glGenTextures(1, &cube_map_texture_ID);
	// Configuration de la texture
	
	glBindTexture	(GL_TEXTURE_CUBE_MAP_ARB, cube_map_texture_ID);
	for (int i = 0; i < 6; i++)
	{
		imageToOpenGL	(&images[i],glTex,w,0,0);
		glTexImage2D	(cube_map_target[i], 0, 3, w, h, 0, pixType, dataType, glTex);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, 0);

	return cube_map_texture_ID;
}

int COGLRenderer::createTexture(unsigned int w,unsigned int h,unsigned int pixType,unsigned int dataType,void *data )
{
	unsigned int texId;

	glGenTextures(1, &texId);					// Create The Texture
	
	if(h==0)
	{
		glBindTexture	(GL_TEXTURE_1D, texId);
		glTexImage1D	(GL_TEXTURE_1D, 0, GL_RGBA16F_ARB , w, 0, pixType, dataType, data);
	
		glTexParameteri	(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri	(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri	(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri	(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glBindTexture	(GL_TEXTURE_1D, 0);
		glDisable		(GL_TEXTURE_1D);

	}
	else
	{
		glBindTexture	(GL_TEXTURE_2D, texId);
		glTexImage2D	(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, pixType, dataType, data);
		glTexParameteri	(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri	(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri	(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri	(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glBindTexture	(GL_TEXTURE_2D, 0);
	}

	return texId;
}

int COGLRenderer::loadTexData(unsigned int texId, unsigned int w,unsigned int h,unsigned int pixType,unsigned int dataType,void *data )
{

	glBindTexture			(GL_TEXTURE_2D, texId);
	glTexSubImage2D			(GL_TEXTURE_2D,0,0,0,w,h,pixType,dataType,data);
	glBindTexture			(GL_TEXTURE_2D, 0);

	return 1;
}




int  COGLRenderer::save_shader_prog(GLuint ProgramObject,const char *file_path,unsigned int *binary_format)
{
	int			   binary_length;
	GLsizei		   program_length;
	unsigned char *program_data;

	COGLRenderer::glGetProgramiv		(ProgramObject,GL_PROGRAM_BINARY_LENGTH,&binary_length);
	if(binary_length>0)
	{
		if(COGLRenderer::glGetProgramBinary!=NULL)
		{
			program_data		=(unsigned char *)aligned_alloc(binary_length);
			COGLRenderer::glGetProgramBinary	(ProgramObject,binary_length,&program_length,binary_format,program_data);
		
			if(program_length>0)
				libc_writefile		(file_path,program_data,binary_length);

			aligned_free		(program_data);
			return 1;
		}
	}

	return 0;
}

GLuint  COGLRenderer::load_shader_prog(const char *file_path,unsigned int binary_format)
{
	void		   *program_data;
	int				binary_length;
	GLuint			ProgramObject;

	binary_length=libc_readfile		(file_path,&program_data);
	if(binary_length<=0)return 0;

	ProgramObject	= 0;

	if(COGLRenderer::glProgramBinary!=NULL)
	{
		ProgramObject	= COGLRenderer::glCreateProgramObjectARB();
		COGLRenderer::glProgramBinary	(ProgramObject, binary_format, program_data, binary_length);
	}

	return ProgramObject;

}
int COGLRenderer::loadFullShader(char *vertSource,char *fragSource)
{
	unsigned int binary_format;
	GLint		compiledv,compiledf;
	GLint		maxLength = 0;
	GLcharARB  *infoLog;

	/*
	if(libc_read_uint("shader_prog_glsl.fmt",&binary_format)>0)
	{
		ProgramObject=load_shader_prog("shader_prog_glsl.bin",binary_format);
		if(ProgramObject>0)return 1;
	}
	*/
				

	vlength=libc_readfile(vertSource,(void **)&VertexShaderSource);
	if(vlength<=0)return 0;	
	
	flength=libc_readfile(fragSource,(void **)&FragmentShaderSource);
	if(flength<=0)return 0;


	vertexShaderObject   = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fragmentShaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

	glShaderSourceARB(vertexShaderObject, 1, &VertexShaderSource, &vlength);
	glShaderSourceARB(fragmentShaderObject, 1, &FragmentShaderSource, &flength);

	glCompileShaderARB(vertexShaderObject);
	glCompileShaderARB(fragmentShaderObject);

	glGetObjectParameterivARB(vertexShaderObject, GL_COMPILE_STATUS, &compiledv);
	glGetObjectParameterivARB(fragmentShaderObject, GL_COMPILE_STATUS, &compiledf);
	
	glGetShaderiv		(fragmentShaderObject, GL_INFO_LOG_LENGTH, &maxLength);
 
	//The maxLength includes the NULL character
	infoLog	=	(GLcharARB  *)aligned_alloc(maxLength+1);
	glGetShaderInfoLog	(fragmentShaderObject, maxLength, &maxLength, infoLog);

	if (compiledv&&compiledf)
	{
			unsigned int binary_format;

			ProgramObject = glCreateProgramObjectARB();
			
			glAttachObjectARB		(ProgramObject, vertexShaderObject);
			glAttachObjectARB		(ProgramObject, fragmentShaderObject);
			
			glProgramParameteriARB	(ProgramObject, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
			glLinkProgramARB		(ProgramObject); 

			if(glGetProgramiv!=NULL)
			{
				save_shader_prog	(ProgramObject,"shader_prog_glsl.bin",&binary_format);
				libc_writefile		("shader_prog_glsl.fmt",&binary_format,4);
			}
		
			//printf(infoLog);
			return 1;
		// yes it compiled!
	}

	else
	{
		char *tt;
		tt=infoLog;
		//We don't need the shader anymore.
		//glDeleteShader(fragmentShaderObject);
		//printf(infoLog);
	}
	aligned_free(infoLog);

	return 0;

}



void COGLRenderer::copyBoxesToTex(CScene *Scene,unsigned int boxDataTexId)
{
	int		n;
	vec3f_t	vec;
	char	UniformName[255];
	float	data;
	GLuint		BoxDataID;
	


	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cubemap");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,0);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"boxDataTex");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,1);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"texture1");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,2);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"texture2");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,3);


	
	data		=	winW;
 	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"winW");
	if(BoxDataID>=0)
		glUniform1fvARB	(BoxDataID,1,&data);

	data		=	winH;
	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"winH");
	if(BoxDataID>=0)
		glUniform1fvARB	(BoxDataID,1,&data);


	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"lightPos");
	if(BoxDataID>=0)
		glUniform3fvARB	(BoxDataID,1,Scene->lightPos);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cameraInvNormal");
	if(BoxDataID>=0)
	{
		float m[9];
		Scene->getCameraInvNormMat_ogl(m);
		glUniformMatrix3fv	(BoxDataID,1,1,m);
	}


	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cameraInvMat");
	if(BoxDataID>=0)
	{
		float m[9];
		Scene->getCameraInvMat_ogl(m);
		glUniformMatrix3fv	(BoxDataID,1,1,m);
	}

	
	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cameraPos");
	if(BoxDataID>=0)
		glUniform3fvARB	(BoxDataID,1,Scene->camPos);


	n=0;
	while(n<6)
	{
		vec3f_t cnorm;
		get_cube_norm(n,cnorm);

		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cube_normz[0]");
		if(BoxDataID>=0)
			glUniform3fvARB	(BoxDataID+n,1,cnorm);
		n++;
	}

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"numboxes");
	glUniform1iARB	(BoxDataID,Scene->n_boxf);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"num_cubes");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,Scene->CubefList->num_item);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"num_spheres");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,Scene->SpherefList->num_item);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"num_cyls");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,Scene->CylfList->num_item);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"num_halos");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,Scene->HalofList->num_item);

	

	Scene->getGeomData	();

	glEnable			(GL_TEXTURE_1D);
	glBindTexture		(GL_TEXTURE_1D, boxDataTexId);
	glTexSubImage1D		(GL_TEXTURE_1D,0,0,2048,GL_RGB,GL_FLOAT,Scene->boxData	);
	glBindTexture		(GL_TEXTURE_1D, 0);
	glDisable			(GL_TEXTURE_1D);


}



void COGLRenderer::copyBoxesToUniform(CScene *Scene)
{
	int		n;
	vec3f_t	vec;
	char	UniformName[255];
	float	data;
	GLuint		BoxDataID;

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cubemap");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,0);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"texture1");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,1);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"texture2");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,2);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"boxDataTex");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,3);

	

	
	data		=	winW;
 	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"winW");
	if(BoxDataID>=0)
		glUniform1fvARB	(BoxDataID,1,&data);

	data		=	winH;
	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"winH");
	if(BoxDataID>=0)
		glUniform1fvARB	(BoxDataID,1,&data);


	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"lightPos");
	if(BoxDataID>=0)
		glUniform3fvARB	(BoxDataID,1,Scene->lightPos);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cameraInvNormal");
	if(BoxDataID>=0)
		glUniformMatrix3fv	(BoxDataID,1,1,Scene->inv_matrice_normales);


	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cameraInvMat");
	if(BoxDataID>=0)
		glUniformMatrix3fv	(BoxDataID,1,1,Scene->inv_matrice);

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cameraPos");
	if(BoxDataID>=0)
		glUniform3fvARB	(BoxDataID,1,Scene->camPos);


	n=0;
	while(n<6)
	{
		vec3f_t cnorm;
		get_cube_norm(n,cnorm);
		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cube_normz[0]");
		if(BoxDataID>=0)
			glUniform3fvARB	(BoxDataID+n,1,cnorm);
		n++;
	}

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"numboxes");
	glUniform1iARB	(BoxDataID,Scene->n_boxf);


	n=0;
	while(n<Scene->n_boxf)
	{
		
		vec[0]		=	Scene->box_testf[n]._parameters[0];
		vec[1]		=	Scene->box_testf[n]._parameters[1];
		vec[2]		=	Scene->box_testf[n]._parameters[4];

		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"minboxData[0]");
		glUniform3fARB	(BoxDataID+n*2+0,vec[0],vec[1],vec[2]);
		
		vec[0]		=	Scene->box_testf[n]._parameters[2];
		vec[1]		=	Scene->box_testf[n]._parameters[3];
 		vec[2]		=	Scene->box_testf[n]._parameters[5];

		glUniform3fARB	(BoxDataID+n*2+1,vec[0],vec[1],vec[2]);
		
		n++;
	}
	
	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"num_cubes");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,Scene->CubefList->num_item);
	
	
	n=0;
	while(n<Scene->CubefList->num_item)
	{
		cubef_t	*cube;

		cube		=	Scene->CubefList->getItemAt(n);

		
		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cube_sizes[0]");
		if(BoxDataID>=0)
		{
			glUniform3fvARB	(BoxDataID+n*2+0,1,cube->center);
			glUniform3fvARB	(BoxDataID+n*2+1,1,cube->size);
		}
		
		data		=	cube->AABOX.cast_id-1;
		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cube_ids[0]");
		if(BoxDataID>=0)
			glUniform1fvARB		(BoxDataID+n,1,&data);


		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cube_mat[0]");
		if(BoxDataID>=0)
		{
			glUniformMatrix3fv (BoxDataID+n,1,1,cube->matrix);
		}

		data		=	cube->material.tid;
		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cube_tex[0]");
		if(BoxDataID>=0)
		{
			glUniform1fvARB	(BoxDataID+n,1,&data);
		}
		
		n++;
	}
	



	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"num_spheres");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,Scene->SpherefList->num_item);
	


	n=0;
	while(n<Scene->SpherefList->num_item)
	{
		spheref_t	*sphere;

		sphere					=	Scene->SpherefList->getItemAt(n);

		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"sphere_centers[0]");
		if(BoxDataID>=0)
		{
			glUniform3fvARB	(BoxDataID+n,1,sphere->center);
		}
		
		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"sphere_sqradius[0]");
		if(BoxDataID>=0)
			glUniform1fvARB		(BoxDataID+n,1,&sphere->sq_radius);


		data		=	sphere->AABOX.cast_id-1;
		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"sphere_ids[0]");
		if(BoxDataID>=0)
		glUniform1fvARB		(BoxDataID+n,1,&data);
		
		n++;
	}

	BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"num_cyls");
	if(BoxDataID>=0)
		glUniform1iARB	(BoxDataID,Scene->CylfList->num_item);
	
	n=0;
	while(n<Scene->CylfList->num_item)
	{
		cylf_t	*cyl;

		cyl		=	Scene->CylfList->getItemAt(n);

		/*
		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cyl_pt1[0]");
		if(BoxDataID>=0)
		{
			glUniform3fvARB	(BoxDataID+n,1,cyl->pt1);
		}

		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cyl_pt2[0]");
		if(BoxDataID>=0)
		{
			glUniform3fvARB	(BoxDataID+n,1,cyl->pt2);
		}
		*/
		
		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cyl_radius[0]");
		if(BoxDataID>=0)
			glUniform1fvARB		(BoxDataID+n,1,&cyl->radius);


		data		=	cyl->AABOX.cast_id-1;
		BoxDataID	=	glGetUniformLocationARB	(ProgramObject,"cyl_ids[0]");
		if(BoxDataID>=0)
			glUniform1fvARB		(BoxDataID+n,1,&data);
		
		

		n++;
	}
}


void COGLRenderer::bindTextures(GLuint cube_map_texture_ID,GLuint boxDataTexId,GLuint glTexId,GLuint glNormId)
{
	glActiveTextureARB	(GL_TEXTURE0);
	glEnable			(GL_TEXTURE_2D);
	glEnable			(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture		(GL_TEXTURE_CUBE_MAP_ARB, cube_map_texture_ID);

	glActiveTextureARB	(GL_TEXTURE1);
	glEnable			(GL_TEXTURE_1D);
	glBindTexture		(GL_TEXTURE_1D, boxDataTexId);

	glActiveTextureARB	(GL_TEXTURE2);
	glEnable			(GL_TEXTURE_2D);
	glBindTexture		(GL_TEXTURE_2D, glTexId);

	glActiveTextureARB	(GL_TEXTURE3);
	glEnable			(GL_TEXTURE_2D);
	glBindTexture		(GL_TEXTURE_2D, glNormId);

	
}
void COGLRenderer::RenderQuad(unsigned int texId)
{
	glLoadIdentity		();						// Reset The Current Modelview Matrix
	glClear				(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glColor3f			(1,1,1);


	if(ProgramObject)
	{
		glUseProgramObjectARB	(ProgramObject);
	}
	else
	{
		glActiveTextureARB	(GL_TEXTURE0);
		glEnable			(GL_TEXTURE_2D);
		glBindTexture		(GL_TEXTURE_2D, texId);
	}
	
	
	

	glBegin	(GL_QUADS);
	
	glTexCoord2f(0,0);
	glVertex2f(-1.0f,-1.0f);
	
	glTexCoord2f(0,winH/1024.0f);
	glVertex2f(-1.0f,1.0f);
	
	glTexCoord2f(winW/1024.0f,winH/1024.0f);
	glVertex2f(1.0f,1.0f);
	
	glTexCoord2f(winW/1024.0f,0);
	glVertex2f(1.0f,-1.0f);
	glEnd();
}