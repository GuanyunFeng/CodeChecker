# 如何绘制出简易的CFG图
## 1.提取c语言函数体，以及对应的函数名。
使用正则表达式：
```Python
pattern = re.compile(r'((?:const[ \t\*]+)?(?:void|int|char|long|double|float|unsigned|unsigned int|unsigned long|long long)[ \t\*]+(?:const[ \t*]+)?)(\w+)(\([^\;]*\))[^\;]*(\{([^{}]*(\{([^{}]*(\{([^{}]*(\{[^{}]*\})*[^\{\}]*?)*\})*[^\{\}]*?)*\})*[^\{\}]*?)*\})', re.S)
```
对读取的c语言代码进行匹配，代码如下：
```Python
def get_c_functions(filepath):
    f = open(filepath, "r")
    str = f.read()
    f.close()
    c_group = c_pattern.findall(str)
    return c_group
```

## 2.在函数体中查找已知的函数名,并使用pygraphviz绘制出cfg图。
```Python
def get_cfg_graph(c_group):
    names = []
    functions = {}
    for g in c_group:
        names.append(g[1])
        functions[g[1]] = g[3]

    graph = gv.AGraph(directed=True)

    for name in names:
        graph.add_node(name)

    for (key, value) in functions.items():
        #print("function #" + key + "# has call function:")
        flag = False
        for name in names:
            result = value.find(name + "(")
            if result !=-1:
                flag = True
                graph.add_edge(key, name)
                #print("    " + name)
        #if flag == False:
            #print("    none")
        #print(" ")
    graph.layout(prog='dot')    
    graph.draw("tmp/cfg.png")
```
