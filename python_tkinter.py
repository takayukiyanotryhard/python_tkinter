"""
@author: Yano, Takayuki
"""

import os
import gettext
import tkinter as tk
import tkinter.ttk as ttk
import iphone_export.ui as iphone

def main():
    root = tk.Tk()

    root.title('ui sample')
    root.geometry('800x600')
    frame = tk.Frame(master = root,relief=tk.RAISED, borderwidth=1)

    notebook = ttk.Notebook()
    tab_one = tk.Frame(notebook, bg='white')
    tab_two = tk.Frame(notebook, bg='blue')
    notebook.add(tab_one, text=_("Export from iPhone"), underline=0)
    notebook.add(tab_two, text="tab2")
    notebook.pack(expand=True, fill='both', padx=10, pady=10)

    iphone.MainWindow().show(root,tab_one)

    root.mainloop()



if __name__ == '__main__':
    main()