#include "msdosc.h"

#include <stdio.h>
#include <stdlib.h>

static BYTE* DiskImage = NULL;
static DWORD DiskImageSize;

static BOOL ExtractMSDOS(const char* path)
{
	int i, j;
	BOOL r = FALSE;
	uint8_t* diskcopy_buffer = NULL;
	char locale_path[MAX_PATH];
	char diskcopy_dll_path[MAX_PATH];
	char* extractlist[] = { "MSDOS   SYS", "COMMAND COM", "IO      SYS", "MODE    COM",
		"KEYB    COM", "KEYBOARDSYS", "KEYBRD2 SYS", "KEYBRD3 SYS", "KEYBRD4 SYS",
		"DISPLAY SYS", "EGA     CPI", "EGA2    CPI", "EGA3    CPI" };

	if (path == NULL)
		return FALSE;

	// There should be a diskcopy.dll in the user's AppData directory.
	// Since we're working with a known copy of diskcopy.dll, just load it
	// in memory and point to the known disk image resource buffer.
	static_sprintf(diskcopy_dll_path, "%s\\%s\\diskcopy.dll", app_data_dir, FILES_DIR);
	if (read_file(diskcopy_dll_path, &diskcopy_buffer) != DISKCOPY_SIZE) {
		uprintf("'diskcopy.dll' was either not found or is invalid");
		goto out;
	}
	DiskImage = &diskcopy_buffer[DISKCOPY_IMAGE_OFFSET];
	DiskImageSize = DISKCOPY_IMAGE_SIZE;

	// Reduce the visible mess by placing all the locale files into a subdir
	static_strcpy(locale_path, path);
	static_strcat(locale_path, "LOCALE\\");
	CreateDirectoryA(locale_path, NULL);

	for (i = 0, r = TRUE; r && i < FAT_FN_DIR_ENTRY_LAST; i++) {
		if (DiskImage[FAT12_ROOTDIR_OFFSET + i * FAT_BYTES_PER_DIRENT] == FAT_DIRENT_DELETED)
			continue;
		for (j = 0; r && j < ARRAYSIZE(extractlist); j++) {
			if (memcmp(extractlist[j], &DiskImage[FAT12_ROOTDIR_OFFSET + i * FAT_BYTES_PER_DIRENT], 8 + 3) == 0) {
				r = ExtractFAT(i, (j < 3) ? path : locale_path);
				if ((j == 2) || (j == 7) || (j == 12))
					UpdateProgress(OP_FILE_COPY, -1.0f);
			}
		}
	}
	if (r)
		r = SetDOSLocale(path, FALSE);

out:
	safe_free(diskcopy_buffer);
	return r;
}
