# -*- coding: UTF-8 -*-
import re
import os
import sys
#import ply


file_type_list = ["h", "c", "cc", "cpp", "H", "C", "cc", "CPP"]
define_rules = re.compile(r'#define[ \t]+(([^ \t])+)[ \t]+([^\n]+)\n')
typedef_rules = re.compile(r'typedef[ \t]+([\w_*]+)[ \t]+([\w_]+)\;')
commet_rules = re.compile(r'((?<=\n)|^)[ \t]*\/\*.*?\*\/\n?|\/\*.*?\*\/|((?<=\n)|^)[ \t]*\/\/[^\n]*\n|\/\/[^\n]*', re.S)


def pre_complie(path_in, path_out):
	#打开文件
	f = open(path_in, "r")
	str = f.read()
	f.close()
	#去注释
	str = re.sub(commet_rules, "", str)
	#define宏定义替换，并去除宏定义语句
	group1 = define_rules.findall(str)
	str = re.sub(define_rules, "", str)
	for g in group1:
		str = str.replace(g[0], g[2])
		'''
	#typedef替换，并去除typedef语句
	group2 = typedef_rules.findall(str)
	str = re.sub(typedef_rules, "", str)
	for g in group2:
		str = str.replace(g[0], g[1])
		'''
	#写入文件
	f = open(path_out, "w")
	f.write(str)


def pre_compile_dir(path, file_types):
    for file in os.listdir(path):
        listpath = path + "//"+file
        if os.path.isdir(listpath):
            listfiles(listpath, file_types)
        elif os.path.isfile(listpath):
            splitlist = listpath.split('.')
            m = len(splitlist)
            prefx = splitlist[m-1]
            #print prefx
            if prefx in file_types:
                pre_complie(listpath, listpath + ".is")