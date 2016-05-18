#include "rwlock.h"

unsigned int RWLock::readers(unsigned int rw)
{
	return rw >> 1;
}

unsigned int RWLock::writing(unsigned int rw)
{
	return rw & 1;
}

unsigned int RWLock::mkrw(unsigned int rds, unsigned int wrt)
{
	return (rds << 1) | wrt;
}

void RWLock::lock_read()
{
	// repeatedly try to add one more reader, without a write lock
	unsigned int old_rw = mkrw(readers(m_rw), 0);
	unsigned int new_rw = mkrw(readers(old_rw) + 1, 0);
	while (!m_rw.compare_exchange_weak(old_rw, new_rw))
	{
		old_rw = mkrw(readers(old_rw), 0);
		new_rw = mkrw(readers(old_rw) + 1, 0);
	}
}

void RWLock::unlock_read()
{
	// repeatedly try to remove a reader (ignoring write lock)
	unsigned int old_rw = m_rw;
	unsigned int new_rw = mkrw(readers(old_rw) - 1, writing(old_rw));
	while (!m_rw.compare_exchange_weak(old_rw, new_rw))
	{
		new_rw = mkrw(readers(old_rw) - 1, writing(old_rw));
	}
}

void RWLock::lock_write()
{
	// repeatedly try to set writing from false to true (ignoring readers)
	unsigned int old_rw = mkrw(readers(m_rw), 0);
	unsigned int new_rw = mkrw(readers(old_rw), 1);
	while (!m_rw.compare_exchange_weak(old_rw, new_rw))
	{
		old_rw = mkrw(readers(old_rw), 0);
		new_rw = mkrw(readers(old_rw), 1);
	}

	// wait for readers to leave
	while (readers(m_rw) > 0);
}

void RWLock::unlock_write()
{
	m_rw = 0;
}

void RWLock::lock_upgrade()
{
	// repeatedly try to set writing from false to true and remove a reader
	unsigned int old_rw = mkrw(readers(m_rw), 0);
	unsigned int new_rw = mkrw(readers(old_rw) - 1, 1);
	while (!m_rw.compare_exchange_weak(old_rw, new_rw))
	{
		old_rw = mkrw(readers(old_rw), 0);
		new_rw = mkrw(readers(old_rw) - 1, 1);
	}

	// wait for readers to leave
	while (readers(m_rw) > 0);
}
