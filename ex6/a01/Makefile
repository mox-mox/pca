.PHONY: parallel
parallel:
	g++ -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -fopenmp -std=c++11 -DPARALLEL a3.cpp -o parallel

.PHONY: sequential
sequential:
	g++ -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wno-unknown-pragmas -std=c++11 a3.cpp -o sequential
