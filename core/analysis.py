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


def scan_num_overflow(function, local_vars):
    out = "正在扫描函数"+function.name+"...\n"
    func = function.func
    types = ["char", "unsigned char", "short", "unsigned short", "int", "unsigned int", "unsigned", "long", "unsigned long", "long long", "unsigned long long"]
    for var_type in types:
        filter_expression="(?<=[\;\{])[ \t\n]*("+var_type+"[ \t\*]+([\w_]+)(\[[\w_]*\])?[ \t]*(\=[^\;\,]*)?[ \t]*(\,[ \t\*]*([\w_]+)(\[[\w_]*\])?[ \t]*(\=[^\;\,]*)?[ \t]*)*\;)"
        var_pattern = re.compile(filter_expression, re.S)
        func = re.sub(var_pattern, "", func)

    equal = re.compile("[^\w_]+([\w_]+)[ \t]*=[ \t]*([\w_]+)[^\w_]+")
    result = equal.findall(func)
    for r in result:
        v1 = local_vars.search_num_var(r[0])
        v2 = local_vars.search_num_var(r[1])
        if  v1 != None and v2 != None:
            if v1.size < v2.size:
                out += "可能存在整数宽度溢出！\n"
                print("可能存在整数宽度溢出！")
    out += "\n"
    return out





def scan_nullp(function, local_vars):
    out = "正在扫描函数"+function.name+"...\n"
    func = function.func
    pointers = []
    types = ["char", "unsigned char", "short", "unsigned short", "int", "unsigned int", "unsigned", "long", "unsigned long", "long long", "unsigned long long"]
    for v in local_vars.array_vars:
        if v.is_on_heap:
            pointers.append(v)

    for var_type in types:
        filter_expression="(?<=[\;\{])[ \t\n]*("+var_type+"[ \t\*]+([\w_]+)(\[[\w_]*\])?[ \t]*(\=[^\;\,]*)?[ \t]*(\,[ \t\*]*([\w_]+)(\[[\w_]*\])?[ \t]*(\=[^\;\,]*)?[ \t]*)*\;)"
        var_pattern = re.compile(filter_expression, re.S)
        func = re.sub(var_pattern, "", func)

    for p in pointers:
        if p.addr != 1:
            equal = re.compile("[^\w_]*"+p.name+"[ \t]*=[ \t]*([\w_]+)[ \t]*(?=[\;\,])")
            result = equal.findall(func)
            func = re.sub(equal, "", func)
            for r in result:
                if r != "NULL":
                    p.addr = 1
    
    for p in pointers:
        if p.addr != 1:
            use = re.compile("[^\w_]+"+p.name+"[^\w_]+")
            result = use.findall(func)
            if len(result) != 0:
                out += "发现空指针"+p.name+"的使用！\n"
                print("发现空指针"+p.name+"的使用！")
    out += "\n"
    return out



def scan_suspicious(function, global_vars, local_vars):
    out = "正在扫描函数"+function.name+"内的可疑函数...\n"
    print("正在扫描函数"+function.name+"内的可疑函数...")
    level1 = ["gets"]
    level2 = ["strcpy", "memcpy", "strcat",
            "sprintf", "vsprintf", "sscanf","scanf", 
            "fscanf", "vfscanf", "vscanf", "vsscanf"]
    level3 = ["getchar", "fgetc", "getc", "fgets",
            "strncpy", "memcpy", "memncpy", "strncat"]
    key_words = []
    key_words.extend(level1)
    key_words.extend(level2)
    key_words.extend(level3)
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
                    out += "极危险： "+word+" 行数：" + str(line) + "\n"
                    print("极危险： "+word+" 行数：" + str(line))
                elif word in level2:
                    out += "较危险： "+word+" 行数：" + str(line) + "\n"
                    print("较危险： "+word+" 行数：" + str(line))
                elif word in level3:
                    out += "低危险： "+word+" 行数：" + str(line) + "\n"
                    print("低危险： "+word+" 行数：" + str(line))
            if word == "strcpy":
                pa = re.compile(r'\((.+?)\,(.+?)\)')
                r = pa.findall(sentence)
                out += check_strcpy(r[0][0], r[0][1], global_vars, local_vars)
    out += "\n"
    return out


def check_strcpy(p1 , p2, global_vars, local_vars):
    out = ""
    len1 = -1
    len2 = -1
    str_pattern = re.compile("\"(.*?)\"")
    r = str_pattern.findall(p2)
    if len(r) != 0:
        len2 = len(r[0]) + 1
        print(len2)

    result1 = local_vars.search_array_var(p1)
    if result1 != None:
        if result1.count == -1:
            out += "未初始化的变量"+p1 + "\n"
            print("未初始化的变量"+p1)
        else:
            len1 = result1.count
    else:
        result1 = global_vars.search_array_var(p1)
        if result1 != None:
            if result1.count == -1:
                out += "未初始化的变量"+p1 + "\n"
                print("未初始化的变量"+p1)
            else:
                len1 = result.count

    result = local_vars.search_array_var(p2)
    if result != None:
        if result.count == -1:
            out += "未初始化的变量"+p2 + "\n"
            print("未初始化的变量"+p2)
        else:
            len2 = result.count
    else:
        result = global_vars.search_array_var(p2)
        if result != None:
            if result.count == -1:
                out += "未初始化的变量"+p2 + "\n"
                print("未初始化的变量"+p2)
            else:
                len2 = result.count
    
    if len1 == -1 or len2 == -1:
        out += "错误的参数！\n"
        print("错误的参数！")
        return out

    if len1 < len2:
        if result1.is_on_heap:
            out += "堆溢出！\n"
            print("堆溢出！")
        else:
            out += "栈溢出！\n"
            print("栈溢出！")
        return out
    
    return out
        

    



def scan_global_var(code):
    code = re.sub(c_pattern, "", code)
    global_vars = scan_local_var(code)
    return global_vars


def scan_local_var(function):
    local_vars = var_list()
    types = ["char", "unsigned char", "short", "unsigned short", "int", "unsigned int", "unsigned", "long", "unsigned long", "long long", "unsigned long long"]
    for var_type in types:
        declarations = []
        filter_expression="(?<=[\;\{])[ \t\n]*("+var_type+"[ \t\*]+([\w_]+)(\[[\w_]*\])?[ \t]*(\=[^\;\,]*)?[ \t]*(\,[ \t\*]*([\w_]+)(\[[\w_]*\])?[ \t]*(\=[^\;\,]*)?[ \t]*)*\;)"
        var_pattern = re.compile(filter_expression, re.S)
        all_decl = var_pattern.findall(function)
        for decl in all_decl:
            declarations.append(decl[0])
            print(decl[0])

        #过滤出栈上面的数组，并获取其分配空间以及赋值情况
        arrays_filter1=var_type+"[ \t]+([\w_]+)(\[([\w\_]+)?\])(?:[ \t]*\={1}[ \t\{]*\"{1}([\w]+)\"{1})?"
        arrays_filter2="\,[ \t]*([\w_]+)(\[([\w\_]+)?\])(?:[ \t]*\={1}[ \t]*\"{1}([\w]+)\"{1})?"
        para_pattern1 = re.compile(arrays_filter1)#得到数组声明的第一个
        para_pattern2 = re.compile(arrays_filter2)#得到数组声明第二个及之后

        for declaration in declarations:
            parameters1 = para_pattern1.findall(declaration)
            parameters2 = para_pattern2.findall(declaration)

            for para in parameters1:
                tmp_var = var_pointer(var_type, para[0])
                tmp_var.addr = 1
                if (para[2] == "" and para[3] == ""):
                    print("syntax error! 错误的数组声明语法。")
                if para[2] != "":
                    tmp_var.set_len(int(para[2]))
                if para[3] != "":
                    if para[2] == "":
                        tmp_var.set_len(len(para[3])+1)
                    tmp_var.set_value(list(para[3]))
                local_vars.append_array_var(tmp_var)

            for para in parameters2:
                tmp_var = var_pointer(var_type, para[0])
                tmp_var.addr = 1
                if (para[2] == "" and para[3] == ""):
                    print("syntax error! 错误的数组声明语法。")
                if para[2] != "":
                    tmp_var.set_len(int(para[2]))
                if para[3] != "":
                    if para[2] == "":
                        tmp_var.set_len(len(para[3])+1)
                    tmp_var.set_value(list(para[3]))
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
                local_vars.append_num_var(tmp_var)
            for para in parameters4:
                tmp_var = var_num(var_type, para[0])
                if para[2].strip().isdigit():
                    tmp_var.set_value(int(para[2].strip()))
                local_vars.append_num_var(tmp_var)

	
	    #获取var*型变量
        var_pointer_filter1=var_type+"[ \t]*\*+[ \t]*([\w\_]+)(?=[ \t]*[\,\;\=]{1})"
        var_pointer_filter2="\,[ \t]*[\*]+[ \t]*([\w\_]+)(?=[ \t]*[\,\;]+)"
        para_pattern5 = re.compile(var_pointer_filter1)
        para_pattern6 = re.compile(var_pointer_filter2)
        for declaration in declarations:
            parameters5 = para_pattern5.findall(declaration)
            parameters6 = para_pattern6.findall(declaration)
            for para in parameters5:
                tmp_var = var_pointer(var_type, para)
                tmp_var.is_on_heap = True
                tmp_var.addr = -1
                local_vars.append_array_var(tmp_var)
            for para in parameters6:
                tmp_var = var_pointer(var_type, para)
                tmp_var.is_on_heap = True
                tmp_var.addr = -1
                local_vars.append_array_var(tmp_var)

        malloc_pat = re.compile("([\w_]+)[ \t]*\=[ \t]*(?:\({1}"+var_type+"[ \t]*\*[ \t]*\))?malloc\([ \t]*([\d]+)[ \t]*\*{1}[ \t]*sizeof\("+var_type+"\)\)")
        malloc=malloc_pat.findall(function)
        #print(malloc)
        for pointer in malloc:
            r = local_vars.search_array_var(pointer[0])
            if r != None:
                r.set_len(int(pointer[1]))


    for v in local_vars.array_vars:
        print(v.name)
    return local_vars