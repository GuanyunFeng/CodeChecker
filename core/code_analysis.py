#!/home/ypang/Softwares/anaconda3/bin/python
# -*- coding: utf-8 -*-
import re
import os
import sys
from graphviz import Digraph
import pygraphviz as gv


names = []
group = []
functions = {}

pattern = re.compile(r'((?:const[ \t\*]+)?(?:void|int|char|long|double|float|unsigned|unsigned int|unsigned long|long long)[ \t\*]+(?:const[ \t*]+)?)(\w+)(\([^\;]*\))[^\;]*(\{([^{}]*(\{([^{}]*(\{([^{}]*(\{[^{}]*\})*[^\{\}]*?)*\})*[^\{\}]*?)*\})*[^\{\}]*?)*\})', re.S)


def get_functions(filepath):
    f = open(filepath, "r")
    str = f.read()
    f.close()
    group = pattern.findall(str)
    #print(group)
    for g in group:
        names.append(g[1])
        functions[g[1]] = g[3]
        #print(g[3]) #函数名
        #print(g[5]) #函数体


def check_num_overflow(function):
    declarations = []
    int_parameters = []
    #获取int相关变量
    int_pattern = re.compile(r'(int[ \t*]+([\w_]+)(\[[\w_]*\])*[ \t]*(\={1}[^\;\,]*)*[ \t]*(\,[ \t*]*([\w_]+)(\[[\w_]*\])*[ \t]*(\={1}[^\;\,]*)*[ \t]*)*\;)', re.S)
    all_decl = int_pattern.findall(function)
    for decl in all_decl:
        declarations.append(decl)
    para_pattern = re.compile(r'j')


def check_stack_overflow(function):
    declarations = []
    char_arrys = []
    #获取char[]相关变量
    decl_pattern = re.compile(r'(char[ \t*]+([\w_]+)(\[[\w_]*\])*[ \t]*(\={1}[^\;\,]*)*[ \t]*(\,[ \t*]*([\w_]+)(\[[\w_]*\])*[ \t]*(\={1}[^\;\,]*)*[ \t]*)*\;)', re.S)
    all_decl = decl_pattern.findall(function)
    print(all_decl)
    for decl in all_decl:
        declarations.append(decl[0])
    #获取
    para_pattern1 = re.compile(r'char[ \t]+([\w_]+)(\[[\w_]*\])')
    para_pattern2 = re.compile(r'\,[ \t*]*([\w_]+)(\[[\w_]*\])')
    for declaration in declarations:
        parameters1 = para_pattern1.findall(declaration)
        parameters2 = para_pattern2.findall(declaration)
        for para in parameters1:
            char_arrys.append(para[0]+para[1])
        for para in parameters2:
            char_arrys.append(para[0]+para[1])
    
    print(char_arrys)


def check_heap_overflow(function):
    pass



#get_functions(filename)
#get_call_relationship(names, functions)

for (key, value) in functions.items():
    print(value)
    check_stack_overflow(value)
    print("")
