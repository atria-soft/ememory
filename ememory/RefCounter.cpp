/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ememory/RefCounter.hpp>
#include <ememory/debug.hpp>
#include <etk/typeInfo.hpp>

ETK_DECLARE_TYPE(ememory::RefCounter);

#ifdef DEBUG
	static int32_t& getLocalDebugCounter() {
		static int32_t g_uid = 0;
		return g_uid;
	}
	void ememory::resetDebugRefCounter() {
		getLocalDebugCounter() = 0;
	}
#endif

ememory::RefCounter::RefCounter() {
	#ifdef DEBUG
		m_uid = getLocalDebugCounter()++;
	#endif
}

ememory::RefCounter::~RefCounter() {
	if (m_refCount != 0) {
		EMEMORY_ERROR("delete a RefCounted element that is keep by somewhere !! " << m_refCount);
	}
}

void ememory::RefCounter::refKeep() {
	m_refCount++;
}

void ememory::RefCounter::refRelease() {
	int refCount = --m_refCount;
	if (refCount == 0) {
		// No more element ==> remove it.
		this->~RefCounter();
		ETK_DELETE(ememory::RefCounter, this);
		// NOTE: Do nothing more than this ==> it will not work
		return;
	}
	if (refCount < 0) {
		#ifdef DEBUG
			EMEMORY_ERROR("request release a refcounted One more time than needed !! " << m_refCount << " uid=" << m_uid);
		#else
			EMEMORY_ERROR("request release a refcounted One more time than needed !! " << m_refCount);
		#endif
		m_refCount = 0;
	}
}

int ememory::RefCounter::getRefCount() const {
	return m_refCount;
}


uint64_t ememory::RefCounter::getRawPointer() const {
	#ifdef DEBUG
		return m_uid;
	#endif
	return uint64_t(this);
}
