"""
Created on 2021/12/20

@author: Yano, Takayuki
"""

import tkinter as tk
import tkinter.ttk as ttk
from iphone_export import modelview as mv
import tkinter
import iphone_export.music_record as music


class MainWindow(object):
    def __init__(self):
        pass

    def show(self, frame, row_num=0, column_num=0):
        self.frame = frame

        elm = self.mv = mv.IPhoneExportUiHandler()

        #        lbl_iphone_export = ttk.Label(master=frame, text=_("Export from IPhone"))

        # 出力先ディレクトリ
        str = _("Choose Output Directory")
        print("dir2:" + str)
        lbl_export_dir = ttk.Label(
            master=frame, text=_("Choose Output Directory"))
        txt_out_dir = ttk.Entry(master=frame, width=60)
        txt_out_dir.insert(tkinter.END, elm.getDefaultDir())
        btn_select_dir = self.ExButton("...", h=2, w=4)
        btn_select_dir.bind("<1>", elm.onSelectDirClicked)
        elm.setTextBox(txt_out_dir)

        btn_export_all_music = self.ExButton(_("ALL Music Export"))
        btn_export_all_music.bind("<1>", elm.onExportAllMusicClicked)
        btn_export_all_video = self.ExButton(_("ALL Video Export"))
        btn_export_all_video.bind("<1>", elm.onExportAllVideoClicked)
        btn_export_all_picture = self.ExButton(_("ALL Picture Export"))
        btn_export_all_picture.bind("<1>", elm.onExportAllPictureClicked)

        btn_export_music = self.ExButton(_("Music Export"))
        btn_export_music.bind("<1>", elm.onExportMusicClicked)
        btn_export_video = self.ExButton(_("Video Export"))
        btn_export_video.bind("<1>", elm.onExportVideoClicked)
        btn_export_picture = self.ExButton(_("Picture Export"))
        btn_export_picture.bind("<1>", elm.onExportPictureClicked)

        btn_export_all = self.ExButton(_("All Export"), w=40)
        btn_export_all.bind("<1>", elm.onExportAllClicked)

        #        lbl_iphone_export.grid(row = 0, column = 0, sticky=tk.W, columnspan=3)

        # ディレクトリ選択
        lbl_export_dir.grid(row=1, column=0, sticky=tk.W, columnspan=3)
        txt_out_dir.grid(row=2, column=0, columnspan=3)
        btn_select_dir.grid(row=2, column=4)

        btn_export_all_music.grid(row=3, column=0, pady=1)
        btn_export_all_video.grid(row=3, column=1, pady=1)
        btn_export_all_picture.grid(row=3, column=2, pady=1)
        btn_export_music.grid(row=4, column=0, pady=1)
        btn_export_video.grid(row=4, column=1, pady=1)
        btn_export_picture.grid(row=4, column=2, pady=1)
        btn_export_all.grid(row=5, column=0, columnspan=3, pady=1)

    def ExButton(self, str, w=0, h=2):
        """
        わざわざ拡張クラス作るまでもないので、、、
        """
        if w > 0:
            return tk.Button(master=self.frame, text=str, height=h, width=w)
        else:
            return tk.Button(master=self.frame, text=str, height=h)

    def setMusicExport(self, callback):
        self.mv.setMusicExportFrameCallback(callback, self.show_music_list)

    def all_check(self):
        value = self.list[0].need_export.get()
        # print("checked:" + str(value))

        for item in self.list:
            item.need_export.set(value)

    def show_music_list(self, frame, list):
        """
        @brief 音楽ファイル一覧を表示する

        :param list: MusicRecordの配列
        """
        print("show_music_list called " + str(len(list)) + " " + str(list))
        self.mv.list = self.list = list

        #autopep8: off
        # タイトルを別に作成する
        title_row = [tk.Label(frame, text="#")
               ,tk.Checkbutton(frame, variable=list[0].need_export, command=self.all_check)
               ,tk.Label(frame, text=_("title"))
               ,tk.Label(frame, text=_("file name"))
               ,tk.Label(frame, text=_("file size"))
               ,tk.Label(frame, text=_("album artist"))
               ,tk.Label(frame, text=_("album title"))
               ,tk.Label(frame, text=_("artist"))
               ,tk.Label(frame, text=_("directory"))
               ]

        # 中身の作成
        self.rows = [[
                tk.Label(frame, text=str(i+1))
                ,tk.Checkbutton(frame, variable=list[i+1].need_export)
                ,tk.Entry(frame, textvariable=tk.StringVar(value=list[i+1].title))
                ,tk.Label(frame, text=list[i+1].value[int(music.col.FILE_NAME)])
                ,tk.Label(frame, text=list[i+1].value[int(music.col.FILE_SIZE)])
                ,tk.Entry(frame, textvariable=tk.StringVar(value=list[i+1].album_artist))
                ,tk.Entry(frame, textvariable=tk.StringVar(value=list[i+1].album))
                ,tk.Entry(frame, textvariable=tk.StringVar(value=list[i+1].artist))
                ,tk.Label(frame, text=list[i+1].value[int(music.col.DIRECTORY)])
            ] for i in range(len(list) - 1)]
        #autopep8: on
        self.rows.insert(0, title_row)

        tk.Button(frame, text=_("export"), command=self.mv.onExportClicked).grid(row=0, column=0)

        r = 1
        for row in self.rows:
            c = 0
            for item in row:
                item.grid(row=r, column=c)
                c = c + 1
            r = r + 1
