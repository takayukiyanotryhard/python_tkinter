
#pragma once
#include <cstdio>

extern wchar_t log_path[];
extern wchar_t log_buf[];
extern LARGE_INTEGER freq;
extern LARGE_INTEGER counter;

#define log(...) \
do { \
    FILE* fp = _wfopen(log_path, L"a"); \
    if (fp) { \
        int len; \
        OutputDebugString(L"fp is not null"); \
        QueryPerformanceCounter(&counter); \
        double t = (double)counter.QuadPart/(double)freq.QuadPart; \
        sprintf(log_buf_u8, __VA_ARGS__); \
        len = MultiByteToWideChar(CP_UTF8, 0, log_buf_u8, (int)strlen(log_buf_u8), log_buf, MAX_PATH); \
        log_buf[len] = '\0'; \
        fwprintf(fp, L"%12.3f %ws(%d) %ws",t, _T(__FILE__),__LINE__, log_buf); \
        fclose(fp); \
    }else { \
        OutputDebugString(L"fp is null"); \
    } \
} while(0)

enum class MEDIA_FILE_KIND {
    MUSIC,
    CAMERA_ROLL,
    HOME_VIDEO,
    PICTURE
};

void init();
bool need_update_db(MEDIA_FILE_KIND kind);
bool need_update_music_db();
bool need_update_camera_db();
bool need_update_video_db();
bool need_update_picture_db();
