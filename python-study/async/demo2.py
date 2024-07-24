import asyncio

async def coroutine_one():
    print('Coroutine one started')
    await asyncio.sleep(1)
    print('Coroutine one ended')

async def coroutine_two():
    print('Coroutine two started')
    await asyncio.sleep(2)
    print('Coroutine two ended')

async def main():
    tasks = [coroutine_one(), coroutine_two()]
    await asyncio.gather(*tasks)

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
