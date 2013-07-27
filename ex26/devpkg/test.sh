set -e

rm -rf /usr/local/.devpkg

# do a setup
./devpkg -S

# regular tar.gz
./devpkg -I http://apache.ip-connect.vn.ua//apr/apr-1.4.8.tar.gz

# regular tar.bz2
./devpkg -I ftp://ftp.ro.debian.org/pub/FreeBSD/ports/distfiles/mongrel2-1.7.5.tar.bz2

# regular .git
./devpkg -I https://github.com/zedshaw/srpmin.git

# manually fetch/build if already installed
./devpkg -F https://github.com/zedshaw/srpmin.git

./devpkg -B https://github.com/zedshaw/srpmin.git

# manually fetch/build it not installed
./devpkg -F http://apache.ip-connect.vn.ua//apr/apr-util-1.5.2.tar.gz

./devpkg -B http://apache.ip-connect.vn.ua//apr/apr-util-1.5.2.tar.gz -c "--with-apr=/usr/local/apr"

# idempotent build test
./devpkg -I http://apache.ip-connect.vn.ua//apr/apr-util-1.5.2.tar.gz -c "--with-apr=/usr/local/apr"


# just run the depends, nothing should build
./devpkg -I ./DEPENDS

# list out what's installed
./devpkg -L
