import os
from ctypes import cdll
import shutil
import glob
import global_variables as g

# from distutils.core import setup, Extension


def init_cpp_module():
    cwd = os.getcwd()
    print("cur:" + cwd)
    lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../extra/libs"))
    dlls = glob.glob(lib_path + "/*.dll")
    for file in dlls:
        shutil.copy(file, cwd + "/")
    #    dll = cdll.LoadLibrary(lib_path + "/imobiledevice.dll")
    g.dll = cdll.LoadLibrary(lib_path + "/dlltest.dll")
    print("load:" + str(g.dll))


init_cpp_module()
