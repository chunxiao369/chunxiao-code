from kafka import KafkaConsumer, KafkaProducer
import json
import time
from datetime import datetime
from collections import defaultdict

consumer = KafkaConsumer(
    'flows',
    bootstrap_servers=['localhost:9092'],
    auto_offset_reset='latest',
    value_deserializer=lambda x: json.loads(x.decode('utf-8'))
)

output_producer = KafkaProducer(
    bootstrap_servers=['localhost:9092'],
    value_serializer=lambda v: json.dumps(v).encode('utf-8')
)

print("启动【按时间窗口 + 目的IP】实时聚合分析（每 10 秒一个窗口）...")

window_start = time.time()

# 每个窗口内按 dip 分组统计
window_stats = defaultdict(lambda: {
    "flow_count": 0,
    "total_up_packets": 0,
    "total_up_bytes": 0,
    "total_down_packets": 0,
    "total_down_bytes": 0
})

for message in consumer:
    flow = message.value
    dip = flow["dip"]
    
    # 累加到当前 dip
    stats = window_stats[dip]
    stats["flow_count"] += 1
    stats["total_up_packets"] += flow["up_packets"]
    stats["total_up_bytes"] += flow["up_bytes"]
    stats["total_down_packets"] += flow["down_packets"]
    stats["total_down_bytes"] += flow["down_bytes"]
    
    # 每 10 秒输出一次
    if time.time() - window_start >= 10:
        window_time = datetime.now().isoformat()
        
        print(f"\n=== 聚合窗口 {window_time}（持续 10 秒）===")
        print(f"共 {len(window_stats)} 个目的IP")
        
        # 按流量大小排序输出（直观看到热点 dip）
        sorted_dips = sorted(
            window_stats.items(),
            #key=lambda x: x[1]["total_up_bytes"] + x[1]["total_down_bytes"],
            key=lambda x: x[1]["flow_count"],
            reverse=True
        )
        
        for dip, s in sorted_dips:
            total_bytes = s["total_up_bytes"] + s["total_down_bytes"]
            print(f"  └─ dip = {dip:15} "
                  f"记录数: {s['flow_count']:3d} | "
                  f"上行: {s['total_up_bytes']:8,} B | "
                  f"下行: {s['total_down_bytes']:8,} B | "
                  f"总流量: {total_bytes:,} 字节")
        
        # 构造结构化输出数据（发到 aggregated topic，供下游使用）
        output_data = {
            "window_start": window_time,
            "window_duration": 10,
            "per_dip": dict(window_stats)   # 每个 dip 的完整统计
        }
        output_producer.send('aggregated', value=output_data)
        
        # 重置窗口
        window_stats.clear()
        window_start = time.time()
