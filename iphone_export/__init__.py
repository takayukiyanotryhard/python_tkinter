import os
import gettext
from . import ui
from ctypes import *
import shutil
import glob
#from distutils.core import setup, Extension


def init_translation():
    # 翻訳ファイルを配置するディレクトリ
    path_to_locale_dir = os.path.abspath(
        os.path.join(
            os.path.dirname(__file__),
            '../locale'
        )
    )

    print(path_to_locale_dir)
    # 翻訳用クラスの設定
    translater = gettext.translation(
        'messages',                   # domain: 辞書ファイルの名前
        localedir=path_to_locale_dir, # 辞書ファイル配置ディレクトリ
        languages=['ja_JP'],          # 翻訳に使用する言語
        fallback=True                 # .moファイルが見つからなかった時は未翻訳の文字列を出力
    )

    # Pythonの組み込みグローバル領域に_という関数を束縛する
    translater.install()

def init_cpp_module():
    cwd = os.getcwd()
    print("cur:" + cwd)
    lib_path = os.path.abspath(
        os.path.join(
            os.path.dirname(__file__),
            '../extra/libs'
        )
    )
    dlls = glob.glob(lib_path + "/*.dll")
    for file in dlls:
        shutil.copy(file, cwd + "/")
#        shutil.copy()
    dll = cdll.LoadLibrary(lib_path + "/imobiledevice.dll")
    print("load:" + str(dll))

init_translation()
init_cpp_module()