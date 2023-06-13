##. 进程启动方式

主进程proc_type=primary
副进程proc_type=secondary，允许多个副进程同时存在，多个副进程可以使用同一个核来启动。

##. 数据通道

每一个副进程使用一个单独的ring。

主进程发送：
rte_ring_enqueue接口

副进程接收：
rte_ring_dequeue接口

主副之前的数据交互可能有两种设计：
一是主进程在匹配某种策略后，将数据送到副进程，不需要再将数据从副进程送回主进程。
推荐用这种方式。
二是匹配插件的逻辑设计，主进程把当前所有的数据送到副进程，副进程自己写复杂的匹配逻辑拿到自己想要的数据。如果未匹配的数据再由副进程送回主进程。
这种性能很可能会是问题。

##. 主进程process_mgmt_table

|name|match|ring|status|
|dns|port == 53|ring_dns|1|
|smtp|port == 25 |ring_smtp|1|

##. 主副进程的管理通道

主进程与副进程之间通信：副进程通知主进程发送数据，主进程检查各个副进程的状态。

副进程启动后，主动向主进程请求开辟数据通道传递数据。副进程发送用rte_mp_msg来发送开辟数据通道的消息，主进程接收用rte_mp_action_register注册回调。
主进程会根据process_mgmt_table中的过滤条件，将匹配的数据发送到相应的ring上。
主进程开辟数据通道向副进程发送数据之后，定时检测副进程的状态，使用函数rte_mp_request_async来检查各个副进程的状态，如果检测到副进程异常，则主动释放数据通道资源。
定时检测副进程的状态的方式，通过reply->msgs[i].param中传回的内容(理论上每个副进程应该在reply->msgs[i].param中带上自己的名字)来判断，如果process_mgmt_table中有名字未能返回，则删除对应的ring。

##. dpi_server与主进程的管理通道

restful设计（编译，启动，发送/停止，卸载）

*启动：*
dpi_server向主进程提交的restful请求中，包括匹配条件与副进程名称。
主进程收到请求后，将根据名称创建副进程；并根据名称建立映射表；


*发送/停止：*
主进程收到请求后，将继续发送/停止向ring发送数据；

*卸载：*
删除相应的ring，停止副进程的运行。


##. 示例

./build/simple_mp -l 0-1 -n 1 --proc-type=primary
./build/simple_mp -l 1-2 -n 1 --proc-type=secondary
