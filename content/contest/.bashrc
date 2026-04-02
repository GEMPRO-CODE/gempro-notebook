c() { g++ $1.cpp -o $1 -std=c++20; }
cs() {
	g++ $1.cpp -o $1 -std=c++20 \
		-fsanitize=address,undefined \
		-fsanitize=signed-integer-overflow -ggdb
}
gen() { 
	local N=${1:-10}
	for i in $(seq 1 $N); do
		python3 gen.py > $i.in
		if [[ -f brute ]]; then
			./brute < $i.in > $i.ans
		fi
	done
}
rr() {
	for i in $(ls *.in); do
		local f=$(basename $i .in)
		./$1 < $f.in > $f.out
		if [[ -f $f.ans ]]; then
			if ! cmp -s $f.out $f.ans; then
				echo WA
				return 1
			fi
		fi
    done
    echo AC
}
