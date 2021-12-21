#pragma once
#include "libimobiledevice/afc.h"
#include <cstdio>

#define ITUNES_DIRECTORY "/iTunes_Control/iTunes"
#define MEDIA_LIBRARY_DB_NAME "MediaLibrary.sqlitedb"

typedef struct str_iphone_file_info {
    //! 4GBà»è„ÇÕîÒëŒâû
    int st_size;
    int st_blocks;
    int st_nlink;
    //!
    int st_ifmt;
    int64_t st_mtime;
    int64_t st_birthtime;
    char* path;
    char* name;
}iphone_file_info_t;

class AfcWrapper
{
private:
    idevice_error_t m_last_error;
    idevice_t m_device;
    const char* udid;
    lockdownd_client_t m_client;
    lockdownd_service_descriptor_t m_service;
    afc_client_t m_afc_client;
    bool m_lockdown;
    bool m_client_service;
    wchar_t* m_localCDB;

    bool start_lockdown();
    bool start_client_service();
    bool session_start();
    void session_dispose();
    void dispose_client_service();
    void dispose_lockdown();
    int get_st_ifmt(char*);
    FILE* open_local_file4write(const char*);
    void set_local_cdb();

public:
    char* device_name;
    AfcWrapper();
    ~AfcWrapper();
    bool copy_media_library_db(wchar_t* tgt);
    bool open_iTunesCDB();
    bool get_file_info(const char* fpath, iphone_file_info_t* info);
    bool copy_file_from_device(const char* fpath, const wchar_t* tgt);
    bool copy_file_to_device(const char* fpath);
    const wchar_t* get_local_cdb();
    bool commit_cdb();
    void copy_directory(const char* dir, char* out_dir);
    bool read_directory(const char* dir, iphone_file_info_t*** pinfos);

};

