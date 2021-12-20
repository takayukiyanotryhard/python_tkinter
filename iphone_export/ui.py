'''
Created on 2021/12/20

@author: Yano, Takayuki
'''

import tkinter as tk
import tkinter.ttk as ttk
from iphone_export import modelview as mv
from tkinter.constants import DISABLED
import tkinter

class MainWindow(object):
    def __init__(self):
        pass

    def show(self, root,frame, row_num = 0, column_num = 0):
        self.frame = frame

        elm = mv.IPhoneExportUiHandler()

#        lbl_iphone_export = ttk.Label(master=frame, text=_("Export from IPhone"))

        # 出力先ディレクトリ
        str = _("Choose Output Directory")
        print("dir2:" + str)
        lbl_export_dir = ttk.Label(master=frame, text = _("Choose Output Directory"))
        txt_out_dir = ttk.Entry(master=frame, width=60)
        txt_out_dir.insert(tkinter.END, elm.getDefaultDir())
        btn_select_dir = self.ExButton("...", h=2, w=4)
        btn_select_dir.bind("<1>", elm.onSelectDirClicked)
        elm.setRoot(root)
        elm.setTextBox(txt_out_dir)

        btn_export_all_music = self.ExButton(_("ALL Music Export"))
        btn_export_all_music.bind("<1>", elm.onExportAllMusicClicked)
        btn_export_all_video =  self.ExButton(_("ALL Video Export"))
        btn_export_all_video.bind("<1>", elm.onExportAllVideoClicked)
        btn_export_all_picture =self.ExButton(_("ALL Picture Export"))
        btn_export_all_picture.bind("<1>", elm.onExportAllPictureClicked)

        btn_export_music = self.ExButton(_("Music Export"))
        btn_export_music.bind("<1>", elm.onExportAllMusicClicked)
        btn_export_video = self.ExButton(_("Video Export"))
        btn_export_video.bind("<1>", elm.onExportVideoClicked)
        btn_export_picture = self.ExButton(_("Picture Export"))
        btn_export_picture.bind("<1>", elm.onExportPictureClicked)

        btn_export_all = self.ExButton(_("All Export"), w=40)
        btn_export_all.bind("<1>", elm.onExportAllClicked)
    #    root.grid_columnconfigure(0, weight=1)

#        lbl_iphone_export.grid(row = 0, column = 0, sticky=tk.W, columnspan=3)

        # ディレクトリ選択
        lbl_export_dir.grid(row = 1, column = 0, sticky=tk.W, columnspan=3)
        txt_out_dir.grid(row = 2, column = 0, columnspan = 3)
        btn_select_dir.grid(row = 2, column = 4)

        btn_export_all_music.grid(row = 3, column = 0, pady=1)
        btn_export_all_video.grid(row = 3, column = 1, pady=1)
        btn_export_all_picture.grid(row = 3, column = 2, pady=1)
        btn_export_music.grid(row = 4, column = 0, pady=1)
        btn_export_video.grid(row = 4, column = 1, pady=1)
        btn_export_picture.grid(row = 4, column = 2, pady=1)
        btn_export_all.grid(row = 5, column = 0, columnspan = 3, pady=1)


    def ExButton(self, str, w=0, h=2):
        """
        わざわざ拡張クラス作るまでもないので、、、
        """
        if (w > 0):
            return tk.Button(master=  self.frame, text = str, height=h, width=w)
        else:
            return tk.Button(master=  self.frame, text = str, height=h)
