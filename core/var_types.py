import re
import sys
from abc import *

class variables:
    __metaclass__ = ABCMeta


    def __init__(self, typename, name):
        self.size = -1       #单元大小，如int指针为4
        self.type_name = typename #指针类型名
        self.name = name #变量名

        if (self.type_name == "int" or 
            self.type_name == "unsigned" or
            self.type_name == "long" or
            self.type_name == "unsigned long"):
            self.size = 4
        elif (self.type_name == "short" or 
            self.type_name == "unsigned short"):
            self.size = 2
        elif (self.type_name == "char" or 
            self.type_name == "unsigned char"):
            self.size = 1
        elif self.type_name == "long long":
            self.size = 8


    @abstractmethod
    def initializ(self):
        pass


    @abstractmethod
    def set_value(self):
        pass





class var_pointer(variables):
    def __init__(self, typename, name):
        super(var_pointer, self).__init__(typename, name)
        self.addr = -1 #-1代表指针未初始化， 0代表空指针， 1代表指向已分配空间
        self.size_total = -1 #指向区域的总大小
        self.count = -1  #单元数量
        self.value_len = -1
        self.value = []
        self.is_on_heap = False


    def copy_array(self, string):
        if len(string) + 1 > self.length:
            if self.is_on_heap:
                print("heap overflow!")
            else:
                print("stack overflow!")
            return False
        else:
            self.value = list(string)
            self.value_len = len(string)
            return True

    
    def append_array(self, string):
        if len(string) + len(self.value) + 1 > self.length:
            if self.is_on_heap:
                print("heap overflow!")
            else:
                print("stack overflow!")
            return False
        else:
            self.value = self.value + list(string)
            self.valid_len += len(string)
            return True


    def set_value(self, ls):
        self.value.clear()
        self.value = list(ls)
        self.value_len = len(self.value)


    def set_len(self, len):
        self.count = len
        if self.size != -1:
            self.size_total = self.size * self.count


    def set_sizetotal(self, sizetotal):
        self.size_total = sizetotal
        if self.size != -1:
            self.count = self.size_total/self.size


    def set_null(self):
        self.addr = 0


    def set_addr(self):
        self.addr = 1


    def free_addr(self):
        self.addr = -1


    def is_null(self):
        if self.addr == 0:
            return True
        return False


    def is_initialized(self):
        if self.addr == -1:
            return True
        return False


    def has_value(self):
        if self.value_len == -1:
            return False
        return True




class var_num(variables):
    def __init__(self, typename, name):
        super(var_num, self).__init__(typename, name)
        self.value = 0
        self.is_signed = True
        self.max = 0
        self.min = 0

        if (self.type_name == "unsigned" or
            self.type_name == "unsigned long" or
            self.type_name == "unsigned int" or
            self.type_name == "unsigned short" or
            self.type_name == "unsigned char"):
            self.is_signed = False
            self.min = 0
            self.max = pow(2, self.size * 8) - 1
        else:
            self.is_signed = True
            self.min = - pow(2, self.size * 8 - 1) + 1
            self.max = pow(2, self.size * 8 -1) - 1


    def set_value(self, num):
        if (num < self.min or num > self.max):
            return False
        self.value = num
        False




class var_list:
    def __init__(self):
        self.num_vars = []
        self.array_vars = []

    def clear_num_vars(self):
        self.num_vars.clear()
    
    def clear_array_vars(self):
        self.array_vars.clear()

    def append_num_var(self, num_var):
        self.num_vars.append(num_var)

    def append_array_var(self, p_var):
        self.array_vars.append(p_var)

    def search_array_var(self, name):
        for var in self.array_vars:
            if var.name == name:
                return var
        return None