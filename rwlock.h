#ifndef RWLOCK_H
#define RWLOCK_H
#include <atomic>

class RWLock
{
	std::atomic<unsigned int> m_rw {0};
	static unsigned int readers(unsigned int);
	static unsigned int writing(unsigned int);
	static unsigned int mkrw(unsigned int, unsigned int);
public:
	RWLock() {};
	void lock_read();
	void unlock_read();
	void lock_write();
	void unlock_write();
	void lock_upgrade();
};

#endif
