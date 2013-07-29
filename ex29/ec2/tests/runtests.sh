echo "Running unit tests:"

for i in tests/*_tests
do
	if test -f $i
	then
		if $VALGRIND ./$i /usr/local/lib/libex29.so uppercase "hello there" 2>> tests/tests.log
		then
			echo $i PASS
		else
			echo "ERROR in test $i: here's tests/tests.log"
			echo "------"
			tail tests/tests.log
			exit 1
		fi
		if $VALGRIND ./$i /usr/local/lib/libex29.so lowercase "hello there" 2>> tests/tests.log
		then
			echo $i PASS
		else
			echo "ERROR in test $i: here's tests/tests.log"
			echo "------"
			tail tests/tests.log
			exit 1
		fi
		if $VALGRIND ./$i /usr/local/lib/libex29.so lowercase "hello there" 2>> tests/tests.log
		then
			echo $i PASS
		else
			echo "ERROR in test $i: here's tests/tests.log"
			echo "------"
			tail tests/tests.log
			exit 1
		fi
	fi
done

echo ""
