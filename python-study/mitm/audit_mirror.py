import socket
import json
from mitmproxy import http

# --- 审计工具配置 ---
AUDIT_HOST = "127.0.0.1"
AUDIT_PORT = 9999  # 你的审计工具监听的端口

class AuditMirror:
    def __init__(self):
        # 使用 UDP 模式进行镜像传输，避免审计工具响应慢导致上网卡顿
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def _send_to_audit(self, data):
        """辅助函数：将数据序列化并发送"""
        try:
            # 将字典转为 JSON 字符串
            payload = json.dumps(data, ensure_ascii=False).encode('utf-8')
            # 如果 payload 过大，UDP 可能会丢包，大流量审计建议用 TCP 或写文件
            self.sock.sendto(payload, (AUDIT_HOST, AUDIT_PORT))
        except Exception as e:
            print(f"镜像传输失败: {e}")

    def request(self, flow: http.HTTPFlow):
        """处理并发送请求数据"""
        request_data = {
            "type": "REQUEST",
            "id": flow.id,  # 关联 ID，用于匹配对应的响应
            "client_ip": flow.client_conn.peername[0],
            "method": flow.request.method,
            "url": flow.request.pretty_url,
            "headers": dict(flow.request.headers),
            "content": flow.request.get_content().decode('utf-8', 'ignore')[:5000] # 截取前5000字符
        }
        self._send_to_audit(request_data)

    def response(self, flow: http.HTTPFlow):
        """处理并发送响应数据"""
        response_data = {
            "type": "RESPONSE",
            "id": flow.id,
            "status_code": flow.response.status_code,
            "headers": dict(flow.response.headers),
            "content": flow.response.get_content().decode('utf-8', 'ignore')[:5000] # 截取前5000字符
        }
        self._send_to_audit(response_data)

# 注册插件
addons = [
    AuditMirror()
]
