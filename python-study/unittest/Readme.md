#unittest学习
首先是要写好TestCase，然后由TestLoader加载TestCase到TestSuite，然后由TextTestRunner来 运行TestSuite，运行的结果保存在TextTestResult中，整个过程集成在unittest.main模块中。


#使用
python -m unittest test_module1 test_module2
python -m unittest test_module.TestClass
python -m unittest test_module.TestClass.test_method

python -m unittest -v test_module

python -m unittest -h

python -m unittest demo0
python -m unittest demo0.TestStringMethods
python -m unittest demo0.TestStringMethods.test_split
