#include <windows.h>
#include <inttypes.h>

#define APPLICATION_NAME            "InfiniteWindows"
#define DISKCOPY_URL                "https://msdl.microsoft.com/download/symbols/diskcopy.dll/54505118173000/diskcopy.dll"
extern char app_data_dir[MAX_PATH], *image_path, *fido_url;
#define FILES_DIR                   APPLICATION_NAME
#define safe_sprintf(dst, count, ...) do {_snprintf(dst, count, __VA_ARGS__); (dst)[(count)-1] = 0; } while(0)
#define static_sprintf(dst, ...) safe_sprintf(dst, sizeof(dst), __VA_ARGS__)
#define DISKCOPY_SIZE               0x16ee00
#define DISKCOPY_IMAGE_OFFSET       0x66d8
#define DISKCOPY_IMAGE_SIZE         0x168000
extern BOOL SetDOSLocale(const char* path, BOOL bFreeDOS);
#define FAT_FN_DIR_ENTRY_LAST       0x40
#define FAT_BYTES_PER_DIRENT        0x20
#define FAT12_ROOTDIR_OFFSET        0x2600
#define FAT_DIRENT_DELETED          0xe5

enum action_type {
    OP_FILE_COPY
};