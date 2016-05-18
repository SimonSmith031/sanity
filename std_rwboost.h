#include <unordered_map>
#include <utility>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

class STDRWBoost
{
	typedef boost::shared_lock<boost::shared_mutex> read_lock_t;
	typedef boost::unique_lock<boost::shared_mutex> write_lock_t;
public:
	STDRWBoost()
	{
	}

	bool insert(const std::pair<const int, int>& pair)
	{
		write_lock_t lock(m_mutex);
		return m_table.insert(pair).second;
	}

	size_t erase(const int& key)
	{
		write_lock_t lock(m_mutex);
		return m_table.erase(key);
	}

	size_t count(const int& key) const
	{
		read_lock_t lock(m_mutex);
		return m_table.count(key);
	}

private:
	std::unordered_map<int, int> m_table;
	mutable boost::shared_mutex m_mutex;
};
