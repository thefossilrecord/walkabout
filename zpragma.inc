// COMPILE TIME CRT CONFIGURATION

#pragma output CRT_ORG_CODE = 33200      // org of compile
#pragma output REGISTER_SP = 0x7500 	// typical stack location when using sp1

#pragma output CLIB_MALLOC_HEAP_SIZE = 0          // no malloc heap
#pragma output CLIB_STDIO_HEAP_SIZE = 0          // no stdio heap (no files)

#pragma output CLIB_FOPEN_MAX = -1         // no FILE* list
#pragma output CLIB_OPEN_MAX = -1         // no fd table
