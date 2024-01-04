
class COGLRenderer
{
public:
	static GLuint	vertexShaderObject, fragmentShaderObject,ProgramObject ;
	static unsigned int	winW,winH;
	static GLcharARB  *VertexShaderSource;
	static GLcharARB  *FragmentShaderSource;
	static int		  vlength;
	static int		  flength;
	static bool		  fullscreen;	// Fullscreen Flag Set To Fullscreen Mode By Default
	static HDC			hDC;		// Private GDI Device Context
	static HGLRC		hRC;		// Permanent Rendering Context
	static HWND			hWnd;		// Holds Our Window Handle
	static HINSTANCE	hInstance;		// Holds The Instance Of The Application

	static BOOL				CreateGLWindow		(char* title, int width, int height, int bits, bool fullscreenflag);
	static GLvoid			KillGLWindow		();
	static int 				resizeGL			(int width,int height);
	static void				Swap				();
	static int 				createTexture		(unsigned int w,unsigned int h,unsigned int pixType,unsigned int dataType,void *data );
	static int 				createCubeMap		(unsigned int w,unsigned int h,unsigned int pixType,unsigned int dataType,image_t *images);
	static int 				imageToOpenGL		(image_t *tex,unsigned char *glTex,int texRes=512,int ofsX=0,int ofsY=0);

	static void				unloadTextures		();

	static void				copyBoxesToTex		(CScene *Scenee,unsigned int boxDataTexId);
	static void				copyBoxesToUniform	(CScene *Scene);
	static int				initExtensions		();
	static int				loadTexData			(GLuint texId, unsigned int w,unsigned int h,unsigned int pixType,unsigned int dataType,void *data );
	static int				loadFullShader		(char *vertSource,char *fragSource);
	static int				save_shader_prog	(GLuint ProgramObject,const char *file_path,unsigned int *binary_format);
	static GLuint			load_shader_prog	(const char *file_path,unsigned int binary_format);


	static void				bindTextures		(GLuint cube_map_texture_ID,GLuint boxDataTexId,GLuint glTexId,GLuint glNormId);
	static void				RenderQuad			(GLuint texId);

	 static PFNGLCREATEPROGRAMOBJECTARBPROC  		glCreateProgramObjectARB;
	 static PFNGLDELETEOBJECTARBPROC         		glDeleteObjectARB;
	 static PFNGLGETPROGRAMIVPROC					glGetProgramiv;
	 static PFNGLGETPROGRAMBINARYPROC				glGetProgramBinary;
	 static PFNGLPROGRAMBINARYPROC					glProgramBinary;
	 static PFNGLPROGRAMPARAMETERIARBPROC			glProgramParameteriARB;
	 static PFNGLUSEPROGRAMOBJECTARBPROC     		glUseProgramObjectARB;
	 static PFNGLCREATESHADEROBJECTARBPROC   		glCreateShaderObjectARB;
	 static PFNGLSHADERSOURCEARBPROC         		glShaderSourceARB;
	 static PFNGLCOMPILESHADERARBPROC        		glCompileShaderARB;
	 static PFNGLGETOBJECTPARAMETERIVARBPROC 		glGetObjectParameterivARB;
	 static PFNGLATTACHOBJECTARBPROC         		glAttachObjectARB;
	 static PFNGLGETINFOLOGARBPROC           		glGetInfoLogARB;
	 static PFNGLLINKPROGRAMARBPROC          		glLinkProgramARB;
	 static PFNGLGETUNIFORMLOCATIONARBPROC   		glGetUniformLocationARB ;
	 static PFNGLUNIFORM4FARBPROC            		glUniform4fARB;
	 static PFNGLUNIFORM3FARBPROC            		glUniform3fARB;
	 static PFNGLUNIFORM3FVARBPROC           		glUniform3fvARB;
	 static PFNGLUNIFORM1FARBPROC            		glUniform1fARB;
	 static PFNGLUNIFORM1IARBPROC            		glUniform1iARB;
	 static PFNGLUNIFORM1IVARBPROC           		glUniform1ivARB;
	 static PFNGLUNIFORM1FVARBPROC           		glUniform1fvARB;
	 static PFNGLGETSHADERIVPROC			 		glGetShaderiv;
	 static PFNGLGETSHADERINFOLOGPROC		 		glGetShaderInfoLog;
	 static PFNGLUNIFORMMATRIX3FVPROC		 		glUniformMatrix3fv;
	 static PFNGLUNIFORMMATRIX4FVPROC		 		glUniformMatrix4fv;
	 static PFNGLGETATTRIBLOCATIONARBPROC			glGetAttribLocationARB;
	 static PFNGLVERTEXATTRIB3FARBPROC				glVertexAttrib3fARB;
	 static PFNGLVERTEXATTRIBPOINTERARBPROC			glVertexAttribPointerARB;
	 static PFNGLENABLEVERTEXATTRIBARRAYARBPROC		glEnableVertexAttribArrayARB ;
	 static PFNGLDISABLEVERTEXATTRIBARRAYARBPROC	glDisableVertexAttribArrayARB;
	 static PFNGLACTIVETEXTUREARBPROC				glActiveTextureARB;
};


