/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ememory/Counter.h>
#include <ememory/debug.h>

ememory::Counter::Counter(bool _fromWeak):
  m_counterShared(1),
  m_counterWeak(0),
  m_mutex() {
	EMEMORY_DBG("new counter(" << _fromWeak << ")");
	if (_fromWeak == true) {
		m_counterShared = 0;
		m_counterWeak = 1;
	}
	EMEMORY_DBG("    ==> w:" << m_counterWeak << " s:" << m_counterShared);
}

ememory::Counter::~Counter() {
	EMEMORY_DBG("delete counter");
}

int64_t ememory::Counter::incrementShared(bool _fromWeak) {
	int64_t out;
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		EMEMORY_DBG("shared++ (start)    ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
		if (m_counterShared != 0 || _fromWeak == false) {
			m_counterShared++;
		}
		out = m_counterShared;
		EMEMORY_DBG("shared++ (stop)     ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
	}
	return out;
}

ememory::Counter::remove ememory::Counter::decrementShared() {
	std::unique_lock<std::mutex> lock(m_mutex);
	EMEMORY_DBG("shared-- (start)    ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
	if (m_counterShared != 0) {
		m_counterShared--;
	} else {
		EMEMORY_ERROR("try to decrement a SharedPtr with no more element in RefCount");
		return ememory::Counter::remove::none;
	}
	if (m_counterShared == 0) {
		if (m_counterWeak == 0) {
			EMEMORY_DBG("shared-- (stop)     ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
			return ememory::Counter::remove::all;
		} else {
			EMEMORY_DBG("shared-- (stop)     ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
			return ememory::Counter::remove::data;
		}
	}
	EMEMORY_DBG("shared-- (stop)     ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
	return ememory::Counter::remove::none;
}

int64_t ememory::Counter::incrementWeak() {
	int64_t out;
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		EMEMORY_DBG("weak++ (start)    ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
		m_counterWeak++;
		out = m_counterWeak;
		EMEMORY_DBG("weak++ (stop)     ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
	}
	return out;
}

ememory::Counter::remove ememory::Counter::decrementWeak() {
	std::unique_lock<std::mutex> lock(m_mutex);
	EMEMORY_DBG("weak-- (stop)    ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
	if (m_counterWeak != 0) {
		m_counterWeak--;
	} else {
		EMEMORY_ERROR("try to decrement a WeakPtr with no more element in RefCount");
		return ememory::Counter::remove::none;
	}
	if (    m_counterShared == 0
	     && m_counterWeak == 0) {
		EMEMORY_DBG("weak-- (stop)     ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
		return ememory::Counter::remove::counter;
	}
	EMEMORY_DBG("weak-- (stop)     ==> w:" << m_counterWeak << " s:" << m_counterShared << "    " << int64_t(this));
	return ememory::Counter::remove::none;
}

int64_t ememory::Counter::getCountWeak() const {
	int64_t out;
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		out = m_counterWeak;
	}
	return out;
}

int64_t ememory::Counter::getCountShared() const {
	int64_t out;
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		out = m_counterShared;
	}
	return out;
}

int64_t ememory::Counter::getCount() const {
	int64_t out;
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		out = m_counterWeak + m_counterShared;
	}
	return out;
}

