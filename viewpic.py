# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'viewpic.ui'
#
# Created by: PyQt5 UI code generator 5.11.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_ViewPic(object):
    def setupUi(self, ViewPic):
        ViewPic.setObjectName("ViewPic")
        ViewPic.resize(800, 500)
        ViewPic.setFixedSize(800, 500)
        self.label = QtWidgets.QLabel(ViewPic)
        self.label.setGeometry(QtCore.QRect(0, 0, 800, 500))
        self.label.setText("")
        self.label.setObjectName("label")

        self.retranslateUi(ViewPic)
        QtCore.QMetaObject.connectSlotsByName(ViewPic)

    def retranslateUi(self, ViewPic):
        _translate = QtCore.QCoreApplication.translate
        ViewPic.setWindowTitle(_translate("ViewPic", "Form"))

