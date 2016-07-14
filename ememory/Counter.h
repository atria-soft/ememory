/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <mutex>
// define type : uintXX_t and intXX_t
#define __STDC_LIMIT_MACROS
// note in android include the macro of min max are overwitten
#include <stdint.h>

namespace ememory {
	class Counter {
		public:
			enum class remove {
				none,
				data,
				counter,
				all
			};
		private:
			int64_t m_counterShared;
			int64_t m_counterWeak;
			mutable std::mutex m_mutex;
		public:
			Counter(bool _fromWeak=false);
			~Counter();
			int64_t incrementShared(bool _fromWeak=false);
			ememory::Counter::remove decrementShared();
			int64_t incrementWeak();
			ememory::Counter::remove decrementWeak();
			int64_t getCountWeak() const;
			int64_t getCountShared() const;
			int64_t getCount() const;
	};
}

