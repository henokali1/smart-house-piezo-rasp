#client
import socket  
import time
import base64
import picamera

camera = picamera.PiCamera()

# create TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# retrieve local hostname
local_hostname = socket.gethostname()

# get fully qualified hostname
local_fqdn = socket.getfqdn()

# get the according IP address
ip_address = socket.gethostbyname(local_hostname)

# bind the socket to the port 23456, and connect
server_address = ('104.248.143.11', 5005)  
sock.connect(server_address)  
print ("connecting to %s (%s) with %s" % (local_hostname, local_fqdn, ip_address))

# define example data to be sent to the server
temperature_data = ["15", "22", "21", "26", "25", "19"]  
for i in range(0,30,1):  
    #new_data = str("From Clinet: %s\n" % entry).encode("utf-8")
    #msg = str(entry) + ' :	From clinet: ' + str(time.time()).encode("utf-8")
    camera.capture('img.jpg')
    img_64 = base64.encodestring(open('img.jpg',"rb").read())
    msg = str(len(img_64)) + '	:	' +  str(i) + '	:	' + str(time.time())
    sock.sendall(msg)
    # wait for two seconds
    # time.sleep(2)

# close connection
sock.close()  







img = 'img.jpg'
img_64 = base64.encodestring(open(img,"rb").read())
print(len(img_64))