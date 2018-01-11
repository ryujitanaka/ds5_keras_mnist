/**
 * Copyright (C) Arm Limited 2017. All rights reserved.
 *
 * Use, modification and redistribution of this file is subject to your possession of a valid
 * DS-5 end user licence agreement and your compliance with all applicable terms and
 * conditions of such licence agreement
 */

#ifndef __INCLUDE_BARMAN_H__
#define __INCLUDE_BARMAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */

/**
 * @defgroup    bm_config   Config settings
 * @{ */

/**
 * @def     BM_CONFIG_MAX_CORES
 * @brief   The maximum number of processor elements supported
 */
#ifndef BM_CONFIG_MAX_CORES
#define BM_CONFIG_MAX_CORES                 1
#endif

/**
 * @def     BM_CONFIG_NUM_PMU_TYPES
 * @brief   The number of processor types supported
 */
#ifndef BM_CONFIG_NUM_PMU_TYPES
#define BM_CONFIG_NUM_PMU_TYPES             1
#endif

/**
 * @def     BM_CONFIG_ENABLE_BUILTIN_MEMFUNCS
 * @brief   When set true, will enable the use of __builtin_memset, __builtin_memcpy etc.
 */
#ifndef BM_CONFIG_ENABLE_BUILTIN_MEMFUNCS
#define BM_CONFIG_ENABLE_BUILTIN_MEMFUNCS   1
#endif

/**
 * @def     BM_CONFIG_USER_SUPPLIED_PMU_DRIVER
 * @brief   The PMU API parameters are defined externally to barman-api.c
 * @details When enabled the user must provide the following defined functions and macros
 *
 *          `#define BM_MAX_PMU_COUNTERS`
 *          - Defines the maximum number of PMU counters the hardware supports
 *          `#define BM_PMU_INVALID_COUNTER_VALUE`
 *          - Defines the value the PMU driver will return if the counter was not read
 *          `#define BM_PMU_HAS_FIXED_CYCLE_COUNTER`
 *          - Must be defined as a boolean value, indicating whether or not the PMU has a fixed cycle counter separate
 *            from the other counters that is always enabled
 *          `#define BM_PMU_CYCLE_COUNTER_ID`
 *          - If BM_PMU_HAS_FIXED_CYCLE_COUNTER is defined true, this must be defined with the counter number for the
 *            fixed cycle counter
 *          `#define BM_PMU_CYCLE_COUNTER_TYPE`
 *          - The event type as defined in the `events.xml` for the fixed cycle counter
 *          `void barman_ext_pmu_init(bm_uint32 n_event_types, const bm_uint32 * event_types)`
 *          - The PMU initialization function for the current core
 *          `void barman_ext_pmu_start(void)`
 *          - The PMU start counting function for the current core
 *          `void barman_ext_pmu_stop(void)`
 *          - The PMU stop counting function for the current core
 *          `bm_uint64 barman_pmu_read_counter(bm_uint32 counter)`
 *          - The PMU counter read function
 */
#ifndef BM_CONFIG_USER_SUPPLIED_PMU_DRIVER
#define BM_CONFIG_USER_SUPPLIED_PMU_DRIVER  0
#endif

/** Value to define {@link BM_CONFIG_USE_DATASTORE} as if the user is to supply the datastore functions */
#define BM_CONFIG_USE_DATASTORE_USER_SUPPLIED             0
/** Value to define {@link BM_CONFIG_USE_DATASTORE} as if the linear ram buffer is used as the data store */
#define BM_CONFIG_USE_DATASTORE_LINEAR_RAM_BUFFER         1
/** Value to define {@link BM_CONFIG_USE_DATASTORE} as if the circular ram buffer is used as the data store */
#define BM_CONFIG_USE_DATASTORE_CIRCULAR_RAM_BUFFER       2
/** Value to define {@link BM_CONFIG_USE_DATASTORE} as if the STM interface is used as the data store */
#define BM_CONFIG_USE_DATASTORE_STM                       3
/** Value to define {@link BM_CONFIG_USE_DATASTORE} as if the user is to supply streaming interface functions */
#define BM_CONFIG_USE_DATASTORE_STREAMING_USER_SUPPLIED   4
/** Value to define {@link BM_CONFIG_USE_DATASTORE} as if the ITM interface is used as the data store */
#define BM_CONFIG_USE_DATASTORE_ITM                       5
/** Value to define {@link BM_CONFIG_USE_DATASTORE} as if the ETM interface is used as the data store */
#define BM_CONFIG_USE_DATASTORE_ETM                       6

/**
 * @def     BM_CONFIG_USE_DATASTORE
 * @brief   Specifies the data store to use
 */
#ifndef BM_CONFIG_USE_DATASTORE
#define BM_CONFIG_USE_DATASTORE                     BM_CONFIG_USE_DATASTORE_LINEAR_RAM_BUFFER
#endif

/**
 * @def     BM_CONFIG_ENABLE_LOGGING
 * @brief   When set true will enable logging messages
 */
#ifndef BM_CONFIG_ENABLE_LOGGING
#define BM_CONFIG_ENABLE_LOGGING                    0
#endif

/**
 * @def     BM_CONFIG_ENABLE_DEBUG_LOGGING
 * @brief   When set true (and BM_CONFIG_ENABLE_LOGGING is set true) will enable debug logging messages
 */
#ifndef BM_CONFIG_ENABLE_DEBUG_LOGGING
#define BM_CONFIG_ENABLE_DEBUG_LOGGING              0
#endif

/**
 * @def     BM_CONFIG_MAX_MMAP_LAYOUTS
 * @brief   The maximum number of mmap layout entries that will be stored in the data header. Should be configured
 *          to reflect the number of sections for any process images that should be mapped.
 */
#ifndef BM_CONFIG_MAX_MMAP_LAYOUTS
#define BM_CONFIG_MAX_MMAP_LAYOUTS                  0
#endif

/**
 * @def     BM_CONFIG_MAX_TASK_INFOS
 * @brief   Include information about current task/process/thread when sampling when non zero
 * @details For single threaded applications this may be defined zero to indicate that no task information may be saved.
 *          For multi threaded applications / RTOS this value indicates the maximum number of entries to store in the data header
 *          for describing process/threads/tasks.
 */
#ifndef BM_CONFIG_MAX_TASK_INFOS
#define BM_CONFIG_MAX_TASK_INFOS                    8
#endif

/**
 * @def     BM_CONFIG_MIN_SAMPLE_PERIOD
 * @brief   The minimum period between samples in ns. Any samples more frequent will be ignored.
 * @details This is performed on a per core basis.
 */
#ifndef BM_CONFIG_MIN_SAMPLE_PERIOD
#define BM_CONFIG_MIN_SAMPLE_PERIOD                 0
#endif

/**
 * @def     BM_CONFIG_RECORDS_PER_HEADER_SENT
 * @brief   How often a header should be sent.
 * @details The number of sample records sent between sending the header. Ignored for in memory datastores.
 */
#ifndef BM_CONFIG_RECORDS_PER_HEADER_SENT
#define BM_CONFIG_RECORDS_PER_HEADER_SENT           500
#endif

/**
 * @def     BM_CONFIG_STM_MIN_CHANNEL_NUMBER
 * @brief   The minimum channel number the STM datastore will use
 * @details The STM datastore will use {@link BM_CONFIG_STM_NUMBER_OF_CHANNELS} starting from this one.
 *          NB: If this is overridden it must be updated in `barman.xml` for the import process.
 */
#ifndef BM_CONFIG_STM_MIN_CHANNEL_NUMBER
#define BM_CONFIG_STM_MIN_CHANNEL_NUMBER            0
#endif

/**
 * @def     BM_CONFIG_STM_NUMBER_OF_CHANNELS
 * @brief   The number of channels the STM datastore will use
 * @details To ensure no data loss this should be at least the number of possible simultaneous calls of the
 *          barman API functions.
 *          NB: If this is overridden it must be updated in `barman.xml` for the import process.
 */
#ifndef BM_CONFIG_STM_NUMBER_OF_CHANNELS
#define BM_CONFIG_STM_NUMBER_OF_CHANNELS            BM_CONFIG_MAX_CORES
#endif

/**
 * @def     BM_CONFIG_ITM_MIN_PORT_NUMBER
 * @brief   The minimum port number the ITM datastore will use
 * @details The ITM datastore will use {@link BM_CONFIG_ITM_NUMBER_OF_PORTS} starting from this one.
 *          NB: If this is overridden it must be updated in `barman.xml` for the import process.
 */
#ifndef BM_CONFIG_ITM_MIN_PORT_NUMBER
#define BM_CONFIG_ITM_MIN_PORT_NUMBER               0
#endif

/**
 * @def     BM_CONFIG_ITM_NUMBER_OF_PORTS
 * @brief   The number of ports the ITM datastore will use
 * @details To ensure no data loss this should be at least the number of possible simultaneous calls of the
 *          barman API functions.
 *          NB: If this is overridden it must be updated in `barman.xml` for the import process.
 */
#ifndef BM_CONFIG_ITM_NUMBER_OF_PORTS
#define BM_CONFIG_ITM_NUMBER_OF_PORTS               BM_CONFIG_MAX_CORES
#endif

/**
 * @def     BM_CONFIG_DWT_SAMPLE_PERIOD
 * @brief   Number of cycles per PC sample or cycle overflow event.
 * @details Valid values are 64 * i or 1024 * i where i is between
 *          1 and 16 inclusive. Other values will be rounded down.
 *          NB: If this is overridden it must be updated in `barman.xml` for the import process.
 */
#ifndef BM_CONFIG_DWT_SAMPLE_PERIOD
#define BM_CONFIG_DWT_SAMPLE_PERIOD                 1024
#endif

/**
 * @def     BARMAN_DISABLED
 * @brief   When defined to a non-zero value will disable the barman entry points at compile time.
 * @details Use to conditionally disable calls to barman (e.g. in production code).
 */
#ifndef BARMAN_DISABLED
#define BARMAN_DISABLED                             0
#endif

/** @} */

/** @{ */
#if (!defined(BARMAN_DISABLED)) || (!BARMAN_DISABLED)
#   define  BM_PUBLIC_FUNCTION                              extern BM_NEVER_INLINE
#   define  BM_PUBLIC_FUNCTION_BODY_STATEMENT(statement)    ;
#   define  BM_PUBLIC_FUNCTION_BODY(val)                    ;
#   define  BM_PUBLIC_FUNCTION_BODY_VOID                    ;
#else
#   define  BM_PUBLIC_FUNCTION                              static BM_ALWAYS_INLINE
#   define  BM_PUBLIC_FUNCTION_BODY_STATEMENT(statement)    { statement; }
#   define  BM_PUBLIC_FUNCTION_BODY(val)                    { return (val); }
#   define  BM_PUBLIC_FUNCTION_BODY_VOID                    {}
#endif

/** @} */

/* ------------------------------------------------------------------------- */

/* We define our own to reduce dependency on external headers, and to avoid
 * conflicts with any definitions in other headers where barman headers
 * are included */

/* armclang doesn't always set __GNUC__ when it should */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6000000) && !defined(__GNUC__)
#define __GNUC__ 4
#endif

#if defined(__GNUC__)
#pragma GCC system_header
#endif

/**
 * @defgroup    bm_compiler_detection   Compiler detection macros
 * @{ */

/**
 * @def         BM_COMPILER_IS_ARM
 * @brief       Evaluates to true if the compiler is ARMCC or ARMCLANG
 * @def         BM_COMPILER_IS_ARMCC
 * @brief       Evaluates to true if the compiler is ARMCC
 * @def         BM_COMPILER_IS_ARMCLANG
 * @brief       Evaluates to true if the compiler is ARMCLANG
 * @def         BM_COMPILER_IS_CLANG
 * @brief       Evaluates to true if the compiler is CLANG
 * @def         BM_COMPILER_IS_GNUC
 * @brief       Evaluates to true if the compiler is GCC compatible
 *
 * @def         BM_COMPILER_AT_LEAST_ARM
 * @brief       Evaluates to true if the compiler is ARMCC or ARMCLANG of at least the version specified
 * @def         BM_COMPILER_AT_LEAST_ARMCC
 * @brief       Evaluates to true if the compiler is ARMCC of at least the version specified
 * @def         BM_COMPILER_AT_LEAST_ARMCLANG
 * @brief       Evaluates to true if the compiler is ARMCLANG of at least the version specified
 * @def         BM_COMPILER_AT_LEAST_CLANG
 * @brief       Evaluates to true if the compiler is CLANG of at least the version specified
 * @def         BM_COMPILER_AT_LEAST_GNUC
 * @brief       Evaluates to true if the compiler is compatible with GCC of at least the version specified
 */
/** @{ */
#if defined(__ARMCC_VERSION)
#define BM_COMPILER_ARM_MAKE_VERSION(a, b, c)       (((a) * 1000000) + ((b) * 10000) + (c))
#define BM_COMPILER_ARM_VERSION_NO                  (__ARMCC_VERSION)
#define BM_COMPILER_AT_LEAST_ARM(a, b, c)           (BM_COMPILER_ARM_VERSION_NO >= BM_COMPILER_ARM_MAKE_VERSION(a, b, c))
#define BM_COMPILER_IS_ARM                          (1)
#else
#define BM_COMPILER_AT_LEAST_ARM(a, b, c)           (0)
#define BM_COMPILER_IS_ARM                          (0)
#endif

#if BM_COMPILER_IS_ARM && !BM_COMPILER_AT_LEAST_ARM(6,0,0)
#define BM_COMPILER_AT_LEAST_ARMCC(a, b, c)         BM_COMPILER_AT_LEAST_ARM(a, b, c)
#define BM_COMPILER_ARMCC_VERSION_NO                BM_COMPILER_ARM_VERSION_NO
#define BM_COMPILER_IS_ARMCC                        (1)
#else
#define BM_COMPILER_AT_LEAST_ARMCC(a, b, c)         (0)
#define BM_COMPILER_IS_ARMCC                        (0)
#endif

#if BM_COMPILER_IS_ARM && BM_COMPILER_AT_LEAST_ARM(6,0,0)
#define BM_COMPILER_AT_LEAST_ARMCLANG(a, b, c)      BM_COMPILER_AT_LEAST_ARM(a, b, c)
#define BM_COMPILER_ARMCLANG_VERSION_NO             BM_COMPILER_ARM_VERSION_NO
#define BM_COMPILER_IS_ARMCLANG                     (1)
#else
#define BM_COMPILER_AT_LEAST_ARMCLANG(a, b, c)      (0)
#define BM_COMPILER_IS_ARMCLANG                     (0)
#endif

#if defined(__clang__)
#define BM_COMPILER_CLANG_MAKE_VERSION(a, b, c)     (((a) * 0x10000) | (((b) & 0xff) * 0x100) | ((c) & 0xff))
#define BM_COMPILER_CLANG_VERSION_NO                BM_COMPILER_CLANG_MAKE_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#define BM_COMPILER_AT_LEAST_CLANG(a, b, c)         (BM_COMPILER_CLANG_VERSION_NO >= BM_COMPILER_CLANG_MAKE_VERSION(a, b, c))
#define BM_COMPILER_IS_CLANG                        (1)
#else
#define BM_COMPILER_AT_LEAST_CLANG(a, b, c)         (0)
#define BM_COMPILER_IS_CLANG                        (0)
#endif

#if defined(__GNUC__)
#define BM_COMPILER_GNUC_MAKE_VERSION(a, b, c)      (((a) * 0x10000) | (((b) & 0xff) * 0x100) | ((c) & 0xff))
#define BM_COMPILER_GNUC_VERSION_NO                 BM_COMPILER_GNUC_MAKE_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#define BM_COMPILER_AT_LEAST_GNUC(a, b, c)          (BM_COMPILER_GNUC_VERSION_NO >= BM_COMPILER_GNUC_MAKE_VERSION(a, b, c))
#define BM_COMPILER_IS_GNUC                         (1)
#else
#define BM_COMPILER_AT_LEAST_GNUC(a, b, c)          (0)
#define BM_COMPILER_IS_GNUC                         (0)
#endif

#if !(defined(__ARMCC_VERSION) || defined(__clang__) || defined(__GNUC__))
#pragma message ("WARNING: Compiler is not recognized")
#endif
/** @} */

/** @} */

/**
 * @defgroup    bm_target_detection Target detection macros
 * @{ */

/**
 * @def         BM_ARM_TARGET_ARCH
 * @brief       Defines the arm architecture level
 */
#if defined(__aarch64__)
# if defined(__ARM_ARCH)
#   define BM_ARM_TARGET_ARCH  __ARM_ARCH
# else
#   define BM_ARM_TARGET_ARCH  8
# endif
#elif defined(__arm__)
# if defined(__ARM_ARCH)
#   define BM_ARM_TARGET_ARCH  __ARM_ARCH
# elif defined(__TARGET_ARCH_ARM)
#   define BM_ARM_TARGET_ARCH  __TARGET_ARCH_ARM
# elif defined(__ARM_ARCH_7__)
#   define BM_ARM_TARGET_ARCH  7
# elif defined(__ARM_ARCH_6__)
#   define BM_ARM_TARGET_ARCH  6
# else
#   define BM_ARM_TARGET_ARCH  0
# endif
#else
# define BM_ARM_TARGET_ARCH  0
#endif

/**
 * @def         BM_ARM_ARCH_PROFILE
 * @brief       Defines the arm architecture profile
 */
#if defined(__ARM_ARCH_PROFILE)
#  define BM_ARM_ARCH_PROFILE __ARM_ARCH_PROFILE
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_8A__) || defined(__TARGET_ARCH_7_A)
#  define BM_ARM_ARCH_PROFILE 'A'
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_8R__) || defined(__TARGET_ARCH_7_R)
#  define BM_ARM_ARCH_PROFILE 'R'
#elif defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_BASE__) || defined(__ARM_ARCH_8M_MAIN__) || defined(__TARGET_ARCH_6_M) || defined(__TARGET_ARCH_7_M) || defined(__TARGET_ARCH_7E_M)
#  define BM_ARM_ARCH_PROFILE 'M'
#endif

/**
 * @def         BM_ARM_ARCH_PROFILE_IS_AR
 * @brief       Defines whether the arm architecture profile includes A and R subset
 */
#if BM_ARM_ARCH_PROFILE == 'A' || BM_ARM_ARCH_PROFILE == 'R' || BM_ARM_ARCH_PROFILE == 'S'
#  define BM_ARM_ARCH_PROFILE_IS_AR 1
#else
#  define BM_ARM_ARCH_PROFILE_IS_AR 0
#endif

/**
 * @def         BM_ARM_64BIT_STATE
 * @brief       Defines whether targeting AArch64
 *
 * @def         BM_ARM_32BIT_STATE
 * @brief       Defines whether targeting AArch32
 */
#if defined(__ARM_64BIT_STATE) || defined(__aarch64__)
#define         BM_ARM_64BIT_STATE 1
#define         BM_ARM_32BIT_STATE 0
#elif defined(__ARM_32BIT_STATE) || defined(__arm__)
#define         BM_ARM_64BIT_STATE 0
#define         BM_ARM_32BIT_STATE 1
#else
#define         BM_ARM_64BIT_STATE 0
#define         BM_ARM_32BIT_STATE 0
#endif

/** Check if the target architecture is ARMv8 */
#define         BM_ARM_TARGET_ARCH_IS_ARMv8()   ((BM_ARM_TARGET_ARCH >= 8) && (BM_ARM_TARGET_ARCH < 900) )
/** Check if the target architecture is ARMv7 */
#define         BM_ARM_TARGET_ARCH_IS_ARMv7()   (BM_ARM_TARGET_ARCH == 7)
/** Check if the target architecture is ARMv6 */
#define         BM_ARM_TARGET_ARCH_IS_ARMv6()   (BM_ARM_TARGET_ARCH == 6)
/** Check if the target architecture is unknown */
#define         BM_ARM_TARGET_ARCH_IS_UNKNOWN() ((BM_ARM_TARGET_ARCH < 6) || (BM_ARM_TARGET_ARCH > 899))

/** @} */

/**
 * @defgroup    bm_integer_types   Basic integer type definitions
 * @{ */
typedef char                bm_bool;    /**< Boolean value */
typedef signed char         bm_int8;    /**< Signed 8-bit value */
typedef unsigned char       bm_uint8;   /**< Unsigned 8-bit value */
typedef signed short        bm_int16;   /**< Signed 16-bit value */
typedef unsigned short      bm_uint16;  /**< Unsigned 16-bit value */
typedef signed int          bm_int32;   /**< Signed 32-bit value */
typedef unsigned int        bm_uint32;  /**< Unsigned 32-bit value */
typedef signed long long    bm_int64;   /**< Signed 64-bit value */
typedef unsigned long long  bm_uint64;  /**< Unsigned 64-bit value */
typedef signed long         bm_intptr;  /**< Signed value of size greater than or equal to a pointer */
typedef unsigned long       bm_uintptr; /**< Unsigned value of size greater than or equal to a pointer */
typedef unsigned long       bm_size_t;  /**< `size_t` type */
/** @} */

/**
 * @defgroup    bm_integer_typed_literals   Basic typed integer literals
 * @{ */
#define BM_INT8(c)    c        /**< Signed 8-bit value */
#define BM_UINT8(c)   c        /**< Unsigned 8-bit value */
#define BM_INT16(c)   c        /**< Signed 16-bit value */
#define BM_UINT16(c)  c        /**< Unsigned 16-bit value */
#define BM_INT32(c)   c        /**< Signed 32-bit value */
#define BM_UINT32(c)  c ## U   /**< Unsigned 32-bit value */
#define BM_INT64(c)   c ## LL  /**< Signed 64-bit value */
#define BM_UINT64(c)  c ## ULL /**< Unsigned 64-bit value */
#define BM_INTPTR(c)  c ## L   /**< Signed value of size greater than or equal to a pointer */
#define BM_UINTPTR(c) c ## UL  /**< Unsigned value of size greater than or equal to a pointer */
/** @} */

/**
 * @defgroup    bm_constant_defines Generic constant values
 * @{
 */
#define BM_FALSE            ((bm_bool) 0)   /**< False value */
#define BM_TRUE             (!BM_FALSE)     /**< True value */
#define BM_NULL             0               /**< Null value */
/** @} */

/**
 * @defgroup    bm_utility_macros   Utility macros
 * @{
 */

/**
 * @def         BM_COUNT_OF
 * @brief       Returns the number of items in a fixed size array
 */
#define BM_COUNT_OF(a)      (sizeof(a) / sizeof(a[0]))

/**
 * @def         asm
 * @brief       Allow the asm keyword in ANSI C89 for compilers that support it
 */
#ifndef asm
#if BM_COMPILER_IS_CLANG || BM_COMPILER_IS_GNUC
#ifdef __STRICT_ANSI__
#define asm                 __asm
#endif
#elif BM_COMPILER_IS_ARMCC
#define asm                 __asm
#else
#pragma message ("WARNING: asm is not defined on this compiler")
#endif
#endif

/**
 * @def         BM_INLINE
 * @brief       Allow the use of the inline keyword even in ANSI C89 for compilers that support it
 */
#if (BM_COMPILER_IS_CLANG || BM_COMPILER_IS_GNUC)
#define BM_INLINE           __inline
#elif (BM_COMPILER_IS_ARMCC || BM_COMPILER_IS_ARMCLANG)
#define BM_INLINE           __inline
#else
#define BM_INLINE
#endif

/**
 * @def         BM_ALWAYS_INLINE
 * @brief       Force a function to be always inlined
 */
#if (BM_COMPILER_IS_CLANG || BM_COMPILER_IS_GNUC)
#define BM_ALWAYS_INLINE    __attribute__((always_inline)) __inline
#elif (BM_COMPILER_IS_ARMCC || BM_COMPILER_IS_ARMCLANG)
#define BM_ALWAYS_INLINE    __attribute__((always_inline)) __inline
#else
#define BM_ALWAYS_INLINE
#endif

/**
 * @def         BM_NEVER_INLINE
 * @brief       Force a function to be never inlined
 */
#if defined(noinline)
/* U-Boot, for example, defines its own noinline macro that collides here */
#define BM_NEVER_INLINE     noinline
#elif (BM_COMPILER_IS_CLANG || BM_COMPILER_IS_GNUC)
#define BM_NEVER_INLINE     __attribute__((noinline))
#elif (BM_COMPILER_IS_ARMCC || BM_COMPILER_IS_ARMCLANG)
#define BM_NEVER_INLINE     __attribute__((noinline))
#else
#define BM_NEVER_INLINE
#endif

/**
 * @def         BM_NONNULL
 * @brief       Defines the __attribute__((nonnull)); allows some compiler checking for non-null values
 */
#if (BM_COMPILER_IS_CLANG || BM_COMPILER_IS_GNUC || BM_COMPILER_IS_ARMCC || BM_COMPILER_IS_ARMCLANG)
#define BM_NONNULL(x)       __attribute__((nonnull x))
#else
#define BM_NONNULL(x)
#endif

/**
 * @def         BM_RET_NONNULL
 * @brief       Defines the __attribute__((returns_nonnull)); allows compiler to know return value is non-null
 */
#if (BM_COMPILER_AT_LEAST_CLANG(3, 5, 0) || BM_COMPILER_AT_LEAST_GNUC(4, 9, 0))
#define BM_RET_NONNULL      __attribute__((returns_nonnull))
#else
#define BM_RET_NONNULL
#endif

/**
 * @def         BM_WEAK
 * @brief       Labels the function weak linkage
 */
#if (BM_COMPILER_IS_CLANG || BM_COMPILER_IS_GNUC || BM_COMPILER_IS_ARMCC || BM_COMPILER_IS_ARMCLANG)
#define BM_WEAK             __attribute__((weak))
#else
#pragma message ("WARNING: BM_WEAK is not defined on this compiler")
#define BM_WEAK
#endif

/**
 * @def         BM_PACKED_TYPE
 * @brief       Labels the function weak linkage
 */
#if (BM_COMPILER_IS_CLANG || BM_COMPILER_IS_GNUC || BM_COMPILER_IS_ARMCC || BM_COMPILER_IS_ARMCLANG)
#define BM_PACKED_TYPE      __attribute__((packed))
#else
#pragma message ("WARNING: BM_PACKED_TYPE is not defined on this compiler")
#define BM_PACKED_TYPE
#endif

/**
 * @def         BM_FORMAT_FUNCTION
 * @brief       Labels the function as having printf or similar format parameters. Compilers that support this attribute will check the
 *              parameters are sane
 */
#if (BM_COMPILER_IS_CLANG || BM_COMPILER_IS_GNUC || BM_COMPILER_IS_ARMCC || BM_COMPILER_IS_ARMCLANG)
#define BM_FORMAT_FUNCTION(archetype, string_index, first_to_check)      __attribute__((format(archetype, string_index, first_to_check)))
#else
#pragma message ("WARNING: BM_FORMAT_FUNCTION is not defined on this compiler")
#define BM_FORMAT_FUNCTION(archetype, string_index, first_to_check)
#endif

/** @} */

/**
 * @defgroup    bm_external_api User supplied external functions
 * @details     Defines the functions that the user must implement to support
 *              the barman agent.
 * @{
 */

/**
 * @brief   Reads the current sample timestamp value
 * @details Must provide some timestamp value for the time at the point of the
 *          call. The timer must provide monotonically incrementing value since
 *          some implementation defined start point. The counter must not
 *          overflow during the period that it is used. The counter is in arbitrary units
 *          and the mechanism for converting those units to nanoseconds is described as part of the
 *          protocol data header.
 * @return  The timestamp value in arbitrary units.
 */
extern bm_uint64 barman_ext_get_timestamp(void);

/**
 * @brief   Disables interrupts on the local processor only.
 *          Used to allow atomic accesses to certain resources (e.g. PMU counters)
 * @return  The current interrupt enablement status value (which must be preserved and passed
 *          to barman_ext_enable_interrupts_local to restore the previous state)
 * @note    A weak implementation of this function is provided that modifies DAIF on AArch64, or
 *          CPSR on AArch32
 */
extern bm_uintptr barman_ext_disable_interrupts_local(void);

/**
 * @brief   Enables interrupts on the local processor only.
 * @param   previous_state  The value that was previously returned from barman_ext_disable_interrupts_local
 * @note    A weak implementation of this function is provided that modifies DAIF on AArch64, or
 *          CPSR on AArch32
 */
extern void barman_ext_enable_interrupts_local(bm_uintptr previous_state);

/**
 * @brief   Given the MPIDR register, returns a unique processor number.
 * @details The implementation must return a value between `[0, N)` where `N` is the maximum number of processors in the system.
 *          For any valid permutation of the arguments a unique value must be returned, which must not change between successive calls to this function for the
 *          same argument values.
 *
 *          @code
 *
 *          //
 *          // Example implementation where processors are arranged as follows:
 *          //
 *          // aff2 | aff1 | aff0 | cpuno
 *          // -----+------+------+------
 *          //    0 |    0 |    0 |     0
 *          //    0 |    0 |    1 |     1
 *          //    0 |    0 |    2 |     2
 *          //    0 |    0 |    3 |     3
 *          //    0 |    1 |    0 |     4
 *          //    0 |    1 |    1 |     5
 *          //
 *          bm_uint32 barman_ext_map_multiprocessor_affinity_to_core_no(bm_uintptr mpidr)
 *          {
 *              return (mpidr & 0x03) + ((mpidr >> 6) & 0x4);
 *          }
 *
 *          @endcode
 *
 * @param   mpidr   The value of the MPIDR register
 * @return  The processor number
 * @note    This function need only be defined when BM_CONFIG_MAX_CORES > 1
 */
extern bm_uint32 barman_ext_map_multiprocessor_affinity_to_core_no(bm_uintptr mpidr);

/**
 * @brief   Given the MPIDR register, return the appropriate cluster number.
 * @details Cluster IDs should be numbered `[0, N)` where `N` is the number of clusters in the system.
 *
 *          @code
 *
 *          //
 *          // Example implementation which is compatible with the example implementation of barman_ext_map_multiprocessor_affinity_to_core_no given
 *          // above.
 *          //
 *          bm_uint32 barman_ext_map_multiprocessor_affinity_to_cluster_no(bm_uintptr mpidr)
 *          {
 *              return ((mpidr >> 8) & 0x1);
 *          }
 *
 *          @endcode
 * @param   mpidr   The value of the MPIDR register
 * @return  The cluster number
 * @note    This function need only be defined when BM_CONFIG_MAX_CORES > 1
 */
extern bm_uint32 barman_ext_map_multiprocessor_affinity_to_cluster_no(bm_uintptr mpidr);

#if BM_CONFIG_MAX_TASK_INFOS > 0

/** @brief  Task ID type */
typedef bm_uint32 bm_task_id_t;

/**
 * @return  Return the current task ID.
 */
extern bm_task_id_t barman_ext_get_current_task_id(void);

#endif

#if BM_CONFIG_USER_SUPPLIED_PMU_DRIVER
# ifndef BM_MAX_PMU_COUNTERS
#   error "BM_MAX_PMU_COUNTERS is not defined, but BM_CONFIG_USER_SUPPLIED_PMU_DRIVER is true"
# endif
# ifndef BM_PMU_INVALID_COUNTER_VALUE
#   error "BM_PMU_INVALID_COUNTER_VALUE is not defined, but BM_CONFIG_USER_SUPPLIED_PMU_DRIVER is true"
# endif
# ifndef BM_PMU_HAS_FIXED_CYCLE_COUNTER
#   error "BM_PMU_HAS_FIXED_CYCLE_COUNTER is not defined, but BM_CONFIG_USER_SUPPLIED_PMU_DRIVER is true"
# elif BM_PMU_HAS_FIXED_CYCLE_COUNTER
#   ifndef BM_PMU_CYCLE_COUNTER_ID
#     error "BM_PMU_CYCLE_COUNTER_ID is not defined, but BM_PMU_HAS_FIXED_CYCLE_COUNTER is true"
#   endif
#   ifndef BM_PMU_CYCLE_COUNTER_TYPE
#     error "BM_PMU_CYCLE_COUNTER_TYPE is not defined, but BM_PMU_HAS_FIXED_CYCLE_COUNTER is true"
#   endif
# endif

/**
 * @brief   Initialize the PMU on the current core.
 * @details This function programs the PMU hardware on the current core. The
 *          function must be called once on each core where we are interested in
 *          reading the PMU.
 * @param   n_event_types
 *      The number of additional configurable events to enable
 * @param   event_types
 *      An array of length `n_event_types` containing the event types for the
 *      additional events to enable
 * @return  The number of events that were enabled which will be whichever is
 *      lower from `n_event_types`, or the maximum number of events supported
 *      by the PMU.
 */
BM_NONNULL((2))
extern bm_uint32 barman_ext_init(bm_uint32 n_event_types, const bm_uint32 * event_types);

/**
 * @brief   Start recording events in the PMU
 */
extern void barman_ext_start(void);

/**
 * @brief   Stop recording events in the PMU
 */
extern void barman_ext_stop(void);

/**
 * @brief   Read the value of a PMU counter
 * @param   counter_no  The counter number
 * @return  The value of the counter, or {@link BM_PMU_INVALID_COUNTER_VALUE}
 *          if the counter was not read. Caller should detect this and skip the counter.
 */
extern bm_uint64 barman_ext_read_counter(bm_uint32 counter_no);

#endif

/** @} */

/**
 * @defgroup    bm_protocol     Protocol settings
 * @{ */

/**
 * @brief   Defines information about the monotonic clock used in the trace.
 * @details Timestamp information is stored in arbitrary units within samples.
 *          This reduces the overhead of making the trace by removing the need to
 *          transform the timestamp into nanoseconds at the point the sample is recorded.
 *          The host expects timestamps to be in nanoseconds.
 *          The arbitrary timestamp information is transformed to nanoseconds according to the following formula:
 *
 *          `bm_uint64 nanoseconds = (((timestamp - timestamp_base) * timestamp_multiplier) / timestamp_divisor;`
 *
 *          Therefore for a clock that already returns time in nanoseconds, `timestamp_multiplier` and
 *          `timestamp_divisor` should be configured as `1` and `1`.
 *          If the clock counts in microseconds then the multiplier and divisor should be set `1000` and `1`.
 *          If the clock counts at a rate of `n` Hz, then multiplier should be set `1000000000` and divisor as `n`.
 *
 */
struct bm_protocol_clock_info
{
    /** The base value of the timestamp such that the this value is zero in the trace */
    bm_uint64 timestamp_base;
    /** The clock rate ratio multiplier */
    bm_uint64 timestamp_multiplier;
    /** The clock rate ratio divisor */
    bm_uint64 timestamp_divisor;
    /** The unix timestamp base value such that a `timestamp_base` maps to `unix_base` unix time value. In nanoseconds */
    bm_uint64 unix_base_ns;
} BM_PACKED_TYPE;

#if BM_CONFIG_MAX_TASK_INFOS > 0
/**
 * @brief   A task information record.
 * @details Describes information about a unique task within the system
 */
struct bm_protocol_task_info
{
    /** The task id */
    bm_task_id_t task_id;
    /** The name of the task */
    const char * task_name;
};
#endif

#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
/**
 * @brief   A MMAP layout record.
 * @details Describes the position of some executable image (or section thereof) in memory allowing the host to map PC values to the appropriate
 *          executable image.
 */
struct bm_protocol_mmap_layout
{
#if BM_CONFIG_MAX_TASK_INFOS > 0
    /** The task ID to associate with the map */
    bm_task_id_t task_id;
#endif
    /** The base address of the image or image section */
    bm_uintptr base_address;
    /** The length of the image or image section */
    bm_uintptr length;
    /** The image section offset */
    bm_uintptr image_offset;
    /** The name of the image */
    const char * image_name;
};
#endif

#if BM_CONFIG_MAX_TASK_INFOS > 0

/**
 * @brief   Add a new task information record
 * @param   timestamp   The timestamp the record is inserted
 * @param   task_entry  The new task entry
 * @return  BM_TRUE on success, BM_FALSE on failure
 */
BM_PUBLIC_FUNCTION
BM_NONNULL((2))
bm_bool barman_add_task_record(bm_uint64 timestamp, const struct bm_protocol_task_info * task_entry)
    BM_PUBLIC_FUNCTION_BODY(BM_TRUE)

#endif /* BM_CONFIG_MAX_TASK_INFOS > 0 */

#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0

/**
 * @brief   Add a new mmap information record
 * @param   timestamp   The timestamp the record is inserted
 * @param   mmap_entry  The new mmap entry
 * @return  BM_TRUE on success, BM_FALSE on failure
 */
BM_PUBLIC_FUNCTION
BM_NONNULL((2))
bm_bool barman_add_mmap_record(bm_uint64 timestamp, const struct bm_protocol_mmap_layout * mmap_entry)
    BM_PUBLIC_FUNCTION_BODY(BM_TRUE)

#endif /* BM_CONFIG_MAX_MMAP_LAYOUTS > 0 */

/** @} */

/**
 * @defgroup    bm_intrinsics   Aliases for intrinsic assembler operations
 * @{
 *
 * @def         barman_wfi_intrinsic
 * @brief       Wait for interrupt
 * @details     Inserts an "WFI" instruction
 *
 * @def         barman_wfe_intrinsic
 * @brief       Wait for event
 * @details     Inserts an "WFE" instruction
 */

#if !BM_ARM_TARGET_ARCH_IS_UNKNOWN()

#define barman_wfi_intrinsic()    asm volatile("wfi")
#define barman_wfe_intrinsic()    asm volatile("wfe")

#else /* for unit tests */

extern void barman_wfi_intrinsic(void);
extern void barman_wfe_intrinsic(void);

#endif

/** @} */

/**
 * @defgroup    bm_public   Public functions
 * @{ */

#if BM_CONFIG_MAX_TASK_INFOS > 0

/**
 * @brief   Reason for a task switch
 */
enum bm_task_switch_reason
{
    /** Thread is preempted */
    BM_TASK_SWITCH_REASON_PREEMPTED = 0,
    /** Thread is blocked waiting (e.g. on IO) */
    BM_TASK_SWITCH_REASON_WAIT = 1
};

#endif

/**
 * @brief   Initialize barman
 * @param   buffer              Pointer to in memory buffer
 * @param   buffer_length       The length of the in memory buffer
 * @param   stm_configuration_registers       Base address of the STM configuration registers.
 *                                            Can be NULL if it will be initialized elsewhere, e.g., by the debugger
 * @param   stm_extended_stimulus_ports       Base address of the STM extended stimulus ports.
 * @param   itm_registers       Base address of the ITM registers.
 * @param   datastore_config    Pointer to configuration to pass to {@link barman_ext_datastore_initialize}
 * @param   target_name         The target device name
 * @param   clock_info          Information about the monotonic clock used for timestamps
 * @param   num_task_entries    The length of the array of task entries in `task_entries`.
 *                              If this value is greater than {@link BM_CONFIG_MAX_TASK_INFOS} then it will be truncated.
 * @param   task_entries        The task information descriptors. Can be NULL.
 * @param   num_mmap_entries    The length of the array of mmap entries in `mmap_entries`.
 *                              If this value is greater than {@link BM_CONFIG_MAX_MMAP_LAYOUT} then it will be truncated.
 * @param   mmap_entries        The mmap image layout descriptors. Can be NULL.
 * @param   timer_sample_rate   Timer based sampling rate; in Hz. Zero indicates no timer based sampling (assumes max 4GHz sample rate).
 *                              This value is informative only and is used for reporting the timer frequency in the Streamline UI.
 * @return  BM_TRUE on success, BM_FALSE on failure
 * @note    If BM_CONFIG_MAX_TASK_INFOS <= 0, then `num_task_entries` and `task_entries` are not present.
 *          If BM_CONFIG_MAX_MMAP_LAYOUTS <= 0, then `num_mmap_entries` and `mmap_entries` are not present.
 */
BM_PUBLIC_FUNCTION
#if (BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_LINEAR_RAM_BUFFER) || (BM_CONFIG_USE_DATASTORE ==  BM_CONFIG_USE_DATASTORE_CIRCULAR_RAM_BUFFER) || ((BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_USER_SUPPLIED) && BM_CONFIG_DATASTORE_USER_SUPPLIED_IS_IN_MEMORY)
bm_bool barman_initialize(bm_uint8 * buffer, bm_uintptr buffer_length,
#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_STM
bm_bool barman_initialize_with_stm_interface(void * stm_configuration_registers, void * stm_extended_stimulus_ports,
#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ITM
#if BM_ARM_ARCH_PROFILE == 'M'
bm_bool barman_initialize_with_itm_interface(
#else
bm_bool barman_initialize_with_itm_interface(void * itm_registers,
#endif
#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ETM
bm_bool barman_initialize_with_etm_interface(
#elif (BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_USER_SUPPLIED) || (BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_STREAMING_USER_SUPPLIED)
bm_bool barman_initialize_with_user_supplied(void * datastore_config,
#else
#error "BM_CONFIG_USE_DATASTORE is not set correctly"
#endif
                          const char * target_name, const struct bm_protocol_clock_info * clock_info,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                          bm_uint32 num_task_entries, const struct bm_protocol_task_info * task_entries,
#endif
#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
                          bm_uint32 num_mmap_entries, const struct bm_protocol_mmap_layout * mmap_entries,
#endif
                          bm_uint32 timer_sample_rate)
    BM_PUBLIC_FUNCTION_BODY(BM_TRUE)

/**
 * @brief   Enable sampling. Should be called once all PMUs are enabled and the data store is configured
 */
BM_PUBLIC_FUNCTION
void barman_enable_sampling(void)
    BM_PUBLIC_FUNCTION_BODY_VOID

/**
 * @brief   Disable sampling. Disables sampling without reconfiguring the PMU. Sampling may be resumed by a call to {@link barman_enable_sampling}
 */
BM_PUBLIC_FUNCTION
void barman_disable_sampling(void)
    BM_PUBLIC_FUNCTION_BODY_VOID

/**
 * @brief   Reads the configured PMU counters for the current core and inserts them into the data store.
 *          May also insert a program counter record using the return address as the PC sample.
 * @param   sample_return_address   BM_TRUE to sample the return address as PC, BM_FALSE to ignore.
 * @note    The PC values are what is shown in the Call Paths view in Streamline. Without calling this with `sample_return_address == BM_TRUE`
 *          or `barman_sample_counters_with_program_counter` with `pc != BM_NULL`, the Call Paths view will be blank.
 * @note    This function would typically be called with `sample_return_address == BM_TRUE` from application code not doing periodic sampling.
 * @note    This function must be run on the core for the PMU that it intends to sample from, and it must not be migrated to another core
 *          for the duration of the call. This is necessary as it will need to program the per-core PMU registers.
 */
BM_PUBLIC_FUNCTION
void barman_sample_counters(bm_bool sample_return_address)
    BM_PUBLIC_FUNCTION_BODY_VOID

/**
 * @brief   Reads the configured PMU counters for the current core and inserts them into the data store.
 *          Inserts a program counter record using the supplied PC value.
 * @param   pc  The PC value to record. The PC entry is not inserted if (pc == BM_NULL)
 * @note    The PC values are what is shown in the Call Paths view in Streamline. Without calling this with `pc != BM_NULL`
 *          or `barman_sample_counters` with `sample_return_address == BM_TRUE`, the Call Paths view will be blank.
 * @note    This function would typically be called from a periodic interrupt handler with the exception return address as pc.
 * @note    This function must be run on the core for the PMU that it intends to sample from, and it must not be migrated to another core
 *          for the duration of the call. This is necessary as it will need to program the per-core PMU registers.
 */
BM_PUBLIC_FUNCTION
void barman_sample_counters_with_program_counter(const void * pc)
    BM_PUBLIC_FUNCTION_BODY_VOID

#if BM_CONFIG_MAX_TASK_INFOS > 0

/**
 * @brief   Record that a task switch has occurred.
 * @param   reason Reason for the task switch
 * @note    This must be called after the task switch has occurred
 *          such that {@link bm_ext_get_current_task} returns the task_id of the switched to task.
 */
BM_PUBLIC_FUNCTION
void barman_record_task_switch(enum bm_task_switch_reason reason)
    BM_PUBLIC_FUNCTION_BODY_VOID

#endif

/**
 * @brief   Wraps WFI instruction, sends events before and after the WFI to log the time in WFI.
 * @details This function is safe to use in place of the usual WFI asm instruction as it will degenerate to just WFI instruction when
 *          barman is disabled.
 */
BM_PUBLIC_FUNCTION
void barman_wfi(void)
    BM_PUBLIC_FUNCTION_BODY_STATEMENT(barman_wfi_intrinsic())

/**
 * @brief   Wraps WFE instruction, sends events before and after the WFE to log the time in WFE.
 * @details This function is safe to use in place of the usual WFI asm instruction as it will degenerate to just WFE instruction when
 *          barman is disabled.
 */
BM_PUBLIC_FUNCTION
void barman_wfe(void)
    BM_PUBLIC_FUNCTION_BODY_STATEMENT(barman_wfe_intrinsic())

/**
 * @brief   May be called before a WFI/WFE or other similar halting event to log entry into the paused state.
 * @details Can be used in situations where `barman_wfi()`/`barman_wfe()` is not suitable.
 * @note    Must be used in pair with `barman_after_idle()`
 * @note    Using `barman_wfi()`/`barman_wfe()` is preferred in most cases as it takes care of calling the before and after functions
 */
BM_PUBLIC_FUNCTION
void barman_before_idle(void)
    BM_PUBLIC_FUNCTION_BODY_VOID

/**
 * @brief   May be called after a WFI/WFE or other similar halting event to log exit from the paused state.
 * @details Can be used in situations where `barman_wfi()`/`barman_wfe()` is not suitable.
 * @note    Must be used in pair with `barman_before_idle()`
 * @note    Using `barman_wfi()`/`barman_wfe()` is preferred in most cases as it takes care of calling the before and after functions
 */
BM_PUBLIC_FUNCTION
void barman_after_idle(void)
    BM_PUBLIC_FUNCTION_BODY_VOID

/**
 * @defgroup  bm_annotation_colors   Color macros to use for annotations
 * @{
 *
 * @def       BM_ANNOTATE_COLOR_XXXX
 * @brief     Named annotation colors
 *
 * @def       BM_ANNOTATE_COLOR_CYCLIC
 * @brief     Annotation color that cycles through a predefined set
 *
 * @def       BM_ANNOTATE_COLOR_RGB(R, G, B)
 * @brief     Create an annotation color from its components
 * @param     R The red component from 0 to 255
 * @param     G The green component from 0 to 255
 * @param     B The blue component from 0 to 255
 */
#define BM_ANNOTATE_COLOR_RED              0x1bff0000
#define BM_ANNOTATE_COLOR_BLUE             0x1b0000ff
#define BM_ANNOTATE_COLOR_GREEN            0x1b00ff00
#define BM_ANNOTATE_COLOR_PURPLE           0x1bff00ff
#define BM_ANNOTATE_COLOR_YELLOW           0x1bffff00
#define BM_ANNOTATE_COLOR_CYAN             0x1b00ffff
#define BM_ANNOTATE_COLOR_WHITE            0x1bffffff
#define BM_ANNOTATE_COLOR_LTGRAY           0x1bbbbbbb
#define BM_ANNOTATE_COLOR_DKGRAY           0x1b555555
#define BM_ANNOTATE_COLOR_BLACK            0x1b000000

#define BM_ANNOTATE_COLOR_CYCLIC           0

#define BM_ANNOTATE_COLOR_RGB(R, G, B)     (0x1b << 24 | (((R) & 0xff) << 16) | (((G) & 0xff) << 8) | ((B) & 0xff))
/** @} */

/**
 * @brief   Adds a string annotation with a display color, and assigns it to a channel.
 * @param   channel  The channel number.
 * @param   color    The annotation color from {@link bm_annotation_colors}.
 * @param   text     The annotation text or null to end previous annotation.
 * @note    Annotation channels and groups are used to organize annotations within the threads and processes section of the Timeline view.
 *          Each annotation channel appears in its own row under the thread. Channels can also be grouped and displayed under a group name, using the `barman_annotate_name_group` function.
 */
BM_PUBLIC_FUNCTION
void barman_annotate_channel(bm_uint32 channel, bm_uint32 color, const char * string)
    BM_PUBLIC_FUNCTION_BODY_VOID

/**
 * @brief   Defines a channel and attaches it to an existing group.
 * @param   channel  The channel number.
 * @param   group    The group number.
 * @param   name     The name of the channel.
 * @note    The channel number must be unique within the task.
 */
BM_PUBLIC_FUNCTION
void barman_annotate_name_channel(bm_uint32 channel, bm_uint32 group, const char * name)
    BM_PUBLIC_FUNCTION_BODY_VOID

/**
 * @brief   Defines an annotation group.
 * @param   group    The group number.
 * @param   name     The name of the group.
 * @note    The group identifier, group, must be unique within the task.
 */
BM_PUBLIC_FUNCTION
void barman_annotate_name_group(bm_uint32 group, const char * name)
    BM_PUBLIC_FUNCTION_BODY_VOID

/**
 * @brief   Adds a bookmark with a string and a color to the Timeline and Log views.
 * @details The string is displayed in the Timeline view when you hover over the bookmark and in the Message column in the Log view.
 * @param   color    The marker color from {@link bm_annotation_colors}.
 * @param   text     The marker text or null for no text.
 */
BM_PUBLIC_FUNCTION
void barman_annotate_marker(bm_uint32 color, const char * text)
    BM_PUBLIC_FUNCTION_BODY_VOID

/** @} */

/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */

/**
 * @defgroup    bm_custom_counters  Custom counter chart related items
 * @{
 */

/**
 * @brief   Chart series composition
 */
enum bm_custom_counter_chart_series_composition
{
    BM_SERIES_COMPOSITION_STACKED = 1, /**< Stacked series */
    BM_SERIES_COMPOSITION_OVERLAY = 2, /**< Overlay series*/
    BM_SERIES_COMPOSITION_LOG10 = 3 /**< Log10 series */
};

/**
 * @brief   Chart rendering type
 */
enum bm_custom_counter_chart_rendering_type
{
    BM_RENDERING_TYPE_FILLED = 1, /**< Filled chart */
    BM_RENDERING_TYPE_LINE = 2, /**< Line chart */
    BM_RENDERING_TYPE_BAR = 3 /**< Bar chart */
};

/**
 * @brief   The series data class
 */
enum bm_custom_counter_series_class
{
    BM_SERIES_CLASS_DELTA = 1, /**< Delta value */
    BM_SERIES_CLASS_INCIDENT = 2, /**< Incidental delta value */
    BM_SERIES_CLASS_ABSOLUTE = 3 /**< Absolute value */
};

/**
 * @brief   The series display type
 */
enum bm_custom_counter_series_display
{
    BM_SERIES_DISPLAY_ACCUMULATE = 1, /**< Accumulate delta values */
    BM_SERIES_DISPLAY_AVERAGE = 2, /**< Average absolute values */
    BM_SERIES_DISPLAY_HERTZ = 3, /**< Accumulate and average over one second */
    BM_SERIES_DISPLAY_MAXIMUM = 4, /**< Maximum absolute value */
    BM_SERIES_DISPLAY_MINIMUM = 5 /**< Minimum absolute value */
};

/**
 * @brief   Custom counter sampling function type
 * @param   value_out   A non-null pointer to a uint64 that will contain the sample value on successful read.
 * @return  BM_TRUE for successful read of sample, BM_FALSE otherwise
 */
typedef bm_bool (* bm_custom_counter_sampling_function)(bm_uint64 * value_out);

/**
 * @brief   Description of a custom chart series
 */
struct bm_custom_counter_chart_series
{
    /** The index of the chart the series belongs to */
    bm_uint32 chart_index;
    /** The name of the series */
    const char * name;
    /** Series units */
    const char * units;
    /** Description */
    const char * description;
    /** Data class */
    enum bm_custom_counter_series_class clazz;
    /** Display type */
    enum bm_custom_counter_series_display display;
    /** Multiplier value */
    double multiplier;
    /** Series colour */
    bm_uint32 colour;
    /** Sampling function; is NULL for push counters */
    bm_custom_counter_sampling_function sampling_function;
};

/**
 * @brief   Description of custom chart
 */
struct bm_custom_counter_chart
{
    /** The name of the chart */
    const char * name;
    /** The series composition */
    enum bm_custom_counter_chart_series_composition series_composition;
    /** The rendering type */
    enum bm_custom_counter_chart_rendering_type rendering_type;
    /** Average CSM selection */
    bm_bool average_selection;
    /** Average cores in aggregate view */
    bm_bool average_cores;
    /** Take percentage of max value */
    bm_bool percentage;
    /** Series are per-cpu */
    bm_bool per_cpu;
    /** The number of series */
    bm_uint32 num_series;
    /** The series */
    const struct bm_custom_counter_chart_series * const * series;
};

/** @} */

/* ----------------- Custom counters ----------------- */

/* ------------------------------------------------------------------------- */

#undef  BM_PUBLIC_FUNCTION
#undef  BM_PUBLIC_FUNCTION_BODY
#undef  BM_PUBLIC_FUNCTION_BODY_VOID

#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_BARMAN_H__ */
