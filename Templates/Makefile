



all: functional_test combination_test permutation_test multipermutation_test

functional_test: functional_test.cpp
	g++ $(CFLAGS) -o $@ $?

combination_test: combination_test.cpp
	g++ $(CFLAGS) -o $@ $?

permutation_test: permutation_test.cpp
	g++ $(CFLAGS) -o $@ $?


multipermutation_test: multipermutation_test.cpp
	g++ $(CFLAGS) -o $@ $?

clean: 
	rm functional_test combination_test permutation_test multipermutation_test

