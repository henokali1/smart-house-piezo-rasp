#!/usr/bin/env python
import os
import time
import serial
import threading
import urllib.request
from gpiozero import LED
from datetime import datetime
from email_sender_mod import send_g_email as es
from ser_com_list import serial_ports as sp

motion_audio_playing = False
plc = LED(21)


print("Avalable port", sp()[0])
ser = serial.Serial(
 port=sp()[0],
 baudrate = 115200,
 parity=serial.PARITY_NONE,
 stopbits=serial.STOPBITS_ONE,
 bytesize=serial.EIGHTBITS,
 timeout=1
)

base_url = 'http://46.101.144.34:5000/'

def post(url):
    t = urllib.request.urlopen(url)
    print(url, "Request Completed")

def strTrd(u):
	t = threading.Thread(target=post, args = (u,))
	t.daemon = True
	t.start()

def motAudio(fn):
	if not motion_audio_playing:
		global motion_audio_playing
		motion_audio_playing = True
		print('Playing Mot Audio File')
		os.system('mpg321 alarm.mp3 &')
		time.sleep(13)
		print('Stopping Mot Audio File')
		motion_audio_playing = False
	else:
		print('Audio Already Playing')


def strMotAudioTrd():
	t = threading.Thread(target=motAudio, args = ('alarm.mp3',))
	t.daemon = True
	t.start()

def noise(fn):
	while 1:
		print('Playing Background Audio File')
		os.system('mpg321 bck_noise.mp3 &')
		print('Stopping Background Audio File')
		print("Background Audio Sleeping for 45")
		time.sleep(45)

def strNoiseAudioTrd():
	t = threading.Thread(target=noise, args = ('bck_noise.mp3',))
	t.daemon = True
	t.start()

def get_millis(start, end):
    now = datetime.now()

    year = str(now.year)
    mon = str(now.month)
    date = str(now.day)

    s_dt_obj = datetime.strptime('{}.{}.{} {}:59'.format(date, mon, year, start),
                           '%d.%m.%Y %H:%M:%S')
    
    e_dt_obj = datetime.strptime('{}.{}.{} {}:59'.format(date, mon, year, end),
                           '%d.%m.%Y %H:%M:%S')

    s_millisec = s_dt_obj.timestamp()
    e_millisec = e_dt_obj.timestamp()

    c_time = time.time()
    in_range = ((s_millisec - c_time)>0) or ((e_millisec - c_time)>0)
    print('s_millisec - c_time= ' +str(s_millisec - c_time))
    print('s_millisec - c_time= ' +str(e_millisec - c_time))
    return {'start_millis': s_millisec, 'end_millis':e_millisec, 'in_range': in_range}

def check_fp_access():

    f = urllib.request.urlopen("http://46.101.144.34:5000/time")
    r = f.read().decode("utf-8") 

    se = r.split('-')

    start = se[0]
    end = se[1]

    #print('Start: {}    End: {}'.format(start, end))

    

    return(get_millis(start, end)['in_range'])


#strMotAudioTrd()
strNoiseAudioTrd()
while 1:
	try:
		x=str(ser.readline(), 'utf-8').split(',')
		print(x)
		key_ = x[-1].replace('\r\n', '')

		if(key_ == 'mot'):
			mot = x[0]
			if mot != '':
				#print("Motion Detected", mot)

				strMotAudioTrd()
				strTrd(base_url + 'motion/' + mot)
				#tr = urllib.request.urlopen(base_url + 'motion/' + mot)
		elif(key_ == 'fp'):
			print('.............................................')
			if(x[0] == '2'):
				print('Sending Email')
				es()
			if(x[0] == '1'):
				print('unlock')
				print('finger print access', check_fp_access())
				if check_fp_access():
					plc.on()
					time.sleep(3)
					plc.off()
				else:
					print('Finger Print Access denied. Time out of index.')
			print('.............................................')
			
		elif(key_ == 'pwr'):
			pwr = x[0]
			if pwr != '':
				#print('Power: ', pwr)
				strTrd(base_url + 'pwr/' + pwr)
				#tr = urllib.request.urlopen(base_url + 'pwr/' + pwr)
		elif(key_ == 'lek'):
			lek = x[0]
			if lek != '':
				#print("Water Leakage", lek)
				strTrd(base_url + 'leak/' + lek)
				#tr = urllib.request.urlopen(base_url + 'leak/' + lek)
		elif(key_ == 'gls'):
			gls = x[0]
			if gls != '':
				#print("Glass Break", gls)
				strTrd(base_url + 'windows/' + gls)
				#tr = urllib.request.urlopen(base_url + 'windows/' + gls)
		elif(key_ == 'cli'):
			temp = x[0]
			hum = x[1]
			coTwo = x[2]
			if (temp != '') and (hum != '') and (coTwo != 0):
				print(temp, hum, coTwo)
				strTrd(base_url + 'temp/' + temp)
				#tr = urllib.request.urlopen(base_url + 'temp/' + temp)
				#print('Temp Resp', tr)
				strTrd(base_url + 'humidity/' + hum)
				#hr = urllib.request.urlopen(base_url + 'humidity/' + hum)
				#print("Hum Resp", hum)
				strTrd(base_url + 'carbon_dioxide/' + coTwo)
				#cr = urllib.request.urlopen(base_url + 'carbon_dioxide/' + coTwo)
				#print('Co2 Resp', cr)
		else:
			pass

	except:
		pass
		