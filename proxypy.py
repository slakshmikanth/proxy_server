import socket
import sys
import time

port = 8080

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('', port))
    print 'bind and create done'
except socket.error:
    print 'Failed to create socket or bind'
    sys.exit()

s.listen(5)
print 'listening'
conn, addr = s.accept()
try:
    request = conn.recv(1024)
    print request
    ind = request.find('Host: ')
    host = request[ind+6:]
    print host.strip()
    res = socket.getaddrinfo(host.strip(), 80, socket.AF_INET, socket.SOCK_STREAM)
    s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s1.connect(res[0][4])
    s1.sendall(request)
    resp = ''
    s1.recv(resp)
    print resp
    s1.close()
    conn.sendall(resp)
    conn.close()
    s.close()
except KeyboardInterrupt:
    conn.close()
    s.close()
    sys.exit()