#include <unordered_map>
#include <utility>

#include "rwlock.h"

class STDRWMax
{
public:
	STDRWMax()
	{
	}

	bool insert(const std::pair<const int, int>& pair)
	{
		lock.lock_write();
		bool r = m_table.insert(pair).second;
		lock.unlock_write();
		return r;
	}

	size_t erase(const int& key)
	{
		lock.lock_write();
		size_t r = m_table.erase(key);
		lock.unlock_write();
		return r;
	}

	size_t count(const int& key) const
	{
		lock.lock_read();
		size_t r = m_table.count(key);
		lock.unlock_read();
		return r;
	}

private:
	std::unordered_map<int, int> m_table;
	mutable RWLock lock;
};
