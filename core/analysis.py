#!/home/ypang/Softwares/anaconda3/bin/python
# -*- coding: utf-8 -*-
import re
import os
import sys
from graphviz import Digraph
import pygraphviz as gv
from .function import *
from .var_types import *



c_pattern = re.compile(r'(((?:const[ \t\*]+)?(void|int|char|long|double|float|unsigned|unsigned int|unsigned long|long long)[ \t\*]+(?:const[ \t*]+)?)(\w+)(\([^\;]*\))[^\;]*(\{([^{}]*(\{([^{}]*(\{([^{}]*(\{[^{}]*\})*[^\{\}]*?)*\})*[^\{\}]*?)*\})*[^\{\}]*?)*\}))', re.S)


def get_c_functions(filepath):
    f = open(filepath, "r")
    str = f.read()
    f.close()
    c_group = c_pattern.findall(str)
    functions = func_list(c_group)
    return functions


def check_num_overflow(functions):
    pass


def check_stack_overflow(functions):
    pass


def check_heap_overflow(function):
    pass


def scan_suspicious(function):
    print("开始函数内可疑函数的扫描...", function.name)
    level1 = ["gets"]
    level2 = ["strcpy", "memcpy", "strcat",
            "sprintf", "vsprintf", "sscanf","scanf", 
            "fscanf", "vfscanf", "vscanf", "vsscanf"]
    level3 = ["getchar", "fgetc", "getc", "fgets",
            "strncpy", "memcpy", "memncpy", "strncat"]
    key_words = level1.extend(level2).enxtend(level3)
    enter = re.compile(r'\n')
    for word in key_words:
        #print("正在寻找不安全的函数"+word+"...")
        pattern = re.compile(word+r'\(.*?\)')
        sentences = pattern.findall(function.func)
        for sentence in sentences:
            result = function.func.find(sentence)
            if result != -1:
                tmp = enter.findall(function.func[:result])
                line = function.line + len(tmp) + 1
                if word in level1:
                    print("极危险： "+word+" 行数：" + str(line))
                elif word in level2:
                    print("较危险： "+word+" 行数：" + str(line))
                elif word in level2:
                    print("较危险： "+word+" 行数：" + str(line))


def scan_global_var(code):
    global_vars = []
    code = re.sub(c_pattern, "", code)
    global_vars = scan_local_var(scan_global_var)
    return global_vars


def scan_local_var(function):
    local_vars = var_list()
    types = ["char", "unsigned char", "short", "unsigned short", "int", "unsigned int", "unsigned", "long", "unsigned long", "long long", "unsigned long long"]
    for var_type in types:
        declarations = []
        filter_expression="[\;\{]{1}[ \t\n]*("+var_type+"[ \t\*]+([\w_]+)(\[[\w_]*\])*[ \t]*(\={1}[^\;\,]*)*[ \t]*(\,[ \t\*]*([\w\_]+)(\[[\w\_]*\])*[ \t]*(\={1}[^\;\,]*)*[ \t]*)*\;)"
        var_pattern = re.compile(filter_expression, re.S)
        all_decl = var_pattern.findall(function)
        for decl in all_decl:
            declarations.append(decl[0])
            print(decl[0])

        #过滤出栈上面的数组，并获取其分配空间以及赋值情况
        arrays_filter1=var_type+"[ \t]+([\w_]+)(\[([\w\_]+)\])(?:[ \t]*\={1}[ \t]*\"{1}([\w]+)\"{1})?"
        arrays_filter2="\,[ \t]*([\w_]+)(\[([\w\_]+)\])(?:[ \t]*\={1}[ \t]*\"{1}([\w]+)\"{1})?"
        para_pattern1 = re.compile(arrays_filter1)#得到数组声明的第一个
        para_pattern2 = re.compile(arrays_filter2)#得到数组声明第二个及之后

        for declaration in declarations:
            parameters1 = para_pattern1.findall(declaration)
            parameters2 = para_pattern2.findall(declaration)

            for para in parameters1:
                tmp_var = var_pointer(var_type, para[0])
                if (para[2] == "" and para[3] == ""):
                    print("syntax error! 错误的数组声明语法。")
                if para[2] != "":
                    tmp_var.set_len(int(para[2]))
                if para[3] != "":
                    tmp_var.set_len(len(para[3])+1)
                    tmp_var.set_value(list(para[3]))
                print(tmp_var.type_name)
                print(tmp_var.name)
                print(tmp_var.count)
                print(tmp_var.value)
                print("")
                local_vars.append_array_var(tmp_var)

            for para in parameters2:
                tmp_var = var_pointer(var_type, para[0])
                if (para[2] == "" and para[3] == ""):
                    print("syntax error! 错误的数组声明语法。")
                if para[2] != "":
                    tmp_var.set_len(int(para[2]))
                if para[3] != "":
                    tmp_var.set_len(len(para[3])+1)
                    tmp_var.set_value(list(para[3]))
                print(tmp_var.type_name)
                print(tmp_var.name)
                print(tmp_var.count)
                print(tmp_var.value)
                print("")
                local_vars.append_array_var(tmp_var)
	
	    #获取var类型变量
        var_filter1=var_type+"[ \t]+([\w_]+)(?!\[)([ \t]*\=[ \t]*([^\,\;]+))?(?=[ \t]*([\,\;]+))"
        var_filter2="\,[ \t]*([\w_]+)(?!\[)([ \t]*\=[ \t]*([^\,\;]+))?(?=[ \t]*([\,\;]+))"
        para_pattern3 = re.compile(var_filter1)
        para_pattern4 = re.compile(var_filter2)
        for declaration in declarations:
            parameters3 = para_pattern3.findall(declaration)
            parameters4 = para_pattern4.findall(declaration)
	
            for para in parameters3:
                tmp_var = var_num(var_type, para[0])
                if para[2].strip().isdigit():
                    tmp_var.set_value(int(para[2].strip()))
                print(tmp_var.type_name)
                print(tmp_var.name)
                print(tmp_var.value)
                print("")
                local_vars.append_num_var(tmp_var)
            for para in parameters4:
                tmp_var = var_num(var_type, para[0])
                if para[2].strip().isdigit():
                    tmp_var.set_value(int(para[2].strip()))
                print(tmp_var.type_name)
                print(tmp_var.name)
                print(tmp_var.value)
                print("")
                local_vars.append_num_var(tmp_var)

	
	    #获取var*型变量
        var_pointer_filter1=var_type+"[ \t]*[\*]+[ \t]*([\w\_]+)(?=[ \t]*[\,\;]+)"
        var_pointer_filter2="\,[\*]+[ \t]*([\w\_]+)(?=[ \t]*[\,\;]+)"
        para_pattern5 = re.compile(var_pointer_filter1)
        para_pattern6 = re.compile(var_pointer_filter2)
        for declaration in declarations:
            parameters5 = para_pattern5.findall(declaration)
            parameters6 = para_pattern6.findall(declaration)
            for para in parameters5:
                tmp_var = var_pointer(var_type, para[0])
                local_vars.append_array_var(tmp_var)
            for para in parameters6:
                tmp_var = var_pointer(var_type, para[0])
                local_vars.append_array_var(tmp_var)
'''
    for l1 in local_vars.num_vars:
        print(l1.type_name)
        print(l1.name)
        print(l1.value)
        print("")

    for l2 in local_vars.array_vars:
        print(l1.type_name)
        print(l1.name)
        #print(l1.count)
        print(l1.value)
        print("")
'''
'''
	#获取malloc分配空间
	    malloc_space="([\w_]+)\={1}\({1}"+var_type+"[ \t]*\*[ \t]*\){1}[ \t]*malloc[ \t]*\({1}[ \t]*([\d]+)[ \t]*\*{1}[ \t]*sizeof\({1}"+var_type+"\){2}"
	    para_pattern7 = re.compile(malloc_space)
	    malloc=para_pattern7.findall(function)
	    for pointer in malloc:
	        var_pointer[pointer[0]]=pointer[1]
'''