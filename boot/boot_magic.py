# 用于为boot loader的映像末尾写上0xAA55
from os.path import abspath
from sys import argv

# sanity check
if len(argv) != 2:
    print('No filepath!')
    exit(1)

# 获取绝对路径
p = abspath(argv[1])

# 读取里面的数据
with open(p, 'rb') as origin:
    buf = origin.read()

# sanity check
num = len(buf)
if num > 510:
    print('Overflow! Max size 510 bytes, now', num, 'bytes.')
    exit(1)
else:
    print('boot block is', num, 'bytes (max 510)')

# 生成bootable block
zeroZone = 510 - num
with open(p, 'wb') as target:
    target.write(buf)
    target.write(b'\x00'*zeroZone)
    target.write(b'\x55\xaa')
