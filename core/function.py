import re
import sys
from .para_types import *

class function:
    def __init__(self, func_all, ret_type, func_name, func):
        self.func_all = func_all
        self.ret_type = ret_type
        self.name = func_name
        self.func = func
        self.para = []
        self.line = -1

    def set_para(self, str):
        pass

    def set_line(self, txt):
        enter = re.compile(r'\n')
        count = txt.find(self.func_all)
        if count != -1:
            result = enter.findall(txt[:count])
            self.line = len(result) + 1




class func_list:
    def __init__(self, group):
        self.flist = []
        for g in group:
            f = function(g[0], g[1], g[2], g[4])
            self.flist.append(f)
    
    def names(self):
        names = []
        for f in self.flist:
            names.append(f.name)
        return names

    def funcs(self):
        funcs = []
        for f in self.flist:
            funcs.append(f.func)
        return funcs

    def paras(self):
        paras = []
        for f in self.flist:
            paras.append(f.para)
        return paras

    def lines(self):
        lines = []
        for f in self.flist:
            lines.append(f.line)
        return lines

    def set_lines(self, txt):
        for f in self.flist:
            f.set_line(txt)