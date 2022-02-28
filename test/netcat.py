# -*- coding: utf-8 -*-
# @Author: tlucanti
# @Date:   2022-02-21 16:38:22
# @Last Modified by:   tlucanti
# @Last Modified time: 2022-02-28 15:47:36

import sys
import socket
import time
import threading

class netcat:

	def __init__(self, ip, port, blocking=False):
		self.sock = socket.socket()
		self.sock.connect((ip, int(port)))
		self.sock.setblocking(blocking)
		print(f'connected to {ip} {port}')
		self.__run = False

	def recv(self):
		while True:
			try:
				data = self.sock.recv(5000).decode('utf-8')
				if (len(data) == 0):
					time.sleep(0.5)
					continue
				if self.__run:
					print(data.strip())
					continue
				return data
			except BlockingIOError:
				time.sleep(0.5)

	def send(self, data=''):
		while True:
			if self.__run:
				data = input()
			data = data.replace(r'\x01', '\x01')
			data = bytes(data + '\r\n', 'utf-8')
			self.sock.send(data)
			if not self.__run:
				return

	def run(self):
		self.__run = True
		t1 = threading.Thread(target=self.recv)
		t2 = threading.Thread(target=self.send)
		t1.start()
		t2.start()


def main():
	if len(sys.argv) != 3:
		print('invalid arguments, usage: <ip> <port>')
		return 0
	ip = sys.argv[1]
	port = sys.argv[2]
	netcat(ip, port).run()


if __name__ == '__main__':
	main()
