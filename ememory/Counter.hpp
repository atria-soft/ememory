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
#include <cstdint>

namespace ememory {
	/**
	 * @brief Couter is an important part of the SharedPtr/WeakPtr implementation. This use a simple refcounting method dut thread-safe
	 */
	class Counter {
		public:
			/**
			 * @brief Remove case of the counter. When decrese the shared our weak counter, it is the upper class that need to remove the counter of the data pointer
			 */
			enum class remove {
				none, //!< Noting to remove
				data, //!< Remove only the data
				counter, //!< Remove only the Counter (data already removed)
				all //!< Remove Data and Counter
			};
		private:
			int64_t m_counterShared; //!< Count of the active SharedPtr
			int64_t m_counterWeak; //!< Count of the active WeakPtr
			mutable std::mutex m_mutex; //!< local counter mutex to prevent the thread concurent removing
		public:
			/**
			 * @brief Contructor
			 * @param[in] _fromWeak Counter is created from a WeakPtr instead of a SharedPtr
			 */
			Counter(bool _fromWeak=false);
			/**
			 * @brief Destrunctor (Not virtual !!)
			 */
			~Counter();
			/**
			 * @brief Increment the shared counter (one more shared user)
			 * @param[in] _fromWeak At true if the increment came from a WeakPtr instead of a SharedPtr
			 * @return Number of current SharedPtr that has been curently access at this data. Return 0 if the increment is not possible (object has no more SharedPtr)
			 */
			int64_t incrementShared(bool _fromWeak=false);
			/**
			 * @brief Decrese of 1 the shared counter
			 * @return The action to do on the pointer or the counter
			 */
			ememory::Counter::remove decrementShared();
			/**
			 * @brief Increment the weak counter (one more weak user)
			 * @return Number of current WeakPtr that has been curently access at this data.
			 */
			int64_t incrementWeak();
			/**
			 * @brief Decrese of 1 the weak counter
			 * @return The action to do on the pointer or the counter
			 */
			ememory::Counter::remove decrementWeak();
			/**
			 * @brief Get the number of weak counter
			 * @return Number of WeakPtr connected.
			 */
			int64_t getCountWeak() const;
			/**
			 * @brief Get the number of shared counter
			 * @return Number of SharedPtr connected.
			 */
			int64_t getCountShared() const;
			/**
			 * @brief Get the number of weak+shared counter
			 * @return Number of WeakPtr and SharedPtr connected.
			 */
			int64_t getCount() const;
	};
}

