#include <Windows.h>
#include "Misc.h"
#include <shlobj.h>

/**
 * �A�v���P�[�V�������ŗL�Ɏg�p����f�B���N�g�����擾����
 */
void get_application_dir(wchar_t** path)
{
    wchar_t* wbuf, *p;
    int len;

    wbuf = (wchar_t*)malloc(MAX_PATH * sizeof(wchar_t));
    p = wbuf;
    if (!p) goto error;

    SHGetSpecialFolderPath(NULL, p, CSIDL_APPDATA, 0);

    len = (int)wcslen(p);
    p = p + len;
    
    // Todo: �b��œ���Ă���
    wsprintf(p, L"\\iphone_access");
    SHCreateDirectoryEx(NULL, wbuf, NULL);

    *path = wbuf;
error:
    ;
}
