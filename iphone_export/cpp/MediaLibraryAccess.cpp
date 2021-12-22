#include <cstdint>
#include <Windows.h>
#include <cstdio>
#include <cstdarg>
#include "MediaLibraryAccess.h"

MediaLibraryAccess::MediaLibraryAccess(const wchar_t* path)
    :m_fpath(0)
    , m_dbh(0)
    , m_mode(JOURNAL_MODE::WAL)
{
    int len;
    if (!path) goto error;
    len = WideCharToMultiByte(CP_UTF8, 0, path, (int)wcslen(path), 0, 0, 0, 0);

    m_fpath = (char*)malloc((size_t)len + 1);
    if (!m_fpath) goto error;

    WideCharToMultiByte(CP_UTF8, 0, path, (int)wcslen(path), m_fpath, len, 0, 0);
    m_fpath[len] = '\0';
error:
    ;
}

MediaLibraryAccess::~MediaLibraryAccess()
{
    commit();
    disconnect();
    if (m_fpath) free(m_fpath);
}

int32_t MediaLibraryAccess::get_music_list(char*** list)
{
    int32_t retval = 0;
    const char* sql = R"(
SELECT i.item_pid, a.album, aa.album_artist, g.genre, i.track_number,e.track_count,
    i.disc_number, e.title, ia.item_artist, e.year, b.path as directory,
    e.location as file_name, e.file_size, c.composer, art.relative_path as art_path
FROM item as i
    INNER JOIN item_extra as e
        ON i.item_pid = e.item_pid
    INNER JOIN base_location as b
        ON i.base_location_id = b.base_location_id
    LEFT OUTER JOIN album as a
        ON i.album_pid = a.album_pid
    LEFT OUTER JOIN album_artist as aa
        ON i.album_artist_pid = aa.album_artist_pid
    LEFT OUTER JOIN genre as g
        ON i.genre_id = g.genre_id
    LEFT OUTER JOIN item_artist as ia
        ON i.item_artist_pid = ia.item_artist_pid
    LEFT OUTER JOIN composer as c
        ON i.composer_pid = c.composer_pid
    LEFT OUTER JOIN(
        SELECT entity_pid, relative_path FROM artwork
        INNER JOIN artwork_token
        on artwork.artwork_token = artwork_token.artwork_token
    )as art
        ON i.item_pid = art.entity_pid
WHERE i.media_type = 8
    ;
        )";
    if(!connect()) goto error;

    retval = read(sql, list);

error:
    disconnect();
    return retval;
}

bool MediaLibraryAccess::connect()
{
    int status = SQLITE_ERROR;
    if (!m_fpath) goto error;

    if (m_dbh) {
        status = SQLITE_OK;
        goto error;
    }
    status = sqlite3_open(m_fpath, &m_dbh);
    if (status != SQLITE_OK) goto error;

    m_mode = journal_mode();
    if (m_mode == JOURNAL_MODE::WAL)
        jounarl_mode_off();

error:
    return status == SQLITE_OK;
}

void MediaLibraryAccess::disconnect()
{
    if (m_dbh) sqlite3_close(m_dbh);
    m_dbh = 0;
}

char* trimhead(char* sql) {
    int len = (int)strlen(sql);
    char *p = sql;
    for (int i = 0; i <len; i++) {
        if ((sql[i] == '\n')
            || (sql[i] == ' ')
            || (sql[i] == '\t')) {
            p++;
        }
        else {
            break;
        }
    }
    return p;
}
void trim(char* sql) {
    int len = (int)strlen(sql);
    for (int i = len - 1; i >= 0; i--) {
        if ((sql[i] == '\n')
            || (sql[i] == ' ')
            || (sql[i] == '\t')) {
            sql[i] = '\0';
        }
        else {
            break;
        }
    }
}

void remove_intermediate_semicolon(char* sql) {
    int len;

    trim(sql);
    len = (int)strlen(sql);

    // 一番後ろはOK
    for (int i = len - 2; i >= 0; i--) {
        if (sql[i] == ';' || sql[i] == '\n') {
            sql[i] = ' ';
        }
    }
}

int MediaLibraryAccess::read(const char* orig_sql, char*** pdata_set)
{
    int retval = 0;
    char** data_set = 0;
    char* data_start = 0, * p;
    int status;
    sqlite3_stmt* stmt = 0;
    int count = 0;
    int column_count = 0, record_count;
    const char* count_template = "SELECT count(*) FROM (%s) as t;";
    char* sql = 0, *buf= 0;
    const char* col;
    int last_alloc = 0;

    if (!m_dbh) goto error;

    // 頭がselectで無ければ弾く
    if (!available_create_view(orig_sql)) goto error;

    // 最初にレコード数を取得して確保するサイズを予測する
    // 大体で
    buf = (char*)malloc(strlen(orig_sql) + 30);
    sql = buf;
    if (!sql) goto error;
    sprintf(sql, count_template, orig_sql);
    // 渡ってきたsqlが;で終わってたら取り除く
    remove_intermediate_semicolon(sql);
    sql = trimhead(sql);
    status = sqlite3_prepare_v2(m_dbh, sql, -1, &stmt, 0);
    if (status != SQLITE_OK) goto error;
    status = sqlite3_step(stmt);
    record_count = sqlite3_column_int(stmt, 0);
    if (!record_count) goto error;
    status = sqlite3_finalize(stmt);
    if (status != SQLITE_OK) goto error;
    // ここまでサイズ推定

    // ここから実データ取得
    sql = buf;
    sprintf(sql, "%s", orig_sql);
    remove_intermediate_semicolon(sql);
    sql = trimhead(sql);
    status = sqlite3_prepare_v2(m_dbh, sql, -1, &stmt, NULL);
    if (status != SQLITE_OK) goto error;

    column_count = sqlite3_column_count(stmt);
    if (!record_count || !column_count) goto error;

    last_alloc =
        // ポインタ分(null終端あり)
        (column_count * (record_count + 1) + 1) * sizeof(char*)
        // 1レコード当たり16文字くらいを想定
        + ((size_t)column_count * ((size_t)record_count + 1) + 1) * 32;

    data_set = (char**)malloc(last_alloc);
    if (!data_set) goto error;
    p = data_start = (char*)&data_set[column_count * (record_count + 1) + 1];
    ((char**)p)[-1] = 0;
    for (int i = 0; i < column_count; i++) {
        data_set[count++] = p;
        col = sqlite3_column_name(stmt, i);
        p += (size_t)sprintf(p, "%s", col) + 1;
    }

    if (last_alloc < 32)  goto error;
    // 実際のデータ取得
    while ((status = sqlite3_step(stmt)) == SQLITE_ROW) {
        for (int i = 0; i < column_count; i++) {
            data_set[count++] = p;
            col = (char*)sqlite3_column_text(stmt, i);
            p += (size_t)sprintf(p, "%s", col) + 1;
            // 足りなくなったら
            if (p > ((char*)data_set + last_alloc)) {
                // Todo: 基本無いので後でやる
            }
        }
    }

    *pdata_set = data_set;
    retval = column_count;

error:
    if (stmt) sqlite3_finalize(stmt); stmt = 0;
    if (buf) free(buf);
    return retval;
}

JOURNAL_MODE MediaLibraryAccess::journal_mode()
{
    JOURNAL_MODE mode = JOURNAL_MODE::OPEN_ERROR;
    sqlite3_stmt* stmt = 0;
    const char sql[] = "PRAGMA journal_mode;";
    int status;
    const uint8_t* result = 0;

    if (!m_dbh) goto error;

    status = sqlite3_prepare_v2(m_dbh, sql, -1, &stmt, NULL);
    if (status != SQLITE_OK) goto error;

    status = sqlite3_step(stmt);
    if (status != SQLITE_ROW) goto error;

    result = sqlite3_column_text(stmt, 0);
    if (!result) goto error;

    if (strcmp((const char*)result, "off") == 0) {
        mode = JOURNAL_MODE::OFF;
    }
    else if (strcmp((const char*)result, "wal") == 0) {
        mode = JOURNAL_MODE::WAL;
    }

    else {
        // それ以外はエラーとする
    }

error:
    if (stmt) sqlite3_finalize(stmt);
    return mode;
}

void make_sql(char* sql, const char* template_sql, ...) {
    int len;
    va_list ap;
    va_start(ap, template_sql);
    vsprintf(sql, template_sql, ap);
    va_end(ap);

    len = (int)strlen(sql);
    for (int i = 0; i < (len - 1); i++) {
        if (sql[i] == '\n') sql[i] = ' ';
    }
}


const char* MediaLibraryAccess::get_mode_str(JOURNAL_MODE mode)
{
    const char* p = 0;
    switch (mode) {
    case JOURNAL_MODE::OFF:
        p = "off";
        break;
    case JOURNAL_MODE::WAL:
        p = "wal";
        break;
    }
    return p;
}

void MediaLibraryAccess::journal_mode(JOURNAL_MODE mode)
{
    sqlite3_stmt* stmt = 0;
    const char temp[] = "PRAGMA journal_mode = %s;";
    char sql[MAX_PATH] = { 0 };
    int status;

    if (!m_dbh) goto error;
    make_sql(sql, temp, get_mode_str(mode));
    OutputDebugStringA(sql);
    OutputDebugStringA("\n");
    status = sqlite3_prepare_v2(m_dbh, sql, -1, &stmt, NULL);
error:
    if (stmt) sqlite3_finalize(stmt);
}


void MediaLibraryAccess::jounarl_mode_off()
{
    journal_mode(JOURNAL_MODE::OFF);
}


void MediaLibraryAccess::commit()
{
    if (m_dbh) {
        int ret = sqlite3_exec(m_dbh, "commit;", 0, 0, 0);
        if (m_mode != JOURNAL_MODE::OPEN_ERROR && m_mode != JOURNAL_MODE::OFF) {
            journal_mode(m_mode);
        }
        sqlite3_close_v2(m_dbh);
    }
    m_dbh = 0;
}


bool MediaLibraryAccess::available_create_view(const char* orig_sql)
{
    char* sql = 0;
    char* buf = 0;
    int len;
    bool retval = false;
    if (!orig_sql || !(*orig_sql)) goto error;

    len = (int)strlen(orig_sql);
    if (len <= 6) goto error;

    buf = (char*)malloc((size_t)len + 1);
    sql = buf;
    if (!sql) goto error;
    memcpy(sql, orig_sql, len);
    sql[len] = '\0';

    sql = trimhead(sql);

    for (int i = 0; i < 6; i++) {
        if ('a' <= sql[i] && sql[i] <= 'z') {
            sql[i] -= 'a' - 'A';
        }
    }

    retval = memcmp(sql, "SELECT", 6) == 0;

error:
    if (buf) free(buf);
    return retval;
}
