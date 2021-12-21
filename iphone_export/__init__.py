import os
import gettext
from . import ui
from ctypes import *
import shutil
import glob
#from distutils.core import setup, Extension

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

init_cpp_module()