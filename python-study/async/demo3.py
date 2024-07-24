import asyncio

async def producer(queue):
    print('Producer started')
    for i in range(5):
        await asyncio.sleep(1)
        await queue.put(i)
    await queue.put(None)
    print('Producer ended')

async def consumer(queue):
    print('Consumer started')
    while True:
        item = await queue.get()
        if item is None:
            queue.task_done()
            break
        print(f'Consumed {item}')
        await asyncio.sleep(0.5)
        queue.task_done()
    print('Consumer ended')

async def main():
    queue = asyncio.Queue()
    producer_task = asyncio.create_task(producer(queue))
    consumer_task = asyncio.create_task(consumer(queue))
    await asyncio.gather(producer_task)
    await queue.join()
    await consumer_task

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
