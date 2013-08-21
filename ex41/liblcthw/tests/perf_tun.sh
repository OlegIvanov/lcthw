echo "Getting callgrind and cachegrind out files:"

for i in tests/*_tests
do
	if test -f $i
	then
		valgrind --tool=callgrind --callgrind-out-file=callgrind.out.$(basename $i) ./$i 2>> perf_tun.log
		valgrind --tool=cachegrind --cachegrind-out-file=cachegrind.out.$(basename $i) ./$i 2>> perf_tun.log
	fi
done

echo ""
