
# virtio

+---------+------+--------+----------+--+
|         +------+        +----------+  |
| user    |      |        |          |  |
| space   |      |        |  guest   |  |
|         |      |        |          |  |
|    +----+ qemu |        | +-+------+  |
|    |    |      |        | | virtio |  |
|    |    |      |        | | driver |  |
|    |    +------+        +-+---++---+  |
|    |                          |       |
|    |       ^                  |       |
|    v       |                  v       |
|            |                          |
+-+-----+-----------------+--+-------+--+
| |tap  |    +------------+ kvm.ko   |  |
| +-----+                 +--+-------+  |
|                kernel                 |
+---------------------------------------+

# vhost-kernel

+---------+------+--------+----------+--+
|         +------+        +----------+  |
| user    |      |        |          |  |
| space   |      |        |  guest   |  |
|         |      |        |          |  |
|         | qemu |        | +-+------+  |
|         |      |        | | virtio |  |
|         |      |        | | driver |  |
|         +------+        +-+---++---+  |
|                               |       |
|                               |       |
|                               v       |
|                                       |
+-+-----+---+-+----+------+----+--+-----+
| |tap  |   | vhost-net.ko|    | kvm.ko |
| +---^-+   +------+----^-+    +----+---+
|     |-------|  kernel |-----------|   |
+---------------------------------------+

# vhost-user

+---------------+------+--+----------+--+
|                +------+  +----------+  |
|  user          |      |  |          |  |
|  space         |      |  |  guest   |  |
|                |      |  |          |  |
|   +-+-------+  | qemu |  | +-+------+  |
|   |  vhost  |  |      |  | | virtio |  |
|   |  back   |  |      |  | | driver |  |
|   +---------+  +------+  +-+---++---+  |
|    |  ^                        |       |
|    |  |                        |       |
|    |  |                        v       |
|    v  |                                |
+-------------------------------+--+-----+
| nic + +-----------------------+ kvm.ko |
|-----+                         +--+-----+
|                 kernel                 |
+----------------------------------------+

from
https://www.programmersought.com/article/55695245833/
