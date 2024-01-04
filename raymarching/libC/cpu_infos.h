




typedef struct
{
unsigned int sse_ver;
unsigned int sse_sub_ver;
unsigned int cache_line;
}cpu_infos_t;


#ifdef __cplusplus 
extern "C" {
#endif

	int getCPUInfos(cpu_infos_t *cpu_infos);

#ifdef __cplusplus 
}
#endif