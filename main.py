# -*- coding: utf-8 -*-


import sys
import re
import threading
from time import ctime,sleep
from PyQt5 import QtWidgets, QtGui
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from gui.mainwindow import Ui_MainWindow
from gui.viewpic import Ui_ViewPic
from gui.cmpwindow import Ui_cmpwindow
from core.pre_compile import *
from core.compare_str import *
from core.compare_cfg import *
from core.analysis import *
#from core.function import *
#from core.para_types import *

class MyThread(threading.Thread):
    def __init__(self, func, args, name):
        threading.Thread.__init__(self)
        self.name = name
        self.func = func
        self.args = args
        self.result = self.func(*self.args)
 
    def get_result(self):
        try:
            return self.result
        except Exception:
            return None

class cmpwindow(QtWidgets.QWidget, Ui_cmpwindow):
    def __init__(self):
        super(cmpwindow,self).__init__()
        self.setupUi(self)
    
    def set_list(self, list):
        listItem  =  []
        for lst in list:
            listItem.append(QListWidgetItem(lst))
        for i in range(len(listItem)):
            self.listView.insertItem(i+1,listItem[i])

    def set_target(self, file):
        self.lineEdit.setText(file)

    def set_file1(self, file):
        self.label_file1.setText(file)

    def set_file2(self, file):
        self.label_file2.setText(file)

    def set_simi1(self, matcher):
        self.label_simi1.setText(str(matcher))

    def set_simi2(self, matcher):
        self.label_simi2.setText(str(matcher))

    def show_html(self, file):
        with open(file,'r') as f:
            str=f.read()
            self.textEdit.setHtml(str)


class picviewer(QtWidgets.QWidget, Ui_ViewPic):
    def __init__(self, path):
        super(picviewer,self).__init__()
        self.setupUi(self)
        png=QtGui.QPixmap(path)
        self.label.setScaledContents(True)
        self.label.setPixmap(png)


class mywindow(QtWidgets.QMainWindow,Ui_MainWindow):
    sample_dir = ""
    sample_filenames = []
    target_dir = ""
    target_filename = ""
    #functions = func_list()

    def __init__(self):
        super(mywindow,self).__init__()
        self.setupUi(self)
        self.action_open.triggered.connect(self.open_file)
        self.action_close.triggered.connect(self.close_file)
        self.action_save.triggered.connect(self.save_file)
        self.action_save_as.triggered.connect(self.save_file_as)
        self.action_samples.triggered.connect(self.open_dir)
        self.action_cmp_str.triggered.connect(self.cmp_str)
        self.action_cmp_cfg.triggered.connect(self.cmp_cfg)
        self.action_nullp.triggered.connect(self.check_nullp)
        self.action_int_width.triggered.connect(self.check_num)
        self.action_heap.triggered.connect(self.check_heap_overflow)
        self.action_stack.triggered.connect(self.check_stack_overflow)
        self.action_checkall.triggered.connect(self.check_all)
        self.action_checkdir.triggered.connect(self.check_dir)
        self.viewer.clicked.connect(self.show_big_png)


    def open_file(self):
        filename, filetype = QFileDialog.getOpenFileName(self,"选取文件",".","All Files (*);;Text Files (*.txt)")
        if QFile.exists(filename):
            with open(filename,'r') as f:
                str=f.read()
                self.code_edit.setText(str)
            self.functions = get_c_functions(filename)
            get_cfg_graph(self.functions)
            self.target_filename = get_filename2(filename)
            self.target_dir = get_file_dir(filename)
        if QFile.exists("./tmp/cfg.png"):
            png=QtGui.QPixmap('./tmp/cfg.png')
            self.viewer.setScaledContents(True)
            self.viewer.setPixmap(png)
            self.w = picviewer('./tmp/cfg.png')
            self.w.show()
            self.tabWidget.setCurrentIndex(2)

    def close_file(self):
        self.viewer.clear()
        self.code_edit.clear()
    
    def save_file(self):
        pass


    def save_file_as(self):
        pass

    
    def show_big_png(self):   
        self.w = picviewer('./tmp/cfg.png')
        self.w.show()


    def cmp_str(self):
        match_dict = {}
        threads = []
        self.w = cmpwindow()
        self.w.show()
        self.w.set_list(self.sample_filenames)
        self.w.set_target(self.target_filename)
        path1 = self.target_dir + '/' + self.target_filename #目标文件
        for file in self.sample_filenames:
            path2 = self.sample_dir + '/' + file #比较的目标文件
            t = MyThread(file_cmp,(path1,path2,), file)
            threads.append(t)
        for i in range(len(self.sample_filenames)):
            threads[i].start()     
        for i in range(len(self.sample_filenames)):
            threads[i].join()
            match_dict[threads[i].name] = threads[i].get_result()
            print("比较完成"+threads[i].name+"!")
        sorted_dict = sorted(match_dict.items(), key = lambda k: k[1])
        for d in sorted_dict:
            print(d[0] + ":  " + str(d[1]*100) + "%")
        self.w.set_file1(sorted_dict[-1][0])
        self.w.set_simi1(str(sorted_dict[-1][1]*100) + "%")
        html = "tmp/"+self.target_filename.split('.')[0] + "_"+sorted_dict[-1][0].split('.')[0]+"_cmp.html"
        self.w.show_html(html)


    def cmp_cfg(self):
        threads = []
        match_dict = {}
        self.w = cmpwindow()
        self.w.show()
        self.w.set_list(self.sample_filenames)
        self.w.set_target(self.target_filename)
        path1 = self.target_dir + '/' + self.target_filename #目标文件
        functions1 = get_c_functions(path1)
        for file in self.sample_filenames:
            path2 = self.sample_dir + '/' + file #比较的目标文件
            functions2 = get_c_functions(path2)
            match_dict[file] = cfg_cmp(functions1,functions2)
        sorted_dict = sorted(match_dict.items(), key = lambda k: k[1])
        for d in sorted_dict:
            print(d[0] + ":  " + str(d[1]*100) + "%")
        self.w.set_file1(sorted_dict[-1][0])
        self.w.set_simi1(str(sorted_dict[-1][1]*100) + "%")
        self.w.set_file2(sorted_dict[0][0])
        self.w.set_simi2(str(sorted_dict[0][1]*100) + "%")



    def open_dir(self):
        dir_path = QFileDialog.getExistingDirectory(self,"choose directory",".")
        self.treeView.setPath(dir_path)
        self.sample_dir = dir_path
        c_file_types = ["c","C", "cpp","CPP"]
        self.sample_filenames = get_filenames(dir_path, c_file_types)


    def check_heap_overflow(self):
        self.tabWidget.setCurrentIndex(0)
        self.output.document().setMaximumBlockCount(100)
        self.output.append("正在进行堆栈溢出检测...")
        with open(self.target_dir + '/' + self.target_filename,'r') as f:
            str=f.read()
            global_vars = scan_global_var(str)
            print(global_vars)
        for f in self.functions.flist:
            local_vars = scan_local_var(f.func)
            out = scan_suspicious(f, global_vars, local_vars)
            self.output.document().setMaximumBlockCount(100)
            self.output.append(out)


    def check_stack_overflow(self):
        self.tabWidget.setCurrentIndex(0)
        self.output.document().setMaximumBlockCount(100)
        self.output.append("正在进行堆栈溢出检测...")
        with open(self.target_dir + '/' + self.target_filename,'r') as f:
            str=f.read()
            global_vars = scan_global_var(str)
            print(global_vars)
        for f in self.functions.flist:
            local_vars = scan_local_var(f.func)
            out = scan_suspicious(f, global_vars, local_vars)
            self.output.document().setMaximumBlockCount(100)
            self.output.append(out)
            

    def check_nullp(self):
        for f in self.functions.flist:
            local_vars = scan_local_var(f.func)
            out = scan_nullp(f, local_vars)
            self.output.document().setMaximumBlockCount(100)
            self.output.append(out)

    def check_num(self):
        for f in self.functions.flist:
            local_vars = scan_local_var(f.func)
            out = scan_num_overflow(f, local_vars)
            self.output.document().setMaximumBlockCount(100)
            self.output.append(out)
    
    def check_all(self):
        self.tabWidget.setCurrentIndex(0)
        self.check_stack_overflow()
        self.check_num()
        self.check_nullp()

    def check_dir(self):
        dir_path = QFileDialog.getExistingDirectory(self,"choose directory",".")
        c_file_types = ["c","C", "cpp","CPP", "h", "hpp"]
        filenames = get_filenames(dir_path, c_file_types)
        for file in filenames:
            path = dir_path + '/' + file #比较的目标文件
            self.check_file(path)

    def check_file(self,path):
        out = "正在扫描文件："+path
        functions = get_c_functions(path)
        self.tabWidget.setCurrentIndex(0)
        self.output.document().setMaximumBlockCount(100)
        with open(path,'r') as fp:
            str=fp.read()
            global_vars = scan_global_var(str)
        self.output.append("正在进行堆栈溢出检测...")
        out += "正在进行堆栈溢出检测..."
        for f in functions.flist:
            local_vars = scan_local_var(f.func)
            out += scan_suspicious(f, global_vars, local_vars)
            self.output.document().setMaximumBlockCount(100)
            self.output.append(out)
        self.output.document().setMaximumBlockCount(100)
        self.output.append("正在进行空指针检测...")
        out += "正在进行空指针检测..."
        for f in functions.flist:
            local_vars = scan_local_var(f.func)
            out += scan_nullp(f, local_vars)
            self.output.document().setMaximumBlockCount(100)
            self.output.append(out)
        self.output.document().setMaximumBlockCount(100)
        self.output.append("正在进行整数宽度溢出检测...")
        out += "正在进行整数宽度溢出检测..."
        for f in functions.flist:
            local_vars = scan_local_var(f.func)
            out += scan_num_overflow(f, local_vars)
            self.output.document().setMaximumBlockCount(100)
            self.output.append(out)
        
        with open(path+".log", "w") as fp:
            fp.write(out)


app = QtWidgets.QApplication(sys.argv)
window = mywindow()
window.show()
sys.exit(app.exec_())
