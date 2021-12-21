"""
@author: Yano, Takayuki
"""

import gettext
import tkinter as tk
import tkinter.ttk as ttk
import iphone_export.ui as iphone

app_title = "iPhone file transfer tool"


def init_translation():
    gettext.translation(
        domain="messages", localedir="locale", languages=["ja_JP"], fallback=True
    ).install("_")
    #_ = t.gettext
    #t.install("_")


def music_list_frame():
    notebook.add(tab_music_list, text=_("Music files"))
    return tab_music_list


def main():
    init_translation()
    root = tk.Tk()

    root.title(_(app_title))
    root.geometry("800x600")
    # frame = tk.Frame(master = root,relief=tk.RAISED, borderwidth=1)

    global notebook
    global tab_music_list
    notebook = ttk.Notebook()
    tab_iphone_main = tk.Frame(notebook, bg="white")
    tab_music_list = tk.Frame(notebook, bg="blue")
    str = _("Export from iPhone")
    print("tabname:" + str)
    notebook.add(tab_iphone_main, text=_("Export from iPhone"), underline=0)
    notebook.pack(expand=True, fill="both", padx=10, pady=10)

    iphone_window = iphone.MainWindow()
    iphone_window.show(tab_iphone_main)
    iphone_window.setMusicExport(music_list_frame)

    root.mainloop()


if __name__ == "__main__":
    main()
