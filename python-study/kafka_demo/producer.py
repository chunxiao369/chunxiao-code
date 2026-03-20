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

#山西省：太原市,大同市,阳泉市,长治市,晋城市,朔州市,晋中市,运城市,忻州市,临汾市,吕梁市
#河北省：石家庄市,唐山市,秦皇岛市,邯郸市,邢台市,保定市,张家口市,承德市,沧州市,廊坊市,衡水市
#陕西省：西安市,铜川市,宝鸡市,咸阳市,渭南市,延安市,汉中市,榆林市,安康市,商洛市
#湖南省：长沙市,株洲市,湘潭市,衡阳市,邵阳市,岳阳市,常德市
provs = ["湖南省", "陕西省", "河北省", "山西省", "上海市", "北京市"]

citys_shx = ["太原市", "大同市", "阳泉市", "长治市", "晋城市", "朔州市", "晋中市", "运城市", "忻州市", "临汾市", "吕梁市"]
citys_hb = ["石家庄市", "唐山市", "秦皇岛市", "邯郸市", "邢台市", "保定市", "张家口市", "承德市", "沧州市", "廊坊市", "衡水市"]
citys_sx = ["西安市", "铜川市", "宝鸡市", "咸阳市", "渭南市", "延安市", "汉中市", "榆林市", "安康市", "商洛市"]
citys_hn = ["长沙市", "株洲市", "湘潭市", "衡阳市", "邵阳市", "岳阳市", "常德市"]

# 建立省份和城市列表的映射字典（关键）
prov_city_map = {
    "湖南省": citys_hn,
    "陕西省": citys_sx,
    "河北省": citys_hb,
    "山西省": citys_shx,
    "上海市": ["上海市"],  # 直辖市直接映射为自身
    "北京市": ["北京市"]   # 直辖市直接映射为自身
}

DIPS = ["8.8.8.8", "114.114.114.114", "1.1.1.1", "223.5.5.5", "10.0.0.1", "172.16.1.100"]
BASE_TYPES = ["HTTP", "HTTPS", "DNS", "SSH", "SMB", "SMTP",  "FTP", "UNKNOWN"]
APPS = ["QQ", "Wechat", "Alipay", "抖音", "bilibili", "喜马拉雅", "douban", "UNKNOWN"]

while True:
    dip = random.choice(DIPS)
    province = random.choice(provs)
    # 先判断省份是否在映射字典中（增加代码健壮性）
    if province in prov_city_map:
        city = random.choice(prov_city_map[province])
        #print(f"随机选中的省份：{province}")
        #print(f"该省份下随机选中的城市：{city}")
    else:
        print(f"暂未配置{province}的城市列表")
    data = {
        "dir": random.choice([1, 2, 3, 4, 5, 6]),
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
        "protocol": random.choice(["TCP", "UDP"]),
        "base_proto": random.choice(BASE_TYPES),
        "dst_province": province,
        "dst_city": city,
        "src_province": "山西省",
        "src_city": random.choice(citys_shx),
        "app_id": random.choice(APPS)
    }
    
    producer.send('flows', value=data)
    print(f"发送 → {data['start_time']}, dip={data['dip']}  protocol={data['protocol']}  上行 {data['up_bytes']} 字节")
    #print(f"已发送: {data['start_time']} | 上行 {data['up_bytes']} 字节")
    
    time.sleep(0.5)  # 每 0.5 秒发一条，模拟持续输入
