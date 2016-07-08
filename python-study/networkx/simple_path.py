#!/usr/bin/env python
#coding:utf-8
"""
Draw a graph with matplotlib.
You must have matplotlib for this to work.
"""
try:
    import matplotlib.pyplot as plt
except:
    raise

import networkx as nx

G=nx.path_graph(8)
G.add_edge('abcd', 'cxxu')
nx.draw(G)
plt.savefig("simple_path.png") # save as png
plt.show() # display
