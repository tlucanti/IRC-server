# -*- coding: utf-8 -*-
# @Author: tlucanti
# @Date:   2022-02-21 16:38:22
# @Last Modified by:   tlucanti
# @Last Modified time: 2022-02-21 20:11:57

import sys
import socket
import time
import threading


def recv(sock):
	while True:
		try:
			data = sock.recv(5000).decode('utf-8')
			if (len(data) == 0):
				time.sleep(0.5)
				continue
			print(data, end='')
		except BlockingIOError:
			time.sleep(0.5)


def send(sock):
	while True:
		data = input().replace(R'\x01', '\x01')
		data = bytes(data + '\n', 'utf-8')
		sock.send(data)


def main():
	ip = sys.argv[1]
	port = sys.argv[2]

	sock = socket.socket()
	sock.connect((ip, int(port)))
	sock.setblocking(0)
	print(f'connected to {ip} {port}')

	t1 = threading.Thread(target=recv, args=[sock])
	t2 = threading.Thread(target=send, args=[sock])
	t1.start()
	t2.start()

if __name__ == '__main__':
	main()
