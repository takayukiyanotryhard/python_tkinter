# やること
1. プリコンパイル済みヘッダーを使用しない
1. インクルードディレクトリの設定   
C/C++ -> 全般 -> 追加のインクルードディレクトリ  
$(ProjectDir)extra\include
1. プリプロセッサに_CRT_SECURE_NO_WARNINGS(_wfopen)
1. プリプロセッサに_CRT_NON_CONFORMING_SWPRINTFS(swprintf)
1. ライブラリの追加(imobiledevice.lib, sqlite3.lib)  
    ```
    dumpbin /exports xxx.dll > xxx.def
    
    rem 関数名だけを抽出
    rem EXPORTS
    rem afc_client_free
    rem afc_client_new
    
    lib /dev:xxx.def /machine:x64 
    ```
1. ライブラリディレクトリの追加  
リンカー -> 全般 -> 追加のライブラリディレクトリ  
$(ProjectDir)\extra\libs
