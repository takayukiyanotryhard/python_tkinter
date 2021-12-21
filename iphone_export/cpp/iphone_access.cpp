#include "iphone_access_exports.h"

#include <cstring>
#include <cstdlib>

#include "iphone_access.h"
#include "Misc.h"
#include <tchar.h>
#include <ctime>


#define MEDIA_LIBRARY_DB_NAME "MediaLibrary.sqlitedb"
#define PHOTOS_DB_NAME "Photos.sqlite"

//! DBの更新間隔
#define NO_UPDATE_DB_DURATION (60 * 60)

//! 本ライブラリ固有のエラーコード
int32_t last_error_no = 0;

//! ログや一時ファイルを保存するディレクトリ
wchar_t* application_dir = 0;

static bool initialized = false;

// ログ用
LARGE_INTEGER freq;
LARGE_INTEGER counter;
wchar_t log_path[MAX_PATH] = { 0 };
char log_buf_u8[MAX_PATH] = { 0 };
wchar_t log_buf[MAX_PATH] = { 0 };

/**
 * ライブラリ固有の初期化
 */
void init() {
    if (initialized) goto error;

    get_application_dir(&application_dir);

    wsprintf(log_path, L"%ws\\iphone_access_lib.log", application_dir);

    QueryPerformanceFrequency(&freq);
    initialized = true;

error:
    ;
}

/**
 * iphone上の音楽ファイル一覧を返す
 *
 * 文字列はutf8フォーマットとする
 * @param list 文字列配列(1行目はカラム名)
 * @param force_update true:必ずDBを取得する false:
 * @return カラム数
 */
IPHONE_API int32_t get_music_list(char** list, bool force_update)
{
    wchar_t* db_fpath = 0;
    int32_t err_no = 0;

    init();

    if (force_update || need_update_music_db()) {
        // iPhoneへのアクセスクラスを初期化し、
        // DBファイルをローカルに保存する
    }
    else {
        // DBファイルをセット
    }

    // DBへのアクセスクラスを初期化

    // 音楽ファイルの一覧を取得

    return 1;
}

/**
 * カメラロールの一覧を返す
 *
 * ファイル種別以外はget_music_listに同じ
 */
IPHONE_API int32_t get_camera_roll_list(char** lists, bool force_update)
{
    return 2;
}

/**
 * ホームビデオ(iTunesから転送した動画)の一覧を返す
 *
 * ファイル種別以外はget_music_listに同じ
 */
IPHONE_API int32_t get_home_video_list(char** lists, bool force_update)
{
    return 3;
}

/**
 * 写真の一覧を返す
 *
 * ファイル種別以外はget_music_listに同じ
 */
IPHONE_API int32_t get_picture_list(char** lists, bool force_update)
{
    return 4;
}

/**
 * ライブラリで発生したエラーの取得
 */
IPHONE_API int32_t get_last_error()
{
    return last_error_no;
}

/**
 * 管理DBのupdateが必要かどうか
 */
bool need_update_db(MEDIA_FILE_KIND kind)
{
    wchar_t db_path[MAX_PATH] = { 0 };
    wchar_t* p = db_path;
    int len;
    bool update = true;
    struct _stat64 stat;
    time_t current;

    time(&current);
    switch (kind) {
    case MEDIA_FILE_KIND::MUSIC:
    case MEDIA_FILE_KIND::HOME_VIDEO:
        wsprintf(p, L"%ws\\%ws", application_dir, _T(MEDIA_LIBRARY_DB_NAME));
        break;
    case MEDIA_FILE_KIND::CAMERA_ROLL:
    case MEDIA_FILE_KIND::PICTURE:
        wsprintf(p, L"%ws\\%ws", application_dir, _T(PHOTOS_DB_NAME));
        break;
    }

    _wstat64(db_path, &stat);

    log("mtime:%lld\n", stat.st_mtime);

    return update;
}

/**
 * 音楽用の管理DBのupdateが必要かどうか
 */
bool need_update_music_db() {
    return need_update_db(MEDIA_FILE_KIND::MUSIC);
}

/**
 * iPhoneで撮影した動画の管理DBのupdateが必要かどうか
 */
bool need_update_camera_db()
{
    return need_update_db(MEDIA_FILE_KIND::CAMERA_ROLL);
}

/**
 * iTunesで転送した動画の管理DBのupdateが必要かどうか
 */
bool need_update_video_db()
{
    return need_update_db(MEDIA_FILE_KIND::HOME_VIDEO);
}

/**
 * 写真用の管理DBのupdateが必要かどうか
 */
bool need_update_picture_db()
{
    return need_update_db(MEDIA_FILE_KIND::PICTURE);
}
