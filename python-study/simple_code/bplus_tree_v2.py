#!/usr/bin/env python3
from random import randint,choice
from bisect import bisect_right,bisect_left
from collections import deque

#Firstly, initial the errors
class InitError(Exception):
    pass
class ParaError(Exception):
    pass
#The key-value class
class KeyValue(object):
    __slots__=('key','value')
    def __init__(self,key,value):
        self.key=key
        self.value=value
    def __str__(self):
        return str((self.key,self.value))
    def __lt__(self,key):
        return self.key<key
    def __gt__(self,key):
        return self.key>key
    def __ge__(self,key):
        return self.key>=key
    def __le__(self,key):
        return self.key<=key

#internal Node class
class Bptree_InterNode(object):
    def __init__(self,M):
        if not isinstance(M,int):
            raise InitError('M must be int')
        if M<=3:
            raise InitError('M must be greater then 3')
        else:
            self.__M=M
            self.clist=[]
            self.ilist=[]
            self.par=None
    def isleaf(self):
        return False
    def isfull(self):
        return len(self.ilist)>=self.M
    def isempty(self):
        return len(self.ilist)<=(self.M+1)//2
    @property
    def M(self):
        return self.__M
#leaf class
class Bptree_Leaf(object):
    def __init__(self,L):
        if not isinstance(L,int):
            raise InitError('L must be int')
        else:
            self.__L=L
            self.vlist=[]
            self.bro=[]
            self.par=None
    def isleaf(self):
        return True
    def isfull(self):
        return len(self.vlist)>self.L
    def isempty(self):
        return len(self.vlist)<=(self.L+1)//2
    @property
    def L(self):
        return self.__L
#Printing the tree
def printbtree(n,h,L,M):
#h deep of the tree, n is the node , L is leaf number and  M is the internal node number
    if n.isleaf():
        if h!=1:
            q=[]
            u=[]
            for count in range(0,len(n.vlist)):
                q.append(n.vlist[count].key)
                u.append(n.vlist[count].value)
            #print(q)
            for count in range(1,L-len(n.vlist)+1):
                q.append(None)
                u.append(None)
            for count in range(1,h):
                print("\t", end='')
            print("Leaf(keys={}, values={})".format(q,u))
        else:
        #No Internal Nodes
            u=[]
            for tt in range(1,L+1):
                u.append(None)
            print("Internal(keys={}, children=".format(u))
            if len(n.vlist)!=0:
                q=[]
                u=[]
                for count in range(0,len(n.vlist)):
                    q.append(n.vlist[count].key)
                    u.append(n.vlist[count].value)
                for count in range(1,L-len(n.vlist)+1):
                    q.append(None)
                    u.append(None)
                for count in range(1,h+1):
                    print("\t", end='')
                print("Leaf(keys={}, values={})".format(q,u))
                for count in range(1,M):
                    print("\t", end='')
                    print(None)
                print(")",end='')
            else:
                for count in range(1,M+1):
                    print("\tNone")
                print(")",end='')
    else:
        q=n.ilist[:]
        #print(q)
        for count in range(1,L-len(n.ilist)+1):
            q.append(None)
        for count in range(1,h):
            print("\t", end='')
        print("Internal(keys={}, children=".format(q))
        for n1 in n.clist:
            printbtree(n1,h+1,L,M)
        for count in range(1,M-len(n.clist)+1):
            for tab in range(1,h+1):
                print("\t", end='')
            print("None")
        for count in range(1,h):
            print("\t", end='')
        if h==1:
            print(")",end='')
        else:
            print(")")
#Obtain all the leafs
def traversal(n):
    result=[]
    result.extend(n.vlist)
    if (n.bro==[]):
        return result
    #print("Tot={}".format(len(n.bro)))
    for i in range(0,len(n.bro))[::-1]:
        q=traversal(n.bro[i])
        result.extend(q)
    return result
#create a Bptree
class BTree:
    """
    B+ Tree class
    """
    def __init__(self, upper_keyx_keys):
        """
        Initializes an empty B+ Tree
        The only parameter is "upper_keyx_keys" which is the upper_keyximum
        number of keys that can be stored in each node.
        """
        self.__M=upper_keyx_keys+1
        self.__L=upper_keyx_keys
        self.__root=Bptree_Leaf(upper_keyx_keys)
        self.__leaf=self.__root
        #raise NotImplementedError()
    @property
    def M(self):
        return self.__M
    @property
    def L(self):
        return self.__L
#insert
    def insert(self, search_key, value):
        """
        Adds a key and value (address) to the B+ Tree.
        Keys upper_keyy be not unique (search key)
        """
        key_value=KeyValue(search_key,value)
        node=self.__root
        def split_node(n1):
            #print("Splitting Node")
            mid=(self.M+2)//2
            newnode=Bptree_InterNode(self.M)
            newnode.ilist=n1.ilist[mid:]
            newnode.clist=n1.clist[mid:]
            newnode.par=n1.par
            for c in newnode.clist:
                c.par=newnode
            if n1.par is None:
                newroot=Bptree_InterNode(self.M)
                newroot.ilist=[n1.ilist[mid-1]]
                newroot.clist=[n1,newnode]
                n1.par=newnode.par=newroot
                self.__root=newroot
            else:
                i=n1.par.clist.index(n1)
                n1.par.ilist.insert(i,n1.ilist[mid-1])
                n1.par.clist.insert(i+1,newnode)
                if n1.par.isfull():
                    split_node(n1.par)
            n1.ilist=n1.ilist[:mid-1]
            n1.clist=n1.clist[:mid]
            return n1.par
        def split_leaf(n2):
            #print("Splitting leaf")
            mid=(self.L+2)//2
            newleaf=Bptree_Leaf(self.L)
            newleaf.vlist=n2.vlist[mid:]
            if n2.par==None:
                newroot=Bptree_InterNode(self.M)
                newroot.ilist=[n2.vlist[mid].key]
                newroot.clist=[n2,newleaf]
                n2.par=newleaf.par=newroot
                self.__root=newroot
            else:
                i=n2.par.clist.index(n2)
                n2.par.ilist.insert(i,n2.vlist[mid].key)
                n2.par.clist.insert(i+1,newleaf)
                newleaf.par=n2.par
                if n2.par.isfull():
                    split_node(n2.par)
                else:
                    pass
            n2.vlist=n2.vlist[:mid]
            n2.bro.append(newleaf)
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
        #raise NotImplementedError()

#Print Tree
    def __str__(self):
        """
        Returns a string representation of the B+ Tree
        """
        root=self.__root
        printbtree(root,1,self.__L,self.__M)   #print the tree
        return ''
        #raise NotImplementedError()
#Range lookup
    def lookup_range(self,low,up):
        lowkey=KeyValue(low,1)
        upkey=KeyValue(up,1)
        l=self.__leaf
        q=traversal(l)
        #print([kv.value for kv in q])
        #q.sort()
        p1=bisect_left(q,lowkey)
        p2=bisect_right(q,upkey)
        #print(p1,p2)
        return [kv.value for kv in q[p1:p2]]
        #kv.key for kv in mybptree.findk()])
#lookup
    def lookup(self, key):
        """
        Returns the address accociated with a key.z
        If there are duplicates with that key, return the first one added.
        If no such key exists, return None.
        """
        q=self.lookup_range(key,key)
        if (q!=[]):
            return q[0]
        return None
        #raise NotImplementedError()
#My own find
    def findk(self, lower=None, upper=None):
        lower_key=KeyValue(lower,1)
        upper_key=KeyValue(upper,1)
        print("{} {}",lower,upper)
        result=[]
        node=self.__root
        leaf=self.__leaf
        if lower_key is not None and upper_key is not None and lower_key>upper_key:
            raise ParaError('upper bound must be greater or equal than lower bound')
        if lower_key is None:
            while True:
                for kv in leaf.vlist:
                    if kv<=upper_key:
                        result.append(kv)
                    else:
                        return result
                if leaf.bro==None:
                    return result
                else:
                    leaf=leaf.bro
        elif upper_key is None:
            index,leaf=search_key(node,lower_key)
            result.extend(leaf.vlist[index:])
            while True:
                if leaf.bro==None:
                    return result
                else:
                    leaf=leaf.bro
                    result.extend(leaf.vlist)
        else:
            #print('Here')
            if lower_key==upper_key:
                i,l=search_key(node,lower_key)
                print("Th={}",i)
                try:
                    if l.vlist[i]==lower_key:
                        result.append(l.vlist[i])
                        q=[]
                        for tur in result:
                            q.append(tur.value)
                        return q
                    else:
                        return result
                except IndexError:
                    return result
            else:
                i1,l1=search_key(node,lower_key)
                i2,l2=search_key(node,upper_key)
                print("Here={}",lower_key)
                if l1 is l2:
                    if i1==i2:
                        return result
                    else:
                        result.extend(l1.vlist[i1:i2])
                        q=[]
                        for tur in result:
                            q.append(tur.value)
                        return q
                else:
                    result.extend(l1.vlist[i1:])
                    l=l1
                    while True:
                        if l.bro==l2:
                            result.extend(l2.vlist[:i2+1])
                            q=[]
                            for tur in result:
                                q.append(tur.value)
                            return q
                        else:
                            result.extend(l.bro.vlist)
                            l=l.bro
        #raise NotImplementedError()
#My own show :simply print the tree for debugging
    def show(self):
        print('this b+tree is:\n')
        q=deque()
        h=0
        q.append([self.__root,h])
        #print('root==',self.__root)
        while True:
            try:
                w,hei=q.popleft()
            except IndexError:
                return
            else:
                if not w.isleaf():
                    print(w.ilist,'the height is',hei)
                    if hei==h:
                        h+=1
                    q.extend([[i,h] for i in w.clist])
                else:
                    print([v.key for v in w.vlist],'the leaf is,',hei)
#Small tests
def test():
    testlist=[]
    mybptree=BTree(3)
    letters = "Hi_class,_my_name_"
    print("Large BTree")
    for i, letter in enumerate(letters):
        mybptree.insert(letter, i)
    #print('the searching result =', mybptree.lookup('a'))
    print('\nkey of this b+tree is \n')
    mybptree.lookup(' ')
    print(mybptree.lookup(' '))
    print(mybptree)
    #print([kv.key for kv in mybptree.traversal()])
    #print [kv.key for kv in mybptree.search(mini,maxi)]
if __name__=='__main__':
    test()

