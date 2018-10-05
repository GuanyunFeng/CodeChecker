# 如何绘制出简易的CFG图
## 1.提取c语言函数体，以及对应的函数名。
使用正则表达式：
```
pattern = re.compile(r'((?:const[ \t\*]+)?(?:void|int|char|long|double|float|unsigned|unsigned int|unsigned long|long long)[ \t\*]+(?:const[ \t*]+)?)(\w+)(\([^\;]*\))[^\;]*(\{([^{}]*(\{([^{}]*(\{([^{}]*(\{[^{}]*\})*[^\{\}]*?)*\})*[^\{\}]*?)*\})*[^\{\}]*?)*\})', re.S)
``
对读取的c语言代码进行匹配，代码如下：
``` c
def get_c_functions(filepath):
    f = open(filepath, "r")
    str = f.read()
    f.close()
    c_group = c_pattern.findall(str)
    return c_group
```

## 2.在函数体中查找已知的函数名。
