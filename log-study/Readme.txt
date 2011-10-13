

在x86上面编译log4c很简单.
下载log4c-1.2.1.tar.gz, 解压, ./configure; make; make install
That's OK!

下面说一下交叉编译log4c遇到的问题.
我的./configure如下:
/mnt/share_disk/program-source/log4c-1.2.1/configure --prefix=/mnt/share_disk/program-files/log4c-ppc/ CC=ppc_82xx-gcc  --host=ppc
出现如下问题:
error: expat.h: No such file or directory

    expat是什么东西呢?
    Expat is James Clark's Expat XML parser library in C.
    It is a stream oriented parser that requires setting handlers to deal with the structure that the parser discovers in the document.

    解决方法1:
    下载expat源代码, 编译安装到某个路径下面, 然后在configure时用--with-expat-prefix指定所安装的路径.
    解决方法2:
    直接--without-expat, 这时log4c将自己用yacc and lex for parsing xml, 我直接用该方法.反正对解析效率没有要求.

undefined reference to `rpl_realloc`
undefined reference to `rpl_malloc`
    原来, 交叉编译时, 会将下面三个变量设为no, 我们强行设置为yes即可.
    在configure前执行
    export ac_cv_func_realloc_0_nonnull=${ac_cv_func_realloc_0_nonnull=yes}
    export ac_cv_func_realloc_works=${ac_cv_func_realloc_works=yes}
    export ac_cv_func_malloc_0_nonnull=yes

忽然想到, protocol_buffer在se上面时会出现malloc空间不足的问题, 代码里面有没有这样的功能呢?
看了一下protobuf-c-0.14的代码,确实没有相应的内容出现.
估计要更改头文件了.

