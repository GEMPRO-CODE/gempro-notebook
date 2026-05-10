c() { g++ $1.cpp -o $1 -std=c++20; }
cs() {
	g++ $1.cpp -o $1 -std=c++20 \
		-fsanitize=address,undefined \
		-fsanitize=signed-integer-overflow -ggdb
}
gen() { 
	local N=${1:-10}
	for i in $(seq 1 $N); do
		python3 gen.py $i > $i.in
	done
}
rr() {
	local ext=${2:-out}
	for i in $(ls *.in); do
		local f=$(basename $i .in)
		./$1 < $f.in > $f.$ext
    done
}
chk() {
	local a=${1:-ans}
	local b=${2:-out}
	for i in $(ls *.$a); do
		local f=$(basename $i .$a)
		echo -n "$f.$a - $f.$b > "
		if cmp -s $f.$a $f.$b; then
			echo OK
		else
			echo NO
		fi
	done
}
