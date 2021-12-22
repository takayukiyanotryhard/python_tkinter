#include <Windows.h>
#include "Misc.h"
#include <shlobj.h>

/**
 * アプリケーションが固有に使用するディレクトリを取得する
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
    
    // Todo: 暫定で入れておく
    wsprintf(p, L"\\iphone_access");
    SHCreateDirectoryEx(NULL, wbuf, NULL);

    *path = wbuf;
error:
    ;
}
