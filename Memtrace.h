//---------------------------------------------------------------------------
#if (_MEMTRACE)
    #ifndef memtraceH
        #define memtraceH
        #if (!defined(_MEMTRACE))
            #define _MEMTRACE 5
        #endif        

        //---------------------------------------------------------------------------
        #include <stdarg.h>


        //---------------------------------------------------------------------------
        #if (__WATCOMC__)
            #define __FUNC__ __FUNCTION__   
        #elif ((__TURBOC__< 0x540) && (!(__ZTC__)))
            #define __FUNC__ "null"
        #endif


        //---------------------------------------------------------------------------
		void *MallocTraced (char *pacFile, int piLine, char *pacFunc, unsigned int piSize);
		void *CallocTraced(char *pacFile, int piLine, char *pacFunc, unsigned int piItems, unsigned int piSize);
		void FreeTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock);
		void *ReallocTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock, unsigned int piSize);
		char *StrDupTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock);
		char *StrCpyTraced (char *pacFile, int piLine, char *pacFunc, char *ppBlock1, char *ppBlock2, unsigned int piBlock1, unsigned int piBlock2);
		int StrCmpTraced (char *pacFile, int piLine, char *pacFunc, char *ppBlock1, char *ppBlock2, unsigned int piBlock1, unsigned int piBlock2);
		char *StrCatTraced (char *pacFile, int piLine, char *pacFunc, char *ppBlock1, char *ppBlock2, unsigned int piBlock1, unsigned int piBlock2);
		void *MemCpyTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock1, void *ppBlock2, size_t piSize, unsigned int piBlock1, unsigned int piBlock2);
		int MemCmpTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock1, void *ppBlock2, unsigned int piSize, unsigned int piBlock1, unsigned int piBlock2);
		void *MemSetTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock, int piValue, unsigned int piSize, unsigned int piBlock);
		void LogTraced (char *pacFile, int piLine, char *pacFunc, int piLevel, char *pacValue, ...);
		void AssertTraced (char *pacFile, int piLine, char *pacFunc, int piTest);
		#define malloc(piSize) MallocTraced(__FILE__, __LINE__, __FUNC__, (piSize))
		#define calloc(piItems, piSize) CallocTraced(__FILE__, __LINE__, __FUNC__, (piItems), (piSize))
		#define free(ppBlock) FreeTraced(__FILE__, __LINE__, __FUNC__, (ppBlock))
		#define realloc(ppBlock, piSize) ReallocTraced(__FILE__, __LINE__, __FUNC__, (ppBlock), (piSize))
		#define strdup(ppBlock) StrDupTraced(__FILE__, __LINE__, __FUNC__, (ppBlock))
		#define strcpy(ppBlock1, ppBlock2) StrCpyTraced(__FILE__, __LINE__, __FUNC__, (ppBlock1), (ppBlock2), (sizeof(ppBlock1)), (sizeof(ppBlock2)))
		#define strcmp(ppBlock1, ppBlock2) StrCmpTraced(__FILE__, __LINE__, __FUNC__, (ppBlock1), (ppBlock2), (sizeof(ppBlock1)), (sizeof(ppBlock2)))
		#define strcat(ppBlock1, ppBlock2) StrCatTraced(__FILE__, __LINE__, __FUNC__, (ppBlock1), (ppBlock2), (sizeof(ppBlock1)), (sizeof(ppBlock2)))
		#define memcpy(ppBlock1, ppBlock2, piSize) MemCpyTraced(__FILE__, __LINE__, __FUNC__, (ppBlock1), (ppBlock2), (piSize), (sizeof(ppBlock1)), (sizeof(ppBlock2)))
		#define memcmp(ppBlock1, ppBlock2, piSize) MemCmpTraced(__FILE__, __LINE__, __FUNC__, (ppBlock1), (ppBlock2), (piSize), (sizeof(ppBlock1)), (sizeof(ppBlock2)))
		#define memset(ppBlock, piValue, piSize) MemSetTraced(__FILE__, __LINE__, __FUNC__, (ppBlock), (piValue), (piSize), (sizeof(ppBlock)))
		#define log(pacValue) LogTraced(__FILE__, __LINE__, __FUNC__, 2, "NOTE: %s", (pacValue))
		#define AssertOld assert
		#undef assert
		#define assert(piTest) AssertTraced(__FILE__, __LINE__, __FUNC__, (piTest))
	#endif
#else
	#define log(pacValue)
#endif
//---------------------------------------------------------------------------
