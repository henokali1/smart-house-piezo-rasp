# -*- coding: UTF-8 -*-

import smtplib

gmail_user = 't.project3333@gmail.com'  
gmail_password = 'projectpsw'

sent_from = gmail_user  
to = ['t.project3333@gmail.com']  
subject = 'House intruder Detected!!!!'  

map_url = """https://goo.gl/maps/6f8Zj5P8cr42"""
body = 'An intruder has been detected at house number 132/432 which is located at ' + map_url + '.\n' + 'Please take the necessary action as soon as possible.'

email_text = """
From: %s  
To: %s  
Subject: %s

%s
""" % (sent_from, ", ".join(to), subject, body)

def send_g_email():
	try:  
	    server = smtplib.SMTP_SSL('smtp.gmail.com', 465)
	    server.ehlo()
	    server.login(gmail_user, gmail_password)
	    server.sendmail(sent_from, to, email_text)
	    server.close()

	    print('Email sent!')
	except:  
	    print('Something went wrong...')