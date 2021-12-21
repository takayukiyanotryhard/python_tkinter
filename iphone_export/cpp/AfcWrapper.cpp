#include <cstdlib>
#include <cstring>
#include <Windows.h>
#include "AfcWrapper.h"
#include "Misc.h"

constexpr auto BUFFER_SIZE = 0x10000;
constexpr auto AFC_PROT = "com.apple.afc";
constexpr auto TOOL_NAME = "iphone_access";

AfcWrapper::AfcWrapper()
    :m_device(NULL)
    , udid(NULL)
    , m_client(NULL)
    , device_name(0)
    , m_service(NULL)
    , m_afc_client(NULL)
    , m_lockdown(false)
    , m_client_service(false)
    , m_localCDB(0)
{
    lockdownd_error_t lockdown_error;
    m_last_error = idevice_new_with_options(&m_device, udid, (idevice_options)((int)IDEVICE_LOOKUP_USBMUX | (int)IDEVICE_LOOKUP_NETWORK));
    if (!m_device) goto error;

    lockdown_error = lockdownd_client_new_with_handshake(m_device, &m_client, TOOL_NAME);
    if (lockdown_error != LOCKDOWN_E_SUCCESS) goto error;

    lockdown_error = lockdownd_get_device_name(m_client, &device_name);
    if (lockdown_error != LOCKDOWN_E_SUCCESS || !device_name) goto error;

error:;

}

AfcWrapper::~AfcWrapper()
{
    if (m_localCDB) free(m_localCDB);
    session_dispose();
    if (device_name) free(device_name);
    if (m_device) idevice_free(m_device);
    if (m_client) lockdownd_client_free(m_client);

}

bool AfcWrapper::copy_media_library_db(wchar_t* tgt_orig)
{
    bool retval = false;
    wchar_t tgt[MAX_PATH] = { 0 };

    if (!tgt_orig) goto error;

    retval = copy_file_from_device(ITUNES_DIRECTORY "/" MEDIA_LIBRARY_DB_NAME, tgt_orig);
    swprintf(tgt, L"%ws-shm", tgt_orig);
    retval = copy_file_from_device(ITUNES_DIRECTORY "/" MEDIA_LIBRARY_DB_NAME "-shm", tgt);
    swprintf(tgt, L"%ws-wal", tgt_orig);
    retval = copy_file_from_device(ITUNES_DIRECTORY "/" MEDIA_LIBRARY_DB_NAME "-wal", tgt);
error:
    return retval;
}

bool AfcWrapper::open_iTunesCDB()
{
    return false;
}

bool AfcWrapper::get_file_info(const char* fpath, iphone_file_info_t* info)
{
    bool retval = false;
    char** infolist = 0;
    char** p;
    afc_error_t afc_error;
    if (!info) goto error;
    if (!fpath || !(*fpath)) goto error;

    if (!session_start()) goto error;

    afc_error = afc_get_file_info(m_afc_client, fpath, &infolist);
    if (afc_error != AFC_E_SUCCESS || !infolist) goto error;
    p = infolist;
    while (*p) {
        if (strcmp(*p, "st_size") == 0) {
            info->st_size = atoi(*(p + 1));
        }
        else if (strcmp(*p, "st_blocks") == 0) {
            info->st_blocks = strtol(*(p + 1), NULL, 0);
        }
        else if (strcmp(*p, "st_nlink") == 0) {
            info->st_nlink = strtol(*(p + 1), NULL, 0);
        }
        else if (strcmp(*p, "st_ifmt") == 0) {
            info->st_ifmt = get_st_ifmt(*(p + 1));
            retval = true;
        }
        else if (strcmp(*p, "st_mtime") == 0) {
            info->st_mtime = strtoll(*(p + 1), NULL, 0);
        }
        else if (strcmp(*p, "st_birthtime") == 0) {
            info->st_birthtime = strtoll(*(p + 1), NULL, 0);
        }
        free(*p); ++p;
        free(*p); ++p;
    }
error:
    if (infolist) free(infolist);
    return retval;
}

bool AfcWrapper::copy_file_from_device(const char* fpath, const wchar_t* tgt)
{
    uint64_t file = 0;
    iphone_file_info_t info = { 0 };
    uint32_t read_size = 0;
    uint32_t ttl_read = 0;
    bool retval = false;
    afc_error_t afc_error;
    FILE* fp = 0;
    char* buf = 0;
    if (!session_start()) goto error;
    if (!get_file_info(fpath, &info)) goto error;
    if (!(fp = _wfopen(tgt, L"wb"))) goto error;

    buf = (char*)malloc(BUFFER_SIZE);
    if (!buf) goto error;

    afc_error = afc_file_open(m_afc_client, fpath, afc_file_mode_t::AFC_FOPEN_RDONLY, &file);
    if (afc_error != AFC_E_SUCCESS || !file) goto error;

    do {
        afc_error = afc_file_read(m_afc_client, file, buf, BUFFER_SIZE, &read_size);
        fwrite(buf, read_size, 1, fp);
        ttl_read += read_size;
        retval = true;
    } while (ttl_read < (uint32_t)info.st_size);
    afc_file_close(m_afc_client, file);

error:
    if (buf) free(buf);
    if (fp)fclose(fp);
    return retval;
}

bool AfcWrapper::copy_file_to_device(const char* fpath)
{
    return false;
}

const wchar_t* AfcWrapper::get_local_cdb()
{
    return nullptr;
}

bool AfcWrapper::commit_cdb()
{
    return false;
}

void AfcWrapper::copy_directory(const char* dir, char* out_dir)
{
}

bool AfcWrapper::read_directory(const char* dir, iphone_file_info_t*** pinfos)
{
    return false;
}

bool AfcWrapper::start_lockdown()
{
    lockdownd_error_t lock_error;
    if (m_lockdown) goto error;

    if (!m_service) {
        if (m_client) {
            lock_error = lockdownd_start_service(m_client, AFC_PROT, &m_service);
            if (lock_error != LOCKDOWN_E_SUCCESS || !m_service) goto error;
            m_lockdown = true;
        }
    }
error:
    return m_lockdown;
}

bool AfcWrapper::start_client_service()
{
    afc_error_t afc_error;
    if (m_client_service) goto error;

    afc_error = afc_client_new(m_device, m_service, &m_afc_client);
    if (afc_error != AFC_E_SUCCESS || !m_afc_client) goto error;
    afc_error = afc_client_start_service(m_device, &m_afc_client, AFC_PROT);
    if (afc_error != AFC_E_SUCCESS || !m_afc_client) goto error;
    m_client_service = true;
error:
    return m_client_service;

}

bool AfcWrapper::session_start()
{
    return start_lockdown() && start_client_service();
}

void AfcWrapper::session_dispose()
{
    dispose_client_service();
    dispose_lockdown();
}

void AfcWrapper::dispose_client_service()
{
    m_client_service = false;
    if (m_afc_client) afc_client_free(m_afc_client);
    m_afc_client = NULL;

}

void AfcWrapper::dispose_lockdown()
{
    m_lockdown = false;
    if (m_service) lockdownd_service_descriptor_free(m_service);
    m_service = NULL;

}

/**
 * ファイル種別を取得する
 * @param st_mode S_IFMTでマスクされた後の値の定義文字列
 * @return S_IFMTでマスクされた値
 */
int AfcWrapper::get_st_ifmt(char* st_mode)
{
    int retval = 0;
    // Windowsで読めるのは3種類だけ
    /**
    if (strcmp(st_mode, "S_IFSOCK") == 0) {
    } else if (strcmp(st_mode, "S_IFLNK") == 0) {
    } else if (strcmp(st_mode, "S_IFBLK") == 0) {

    } else */if (strcmp(st_mode, "S_IFREG") == 0) {
        retval = S_IFREG;
    }
    else if (strcmp(st_mode, "S_IFDIR") == 0) {
        retval = S_IFDIR;
    }
    else if (strcmp(st_mode, "S_IFCHR") == 0) {
        retval = S_IFCHR;
    }/*
     else if (strcmp(st_mode, "S_IFIFO") == 0) {
    } */
    return retval;
}

FILE* AfcWrapper::open_local_file4write(const char*)
{
    return nullptr;
}

void AfcWrapper::set_local_cdb()
{
}
