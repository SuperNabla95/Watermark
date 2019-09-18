# libraries
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
import pandas as pd

def read_data(path) :
    data = {}
    last_name = None
    fin = open(path,'r')
    lines = fin.readlines()
    for line in lines :
        line=line.split('\n')[0]
        if line.startswith('@') :
            name=line.split('@')[1]
            data[name] = {'start':0,'color':'#00000000','list':[]}
            last_name = name
        else :
            tokens=line.split('@')
            start = float(tokens[0])
            color = tokens[1]
            old_start = data[last_name]['start']
            old_color = data[last_name]['color']
            data[last_name]['start'] = start
            data[last_name]['color'] = color
            data[last_name]['list'].append([old_color,start-old_start])
    fin.close()
    return data

def compute_bars(data) :
    n = len(data)
    bars = []
    colors = []
    names = {}
    count = 0
    for name in data :
        names[name] = count
        count+=1
    color = None
    todo = True
    while todo :
        bar = [0] * n
        todo = False
        for name in data :
            l = data[name]['list']
            if len(l) > 0 :
                todo = True
                if color == None :
                    color = l[0][0]
                    colors.append(color)
                    width = l[0][1]
                    l.remove(l[0])
                    index = names[name]
                    bar[index] = width
                elif color == l[0][0] :
                    width = l[0][1]
                    l.remove(l[0])
                    index = names[name]
                    bar[index] = width
        if todo :
            bars.append(bar)
            color = None
    min0 = min(bars[0])
    bars[0] = [x-min0 for x in bars[0]]
    return bars,colors
                   
def left_values(bars) :
    nt = len(bars[0])
    lefts = []
    prev = [0] * nt
    for bar in bars :
        lefts.append(prev)
        prev = np.add(prev, bar).tolist()
    return lefts,nt


data = read_data('farm-map_2_2')
bars,colors = compute_bars(data)
print(bars,colors)
lefts,nt = left_values(bars)
print(lefts)

# y-axis in bold
rc('font', weight='bold')
 
# The position of the bars on the x-axis
r = list(reversed(range(nt)))
 
# Names of group and bar width
names = [name for name in data]

num_bars = len(bars)
for i in range(num_bars) :
    print(i,bars[i])
    plt.barh(r,bars[i],color=colors[i],left=lefts[i],linewidth=0)
 
# Custom X axis
plt.yticks(r, names, fontweight='bold')
plt.ylabel("group")
 
# Show graphic
plt.show()
