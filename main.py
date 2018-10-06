# -*- coding: utf-8 -*-


import sys
from PyQt5 import QtWidgets, QtGui
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from mainwindow import Ui_MainWindow
from viewpic import Ui_ViewPic
from cmpwindow import Ui_cmpwindow
from core.pre_compile import *
from core.compare_str import *
from core.compare_cfg import *
#from core.analysis import *
#from core.function import *
#from core.para_types import *

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
        pass

    def set_file2(self, file):
        pass

    def show_html(self, file):
        pass


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

    def __init__(self):
        super(mywindow,self).__init__()
        self.setupUi(self)
        self.action_open.triggered.connect(self.open_file)
        self.action_close.triggered.connect(self.close_file)
        self.action_save.triggered.connect(self.save_file)
        self.action_save_as.triggered.connect(self.save_file_as)
        self.action_samples.triggered.connect(self.open_dir)
        self.action_cmp_str.triggered.connect(self.cmp_str)
        self.viewer.clicked.connect(self.show_big_png)


    def open_file(self):
        filename, filetype = QFileDialog.getOpenFileName(self,"选取文件",".","All Files (*);;Text Files (*.txt)")
        if QFile.exists(filename):
            with open(filename,'r') as f:
                str=f.read()
                self.code_edit.setText(str)
            c_group = get_c_functions(filename)
            get_cfg_graph(c_group)
            self.target_filename = get_filename2(filename)
            self.target_dir = get_file_dir(filename)
            print(self.target_filename)
            print(self.target_dir)
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
        match_list = []
        self.w = cmpwindow()
        self.w.show()
        self.w.set_list(self.sample_filenames)
        self.w.set_target(self.target_filename)
        path1 = self.target_dir + '\\' + self.target_filename #目标文件
        for file in self.sample_filenames:
            path2 = self.sample_dir + '\\' + file #比较的目标文件
        matcher = file_cmp(path1, path2)
        
    

    def open_dir(self):
        dir_path = QFileDialog.getExistingDirectory(self,"choose directory",".")
        self.treeView.setPath(dir_path)
        self.sample_dir = dir_path
        c_file_types = ["c","C", "cpp","CPP"]
        self.sample_filenames = get_filenames(dir_path, c_file_types)
        print(self.sample_dir)
        print(self.sample_filenames)


app = QtWidgets.QApplication(sys.argv)
window = mywindow()
window.show()
sys.exit(app.exec_())
