
## 说明

  当前测试依赖将软件启动好，且将数据包放在指定目录下
  后续软件启动过程也做好自动化
  启动好后，对pat_common中的ip/port进行配置
  默认按文件名ASCLL码顺序执行，文件内的用例按照从上往下执行。
  为了避免每个文件的测试中install的延时带来效率问题，所以写了一次install的函数
  用order指定顺序，用例执行顺序order值0,1,2...-2,-1
  另外对结果的判断，除了用entry计数，还应该加入落盘文件大小判断

## 运行单个用例

pytest elem_http_test.py

## 运行所有

pytest

## 运行结果

失败
3 failed, 6 passed in 1.82 seconds

成功
3 passed in 1.41 seconds
