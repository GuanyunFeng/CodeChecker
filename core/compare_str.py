#!/home/ypang/Softwares/anaconda3/bin/python
# -*- coding: utf-8 -*-
import sys
import os
import difflib
import Levenshtein
import numpy as np
#from pre_compile import *


def jaccard(p,q):
    c = [a for i in p if v in b]
    return float(len(c))/(len(a)+len(b)-len(b))


#计算海明距离
def hamming_distance(s1, s2):
    assert len(s1) == len(s2)
    return sum(ch1 != ch2 for ch1, ch2 in zip(s1, s2))


#使用numpy计算余弦相似度
def cos_sim(s1, s2):
    s1 = np.mat(s1) 
    s2 = np.mat(s2)
    num = float(s1 * s2.T)
    denom = np.linalg.norm(s1) * np.linalg.norm(s2)
    cos = num / denom
    sim = 0.5 + 0.5 * cos
    return sim



#计算字符串编辑距离
def levenshtein_edit(source, target):
    if len(s1) < len(s2):
        return levenshtein2(s2, s1)
    if len(s2) == 0:
        return len(s1)
        
    previous_row = range(len(s2) + 1)
    for i, c1 in enumerate(s1):
        current_row = [i + 1]
        for j, c2 in enumerate(s2):
            insertions = previous_row[j + 1] + 1
            deletions = current_row[j] + 1
            substitutions = previous_row[j] + (c1 != c2)
            current_row.append(min(insertions, deletions, substitutions))
        previous_row = current_row
    
    return previous_row[-1]


#计算莱文斯坦比
def levenshtein_ratio(source, target):
    sum = len(source) + len(target)
    ld = levenshtein_edit(source, target)
    return (sum - ld)/sum

def file_cmp(path1, path2):
    filename1 = get_filename(path1)
    filename2 = get_filename(path2)
    
    file1_line = open(path1).readlines()
    file2_line = open(path2).readlines()
    str1 = open(path1).read()
    str2 = open(path2).read()
    f = open("tmp/"+filename1 + "_" + filename2 + "_cmp.html", 'w')
    d = difflib.HtmlDiff()
    f.write(d.make_file(file1_line,file2_line))
    f.close()
    matcher = difflib.SequenceMatcher(None, file1_line, file2_line).ratio()
    # 计算莱文斯坦比
    sim2 = Levenshtein.ratio(str1, str2)
    # 计算jaro距离 
    sim3 = Levenshtein.jaro(str1, str2 )
     # Jaro–Winkler距离 
    sim4 = Levenshtein.jaro_winkler(str1 , str2)
    matcher = matcher*0.25 + sim2*0.25 + sim3*0.25 + sim4*0.25
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