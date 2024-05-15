import numpy as np
import faiss
d = 64                                           # 向量维度
nb = 100000                                      # index向量库的数据量
nq = 10000                                       # 待检索query的数目
np.random.seed(1234)
xb = np.random.random((nb, d)).astype('float32')
xb[:, 0] += np.arange(nb) / 1000.                # index向量库的向量
                                                 #[:,0]是数组的第一维的第0个数据
                                                 #xb[:, m:n]输出结果是：取所有数据的第m到n-1列数据，含左不含右（是列，不是行）
                                                 #xb[::-1]输出结果是：数组xb的逆数组（从后往前重新组合）
xq = np.random.random((nq, d)).astype('float32')
xq[:, 0] += np.arange(nq) / 1000.                # 待检索的query向量

# index = faiss.IndexFlatL2(d)
# param = "Flat"
param = 'HNSW64'
index = faiss.index_factory(d, param, faiss.METRIC_L2)
                                                #METRIC_INNER_PRODUCT（内积）
                                                #METRIC_L1（曼哈顿距离）
                                                #METRIC_L2（欧氏距离）
                                                #METRIC_Linf（无穷范数）
                                                #METRIC_Lp（p范数）
                                                #METRIC_BrayCurtis（BC相异度）
                                                #METRIC_Canberra（兰氏距离/堪培拉距离）
                                                #METRIC_JensenShannon（JS散度）
'''
第二个参数的选择
param = 'Flat'
优点：该方法是Faiss所有index中最准确的，召回率最高的方法，没有之一；
缺点：速度慢，占内存大。
使用：向量候选集很少，在50万以内，并且内存不紧张。

param = 'IVF100,Flat'
优点：IVF主要利用倒排的思想，在文档检索场景下的倒排技术是指，一个kw后面挂上很多个包含该词的doc，由于kw数量远远小于doc，因此会大大减少了检索的时间。在向量中如何使用倒排呢？可以拿出每个聚类中心下的向量ID，每个中心ID后面挂上一堆非中心向量，每次查询向量的时候找到最近的几个中心ID，分别搜索这几个中心下的非中心向量。通过减小搜索范围，提升搜索效率。
缺点：速度也还不是很快。
使用情况：相比Flat会大大增加检索的速度，建议百万级别向量可以使用。

param =  'PQ16' 
优点：利用乘积量化的方法，改进了普通检索，将一个向量的维度切成x段，每段分别进行检索，每段向量的检索结果取交集后得出最后的TopK。因此速度很快，而且占用内存较小，召回率也相对较高。
缺点：召回率相较于暴力检索，下降较多。
使用情况：内存及其稀缺，并且需要较快的检索速度，不那么在意召回率

param =  'IVF100,PQ16'
优点：工业界大量使用此方法，各项指标都均可以接受，利用乘积量化的方法，改进了IVF的k-means，将一个向量的维度切成x段，每段分别进行k-means再检索。
缺点：集百家之长，自然也集百家之短
使用情况：一般来说，各方面没啥特殊的极端要求的话，最推荐使用该方法！

param = 'HNSW64'
HNSW，即Hierarchical Navigable Small World graphs（分层-可导航-小世界-图）的缩写，可以说是在工业界影响力最大的基于图的近似最近邻搜索算法（Approximate Nearest Neighbor，ANN），没有之一。HNSW 是一种非常流行和强大的算法，具有超快的搜索速度和出色的召回率。

优点：该方法为基于图检索的改进方法，检索速度极快，10亿级别秒出检索结果，而且召回率几乎可以媲美Flat，最高能达到惊人的97%。检索的时间复杂度为loglogn，几乎可以无视候选向量的量级了。并且支持分批导入，极其适合线上任务，毫秒级别体验。
缺点：构建索引极慢，占用内存极大（是Faiss中最大的，大于原向量占用的内存大小）
参数：HNSWx中的x为构建图时每个点最多连接多少个节点，x越大，构图越复杂，查询越精确，当然构建index时间也就越慢，x取4~64中的任何一个整数。
使用情况：不在乎内存，并且有充裕的时间来构建index
'''

print(index.is_trained)                          # 输出为True，代表该类index不需要训练，只需要add向量进去即可
index.add(xb)                                    # 将向量库中的向量加入到index中
print("vector sum: ", index.ntotal)                              # 输出index中包含的向量总数，为100000

k = 4                                            # topK的K值
D, I = index.search(xq, k)                       # xq为待检索向量，返回的I为每个待检索query最相似TopK的索引list，D为其对应的距离

print(I[:5])
                                                 #[:5]是数据的前5行，[-5:]是数据的倒数5行
print(D[-5:])
