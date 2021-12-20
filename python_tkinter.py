import tkinter as tk
import iphone_export as iphone

def main():
    root = tk.Tk()

    root.title('ui sample')
    root.geometry('800x600')
    frame = tk.Frame(master = root,relief=tk.RAISED, borderwidth=1)

    iphone.ui.show(frame)

    root.mainloop()

if __name__ == '__main__':
    main()