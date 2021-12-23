"""
Created on 2021/12/20

@author: Yano, Takayuki
"""
import os
import tkinter.filedialog as filedialog
from _overlapped import NULL
from iphone_export import model as m
import tkinter


class IPhoneExportUiHandler(object):
    """
    classdocs
    """

    def __init__(self):
        """
        Constructor

        """
        self.btn = NULL

        self.controller = m.IPhoneExport()

    def onExportAllClicked(self, e):
        self.controller.export("", self.controller.EXPORT_TYPE_MUSIC)
        self.controller.export("", self.controller.EXPORT_TYPE_VIDEO)
        self.controller.export("", self.controller.EXPORT_TYPE_PICTURE)

    def onExportAllMusicClicked(self, e):
        self.controller.export("", self.controller.EXPORT_TYPE_MUSIC)

    def onExportAllVideoClicked(self, e):
        self.controller.export("", self.controller.EXPORT_TYPE_VIDEO)

    def onExportAllPictureClicked(self, e):
        self.controller.export("", self.controller.EXPORT_TYPE_PICTURE)

    def onExportMusicClicked(self, e):
        frame = self.frame_callback()
        print("frame:" + str(frame))
        print("list_callback:" + str(self.list_callback))
        self.list_callback(frame, self.controller.get_music_list())

    def onExportVideoClicked(self, e):
        pass

    def onExportPictureClicked(self, e):
        pass

    def onExportClicked(self):
        # listの内、チェックされている項目を取得する
        list = self.list

        exports = [item for item in list if item.need_export.get()]
        self.controller.export_specific(exports)

        # モデルに個別のエクスポートアイテムを通知してエクスポートする
    def setExportListCallback(self, callback):
        self.get_export_list = callback

    def setDeviceIndicator(self, btn2):
        self.btn = btn2

    def getDefaultDir(self):
        return "" + os.path.expanduser("~")

    def setTextBox(self, tb):
        self.txt_outdir = tb

    def onSelectDirClicked(self, e):
        """
        3点リーダがクリックされた場合
        表示中のディレクトリを初期ディレクトリとしてファイルを開くダイアログを開く
        表示中のディレクトリが存在しない場合はユーザディレクトリを初期ディレクトリとして開く
        """

        # os.path.expanduser("~")
        dpath = self.txt_outdir.get()
        if os.path.exists(dpath):
            pass
        else:
            dpath = getDefaultDir()

        ret = filedialog.askdirectory(initialdir=dpath)

        if len(ret) > 0:
            self.txt_outdir.delete(0, tkinter.END)
            self.txt_outdir.insert(tkinter.END, ret)

        # Todo: 押したときにボタンがへこむ
        # after使っても変わらなかった
        # 後まわし

    def setMusicExportFrameCallback(self, callback, list_callback):
        self.frame_callback = callback
        self.list_callback = list_callback
