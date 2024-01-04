#include <windows.h>


void *aligned_alloc(unsigned int size)
{
	void			*ptr;
	unsigned int	iptr,aptr;
	
	ptr		=	HeapAlloc	(GetProcessHeap(),HEAP_ZERO_MEMORY,size+32);
	iptr	=	(unsigned long)(ptr);
	aptr	=	(iptr&0xFFFFFFF0)+32;
	
	*((unsigned int *)(aptr-4))=iptr;
	return ((void *)(aptr));
}


void aligned_free(void	 *ptr)
{
	unsigned int	iptr,a_ptr;
	
	a_ptr	=	(unsigned long)(ptr);
	iptr	=  *((unsigned int *)(a_ptr-4));

	HeapFree(GetProcessHeap(),0,(void *)iptr);
	
}

void *_intel_fast_memcpy(void *dst,void *src,size_t size)
{
	int n;
	n=0;
	while(n<size)
	{
		*(((unsigned char *)(dst))++)=*(((unsigned char *)(src))++);
		n++;
	}

	return dst;
}

int libc_read_uint(const char *filePath,unsigned int *data)
{
	HANDLE			newFile;
	DWORD			readb=0;
	
	newFile	=	CreateFile(filePath,FILE_READ_DATA,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(newFile==INVALID_HANDLE_VALUE)
		return 0;

	ReadFile	(newFile,data,4,&readb,NULL);
	CloseHandle (newFile);

	return readb;
}


int libc_readfile(const char *filePath,void **buffer)
{
	unsigned int	file_len;
	HANDLE			newFile;
	unsigned char	*data;
	DWORD			readb=0;
	
	newFile	=	CreateFile(filePath,FILE_READ_DATA,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(newFile==INVALID_HANDLE_VALUE)
		return 0;

	file_len	=GetFileSize(newFile,NULL);
	data		=aligned_alloc(file_len);
	
	ReadFile	(newFile,data,file_len,&readb,NULL);
	CloseHandle (newFile);

	*buffer	=	data;

	return file_len;
}

int libc_writefile(const char *filePath,void *buffer,unsigned int length)
{
	unsigned int	file_len;
	HANDLE			newFile;
	unsigned char	*data;
	DWORD			writeb=0;
	
	newFile	=	CreateFile(filePath,FILE_WRITE_DATA,FILE_SHARE_WRITE,NULL,CREATE_NEW,0,NULL);
	if(newFile==INVALID_HANDLE_VALUE)
		return 0;

	WriteFile	(newFile,buffer,length,&writeb,NULL);
	CloseHandle (newFile);
	return 1;
}
/*
void *fopen(const char *filePath,const char *mode)
{
}

void *fprintf(const char *file,const char *format,...)
{
}

void *fputs(void *file,const char *data)
{
}

void fclose(void *file)
{
}
*/
BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

int __cdecl _purecall(void)
{
	return 0;
}


