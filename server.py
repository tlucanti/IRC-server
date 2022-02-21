# -*- coding: utf-8 -*-
# @Author: tlucanti
# @Date:   2022-02-21 20:21:44
# @Last Modified by:   tlucanti
# @Last Modified time: 2022-02-21 20:29:59

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
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.bind((ip, int(port)))
	sock.listen(10)
	conn, addr = sock.accept()
	print('connected:', addr)

	t1 = threading.Thread(target=recv, args=[conn])
	t2 = threading.Thread(target=send, args=[conn])
	t1.start()
	t2.start()

if __name__ == '__main__':
	main()
