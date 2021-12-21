"""
@author: Yano, Takayuki
"""

import os
import gettext
import tkinter as tk
import tkinter.ttk as ttk
import iphone_export.ui as iphone
from _ast import AugLoad


def init_translation():
    t = gettext.translation(
        domain = 'messages',
        localedir='locale',
        languages=['ja_JP'],
        fallback=True
    )
    _ = t.gettext
    t.install("_")

def music_list_frame():
    notebook.add(tab_two, text=_("Music files"))
    return tab_two

def main():
    init_translation()
    root = tk.Tk()

    root.title('ui sample')
    root.geometry('800x600')
    frame = tk.Frame(master = root,relief=tk.RAISED, borderwidth=1)

    global notebook
    global tab_two
    notebook = ttk.Notebook()
    tab_one = tk.Frame(notebook, bg='white')
    tab_two = tk.Frame(notebook, bg='blue')
    str = _("Export from iPhone")
    print("tabname:" + str)
    notebook.add(tab_one, text=_("Export from iPhone"), underline=0)
    notebook.pack(expand=True, fill='both', padx=10, pady=10)

    iphone_window = iphone.MainWindow()
    iphone_window.show(tab_one)
    iphone_window.setMusicExport(music_list_frame)

    root.mainloop()



if __name__ == '__main__':
    main()
