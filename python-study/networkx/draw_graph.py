#coding:utf-8
import networkx as nx
import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif']=['SimHei'] #用来正常显示中文标签
plt.rcParams['axes.unicode_minus']=False #用来正常显示负号

#有中文出现的情况，需要u'内容'
def draw_graph(graph):
    # extract nodes from graph
    nodes = set([n1 for n1, n2 in graph] + [n2 for n1, n2 in graph])
    # create networkx graph
    G=nx.Graph()
    # add nodes
    for node in nodes:
        G.add_node(node)
    # add edges
    for edge in graph:
        G.add_edge(edge[0], edge[1])
    # draw graph
    pos = nx.shell_layout(G)
    #nx.draw(G, pos)
    nx.draw_networkx(G, pos, font_family='sans-serif')
    # show graph
    plt.show()

# draw example
#graph = [(25, 20), ("aaa", "bbb"), ('大小'.decode('utf8'), '多少'.decode('utf8'))]
graph = [(25, 20), ("aaa", "bbb"), (u'大小', u'多少')]
draw_graph(graph)
