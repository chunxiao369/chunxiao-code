def Fab(max):
    n, a, b = 0, 0, 1
    while n < max:
        yield b
        # print b
        a, b = b, a + b
        n = n + 1
if __name__ == '__main__':
    for n in Fab(10):
        print n
