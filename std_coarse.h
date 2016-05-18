#include <mutex>
#include <unordered_map>
#include <utility>

class STDCoarse
{
public:
	STDCoarse()
	{
	}

	bool insert(const std::pair<const int, int>& pair)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_table.insert(pair).second;
	}

	size_t erase(const int& key)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_table.erase(key);
	}

	size_t count(const int& key) const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_table.count(key);
	}

private:
	std::unordered_map<int, int> m_table;
	mutable std::mutex m_mutex;
};
