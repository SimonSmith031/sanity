#ifndef READ_TEST_H
#define READ_TEST_H

#include <atomic>
#include <chrono>
#include <random>
#include <thread>
#include <vector>

// return a random size_t >= min and <= max if provided
inline unsigned int random_uint(unsigned int min, unsigned int max = std::numeric_limits<unsigned int>::max())
{
	static thread_local std::random_device random_device;
	static thread_local std::mt19937 generator(random_device());

	std::uniform_int_distribution<unsigned int> dist(min, max);
	return dist(generator);
}

template<class T>
std::chrono::high_resolution_clock::rep read_test(int key_max, int num_threads, int iters, int pop)
{
	std::atomic<int> barrier_1, barrier_2, barrier_3;

	std::chrono::high_resolution_clock::time_point start_time, end_time;

	barrier_1 = 0; barrier_2 = 0; barrier_3 = 0;

	T map;

	auto task = [&](int id)
	{
		if (id == 0)
		{
			// pre-populate the map
			for (int i = 0; i < pop; ++i)
			{
				int val = random_uint(0, key_max);
				map.insert(std::make_pair(val, -val));
			}
		}

		barrier_1++;
		while (barrier_1 < num_threads) { }
		if (id == 0) start_time = std::chrono::high_resolution_clock::now();
		barrier_2++;
		while (barrier_2 < num_threads) { }

		for (int i = 0; i < iters; ++i) map.count(random_uint(0, key_max));

		barrier_3++;
		while (barrier_3 < num_threads) {}
		if (id == 0) end_time = std::chrono::high_resolution_clock::now();
	};

	std::vector<std::thread> threads;

	for (int i = 0; i < num_threads; ++i) threads.push_back(std::thread(task, i));
	for (int i = 0; i < num_threads; ++i) threads[i].join();

	return (end_time - start_time).count();
}

#endif
