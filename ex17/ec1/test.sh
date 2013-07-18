set -e
printf "\033c"

echo "./ex17 db.dat c 100 200\n"
./ex17 db.dat c 100 200

echo "./ex17 db.dat s 0 25 \"Oleg Ivanov\" iosmsft@outlook.com\n"
./ex17 db.dat s 0 25 "Oleg Ivanov" iosmsft@outlook.com
echo "\n"

echo "./ex17 db.dat l\n"
./ex17 db.dat l
echo "\n"

echo "./ex17 db.dat s 10 80 \"Valentin Valentinov\" valik@valik.com\n"
./ex17 db.dat s 10 80 "Valentin Valentinov" valik@valik.com
echo "\n"

echo "./ex17 db.dat l\n"
./ex17 db.dat l
echo "\n"

echo "./ex17 db.dat s 99 1 \"Dmitry Petrov\" dp@gmail.com\n"
./ex17 db.dat s 99 1 "Dmitry Petrov" dp@gmail.com
echo "\n"

echo "./ex17 db.dat l\n"
./ex17 db.dat l
echo "\n"

echo "./ex17 db.dat f id=10\n"
./ex17 db.dat f id=0
echo "\n"

echo "./ex17 db.dat f id=99\n"
./ex17 db.dat f id=99
echo "\n"

echo "./ex17 db.dat s 21 13 \"Oleg Ivanov\" dp@gmail.com\n"
./ex17 db.dat s 21 13 "Oleg Ivanov" dp@gmail.com
echo "\n"

echo "./ex17 db.dat l\n"
./ex17 db.dat l
echo "\n"

echo "./ex17 db.dat f \"name=Oleg Ivanov\"\n"
./ex17 db.dat f "name=Oleg Ivanov"
echo "\n"

echo "./ex17 db.dat f \"name=Oleg Ivanov\" age=25\n"
./ex17 db.dat f "name=Oleg Ivanov" age=25
echo "\n"

echo "./ex17 db.dat s 39 13 \"Oleg Ivanov\" x@y.com\n"
./ex17 db.dat s 39 13 "Oleg Ivanov" x@y.com
echo "\n"

echo "./ex17 db.dat l\n"
./ex17 db.dat l
echo "\n"

echo "./ex17 db.dat f \"name=Oleg Ivanov\" email=x@y.com\n"
./ex17 db.dat f "name=Oleg Ivanov" email=x@y.com
echo "\n"

echo "./ex17 db.dat d 0\n"
./ex17 db.dat d 0

echo "./ex17 db.dat d 10\n"
./ex17 db.dat d 10

echo "./ex17 db.dat d 99\n"
./ex17 db.dat d 99

echo "./ex17 db.dat l\n"
./ex17 db.dat l
echo "\n"

echo "./ex17 db.dat g 39\n"
./ex17 db.dat g 39
echo "\n"

echo "./ex17 db.dat g 0\n"
./ex17 db.dat g 0
echo "\n"
