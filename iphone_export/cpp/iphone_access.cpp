#include "iphone_access_exports.h"

#include <cstring>
#include <cstdlib>

#include "iphone_access.h"
#include "Misc.h"
#include <tchar.h>
#include <ctime>
#include "AfcWrapper.h"
#include "MediaLibraryAccess.h"


#define PHOTOS_DB_NAME "Photos.sqlite"

//! DB�̍X�V�Ԋu
#define NO_UPDATE_DB_DURATION (60 * 60)

//! �{���C�u�����ŗL�̃G���[�R�[�h
int32_t last_error_no = 0;

//! ���O��ꎞ�t�@�C����ۑ�����f�B���N�g��
wchar_t* application_dir = 0;

static bool initialized = false;

// ���O�p
LARGE_INTEGER freq;
LARGE_INTEGER counter;
wchar_t log_path[MAX_PATH] = { 0 };
char log_buf_u8[MAX_PATH] = { 0 };
wchar_t log_buf[MAX_PATH] = { 0 };

/**
 * ���C�u�����ŗL�̏�����
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

int32_t test_code(bool a)
{
    return a ? 1 : 2;
}

/**
 * iphone��̉��y�t�@�C���ꗗ��Ԃ�
 *
 * �������utf8�t�H�[�}�b�g�Ƃ���
 * @param list ������z��(1�s�ڂ̓J������)
 * @param force_update true:�K��DB���擾���� false:
 * @return �J������
 */
IPHONE_API int32_t get_music_list(char*** list, bool force_update)
{
    wchar_t db_fpath[MAX_PATH] = { 0 };
    int32_t err_no = 0;
    int32_t column = 0;

    init();

    wsprintf(db_fpath, L"%ws\\%ws", application_dir, _T(MEDIA_LIBRARY_DB_NAME));
    if (force_update || need_update_music_db()) {
        // iPhone�ւ̃A�N�Z�X�N���X�����������A
        // DB�t�@�C�������[�J���ɕۑ�����
        AfcWrapper* afc = new AfcWrapper();
        afc->copy_media_library_db(db_fpath);
    }
    else {
        // DB�t�@�C�����Z�b�g
    }

    // DB�ւ̃A�N�Z�X�N���X��������
    MediaLibraryAccess* db = new MediaLibraryAccess(db_fpath);

    // ���y�t�@�C���̈ꗗ���擾
    column = db->get_music_list(list);

    return column;
}

/**
 * �J�������[���̈ꗗ��Ԃ�
 *
 * �t�@�C����ʈȊO��get_music_list�ɓ���
 */
IPHONE_API int32_t get_camera_roll_list(char** lists, bool force_update)
{
    return 2;
}

/**
 * �z�[���r�f�I(iTunes����]����������)�̈ꗗ��Ԃ�
 *
 * �t�@�C����ʈȊO��get_music_list�ɓ���
 */
IPHONE_API int32_t get_home_video_list(char** lists, bool force_update)
{
    return 3;
}

/**
 * �ʐ^�̈ꗗ��Ԃ�
 *
 * �t�@�C����ʈȊO��get_music_list�ɓ���
 */
IPHONE_API int32_t get_picture_list(char** lists, bool force_update)
{
    return 4;
}

/**
 * ���C�u�����Ŕ��������G���[�̎擾
 */
IPHONE_API int32_t get_last_error()
{
    return last_error_no;
}

void allocate_free(char** p)
{
    if (p) free(p);
}

/**
 * �Ǘ�DB��update���K�v���ǂ���
 */
bool need_update_db(MEDIA_FILE_KIND kind)
{
    wchar_t db_path[MAX_PATH] = { 0 };
    wchar_t* p = db_path;
    bool update = true;
    struct _stat64 stat = { 0 };
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

    // �t�@�C�������݂��Ȃ��ꍇ��st_mtime��0������̂ōX�V���[�g�ɓ���
    update = (current - stat.st_mtime > NO_UPDATE_DB_DURATION);

    log("mtime:%lld\n", stat.st_mtime);

    return update;
}

/**
 * ���y�p�̊Ǘ�DB��update���K�v���ǂ���
 */
bool need_update_music_db() {
    return need_update_db(MEDIA_FILE_KIND::MUSIC);
}

/**
 * iPhone�ŎB�e��������̊Ǘ�DB��update���K�v���ǂ���
 */
bool need_update_camera_db()
{
    return need_update_db(MEDIA_FILE_KIND::CAMERA_ROLL);
}

/**
 * iTunes�œ]����������̊Ǘ�DB��update���K�v���ǂ���
 */
bool need_update_video_db()
{
    return need_update_db(MEDIA_FILE_KIND::HOME_VIDEO);
}

/**
 * �ʐ^�p�̊Ǘ�DB��update���K�v���ǂ���
 */
bool need_update_picture_db()
{
    return need_update_db(MEDIA_FILE_KIND::PICTURE);
}
