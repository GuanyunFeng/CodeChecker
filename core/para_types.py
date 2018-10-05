import re
import sys
from abc import *

class parameter():
    __metaclass__ = ABCMeta
    def __init__(self, typename, name):
        self.size = -1       #单元大小，如int指针为4
        self.type_name = typename #指针类型名
        self.name = name #变量名

        if (self.type_name == "int" or 
            self.type_name = "unsigned" or
            self.type_name == "long" or
            self.type_name = "unsigned long"):
            self.size = 4
        elif (self.type_name == "short" or 
            self.type_name = "unsigned short"):
            self.size = 2
        elif (self.type_name == "char" or 
            self.type_name = "unsigned char"):
            self.size = 1
        elif self.type_name == "long long":
            self.size = 8




class para_pointer(parameter):
    def __init__(self, typename, name):
        super(para_pointer, self).__init__(self.typename, name)
        self.addr = -1 #-1代表指针未初始化， 0代表空指针， 1代表指向已分配空间
        self.size_total = -1 #指向区域的总大小
        self.count = -1  #单元数量


    def set_null(self):
        self.addr = 0


    def set_addr(self):
        self.addr = 1


    def free_addr(self):
        sefl.addr = -1


    def set_len(self, len):
        self.count = len
        if self.size != -1:
            self.size_total = self.size * self.count


    def set_sizetotal(self, sizetotal):
        self.size_total = sizetotal
        if self.size != -1:
            self.count = self.size_total/self.size


    def is_null(self):
        if self.addr == 0:
            return True


    def is_initialized(self):
        if self.addr == -1:
            return True




class pare_num(parameter):
    def __init__(self, typename, name):
        super(pare_num, self).__init__(typename, name)
        self.value = 0
        self.is_signed = False
        



class para_array(para_pointer):
    def __init__(self):
        self.value = []
        self.is_on_heap = False