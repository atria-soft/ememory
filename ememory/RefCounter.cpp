/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ememory/RefCounter.hpp>
#include <ememory/debug.hpp>

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
		EMEMORY_ERROR("request release a refcounted One more time than needed !! " << m_refCount);
		m_refCount = 0;
	}
}

int ememory::RefCounter::getRefCount() const {
	return m_refCount;
}