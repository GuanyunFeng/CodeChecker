# 关于代码同源性检测
使用两种方法，一种是基于字符串比较的同源性检测，另一种是基于程序cfg图的同源性检测。
##基于字符串检测
这里使用了ply库来提取c语言代码的token，然后使用difflib库对提取文件进行比较。
##基于cfg图检测
pass