TESTEXEC="tests/ex29_tests"
LIBRARYPATH="/usr/local/lib/libex29.so"

echo "try it out with some things that work"
$TESTEXEC $LIBRARYPATH print_a_message "hello there"
$TESTEXEC $LIBRARYPATH uppercase "hello there"
$TESTEXEC $LIBRARYPATH lowercase "HELLO THERE"
$TESTEXEC $LIBRARYPATH fail_on_purpose "i fail"

echo "try to give it bad args"
$TESTEXEC $LIBRARYPATH fail_on_purpose

echo "try calling a function that is not there"
$TESTEXEC $LIBRARYPATH adfasfasdf asdfadff

echo "try loading a .so that is not there"
$TESTEXEC /usr/local/lib/libex.so adfasfasdf asdfadfas
