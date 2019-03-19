import urllib.request
import time
import random

from datetime import datetime





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

while 1:
    f = urllib.request.urlopen("http://46.101.144.34:5000/time")
    r = f.read().decode("utf-8") 

    se = r.split('-')

    start = se[0]
    end = se[1]

    #print('Start: {}    End: {}'.format(start, end))

    

    #print(get_millis(start, end)['in_range'])
    
    print(check_fp_access())
    time.sleep(1)
