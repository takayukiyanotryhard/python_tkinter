#include <cstdint>
#include <Windows.h>
#include "MediaLibraryAccess.h"

MediaLibraryAccess::MediaLibraryAccess(const wchar_t* path)
    :m_fpath(0)
    , m_dbh(0)
    , m_mode(JOURNAL_MODE::WAL)
{
    int len;
    if (!path) goto error;
    len = WideCharToMultiByte(CP_UTF8, 0, path, wcslen(path), 0, 0, 0, 0);

    m_fpath = (char*)malloc((size_t)len + 1);
    if (!m_fpath) goto error;

    WideCharToMultiByte(CP_UTF8, 0, path, wcslen(path), m_fpath, len, 0, 0);
error:
    ;
}

MediaLibraryAccess::~MediaLibraryAccess()
{

}

int32_t MediaLibraryAccess::get_music_list(char** list)
{
    return 5;
}
