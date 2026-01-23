c() {
	g++ $1.cpp -o $1 -std=c++20
}
cs() {
	g++ $1.cpp -o $1 -std=c++20 -fsanitize=address,undefined,signed-integer-overflow -ggdb
}
rt() { python3 $3.py >t.in; ./$1 <t.in >t.out; ./$2 <t.in >t.ans; }
st() {
    for i in {1..100}; do
        rt $@
        cmp -s t.out t.ans || { echo WA; return 1; }
    done
    echo AC
}
