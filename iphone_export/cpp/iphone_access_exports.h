#pragma once

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// Windows ヘッダー ファイル
#include <windows.h>
#include <cstdint>

#ifdef IPHONEACCESS_EXPORTS
#define IPHONE_API extern "C" __declspec(dllexport)
#else
#define IPHONE_API extern "C" __declspec(dllimport)
#endif

/** 本ライブラリで発生したエラー内容の定義一覧 */
enum {
    ERROR_nantoka = -200,
};

IPHONE_API int32_t get_music_list(char** lists, bool force_update);
IPHONE_API int32_t get_camera_roll_list(char** lists, bool force_update);
IPHONE_API int32_t get_home_video_list(char** lists, bool force_update);
IPHONE_API int32_t get_picture_list(char** lists, bool force_update);
IPHONE_API int32_t get_last_error();

