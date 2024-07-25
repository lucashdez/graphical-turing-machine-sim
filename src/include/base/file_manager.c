#include <stdio.h>
static struct FM_File
fm_open_file(String8 path, String8 mode) {
	void* handle = 0;

#ifdef _WIN32
	DWORD shareMode = 0x00000004 | 0x00000001 | 0x00000002;
	DWORD desiredAccess = 0x80000000; //ALL POSSIBLE
	LPSECURITY_ATTRIBUTES lpSecurityAttributes = 0;
	DWORD dwCreationDisposition = 0;
	DWORD dwFlagsAndAttributes = 0;

	for(int i = 0; i < mode.size; i++) {
		switch(mode.str[i]) {
		case 'r': {
			dwCreationDisposition |= 3;
			dwFlagsAndAttributes |= 0x1;
		} break;
		case 'w': {
			// TODO (lucashdez): Create the file for write mode
		} break;
		}
	}

	handle = CreateFileA((char*)path.str, desiredAccess, shareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, 0);
	DWORD err;			  
	if(handle == INVALID_HANDLE_VALUE) {
		 err = GetLastError();
	}
	
#endif
	return (struct  FM_File){
		mm_scratch_arena(),
		handle, 
		0,
		path,
		{0},
		0,
	};
}

static void
fm_read_file(struct FM_File *file) {
	file->contents.size = file->size;
	file->contents.str = MMPushArrayZeros(&file->arena, u8, file->size);
	#ifdef _WIN32
	DWORD bytes_read = 0;
	int res = ReadFile(file->handle, (void*)file->contents.str, file->size, &bytes_read, 0);
	#endif
}

static u64
fm_file_size(struct FM_File *file) {
	if(file->size == 0) {
#ifdef _WIN32
		DWORD high_part_size = 0; // This is WENDOWS with that bs.
		DWORD low_part_size = GetFileSize(file->handle, &high_part_size);
		file->size = (u64)(low_part_size | high_part_size);
#endif
	}
	return file->size;
}
