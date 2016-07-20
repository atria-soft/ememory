/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <elog/log.h>
#include <assert.h>

namespace ememory {
	int32_t getLogId();
};

#define EMEMORY_BASIC(info,data) ELOG_BASE(ememory::getLogId(),info,data)

#define EMEMORY_PRINT(data)         EMEMORY_BASIC(-1, data)
#define EMEMORY_CRITICAL(data)      EMEMORY_BASIC(1, data)
#define EMEMORY_ERROR(data)         EMEMORY_BASIC(2, data)
#define EMEMORY_WARNING(data)       EMEMORY_BASIC(3, data)
#ifdef DEBUG
	#define EMEMORY_INFO(data)      EMEMORY_BASIC(4, data)
	#define EMEMORY_DEBUG(data)     EMEMORY_BASIC(5, data)
	#define EMEMORY_VERBOSE(data)   EMEMORY_BASIC(6, data)
	#define EMEMORY_TODO(data)      EMEMORY_BASIC(4, "TODO : " << data)
#else
	#define EMEMORY_INFO(data)      do { } while(false)
	#define EMEMORY_DEBUG(data)     do { } while(false)
	#define EMEMORY_VERBOSE(data)   do { } while(false)
	#define EMEMORY_TODO(data)      do { } while(false)
#endif

#if 0
	#define EMEMORY_DBG(data)     EMEMORY_BASIC(5, data)
#else
	#define EMEMORY_DBG(data)     do { } while(false)
#endif

#define EMEMORY_HIDDEN(data)          do { } while(false)

#define EMEMORY_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			EMEMORY_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)


