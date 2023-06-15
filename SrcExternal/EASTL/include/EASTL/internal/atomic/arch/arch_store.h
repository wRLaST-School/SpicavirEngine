/////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_ATOMIC_INTERNAL_ARCH_STORE_H
#define EASTL_ATOMIC_INTERNAL_ARCH_STORE_H

#if defined(EA_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif


/////////////////////////////////////////////////////////////////////////////////
//
// void EASTL_ARCH_ATOMIC_STORE_*_N(type, type * ptr, type val)
//
#if defined(EASTL_ARCH_ATOMIC_STORE_RELAXED_8)
	#define EASTL_ARCH_ATOMIC_STORE_RELAXED_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_RELAXED_8_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_STORE_RELEASE_8)
	#define EASTL_ARCH_ATOMIC_STORE_RELEASE_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_RELEASE_8_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_STORE_SEQ_CST_8)
	#define EASTL_ARCH_ATOMIC_STORE_SEQ_CST_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_SEQ_CST_8_AVAILABLE 0
#endif


#if defined(EASTL_ARCH_ATOMIC_STORE_RELAXED_16)
	#define EASTL_ARCH_ATOMIC_STORE_RELAXED_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_RELAXED_16_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_STORE_RELEASE_16)
	#define EASTL_ARCH_ATOMIC_STORE_RELEASE_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_RELEASE_16_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_STORE_SEQ_CST_16)
	#define EASTL_ARCH_ATOMIC_STORE_SEQ_CST_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_SEQ_CST_16_AVAILABLE 0
#endif


#if defined(EASTL_ARCH_ATOMIC_STORE_RELAXED_32)
	#define EASTL_ARCH_ATOMIC_STORE_RELAXED_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_RELAXED_32_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_STORE_RELEASE_32)
	#define EASTL_ARCH_ATOMIC_STORE_RELEASE_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_RELEASE_32_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_STORE_SEQ_CST_32)
	#define EASTL_ARCH_ATOMIC_STORE_SEQ_CST_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_SEQ_CST_32_AVAILABLE 0
#endif


#if defined(EASTL_ARCH_ATOMIC_STORE_RELAXED_64)
	#define EASTL_ARCH_ATOMIC_STORE_RELAXED_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_RELAXED_64_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_STORE_RELEASE_64)
	#define EASTL_ARCH_ATOMIC_STORE_RELEASE_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_RELEASE_64_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_STORE_SEQ_CST_64)
	#define EASTL_ARCH_ATOMIC_STORE_SEQ_CST_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_SEQ_CST_64_AVAILABLE 0
#endif


#if defined(EASTL_ARCH_ATOMIC_STORE_RELAXED_128)
	#define EASTL_ARCH_ATOMIC_STORE_RELAXED_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_RELAXED_128_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_STORE_RELEASE_128)
	#define EASTL_ARCH_ATOMIC_STORE_RELEASE_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_RELEASE_128_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_STORE_SEQ_CST_128)
	#define EASTL_ARCH_ATOMIC_STORE_SEQ_CST_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_STORE_SEQ_CST_128_AVAILABLE 0
#endif


#endif /* EASTL_ATOMIC_INTERNAL_ARCH_STORE_H */
