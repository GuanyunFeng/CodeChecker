# CodeChecker
## 环境依赖及安装方法
Ubuntu18.04+Python3.6.5+pyqt5+pygraphviz+ply
仅在python3环境下进行测试，python2不一定能够正常运行。
### PyQt5安装
直接使用pip安装
```
python3：pip install pyqt5
python2: pip install python-qt
```
### pygraghviz安装
直接使用pip安装
```
pip install pydot
pip install graphviz
pip install pygraphviz
```
在windows下安装graphviz后需要添加环境变量，否则安装pygraphviz会出现报错。

### ply安装
```
pip install ply
```


## 使用方法
无需安装，下载后解压即可使用。进入下载文件夹后，使用以下命令：
```
tar -zxvf CodeChecker.tar.gz
cd CodeChecker
python main.py
```
