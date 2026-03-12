from kafka import KafkaProducer
import json
import time
import random
from datetime import datetime

producer = KafkaProducer(
    bootstrap_servers=['localhost:9092'],
    value_serializer=lambda v: json.dumps(v).encode('utf-8')
)

print("开始持续生产流量数据...（Ctrl+C 停止）")

DIPS = ["8.8.8.8", "114.114.114.114", "1.1.1.1", "223.5.5.5", "10.0.0.1", "172.16.1.100"]

while True:
    dip = random.choice(DIPS)
    data = {
        "start_time": datetime.now().isoformat(),      # 开始时间
        "duration": random.randint(5, 120),            # 持续时间（秒）
        "up_packets": random.randint(20, 2000),        # 上行包数
        "up_bytes": random.randint(2000, 200000),      # 上行字节数
        "down_packets": random.randint(30, 4000),      # 下行包数
        "down_bytes": random.randint(3000, 400000),    # 下行字节数
        "sip": f"192.168.{random.randint(1,255)}.{random.randint(1,255)}",   # 源IP
        "dip": dip,                                                          # 目的IP（聚合关键字段）
        "src_port": random.randint(1024, 65535),
        "dst_port": random.choice([80, 443, 22, 3389, 53, 3306]),
        "protocol": random.choice(["TCP", "UDP"])
    }
    
    producer.send('flows', value=data)
    print(f"发送 → {data['start_time']}, dip={data['dip']}  protocol={data['protocol']}  上行 {data['up_bytes']} 字节")
    #print(f"已发送: {data['start_time']} | 上行 {data['up_bytes']} 字节")
    
    time.sleep(0.2)  # 每 0.2 秒发一条，模拟持续输入
