'''
Created on 2021/12/20

@author: th-yano
'''
import tkinter as tk
from _overlapped import NULL

class MyClass(object):
    '''
    classdocs
    '''

    def __init__(self):
        '''
        Constructor

        '''
        self.btn = NULL

    def lbl_click(self, e):
        print(self)
        if (self.btn['state'] == tk.NORMAL):
            self.btn['state'] = tk.DISABLED
        else:
            self.btn['state'] = tk.NORMAL
        
        
    def setTarget(self, btn2):
        self.btn = btn2
        