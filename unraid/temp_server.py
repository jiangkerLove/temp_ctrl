# coding=utf-8
import os
import socket

from time import sleep


def get_temp():
    temp = os.popen("ls -1 /dev/sd? | xargs -n1 smartctl -A | grep Celsius").read()
    item_temp = temp.split('\n')
    max_temp = 0
    for item in item_temp:
        if not item:
            continue
        split = item.split(' ')
        index = split.index("(Min/Max")
        item_temp = split[index - 1]
        max_temp = max(int(item_temp), max_temp)
    return max_temp

def convert(temp):
    if temp > 44:
        return 1000
    elif temp < 35:
        return 0
    else:
        return (temp - 35) * 100

sk = socket.socket()  # 创建socket对象
sk.bind(("0.0.0.0", 9001))  # 绑定IP和端口
sk.listen()  # 开启监听

print("start listen")

while 1:  # 让服务端和客户端循环通信
    conn, address = sk.accept()  # 等待客户端连接 阻塞
    print("accept client")
    try:
        while 1:
            temp = get_temp()
            send_msg = str(convert(temp))+'\n'  # 要发送的消息
            conn.send(send_msg.encode("utf-8"))  # 发送消息给客户端
            print("send temp success, current temp: " + str(temp) + "fan speed: " + send_msg)
            sleep(1)
    except OSError:
        print("send to client error")
        conn.close()
