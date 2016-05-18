#include <iostream>

#include "std_coarse.h"
#include "std_rwboost.h"
#include "std_rwmax.h"
#include "read_test.h"

int main(int argc, char** argv)
{
	int key_max = 100000;
	int threads = 4;
	int iters = 1000000;
	int prepop = 500000;

	long int diff =
	read_test<STDCoarse>(key_max, threads, iters, prepop) -
	read_test<STDRWBoost>(key_max, threads, iters, prepop);

	std::cout << "Course vs. Boost " << diff << std::endl;

	diff =
	read_test<STDCoarse>(key_max, threads, iters, prepop) -
	read_test<STDRWMax>(key_max, threads, iters, prepop);

	std::cout << "Course vs. Custom " << diff << std::endl;

	diff =
	read_test<STDRWBoost>(key_max, threads, iters, prepop) -
	read_test<STDRWMax>(key_max, threads, iters, prepop);

	std::cout << "Boost vs. Custom " << diff << std::endl;

	return 0;
}
