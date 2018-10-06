#!/home/ypang/Softwares/anaconda3/bin/python
# -*- coding: utf-8 -*-
import re
import os
import sys
import pygraphviz as gv
import difflib
from .function import *

c_pattern = re.compile(r'((?:const[ \t\*]+)?(void|int|char|long|double|float|unsigned|unsigned int|unsigned long|long long)[ \t\*]+(?:const[ \t*]+)?)(\w+)(\([^\;]*\))[^\;]*(\{([^{}]*(\{([^{}]*(\{([^{}]*(\{[^{}]*\})*[^\{\}]*?)*\})*[^\{\}]*?)*\})*[^\{\}]*?)*\})', re.S)

'''
    input:
        filepath:要分析的c代码路径（包括文件名），字符串
    output:
        group:获得的函数列表
'''
def get_c_functions(filepath):
    f = open(filepath, "r")
    str = f.read()
    f.close()
    c_group = c_pattern.findall(str)
    functions = func_list(c_group)
    return functions


'''
    input:
        names: 函数名称，列表
        functions：函数名称和对应的函数体，字典
    output:
        在当前文件夹下生成“cfg.png”,并且在命令行中打印函数调用
'''
def get_cfg_graph(functions):
    graph = gv.AGraph(directed=True)
    for name in functions.names():
        graph.add_node(name)
    for f in functions:
        flag = False
        for name in functions.names():
            result = f.func.find(name + "(")
            if result !=-1:
                flag = True
                graph.add_edge(f.name, name)
    graph.layout(prog='dot')    
    graph.draw("tmp/cfg.png")


def get_sub_tree(func_name, names, functions):
    tree = []
    #tree.append(func_name)
    if func_name not in names:
        print("main function not found")
        exit(0)
    for name in names:
        result = functions[func_name].find(name + "(")
        if result != -1:
            tree.append(get_sub_tree(name))
    return tree


def get_cfg_tree(group):
    names = []
    functions = {}
    for g in c_group:
        names.append(g[1])
        functions[g[1]] = g[3]
    tree = get_sub_tree("main", names, functions)
    return tree


def tree_to_list(tree, list):
    size = len(tree)
    list.append(size)
    for i in range(size):
        tree_to_list(tree[i], list)


def cmp_cfg_tree(tree1, tree2):
    list1 = []
    list2 = []
    tree_to_list(tree1, list1)
    tree_to_list(tree2, list2)
    print(list1)
    print(list2)
    matcher = difflib.SequenceMatcher(None, list1, list2).ratio()
    print(matcher)
    return matcher    


