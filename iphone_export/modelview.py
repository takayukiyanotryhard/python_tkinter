'''
Created on 2021/12/20

@author: Yano, Takayuki
'''
import os
import tkinter as tk
import tkinter.filedialog as filedialog
from _overlapped import NULL
from iphone_export import model as m
from lib2to3.fixer_util import String
import tkinter

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

    def onExportAllClicked(self, e):
        controller.export("", self.controller.EXPORT_TYPE_MUSIC)
        controller.export("", self.controller.EXPORT_TYPE_VIDEO)
        controller.export("", self.controller.EXPORT_TYPE_PICTURE)

    def onExportAllMusicClicked(self, e):
        controller.export("", self.controller.EXPORT_TYPE_MUSIC)

    def onExportAllVideoClicked(self, e):
        controller.export("", self.controller.EXPORT_TYPE_VIDEO)
    def onExportAllPictureClicked(self, e):
        controller.export("", self.controller.EXPORT_TYPE_PICTURE)
    def onExportMusicClicked(self, e):
        pass
    def onExportVideoClicked(self, e):
        pass
    def onExportPictureClicked(self, e):
        pass


    def setDeviceIndicator(self, btn2):
        self.btn = btn2

    def getDefaultDir(self):
        return "" + os.path.expanduser('~')

    def setTextBox(self, tb):
        self.txt_outdir = tb

    def onSelectDirClicked(self, e):
        """
        3点リーダがクリックされた場合
        表示中のディレクトリを初期ディレクトリとしてファイルを開くダイアログを開く
        表示中のディレクトリが存在しない場合はユーザディレクトリを初期ディレクトリとして開く
        """

        #os.path.expanduser("~")
        dpath = self.txt_outdir.get()
        if (os.path.exists(dpath)):
            pass
        else:
            dpath = getDefaultDir()

        ret = filedialog.askdirectory(initialdir=dpath)

        if(len(ret) > 0):
            self.txt_outdir.delete(0, tkinter.END)
            self.txt_outdir.insert(tkinter.END, ret)

        #Todo: after使ってもへこむものはへこむ
        #後で
