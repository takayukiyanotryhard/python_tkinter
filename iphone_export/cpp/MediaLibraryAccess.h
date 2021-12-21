#pragma once
#include "sqlite3.h"

enum class JOURNAL_MODE {
    OFF,
    DELETE_MODE,
    TRUNCATE,
    PERSIST,
    MEMORY,
    WAL,
    OPEN_ERROR
};

class MediaLibraryAccess
{
private:
    char* m_fpath;
    ::sqlite3* m_dbh;
    JOURNAL_MODE m_mode;

public:
    MediaLibraryAccess(const wchar_t*);
    ~MediaLibraryAccess();
    int32_t get_music_list(char** list);
};

