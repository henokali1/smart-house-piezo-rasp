import urllib.request


t = urllib.request.urlopen("http://46.101.144.34:5000/temp/33")
h = urllib.request.urlopen("http://46.101.144.34:5000/humidity/77")
c02 = urllib.request.urlopen("http://46.101.144.34:5000/carbon_dioxide/99")
print(t.read())
print(h.read())
print(c02.read())

