import re
import sys
from .para_types import *

class function:
    def __init__(self, ret_type, func_name, func):
        self.ret_type = ret_type
        self.name = func_name
        self.func = func
        self.para = []

    def set_para(self, str):
        pass


class func_list:
    def __init__(self, group):
        self.list = []
        for g in group:
            f = function(g[1], g[2], g[4])
            list.append(f)
    
    def names(self):
        names = []
        for f in self.list:
            names.append(f.name)
        return names

    def funcs(self):
        funcs = []
        for f in self.list:
            funcs.append(f.func)
        return funcs

    def paras(self):
        paras = []
        for f in self.list:
            paras.append(f.para)
        return paras