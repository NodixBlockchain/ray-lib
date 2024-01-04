#ifdef __cplusplus 
extern "C" {
#endif
dllexport int libc_readfile(const char *filePath,void **buffer);
dllexport int libc_writefile(const char *filePath,void *buffer,unsigned int length);
dllexport int libc_read_uint(const char *filePath,unsigned int *data);

#ifdef __cplusplus 
}
#endif