#/bin/python3
import asyncio
import time
import requests
async def test2(i):
    r = await other_test(i)
    print("third...", i, r)
async def other_test(i):
    r = requests.get(i)
    print("first ", i)
    await asyncio.sleep(4)
    print("second...", time.time() - start)
    return r
url = ["https://segmentfault.com/p/1210000013564725",
       "https://www.jianshu.com/p/83badc8028bd",
       "https://www.baidu.com/"]
loop = asyncio.get_event_loop()
task = [asyncio.ensure_future(test2(i)) for i in url]
start = time.time()
loop.run_until_complete(asyncio.wait(task))
endtime = time.time() - start
print("end,,,,", endtime)
loop.close()
