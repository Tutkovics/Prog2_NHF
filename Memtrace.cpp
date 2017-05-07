//---------------------------------------------------------------------------
#if (_MEMTRACE)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <dos.h>
#include <assert.h>

#if (__WATCOMC__)
	#define __FUNC__ __FUNCTION__
#elif ((__TURBOC__< 0x540) && (!(__ZTC__)))
	#define __FUNC__ "null"
#endif

#ifndef min
	#define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
	#define max(a, b)  (((a) > (b)) ? (a) : (b))
#endif

void InitializeTraced (void);
void EndTraced (void);
void *MallocTraced (char *pacFile, int piLine, char *pacFunc, unsigned int piSize);
void *CallocTraced(char *pacFile, int piLine, char *pacFunc, unsigned int piItems, unsigned int piSize);
void FreeTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock);
void *ReallocTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock, unsigned int piSize);
char *StrDupTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock);
char *StrCpyTraced (char *pacFile, int piLine, char *pacFunc, char *ppBlock1, char *ppBlock2, unsigned int piBlock1, unsigned int piBlock2);
int StrCmpTraced (char *pacFile, int piLine, char *pacFunc, char *ppBlock1, char *ppBlock2, unsigned int piBlock1, unsigned int piBlock2);
char *StrCatTraced (char *pacFile, int piLine, char *pacFunc, char *ppBlock1, char *ppBlock2, unsigned int piBlock1, unsigned int piBlock2);
void *MemCpyTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock1, void *ppBlock2, unsigned int piSize);
void *MemSetTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock, int piValue, unsigned int piSize);
int MemCmpTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock1, void *ppBlock2, unsigned int piSize);
void LogTraced (char *pacFile, int piLine, char *pacFunc, int piLevel, char *pacValue, ...);
void AssertTraced (char *pacFile, int piLine, char *pacFunc, int piTest);
void AddBlock (char *pacFile, unsigned int piLine, char *pacFunc, void *ppBlock, unsigned int piSize);
void RemoveBlock (char *pacFile, unsigned int piLine, char *pacFunc, void *ppBlock);
int GetBlock (void *ppBlock);
int CheckBlockBounds (void *ppBlock, unsigned int piBlock, unsigned int piSize);
void UpdatePeaks (long plSize);


//---------------------------------------------------------------------------
#define VERSION_TRACED 191

#if ((__FLAT__) || (_FLAT_))
	#define MAX_BLOCKS_TRACED (500000/sizeof(udtBlock))
#else
	#define MAX_BLOCKS_TRACED (50000/sizeof(udtBlock))
#endif



//---------------------------------------------------------------------------
struct udtCounts
{
	clock_t clkStart;
	clock_t clkStop;
	unsigned long lPeakMemory;
	unsigned long lMallocSize;
	unsigned long lCallocSize;
	unsigned long lReallocSize;
	unsigned long lNewSize;
	unsigned long lNewASize;
	unsigned int iMallocs;
	unsigned int iCallocs;
	unsigned int iReallocs;
	unsigned int iFrees;
	unsigned int iNews;
	unsigned int iNewsA;
	unsigned int iDeletes;
	unsigned int iDeletesA;
	unsigned int iStrCpys;
	unsigned int iStrCmps;
	unsigned int iStrCats;
	unsigned int iMemCpys;
	unsigned int iMemCmps;
	unsigned int iMemSets;
	unsigned int iMaxBlock;
	unsigned int iMinBlock;
} mudtCounts;

struct udtBlock
{
	char acFile[128];
	char acFunc[64];
	void *pBlock;
	unsigned int iSize;
	unsigned int iLine;
} mudtBlocks[MAX_BLOCKS_TRACED];

struct udtMap
{
	void *pBlock;
	unsigned int iSize;
};

char macLogName[256];
FILE *mpLogFile;
FILE *mpMapFile;
unsigned int miInitialized=0;
unsigned int miBlocks;


//---------------------------------------------------------------------------
void InitializeTraced (void)
{
	if (!miInitialized)
	{
		miInitialized=1;
		mudtCounts.clkStart=clock();

		memset(&mudtCounts, 0, sizeof(mudtCounts));
		mudtCounts.iMinBlock=-1;

		miBlocks=0;
		memset(mudtBlocks, 0, sizeof(mudtBlocks));

		strcpy(macLogName, "memtrace.map");
		mpMapFile=fopen(macLogName, "wb+");

		strcpy(macLogName, "memtrace.log");
		mpLogFile=fopen(macLogName, "wt");

		fputs("------------------------------------------------------------------------------\n", mpLogFile);
		fprintf(mpLogFile, "|                          MEMTRACE v%d.%02d for C/C++                          |\n", VERSION_TRACED/100, VERSION_TRACED%100);
		fputs("|    A portable memory leak and buffer overrun/underrun detection library.   |\n", mpLogFile);
		fputs("|            (c) Copyright 2002-2004 by Javier Gutierrez Chamorro            |\n", mpLogFile);
		fputs("------------------------------------------------------------------------------\n", mpLogFile);

		fputs("\n\nOPTIONS\n-------\n", mpLogFile);
		fprintf(mpLogFile, "Compilation timestamp:    %s %s\n", __DATE__, __TIME__);
		fputs("Compiler:                 ", mpLogFile);
		#if (__TURBOC__)
			fputs("Turbo/Borland ", mpLogFile);
			#ifdef __cplusplus
				fputs("C++ ", mpLogFile);
			#else
				fputs("C ", mpLogFile);
			#endif
			fprintf(mpLogFile, "%X.%02X ", __BORLANDC__>>8, __BORLANDC__&0xFE);
		#elif (_MSC_VER)
			fputs("Microsoft/Quick/Visual ", mpLogFile);
			#ifdef __cplusplus
				fputs("C++ ", mpLogFile);
			#else
				fputs("C ", mpLogFile);
			#endif
			fprintf(mpLogFile, "%d.%02d ", _MSC_VER/100, _MSC_VER%100);
		#elif (__WATCOMC__)
			fputs("Watcom/OpenWatcom ", mpLogFile);
			#ifdef __cplusplus
				fputs("C++ ", mpLogFile);
			#else
				fputs("C ", mpLogFile);
			#endif
			fprintf(mpLogFile, "%d.%02d ", __WATCOMC__/100, __WATCOMC__%100);
		#elif (__ZTC__)
			fputs("Zortech/Symantec/DigitalMars ", mpLogFile);
			#ifdef __cplusplus
				fputs("C++ ", mpLogFile);
			#else
				fputs("C ", mpLogFile);
			#endif
			fprintf(mpLogFile, "%X.%02X ", __ZTC__>>8, __ZTC__&0xFE);
		#elif (__GNUC__)
			fputs("GCC ", mpLogFile);
			#ifdef __cplusplus
				fputs("C++ ", mpLogFile);
			#else
				fputs("C ", mpLogFile);
			#endif
			fprintf(mpLogFile, "%d.%02d ", __GNUC__, __GNUC_MINOR__);
		#elif (__IBMC__)
			fputs("IBM ", mpLogFile);
			#ifdef __cplusplus
				fputs("C++ ", mpLogFile);
			#else
				fputs("C ", mpLogFile);
			#endif
			fprintf(mpLogFile, "%d.%02d ", __IBMC__/100, __IBMC__%100);
		#elif (__HIGHC__)
			fputs("Metaware High ", mpLogFile);
			#ifdef __cplusplus
				fputs("C++ ", mpLogFile);
			#else
				fputs("C ", mpLogFile);
			#endif
		#elif (__CCDL__)
			fputs("Ladsoft ", mpLogFile);
			#ifdef __cplusplus
				fputs("C++ ", mpLogFile);
			#else
				fputs("C ", mpLogFile);
			#endif
		#elif (__POWERC)
			fputs("Power  ", mpLogFile);
			#ifdef __cplusplus
				fputs("C++ ", mpLogFile);
			#else
				fputs("C ", mpLogFile);
			#endif
		#endif
		#if ((__WIN32__) || (__WINDOWS__) || (__NT__))
			fputs("(Windows", mpLogFile);
		#elif (__OS2__)
			fputs("(OS/2", mpLogFile);
		#elif (__QNX__)
			fputs("(QNX", mpLogFile);
		#elif (__NETWARE__)
			fputs("(Netware", mpLogFile);
		#else
			fputs("(DOS", mpLogFile);
		#endif
		fprintf(mpLogFile, " %d bits)\n", sizeof(int)<<3);
		fputs("Memory model:             ", mpLogFile);
		#if (__TINY__)
			fputs("Tiny", mpLogFile);
		#elif (__SMALL__)
			fputs("Small", mpLogFile);
		#elif (__MEDIUM__)
			fputs("Medium", mpLogFile);
		#elif (__COMPACT__)
			fputs("Compact", mpLogFile);
		#elif (__LARGE__)
			fputs("Large", mpLogFile);
		#elif (__HUGE__)
			fputs("Huge", mpLogFile);
		#elif ((__FLAT__) || (_FLAT_))
			fputs("Flat", mpLogFile);
		#else
			fputs("Default", mpLogFile);
		#endif
		#ifdef _DEBUG
			fputs(" (Debug).\n", mpLogFile);
		#else
			fputs(" (Release).\n", mpLogFile);
		#endif


		fprintf(mpLogFile, "MEMTRACE reporting level: %d (_MEMTRACE=%d)\n", _MEMTRACE, _MEMTRACE);

		fputs("\n\nEXECUTION TRACE\n---------------\n", mpLogFile);
		atexit(EndTraced);
	}
}


//---------------------------------------------------------------------------
void EndTraced (void)
{
	unsigned int iCount;

	if (miInitialized)
	{
		miInitialized=0;
		mudtCounts.clkStop=clock();

		fputs("\n\nREPORT\n------\n", mpLogFile);
		for (iCount=0; iCount<miBlocks; iCount++)
		{
			fprintf(mpLogFile, "\t- Block 0x%p (%d bytes) allocated at %s(%d) in %s() has not been freed.\n", mudtBlocks[iCount].pBlock,
			mudtBlocks[iCount].iSize, mudtBlocks[iCount].acFile, mudtBlocks[iCount].iLine, mudtBlocks[iCount].acFunc, mudtBlocks[iCount].iLine);
		}

		fputs("\n\nSUMARY\n------\n", mpLogFile);
		fprintf(mpLogFile, "\t- malloc(): %d call(s) (%ld bytes).\n", mudtCounts.iMallocs, mudtCounts.lMallocSize);
		fprintf(mpLogFile, "\t- calloc(): %d call(s) (%ld bytes).\n", mudtCounts.iCallocs, mudtCounts.lCallocSize);
		fprintf(mpLogFile, "\t- free(): %d call(s).\n", mudtCounts.iFrees);
		if (mudtCounts.iMallocs+mudtCounts.iCallocs!=mudtCounts.iFrees)
		{
			fputs("\t  WARNING: Allocated and freed blocks do not match.\n", mpLogFile);
		}

		fprintf(mpLogFile, "\t- realloc(): %d call(s) (%ld bytes).\n", mudtCounts.iReallocs, mudtCounts.lReallocSize);

		fprintf(mpLogFile, "\t- new: %d call(s) (%ld bytes).\n", mudtCounts.iNews, mudtCounts.lNewSize);
		fprintf(mpLogFile, "\t- delete: %d call(s).\n", mudtCounts.iDeletes);
		if (mudtCounts.iNews!=mudtCounts.iDeletes)
		{
			fputs("\t  WARNING: Allocated and freed blocks do not match.\n", mpLogFile);
		}

		fprintf(mpLogFile, "\t- new[]: %d call(s) (%ld bytes).\n", mudtCounts.iNewsA, mudtCounts.lNewASize);
		fprintf(mpLogFile, "\t- delete[]: %d call(s).\n", mudtCounts.iDeletesA);
		if (mudtCounts.iNewsA!=mudtCounts.iDeletesA)
		{
			fputs("\t  WARNING: Allocated and freed blocks do not match.\n", mpLogFile);
		}
		fprintf(mpLogFile, "\t- strcpy(): %d call(s).\n", mudtCounts.iStrCpys);
		fprintf(mpLogFile, "\t- strcmp(): %d call(s).\n", mudtCounts.iStrCmps);
		fprintf(mpLogFile, "\t- strcat(): %d call(s).\n", mudtCounts.iStrCats);
		fprintf(mpLogFile, "\t- memcpy(): %d call(s).\n", mudtCounts.iMemCpys);
		fprintf(mpLogFile, "\t- memcmp(): %d call(s).\n", mudtCounts.iMemCmps);
		fprintf(mpLogFile, "\t- memset(): %d call(s).\n", mudtCounts.iMemSets);

		fputs("\n\nTOTALS\n------\n", mpLogFile);
		fprintf(mpLogFile, "\t- Execution time: %d.%02d seconds.\n", (mudtCounts.clkStop-mudtCounts.clkStart)/((unsigned int) CLK_TCK), (mudtCounts.clkStop-mudtCounts.clkStart)*100/((unsigned int) CLK_TCK)%100);
		fprintf(mpLogFile, "\t- Peak memory used: %ld bytes.\n", mudtCounts.lPeakMemory);
		fprintf(mpLogFile, "\t- Largest allocated block: %d bytes.\n", mudtCounts.iMaxBlock);
		fprintf(mpLogFile, "\t- Smallest allocated block: %d bytes.\n", mudtCounts.iMinBlock);
		fprintf(mpLogFile, "\t- Memory allocated: %d blocks (%ld bytes).\n", mudtCounts.iMallocs+mudtCounts.iCallocs+mudtCounts.iNews+mudtCounts.iNewsA, mudtCounts.lMallocSize+mudtCounts.lCallocSize+mudtCounts.lNewSize+mudtCounts.lNewASize);
		fprintf(mpLogFile, "\t- Memory freed: %d blocks.\n", mudtCounts.iFrees+mudtCounts.iDeletes+mudtCounts.iDeletesA);
		fprintf(mpLogFile, "\t- Memory moved: %d blocks (%ld Bytes).\n", mudtCounts.iReallocs, mudtCounts.lReallocSize);

		// Create memory map usage
		unsigned int iEntries;
		struct udtMap udtMap;
		unsigned int aiFreqs[2000];

		memset(aiFreqs, 0, sizeof(aiFreqs));
		fseek(mpMapFile, 0, SEEK_END);
		iEntries=ftell(mpMapFile)/sizeof(udtMap);
		for (iCount=0; iCount<iEntries; iCount++)
		{

		}


		fclose(mpMapFile);
		remove("memtrace.map");
		fclose(mpLogFile);
	}
}


//---------------------------------------------------------------------------
void *MallocTraced (char *pacFile, int piLine, char *pacFunc, unsigned int piSize)
{
	void *pMemory;

	InitializeTraced();
	pMemory=malloc(piSize);
	mudtCounts.iMallocs++;
	mudtCounts.lMallocSize+=piSize;
	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: 0x%p=malloc(%d)", pMemory, piSize);
	AddBlock(pacFile, piLine, pacFunc, pMemory, piSize);
	if (pMemory==NULL)
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to malloc() failed.");
	}
	return(pMemory);
}


//---------------------------------------------------------------------------
void *CallocTraced (char *pacFile, int piLine, char *pacFunc, unsigned int piItems, unsigned int piSize)
{
	void *pMemory;

	InitializeTraced();
	pMemory=calloc(piItems, piSize);
	mudtCounts.iCallocs++;
	mudtCounts.lCallocSize+=(piItems*piSize);
	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: 0x%p=calloc(%d, %d)", pMemory, piItems, piSize);
	AddBlock(pacFile, piLine, pacFunc, pMemory, piItems*piSize);
	if (pMemory==NULL)
	{
			LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to calloc() failed.");
	}
	return(pMemory);
}


//---------------------------------------------------------------------------
void FreeTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock)
{
	InitializeTraced();
	mudtCounts.iFrees++;
	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: free(0x%p)", ppBlock);
	RemoveBlock(pacFile, piLine, pacFunc, ppBlock);
	if (ppBlock==NULL)
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to free() failed.");
	}
	free(ppBlock);
}


//---------------------------------------------------------------------------
void *ReallocTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock, unsigned int piSize)
{
	void *pMemory;

	InitializeTraced();
	if (ppBlock==NULL)
	{
		pMemory=malloc(piSize);
		mudtCounts.iMallocs++;
		mudtCounts.lMallocSize+=piSize;
		AddBlock(pacFile, piLine, pacFunc, pMemory, piSize);
	}
	else if (piSize==0)
	{
		mudtCounts.iFrees++;
		RemoveBlock(pacFile, piLine, pacFunc, ppBlock);
		free(ppBlock);
		pMemory=0;
	}
	else
	{
		mudtCounts.iReallocs++;
		mudtCounts.lReallocSize+=piSize;
		RemoveBlock(pacFile, piLine, pacFunc, ppBlock);
		pMemory=realloc(ppBlock, piSize);
		AddBlock(pacFile, piLine, pacFunc, pMemory, piSize);
	}

	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: 0x%p=realloc(0x%p, %d)", pMemory, ppBlock, piSize);
	if (pMemory==NULL)
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to realloc() failed.");
	}
	mudtCounts.iReallocs++;
	mudtCounts.lReallocSize+=piSize;
	return(pMemory);
}


//---------------------------------------------------------------------------
char *StrDupTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock)
{
	unsigned int iSize;
	char *pMemory;

	InitializeTraced();
	iSize=strlen((char *) ppBlock)+1;
	pMemory=(char *) malloc(iSize);
	mudtCounts.iMallocs++;
	mudtCounts.lMallocSize+=iSize;
	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: 0x%p=strdup(0x%p)", pMemory, ppBlock);
	AddBlock(pacFile, piLine, pacFunc, pMemory, iSize);
	if (pMemory==NULL)
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to strdup() failed.");
	}
	memcpy(pMemory, ppBlock, iSize);
	return(pMemory);
}


//---------------------------------------------------------------------------
char *StrCpyTraced (char *pacFile, int piLine, char *pacFunc, char *ppBlock1, char *ppBlock2, unsigned int piBlock1, unsigned int piBlock2)
{
	char *pMemory;

	InitializeTraced();
	pMemory=strcpy(ppBlock1, ppBlock2);
	mudtCounts.iStrCpys++;
	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: 0x%p=strcpy(0x%p, \"%s\")", pMemory, ppBlock1, ppBlock2);
	if ((CheckBlockBounds(ppBlock1, piBlock1, strlen(ppBlock2)+1)<0) ||
		(CheckBlockBounds(ppBlock2, piBlock2, strlen(ppBlock2)+1)<0))
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to strcpy() out of bounds.");
	}
	return(pMemory);
}


//---------------------------------------------------------------------------
int StrCmpTraced (char *pacFile, int piLine, char *pacFunc, char *ppBlock1, char *ppBlock2, unsigned int piBlock1, unsigned int piBlock2)
{
	int iResult;

	InitializeTraced();
	iResult=strcmp(ppBlock1, ppBlock2);
	mudtCounts.iStrCmps++;
	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: %d=strcmp(0x%p, \"%s\")", iResult, ppBlock1, ppBlock2);
	if ((CheckBlockBounds(ppBlock1, piBlock1, strlen(ppBlock2)+1)<0) ||
		(CheckBlockBounds(ppBlock2, piBlock2, strlen(ppBlock2)+1)<0))
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to strcmp() out of bounds.");
	}
	return(iResult);
}


//---------------------------------------------------------------------------
char *StrCatTraced (char *pacFile, int piLine, char *pacFunc, char *ppBlock1, char *ppBlock2, unsigned int piBlock1, unsigned int piBlock2)
{
	char *pMemory;

	InitializeTraced();
	pMemory=strcat(ppBlock1, ppBlock2);
	mudtCounts.iStrCats++;
	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: 0x%p=strcat(\"%s\", \"%s\")", pMemory, ppBlock1, ppBlock2);
	if (CheckBlockBounds(ppBlock1, piBlock1, strlen(ppBlock1)+strlen(ppBlock2)+2)<0)
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to strcat() out of bounds in first parameter.");
	}
	return(pMemory);
}


//---------------------------------------------------------------------------
void *MemCpyTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock1, void *ppBlock2, size_t piSize, unsigned int piBlock1, unsigned int piBlock2)
{
	void *pMemory;

	InitializeTraced();
	pMemory=memcpy(ppBlock1, ppBlock2, piSize);
	mudtCounts.iMemCpys++;
	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: 0x%p=memcpy(0x%p, 0x%p, %d)", pMemory, ppBlock1, ppBlock2, piSize);
	if ((CheckBlockBounds(ppBlock1, piBlock1, piSize)<0) ||
		(CheckBlockBounds(ppBlock2, piBlock2, piSize)<0))
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to memcpy() out of bounds.");
	}
	return(pMemory);
}

//---------------------------------------------------------------------------
int MemCmpTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock1, void *ppBlock2, unsigned int piSize, unsigned int piBlock1, unsigned int piBlock2)
{
	int iResult;

	InitializeTraced();
	iResult=memcmp(ppBlock1, ppBlock2, piSize);
	mudtCounts.iMemCmps++;
	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: %d=memcmp(0x%p, 0x%p, %d)", iResult, ppBlock1, ppBlock2, piSize);
	if ((CheckBlockBounds(ppBlock1, piBlock1, piSize)<0) ||
		(CheckBlockBounds(ppBlock2, piBlock2, piSize)<0))
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to memcmp() out of bounds.");
	}
	return(iResult);
}


//---------------------------------------------------------------------------
void *MemSetTraced (char *pacFile, int piLine, char *pacFunc, void *ppBlock, int piValue, unsigned int piSize, unsigned int piBlock)
{
	void *pMemory;

	InitializeTraced();
	pMemory=memset(ppBlock, piValue, piSize);
	mudtCounts.iMemSets++;
	LogTraced(pacFile, piLine, pacFunc, 5, "TRACE: 0x%p=memset(0x%p, '\\x%x', %d)", pMemory, ppBlock, piValue, piSize);
	if (CheckBlockBounds(ppBlock, piBlock, piSize)<0)
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Call to memset() out of bounds.");
	}
	return(pMemory);
}

//---------------------------------------------------------------------------
#if (__cplusplus)
	void* cdecl operator new (unsigned piSize)
	{
		void *pMemory;

		InitializeTraced();
		pMemory=malloc(piSize);
		mudtCounts.iNews++;
		mudtCounts.lNewSize+=piSize;
		//LogTraced(__FILE__, __LINE__, __FUNC__, 5, "TRACE: 0x%p=new(%d)", pMemory, piSize);
		LogTraced("", 0, "inline", 5, "TRACE: 0x%p=new(%d)", pMemory, piSize);
		AddBlock(__FILE__, __LINE__, __FUNC__, pMemory, piSize);
		if (pMemory==NULL)
		{
			//LogTraced(__FILE__, __LINE__, __FUNC__, 4, "WARNING: Call to new failed.");
			LogTraced("", 0, "inline", 4, "WARNING: Call to new failed.");
		}
		return(pMemory);
	}

	void cdecl operator delete (void *ppBlock)
	{
		InitializeTraced();
		mudtCounts.iDeletes++;
		//LogTraced(__FILE__, __LINE__, __FUNC__, 5, "TRACE: delete(0x%p)", ppBlock);
		LogTraced("", 0, "inline", 5, "TRACE: delete(0x%p)", ppBlock);
		RemoveBlock(__FILE__, __LINE__, __FUNC__, ppBlock);
		if (ppBlock==NULL)
		{
			//LogTraced(__FILE__, __LINE__, __FUNC__, 4, "WARNING: Call to delete failed.");
			LogTraced("", 0, "inline", 4, "WARNING: Call to delete failed.");
		}
		free(ppBlock);
	}
	#if ((__TURBOC__>=0x500) || (__ZTC__))
		void* cdecl operator new[] (unsigned int piSize)
		{
			void *pMemory;

			InitializeTraced();
			pMemory=malloc(piSize);
			mudtCounts.iNewsA++;
			mudtCounts.lNewASize+=piSize;
			//LogTraced(__FILE__, __LINE__, __FUNC__, 5, "TRACE: 0x%p=new(%d)[]", pMemory, piSize);
			LogTraced("", 0, "inline", 5, "TRACE: 0x%p=new(%d)[]", pMemory, piSize);
			AddBlock(__FILE__, __LINE__, __FUNC__, pMemory, piSize);
			if (pMemory==NULL)
			{
				//LogTraced(__FILE__, __LINE__, __FUNC__, 4, "WARNING: Call to new[] failed.");
				LogTraced("", 0, "inline", 4, "WARNING: Call to new[] failed.");
			}
			return(pMemory);
		}

		void cdecl operator delete[] (void *ppBlock)
		{
			InitializeTraced();
			mudtCounts.iDeletesA++;
			//LogTraced(__FILE__, __LINE__, __FUNC__, 5, "TRACE: delete(0x%p)[]", ppBlock);
			LogTraced("", 0, "inline", 5, "TRACE: delete(0x%p)[]", ppBlock);
			RemoveBlock(__FILE__, __LINE__, __FUNC__, ppBlock);
			if (ppBlock==NULL)
			{
				//LogTraced(__FILE__, __LINE__, __FUNC__, 4, "WARNING: Call to delete[] failed.");
				LogTraced("", 0, "inline", 4, "WARNING: Call to delete[] failed.");
			}
			free(ppBlock);
		}
		#endif
#endif


//---------------------------------------------------------------------------
void LogTraced (char *pacFile, int piLine, char *pacFunc, int piLevel, char *pacValue, ...)
{
	int iCount;
	FILE *pFile;
	va_list pList;
	char acArguments[256];
	time_t udtTime;
	struct tm udtDate;

	if (piLevel<=_MEMTRACE)
	{
		InitializeTraced();
		va_start(pList, pacValue);
		vsprintf(acArguments, pacValue, pList);
		va_end(pList);
		udtTime=time(NULL);
		memcpy(&udtDate, localtime(&udtTime), sizeof(udtDate));
		fprintf(mpLogFile, "[%02d/%02d/%04d ", udtDate.tm_mday, udtDate.tm_mon+1, udtDate.tm_year+1900);
		fprintf(mpLogFile, "%02d:%02d:%02d:%02d] ", udtDate.tm_hour, udtDate.tm_min, udtDate.tm_sec, clock()%100);
		fprintf(mpLogFile, "%s(%d). %s(). %s\n", pacFile, piLine, pacFunc, acArguments);

		if (piLine>0)
		{
			pFile=fopen(pacFile, "rt");
			if (pFile)
			{
				for (iCount=0; iCount<piLine; iCount++)
				{
					fgets(acArguments, 255, pFile);
				}
				fprintf(mpLogFile, "%s\n", acArguments);
				fclose(pFile);
			}
		}
		else
		{
				fputs("\n", mpLogFile);
		}
		fflush(mpLogFile);
	}
}


//---------------------------------------------------------------------------
void AssertTraced (char *pacFile, int piLine, char *pacFunc, int piTest)
{
	if (!piTest)
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: assert(%d) failed.", piTest);
		assert(piTest);
	}
	else
	{
		LogTraced(pacFile, piLine, pacFunc, 2, "NOTE: assert(%d).", piTest);
	}
}


//---------------------------------------------------------------------------
void AddBlock (char *pacFile, unsigned int piLine, char *pacFunc, void *ppBlock, unsigned int piSize)
{
	struct udtMap udtMap;

	if (miBlocks<MAX_BLOCKS_TRACED)
	{
		mudtBlocks[miBlocks].pBlock=ppBlock;
		mudtBlocks[miBlocks].iSize=piSize;
		strcpy(mudtBlocks[miBlocks].acFile, pacFile);
		strcpy(mudtBlocks[miBlocks].acFunc, pacFunc);
		mudtBlocks[miBlocks].iLine=piLine;
		miBlocks++;
	}
	else
	{
		LogTraced(pacFile, piLine, pacFunc, 1, "ERROR: Symbol table full");
	}
	udtMap.pBlock=ppBlock;
	udtMap.iSize=piSize;
	fseek(mpMapFile, 0, SEEK_END);
	fwrite(&udtMap, sizeof(udtMap), 1, mpMapFile);
	UpdatePeaks(piSize);
}


//---------------------------------------------------------------------------
void RemoveBlock (char *pacFile, unsigned int piLine, char *pacFunc, void *ppBlock)
{
	unsigned int iCount;
	int iBlock;

	iBlock=-1;
	for (iCount=0; iCount<miBlocks; iCount++)
	{
		if (ppBlock==mudtBlocks[iCount].pBlock)
		{
			iBlock=iCount;
			break;
		}
	}

	if (iBlock>=0)
	{
		UpdatePeaks(-mudtBlocks[iBlock].iSize);
		if (miBlocks>0)
		{
			miBlocks--;
			memcpy(&mudtBlocks[iBlock], &mudtBlocks[miBlocks], sizeof(udtBlock));
			memset(&mudtBlocks[miBlocks], 0, sizeof(udtBlock));
		}
		else
		{
			memset(&mudtBlocks[miBlocks], 0, sizeof(udtBlock));
		}
	}
	else
	{
		LogTraced(pacFile, piLine, pacFunc, 4, "WARNING: Atempting to free a non allocated block at 0x%p.", ppBlock);
	}
}


//---------------------------------------------------------------------------
int GetBlock (void *ppBlock)
{
	unsigned int iCount;

	for (iCount=0; iCount<miBlocks; iCount++)
	{
		if (ppBlock==mudtBlocks[iCount].pBlock)
		{
				return(iCount);
		}
		else
		{
			return(-1);
		}
	}
	return(0);
}


//---------------------------------------------------------------------------
int CheckBlockBounds (void *ppBlock, unsigned int piBlock, unsigned int piSize)
{
	unsigned int iCount;

	for (iCount=0; iCount<miBlocks; iCount++)
	{
		if ((ppBlock>=mudtBlocks[iCount].pBlock) && (ppBlock<=((char *) mudtBlocks[iCount].pBlock)+mudtBlocks[iCount].iSize-piSize))
		{
			return(iCount);
		}
	}
	if (piBlock>=piSize)
	{
		return(0);
	}
	else
	{
		return(-1);
	}
}


//---------------------------------------------------------------------------
void UpdatePeaks (long plSize)
{
	static unsigned long lCurrentPeak=0;

	lCurrentPeak+=plSize;
	mudtCounts.lPeakMemory=max(lCurrentPeak, mudtCounts.lPeakMemory);
	if (plSize>0)
	{
		mudtCounts.iMaxBlock=max(plSize, mudtCounts.iMaxBlock);
		mudtCounts.iMinBlock=min(plSize, mudtCounts.iMinBlock);
	}
}


//---------------------------------------------------------------------------
#endif


