#!/home/ypang/Softwares/anaconda3/bin/python
# -*- coding: utf-8 -*-
import sys
import os
import difflib
#from pre_compile import *

def file_cmp(path1, path2):
    filename1 = get_filename(path1)
    filename2 = get_filename(path2)
    
    file1_line = open(path1).readlines()
    file2_line = open(path2).readlines()
    f = open(filename1 + "_" + filename2 + "_cmp.html", 'w')
    d = difflib.HtmlDiff()
    f.write(d.make_file(file1_line,file2_line))
    f.close()
    matcher = difflib.SequenceMatcher(None, file1_line, file2_line).ratio()
    return matcher

'''
    input:path为完整路径含文件名
    output:filename文件名不含后缀
'''
def get_filename(path):
    filepath,tempfilename = os.path.split(path)
    filename,extension = os.path.splitext(tempfilename)
    return filename

'''
    input:path为完整路径含文件名
    output:filename文件名含后缀
'''
def get_filename2(path):
    filepath,tempfilename = os.path.split(path)
    return tempfilename

'''
    input:path为完整路径含文件名
    output:filepath为文件路径
'''
def get_file_dir(path):
    filepath,tempfilename = os.path.split(path)
    return filepath


def get_filenames(dir, file_types):
    names = []
    trav_dir(dir, names, file_types)
    return names


def trav_dir(dir, filelist, file_types):
    for file in os.listdir(dir):
        listpath = dir + "//"+file
        if os.path.isdir(listpath):
            trav_dir(listpath, filelist, file_types)
        elif os.path.isfile(listpath):
            splitlist = listpath.split('.')
            m = len(splitlist)
            prefx = splitlist[m-1]
            if prefx in file_types:
                filelist.append(file)