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
    bool available_create_view(const char* sql);

public:
    MediaLibraryAccess(const wchar_t*);
    ~MediaLibraryAccess();
    int32_t get_music_list(char** list);
    bool connect();
    void disconnect();
    JOURNAL_MODE journal_mode();
    void journal_mode(JOURNAL_MODE);
    void commit();
    int read(const char* sql, char*** pdata_set);
    void jounarl_mode_off();
    const char* get_mode_str(JOURNAL_MODE mode);

};

