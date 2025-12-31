from mitmproxy import ctx
from mitmproxy import http
from urllib.parse import urlparse, parse_qs, unquote
import socket

# 假设审计工具监听本地 UDP/TCP 端口
audit_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def request(flow: http.HTTPFlow):
    info = ctx.log.info
    #flow.request.headers['User-Agent'] = 'MitmProxy'
    #print(flow.request.headers)
    request = flow.request
    info("embedway debug, host: " + request.host)
    info("embedway debug,  url: " + request.url)
    url = request.url
    parsed_url = urlparse(url)
    query_params = parsed_url.query

    params_dict = parse_qs(query_params)
    wd_encoded = params_dict.get("wd", [None])[0]  # 提取wd的值

    # URL解码（将%xx格式转换为汉字）
    if wd_encoded:
        wd_decoded = unquote(wd_encoded, encoding="utf-8")
        print("\n\n---------------------------")
        print(f"搜索内容：{wd_encoded}")
        print("---------------------------\n\n")

def response(flow: http.HTTPFlow):
    response = flow.response
    info = ctx.log.info
    payload = f"URL: {flow.request.url}\n\n{flow.response.content}"
    info("embedway debug, rsp code: " + str(response.status_code))
    print("embedway response payload, ", payload.encode())

#def response(flow: http.HTTPFlow):
#    # 将原始解密后的响应流镜像出去
#    payload = f"URL: {flow.request.url}\n\n{flow.response.content}"
#    audit_socket.sendto(payload.encode(), ("127.0.0.1", 9999))
