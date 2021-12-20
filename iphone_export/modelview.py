'''
Created on 2021/12/20

@author: th-yano
'''
import os
import tkinter as tk
import tkinter.filedialog as filedialog
from _overlapped import NULL
from iphone_export import model as m
from lib2to3.fixer_util import String

class IPhoneExportUiHandler(object):
    '''
    classdocs
    '''

    def __init__(self):
        '''
        Constructor

        '''
        self.btn = NULL

        self.controller = m.IPhoneExport()



    def onExportButtonClicked(self, e):
        controller.export("", 1)


    def setDeviceIndicator(self, btn2):
        self.btn = btn2

    def getDefaultDir(self):
        return "" + os.path.expanduser('~')

    def onSelectDirClicked(self, e):
        """
        3点リーダがクリックされた場合
        表示中のディレクトリを初期ディレクトリとしてファイルを開くダイアログを開く
        表示中のディレクトリが存在しない場合はユーザディレクトリを初期ディレクトリとして開く
        """
        
        #os.path.expanduser("~")
        ret = filedialog.askdirectory()
