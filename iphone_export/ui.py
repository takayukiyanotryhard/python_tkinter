'''
Created on 2021/12/20

@author: th-yano
'''

import tkinter as tk
from iphone_export import modelview as mv
from tkinter.constants import DISABLED

def show(frame, row_num = 0, column_num = 0):
    frame.grid(row=row_num, column=column_num)

    elm = mv.IPhoneExportUiHandler()

    lbl_iphone_export = tk.Label(master=frame, text=_("Export from IPhone"))


    lbl_export_dir = tk.Label(master=frame, text = _("Choose Output Directory"))
    txt_out_dir = tk.Entry(master=frame, text=elm.getDefaultDir())

    btn_select_dir = tk.Button(master=frame, text = "...")
    btn_select_dir.bind("<1>", elm.onSelectDirClicked)

    btn_export_music = tk.Button(master = frame, text=_("Music Export"))
    btn_export_music.bind("<1>", elm.onExportButtonClicked)

    btn2 = tk.Button(master = frame, text="kakikukeko", state = DISABLED)

    lbl_iphone_export.grid(row = 0, column = 0)

    # ディレクトリ選択
    lbl_export_dir.grid(row = 1, column = 0)
    txt_out_dir.grid(row = 2, column = 0)
    btn_select_dir.grid(row = 2, column = 1)

    btn_export_music.grid(row = 3, column = 0)
    btn2.grid(row = 4, column = 0)

    elm.setDeviceIndicator(btn2)
