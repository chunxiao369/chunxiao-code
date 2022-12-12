
在安装hyperscan时候，需要将pcre的源码放到hyperscan的目录下，注意使用版本pcre-8.41
匹配函数ch_scan，相比hs_scan，其命中回调函数增加了捕获组个数，利用这个可以将各个捕获组的匹配信息写到传入的参数中
示范代码（采用hyperscan5.4）见附件，编译方式 gcc -o main main.c  -lchimera -lpcre -lhs -lstdc++ -lm
