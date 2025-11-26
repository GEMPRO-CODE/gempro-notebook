c() {
	g++ "$1.cpp" -o "$1" -std=c++20 -fsanitize=address,undefined,signed-integer-overflow -ggdb
}
cs() {
	g++ "$1.cpp" -o "$1" -std=c++20
}
