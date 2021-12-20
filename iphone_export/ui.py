'''
Created on 2021/12/20

@author: th-yano
'''

import tkinter as tk
from iphone_export import modelview as mv
from tkinter.constants import DISABLED

def show(frame, row_num = 0, column_num = 0):
    frame.grid(row=row_num, column=column_num)

    elm = mv.MyClass()

    label = tk.Button(master = frame, text="aiueo")
    label.bind("<1>", elm.lbl_click)
    label.pack()

    btn2 = tk.Button(master = frame, text="kakikukeko", state = DISABLED)
    btn2.pack()

    elm.setTarget(btn2)