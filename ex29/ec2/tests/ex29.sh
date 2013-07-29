echo "try it out with some things that work"
./ex29 ./libex29.so print_a_message "hello there"
./ex29 ./libex29.so uppercase "hello there"
./ex29 ./libex29.so lowercase "HELLO THERE"
./ex29 ./libex29.so fail_on_purpose "i fail"

echo "try to give it bad args"
./ex29 ./libex29.so fail_on_purpose

echo "try calling a function that is not there"
./ex29 ./libex29.so adfasfasdf asdfadff

echo "try loading a .so that is not there"
./ex29 ./libex.so adfasfasdf asdfadfas
