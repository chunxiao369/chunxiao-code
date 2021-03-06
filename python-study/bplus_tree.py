#!/usr/bin/env python
#coding=utf-8
from random import randint, choice
from bisect import bisect_right, bisect_left
from collections import deque

class InitError(Exception):
    pass
class ParaError(Exception):
    pass

#生成键值对
class KeyValue(object):
    __slots__=('key', 'value')
    def __init__(self, key, value):
        self.key=int(key) #一定要保证键值是整型
        self.value=value

    def __str__(self):
        return str((self.key, self.value))

    def __cmp__(self, key):
        if self.key>key:
            return 1
        elif self.key < key:
            return -1
        else:
            return 0

    def __lt__(self, other):
        if (type(self) == type(other)):
            return self.key < other.key;
        else:
            return int(self.key) < int(other);

    def __eq__(self, other):
        if (type(self) == type(other)):
            return self.key == other.key;
        else:
            return int(self.key) == int(other);

    def __gt__(self, other):
        return not self < other;


#B+树实现
#实现过程和btree很像，不过有几点显著不同。
#1.内节点不存储key-value，只存放key
#
#2.沿着内节点搜索的时候，查到索引相等的数要向树的右边走。所以二分查找要选择
#bisect_right
#
#3.在叶子节点满的时候，并不是先分裂再插入而是先插入再分裂。因为b+tree无法保证
#分裂的两个节点的大小都是相等的。在奇数大小的数据分裂的时候右边的子节点会比左
#边的大。如果先分裂再插入无法保证插入的节点一定会插在数量更少的子节点上，满足
#节点数量平衡的条件。
#
#4.在删除数据的时候，b+tree的左右子节点借数据的方式比btree更加简单有效，只把子
#节点的子树直接剪切过来，再把索引变一下就行了，而且叶子节点的兄弟指针也不用动。
#
class Bptree(object):
    class __InterNode(object):
        def __init__(self, M):
            if not isinstance(M, int):
                raise InitError('M must be int')
            if M<=3:
                raise InitError('M must be greater then 3')
            else:
                self.__M=M
                self.clist=[] #存放区间
                self.ilist=[] #存放索引/序号
                self.par=None
        def isleaf(self):
            return False
        def isfull(self):
            return len(self.ilist)>=self.M-1
        def isempty(self):
            return len(self.ilist)<=(self.M+1)/2-1
        @property
        def M(self):
            return self.__M

    #叶子
    class __Leaf(object):
        def __init__(self,L):
            if not isinstance(L,int):
                raise InitError('L must be int')
            else:
                self.__L=L
                self.vlist=[]
                self.bro=None #兄弟结点
                self.par=None #父结点
        def isleaf(self):
            return True
        def isfull(self):
            return len(self.vlist)>self.L
        def isempty(self):
            return len(self.vlist)<=(self.L+1)/2
        @property
        def L(self):
            return self.__L

    #初始化
    def __init__(self,M,L):
        if L>M:
            raise InitError('L must be less or equal then M')
        else:
            self.__M=M
            self.__L=L
            self.__root=Bptree.__Leaf(L)
            self.__leaf=self.__root
    @property
    def M(self):
        return self.__M
    @property
    def L(self):
        return self.__L

    #插入
    def insert(self, key_value):
        node=self.__root
        def split_node(n1):
            mid=self.M//2 #此处注意，可能出错
            newnode=Bptree.__InterNode(self.M)
            newnode.ilist=n1.ilist[mid:]
            newnode.clist=n1.clist[mid:]
            newnode.par=n1.par
            for c in newnode.clist:
                c.par=newnode
            if n1.par is None:
                newroot=Bptree.__InterNode(self.M)
                newroot.ilist=[n1.ilist[mid-1]]
                newroot.clist=[n1,newnode]
                n1.par=newnode.par=newroot
                self.__root=newroot
            else:
                i=n1.par.clist.index(n1)
                n1.par.ilist.insert(i,n1.ilist[mid-1])
                n1.par.clist.insert(i+1,newnode)
            n1.ilist=n1.ilist[:mid-1]
            n1.clist=n1.clist[:mid]
            return n1.par
        def split_leaf(n2):
            mid=(self.L+1)//2
            newleaf=Bptree.__Leaf(self.L)
            newleaf.vlist=n2.vlist[mid:]
            if n2.par==None:
                newroot=Bptree.__InterNode(self.M)
                newroot.ilist=[n2.vlist[mid].key]
                newroot.clist=[n2,newleaf]
                n2.par=newleaf.par=newroot
                self.__root=newroot
            else:
                i=n2.par.clist.index(n2)
                n2.par.ilist.insert(i,n2.vlist[mid].key)
                n2.par.clist.insert(i+1,newleaf)
                newleaf.par=n2.par
            n2.vlist=n2.vlist[:mid]
            n2.bro=newleaf
        def insert_node(n):
            if not n.isleaf():
                if n.isfull():
                    insert_node(split_node(n))
                else:
                    p=bisect_right(n.ilist,key_value)
                    insert_node(n.clist[p])
            else:
                p=bisect_right(n.vlist,key_value)
                n.vlist.insert(p,key_value)
                if n.isfull():
                    split_leaf(n)
                else:
                    return
        insert_node(node)

    #搜索
    def search(self,mi=None,ma=None):
        result=[]
        node=self.__root
        leaf=self.__leaf
        if mi is None or ma is None:
            raise ParaError('you need to setup searching range')
        elif mi > ma:
            raise ParaError('upper bound must be greater or equal than lower bound')
        def search_key(n,k):
            if n.isleaf():
                p=bisect_left(n.vlist,k)
                return (p,n)
            else:
                p=bisect_right(n.ilist,k)
                return search_key(n.clist[p],k)
        if mi is None:
            while True:
                for kv in leaf.vlist:
                    if kv<=ma:
                        result.append(kv)
                    else:
                        return result
                if leaf.bro==None:
                    return result
                else:
                    leaf=leaf.bro
        elif ma is None:
            index,leaf=search_key(node,mi)
            result.extend(leaf.vlist[index:])
            while True:
                if leaf.bro==None:
                    return result
                else:
                    leaf=leaf.bro
                    result.extend(leaf.vlist)
        else:
            if mi==ma:
                i,l=search_key(node,mi)
                try:
                    if l.vlist[i]==mi:
                        result.append(l.vlist[i])
                        return result
                    else:
                        return result
                except IndexError:
                    return result
            else:
                i1,l1=search_key(node,mi)
                i2,l2=search_key(node,ma)
                if l1 is l2:
                    if i1==i2:
                        return result
                    else:
                        result.extend(l2.vlist[i1:i2])
                        return result
                else:
                    result.extend(l1.vlist[i1:])
                    l=l1
                    while True:
                        if l.bro==l2:
                            result.extend(l2.vlist[:i2])
                            return result
                        elif l.bro != None:
                            result.extend(l.bro.vlist)
                            l=l.bro
                        else:
                            return result;
    def traversal(self):
        result=[]
        l=self.__leaf
        while True:
            result.extend(l.vlist)
            if l.bro==None:
                return result
            else:
                l=l.bro
    def show(self):
        print('this b+tree is:\n')
        q=deque()
        h=0
        q.append([self.__root,h])
        while True:
            try:
                #print ("len q :", len(q))
                w,hei=q.popleft()
            except IndexError:
                return
            else:
                if not w.isleaf():
                    print(w.ilist,'the height is',hei)
                    #print ((i for i in w.clist), h, "len:", len(w.clist))
                    if hei==h:
                        h+=1
                    q.extend([[i,h] for i in w.clist])
                else:
                    print([(v.key,v.value) for v in w.vlist],'the leaf is,',hei)

    #删除
    def delete(self,key_value):
        def merge(n,i):
            if n.clist[i].isleaf():
                n.clist[i].vlist=n.clist[i].vlist+n.clist[i+1].vlist
                n.clist[i].bro=n.clist[i+1].bro
            else:
                n.clist[i].ilist=n.clist[i].ilist+[n.ilist[i]]+n.clist[i+1].ilist
                n.clist[i].clist=n.clist[i].clist+n.clist[i+1].clist
            n.clist.remove(n.clist[i+1])
            n.ilist.remove(n.ilist[i])
            if n.ilist==[]:
                n.clist[0].par=None
                self.__root=n.clist[0]
                del n
                return self.__root
            else:
                return n
        def tran_l2r(n,i):
            if not n.clist[i].isleaf():
                n.clist[i+1].clist.insert(0,n.clist[i].clist[-1])
                n.clist[i].clist[-1].par=n.clist[i+1]
                n.clist[i+1].ilist.insert(0,n.ilist[i])
                n.ilist[i]=n.clist[i].ilist[-1]
                n.clist[i].clist.pop()
                n.clist[i].ilist.pop()
            else:
                n.clist[i+1].vlist.insert(0,n.clist[i].vlist[-1])
                n.clist[i].vlist.pop()
                n.ilist[i]=n.clist[i+1].vlist[0].key
        def tran_r2l(n,i):
            if not n.clist[i].isleaf():
                n.clist[i].clist.append(n.clist[i+1].clist[0])
                n.clist[i+1].clist[0].par=n.clist[i]
                n.clist[i].ilist.append(n.ilist[i])
                n.ilist[i]=n.clist[i+1].ilist[0]
                n.clist[i+1].clist.remove(n.clist[i+1].clist[0])
                n.clist[i+1].ilist.remove(n.clist[i+1].ilist[0])
            else:
                n.clist[i].vlist.append(n.clist[i+1].vlist[0])
                n.clist[i+1].vlist.remove(n.clist[i+1].vlist[0])
                n.ilist[i]=n.clist[i+1].vlist[0].key
        def del_node(n,kv):
            if not n.isleaf():
                p=bisect_right(n.ilist,kv)
                if p==len(n.ilist):
                    if not n.clist[p].isempty():
                        return del_node(n.clist[p],kv)
                    elif not n.clist[p-1].isempty():
                        tran_l2r(n,p-1)
                        return del_node(n.clist[p],kv)
                    else:
                        return del_node(merge(n,p),kv)
                else:
                    if not n.clist[p].isempty():
                        return del_node(n.clist[p],kv)
                    elif not n.clist[p+1].isempty():
                        tran_r2l(n,p)
                        return del_node(n.clist[p],kv)
                    else:
                        return del_node(merge(n,p),kv)
            else:
                p=bisect_left(n.vlist,kv)
                try:
                    pp=n.vlist[p]
                except IndexError:
                    return -1
                else:
                    if pp!=kv:
                        return -1
                    else:
                        n.vlist.remove(kv)
                        return 0
        del_node(self.__root,key_value)

key_list = (1, 7, 38, 55, 89, 225, 251, 372, 410, 436, 510, 691, 693, 721, 723, 724, 860, 926, 985, 991)
def test():
    #初始化数据源
    mini=50
    maxi=200
    testlist=[]
    print "len is: ", len(key_list)
    for i in range(20):
        #key=randint(1,1000)
        key=key_list[i]
        value=choice(['Do', 'Re', 'Mi', 'Fa', 'So', 'La', 'Si'])
        testlist.append(KeyValue(key,value))

    #初始化B树
    mybptree=Bptree(4, 4)
    '''
    for x in testlist:
        print x.key, x.value
    '''
    #插入操作
    for x in testlist:
        mybptree.insert(x)
        print "\n\n-------------------------------"
        mybptree.show()

    print "-------------------------------"
    mybptree.show()

    #查找操作
    print('\nnow we are searching item between %d and %d\n==>'%(mini,maxi))
    print([v.key for v in mybptree.search(mini,maxi)])


    #删除操作
    mybptree.delete(testlist[0])
    print('\n删除 {0}后， the newtree is:\n'.format(testlist[0]));
    mybptree.show()


    #深度遍历操作
    print('\nkey of this b+tree is \n')
    print([kv.key for kv in mybptree.traversal()])




if __name__ == '__main__':
    test();
