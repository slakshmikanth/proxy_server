import socket
import sys
import time

def from_string(s):
  "Convert dotted IPv4 address to integer."
  return reduce(lambda a,b: a<<8 | b, map(int, s.split(".")))
 
def to_string(ip):
  "Convert 32-bit integer to dotted IPv4 address."
  return ".".join(map(lambda n: str(ip>>n & 0xFF), [24,16,8,0]))

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
    while True:
        request = conn.recv(100000)
        print request
        if request[0:7] == 'CONNECT':
            Servport = 443
        else:
            Servport = 80
        ind = request.find('Host: ')
        ind2 = request.find('User-Agent:')
        host = request[ind+6:ind2]
        print host.strip()
        res = socket.getaddrinfo(host.strip(), 80, socket.AF_INET, socket.SOCK_STREAM)
        s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s1.connect(res[0][4])
        s1.sendall(request)
        resp = ''
        resp1 = 'a'
        while(resp1 != ''):
            resp1 = ''
            resp1 = s1.recv(1024)
            print resp1
            resp += resp1
        print resp
        s1.close()
        conn.sendall(resp)
        #conn.close()
    s.close()
except KeyboardInterrupt, AttributeError:
    conn.close()
    s.close()
    sys.exit()