/**
 * Copyright (C) Arm Limited 2017. All rights reserved.
 *
 * Use, modification and redistribution of this file is subject to your possession of a valid
 * DS-5 end user licence agreement and your compliance with all applicable terms and
 * conditions of such licence agreement
 */

#include "barman.h"

#if (!defined(BARMAN_DISABLED)) || (!BARMAN_DISABLED)
/* ------------------------------------------------------------------------- */

/** Value to define {@link BM_CONFIG_USE_DATASTORE} as if the user is to supply the datastore functions */
#define BM_CONFIG_USE_DATASTORE_USER_SUPPLIED       0

/* ------------------------------------------------------------------------- */

/**
 * @defgroup    bm_pmu_constants    Processor PMU related constants
 * @{ */

/**
 * @brief Enumerates all the supported processor CPUID values.
 */
enum barman_processor_cpuids {
    /** Cortex-A9 */
    BARMAN_PROCESSOR_CPUID_CORTEX_A9 = 0x41c09
};


/**
 * @brief Enumerates all the event type constants for all the supported processors.
 */
enum barman_processor_pmu_events {

    /* ---- Processor events for Cortex-A9 (0x41c09) ---- */

    /** [0xffffffff] Cortex-A9/Clock/Cycles - The number of core clock cycles */
    BARMAN_PMU_EVENT_CORTEX_A9_CLOCK_CYCLES = 0xffffffff,
    /** [0x0] Cortex-A9/Software/Increment - Incremented only on writes to the Software Increment Register */
    BARMAN_PMU_EVENT_CORTEX_A9_SOFTWARE_INCREMENT = 0x0,
    /** [0x1] Cortex-A9/Cache/Instruction refill - Instruction fetch that causes a refill of at least the level of instruction or unified cache closest to the processor */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_INSTRUCTION_REFILL = 0x1,
    /** [0x2] Cortex-A9/Cache/Inst TLB refill - Instruction fetch that causes a TLB refill of at least the level of TLB closest to the processor */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_INST_TLB_REFILL = 0x2,
    /** [0x3] Cortex-A9/Cache/Data refill - Memory Read or Write operation that causes a refill of at least the level of data or unified cache closest to the processor */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_DATA_REFILL = 0x3,
    /** [0x4] Cortex-A9/Cache/Data access - Memory Read or Write operation that causes a cache access to at least the level of data or unified cache closest to the processor */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_DATA_ACCESS = 0x4,
    /** [0x5] Cortex-A9/Cache/Data TLB refill - Memory Read or Write operation that causes a TLB refill of at least the level of TLB closest to the processor */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_DATA_TLB_REFILL = 0x5,
    /** [0x6] Cortex-A9/Instruction/Load - Memory-reading instruction architecturally executed */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_LOAD = 0x6,
    /** [0x7] Cortex-A9/Instruction/Store - Memory-writing instruction architecturally executed */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_STORE = 0x7,
    /** [0x9] Cortex-A9/Exception/Taken - Exception taken */
    BARMAN_PMU_EVENT_CORTEX_A9_EXCEPTION_TAKEN = 0x9,
    /** [0xa] Cortex-A9/Exception/Return - Exception return architecturally executed */
    BARMAN_PMU_EVENT_CORTEX_A9_EXCEPTION_RETURN = 0xa,
    /** [0xb] Cortex-A9/Instruction/CONTEXTIDR - Instruction that writes to the CONTEXTIDR architecturally executed */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_CONTEXTIDR = 0xb,
    /** [0xc] Cortex-A9/Branch/PC change - Software change of the Program Counter, except by an exception, architecturally executed */
    BARMAN_PMU_EVENT_CORTEX_A9_BRANCH_PC_CHANGE = 0xc,
    /** [0xd] Cortex-A9/Branch/Immediate - Immediate branch architecturally executed */
    BARMAN_PMU_EVENT_CORTEX_A9_BRANCH_IMMEDIATE = 0xd,
    /** [0xf] Cortex-A9/Memory/Unaligned access - Unaligned access architecturally executed */
    BARMAN_PMU_EVENT_CORTEX_A9_MEMORY_UNALIGNED_ACCESS = 0xf,
    /** [0x10] Cortex-A9/Branch/Mispredicted - Branch mispredicted or not predicted */
    BARMAN_PMU_EVENT_CORTEX_A9_BRANCH_MISPREDICTED = 0x10,
    /** [0x12] Cortex-A9/Branch/Potential prediction - Branch or other change in program flow that could have been predicted by the branch prediction resources of the processor */
    BARMAN_PMU_EVENT_CORTEX_A9_BRANCH_POTENTIAL_PREDICTION = 0x12,
    /** [0x40] Cortex-A9/Java/Bytecode execute - Counts the number of Java bytecodes being decoded, including speculative ones */
    BARMAN_PMU_EVENT_CORTEX_A9_JAVA_BYTECODE_EXECUTE = 0x40,
    /** [0x41] Cortex-A9/Java/SW bytecode execute - Counts the number of software Java bytecodes being decoded, including speculative ones */
    BARMAN_PMU_EVENT_CORTEX_A9_JAVA_SW_BYTECODE_EXECUTE = 0x41,
    /** [0x42] Cortex-A9/Jazelle/Backward branch execute - Counts the number of Jazelle taken branches being executed. This includes the branches that are flushed because of a previous load/store that aborts late. */
    BARMAN_PMU_EVENT_CORTEX_A9_JAZELLE_BACKWARD_BRANCH_EXECUTE = 0x42,
    /** [0x50] Cortex-A9/Cache/Coherency miss - Counts the number of coherent linefill requests performed by the Cortex-A9 processor that also miss in all the other Cortex-A9 processors. This means that the request is sent to the external memory. */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_COHERENCY_MISS = 0x50,
    /** [0x51] Cortex-A9/Cache/Coherency hit - Counts the number of coherent linefill requests performed by the Cortex-A9 processor that hit in another Cortex-A9 processor. This means that the linefill data is fetched directly from the relevant Cortex-A9 cache. */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_COHERENCY_HIT = 0x51,
    /** [0x60] Cortex-A9/Cache/Inst dependent stall - Counts the number of cycles where the processor: is ready to accept new instructions, does not receive a new instruction, because: the instruction side is unable to provide one or the instruction cache is performing at least one linefill */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_INST_DEPENDENT_STALL = 0x60,
    /** [0x61] Cortex-A9/Cache/Data dependent stall - Counts the number of cycles where the processor has some instructions that it cannot issue to any pipeline, and the Load Store unit has at least one pending linefill request, and no pending TLB requests */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_DATA_DEPENDENT_STALL = 0x61,
    /** [0x62] Cortex-A9/Cache/TLB stall - Counts the number of cycles where the processor is stalled waiting for the completion of translation table walks from the main TLB. The processor stalls because the instruction side is not able to provide the instructions, or the data side is not able to provide the necessary data. */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_TLB_STALL = 0x62,
    /** [0x63] Cortex-A9/Intrinsic/STREX pass - Counts the number of STREX instructions architecturally executed and passed */
    BARMAN_PMU_EVENT_CORTEX_A9_INTRINSIC_STREX_PASS = 0x63,
    /** [0x64] Cortex-A9/Intrinsic/STREX fail - Counts the number of STREX instructions architecturally executed and failed */
    BARMAN_PMU_EVENT_CORTEX_A9_INTRINSIC_STREX_FAIL = 0x64,
    /** [0x65] Cortex-A9/Cache/Data eviction - Counts the number of eviction requests because of a linefill in the data cache */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_DATA_EVICTION = 0x65,
    /** [0x66] Cortex-A9/Pipeline/Issue stage no dispatch - Counts the number of cycles where the issue stage does not dispatch any instruction because it is empty or cannot dispatch any instructions */
    BARMAN_PMU_EVENT_CORTEX_A9_PIPELINE_ISSUE_STAGE_NO_DISPATCH = 0x66,
    /** [0x67] Cortex-A9/Pipeline/Issue stage empty - Counts the number of cycles where the issue stage is empty */
    BARMAN_PMU_EVENT_CORTEX_A9_PIPELINE_ISSUE_STAGE_EMPTY = 0x67,
    /** [0x68] Cortex-A9/Instruction/Executed - Counts the number of instructions going through the Register Renaming stage. This number is an approximate number of the total number of instructions speculatively executed, and an even more approximate number of the total number of instructions architecturally executed. The approximation depends mainly on the branch misprediction rate. */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_EXECUTED = 0x68,
    /** [0x69] Cortex-A9/Cache/Data linefills - Counts the number of linefills performed on the external AXI bus. This event counts all data linefill requests, caused by: loads, including speculative ones, stores, PLD, prefetch or page table walk. */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_DATA_LINEFILLS = 0x69,
    /** [0x6a] Cortex-A9/Cache/Prefetch linefills - Counts the number of data linefills caused by prefetcher requests */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_PREFETCH_LINEFILLS = 0x6a,
    /** [0x6b] Cortex-A9/Cache/Prefetch hits - Counts the number of cache hits in a line that belongs to a stream followed by the prefetcher. This includes: lines that have been prefetched by the automatic data prefetcher and lines already present in the cache, before the prefetcher action. */
    BARMAN_PMU_EVENT_CORTEX_A9_CACHE_PREFETCH_HITS = 0x6b,
    /** [0x6e] Cortex-A9/Procedure/Return - Counts the number of procedure returns whose condition codes do not fail, excluding all returns from exception. This count includes procedure returns that are flushed because of a previous load/store that aborts late. */
    BARMAN_PMU_EVENT_CORTEX_A9_PROCEDURE_RETURN = 0x6e,
    /** [0x70] Cortex-A9/Instruction/Main execution unit - Counts the number of instructions being executed in the main execution pipeline of the processor, the multiply pipeline and arithmetic logic unit pipeline. The counted instructions are still speculative. */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_MAIN_EXECUTION_UNIT = 0x70,
    /** [0x71] Cortex-A9/Instruction/Second execution unit - Counts the number of instructions being executed in the processor second execution pipeline (ALU). The counted instructions are still speculative. */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_SECOND_EXECUTION_UNIT = 0x71,
    /** [0x72] Cortex-A9/Instruction/Load/Store - Counts the number of instructions being executed in the Load/Store unit. The counted instructions are still speculative. */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_LOAD_STORE = 0x72,
    /** [0x73] Cortex-A9/Instruction/Floating point - Counts the number of floating-point instructions going through the Register Rename stage. Instructions are still speculative in this stage. */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_FLOATING_POINT = 0x73,
    /** [0x74] Cortex-A9/Instruction/NEON - Counts the number of NEON instructions going through the Register Rename stage. Instructions are still speculative in this stage. */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_NEON = 0x74,
    /** [0x80] Cortex-A9/Stalls/PLD - Counts the number of cycles where the processor is stalled because PLD slots are all full */
    BARMAN_PMU_EVENT_CORTEX_A9_STALLS_PLD = 0x80,
    /** [0x81] Cortex-A9/Stalls/Memory write - Counts the number of cycles when the processor is stalled. The data side is stalled also, because it is full and executes writes to the external memory. */
    BARMAN_PMU_EVENT_CORTEX_A9_STALLS_MEMORY_WRITE = 0x81,
    /** [0x82] Cortex-A9/Stalls/Inst main TLB miss - Counts the number of stall cycles because of main TLB misses on requests issued by the instruction side */
    BARMAN_PMU_EVENT_CORTEX_A9_STALLS_INST_MAIN_TLB_MISS = 0x82,
    /** [0x83] Cortex-A9/Stalls/Data main TLB miss - Counts the number of stall cycles because of main TLB misses on requests issued by the data side */
    BARMAN_PMU_EVENT_CORTEX_A9_STALLS_DATA_MAIN_TLB_MISS = 0x83,
    /** [0x84] Cortex-A9/Stalls/Inst micro TLB miss - Counts the number of stall cycles because of micro TLB misses on the instruction side. This event does not include main TLB miss stall cycles that are already counted in the corresponding main TLB event. */
    BARMAN_PMU_EVENT_CORTEX_A9_STALLS_INST_MICRO_TLB_MISS = 0x84,
    /** [0x85] Cortex-A9/Stalls/Data micro TLB miss - Counts the number of stall cycles because of micro TLB misses on the data side. This event does not include main TLB miss stall cycles that are already counted in the corresponding main TLB event. */
    BARMAN_PMU_EVENT_CORTEX_A9_STALLS_DATA_MICRO_TLB_MISS = 0x85,
    /** [0x86] Cortex-A9/Stalls/DMB - Counts the number of stall cycles because of the execution of a DMB. This includes all DMB instructions being executed, even speculatively. */
    BARMAN_PMU_EVENT_CORTEX_A9_STALLS_DMB = 0x86,
    /** [0x8a] Cortex-A9/Clock/Integer core - Counts the number of cycles when the integer core clock is enabled */
    BARMAN_PMU_EVENT_CORTEX_A9_CLOCK_INTEGER_CORE = 0x8a,
    /** [0x8b] Cortex-A9/Clock/Data engine - Counts the number of cycles when the data engine clock is enabled */
    BARMAN_PMU_EVENT_CORTEX_A9_CLOCK_DATA_ENGINE = 0x8b,
    /** [0x8c] Cortex-A9/Clock/NEON - Counts the number of cycles when the NEON SIMD clock is enabled */
    BARMAN_PMU_EVENT_CORTEX_A9_CLOCK_NEON = 0x8c,
    /** [0x8d] Cortex-A9/Memory/TLB inst allocations - Counts the number of TLB allocations because of Instruction requests */
    BARMAN_PMU_EVENT_CORTEX_A9_MEMORY_TLB_INST_ALLOCATIONS = 0x8d,
    /** [0x8e] Cortex-A9/Memory/TLB data allocations - Counts the number of TLB allocations because of Data requests */
    BARMAN_PMU_EVENT_CORTEX_A9_MEMORY_TLB_DATA_ALLOCATIONS = 0x8e,
    /** [0x90] Cortex-A9/Instruction/ISB - Counts the number of ISB instructions architecturally executed */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_ISB = 0x90,
    /** [0x91] Cortex-A9/Instruction/DSB - Counts the number of DSB instructions architecturally executed */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_DSB = 0x91,
    /** [0x92] Cortex-A9/Instruction/DMB - Counts the number of DMB instructions speculatively executed */
    BARMAN_PMU_EVENT_CORTEX_A9_INSTRUCTION_DMB = 0x92,
    /** [0x93] Cortex-A9/External/Interrupts - Counts the number of external interrupts executed by the processor */
    BARMAN_PMU_EVENT_CORTEX_A9_EXTERNAL_INTERRUPTS = 0x93,
    /** [0xa0] Cortex-A9/PLE/Cache line rq completed - Counts the number of PLE cache line requests completed */
    BARMAN_PMU_EVENT_CORTEX_A9_PLE_CACHE_LINE_RQ_COMPLETED = 0xa0,
    /** [0xa1] Cortex-A9/PLE/Cache line rq skipped - Counts the number of PLE cache line requests skipped */
    BARMAN_PMU_EVENT_CORTEX_A9_PLE_CACHE_LINE_RQ_SKIPPED = 0xa1,
    /** [0xa2] Cortex-A9/PLE/FIFO flush - Counts the number of PLE FIFO flush requests */
    BARMAN_PMU_EVENT_CORTEX_A9_PLE_FIFO_FLUSH = 0xa2,
    /** [0xa3] Cortex-A9/PLE/Request completed - Counts the number of PLE FIFO flush completed */
    BARMAN_PMU_EVENT_CORTEX_A9_PLE_REQUEST_COMPLETED = 0xa3,
    /** [0xa4] Cortex-A9/PLE/FIFO overflow - Counts the number of PLE FIFO flush overflowed */
    BARMAN_PMU_EVENT_CORTEX_A9_PLE_FIFO_OVERFLOW = 0xa4,
    /** [0xa5] Cortex-A9/PLE/Request programmed - Counts the number of PLE FIFO flush program requests */
    BARMAN_PMU_EVENT_CORTEX_A9_PLE_REQUEST_PROGRAMMED = 0xa5
};


/** @} */

/* ------------------------------------------------------------------------- */

/* We define our own to reduce dependency on external headers, and to avoid
 * conflicts with any definitions in other headers where barman headers
 * are included */

/**
 * @defgroup    bm_utility_macros   Utility macros
 * @{
 */

/**
 * @def         BM_BIG_ENDIAN
 */
#if defined(__ARM_BIG_ENDIAN) || defined(__BIG_ENDIAN) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define BM_BIG_ENDIAN       (1)
#endif

/**
 * @def         BM_BIT
 * @brief       Sets the `b`th bit in an integer value
 */
#define BM_BIT(b)           (1ul << (b))

/**
 * @def         BM_GET_BYTE
 * @brief       Gets the `b`th byte in an integer value
 */
#define BM_GET_BYTE(byte, value)           (0xff & ((value) >> 8 * (byte)))

/**
 * @def         BM_SWAP_ENDIANESS_32
 * @brief       Swaps the endianess of a 32 bit integer
 */
#define BM_SWAP_ENDIANESS_32(value)        (BM_GET_BYTE(0, value) << 24 | BM_GET_BYTE(1, value) << 16 | BM_GET_BYTE(2, value) << 8 | BM_GET_BYTE(3, value))

/**
 * @def         BM_MIN
 * @brief       Returns the result of (a < b ? a : b)
 */
#define BM_MIN(a, b)        ((a) < (b) ? (a) : (b))

/**
 * @def         BM_MAX
 * @brief       Returns the result of (a > b ? a : b)
 */
#define BM_MAX(a, b)        ((a) > (b) ? (a) : (b))

/**
 * @def         BM_STRINGIFY_TOK
 * @brief       The parameter token(s) are converted into a string
 */
#define BM_STRINGIFY_TOK(x) #x

/**
 * @def         BM_STRINGIFY_VAL
 * @brief       The value of the parameter is converted into a string
 */
#define BM_STRINGIFY_VAL(x) BM_STRINGIFY_TOK(x)

/**
 * @def         BM_UNALIGNED_POINTER_TYPE
 * @brief       Makes type type into an unaligned pointer type
 */
#if (BM_COMPILER_IS_CLANG || BM_COMPILER_IS_GNUC || BM_COMPILER_IS_ARMCC || BM_COMPILER_IS_ARMCLANG)
#define BM_UNALIGNED_POINTER_TYPE(type)     type __attribute__((aligned(1)))  *
#else
#pragma message ("WARNING: BM_UNALIGNED_POINTER_TYPE is not defined on this compiler")
#define BM_UNALIGNED_POINTER_TYPE(type)     type *
#endif

/**
 * @def         BM_ALIGN
 * @brief       Force alignment attribute
 */
#if (BM_COMPILER_IS_GNUC)
#define BM_ALIGN(alignment)     __attribute__((aligned(alignment)))
#else
#pragma message ("WARNING: BM_ALIGN is not defined on this compiler")
#define BM_ALIGN(alignment)
#endif

/**
 * @def         BM_ASSUME_ALIGNED
 * @brief       This function returns its first argument,
 *              and allows the compiler to assume that the returned pointer is at least alignment bytes aligned
 */
#if (BM_COMPILER_IS_GNUC && !BM_COMPILER_IS_ARMCC )
#define BM_ASSUME_ALIGNED(pointer, alignment)     __builtin_assume_aligned(pointer, alignment)
#else
#define BM_ASSUME_ALIGNED(pointer, alignment)     (pointer)
#if !BM_COMPILER_IS_ARMCC
#pragma message ("WARNING: BM_ASSUME_ALIGNED is not defined on this compiler, -Wcast-align may be triggered")
#endif
#endif

/**
 * @def         BM_ASSUME_ALIGNED_CAST
 * @brief       This function returns its second argument cast a pointer to type,
 *              and allows the compiler to assume that the returned pointer is at least aligned the size of type
 */
#define BM_ASSUME_ALIGNED_CAST(type, pointer)     ((type * ) (bm_uintptr)(pointer))

/**
 * @def         BM_UNALIGNED_CAST_DEREF_ASSIGN
 * @brief       Casts pointer to pointer to unaligned type, dereferences it and assigns value to it
 */
#if (BM_COMPILER_IS_GNUC)
#define BM_UNALIGNED_CAST_DEREF_ASSIGN(type, pointer, value)                \
    do {                                                                    \
        struct {type field __attribute__((packed));} * _bm_struct_ptr;      \
        _bm_struct_ptr = __extension__ (__typeof(_bm_struct_ptr)) pointer;  \
        _bm_struct_ptr->field = value;                                      \
    } while (0)
#else
#pragma message ("WARNING: BM_UNALIGNED_CAST_DEREF_ASSIGN is not defined on this compiler")
#define BM_UNALIGNED_CAST_DEREF_ASSIGN(type, pointer, value)     do {*((type *) (pointer)) = value; } while (0)
#endif

/**
 * @def         BM_C_ASSERT
 * @brief       Encodes an expression that evaluates to the value of `res`, but for compilers that support it
 *              will use whatever means to encode a statement that will cause the compiler to fail if `test`
 *              evaluates at compile time to false.
 *              `token` must be a identifier token that is inserted into the generated expression to help identify the
 *              failure.
 */
#if (BM_COMPILER_IS_CLANG || BM_COMPILER_IS_GNUC)
#define BM_C_ASSERT(test, token, res)       ({ enum {token = 1 / ((test) ? 1 : 0)}; (res); })
#else
#define BM_C_ASSERT(test, token, res)       (res)
#endif

/**
 * @def         BM_MEMORY_MAPPED_REGISTER
 * @brief       Creates a memory mapped register l-value at `address` of type `size_type`
 */
#define BM_MEMORY_MAPPED_REGISTER(address, size_type)    (*BM_ASSUME_ALIGNED_CAST(volatile size_type, ((void*) (address))))
#define BM_MEMORY_MAPPED_REGISTER_8(address)             BM_MEMORY_MAPPED_REGISTER(address, bm_uint8)
#define BM_MEMORY_MAPPED_REGISTER_16(address)            BM_MEMORY_MAPPED_REGISTER(address, bm_uint16)
#define BM_MEMORY_MAPPED_REGISTER_32(address)            BM_MEMORY_MAPPED_REGISTER(address, bm_uint32)
#define BM_MEMORY_MAPPED_REGISTER_64(address)            BM_MEMORY_MAPPED_REGISTER(address, bm_uint64)

/**
 * @brief The unit type which only has one value
 */
typedef enum { BM_UNIT_TYPE_VALUE = 1 } bm_unit_type;

/** @} */

/**
 * @defgroup    bm_memutils Memory utility functions
 * @{ */

#if BM_CONFIG_ENABLE_BUILTIN_MEMFUNCS

/**
 * @brief   Implements `memset` functionality
 */
#define barman_memset(ptr, value, num) __builtin_memset(ptr, value, num)

/**
 * @brief   Implements `memcpy` functionality
 */
#define barman_memcpy(dest, src, count) __builtin_memcpy(dest, src, count)

#else

/**
 * @brief   Implements `memset` functionality
 */
BM_RET_NONNULL
BM_NONNULL((1))
void * barman_memset(void * ptr, int value, bm_size_t num);

/**
 * @brief   Implements `memcpy` functionality
 */
BM_RET_NONNULL
BM_NONNULL((1,2))
void * barman_memcpy(void * dest, const void * src, bm_size_t num);

#endif

/** @} */

/**
 * @defgroup    bm_core_set Core set type
 * @{ */

/** Structure that forms a bitmask where each bit represents the `n`th core. A set bit indicates the core is excluded from the configuration */
typedef bm_uint8 bm_core_set[((BM_CONFIG_MAX_CORES + 7) / 8)];

/**
 * @brief   Determine if a core is in the set.
 * @param   disallowed_cores    The value to examine
 * @param   core                The core number to check
 * @return  BM_TRUE if marked in the set, BM_FALSE otherwise
 */
static BM_INLINE bm_bool barman_core_set_is_set(const bm_core_set core_set, bm_uint32 core)
{
    const bm_uint32 byte = core >> 3;
    const bm_uint32 bit = core & 7;

    if (core_set == BM_NULL) {
        return BM_FALSE;
    }

    if (byte >= sizeof(bm_core_set)) {
        return BM_FALSE;
    }

    return (core_set[byte] & BM_BIT(bit)) != 0;
}

/** @} */

/**
 * @brief   Set the PMU configuration settings for the PMU family that matches a given MIDR
 * @param   midr                The MIDR that the data is for
 * @param   n_event_types       The number of configurable events to enable
 * @param   event_types         An array of length `n_event_types` containing the event types for the events to enable
 * @param   allowed_cores       Array enumerating the cores that should be included in this configuration. BM_NULL will be interpreted as all *included*.
 * @return  BM_TRUE on success, BM_FALSE on failure.
 * @note    This call will fail if the data for a MIDR (and cores) has already been programmed
 * @ingroup bm_public
 */
BM_NONNULL((3))
bm_bool barman_initialize_pmu_family(bm_uint32 midr, bm_uint32 n_event_types, const bm_uint32 * event_types, const bm_core_set allowed_cores);

/* ------------------------------------------------------------------------- */

/* ----------------- Custom counters ----------------- */

#define BM_NUM_CUSTOM_COUNTERS 0
#define BM_CUSTOM_CHARTS_COUNT 0



/* ------------------------------------------------------------------------- */

/**
 * Datastore configuration for the STM backend
 */
struct bm_datastore_config_stm
{
    /** Base address of the STM configuration registers */
    void * configuration_registers;
    /** Base address of the STM extended stimulus ports */
    void * extended_stimulus_ports;
};

/**
 * Initialize the STM
 *
 * @param config    STM run-time configuration
 */
bm_bool barman_stm_init(struct bm_datastore_config_stm config);

/**
 * Write as STM frame
 *
 * @param data    Data to write in the frame
 * @param length  Length of the frame
 * @param channel Channel to write the frame on
 * @param flush   Whether to flush the channel after writing the frame (costs a flag packet)
 */
void barman_stm_write_frame(const bm_uint8 * data, bm_uintptr length, bm_uint16 channel, bm_bool flush);

struct bm_datastore_config_itm
{
    /** Base address of the ITM registers */
    void * registers;
};

/**
 * Initialize the ITM
 *
 * @param config    ITM run-time configuration
 * @return          True if successful
 */
bm_bool barman_itm_init(struct bm_datastore_config_itm config);

/**
 * Write as ITM frame
 *
 * @param data    Data to write in the frame
 * @param length  Length of the frame
 * @param channel Channel to write the frame on
 * @param flush   Whether to flush the channel after writing the frame (costs a 16 bit packet)
 */
void barman_itm_write_frame(const bm_uint8 * data, bm_uintptr length, bm_uint16 channel, bm_bool flush);

#if BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_LINEAR_RAM_BUFFER
#   define BM_DATASTORE_IS_IN_MEMORY                            1
#   define BM_DATASTORE_USES_STREAMING_INTERFACE                0

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_CIRCULAR_RAM_BUFFER
#   define BM_DATASTORE_IS_IN_MEMORY                            1
#   define BM_DATASTORE_USES_STREAMING_INTERFACE                0

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_STM
#   define BM_DATASTORE_IS_IN_MEMORY                            0
#   define BM_DATASTORE_USES_STREAMING_INTERFACE                1
    typedef struct bm_datastore_config_stm bm_datastore_config;

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ITM
#   define BM_DATASTORE_IS_IN_MEMORY                            0
#   define BM_DATASTORE_USES_STREAMING_INTERFACE                1
    typedef struct bm_datastore_config_itm bm_datastore_config;

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ETM
#   define BM_DATASTORE_IS_IN_MEMORY                            0
#   define BM_DATASTORE_USES_STREAMING_INTERFACE                1
    typedef bm_unit_type bm_datastore_config;

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_STREAMING_USER_SUPPLIED
#   define BM_DATASTORE_IS_IN_MEMORY                            0
#   define BM_DATASTORE_USES_STREAMING_INTERFACE                1
    typedef void * bm_datastore_config;

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_USER_SUPPLIED
# ifndef BM_CONFIG_DATASTORE_USER_SUPPLIED_IS_IN_MEMORY
#   error "BM_CONFIG_DATASTORE_USER_SUPPLIED_IS_IN_MEMORY must be defined true or false"
# endif
#   define BM_DATASTORE_IS_IN_MEMORY                            BM_CONFIG_DATASTORE_USER_SUPPLIED_IS_IN_MEMORY
#   define BM_DATASTORE_USES_STREAMING_INTERFACE                0

#if !BM_DATASTORE_IS_IN_MEMORY
    typedef void * bm_datastore_config;
#endif

#else
#   error "BM_CONFIG_USE_DATASTORE is not set correctly"
#endif

#if BM_DATASTORE_IS_IN_MEMORY
    /**
     * Datastore configuration for in memory datastores
     */
    struct bm_datastore_config_in_memory
    {
        /**  Pointer to in memory buffer */
        bm_uint8 * buffer;
        /** The length of the in memory buffer, or 0 if configured for other type of data store */
        bm_uintptr buffer_length;
    };

    typedef struct bm_datastore_config_in_memory bm_datastore_config;
#endif

/**
 * @brief   Initialize the protocol and underlying data store
 * @param   datastore_config    A backend specific configuration struct
 * @param   target_name     The target device name
 * @param   clock_info      Information about the monotonic clock used for timestamps
 * @param   num_task_entries    The length of the array of task entries in `task_entries`.
 *                              If this value is greater than {@link BM_CONFIG_MAX_TASK_INFOS} then it will be truncated.
 * @param   task_entries    The task information descriptors. Can be NULL.
 * @param   num_mmap_entries    The length of the array of mmap entries in `mmap_entries`.
 *                              If this value is greater than {@link BM_CONFIG_MAX_MMAP_LAYOUT} then it will be truncated.
 * @param   mmap_entries    The mmap image layout descriptors. Can be NULL.
 * @param   timer_sample_rate   Timer based sampling rate; in Hz. Zero indicates no timer based sampling (assumes max 4GHz sample rate)
 * @return  BM_TRUE on success, BM_FALSE on failure
 * @note    If BM_CONFIG_MAX_TASK_INFOS <= 0, then `num_task_entries` and `task_entries` are not present.
 *          If BM_CONFIG_MAX_MMAP_LAYOUTS <= 0, then `num_mmap_entries` and `mmap_entries` are not present.
 */
BM_NONNULL((2, 3))
bm_bool barman_protocol_initialize(bm_datastore_config datastore_config,
                                   const char * target_name, const struct bm_protocol_clock_info * clock_info,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                   bm_uint32 num_task_entries, const struct bm_protocol_task_info * task_entries,
#endif
#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
                                   bm_uint32 num_mmap_entries, const struct bm_protocol_mmap_layout * mmap_entries,
#endif
                                   bm_uint32 timer_sample_rate
                                  );

/**
 * @brief   Store the PMU counters that were configured for a given PMU
 * @param   timestamp       The timestamp the settings were configured
 * @param   midr            The MIDR register value of the core
 * @param   mpidr           The MPIDR register value of the core
 * @param   core            The core number the settings were for
 * @param   num_counters    The number of counters (the length of `counter_types`)
 * @param   counter_types   The PMU event types that were configured.
 * @return  BM_TRUE on success, BM_FALSE on failure
 * @ingroup bm_protocol
 */
BM_NONNULL((6))
bm_bool barman_protocol_write_pmu_settings(bm_uint64 timestamp, bm_uint32 midr, bm_uintptr mpidr, bm_uint32 core, bm_uint32 num_counters, const bm_uint32 * counter_types);

/**
 * @brief   Store a sample record for a core
 * @param   timestamp       The timestamp the sample was recorded
 * @param   core            The core number the settings were for
 * @param   task_id         The task ID associated with the sample, or zero if not associated with any task
 * @param   pc              The program counter value to associate with the sample, or ignored if BM_NULL
 * @param   num_counters    The length of `counter_values`
 * @param   counter_values  The counter values recorded for each counter.
 * @param   num_custom_counters     The number of customer counter values (length of `custom_counter_ids` and `custom_counter_values`)
 * @param   custom_counter_ids      Array of custom counter ids where each id maps to the value in `custom_counter_values` at the same index
 * @param   custom_counter_values   Array of custom counter values
 * @return  BM_TRUE on success, BM_FALSE on failure
 * @note    It is expected that there is a 1:1 mapping from `counter_value` to `counter_type` that was specified in
 *          {@link barman_protocol_write_pmu_settings} such that `counter_values[n]` is the value for counter specified in `counter_types[n]` for any valid
 *          value of `n`.
 * @note    If BM_CONFIG_MAX_TASK_INFOS <= 0, then `task_id` is not present.
 * @ingroup bm_protocol
 */
BM_NONNULL((5 + (BM_CONFIG_MAX_TASK_INFOS > 0 ? 1 : 0), 7 + (BM_CONFIG_MAX_TASK_INFOS > 0 ? 1 : 0), 8 + (BM_CONFIG_MAX_TASK_INFOS > 0 ? 1 : 0)))
bm_bool barman_protocol_write_sample(bm_uint64 timestamp, bm_uint32 core,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                     bm_task_id_t task_id,
#endif
                                     const void * pc,
                                     bm_uint32 num_counters, const bm_uint64 * counter_values,
                                     bm_uint32 num_custom_counters, const bm_uint32 * custom_counter_ids, const bm_uint64 * custom_counter_values);

#if BM_CONFIG_MAX_TASK_INFOS > 0
/**
 * @brief   Store a task switch record for a core
 * @param   timestamp       The timestamp the settings were configured
 * @param   core            The core number the settings were for
 * @param   task_id         The new task ID
 * @param   reason          The reason for the task switch
 * @return  BM_TRUE on success, BM_FALSE on failure
 */
bm_bool barman_protocol_write_task_switch(bm_uint64 timestamp, bm_uint32 core, bm_task_id_t task_id, bm_uint8 reason);
#endif

#if BM_CONFIG_MIN_SAMPLE_PERIOD > 0
/**
 * @brief   Gets the minimum sample period
 * @return  bm_uint64 The minimum sample period in the same unit returned by {@link bm_ext_get_timestamp}
 */
bm_uint64 barman_protocol_get_minimum_sample_period(void);
#endif

#if BM_NUM_CUSTOM_COUNTERS > 0
/**
 * @brief   Store a custom counter value record
 * @param   counter_id  The index of the custom counter
 * @param   timestamp   The timestamp of the event
 * @param   core        The core number the sample was taken on
 * @param   value       The counter value
 */
bm_bool barman_protocol_write_per_core_custom_counter(bm_uint64 timestamp, bm_uint32 core,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                                      bm_task_id_t task_id,
#endif
                                                      bm_uint32 counter_id, bm_uint64 value);
#endif

/**
 * @brief   Write a WFI/WFE halting event record
 * @param   timestamp       The timestamp of the event
 * @param   core            The core number the sample was taken on
 * @param   entered_halt    True if the record represents the time the processor entered the halt state,
 *                          False if the record represents the time the processor exited the halt state
 * @return  BM_TRUE on success, BM_FALSE on failure
 */
bm_bool barman_protocol_write_halt_event(bm_uint64 timestamp, bm_uint32 core, bm_bool entered_halt);

/**
 * @brief   Write an annotation_record
 * @param   timestamp       The timestamp of the annotation
 * @param   core            The core (ignored by record but needed for datastore operation)
 * @param   task_id         The task ID associated with the annotation, or zero if not associated with any task
 * @param   type            The annotation type
 * @param   channel         The channel number
 * @param   group           The group number
 * @param   color           The color
 * @param   data_length     The length of the annotation data
 * @param   data            The annotation data
 * @return  BM_TRUE on success, BM_FALSE on failure
 */
bm_bool barman_protocol_write_annotation(bm_uint64 timestamp, bm_uint32 core,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                         bm_task_id_t task_id,
#endif
                                         bm_uint8 type, bm_uint32 channel, bm_uint32 group, bm_uint32 color, bm_uintptr data_length, const bm_uint8 * data);

/** @} */

/* ------------------------------------------------------------------------- */

bm_bool barman_generated_initialize(void)
{
    /* Cortex-A9 */
    {
        const bm_uint32 pmu_event_types[6] = {104, 112, 113, 114, 115, 116};

        if (!barman_initialize_pmu_family(0x4100c090, 6, pmu_event_types, BM_NULL)) {
            return BM_FALSE;
        }
    }

    return BM_TRUE;
}



/* ------------------------------------------------------------------------- */

#if defined(__GNUC__)

#endif

/**
 * @defgroup    bm_atomics  Atomic operations
 * @{
 *
 * @def     BM_ATOMIC_TYPE
 * @brief   Marks some type as being atomic as if the C11 _Atomic modifier were applied
 *
 * @def     BM_ATOMIC_VAR_INIT
 * @brief   Defined to match the C11 expression `ATOMIC_VAR_INIT(var)`
 *
 * @def     barman_atomic_init
 * @brief   Defined to match the C11 expression `atomic_init(pointer)`
 *
 * @def     barman_atomic_is_lock_free
 * @brief   Defined to match the C11 expression `atomic_is_lock_free(pointer)`
 *
 * @def     barman_atomic_store
 * @brief   Defined to match the C11 expression `atomic_store_explicit(pointer, val, memory_order_release)`
 *
 * @def     barman_atomic_load
 * @brief   Defined to match the C11 expression `atomic_load_explicit(pointer, memory_order_acquire)`
 *
 * @def     barman_atomic_exchange
 * @brief   Defined to match the C11 expression `atomic_exchange_explicit(pointer, val, memory_order_acq_rel)`
 *
 * @def     barman_atomic_cmp_ex_strong_value
 * @brief   Defined to match the C11 expression `atomic_compare_exchange_strong_explicit(pointer, oldval, newval, memory_order_acq_rel, memory_order_relaxed)`
 *          except that the second parameter (`oldval`) is a value rather than a pointer to a value
 *
 * @def     barman_atomic_cmp_ex_weak_value
 * @brief   Defined to match the C11 expression `atomic_compare_exchange_weak_explicit(pointer, oldval, newval, memory_order_acq_rel, memory_order_relaxed)`
 *          except that the second parameter (`oldval`) is a value rather than a pointer to a value
 *
 * @def     barman_atomic_cmp_ex_strong_pointer
 * @brief   Defined to match the C11 expression `atomic_compare_exchange_strong_explicit(pointer, expected, newval, memory_order_acq_rel, memory_order_relaxed)`
 *
 * @def     barman_atomic_cmp_ex_weak_pointer
 * @brief   Defined to match the C11 expression `atomic_compare_exchange_weak_explicit(pointer, expected, newval, memory_order_acq_rel, memory_order_relaxed)`
 *
 * @def     barman_atomic_fetch_add
 * @brief   Defined to match the C11 expression `atomic_fetch_add_explicit(pointer, val, memory_order_acq_rel)`
 *
 * @def     barman_atomic_fetch_sub
 * @brief   Defined to match the C11 expression `atomic_fetch_sub_explicit(pointer, val, memory_order_acq_rel)`
 *
 * @def     barman_atomic_fetch_and
 * @brief   Defined to match the C11 expression `atomic_fetch_and_explicit(pointer, val, memory_order_acq_rel)`
 *
 * @def     barman_atomic_fetch_or
 * @brief   Defined to match the C11 expression `atomic_fetch_or_explicit(pointer, val, memory_order_acq_rel)`
 *
 * @def     barman_atomic_fetch_xor
 * @brief   Defined to match the C11 expression `atomic_fetch_xor_explicit(pointer, val, memory_order_acq_rel)`
 *
 * @def     barman_atomic_add_fetch
 * @brief   Defined to match the C11 expression `atomic_fetch_add_explicit(pointer, val, memory_order_acq_rel) + val`
 *
 * @def     barman_atomic_sub_fetch
 * @brief   Defined to match the C11 expression `atomic_fetch_sub_explicit(pointer, val, memory_order_acq_rel) - val`
 *
 * @def     barman_atomic_and_fetch
 * @brief   Defined to match the C11 expression `atomic_fetch_and_explicit(pointer, val, memory_order_acq_rel) & val`
 *
 * @def     barman_atomic_or_fetch
 * @brief   Defined to match the C11 expression `atomic_fetch_or_explicit(pointer, val, memory_order_acq_rel) | val`
 *
 * @def     barman_atomic_xor_fetch
 * @brief   Defined to match the C11 expression `atomic_fetch_xor_explicit(pointer, val, memory_order_acq_rel) ^ val`
 */

#if (BM_COMPILER_AT_LEAST_GNUC(4, 7, 0) && !BM_COMPILER_IS_ARMCC) || BM_COMPILER_AT_LEAST_CLANG(3, 4, 0)

#define BM_ATOMIC_TYPE(type)                    type
#define BM_ATOMIC_VAR_INIT(val)                 (val)
#define barman_atomic_init(pointer, val)        __atomic_store_n((pointer), (val), __ATOMIC_RELAXED)
#define barman_atomic_is_lock_free(pointer)                         \
    (__extension__                                                  \
    ({  __typeof(pointer) _bma_tmp_pointer_ = (pointer);            \
        __atomic_always_lock_free(sizeof(*_bma_tmp_pointer_),       \
                                  _bma_tmp_pointer_);               \
    }))
#define barman_atomic_store(pointer, val)       __atomic_store_n((pointer), (val), __ATOMIC_RELEASE)
#define barman_atomic_load(pointer)             __atomic_load_n((pointer), __ATOMIC_ACQUIRE)
#define barman_atomic_exchange(pointer, val)    __atomic_exchange_n((pointer), (val), __ATOMIC_ACQ_REL)
#define barman_atomic_cmp_ex_strong_value(pointer, oldval, newval)  \
    (__extension__                                                  \
    ({  __typeof(pointer) _bma_tmp_pointer_ = (pointer);            \
        __typeof(*_bma_tmp_pointer_) _bma_tmp_oldval_ = (oldval);   \
        __atomic_compare_exchange_n(_bma_tmp_pointer_,              \
                                    &_bma_tmp_oldval_,              \
                                    (newval),                       \
                                    BM_FALSE,                       \
                                    __ATOMIC_ACQ_REL,               \
                                    __ATOMIC_RELAXED); }))
#define barman_atomic_cmp_ex_weak_value(pointer, oldval, newval)    \
    (__extension__                                                  \
    ({  __typeof(pointer) _bma_tmp_pointer_ = (pointer);            \
        __typeof(*_bma_tmp_pointer_) _bma_tmp_oldval_ = (oldval);   \
        __atomic_compare_exchange_n(_bma_tmp_pointer_,              \
                                    &_bma_tmp_oldval_,              \
                                    (newval),                       \
                                    BM_TRUE,                        \
                                    __ATOMIC_ACQ_REL,               \
                                    __ATOMIC_RELAXED); }))
#define barman_atomic_cmp_ex_strong_pointer(pointer, expected, newval)  \
        __atomic_compare_exchange_n((pointer), (expected), (newval), BM_FALSE, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED)
#define barman_atomic_cmp_ex_weak_pointer(pointer, expected, newval)    \
        __atomic_compare_exchange_n((pointer), (expected), (newval), BM_TRUE, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED)
#define barman_atomic_fetch_add(pointer, val)   __atomic_fetch_add((pointer), (val), __ATOMIC_ACQ_REL)
#define barman_atomic_fetch_sub(pointer, val)   __atomic_fetch_sub((pointer), (val), __ATOMIC_ACQ_REL)
#define barman_atomic_fetch_and(pointer, val)   __atomic_fetch_and((pointer), (val), __ATOMIC_ACQ_REL)
#define barman_atomic_fetch_or(pointer, val)    __atomic_fetch_or((pointer), (val), __ATOMIC_ACQ_REL)
#define barman_atomic_fetch_xor(pointer, val)   __atomic_fetch_xor((pointer), (val), __ATOMIC_ACQ_REL)
#define barman_atomic_add_fetch(pointer, val)   __atomic_add_fetch((pointer), (val), __ATOMIC_ACQ_REL)
#define barman_atomic_sub_fetch(pointer, val)   __atomic_sub_fetch((pointer), (val), __ATOMIC_ACQ_REL)
#define barman_atomic_and_fetch(pointer, val)   __atomic_and_fetch((pointer), (val), __ATOMIC_ACQ_REL)
#define barman_atomic_or_fetch(pointer, val)    __atomic_or_fetch((pointer), (val), __ATOMIC_ACQ_REL)
#define barman_atomic_xor_fetch(pointer, val)   __atomic_xor_fetch((pointer), (val), __ATOMIC_ACQ_REL)

#elif (BM_COMPILER_AT_LEAST_GNUC(4, 4, 0) || BM_COMPILER_AT_LEAST_ARMCC(5, 0, 0))

#if (BM_ARM_TARGET_ARCH > 6)
#define BM_ATOMIC_TYPE(type)                    type __attribute__((aligned(sizeof(type))))
#else
#define BM_ATOMIC_TYPE(type)                    type __attribute__((aligned(sizeof(void *))))
#endif

#define BM_ATOMIC_VAR_INIT(val)                 (val)
#define barman_atomic_init(pointer, val)        do { *(pointer) = (val); } while (0)
#define barman_atomic_is_lock_free(pointer)                                         \
    (__extension__                                                                  \
    (((BM_ARM_TARGET_ARCH > 6) && (__alignof__(*(pointer)) == sizeof(*(pointer))))  \
     || ((BM_ARM_TARGET_ARCH == 6) && (__alignof__(*(pointer)) == 4))))
#define barman_atomic_store(pointer, val)       do { __sync_synchronize(); *(pointer) = (val); } while (0)
#define barman_atomic_load(pointer)                         \
    (__extension__                                          \
    ({                                                      \
        __typeof(*(pointer)) _bma_tmp_val_ = *(pointer);    \
        __sync_synchronize();                               \
        (_bma_tmp_val_);                                    \
    }))
#define barman_atomic_exchange(pointer, val)                                            \
    (__extension__                                                                      \
    ({                                                                                  \
        volatile __typeof(pointer) _bma_tmp_pointer_ = (pointer);                       \
        __sync_synchronize();                                                           \
        __typeof(*_bma_tmp_pointer_) _bma_tmp_val_ = *_bma_tmp_pointer_;                \
        while (!__sync_bool_compare_and_swap(_bma_tmp_pointer_, _bma_tmp_val_, (val)))  \
        {                                                                               \
            _bma_tmp_val_ = *_bma_tmp_pointer_;                                         \
        }                                                                               \
        (_bma_tmp_val_);                                                                \
    }))
#define barman_atomic_cmp_ex_strong_value(pointer, oldval, newval)  \
    __sync_bool_compare_and_swap((pointer), (oldval), (newval))
#define barman_atomic_cmp_ex_weak_value(pointer, oldval, newval)    \
    barman_atomic_cmp_ex_strong_value((pointer), (oldval), (newval))
#define barman_atomic_cmp_ex_strong_pointer(pointer, expected, newval)                                                      \
    (__extension__                                                                                                          \
    ({                                                                                                                      \
        __typeof(*(pointer)) _bma_tmp_expected_val_ = *(expected);                                                          \
        __typeof(*(pointer)) _bma_tmp_result_ = __sync_val_compare_and_swap((pointer), _bma_tmp_expected_val_, (newval));   \
        *(expected) = _bma_tmp_result_;                                                                                     \
        (_bma_tmp_expected_val_ == _bma_tmp_result_);                                                                       \
    }))
#define barman_atomic_cmp_ex_weak_pointer(pointer, expected, newval)        \
    barman_atomic_cmp_ex_strong_pointer((pointer), (expected), (newval))
#define barman_atomic_fetch_add(pointer, val)   __sync_fetch_and_add((pointer), (val))
#define barman_atomic_fetch_sub(pointer, val)   __sync_fetch_and_sub((pointer), (val))
#define barman_atomic_fetch_and(pointer, val)   __sync_fetch_and_and((pointer), (val))
#define barman_atomic_fetch_or(pointer, val)    __sync_fetch_and_or((pointer), (val))
#define barman_atomic_fetch_xor(pointer, val)   __sync_fetch_and_xor((pointer), (val))
#define barman_atomic_add_fetch(pointer, val)   __sync_add_and_fetch((pointer), (val))
#define barman_atomic_sub_fetch(pointer, val)   __sync_sub_and_fetch((pointer), (val))
#define barman_atomic_and_fetch(pointer, val)   __sync_and_and_fetch((pointer), (val))
#define barman_atomic_or_fetch(pointer, val)    __sync_or_and_fetch((pointer), (val))
#define barman_atomic_xor_fetch(pointer, val)   __sync_xor_and_fetch((pointer), (val))

#else

#error "Unsupported compiler version. Atomic operations not implemented."

#define BM_ATOMIC_TYPE(type)
#define BM_ATOMIC_VAR_INIT(val)
#define barman_atomic_init(pointer, val)
#define barman_atomic_is_lock_free(pointer)
#define barman_atomic_store(pointer, val)
#define barman_atomic_load(pointer)
#define barman_atomic_exchange(pointer, val)
#define barman_atomic_cmp_ex_strong_value(pointer, oldval, newval)
#define barman_atomic_cmp_ex_weak_value(pointer, oldval, newval)
#define barman_atomic_cmp_ex_strong_pointer(pointer, expected, newval)
#define barman_atomic_cmp_ex_weak_pointer(pointer, expected, newval)
#define barman_atomic_fetch_add(pointer, val)
#define barman_atomic_fetch_sub(pointer, val)
#define barman_atomic_fetch_and(pointer, val)
#define barman_atomic_fetch_or(pointer, val)
#define barman_atomic_fetch_xor(pointer, val)
#define barman_atomic_add_fetch(pointer, val)
#define barman_atomic_sub_fetch(pointer, val)
#define barman_atomic_and_fetch(pointer, val)
#define barman_atomic_or_fetch(pointer, val)
#define barman_atomic_xor_fetch(pointer, val)

#endif

/* Atomic types */
typedef BM_ATOMIC_TYPE(bm_bool) bm_atomic_bool;         /**< Atomic bm_bool */
typedef BM_ATOMIC_TYPE(bm_int8) bm_atomic_int8;         /**< Atomic bm_int8 */
typedef BM_ATOMIC_TYPE(bm_int16) bm_atomic_int16;       /**< Atomic bm_int16 */
typedef BM_ATOMIC_TYPE(bm_int32) bm_atomic_int32;       /**< Atomic bm_int32 */
typedef BM_ATOMIC_TYPE(bm_int64) bm_atomic_int64;       /**< Atomic bm_int64 */
typedef BM_ATOMIC_TYPE(bm_intptr) bm_atomic_intptr;     /**< Atomic bm_intptr */
typedef BM_ATOMIC_TYPE(bm_uint8) bm_atomic_uint8;       /**< Atomic bm_uint8 */
typedef BM_ATOMIC_TYPE(bm_uint16) bm_atomic_uint16;     /**< Atomic bm_uint16 */
typedef BM_ATOMIC_TYPE(bm_uint32) bm_atomic_uint32;     /**< Atomic bm_uint32 */
typedef BM_ATOMIC_TYPE(bm_uint64) bm_atomic_uint64;     /**< Atomic bm_uint64 */
typedef BM_ATOMIC_TYPE(bm_uintptr) bm_atomic_uintptr;   /**< Atomic bm_uintptr */

/** @} */

/**
 * @defgroup    bm_intrinsics   Aliases for intrinsic assembler operations
 * @{
 *
 * @def         barman_isb
 * @brief       Instruction Synchronization Barrier
 * @details     Inserts an "ISB SY" instruction
 *
 * @def         barman_dsb
 * @brief       Data Synchronization Barrier
 * @details     Inserts an "DSB SY" instruction
 *
 * @def         barman_dmb
 * @brief       Data Memory Barrier
 * @details     Inserts an "DMB SY" instruction
 */

#if !BM_ARM_TARGET_ARCH_IS_UNKNOWN()

#define barman_isb()    asm volatile("isb sy")
#define barman_dsb()    asm volatile("dsb sy")
#define barman_dmb()    asm volatile("dmb sy")

#else /* for unit tests */

#define barman_isb()
#define barman_dsb()
#define barman_dmb()

#endif

/**
 * @def         BM_READ_SYS_REG(op1, CRn, CRm, op2, out)
 * @brief       Read from a system register
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRn
 *              Is the CRn parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRn" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       op2
 *              Is the op2/opc2 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op2"/"opc2" field.
 * @param       out Type "bm_uinptr"
 *              L-value to write the result into.
 *
 * @def         BM_WRITE_SYS_REG(op1, CRn, CRm, op2, in)
 * @brief       Writes to a system register
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRn
 *              Is the CRn parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRn" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       op2
 *              Is the op2/opc2 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op2"/"opc2" field.
 * @param       in Type "bm_uinptr"
 *              Value to write into the register.
 *
 * @def         BM_READ_SPECIAL_REG(name, out)
 * @brief       Read from a special purpose register
 * @param       name
 *              Name of the special purpose register to read.
 * @param       out Type "bm_uinptr"
 *              L-value to write the result into.
 *
 * @def         BM_WRITE_SPECIAL_REG(name, in)
 * @brief       Write to a special purpose register
 * @param       name
 *              Name of the special purpose register to write into.
 * @param       in Type "bm_uinptr"
 *              Value to write into the register.
 *
 * @def         BM_SYS_REG_OP0_ENCODING
 * @brief       Usual op0/coproc encoding for system registers
 */

#if BM_ARM_64BIT_STATE
# define    BM_SYS_REG_OP0_ENCODING                   3
#elif BM_ARM_32BIT_STATE

/**
 * @def         BM_READ_SYS_REG_64(op1, CRm, out)
 * @brief       Read from a system register
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       out Type "bm_uint64"
 *              L-value to write the result into.
 *
 * @def         BM_WRITE_SYS_REG_64(op1, CRm, in)
 * @brief       Writes to a system register
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       in Type "bm_uint64"
 *              Value to write into the register.
 */
# define    BM_SYS_REG_OP0_ENCODING                   15
# define    BM_READ_SYS_REG_64(op1, CRm, out)                                                   \
                do {                                                                            \
                    bm_uint32 _bm_mrrc_lower;                                                   \
                    bm_uint32 _bm_mrrc_upper;                                                   \
                    BM_MRRC(BM_SYS_REG_OP0_ENCODING, op1, CRm, _bm_mrrc_lower, _bm_mrrc_upper); \
                    out = (((bm_uint64) _bm_mrrc_upper) << 32) | _bm_mrrc_lower;                \
                } while (0)
# define    BM_WRITE_SYS_REG_64(op1, CRm, in)         BM_MCRR(BM_SYS_REG_OP0_ENCODING, op1, CRm, (bm_uint32) in, (bm_uint32) (in >> 32))
#else
# define    BM_SYS_REG_OP0_ENCODING                   ERROR_NOT_ARM
#endif

#define     BM_READ_SYS_REG(op1, CRn, CRm, op2, out)  BM_MRC(BM_SYS_REG_OP0_ENCODING, op1, CRn, CRm, op2, out)
#define     BM_WRITE_SYS_REG(op1, CRn, CRm, op2, in)  BM_MCR(BM_SYS_REG_OP0_ENCODING, op1, CRn, CRm, op2, in)
#define     BM_READ_SPECIAL_REG(name, out)            BM_MRS(name, out)
#define     BM_WRITE_SPECIAL_REG(name, in)            BM_MSR(name, in)

#if BM_ARM_64BIT_STATE || BM_COMPILER_IS_ARMCC
/* AArch64 and (AArch32 with ARMASM psuedo instruction) allow accessing system registers by name */

/**
 * @def         BM_READ_SYS_REG_NAMED(name, out)
 * @brief       Read from a system register by name
 * @param       name
 *              Name of the system register to read.
 * @param       out Type "bm_uinptr"
 *              L-value to write the result into.
 *
 * @def         BM_WRITE_SYS_REG_NAMED(name, in)
 * @brief       Write to a system register by name
 * @param       name
 *              Name of the system register to write into.
 * @param       in Type "bm_uinptr"
 *              Value to write into the register.
 */
#define     BM_READ_SYS_REG_NAMED(name, out)          BM_MRS(name, out)
#define     BM_WRITE_SYS_REG_NAMED(name, in)          BM_MSR(name, in)
#endif

/**
 * @def         BM_MRC(op0, op1, CRn, CRm, op2, Rt)
 * @brief       MRC instruction (or manually encoded MRS on AArch64)
 * @param       op0
 *              Is the op1/coproc parameter within the System register encoding space,
 *              in the range 0 to 7 or 8 to 15, respectively, encoded in the "op1"/"coproc" field.
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRn
 *              Is the CRn parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRn" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       op2
 *              Is the op2/opc2 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op2"/"opc2" field.
 * @param       Rt Type "bm_uinptr"
 *              L-value to write the result into.
 *
 * @def         BM_MCR(op0, op1, CRn, CRm, op2, Rt)
 * @brief       MCR instruction (or manually encoded MSR on AArch64)
 * @param       op0
 *              Is the op1/coproc parameter within the System register encoding space,
 *              in the range 0 to 7 or 8 to 15, respectively, encoded in the "op1"/"coproc" field.
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRn
 *              Is the CRn parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRn" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       op2
 *              Is the op2/opc2 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op2"/"opc2" field.
 * @param       Rt Type "bm_uinptr"
 *              Value to write into the register.
 *
 * @def         BM_MRS(name, Rt)
 * @brief       MRS instruction
 * @param       name
 *              Name of the special purpose register to read.
 * @param       Rt Type "bm_uinptr"
 *              L-value to write the result into.
 *
 * @def         BM_MSR(name, Rt)
 * @brief       MSR instruction
 * @param       name
 *              Name of the special purpose register to write into.
 * @param       in Type "bm_uinptr"
 *              Value to write into the register.
 */

#if BM_ARM_64BIT_STATE
# define    BM_MRC(op0, op1, CRn, CRm, op2, Rt)    asm volatile ("mrs %0, S" BM_STRINGIFY_VAL(op0) "_" BM_STRINGIFY_VAL(op1) "_C" BM_STRINGIFY_VAL(CRn) "_C" BM_STRINGIFY_VAL(CRm) "_" BM_STRINGIFY_VAL(op2) : "=r"(Rt))
# define    BM_MCR(op0, op1, CRn, CRm, op2, Rt)    asm volatile ("msr S" BM_STRINGIFY_VAL(op0) "_" BM_STRINGIFY_VAL(op1) "_C" BM_STRINGIFY_VAL(CRn) "_C" BM_STRINGIFY_VAL(CRm) "_" BM_STRINGIFY_VAL(op2) ", %0" :: "r"(Rt))
# define    BM_MRS(name, Rt)                       asm volatile ("mrs %0, " BM_STRINGIFY_VAL(name) : "=r"(Rt))
# define    BM_MSR(name, Rt)                       asm volatile ("msr " BM_STRINGIFY_VAL(name) ", %0" :: "r"(Rt))
#elif BM_ARM_32BIT_STATE

/**
 * @def         BM_MRRC(op0, op1, CRm, Rt)
 * @brief       MRRC instruction
 * @param       op0
 *              Is the System register encoding space,
 *              in the range 8 to 15, encoded in the "op1"/"coproc" field.
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       Rt Type "bm_uint32"
 *              L-value to write the first result into.
 * @param       Rt2 Type "bm_uint32"
 *              L-value to write the second result into.
 *
 * @def         BM_MCRR(op0, op1, CRm, Rt)
 * @brief       MCRR instruction
 * @param       op0
 *              Is the System register encoding space,
 *              in the range 8 to 15, encoded in the "op1"/"coproc" field.
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       Rt Type "bm_uint32"
 *              First value to write into the register.
 * @param       Rt2 Type "bm_uint32"
 *              Second value to write into the register.
 */
# if BM_COMPILER_IS_ARMCC
#   define  BM_MRC(op0, op1, CRn, CRm, op2, Rt)    __asm ("mrc p" BM_STRINGIFY_VAL(op0) ", " BM_STRINGIFY_VAL(op1) ", " BM_STRINGIFY_VAL(Rt) ", c" BM_STRINGIFY_VAL(CRn) ", c" BM_STRINGIFY_VAL(CRm) ", " BM_STRINGIFY_VAL(op2))
#   define  BM_MCR(op0, op1, CRn, CRm, op2, Rt)    __asm ("mcr p" BM_STRINGIFY_VAL(op0) ", " BM_STRINGIFY_VAL(op1) ", " BM_STRINGIFY_VAL(Rt) ", c" BM_STRINGIFY_VAL(CRn) ", c" BM_STRINGIFY_VAL(CRm) ", " BM_STRINGIFY_VAL(op2))
#   define  BM_MRRC(op0, op1, CRm, Rt, Rt2)        __asm ("mrrc p" BM_STRINGIFY_VAL(op0) ", " BM_STRINGIFY_VAL(op1) ", " BM_STRINGIFY_VAL(Rt) ", " BM_STRINGIFY_VAL(Rt2) ", c" BM_STRINGIFY_VAL(CRm))
#   define  BM_MCRR(op0, op1, CRm, Rt, Rt2)        __asm ("mcrr p" BM_STRINGIFY_VAL(op0) ", " BM_STRINGIFY_VAL(op1) ", " BM_STRINGIFY_VAL(Rt) ", " BM_STRINGIFY_VAL(Rt2) ", c" BM_STRINGIFY_VAL(CRm))
#   define  BM_MRS(name, Rt)                       __asm ("mrs " BM_STRINGIFY_VAL(Rt) ", " BM_STRINGIFY_VAL(name))
#   define  BM_MSR(name, Rt)                       __asm ("msr " BM_STRINGIFY_VAL(name) ", " BM_STRINGIFY_VAL(Rt))
# else
#   define  BM_MRC(op0, op1, CRn, CRm, op2, Rt)    asm volatile ("mrc p" BM_STRINGIFY_VAL(op0) ", " BM_STRINGIFY_VAL(op1) ", %0, c" BM_STRINGIFY_VAL(CRn) ", c" BM_STRINGIFY_VAL(CRm) ", " BM_STRINGIFY_VAL(op2) : "=r"(Rt))
#   define  BM_MCR(op0, op1, CRn, CRm, op2, Rt)    asm volatile ("mcr p" BM_STRINGIFY_VAL(op0) ", " BM_STRINGIFY_VAL(op1) ", %0, c" BM_STRINGIFY_VAL(CRn) ", c" BM_STRINGIFY_VAL(CRm) ", " BM_STRINGIFY_VAL(op2) :: "r"(Rt))
#   define  BM_MRRC(op0, op1, CRm, Rt, Rt2)        asm volatile ("mrrc p" BM_STRINGIFY_VAL(op0) ", " BM_STRINGIFY_VAL(op1) ", %0, %1, c" BM_STRINGIFY_VAL(CRm) : "=r"(Rt), "=r"(Rt2))
#   define  BM_MCRR(op0, op1, CRm, Rt, Rt2)        asm volatile ("mcrr p" BM_STRINGIFY_VAL(op0) ", " BM_STRINGIFY_VAL(op1) ", %0, %1, c" BM_STRINGIFY_VAL(CRm) :: "r"(Rt), "r"(Rt2))
#   define  BM_MRS(name, Rt)                       asm volatile ("mrs %0, " BM_STRINGIFY_VAL(name) : "=r"(Rt))
#   define  BM_MSR(name, Rt)                       asm volatile ("msr " BM_STRINGIFY_VAL(name) ", %0" :: "r"(Rt))
# endif
#else
# define    BM_MRC(op0, op1, CRn, CRm, op2, Rt)    Rt = 0
# define    BM_MCR(op0, op1, CRn, CRm, op2, Rt)    (void) (Rt)
# define    BM_MRS(name, Rt)                       Rt = 0
# define    BM_MSR(name, Rt)                       (void) (Rt)
#endif

/** @} */

#if BM_ARM_ARCH_PROFILE == 'M'

#define BM_ITM_BASE_ADDRESS    ((void *) 0xE0000000)
#define BM_DWT_BASE_ADDRESS    ((void *) 0xE0001000)
#define BM_CPUID_ADDRESS       ((void *) 0xE000ED00)

#endif

/**
 * @defgroup    bm_public_functions Various public functions
 * @{
 */

/**
 * @brief   Read the MIDR value
 * @return  The MIDR value
 */
static BM_INLINE bm_uint32 barman_midr(void)
{
#if BM_ARM_ARCH_PROFILE == 'M'
    return BM_MEMORY_MAPPED_REGISTER_32(BM_CPUID_ADDRESS);
#else
    bm_uintptr val;
    BM_READ_SYS_REG(0, 0, 0, 0, val); /* MIDR */
    return val;
#endif
}

/**
 * @brief   Read the MPIDR value
 * @return  The MPIDR value
 */
static BM_INLINE bm_uintptr barman_mpidr(void)
{
#if BM_ARM_ARCH_PROFILE == 'M'
#if BM_CONFIG_MAX_CORES == 1
    return 0;
#else
#error "Multi core M profile not supported"
#endif
#else
    bm_uintptr val;
    BM_READ_SYS_REG(0, 0, 0, 5, val); /* MPIDR */
    return val;
#endif
}

/** @} */

#if (defined(__GNUC__) || defined(__clang__) || defined(__ARMCC_VERSION))

#endif

/* Helpers to enable or disable logging, but retain validation by compiler */
/** @{ */
#if BM_CONFIG_ENABLE_LOGGING
#define BM_LOGGING_LINKAGE          extern
#define BM_LOGGING_BODY             ;
#else
#define BM_LOGGING_LINKAGE          static BM_ALWAYS_INLINE
#define BM_LOGGING_BODY             {}
#endif

#if BM_CONFIG_ENABLE_DEBUG_LOGGING
#define BM_DEBUG_LOGGING_LINKAGE    extern
#define BM_DEBUG_LOGGING_BODY       ;
#else
#define BM_DEBUG_LOGGING_LINKAGE    static BM_ALWAYS_INLINE
#define BM_DEBUG_LOGGING_BODY       {}
#endif

/** @} */

/**
 * @defgroup    bm_log  Debugging facilities
 * @{ */

/**
 * @brief   Print a debug message
 * @param   message
 */
BM_DEBUG_LOGGING_LINKAGE
BM_FORMAT_FUNCTION(printf, 1, 2)
void barman_ext_log_debug(const char * message, ...) BM_DEBUG_LOGGING_BODY

/**
 * @brief   Print an info message
 * @param   message
 */
BM_LOGGING_LINKAGE
BM_FORMAT_FUNCTION(printf, 1, 2)
void barman_ext_log_info(const char * message, ...) BM_LOGGING_BODY

/**
 * @brief   Print a warning message
 * @param   message
 */
BM_LOGGING_LINKAGE
BM_FORMAT_FUNCTION(printf, 1, 2)
void barman_ext_log_warning(const char * message, ...) BM_LOGGING_BODY

/**
 * @brief   Print an error message
 * @param   message
 */
BM_LOGGING_LINKAGE
BM_FORMAT_FUNCTION(printf, 1, 2)
void barman_ext_log_error(const char * message, ...) BM_LOGGING_BODY

/** @brief  Insert a debug message with the file, line number and function name prefixed */
#define BM_DEBUG(message, ...)      barman_ext_log_debug("[%s:%u - %s] " message, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
/** @brief  Insert an info message with the file, line number and function name prefixed */
#define BM_INFO(message, ...)       barman_ext_log_info("[%s:%u - %s] " message, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
/** @brief  Insert a warning message with the file, line number and function name prefixed */
#define BM_WARNING(message, ...)    barman_ext_log_warning("[%s:%u - %s] " message, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
/** @brief  Insert an error message with the file, line number and function name prefixed */
#define BM_ERROR(message, ...)      barman_ext_log_error("[%s:%u - %s] " message, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

/** @} */

#undef BM_LOGGING_LINKAGE
#undef BM_LOGGING_BODY
#undef BM_DEBUG_LOGGING_LINKAGE
#undef BM_DEBUG_LOGGING_BODY

/**
 * @defgroup    bm_multicore    Multicore related functions
 * @{ */

/**
 * @brief   Detects the core number for the current core.
 * @details Returns a number which uniquely identifies the current core. The
 *          value returned will be between `[0, N)` where `N` is the number of
 *          cores on the system.
 * @note    The value returned may not be in the range `[0, BM_CONFIG_MAX_CORES)`
 *          and so any code using the value returned here must bounds check the
 *          result if they are expecting a value within this range.
 * @return  The core number
 */
bm_uint32 barman_get_core_no(void);

/** @} */

/**
 * @defgroup    bm_arm_pmu    Arm PMU driver
 * @{ */

/** The maximum number of counters supported */
#define BM_ARM_PMU_MAX_PMU_COUNTERS      32

/** The value returned by {@link barman_arm_pmu_read_counter} if the counter was not read */
#define BM_ARM_PMU_INVALID_COUNTER_VALUE (~0ull)

/**
 * @brief   Initialize the ARMv7 PMU on the current core.
 * @details This function programs the PMU hardware on the current core. The
 *          function must be called once on each core where we are interested in
 *          reading the PMU.
 * @param   enable_pl0_access
 *      Enable reading the counter values from EL0. NB: Setting this bit will enable all registers to be accessible from
 *      user space, not just the counter read registers.
 * @param   enable_interrupts
 *      Enable PMU interrupts
 * @param   enable_cycle_counter
 *      Enable the 64-bit cycle counter
 * @param   n_event_types
 *      The number of additional configurable events to enable
 * @param   event_types
 *      An array of length `n_event_types` containing the event types for the
 *      additional events to enable
 * @return  The number of events that were enabled which will be whichever is
 *      lower from `n_event_types`, or the maximum number of events supported
 *      by the PMU. The number is increased by one if enable_cycle_counter was set.
 */
BM_NONNULL((5))
bm_int32 barman_arm_pmu_init(bm_bool enable_pl0_access, bm_bool enable_interrupts, bm_bool enable_cycle_counter, bm_uint32 n_event_types,
                                 const bm_uint32 * event_types);

/**
 * @brief   Start recording events in the PMU
 */
void barman_arm_pmu_start(void);

/**
 * @brief   Stop recording events in the PMU
 */
void barman_arm_pmu_stop(void);

/**
 * @brief   Read the value of a PMU counter
 * @param   counter_no
 *      The counter number, where 0 is the cycle counter and [1, N] are the user
 *      defined event counters as specified to {@link barman_arm_pmu_init} in
 *      `event_types` parameter.
 * @return  The value of the counter, or {@link BM_INVALID_COUNTER_VALUE}
 *          if the counter was not read. Caller should detect this and skip the counter.
 */
bm_uint64 barman_arm_pmu_read_counter(bm_uint32 counter_no);

/** @} */

/*
 * @brief   Initialize the Arm DWT on the current core.
 * @details This function programs the DWT hardware on the current core. The
 *          function must be called once on each core where we want the DWT to
 *          output counter overflow packets.
 * @param   enable_pc_sampling          Enable PC sampling
 * @param   enable_exception_tracing    Enable exception tracing
 * @param   n_event_types   The number of additional configurable events to enable
 * @param   event_types     An array of length `n_event_types` containing the event types for the
 *                          additional events to enable
 * @param   cycle_counter_overflow  Number of cycles per PC sample or cycle overflow event.
 *                                  Valid values are 64 * i or 1024 * i where i is between
 *                                  1 and 16 inclusive. Other values will be rounded down or up to 64.
 * @return  0 if successful, -1 otherwise
 */
BM_NONNULL((4))
bm_int32 barman_arm_dwt_init(bm_bool enable_pc_sampling,
                             bm_bool enable_exception_tracing,
                             bm_uint32 n_event_types, const bm_uint32 * event_types,
                             bm_uint32 cycle_counter_overflow);

/**
 * @brief   Start recording events in the DWT
 */
void barman_arm_dwt_start(void);

/**
 * @brief   Stop recording events in the DWT
 */
void barman_arm_dwt_stop(void);

/* Select the appropriate PMU device */
/** @{ */
#if BM_CONFIG_USER_SUPPLIED_PMU_DRIVER
#   define barman_pmu_init(ne, et)          barman_ext_init(ne, et)
#   define barman_pmu_start()               barman_ext_start()
#   define barman_pmu_stop()                barman_ext_stop()
#   define barman_pmu_read_counter(n)       barman_ext_read_counter(n)
#   define barman_midr()                    barman_ext_midr()
#   define barman_mpidr()                   barman_ext_mpidr()
#elif BM_ARM_ARCH_PROFILE_IS_AR && BM_ARM_TARGET_ARCH >= 7
#   define BM_MAX_PMU_COUNTERS              BM_ARM_PMU_MAX_PMU_COUNTERS
#   define BM_PMU_INVALID_COUNTER_VALUE     BM_ARM_PMU_INVALID_COUNTER_VALUE
#   define BM_PMU_HAS_FIXED_CYCLE_COUNTER   1
#   define BM_PMU_CYCLE_COUNTER_ID          0
#   define BM_PMU_CYCLE_COUNTER_TYPE        ~0u
#   define barman_pmu_init(ne, et)          barman_arm_pmu_init(BM_TRUE, BM_FALSE, BM_PMU_HAS_FIXED_CYCLE_COUNTER, ne, et)
#   define barman_pmu_start()               barman_arm_pmu_start()
#   define barman_pmu_stop()                barman_arm_pmu_stop()
#   define barman_pmu_read_counter(n)       barman_arm_pmu_read_counter(n)
#elif BM_ARM_ARCH_PROFILE == 'M' && BM_ARM_TARGET_ARCH >= 7
#   define BM_MAX_PMU_COUNTERS              6
#   define BM_PMU_INVALID_COUNTER_VALUE     0
#   define barman_pmu_init(ne, et)          barman_arm_dwt_init(BM_TRUE, BM_TRUE, ne, et, BM_CONFIG_DWT_SAMPLE_PERIOD)
#   define barman_pmu_start()               barman_arm_dwt_start()
#   define barman_pmu_stop()                barman_arm_dwt_stop()
#   define barman_pmu_read_counter(n)       0
#else
#   pragma message ("WARNING: PMU driver not known")
#   define BM_MAX_PMU_COUNTERS              1
#   define BM_PMU_INVALID_COUNTER_VALUE     0
#   define BM_PMU_HAS_FIXED_CYCLE_COUNTER   1
#   define BM_PMU_CYCLE_COUNTER_ID          0
#   define BM_PMU_CYCLE_COUNTER_TYPE        0
#   define barman_pmu_init(ne, et)          0
#   define barman_pmu_start()
#   define barman_pmu_stop()
#   define barman_pmu_read_counter(n)       BM_PMU_INVALID_COUNTER_VALUE
#endif
/** @} */

/* ********************************** */
#if (BM_ARM_ARCH_PROFILE == 'M') && (BM_CONFIG_MIN_SAMPLE_PERIOD > 0)
#error "BM_CONFIG_MIN_SAMPLE_PERIOD is not supported on M profile"
#endif

#if BM_CONFIG_USER_SUPPLIED_PMU_DRIVER
extern bm_uint32 barman_ext_midr(void);
extern bm_uintptr barman_ext_mpidr(void);
#endif

/**
 * @defgroup    bm_public_internals Public function internals
 * @{ */

/** Make sure the size of temporary array is at least one */
#define BM_CUSTOM_COUNTER_ARRAY_SIZE    (BM_NUM_CUSTOM_COUNTERS > 0 ? BM_NUM_CUSTOM_COUNTERS : 1)

/** Mask for bits that are used to construct CPUID values */
#define BM_MIDR_CPUID_MASK 0xff00fff0

/**
 * @brief   Target state for per-core PMU config
 */
enum bm_pmu_target_state
{
    BM_PMU_STATE_UNINITIALIZED = 0, /**< The PMU is not initialized */
    BM_PMU_STATE_SHOULD_START,      /**< The PMU should start on the next sample */
    BM_PMU_STATE_SHOULD_STOP,       /**< The PMU should stop on the next sample */
    BM_PMU_STATE_STARTED,           /**< The PMU is started */
    BM_PMU_STATE_STOPPED            /**< The PMU is stopped */
};

/** Atomic version of {@link bm_pmu_target_state} */
typedef BM_ATOMIC_TYPE(enum bm_pmu_target_state) bm_atomic_pmu_target_state;

/**
 * @brief   PMU initialization state for a PMU
 */
enum bm_pmu_init_state
{
    BM_PMU_INIT_UNINITIALIZED = 0,  /**< The PMU is uninitialized */
    BM_PMU_INIT_INITIALIZING,       /**< The PMU is initializing */
    BM_PMU_INIT_INITIALIZED         /**< The PMU is initialized */
};

/** Atomic version of {@link bm_pmu_init_state} */
typedef BM_ATOMIC_TYPE(enum bm_pmu_init_state) bm_atomic_pmu_init_state;

/** Try change state result */
enum bm_try_change_pmu_result
{
    BM_PMU_STATE_CHANGE_FAILED,     /**< State change failed */
    BM_PMU_STATE_CHANGE_SUCCEEDED,  /**< State change succeeded */
    BM_PMU_STATE_CHANGE_RETRY       /**< State change must be retried */
};

/**
 * @brief   API PMU family settings
 */
struct bm_pmu_family_settings
{
    /** The MIDR to match */
    bm_uint32 midr;
    /** The number of events in event_types */
    bm_uint32 num_events;
    /** The event types to configure */
    bm_uint32 event_types[BM_MAX_PMU_COUNTERS];
    /** The allowed cores bitmap */
    bm_core_set allowed_cores;
};

/**
 * @brief   API per-core settings
 */
struct bm_per_core_settings
{
    /** The number of counters that can be polled */
    bm_uint32 num_counters;
    /** The init state of the PMU */
    bm_atomic_pmu_init_state init_state;
    /** The state of the core */
    bm_atomic_pmu_target_state state;
    /** The PMU family configured for the core */
    bm_uint32 pmu_family;
#if BM_CONFIG_MIN_SAMPLE_PERIOD > 0
    /** The last successful sample timestamp */
    bm_atomic_uint64 last_sample_timestamp;
#endif
};

#if BM_CONFIG_NUM_PMU_TYPES <= 0
#error "Invalid value for BM_CONFIG_NUM_PMU_TYPES"
#endif

/**
 * @brief   API configuration settings
 */
struct bm_settings
{
    /** Number of items stored in pmu_family_settings */
    bm_uint32 num_pmu_family_settings;
    /** The PMU family settings (assumes at most one per core) */
    struct bm_pmu_family_settings pmu_family_settings[BM_CONFIG_NUM_PMU_TYPES];
    /** Per core settings */
    struct bm_per_core_settings per_core_settings[BM_CONFIG_MAX_CORES];
#if BM_CONFIG_MIN_SAMPLE_PERIOD > 0
    /** The minimum_sample_period in the same units as returned by {@link barman_ext_get_timestamp} */
    bm_uint64 minimum_sample_period;
#endif
    /** First 'start' has happened */
    bm_atomic_bool started;
};

/** API settings */
static struct bm_settings bm_settings;

/**
 * @def     BM_GET_RETURN_ADDRESS
 * @brief   Get the return address of a function
 */
#if ((!BM_COMPILER_IS_ARMCC && BM_COMPILER_AT_LEAST_GNUC(4, 5, 0)) || BM_COMPILER_IS_CLANG)
#define BM_GET_RETURN_ADDRESS()         __builtin_extract_return_addr(__builtin_return_address(0))
#elif (BM_COMPILER_IS_GNUC || BM_COMPILER_IS_ARMCC)
#define BM_GET_RETURN_ADDRESS()         __builtin_return_address(0)
#else
#pragma message ("WARNING: BM_GET_RETURN_ADDRESS() was not defined for this compiler")
#define BM_GET_RETURN_ADDRESS()         BM_NULL
#endif

/* ********************************** */

/**
 * @brief   Initialize a core's PMU
 * @param   core    The core to initialize
 * @return  BM_TRUE if successful, BM_FALSE otherwise
 */
static BM_INLINE enum bm_try_change_pmu_result barman_initialize_pmu(bm_uint32 core)
{
    const bm_uint32 midr = barman_midr();
    const bm_uintptr mpidr = barman_mpidr();
    bm_uint32 index, num_counters;
    bm_int32 num_counters_setup;
    bm_bool found_match = BM_FALSE;
    enum bm_pmu_init_state init_state;

#if BM_PMU_HAS_FIXED_CYCLE_COUNTER
    bm_uint32 counter_types[BM_MAX_PMU_COUNTERS] = {0};
    bm_uint32 counter;
#endif

#if BM_MAX_PMU_COUNTERS <= 0
#error "BM_MAX_PMU_COUNTERS is invalid"
#endif

    /* examine current state; do not initialize if busy or if already initialized */
    init_state = barman_atomic_load(&bm_settings.per_core_settings[core].init_state);
    do {
        switch (init_state)
        {
            case BM_PMU_INIT_UNINITIALIZED: {
                break;
            }
            case BM_PMU_INIT_INITIALIZING: {
                return BM_PMU_STATE_CHANGE_RETRY;
            }
            case BM_PMU_INIT_INITIALIZED: {
                return BM_PMU_STATE_CHANGE_SUCCEEDED;
            }
            default: {
                BM_DEBUG("Unexpected value for init_state: %i\n", init_state);
                return BM_PMU_STATE_CHANGE_FAILED;
            }
        }
    } while (barman_atomic_cmp_ex_weak_pointer(&bm_settings.per_core_settings[core].init_state, &init_state, BM_PMU_INIT_INITIALIZING));

    /* find the best matched configuration */
    for (index = 0; index < bm_settings.num_pmu_family_settings; ++index) {
        found_match = ((bm_settings.pmu_family_settings[index].midr & BM_MIDR_CPUID_MASK) == (midr & BM_MIDR_CPUID_MASK)) && barman_core_set_is_set(bm_settings.pmu_family_settings[index].allowed_cores, core);
        if (found_match) {
            BM_DEBUG("Found matching PMU settings for processor (midr=0x%x, no=%u): #%u\n", midr, core, index);
            break;
        }
    }

    if (!found_match) {
        BM_ERROR("Unable to initialize PMU for processor (midr=0x%x, no=%u), no matching PMU family settings\n", midr, core);
        barman_atomic_store(&bm_settings.per_core_settings[core].init_state, BM_PMU_INIT_UNINITIALIZED);
        return BM_PMU_STATE_CHANGE_FAILED;
    }

    /* save the pmu family */
    bm_settings.per_core_settings[core].pmu_family = index;

    /* init the pmu */
    num_counters_setup = barman_pmu_init(bm_settings.pmu_family_settings[index].num_events, bm_settings.pmu_family_settings[index].event_types);

    /* validate number of counters */
    if (num_counters_setup < 0) {
        BM_ERROR("Unable to initialize PMU for processor (midr=0x%x, no=%u)\n", midr, core);
        barman_atomic_store(&bm_settings.per_core_settings[core].init_state, BM_PMU_INIT_UNINITIALIZED);
        return BM_PMU_STATE_CHANGE_FAILED;
    }

    /* limit number of counters and store */
    num_counters = BM_MIN((bm_uint32) num_counters_setup, BM_MAX_PMU_COUNTERS);
    bm_settings.per_core_settings[core].num_counters = num_counters;

    /* store the events in the datastore */
#if BM_PMU_HAS_FIXED_CYCLE_COUNTER

    /* insert cycle counter into types map */
    for (counter = 0; counter < num_counters; ++counter) {
        if (BM_PMU_CYCLE_COUNTER_ID == counter) {
            counter_types[counter] = BM_PMU_CYCLE_COUNTER_TYPE;
        }
        else {
            counter_types[counter] = bm_settings.pmu_family_settings[index].event_types[(counter > BM_PMU_CYCLE_COUNTER_ID ? counter - 1 : counter)];
        }
    }

    /* send */
    if (!barman_protocol_write_pmu_settings(barman_ext_get_timestamp(), midr, mpidr, core, num_counters, counter_types)) {
        BM_ERROR("Unable to initialize PMU for processor (midr=0x%x, no=%u), could not store PMU settings\n", midr, core);
        barman_atomic_store(&bm_settings.per_core_settings[core].init_state, BM_PMU_INIT_UNINITIALIZED);
        return BM_PMU_STATE_CHANGE_FAILED;
    }

#else

    /* send */
    if (!barman_protocol_write_pmu_settings(barman_ext_get_timestamp(), midr, mpidr, core, num_counters, bm_settings.pmu_family_settings[index].event_types)) {
        BM_ERROR("Unable to initialize PMU for processor (midr=0x%x, no=%u), could not store PMU settings\n", midr, core);
        barman_atomic_store(&bm_settings.per_core_settings[core].init_state, BM_PMU_INIT_UNINITIALIZED);
        return BM_PMU_STATE_CHANGE_FAILED;
    }

#endif

#if BM_CONFIG_MIN_SAMPLE_PERIOD > 0
    /* initialize the sample sample rate limit variables */
    bm_settings.minimum_sample_period = barman_protocol_get_minimum_sample_period();
    BM_ATOMIC_VAR_INIT(bm_settings.per_core_settings[core].last_sample_timestamp) = barman_ext_get_timestamp();
#endif

    /* mark initialized */
    BM_INFO("Initialize PMU for processor (midr=0x%x, no=%u) with %u counters\n", midr, core, num_counters);
    barman_atomic_store(&bm_settings.per_core_settings[core].init_state, BM_PMU_INIT_INITIALIZED);
    return BM_PMU_STATE_CHANGE_SUCCEEDED;
}

/**
 * @brief   Try to transition the PMU state for a given core to some new state
 * @param   core            The core number
 * @param   current_state   [IN/OUT] The current state value
 * @param   target_state    The new state
 * @return  BM_TRUE if the state was changed, BM_FALSE if the transition is invalid
 * @note    Allowable transitions are:
 *
 *          `BM_PMU_STATE_UNINITIALIZED     -> BM_PMU_STATE_SHOULD_START, BM_PMU_STATE_SHOULD_STOP`
 *          `BM_PMU_STATE_SHOULD_START      -> BM_PMU_STATE_SHOULD_STOP, BM_PMU_STATE_STARTED, BM_PMU_STATE_STOPPED, BM_PMU_STATE_UNINITIALIZED`
 *          `BM_PMU_STATE_SHOULD_STOP       -> BM_PMU_STATE_SHOULD_START, BM_PMU_STATE_STARTED, BM_PMU_STATE_STOPPED, BM_PMU_STATE_UNINITIALIZED`
 *          `BM_PMU_STATE_STARTED           -> BM_PMU_STATE_SHOULD_STOP, BM_PMU_STATE_STOPPED`
 *          `BM_PMU_STATE_STOPPED           -> BM_PMU_STATE_SHOULD_START, BM_PMU_STATE_STARTED`
 */
BM_NONNULL((2))
static enum bm_try_change_pmu_result barman_try_change_pmu_state(bm_uint32 core, enum bm_pmu_target_state * current_state,
                                                                     enum bm_pmu_target_state target_state)
{
    switch (target_state) {
    case BM_PMU_STATE_UNINITIALIZED: {
        switch (*current_state) {
        case BM_PMU_STATE_UNINITIALIZED: {
            /* ok */
            return BM_PMU_STATE_CHANGE_SUCCEEDED;
        }
        case BM_PMU_STATE_SHOULD_START:
        case BM_PMU_STATE_SHOULD_STOP: {
            /* just transition */
            if (barman_atomic_cmp_ex_strong_pointer(&bm_settings.per_core_settings[core].state, current_state, target_state)) {
                return BM_PMU_STATE_CHANGE_SUCCEEDED;
            }
            /* try again */
            break;
        }
        case BM_PMU_STATE_STOPPED:
        case BM_PMU_STATE_STARTED:
        default: {
            /* invalid transition */
            return BM_PMU_STATE_CHANGE_FAILED;
        }
        }
        break;
    }
    case BM_PMU_STATE_SHOULD_START: {
        switch (*current_state) {
        case BM_PMU_STATE_UNINITIALIZED:
        case BM_PMU_STATE_SHOULD_STOP:
        case BM_PMU_STATE_STOPPED: {
            /* just transition */
            if (barman_atomic_cmp_ex_strong_pointer(&bm_settings.per_core_settings[core].state, current_state, target_state)) {
                return BM_PMU_STATE_CHANGE_SUCCEEDED;
            }
            /* try again */
            break;
        }
        case BM_PMU_STATE_SHOULD_START:
        case BM_PMU_STATE_STARTED: {
            /* no change */
            return BM_PMU_STATE_CHANGE_SUCCEEDED;
        }
        default: {
            /* invalid transition */
            return BM_PMU_STATE_CHANGE_FAILED;
        }
        }
        break;
    }
    case BM_PMU_STATE_SHOULD_STOP: {
        switch (*current_state) {
        case BM_PMU_STATE_SHOULD_START:
        case BM_PMU_STATE_STARTED: {
            /* just transition */
            if (barman_atomic_cmp_ex_strong_pointer(&bm_settings.per_core_settings[core].state, current_state, target_state)) {
                return BM_PMU_STATE_CHANGE_SUCCEEDED;
            }
            /* try again */
            break;
        }
        case BM_PMU_STATE_SHOULD_STOP:
        case BM_PMU_STATE_STOPPED: {
            /* no change */
            return BM_PMU_STATE_CHANGE_SUCCEEDED;
        }
        case BM_PMU_STATE_UNINITIALIZED:
        default: {
            /* invalid transition */
            return BM_PMU_STATE_CHANGE_FAILED;
        }
        }
        break;
    }
    case BM_PMU_STATE_STARTED: {
        switch (*current_state) {
        case BM_PMU_STATE_SHOULD_START:
        case BM_PMU_STATE_STOPPED:
        case BM_PMU_STATE_SHOULD_STOP: {
            /* check initialized */
            const enum bm_pmu_init_state init_state = barman_atomic_load(&bm_settings.per_core_settings[core].init_state);

            if (init_state == BM_PMU_INIT_INITIALIZED) {
                /* transition, then start */
                if (barman_atomic_cmp_ex_strong_pointer(&bm_settings.per_core_settings[core].state, current_state, target_state)) {
                    barman_pmu_start();
                    return BM_PMU_STATE_CHANGE_SUCCEEDED;
                }
            }
            else {
                /* try change from should-stop to should-start */
                if (*current_state == BM_PMU_STATE_SHOULD_STOP) {
                    if (barman_atomic_cmp_ex_strong_pointer(&bm_settings.per_core_settings[core].state, current_state, BM_PMU_STATE_SHOULD_START)) {
                        *current_state = BM_PMU_STATE_SHOULD_START;
                    }
                }

                /* invalid transition */
                return BM_PMU_STATE_CHANGE_FAILED;
            }

            /* try again */
            break;
        }
        case BM_PMU_STATE_STARTED: {
            /* no change */
            return BM_PMU_STATE_CHANGE_SUCCEEDED;
        }
        case BM_PMU_STATE_UNINITIALIZED:
        default: {
            /* invalid transition */
            return BM_PMU_STATE_CHANGE_FAILED;
        }
        }
        break;
    }
    case BM_PMU_STATE_STOPPED: {
        switch (*current_state) {
        case BM_PMU_STATE_SHOULD_START:
        case BM_PMU_STATE_SHOULD_STOP:
        case BM_PMU_STATE_STARTED: {
            /* check initialized */
            const enum bm_pmu_init_state init_state = barman_atomic_load(&bm_settings.per_core_settings[core].init_state);

            if (init_state == BM_PMU_INIT_INITIALIZED) {
                /* transition, then stop */
                if (barman_atomic_cmp_ex_strong_pointer(&bm_settings.per_core_settings[core].state, current_state, target_state)) {
                    barman_pmu_stop();
                    return BM_PMU_STATE_CHANGE_SUCCEEDED;
                }
            }
            else {
                /* try change from should-start to should-stop */
                if (*current_state == BM_PMU_STATE_SHOULD_START) {
                    if (barman_atomic_cmp_ex_strong_pointer(&bm_settings.per_core_settings[core].state, current_state, BM_PMU_STATE_SHOULD_STOP)) {
                        *current_state = BM_PMU_STATE_SHOULD_STOP;
                    }
                }

                /* invalid transition */
                return BM_PMU_STATE_CHANGE_FAILED;
            }

            /* try again */
            break;
        }
        case BM_PMU_STATE_STOPPED: {
            /* no change */
            return BM_PMU_STATE_CHANGE_SUCCEEDED;
        }

        case BM_PMU_STATE_UNINITIALIZED:
        default: {
            /* invalid transition */
            return BM_PMU_STATE_CHANGE_FAILED;
        }
        }
        break;
    }
    default: {
        /* unknown transition */
        return BM_PMU_STATE_CHANGE_FAILED;
    }
    }

    return BM_PMU_STATE_CHANGE_RETRY;
}

/**
 * @brief   Transition the PMU state for a given core to some new state
 * @param   core            The core number
 * @param   target_state    The new state
 * @return  BM_TRUE if the state was changed, BM_FALSE if the transition is invalid
 */
static BM_INLINE bm_bool barman_change_pmu_state(bm_uint32 core, enum bm_pmu_target_state target_state)
{
    bm_atomic_pmu_target_state current_state = barman_atomic_load(&bm_settings.per_core_settings[core].state);
    enum bm_try_change_pmu_result result;

    do {
        result = barman_try_change_pmu_state(core, &current_state, target_state);
    } while (result == BM_PMU_STATE_CHANGE_RETRY);

    return result == BM_PMU_STATE_CHANGE_SUCCEEDED;
}

/**
 * @brief   Attempt to init the PMU and then transition to the correct state
 * @param   core            The core number
 * @param   current_state   [IN/OUT] The current state
 * @param   target_state    The target state
 * @retval  BM_PMU_STATE_CHANGE_SUCCEEDED   If the PMU was initialized and the state changed
 * @retval  BM_PMU_STATE_CHANGE_FAILED      If the PMU is busy being initialized by another thread
 * @retval  BM_PMU_STATE_CHANGE_RETRY       If the state change failed
 */
static BM_INLINE enum bm_try_change_pmu_result barman_init_and_transition_pmu(bm_uint32 core,enum bm_pmu_target_state * current_state, enum bm_pmu_target_state target_state)
{
    /* init if necessary */
    enum bm_try_change_pmu_result result = barman_initialize_pmu(core);

    if (result == BM_PMU_STATE_CHANGE_SUCCEEDED) {
        /* The init succeeded; try to change state */
        return barman_try_change_pmu_state(core, current_state, target_state);
    }

    else if (result == BM_PMU_STATE_CHANGE_FAILED) {
        /* state must become uninitialized */
        barman_atomic_store(&bm_settings.per_core_settings[core].state, BM_PMU_STATE_UNINITIALIZED);
        *current_state = BM_PMU_STATE_UNINITIALIZED;
    }

    /* either the PMU failed to initialized, or the PMU is being initialized by another thread so fail to prevent deadlock */
    return BM_PMU_STATE_CHANGE_FAILED;
}

/**
 * @brief   Transitions the state from a `SHOULD_xxx` state at the start of a sample
 * @param   core    The current core number
 * @return  The state after any transition
 */
static BM_INLINE enum bm_pmu_target_state barman_transition_pmu_state_on_sample(bm_uint32 core)
{
    enum bm_pmu_target_state current_state = barman_atomic_load(&bm_settings.per_core_settings[core].state);
    enum bm_try_change_pmu_result result;

    do
    {
        switch (current_state)
        {
            case BM_PMU_STATE_SHOULD_START:
            case BM_PMU_STATE_SHOULD_STOP: {
                const enum bm_pmu_target_state target_state = (current_state == BM_PMU_STATE_SHOULD_START ? BM_PMU_STATE_STARTED : BM_PMU_STATE_STOPPED);

                /* init and transition */
                result = barman_init_and_transition_pmu(core, &current_state, target_state);

                if (result == BM_PMU_STATE_CHANGE_SUCCEEDED) {
                    /* the pmu was initialized and state change succeeded */
                    return target_state;
                }
                else if (result == BM_PMU_STATE_CHANGE_FAILED) {
                    /* the pmu was in the process of initializing, fail as another thread is busy */
                    return current_state;
                }

                /* try again */
                break;
            }
            case BM_PMU_STATE_STOPPED:
            case BM_PMU_STATE_STARTED:
            case BM_PMU_STATE_UNINITIALIZED:
            default: {
                /* no change */
                return current_state;
            }
        }
    }
    while (1);
}

#if BM_NUM_CUSTOM_COUNTERS > 0

/**
 * @brief   Read a single custom counter value
 * @param   counter     The index of the counter to read
 * @param   value_out   The pointer to write the counter value to
 * @return  BM_TRUE if the value was sampled, BM_FALSE if not (i.e. is not a sampled counter, or failed to read value)
 */
BM_NONNULL((2))
static bm_bool barman_sample_custom_counter(bm_uint32 counter, bm_uint64 * value_out)
{
    if ((counter < BM_NUM_CUSTOM_COUNTERS) && (BM_CUSTOM_CHARTS_SERIES[counter]->sampling_function != BM_NULL)) {
        return BM_CUSTOM_CHARTS_SERIES[counter]->sampling_function(value_out);
    }

    return BM_FALSE;
}

#endif

/** @} */

/* ********************************** */

bm_bool barman_initialize_pmu_family(bm_uint32 midr, bm_uint32 n_event_types, const bm_uint32 * event_types,
                                     const bm_core_set allowed_cores)
{
    const bm_size_t allowed_length = sizeof(bm_core_set);
    bm_uint32 p_index, d_index;

    /* check not already started */
    if (barman_atomic_load(&bm_settings.started)) {
        BM_ERROR("Cannot configure a new PMU family once sampling is started\n");
        return BM_FALSE;
    }

    /* check not full */
    if (bm_settings.num_pmu_family_settings >= BM_COUNT_OF(bm_settings.pmu_family_settings)) {
        BM_ERROR("No more space for configuration settings when configuring PMU family\n");
        return BM_FALSE;
    }

    /* iterate over PMU configurations to check not already set */
    for (p_index = 0; p_index < bm_settings.num_pmu_family_settings; ++p_index) {
        /* check not same MIDR */
        if (bm_settings.pmu_family_settings[p_index].midr == midr) {
            /* check no overlap of allowed cores */
            for (d_index = 0; d_index < allowed_length; ++d_index) {
                const bm_uint8 allowed_pmu = (bm_settings.pmu_family_settings[p_index].allowed_cores[d_index]);
                const bm_uint8 allowed_new = (allowed_cores != BM_NULL ? allowed_cores[d_index] : ~0);
                /* must be no overlap of allowed cores */
                if ((allowed_pmu & allowed_new) != 0) {
                    BM_ERROR("Overlapping core bitmaps when configuring new PMU family\n");
                    return BM_FALSE;
                }
            }
        }
    }

    /* write the new configuration */
    p_index = bm_settings.num_pmu_family_settings++;
    bm_settings.pmu_family_settings[p_index].midr = midr;
    bm_settings.pmu_family_settings[p_index].num_events = BM_MIN(n_event_types, BM_COUNT_OF(bm_settings.pmu_family_settings[p_index].event_types));
    for (d_index = 0; d_index < bm_settings.pmu_family_settings[p_index].num_events; ++d_index) {
        bm_settings.pmu_family_settings[p_index].event_types[d_index] = event_types[d_index];
    }
    for (d_index = 0; d_index < allowed_length; ++d_index) {
        bm_settings.pmu_family_settings[p_index].allowed_cores[d_index] = (allowed_cores != BM_NULL ? allowed_cores[d_index] : ~0);
    }

    BM_INFO("PMU family #%u configured as (midr=0x%x, n_event_types=%u)\n", p_index, midr, bm_settings.pmu_family_settings[p_index].num_events);

    return BM_TRUE;
}

void barman_enable_sampling(void)
{
    bm_uint32 index;

    barman_atomic_store(&bm_settings.started, BM_TRUE);

    for (index = 0; index < BM_CONFIG_MAX_CORES; ++index) {
        barman_change_pmu_state(index, BM_PMU_STATE_SHOULD_START);
    }

#if BM_CONFIG_MAX_CORES == 1
    /*
     * If we only have one core (that's this one) so we can start
     * the PMU now without waiting for barman_sample_counters to be called
     * on the core.
     */
    barman_transition_pmu_state_on_sample(0);
#endif
}

void barman_disable_sampling(void)
{
    bm_uint32 index;

    for (index = 0; index < BM_CONFIG_MAX_CORES; ++index) {
        barman_change_pmu_state(index, BM_PMU_STATE_SHOULD_STOP);
    }

#if BM_CONFIG_MAX_CORES == 1
    barman_transition_pmu_state_on_sample(0);
#endif
}

void barman_sample_counters(bm_bool sample_return_address)
{
    barman_sample_counters_with_program_counter(sample_return_address ? BM_GET_RETURN_ADDRESS() : BM_NULL);
}

void barman_sample_counters_with_program_counter(const void * pc)
{
    const bm_uint32 core = barman_get_core_no();

    enum bm_pmu_target_state current_state;
    bm_uint64 counter_values[BM_MAX_PMU_COUNTERS] = { BM_PMU_INVALID_COUNTER_VALUE };
    bm_uint64 timestamp;
    bm_uint32 num_counters;
    bm_uint32 counter;
#if BM_CONFIG_MIN_SAMPLE_PERIOD > 0
    bm_bool was_successful;
    bm_uint64 last_timestamp;
    bm_uint64 min_period;
#endif
    bm_uint32 valid_custom_counters = 0;
    bm_uint32 custom_counter_ids[BM_CUSTOM_COUNTER_ARRAY_SIZE];
    bm_uint64 custom_counter_values[BM_CUSTOM_COUNTER_ARRAY_SIZE];

    /* validate core */
    if (core >= BM_CONFIG_MAX_CORES) {
        return;
    }

    /* validate initialized and started; transition state if required */
    current_state = barman_transition_pmu_state_on_sample(core);
    if (current_state != BM_PMU_STATE_STARTED) {
        return;
    }

    /* validate has some counters */
    num_counters = bm_settings.per_core_settings[core].num_counters;
    if (num_counters == 0) {
        return;
    }

    /* get timestamp */
    timestamp = barman_ext_get_timestamp();

#if BM_CONFIG_MIN_SAMPLE_PERIOD > 0
    last_timestamp = barman_atomic_load(&bm_settings.per_core_settings[core].last_sample_timestamp);
    min_period = bm_settings.minimum_sample_period;

    if ((last_timestamp + min_period) > timestamp) {
        /* Skip this sample */
        return;
    }
#endif

    /* read all the counters */
    for (counter = 0; counter < num_counters; ++counter) {
        counter_values[counter] = barman_pmu_read_counter(counter);
    }

    /* sample custom counters */
#if BM_NUM_CUSTOM_COUNTERS > 0
    for (counter = 0; counter < BM_NUM_CUSTOM_COUNTERS; ++counter) {
        if (barman_sample_custom_counter(counter, &(custom_counter_values[valid_custom_counters]))) {
            custom_counter_ids[valid_custom_counters] = counter;
            valid_custom_counters += 1;
        }
    }
#endif

    /* write the sample */

#if BM_CONFIG_MIN_SAMPLE_PERIOD > 0
    was_successful =
#endif

    barman_protocol_write_sample(timestamp, core,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                 barman_ext_get_current_task_id(),
#endif
                                 pc, num_counters, counter_values,
                                 valid_custom_counters, custom_counter_ids, custom_counter_values);

#if BM_CONFIG_MIN_SAMPLE_PERIOD > 0
    if (was_successful) {
        barman_atomic_store(&bm_settings.per_core_settings[core].last_sample_timestamp, timestamp);
    }
#endif
}

#if BM_CONFIG_MAX_TASK_INFOS > 0
void barman_record_task_switch(enum bm_task_switch_reason reason)
{
    const bm_uint32 core = barman_get_core_no();

    bm_uint64 timestamp;

    /* validate core */
    if (core >= BM_CONFIG_MAX_CORES) {
        return;
    }

    /* get timestamp */
    timestamp = barman_ext_get_timestamp();

    /* write the task switch record */
    barman_protocol_write_task_switch(timestamp, core, barman_ext_get_current_task_id(), reason);
}
#endif

void barman_wfi(void)
{
    const bm_uint32 core = barman_get_core_no();

    /* validate core */
    if (core >= BM_CONFIG_MAX_CORES) {
        return;
    }

    /* send before event */
    barman_protocol_write_halt_event(barman_ext_get_timestamp(), core, BM_TRUE);

    /* do WFI */
    barman_wfi_intrinsic();

    /* send after event */
    barman_protocol_write_halt_event(barman_ext_get_timestamp(), core, BM_FALSE);
}

void barman_wfe(void)
{
    const bm_uint32 core = barman_get_core_no();

    /* validate core */
    if (core >= BM_CONFIG_MAX_CORES) {
        return;
    }

    /* send before event */
    barman_protocol_write_halt_event(barman_ext_get_timestamp(), core, BM_TRUE);

    /* do WFE */
    barman_wfe_intrinsic();

    /* send after event */
    barman_protocol_write_halt_event(barman_ext_get_timestamp(), core, BM_FALSE);
}

void barman_before_idle(void)
{
    const bm_uint32 core = barman_get_core_no();

    /* validate core */
    if (core >= BM_CONFIG_MAX_CORES) {
        return;
    }

    /* send before event */
    barman_protocol_write_halt_event(barman_ext_get_timestamp(), core, BM_TRUE);
}

void barman_after_idle(void)
{
    const bm_uint32 core = barman_get_core_no();

    /* validate core */
    if (core >= BM_CONFIG_MAX_CORES) {
        return;
    }

    /* send before event */
    barman_protocol_write_halt_event(barman_ext_get_timestamp(), core, BM_FALSE);
}

/**
 * @brief   Annotation types
 */
enum bm_annotation_types
{
    BM_ANNOTATION_TYPE_STRING = 0,       /**< A text annotation */
    BM_ANNOTATION_TYPE_BOOKMARK = 1,     /**< A book mark annotation */
    BM_ANNOTATION_TYPE_CHANNEL_NAME = 2, /**< An instruction to name a channel */
    BM_ANNOTATION_TYPE_GROUP_NAME = 3    /**< An instruction to name a group */
};

static void barman_annotate_generic_string(enum bm_annotation_types type, bm_uint32 channel, bm_uint32 group, bm_uint32 color, const char * string)
{
    const bm_uint32 core = barman_get_core_no();
    bm_uintptr string_length = 0;

    /* validate core */
    if (core >= BM_CONFIG_MAX_CORES) {
        return;
    }

    if (string != BM_NULL)
    {
        while (string[string_length] != 0) string_length++;
        string_length++; /* and the null byte */
    }

    barman_protocol_write_annotation(barman_ext_get_timestamp(), core,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                     barman_ext_get_current_task_id(),
#endif
                                     type, channel, group, color, string_length, (const bm_uint8 *) string);
}

void barman_annotate_channel(bm_uint32 channel, bm_uint32 color, const char * text)
{
    barman_annotate_generic_string(BM_ANNOTATION_TYPE_STRING, channel, 0, color, text);
}

void barman_annotate_name_channel(bm_uint32 channel, bm_uint32 group, const char * name)
{
    barman_annotate_generic_string(BM_ANNOTATION_TYPE_CHANNEL_NAME, channel, group, 0, name);
}

void barman_annotate_name_group(bm_uint32 group, const char * name)
{
    barman_annotate_generic_string(BM_ANNOTATION_TYPE_GROUP_NAME, 0, group, 0, name);
}

void barman_annotate_marker(bm_uint32 color, const char * text)
{
    barman_annotate_generic_string(BM_ANNOTATION_TYPE_BOOKMARK, 0, 0, color, text);
}

#if BM_COMPILER_IS_ARMCC

/* Don't warn that these hide builtin functions because the C library is missing them */
#pragma push
#pragma diag_suppress 2687

__asm bm_uint64 __sync_fetch_and_add_8(bm_uint64* ptr, bm_uint64 val)
{
    push   {r4, r5, r6, r7}
    dmb    ish
1
    ldrexd r4, r5, [r0]
    adds   r6, r4, r2
    adc    r7, r5, r3
    strexd r1, r6, r7, [r0]
    cmp    r1, #0
    bne    %b1
    mov    r0, r4
    mov    r1, r5
    dmb    ish
    pop    {r4, r5, r6, r7}
    bx     lr
}

__asm bm_uint64 __sync_sub_and_fetch_8(bm_uint64* ptr, bm_uint64 val)
{
    push   {r4, r5}
    dmb    ish
1
    ldrexd r4, r5, [r0]
    subs   r4, r4, r2
    sbc    r5, r5, r3
    strexd r1, r4, r5, [r0]
    cmp    r1, #0
    bne    %b1
    mov    r0, r4
    mov    r1, r5
    dmb    ish
    pop    {r4, r5}
    bx     lr
}

__asm bm_uint64 __sync_val_compare_and_swap_8(bm_uint64* ptr, bm_uint64 oldval, bm_uint64 newval)
{
    push   {r4, r5, r6, r7}
    ldrd   r4, [sp, #16]
    dmb    ish
1
    ldrexd r6, r7, [r0]
    cmp    r7, r3
    cmpeq  r6, r2
    bne    %f2
    strexd r1, r4, r5, [r0]
    cmp    r1, #0
    bne    %b1
2
    mov    r0, r6
    mov    r1, r7
    dmb    ish
    pop    {r4, r5, r6, r7}
    bx     lr
}

__asm bm_bool __sync_bool_compare_and_swap_8(bm_uint64* ptr, bm_uint64 oldval, bm_uint64 newval)
{
    push   {r4, r5, r6, r7}
    ldrd   r4, [sp, #16]
    dmb    ish
1
    ldrexd r6, r7, [r0]
    cmp    r7, r3
    cmpeq  r6, r2
    bne    %f2
    strexd r1, r4, r5, [r0]
    cmp    r1, #0
    bne    %b1
2
    dmb    ish
    moveq  r0, #1
    movne  r0, #0
    pop    {r4, r5, r6, r7}
    bx     lr
}

/*
 * Provide weak definitions of the 4 and 1 atomics
 * in case microlib is used which is missing them.
 */
__attribute__((weak)) __asm bm_uint32 __sync_fetch_and_add_4(bm_uint32* ptr, bm_uint32 val)
{
    dmb    ish
1
    ldrex  r3, [r0]
    add    r2, r3, r1
    strex  ip, r2, [r0]
    cmp    ip, #0
    bne    %b1
    dmb    ish
    mov    r0, r3
    bx     lr
}
__attribute__((weak)) __asm bm_uint32 __sync_sub_and_fetch_4(bm_uint32* ptr, bm_uint32 val)
{
    dmb    ish
1
    ldrex  r3, [r0]
    sub    r3, r3, r1
    strex  r2, r3, [r0]
    cmp    r2, #0
    bne    %b1
    dmb    ish
    mov    r0, r3
    bx     lr
}

__attribute__((weak)) __asm bm_uint32 __sync_val_compare_and_swap_4(bm_uint32* ptr, bm_uint32 oldval, bm_uint32 newval)
{
    dmb    ish
1
    ldrex  r3, [r0]
    cmp    r3, r1
    bne    %f2
    strex  ip, r2, [r0]
    cmp    ip, #0
    bne    %b1
2
    dmb    ish
    mov    r0, r3
    bx     lr
}

__attribute__((weak)) __asm bm_bool __sync_bool_compare_and_swap_4(bm_uint32* ptr, bm_uint32 oldval, bm_uint32 newval)
{
    dmb    ish
1
    ldrex  ip, [r0]
    cmp    ip, r1
    bne    %f2
    strex  r3, r2, [r0]
    cmp    r3, #0
    bne    %b1
2
    dmb    ish
    moveq  r0, #1
    movne  r0, #0
    bx     lr
}

__attribute__((weak)) __asm bm_bool __sync_bool_compare_and_swap_2(bm_uint16* ptr, bm_uint16 oldval, bm_uint16 newval)
{
    dmb    ish
1
    ldrexh ip, [r0]
    cmp    ip, r1
    bne    %f2
    strexh r3, r2, [r0]
    cmp    r3, #0
    bne    %b1
2
    dmb    ish
    moveq  r0, #1
    movne  r0, #0
    bx     lr
}

__attribute__((weak)) __asm bm_uint8 __sync_fetch_and_add_1(bm_uint8* ptr, bm_uint8 val)
{
    dmb    ish
1
    ldrexb r3, [r0]
    add    r2, r3, r1
    strexb ip, r2, [r0]
    cmp    ip, #0
    bne    %b1
    dmb    ish
    uxtb   r0, r3
    bx     lr
}
__attribute__((weak)) __asm bm_uint8 __sync_sub_and_fetch_1(bm_uint8* ptr, bm_uint8 val)
{
    dmb    ish
1
    ldrexb r3, [r0]
    sub    r3, r3, r1
    strexb r2, r3, [r0]
    cmp    r2, #0
    bne    %b1
    dmb    ish
    uxtb   r0, r3
    bx     lr
}

__attribute__((weak)) __asm bm_uint8 __sync_val_compare_and_swap_1(bm_uint8* ptr, bm_uint8 oldval, bm_uint8 newval)
{
    dmb    ish
1
    ldrexb r3, [r0]
    cmp    r3, r1
    bne    %f2
    strexb ip, r2, [r0]
    cmp    ip, #0
    bne    %b1
2
    dmb    ish
    mov    r0, r3
    bx     lr
}
__attribute__((weak)) __asm bm_bool __sync_bool_compare_and_swap_1(bm_uint8* ptr, bm_uint8 oldval, bm_uint8 newval)
{
    dmb    ish
1
    ldrexb ip, [r0]
    cmp    ip, r1
    bne    %f2
    strexb r3, r2, [r0]
    cmp    r3, #0
    bne    %b1
2
    dmb    ish
    moveq  r0, #1
    movne  r0, #0
    bx     lr
}

__attribute__((weak)) __asm void __sync_synchronize(void)
{
    dmb    ish
    bx     lr
}

#pragma pop

#endif

/* ------------------------------------------------------------------------- */



/* ------------------------------------------------------------------------- */

#if BM_ARM_ARCH_PROFILE == 'M'
#endif

/**
 * Do any generated initialization
 *
 * @return True if successful
 */
extern bm_bool barman_generated_initialize(void);

#if (BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_LINEAR_RAM_BUFFER) || (BM_CONFIG_USE_DATASTORE ==  BM_CONFIG_USE_DATASTORE_CIRCULAR_RAM_BUFFER) || ((BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_USER_SUPPLIED) && BM_DATASTORE_IS_IN_MEMORY)
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
{
#if BM_DATASTORE_IS_IN_MEMORY
    struct bm_datastore_config_in_memory datastore_config;

    if (buffer == BM_NULL) {
        BM_ERROR("buffer must not be null\n");
        return BM_FALSE;
    }
    datastore_config.buffer = buffer;
    datastore_config.buffer_length = buffer_length;

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_STM
    struct bm_datastore_config_stm datastore_config;

    if (stm_extended_stimulus_ports == BM_NULL) {
        BM_ERROR("stm_extended_stimulus_ports must not be null\n");
        return BM_FALSE;
    }
    datastore_config.configuration_registers = stm_configuration_registers;
    datastore_config.extended_stimulus_ports = stm_extended_stimulus_ports;

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ITM
    struct bm_datastore_config_itm datastore_config;

#if BM_ARM_ARCH_PROFILE == 'M'
    datastore_config.registers = BM_ITM_BASE_ADDRESS;
#else
    if (itm_registers == BM_NULL) {
        BM_ERROR("itm_registers must not be null\n");
        return BM_FALSE;
    }
    datastore_config.registers = itm_registers;

#endif
#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ETM
    bm_unit_type datastore_config = BM_UNIT_TYPE_VALUE;
#endif

    /* validate target_name */
    if (target_name == BM_NULL) {
        BM_ERROR("target_name must be provided\n");
        return BM_FALSE;
    }

    /* validate clock_info */
    if (clock_info == BM_NULL) {
        BM_ERROR("clock_info must be provided\n");
        return BM_FALSE;
    }

    if (!barman_protocol_initialize(datastore_config,
                                    target_name, clock_info,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                    num_task_entries, task_entries,
#endif
#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
                                    num_mmap_entries, mmap_entries,
#endif
                                    timer_sample_rate)) {
        return BM_FALSE;
    };

    return barman_generated_initialize();
}

/**
 * @defgroup    bm_cache  Cache operations
 * @{
 */

/**
 * @brief   Clean some area of memory from the cache
 * @param   pointer The pointer to the start of the area to clean
 * @param   length  The length of the area to clean
 */
BM_NONNULL((1))
extern void barman_cache_clean(void * pointer, bm_uintptr length);

/** @} */

/**
 * @defgroup    bm_data_store   Data Store: Interface
 * @brief       The data storage interface defines a mechanism for writing arbitrary sized blocks of binary data.
 * @details     The interface is designed to allow zero-copy where possible. Users must get a block using
 *              {@link barman_ext_datastore_get_block}, write any data to it, then commit the block using
 *              {@link barman_ext_datastore_commit_block}.
 * @note        There is no mechanism to free a block, or mark one as invalid so if it is possible to fail between the
 *              call to {@link barman_ext_datastore_get_block} and {@link barman_ext_datastore_commit_block} it is the
 *              responsibility of the data protocol to handle that.
 * @note        The data storage layer is not responsible for any form of encoding it will simply push blocks of bytes
 *              into whereever they are stored. It is the responsibility of the protocol to ensure the data is
 *              decodable.
 *              _NB_ however, the data store will frame in-memory blocks with a {@link bm_datastore_block_length} length
 *              prefix. The protocol should not encode the length of the block into the binary data it writes.
 * @note        The user should not call {@link barman_ext_datastore_get_block} more than once for the same value of `core`
 *              before calling {@link barman_ext_datastore_commit_block} (i.e. the calls should come in pairs for any
 *              particular value of `core`). It is *undefined behaviour* to do otherwise. Implementations should take
 *              into account that it is valid to call {@link barman_ext_datastore_commit_block} for different values of `core`
 *              concurrently.
 *              If {@link barman_ext_datastore_get_block} returns null, there is no requirement to call
 *              {@link barman_ext_datastore_commit_block}.
 * @note        The buffer is allowed to return a block of memory that longer than the length requested in get_block
 *              for alignment or any other reason.
 * @{ */

/** Header length */
typedef bm_atomic_uint64    bm_datastore_header_length;

/** Data block length. This value may be marked with a padding in the MSB to indicate the block is padding rather than data */
typedef bm_atomic_uintptr   bm_datastore_block_length;

/** The length of a block encodes a marker in the MSB that says whether the block is valid or padding */
#define BM_DATASTORE_BLOCK_PADDING_BIT          (1ul << (sizeof(bm_datastore_block_length) * 8 - 1))
/** Extract the actual length value from the encoded block length */
#define BM_DATASTORE_GET_LENGTH_VALUE(v)        ((v) & ~BM_DATASTORE_BLOCK_PADDING_BIT)
/** Test if is padding block */
#define BM_DATASTORE_IS_PADDING_BLOCK(v)        (((v) & BM_DATASTORE_BLOCK_PADDING_BIT) != 0)
/** Encode block length */
#define BM_DATASTORE_ENCODE_PADDING_BLOCK(v, p) (BM_DATASTORE_GET_LENGTH_VALUE(v) | ((p) ? BM_DATASTORE_BLOCK_PADDING_BIT : 0))

/**
 * @brief   Structure passed to most in-memory data stores that forms part of the protocol header and contains
 *          data about the layout of the in-memory data.
 * @note    It is *undefined behaviour* if some external entity modifies any of the values in the header struct
 *          whilst they may be used by a datastore.
 */
struct bm_datastore_header_data
{
    bm_datastore_header_length buffer_length;   /**< The length of the buffer */
    bm_datastore_header_length write_offset;    /**< The current write offset; points to the first unwritten byte within the buffer.
                                                     For ring buffers this is the first byte past the end of the ring */
    bm_datastore_header_length read_offset;     /**< The current read offset. For ring buffers this is the start of the ring */
    bm_datastore_header_length total_written;   /**< Total number of bytes consumed; always increments */
    bm_uint8 * base_pointer;                    /**< The base address of the buffer */
};

#if BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_USER_SUPPLIED

/**
 * @brief   Initialize the data store
 * @param   header_data              A {@link struct bm_datastore_header_data} * header_data for in-memory data stores
 * @param   datastore_config         A void * that can be used to pass anything
 * @return  BM_TRUE if the data store initialized properly, BM_FALSE otherwise
 */
extern bm_bool barman_ext_datastore_initialize(
#if BM_CONFIG_DATASTORE_USER_SUPPLIED_IS_IN_MEMORY
    struct bm_datastore_header_data * header_data);
#else
    void * datastore_config);
#endif

/**
 * @brief   Get a pointer to a block of memory of `length` bytes which can be written to.
 * @details Where it is significant to the underlying data store, the value of `core` may be used to select some
 *          appropriate internal buffer or other data structure.
 * @param   core    The core number.
 * @param   length  The length of the data to write
 * @return  A pointer to the block of memory that may be written to, or null if the buffer is full / unable to write.
 * @note    It is *undefined behaviour* to write more that `length` bytes of data to the block.
 * @ingroup bm_external_api
 */
extern bm_uint8 * barman_ext_datastore_get_block(bm_uint32 core, bm_datastore_block_length length);

/**
 * @brief   Commit a completed block of memory.
 * @param   core            The core number.
 * @param   block_pointer   The non-null value previously returned by the *last* call to {@link barman_ext_datastore_get_block}
 *                          with the matching value of argument `core`.
 * @note    It is *undefined behaviour* to call this function with a value that does not match that which was returned
 *          by the *last* call to {@link barman_ext_datastore_get_block} with the matching value of argument `core`.
 * @ingroup bm_external_api
 */
BM_NONNULL((2))
extern void barman_ext_datastore_commit_block(bm_uint32 core, bm_uint8 * block_pointer);

/**
 * @brief   Close the data store.
 * @details Marks the data store as closed. No subsequent writes should complete.
 *          No calls to {@link barman_ext_datastore_commit_block} should complete.
 * @note    It is *undefined behaviour* to call {@link barman_ext_datastore_get_block} before the close, with the matching
 *          {@link barman_ext_datastore_commit_block} afterwards.
 * @ingroup bm_external_api
 */
extern void barman_ext_datastore_close(void);

/**
 * @brief   The contents of the protocol header have been updated
 * @details The data store should store / update / transmit the data that makes up the header.
 * @param   timestamp   The timestamp of the change
 * @param   header      The address of the data
 * @param   length      The length of the data
 */
BM_NONNULL((2))
extern void barman_ext_datastore_notify_header_updated(bm_uint64 timestamp, const void * header, bm_uintptr length);

#endif

/** @} */

/* Select the appropriate datastore */
/** @{ */
#if BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_LINEAR_RAM_BUFFER

/**
 * @defgroup    bm_data_store_linear_ram_buffer     Data Store: The linear RAM buffer data store
 * @brief       Stores data in a fixed length linear RAM buffer. Onces the space in the buffer is exhausted, the buffer
 *              will become full and all subsequent calls to {@link barman_linear_ram_buffer_get_block} will return null.
 * @{ */

/**
 * @brief   Initialize the linear ram buffer
 * @param   header_data The header object to store data into
 * @return  BM_TRUE if initialized successfully, BM_FALSE if not
 * @note    If this function is called multiple times, it will fail unless the buffer was previously closed
 *
 */
BM_NONNULL((1)) bm_bool barman_linear_ram_buffer_initialize(struct bm_datastore_header_data * header_data);

/**
 * @brief   Get a pointer to a block of memory of `length` bytes which can be written to.
 * @see     barman_datastore_get_block
 */
bm_uint8 * barman_linear_ram_buffer_get_block(bm_uint32 core, bm_datastore_block_length length);

/**
 * @brief   Commit a completed block of memory.
 * @see     barman_datastore_commit_block
 */
BM_NONNULL((2)) void barman_linear_ram_buffer_commit_block(bm_uint32 core, bm_uint8 * block_pointer);

/**
 * @brief   Close the data store.
 * @see     barman_datastore_close
 */
extern void barman_linear_ram_buffer_close(void);

/** @} */

#   define barman_datastore_initialize(header_data)             barman_linear_ram_buffer_initialize(header_data)
#   define barman_datastore_get_block(core, length)             barman_linear_ram_buffer_get_block(core, length)
#   define barman_datastore_commit_block(core, block_pointer)   barman_linear_ram_buffer_commit_block(core, block_pointer)
#   define barman_datastore_close()                             barman_linear_ram_buffer_close()
#   define barman_datastore_notify_header_updated(timestamp, header, length)    barman_cache_clean(header, length)
#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_CIRCULAR_RAM_BUFFER

/**
 * @defgroup    bm_data_store_circular_ram_buffer     Data Store: The circular RAM buffer data store
 * @brief       Stores data in a fixed length circular RAM buffer.
 * @{ */

/**
 * @brief   Initialize the circular ram buffer
 * @param   header_data The header object to store data into
 * @return  BM_TRUE if initialized successfully, BM_FALSE if not
 * @note    If this function is called multiple times, it will fail unless the buffer was previously closed
 *
 */
BM_NONNULL((1))
bm_bool barman_circular_ram_buffer_initialize(struct bm_datastore_header_data * header_data);

/**
 * @brief   Get a pointer to a block of memory of `length` bytes which can be written to.
 * @see     barman_datastore_get_block
 */
bm_uint8 * barman_circular_ram_buffer_get_block(bm_uint32 core, bm_datastore_block_length length);

/**
 * @brief   Commit a completed block of memory.
 * @see     barman_datastore_commit_block
 */
BM_NONNULL((2))
void barman_circular_ram_buffer_commit_block(bm_uint32 core, bm_uint8 * block_pointer);

/**
 * @brief   Close the data store.
 * @see     barman_datastore_close
 */
extern void barman_circular_ram_buffer_close(void);

/** @} */

#   define barman_datastore_initialize(header_data)             barman_circular_ram_buffer_initialize(header_data)
#   define barman_datastore_get_block(core, length)             barman_circular_ram_buffer_get_block(core, length)
#   define barman_datastore_commit_block(core, block_pointer)   barman_circular_ram_buffer_commit_block(core, block_pointer)
#   define barman_datastore_close()                             barman_circular_ram_buffer_close()
#   define barman_datastore_notify_header_updated(timestamp, header, length)    barman_cache_clean(header, length)
#elif BM_DATASTORE_USES_STREAMING_INTERFACE

/**
 * @defgroup    bm_data_store_streaming_interface     Data Store: A streaming datastore
 * @brief       Streams data over some multi-channel interface
 * @{ */

/**
 * @brief   Initialize the streaming datastore
 * @param   datastore_config A {@link bm_datastore_config}
 * @return  BM_TRUE if initialized successfully, BM_FALSE if not
 * @note    If this function is called multiple times, it will fail unless the buffer was previously closed
 *
 */
bm_bool barman_streaming_interface_initialize(bm_datastore_config datastore_config);

/**
 * @brief   Get a pointer to a block of memory of `length` bytes which can be written to.
 * @see     barman_datastore_get_block
 */
bm_uint8 * barman_streaming_interface_get_block(bm_datastore_block_length length);

/**
 * @brief   Commit a completed block of memory.
 * @see     barman_datastore_commit_block
 */
BM_NONNULL((1)) void barman_streaming_interface_commit_block(bm_uint8 * block_pointer);

/**
 * @brief   Close the data store.
 * @see     barman_datastore_close
 */
void barman_streaming_interface_close(void);

/**
 * @brief   The contents of the protocol header have been updated
 * @details The data store should store / update / transmit the data that makes up the header.
 * @param   timestamp   The timestamp of the change
 * @param   header      The address of the data
 * @param   length      The length of the data
 */
BM_NONNULL((2))
void barman_streaming_interface_notify_header_updated(bm_uint64 timestamp, const void * header, bm_uintptr length);

/** @} */

#   define barman_datastore_initialize(config)                  barman_streaming_interface_initialize(config)
#   define barman_datastore_get_block(core, length)             barman_streaming_interface_get_block(length)
#   define barman_datastore_commit_block(core, block_pointer)   barman_streaming_interface_commit_block(block_pointer)
#   define barman_datastore_close()                             barman_streaming_interface_close()
#   define barman_datastore_notify_header_updated(timestamp, header, length)    barman_streaming_interface_notify_header_updated(timestamp, header, length)
#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_USER_SUPPLIED
#   define barman_datastore_initialize(config_or_header_data)   barman_ext_datastore_initialize(config_or_header_data)
#   define barman_datastore_get_block(core, length)             barman_ext_datastore_get_block(core, length)
#   define barman_datastore_commit_block(core, block_pointer)   barman_ext_datastore_commit_block(core, block_pointer)
#   define barman_datastore_close()                             barman_ext_datastore_close()
#   define barman_datastore_notify_header_updated(timestamp, header, length)    barman_ext_datastore_notify_header_updated(timestamp, header, length)
#endif
/** @} */

/* *************************************** */

/**
 * @def     BM_PROTOCOL_MAGIC_BYTES
 * @brief   Protocol header magic bytes.
 * @details Encodes the string "BARMAN32" or "BARMAN64" depending on the bitness of the target.
 *          The value is written in native endianness to identify the endianness of the target.
 *
 * @def     BM_PROTOCOL_MAGIC_BYTES_64
 * @brief   64 bit target magic bytes
 *
 * @def     BM_PROTOCOL_MAGIC_BYTES_32
 * @brief   32 bit target magic bytes
 */
#define BM_PROTOCOL_MAGIC_BYTES_64      0x4241524D414E3634ull
#define BM_PROTOCOL_MAGIC_BYTES_32      0x4241524D414E3332ull
#define BM_PROTOCOL_MAGIC_BYTES         (sizeof(void*) == 8 ? BM_PROTOCOL_MAGIC_BYTES_64 : BM_PROTOCOL_MAGIC_BYTES_32)

#if BM_BIG_ENDIAN
#define BM_PROTOCOL_MAGIC_BYTES_FIRST_WORD    (bm_uint32)(BM_PROTOCOL_MAGIC_BYTES >> 32)
#define BM_PROTOCOL_MAGIC_BYTES_SECOND_WORD   (bm_uint32)(BM_PROTOCOL_MAGIC_BYTES)
#else
#define BM_PROTOCOL_MAGIC_BYTES_FIRST_WORD    (bm_uint32)(BM_PROTOCOL_MAGIC_BYTES)
#define BM_PROTOCOL_MAGIC_BYTES_SECOND_WORD   (bm_uint32)(BM_PROTOCOL_MAGIC_BYTES >> 32)
#endif

/**
 * @brief   Defines the current protocol version number
 * @details Version     Description
 *          1           First release in Streamline 6.0
 *          2           Second release in Streamline 6.1; forwards compatible extension from 1 adding
 *                      WFI records and textual annotations.
 *          3           Release in Streamline 6.3; forwards compatible extension from 2 adding
 *                      PC sample, event counter without task ID and warning records (not currently used by barman, only by streamline).
 */
#define BM_PROTOCOL_VERSION             3

/** @brief  String table length */
#define BM_PROTOCOL_STRING_TABLE_LENGTH 1024

/**
 * @brief   Contains various compile time configurable constants
 */
struct bm_protocol_config_values
{
    /** The value of BM_CONFIG_MAX_CORES; gives the length of `per_core_pmu_settings` */
    bm_uint32 max_cores;
    /** The value of BM_CONFIG_MAX_TASK_INFOS; gives the length of `task_info` */
    bm_uint32 max_task_infos;
    /** The value of BM_CONFIG_MAX_MMAP_LAYOUTS; gives the length of `mmap_layout` */
    bm_uint32 max_mmap_layout;
    /** The value of BM_MAX_PMU_COUNTERS; gives the length of `per_core_pmu_settings.counter_types` */
    bm_uint32 max_pmu_counters;
    /** The maximum length of the string table */
    bm_uint32 max_string_table_length;
    /** The value of BM_NUM_CUSTOM_COUNTERS; gives the number of custom counters, length of `custom_counters` */
    bm_uint32 num_custom_counters;
}BM_PACKED_TYPE;

/**
 * @brief   Describes the per-core pmu settings
 */
struct bm_protocol_header_pmu_settings
{
    /** The timestamp the configuration was written */
    bm_uint64 configuration_timestamp;
    /** The MIDR of the core; MIDR register */
    bm_uint32 midr;
    /** The multiprocessor affinity register value (MPIDR) */
    bm_uintptr mpidr;
    /** The cluster number of the processor */
    bm_uint32 cluster_id;
    /** The number of valid entries in `counter_types` */
    bm_uint32 num_counters;
    /** The record of counter types associated with the core's PMU */
    bm_uint32 counter_types[BM_MAX_PMU_COUNTERS];
}BM_PACKED_TYPE;

#if BM_CONFIG_MAX_TASK_INFOS > 0
/**
 * @brief   A task information record. Describes information about a unique task within the system
 */
struct bm_protocol_header_task_info
{
    /** The timestamp the record was inserted */
    bm_uint64 timestamp;
    /** The task id */
    bm_task_id_t task_id;
    /** The offset of name of the task in the string table */
    bm_uint32 task_name_ptr;
}BM_PACKED_TYPE;
#endif

#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
/**
 * @brief   A MMAP record; describes an executable image's position in memory
 */
struct bm_protocol_header_mmap_layout
{
#if BM_CONFIG_MAX_TASK_INFOS > 0
    /** The timestamp the record was inserted */
    bm_uint64 timestamp;
    /** The task ID to associate with the map */
    bm_task_id_t task_id;
#endif
    /** The base address of the image or image section */
    bm_uintptr base_address;
    /** The length of the image or image section */
    bm_uintptr length;
    /** The image section offset */
    bm_uintptr image_offset;
    /** The offset of name of the image in the string table */
    bm_uint32 image_ptr;
}BM_PACKED_TYPE;
#endif

/**
 * @brief   A custom chart description
 */
struct bm_protocol_header_custom_chart
{
    /** The offset of name of the chart in the string table */
    bm_uint32 name_ptr;
    /** The series composition */
    bm_uint8 series_composition;
    /** The rendering type */
    bm_uint8 rendering_type;
    /** Boolean flags: average_selection, average_cores, percentage, per_cpu */
    bm_uint8 boolean_flags;
}BM_PACKED_TYPE;

/**
 * @brief   A custom chart series description
 */
struct bm_protocol_header_custom_chart_series
{
    /** The index of the chart the series belongs to */
    bm_uint32 chart_index;
    /** The offset of name of the chart in the string table */
    bm_uint32 name_ptr;
    /** The offset of the Series units in the string table */
    bm_uint32 units_ptr;
    /** The description string pointer */
    bm_uint32 description_ptr;
    /** Series colour */
    bm_uint32 colour;
    /** Multiplier value */
    double multiplier;
    /** Data class */
    bm_uint8 clazz;
    /** Display type */
    bm_uint8 display;
    /** Boolean flags: sampled */
    bm_uint8 boolean_flags;
}BM_PACKED_TYPE;

/**
 * @brief   String table
 */
struct bm_protocol_header_string_table
{
    /** The amount of string table that is used */
    bm_atomic_uint32 string_table_length;
    /** The string table. A sequence of null-terminated strings referenced from elsewhere in the header */
    char string_table[BM_PROTOCOL_STRING_TABLE_LENGTH];
};

/**
 * @brief   In memory protocol header page which is stored at the head of the in memory data buffer
 * @note    Must maintain 8-byte alignment internally as it contains atomic uint64s. No BM_PACKED_TYPE
 */
struct bm_protocol_header
{
    /* -- 00 --------- Every thing past here is known offset */

    /** Magic bytes value */
    /* 00 */ bm_uint64 magic_bytes;
    /** Protocol version value */
    /* 08 */ bm_uint32 protocol_version;
    /** The length of this struct; i.e. sizeof(struct bm_protocol_header) */
    /* 12 */ bm_uint32 header_length;
    /** Data store type */
    /* 16 */ bm_uint32 data_store_type;
    /** The offset into the string table that contains the target description string */
    /* 20 */ bm_uint32 target_name_ptr;
    /** Timestamp of last attempt to write a sample (even if write failed) */
    /* 24 */ bm_atomic_uint64 last_timestamp;
    /** Timer based sampling rate; in Hz. Zero indicates no timer based sampling (assumes max 4GHz sample rate) */
    /* 32 */ bm_uint32 timer_sample_rate;
    /** Config constant values */
    /* 36 */ struct bm_protocol_config_values config_constants;
    /** Clock parameters */
    /* 60 */ struct bm_protocol_clock_info clock_info;

    /* -- 92 --------- Every thing past here is calculated offset */

    /** The string table */
    struct bm_protocol_header_string_table string_table;
    /** Per-core PMU configuration settings. Each index maps to a core. */
    struct bm_protocol_header_pmu_settings per_core_pmu_settings[BM_CONFIG_MAX_CORES];
#if BM_CONFIG_MAX_TASK_INFOS > 0
    /** Number of task records that contain data */
    bm_atomic_uint32 num_task_entries;
    /** Task information */
    struct bm_protocol_header_task_info task_info[BM_CONFIG_MAX_TASK_INFOS];
#endif
#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
    /** Number of mmap records that contain data */
    bm_atomic_uint32 num_mmap_layout_entries;
    /** MMAP information */
    struct bm_protocol_header_mmap_layout mmap_layout[BM_CONFIG_MAX_MMAP_LAYOUTS];
#endif
#if BM_NUM_CUSTOM_COUNTERS > 0
    /** Number of custom charts (must be equal to BM_CUSTOM_CHARTS_COUNT) */
    bm_uint32 num_custom_charts;
    /** Custom chart descriptions */
    struct bm_protocol_header_custom_chart custom_charts[BM_CUSTOM_CHARTS_COUNT];
    /** Custom chart series */
    struct bm_protocol_header_custom_chart_series custom_charts_series[BM_NUM_CUSTOM_COUNTERS];
#endif
    /** Data store parameters (for in memory buffers) */
    struct bm_datastore_header_data data_store_parameters;
};

/**
 * @brief   Record types
 */
enum bm_protocol_record_types
{
    BM_PROTOCOL_RECORD_SAMPLE = 1,                         /**< Counter sample */
    BM_PROTOCOL_RECORD_SAMPLE_WITH_PC = 2,                 /**< Counter sample with PC value */
    BM_PROTOCOL_RECORD_TASK_SWITCH = 3,                    /**< Task switch */
    BM_PROTOCOL_RECORD_CUSTOM_COUNTER = 4,                 /**< Custom counter value */
    BM_PROTOCOL_RECORD_ANNOTATION = 5,                     /**< Annotation */
    BM_PROTOCOL_RECORD_HALT_EVENT = 6,                     /**< Halting event (WFI/WFE) */
    BM_PROTOCOL_RECORD_PC_WITHOUT_TASK_ID = 7,             /**< PC sample that doesn't have a task ID regardless of BM_MAX_TASK_INFOS */
    BM_PROTOCOL_RECORD_EVENT_COUNTER_WITHOUT_TASK_ID = 8,  /**< Counter value that doesn't have a task ID regardless of BM_MAX_TASK_INFOS */
    BM_PROTOCOL_RECORD_WARNING = 9                         /**< Warning for streamline to interpret */
};

/**
 * @brief   Record header
 */
struct bm_protocol_record_header
{
    /** Identifies the record type */
    bm_uint32 record_type;
    /** The core number. A value of ~BM_UINT32(0) means no specific core. */
    bm_uint32 core;
    /** The timestamp of the event. A value of ~BM_UINT64(0) means the last timestamp should be used as an approximation. */
    bm_uint64 timestamp;
}BM_PACKED_TYPE;

/**
 * @brief   Sample record (PC / counter values are appended afterwards)
 */
struct bm_protocol_sample
{
    /** Record header, as all records must have */
    struct bm_protocol_record_header header;
#if BM_CONFIG_MAX_TASK_INFOS > 0
    /** Task id field */
    bm_task_id_t task_id;
#endif
#if BM_NUM_CUSTOM_COUNTERS > 0
    /** The number of custom counter values sent */
    bm_uint32 num_custom_counters;
#endif
}BM_PACKED_TYPE;

#if BM_NUM_CUSTOM_COUNTERS > 0
/**
 * @brief   Sample record custom counter value entry
 */
struct bm_protocol_sample_custom_counter_value
{
    /** Custom counter id */
    bm_uint32 id;
    /** Custom counter value */
    bm_uint64 value;
}BM_PACKED_TYPE;

/**
 * @brief   Custom counter record
 */
struct bm_protocol_custom_counter_record
{
    /** Record header, as all records must have */
    struct bm_protocol_record_header header;
#if BM_CONFIG_MAX_TASK_INFOS > 0
    /** Task id field */
    bm_task_id_t task_id;
#endif
    /** The custom counter id */
    bm_uint32 counter;
    /** The custom counter value */
    bm_uint64 value;
}BM_PACKED_TYPE;

/**
 * @brief   Enumerate boolean flags
 */
enum
{
    /** bm_protocol_header_custom_chart.boolean_flags - average_selection */
    BM_CHART_FLAG_AVERAGE_SELECTION = 0x01,/**!< BM_CHART_FLAG_AVERAGE_SELECTION */
    /** bm_protocol_header_custom_chart.boolean_flags - average_cores */
    BM_CHART_FLAG_AVERAGE_CORES = 0x02,    /**!< BM_CHART_FLAG_AVERAGE_CORES */
    /** bm_protocol_header_custom_chart.boolean_flags - percentage */
    BM_CHART_FLAG_PERCENTAGE = 0x04,       /**!< BM_CHART_FLAG_PERCENTAGE */
    /** bm_protocol_header_custom_chart.boolean_flags - per_cpu */
    BM_CHART_FLAG_PER_CPU = 0x08,          /**!< BM_CHART_FLAG_PER_CPU */

    /** bm_protocol_header_custom_chart_series.boolean_flags - sampled */
    BM_CHART_SERIES_FLAG_SAMPLED = 0x01    /**!< BM_CHART_SERIES_FLAG_SAMPLED */
};

#endif

#if BM_CONFIG_MAX_TASK_INFOS > 0
/**
 * @brief   Task switch
 */
struct bm_protocol_task_switch
{
    /** Record header, as all records must have */
    struct bm_protocol_record_header header;
    /** Task id field */
    bm_task_id_t task_id;
    /** The reason for the task switch */
    bm_uint8 reason;
}BM_PACKED_TYPE;
#endif

/**
 * @brief   Halting event record
 */
struct bm_protocol_halting_event_record
{
    /** Record header, as all records must have */
    struct bm_protocol_record_header header;
    /** Non-zero if entered halting state, Zero if exited */
    bm_uint8 entered_halt;
}BM_PACKED_TYPE;

/**
 * @brief   Annotation record
 */
struct bm_protocol_annotation_record
{
    /** Record header, as all records must have */
    struct bm_protocol_record_header header;
#if BM_CONFIG_MAX_TASK_INFOS > 0
    /** Task id field */
    bm_task_id_t task_id;
#endif
    /** Length of the byte data that follows the record */
    bm_uintptr data_length;
    /** Annotation channel */
    bm_uint32 channel;
    /** Annotation group */
    bm_uint32 group;
    /** Annotation color */
    bm_uint32 color;
    /** Annotation type */
    bm_uint8 type;
}BM_PACKED_TYPE;

/* *************************************** */

#if BM_DATASTORE_IS_IN_MEMORY

/** The configured protocol header at the start of the memory buffer */
static BM_ATOMIC_TYPE(struct bm_protocol_header *) bm_protocol_header_;

/** @brief  Returns the pointer to the protocol header object */
static BM_ALWAYS_INLINE struct bm_protocol_header * bm_protocol_header(void)
{
    return barman_atomic_load(&bm_protocol_header_);
}

#else

/** The configured protocol header in static storage */
static struct bm_protocol_header bm_protocol_header_;

/** @brief  Returns the pointer to the protocol header object */
BM_RET_NONNULL
static BM_ALWAYS_INLINE struct bm_protocol_header * bm_protocol_header(void)
{
    return &bm_protocol_header_;
}

#endif

#if BM_DATASTORE_IS_IN_MEMORY || BM_CONFIG_RECORDS_PER_HEADER_SENT <= 0

#define barman_datastore_commit_block_and_header(core, block_pointer)   barman_datastore_commit_block(core, block_pointer)

#else

static bm_atomic_uint32 record_counter = BM_ATOMIC_VAR_INIT(0);

BM_NONNULL((2)) static void barman_datastore_commit_block_and_header(bm_uint32 core, bm_uint8 * block_pointer)
{
    /* Commit the block */
    barman_datastore_commit_block(core, block_pointer);

    /* Check if we need to update the header */
    if (barman_atomic_fetch_add(&record_counter, 1) == BM_CONFIG_RECORDS_PER_HEADER_SENT)
    {
        barman_atomic_store(&record_counter, 0);
        barman_datastore_notify_header_updated(0, bm_protocol_header(), sizeof(struct bm_protocol_header));
    }
}

#endif

/* *************************************** */

/**
 * @brief   Adjust the last_timestamp value in the data_store_parameters field so that it is equal to the timestamp parameter, but only if the current value
 *          is less than timestamp, otherwise exists without modification.
 * @param   timestamp   The new timestamp value
 */
BM_NONNULL((1))
static BM_INLINE void barman_protocol_update_last_sample_timestamp(struct bm_protocol_header * header_ptr, bm_uint64 timestamp)
{
#if BM_ARM_ARCH_PROFILE != 'M'
    /* atomic CAS update the timestamp */
    bm_uint64 current_value = barman_atomic_load(&header_ptr->last_timestamp);

    while ((current_value < timestamp) && !barman_atomic_cmp_ex_weak_pointer(&header_ptr->last_timestamp, &current_value, timestamp))
        ;
#endif
}

/**
 * @brief   Initialize a bm_protocol_record_header
 * @param   header      The object to initialize
 * @param   core        The core number
 * @param   record_type The record type
 * @param   timestamp   The timestamp value
 */
BM_NONNULL((1))
static BM_INLINE void barman_protocol_init_record_header(struct bm_protocol_record_header * header, bm_uint32 core, enum bm_protocol_record_types record_type,
                                                         bm_uint64 timestamp)
{
    barman_memset(header, 0, sizeof(*header));

    header->record_type = record_type;
    header->core = core;
    header->timestamp = timestamp;
}

/**
 * @brief   Insert an item into the string table
 * @param   string_table
 * @param   string
 * @param   max_length
 * @return  The index of the string (or a substring of it) in the table
 */
BM_NONNULL((1))
static bm_uint32 barman_protocol_string_table_insert(struct bm_protocol_header_string_table * string_table, const char * string, bm_uint32 max_length)
{
    bm_uint32 table_length = barman_atomic_load(&string_table->string_table_length);
    bm_uint32 table_offset, restart_offset, string_length = 0, longest_match;
    bm_bool table_full = BM_FALSE;

    /* null pointer becomes empty string */
    if (string == BM_NULL) {
        string = "";
    }

    /* get string length */
    while (string[string_length] != 0) {
        if (string_length == max_length) {
            BM_WARNING("Truncating to %d characters: %s", string_length, string);
            break;
        }
        string_length += 1;
    }

    /* use atomic RMW to update string_table_length length */
    do {
        longest_match = 0;

        /* search the table to find the string */
        for (table_offset = 0; table_offset < table_length; table_offset = restart_offset) {
            bm_uint32 string_offset;
            bm_bool failed = BM_FALSE;

            /* assume retry from next character */
            restart_offset = table_offset + 1;

            /* search for matching string */
            for (string_offset = 0; (string_offset <= string_length) && (!failed); ++string_offset) {
                /* the character to match, null terminator is forced as last character */
                const char string_char = (string_offset < string_length ? string[string_offset] : 0);

                /* if character does not match string table, or offset is out of bounds then fail */
                if (((table_offset + string_offset) >= table_length) || (string_table->string_table[table_offset + string_offset] != string_char)) {
                    failed = BM_TRUE;
                    break;
                }
                else {
                    /* update longest match length, this is used if we have to truncate and restart */
                    longest_match = BM_MAX(longest_match, string_offset + 1);

                    /* find the next character that matches the first character of `string` so that we can resume searching from there */
                    if ((string_offset > 0) && (restart_offset == (table_offset + 1)) && (string[0] == string_char)) {
                        restart_offset = table_offset + string_offset;
                    }
                }
            }

            if (!failed) {
                return table_offset;
            }
        }

        /* validate can fit */
        if ((table_length + string_length + 1) > BM_PROTOCOL_STRING_TABLE_LENGTH) {
            const bm_uint32 avail_length = (BM_PROTOCOL_STRING_TABLE_LENGTH > table_length ? (BM_PROTOCOL_STRING_TABLE_LENGTH - table_length) - 1 : 0);
            const bm_uint32 restart_length = (string_length > longest_match ? longest_match : longest_match - 1);

            /* if string length already zero then something bugged; just force last char to be zero */
            if ((string_length == 0) || (table_length > BM_PROTOCOL_STRING_TABLE_LENGTH)) {
                BM_ERROR("string table corrupted. No null terminator.");
                string_table->string_table[BM_PROTOCOL_STRING_TABLE_LENGTH - 1] = 0;
                barman_atomic_store(&string_table->string_table_length, BM_PROTOCOL_STRING_TABLE_LENGTH);
                return BM_PROTOCOL_STRING_TABLE_LENGTH - 1;
            }

            /* try again with shorter string */
            if (string_length > 0) {
                string_length = BM_MAX(avail_length, restart_length);
                table_full = BM_TRUE;
            }

            if (string_length == 0) {
                string = "";
            }

            continue;
        }

        /* append the string */
        if (barman_atomic_cmp_ex_strong_pointer(&string_table->string_table_length, &table_length, table_length + string_length + 1)) {
            bm_uint32 string_offset;
            /* copy in the string */
            for (string_offset = 0; string_offset < string_length; ++string_offset) {
                string_table->string_table[table_length + string_offset] = string[string_offset];
            }
            string_table->string_table[table_length + string_length] = 0;

            if (table_full) {
                BM_WARNING("String table full, truncating to %d characters: %s", string_length, string);
            }

            return table_length;
        }
    } while (BM_TRUE);
}

#if BM_CONFIG_MAX_TASK_INFOS > 0
/**
 * @brief   Fill a `task_info` record
 * @param   header_ptr
 * @param   index
 * @param   timestamp
 * @param   task_entry
 */
BM_NONNULL((1, 4))
static BM_INLINE void barman_protocol_fill_task_record(struct bm_protocol_header * header_ptr, bm_uint32 index, bm_uint64 timestamp,
                                                       const struct bm_protocol_task_info * task_entry)
{
    header_ptr->task_info[index].timestamp = timestamp;
    header_ptr->task_info[index].task_id = task_entry->task_id;
    header_ptr->task_info[index].task_name_ptr = barman_protocol_string_table_insert(&header_ptr->string_table, task_entry->task_name, 31);
}
#endif

#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
/**
 * @brief   Fill a `mmap_layout` record
 * @param   header_ptr
 * @param   index
 * @param   timestamp
 * @param   mmap_entry
 */
BM_NONNULL((1, 4))
static BM_INLINE void barman_protocol_fill_mmap_record(struct bm_protocol_header * header_ptr, bm_uint32 index, bm_uint64 timestamp,
                                                       const struct bm_protocol_mmap_layout * mmap_entry)
{
#if BM_CONFIG_MAX_TASK_INFOS > 0
    header_ptr->mmap_layout[index].timestamp = timestamp;
    header_ptr->mmap_layout[index].task_id = mmap_entry->task_id;
#endif
    header_ptr->mmap_layout[index].base_address = mmap_entry->base_address;
    header_ptr->mmap_layout[index].length = mmap_entry->length;
    header_ptr->mmap_layout[index].image_offset = mmap_entry->image_offset;
    header_ptr->mmap_layout[index].image_ptr = barman_protocol_string_table_insert(&header_ptr->string_table, mmap_entry->image_name, 0xffffffffu);
}
#endif

/**
 * @brief   Get a block and fill the record header
 * @param   length          The length of the block to get
 * @param   timestamp       The timestamp to put in the header
 * @param   core            The core number to put in the header
 * @param   record_type     The record type to put in the header
 * @return  Pointer to the block, BM_NULL if failure
 */
static bm_uint8 * barman_protocol_get_block_and_fill_header(bm_datastore_block_length length, bm_uint32 core, enum bm_protocol_record_types record_type, bm_uint64 timestamp)
{
    struct bm_protocol_header * const header_ptr = bm_protocol_header();
    bm_uint8 * block;

    /* validate has header configured */
    if ((header_ptr == BM_NULL) || (header_ptr->magic_bytes != BM_PROTOCOL_MAGIC_BYTES)) {
        BM_ERROR("Could not write as not initialized\n");
        return BM_NULL;
    }

    /* validate core */
    if (core >= BM_CONFIG_MAX_CORES) {
        BM_DEBUG("Could not write as core > BM_CONFIG_MAX_CORES\n");
        return BM_NULL;
    }

    /* update the last_timestamp value */
    barman_protocol_update_last_sample_timestamp(header_ptr, timestamp);

    /* Get the block */
    block = barman_datastore_get_block(core, length);
    if (block != BM_NULL) {
        /* fill it */
        barman_protocol_init_record_header((struct bm_protocol_record_header *) block, core, record_type, timestamp);
    }

    return block;
}

#if BM_NUM_CUSTOM_COUNTERS > 0
/**
 * @brief   Fill a `custom_chart` record
 * @param   header_ptr
 * @param   index
 * @param   chart
 */
BM_NONNULL((1, 3))
static BM_INLINE void barman_protocol_fill_custom_chart_record(struct bm_protocol_header * header_ptr, bm_uint32 index, const struct bm_custom_counter_chart * chart)
{
    header_ptr->custom_charts[index].name_ptr = barman_protocol_string_table_insert(&header_ptr->string_table, chart->name, 0xffffffffu);
    header_ptr->custom_charts[index].rendering_type = chart->rendering_type;
    header_ptr->custom_charts[index].series_composition = chart->series_composition;
    header_ptr->custom_charts[index].boolean_flags = (chart->average_selection ? BM_CHART_FLAG_AVERAGE_SELECTION : 0) |
                                                     (chart->average_cores ? BM_CHART_FLAG_AVERAGE_CORES : 0) |
                                                     (chart->percentage ? BM_CHART_FLAG_PERCENTAGE : 0) |
                                                     (chart->per_cpu ? BM_CHART_FLAG_PER_CPU : 0);
}

/**
 * @brief   Fill a `custom_chart_series` record
 * @param   header_ptr
 * @param   index
 * @param   series
 */
BM_NONNULL((1, 3))
static BM_INLINE void barman_protocol_fill_custom_chart_series_record(struct bm_protocol_header * header_ptr, bm_uint32 index, const struct bm_custom_counter_chart_series * series)
{
    header_ptr->custom_charts_series[index].chart_index = series->chart_index;
    header_ptr->custom_charts_series[index].name_ptr = barman_protocol_string_table_insert(&header_ptr->string_table, series->name, 0xffffffffu);
    header_ptr->custom_charts_series[index].units_ptr = barman_protocol_string_table_insert(&header_ptr->string_table, series->units, 0xffffffffu);
    header_ptr->custom_charts_series[index].description_ptr = barman_protocol_string_table_insert(&header_ptr->string_table, series->description, 0xffffffffu);
    header_ptr->custom_charts_series[index].multiplier = series->multiplier;
    header_ptr->custom_charts_series[index].display = series->display;
    header_ptr->custom_charts_series[index].clazz = series->clazz;
    header_ptr->custom_charts_series[index].colour = series->colour;
    header_ptr->custom_charts_series[index].boolean_flags = (series->sampling_function != BM_NULL ? BM_CHART_SERIES_FLAG_SAMPLED : 0);
}
#endif

/* *************************************** */

bm_bool barman_protocol_initialize(bm_datastore_config datastore_config,
                                   const char * target_name,
                                   const struct bm_protocol_clock_info * clock_info,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                   bm_uint32 num_task_entries,
                                   const struct bm_protocol_task_info * task_entries,
#endif
#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
                                   bm_uint32 num_mmap_entries,
                                   const struct bm_protocol_mmap_layout * mmap_entries,
#endif
                                   bm_uint32 timer_sample_rate)
{
#if BM_DATASTORE_IS_IN_MEMORY
    const bm_uint32 HEADER_SIZE_ALIGNED_8 = ((sizeof(struct bm_protocol_header) + 7) & ~7ul); /* the data buffer pointer must be aligned to 8 byte boundary */

    bm_uintptr alignment;
#endif
    struct bm_protocol_header * header_ptr = bm_protocol_header();
#if (BM_CONFIG_MAX_TASK_INFOS > 0) || (BM_CONFIG_MAX_MMAP_LAYOUTS > 0) || (BM_NUM_CUSTOM_COUNTERS > 0)
    bm_uint32 index;
#endif

    if ((clock_info->timestamp_multiplier == 0) || (clock_info->timestamp_divisor == 0)) {
        BM_ERROR("clock_info is invalid. multiplier and divisor cannot be zero\n");
        return BM_FALSE;
    }

#if BM_CONFIG_MAX_TASK_INFOS > 0
    /* validate task_entries */
    if ((task_entries == BM_NULL) && (num_task_entries != 0)) {
        BM_ERROR("task_entries is invalid.\n");
        return BM_FALSE;
    }
#endif

#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
    /* validate mmap_entries */
    if ((mmap_entries == BM_NULL) && (num_mmap_entries != 0)) {
        BM_ERROR("mmap_entries is invalid.\n");
        return BM_FALSE;
    }
#endif

#if BM_DATASTORE_IS_IN_MEMORY

    /* validate not already initialized */
    if (header_ptr != BM_NULL) {
        BM_ERROR("Protocol cannot be initialized twice\n");
        return BM_FALSE;
    }

    /* buffer argument must not be null */
    if (datastore_config.buffer == BM_NULL) {
        BM_ERROR("Protocol cannot be initialized with (buffer == NULL)\n");
        return BM_FALSE;
    }

    /* calculate alignment of buffer to 8 byte boundary */
    alignment = ((((bm_uintptr) datastore_config.buffer) + 7) & ~7ul) - ((bm_uintptr) datastore_config.buffer);

    /* validate buffer has enough space for the header */
    if (datastore_config.buffer_length < (HEADER_SIZE_ALIGNED_8 + alignment)) {
        BM_ERROR("Protocol cannot be initialized as buffer length is less that minimum required (%lu vs %lu)\n", datastore_config.buffer_length,
                 alignment + HEADER_SIZE_ALIGNED_8);
        return BM_FALSE;
    }

    /* align buffer */
    datastore_config.buffer += alignment;
    datastore_config.buffer_length -= alignment;

    /* Swap the pointer */
    if (!barman_atomic_cmp_ex_strong_value(&bm_protocol_header_, header_ptr, (struct bm_protocol_header * ) BM_ASSUME_ALIGNED(datastore_config.buffer, 8))) {
        BM_ERROR("Protocol cannot be initialized twice\n");
        return BM_FALSE;
    }

    header_ptr = (struct bm_protocol_header *) BM_ASSUME_ALIGNED(datastore_config.buffer, 8);
    datastore_config.buffer += HEADER_SIZE_ALIGNED_8;
    datastore_config.buffer_length -= HEADER_SIZE_ALIGNED_8;

#else

    /* validate not already initialized */
    /* M profile isn't 64 bit atomic so we just rely on the first half being atomic */
    if (barman_atomic_load((bm_uint32 *)&header_ptr->magic_bytes) == BM_PROTOCOL_MAGIC_BYTES_FIRST_WORD) {
        BM_ERROR("Protocol cannot be initialized twice\n");
        return BM_FALSE;
    }

#endif

    /* initialize header */
    barman_memset(header_ptr, 0, sizeof(*header_ptr));

    header_ptr->protocol_version = BM_PROTOCOL_VERSION;
    header_ptr->header_length = sizeof(*header_ptr);
    header_ptr->data_store_type = BM_CONFIG_USE_DATASTORE;
    header_ptr->last_timestamp = 0;
#if BM_DATASTORE_IS_IN_MEMORY
    header_ptr->data_store_parameters.base_pointer = datastore_config.buffer;
    header_ptr->data_store_parameters.buffer_length = datastore_config.buffer_length;
#else
    header_ptr->data_store_parameters.base_pointer = BM_NULL;
    header_ptr->data_store_parameters.buffer_length = 0;
#endif
    header_ptr->timer_sample_rate = timer_sample_rate;
    header_ptr->config_constants.max_cores = BM_CONFIG_MAX_CORES;
    header_ptr->config_constants.max_task_infos = (BM_CONFIG_MAX_TASK_INFOS > 0 ? BM_CONFIG_MAX_TASK_INFOS : 0);
    header_ptr->config_constants.max_mmap_layout = (BM_CONFIG_MAX_MMAP_LAYOUTS > 0 ? BM_CONFIG_MAX_MMAP_LAYOUTS : 0);
    header_ptr->config_constants.max_pmu_counters = BM_MAX_PMU_COUNTERS;
    header_ptr->config_constants.max_string_table_length = BM_PROTOCOL_STRING_TABLE_LENGTH;
    header_ptr->config_constants.num_custom_counters = (BM_NUM_CUSTOM_COUNTERS > 0 ? BM_NUM_CUSTOM_COUNTERS : 0);
    header_ptr->clock_info.timestamp_base = clock_info->timestamp_base;
    header_ptr->clock_info.timestamp_divisor = clock_info->timestamp_divisor;
    header_ptr->clock_info.timestamp_multiplier = clock_info->timestamp_multiplier;
    header_ptr->clock_info.unix_base_ns = clock_info->unix_base_ns;
    header_ptr->string_table.string_table_length = 0;
    header_ptr->target_name_ptr = barman_protocol_string_table_insert(&header_ptr->string_table, target_name, 255);

#if BM_CONFIG_MAX_TASK_INFOS > 0
    /* add task entries */
    header_ptr->num_task_entries = BM_MIN(num_task_entries, BM_CONFIG_MAX_TASK_INFOS);
    for (index = 0; index < header_ptr->num_task_entries; ++index) {
        barman_protocol_fill_task_record(header_ptr, index, header_ptr->clock_info.timestamp_base, &task_entries[index]);
    }
#endif

#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
    /* add mmap entries */
    header_ptr->num_mmap_layout_entries = BM_MIN(num_mmap_entries, BM_CONFIG_MAX_MMAP_LAYOUTS);
    for (index = 0; index < header_ptr->num_mmap_layout_entries; ++index) {
        barman_protocol_fill_mmap_record(header_ptr, index, header_ptr->clock_info.timestamp_base, &mmap_entries[index]);
    }
#endif

#if BM_NUM_CUSTOM_COUNTERS > 0
    /* add custom counter information */
    header_ptr->num_custom_charts = BM_CUSTOM_CHARTS_COUNT;
    for (index = 0; index < BM_CUSTOM_CHARTS_COUNT; ++index) {
        barman_protocol_fill_custom_chart_record(header_ptr, index, BM_CUSTOM_CHARTS[index]);
    }
    for (index = 0; index < BM_NUM_CUSTOM_COUNTERS; ++index) {
        barman_protocol_fill_custom_chart_series_record(header_ptr, index, BM_CUSTOM_CHARTS_SERIES[index]);
    }
#endif

    /* init data store */
#if BM_DATASTORE_IS_IN_MEMORY
    if (!barman_datastore_initialize(&header_ptr->data_store_parameters)) {
        /* uninitialize */
        barman_atomic_store(&bm_protocol_header_, BM_NULL);
#else
    if (!barman_datastore_initialize(datastore_config)) {
        /* uninitialize */
        barman_atomic_store((bm_uint32 *)&header_ptr->magic_bytes, 0);
#endif

        BM_ERROR("Protocol failed to initialize data store\n");
        return BM_FALSE;
    }

    /* set the magic bytes to indicate initialized */
    ((bm_uint32 *)&header_ptr->magic_bytes)[1] = BM_PROTOCOL_MAGIC_BYTES_SECOND_WORD;
    barman_atomic_store((bm_uint32 *)&header_ptr->magic_bytes, BM_PROTOCOL_MAGIC_BYTES_FIRST_WORD);

    /* notify datastore header changed */
    barman_datastore_notify_header_updated(clock_info->timestamp_base, header_ptr, sizeof(*header_ptr));

    return BM_TRUE;
}

#if BM_CONFIG_MAX_TASK_INFOS > 0

bm_bool barman_add_task_record(bm_uint64 timestamp, const struct bm_protocol_task_info * task_entry)
{
    struct bm_protocol_header * const header_ptr = bm_protocol_header();
    bm_uint32 index;

    /* validate has header configured */
    if ((header_ptr == BM_NULL) || (header_ptr->magic_bytes != BM_PROTOCOL_MAGIC_BYTES)) {
        BM_ERROR("Could not add task info as not initialized\n");
        return BM_FALSE;
    }

    /* use atomic CAS loop to update the index */
    index = barman_atomic_load(&header_ptr->num_task_entries);
    do {
        /* too many items? */
        if (index >= BM_CONFIG_MAX_TASK_INFOS) {
            return BM_FALSE;
        }
        /* perform CAS */
        else if (barman_atomic_cmp_ex_weak_pointer(&header_ptr->num_task_entries, &index, index + 1)) {
            /* update record */
            barman_protocol_fill_task_record(header_ptr, index, timestamp, task_entry);
            /* Update last modified timestamp in header */
            barman_protocol_update_last_sample_timestamp(header_ptr, timestamp);
            /* notify datastore header changed */
            barman_datastore_notify_header_updated(timestamp, header_ptr, sizeof(*header_ptr));
            return BM_TRUE;
        }
        /* failed; retry */
    } while (BM_TRUE);
}

#endif

#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0

bm_bool barman_add_mmap_record(bm_uint64 timestamp, const struct bm_protocol_mmap_layout * mmap_entry)
{
    struct bm_protocol_header * const header_ptr = bm_protocol_header();
    bm_uint32 index;

    /* validate has header configured */
    if ((header_ptr == BM_NULL) || (header_ptr->magic_bytes != BM_PROTOCOL_MAGIC_BYTES)) {
        BM_ERROR("Could not add task info as not initialized\n");
        return BM_FALSE;
    }

    /* update the last_timestamp value */
    barman_protocol_update_last_sample_timestamp(header_ptr, timestamp);

    /* use atomic CAS loop to update the index */
    index = barman_atomic_load(&header_ptr->num_mmap_layout_entries);
    do {
        /* too many items? */
        if (index >= BM_CONFIG_MAX_MMAP_LAYOUTS) {
            return BM_FALSE;
        }
        /* perform CAS */
        else if (barman_atomic_cmp_ex_weak_pointer(&header_ptr->num_mmap_layout_entries, &index, index + 1)) {
            /* update record */
            barman_protocol_fill_mmap_record(header_ptr, index, timestamp, mmap_entry);
            /* Update last modified timestamp in header */
            barman_protocol_update_last_sample_timestamp(header_ptr, timestamp);
            /* notify datastore header changed */
            barman_datastore_notify_header_updated(timestamp, header_ptr, sizeof(*header_ptr));
            return BM_TRUE;
        }
        /* failed; retry */
    } while (BM_TRUE);
}

#endif

bm_bool barman_protocol_write_pmu_settings(bm_uint64 timestamp, bm_uint32 midr, bm_uintptr mpidr, bm_uint32 core, bm_uint32 num_counters, const bm_uint32 * counter_types)
{
    struct bm_protocol_header * const header_ptr = bm_protocol_header();
    bm_uint32 counter;

    /* validate has header configured */
    if ((header_ptr == BM_NULL) || (header_ptr->magic_bytes != BM_PROTOCOL_MAGIC_BYTES)) {
        BM_ERROR("Could not write PMU settings as not initialized\n");
        return BM_FALSE;
    }

    /* validate will fit in data */
    if (core >= BM_COUNT_OF(header_ptr->per_core_pmu_settings)) {
        BM_DEBUG("Could not write PMU settings as core > BM_CONFIG_MAX_CORES\n");
        return BM_FALSE;
    }

    /* validate not overwriting */
    if (header_ptr->per_core_pmu_settings[core].num_counters > 0) {
        BM_DEBUG("Could not write PMU settings already set for core %u\n", core);
        return BM_FALSE;
    }

    /* update the last_timestamp value */
    barman_protocol_update_last_sample_timestamp(header_ptr, timestamp);

    /* write settings */
    header_ptr->per_core_pmu_settings[core].configuration_timestamp = timestamp;
    header_ptr->per_core_pmu_settings[core].midr = midr;
    header_ptr->per_core_pmu_settings[core].mpidr = mpidr;
    header_ptr->per_core_pmu_settings[core].cluster_id = barman_ext_map_multiprocessor_affinity_to_cluster_no(mpidr);
    header_ptr->per_core_pmu_settings[core].num_counters = BM_MIN(num_counters, BM_COUNT_OF(header_ptr->per_core_pmu_settings[core].counter_types));
    for (counter = 0; counter < header_ptr->per_core_pmu_settings[core].num_counters; ++counter) {
        header_ptr->per_core_pmu_settings[core].counter_types[counter] = counter_types[counter];
    }

    /* notify datastore header changed */
    barman_datastore_notify_header_updated(timestamp, header_ptr, sizeof(*header_ptr));

    return BM_TRUE;
}

#if BM_CONFIG_MIN_SAMPLE_PERIOD > 0
bm_uint64 barman_protocol_get_minimum_sample_period(void)
{
    struct bm_protocol_header * const header_ptr = bm_protocol_header();

    /* validate has header configured */
    if ((header_ptr == BM_NULL) || (header_ptr->magic_bytes != BM_PROTOCOL_MAGIC_BYTES)) {
        BM_ERROR("Could not calculate minimum sample period as not initialized\n");
        return ~0; /* a really large sample period should prevent from being sampled */
    }

    return (BM_CONFIG_MIN_SAMPLE_PERIOD * header_ptr->clock_info.timestamp_divisor) / header_ptr->clock_info.timestamp_multiplier;
}
#endif

bm_bool barman_protocol_write_sample(bm_uint64 timestamp, bm_uint32 core,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                     bm_task_id_t task_id,
#endif
                                     const void * pc,
                                     bm_uint32 num_counters, const bm_uint64 * counter_values,
                                     bm_uint32 num_custom_counters, const bm_uint32 * custom_counter_ids, const bm_uint64 * custom_counter_values)
{
    const bm_datastore_block_length length = sizeof(struct bm_protocol_sample) +
                                                (pc != BM_NULL ? sizeof(void *) : 0) +
                                                (num_counters * sizeof(bm_uint64)) +
#if BM_NUM_CUSTOM_COUNTERS > 0
                                                (num_custom_counters * (sizeof(struct bm_protocol_sample_custom_counter_value))) +
#endif
                                                0;
    struct bm_protocol_sample * pointer;
    bm_uint8 * value_pointer;
    bm_uint32 index;

    /* Get the block and fill the header */
    pointer = (struct bm_protocol_sample *) barman_protocol_get_block_and_fill_header(length, core, (pc != BM_NULL ? BM_PROTOCOL_RECORD_SAMPLE_WITH_PC : BM_PROTOCOL_RECORD_SAMPLE), timestamp);
    if (pointer == BM_NULL) {
        return BM_FALSE;
    }

    /* fill the rest */
#if BM_CONFIG_MAX_TASK_INFOS > 0
    pointer->task_id = task_id;
#endif
#if BM_NUM_CUSTOM_COUNTERS > 0
    pointer->num_custom_counters = num_custom_counters;
#endif

    /* append pc */
    if (pc != BM_NULL) {
        bm_uint8 * pc_pointer = ((bm_uint8*) pointer) + sizeof(struct bm_protocol_sample);
        BM_UNALIGNED_CAST_DEREF_ASSIGN(const void*, pc_pointer, pc);
    }

    /* append values */
    value_pointer = (((bm_uint8*) pointer) + sizeof(struct bm_protocol_sample) + (pc != BM_NULL ? sizeof(void *) : 0));
    for (index = 0; index < num_counters; ++index) {
        BM_UNALIGNED_CAST_DEREF_ASSIGN(bm_uint64, value_pointer, counter_values[index]);
        value_pointer += sizeof(bm_uint64);
    }

#if BM_NUM_CUSTOM_COUNTERS > 0
    /* append custom counters */
    struct bm_protocol_sample_custom_counter_value * custom_value_pointer = (struct bm_protocol_sample_custom_counter_value *) value_pointer;
    for (index = 0; index < num_custom_counters; ++index) {
        custom_value_pointer[index].id = custom_counter_ids[index];
        custom_value_pointer[index].value = custom_counter_values[index];
    }
#endif

    /* commit the data */
    barman_datastore_commit_block_and_header(core, (bm_uint8* ) pointer);

    return BM_TRUE;
}

#if BM_CONFIG_MAX_TASK_INFOS > 0
bm_bool barman_protocol_write_task_switch(bm_uint64 timestamp, bm_uint32 core, bm_task_id_t task_id, bm_uint8 reason)
{
    const bm_datastore_block_length length = sizeof(struct bm_protocol_task_switch);
    struct bm_protocol_task_switch * pointer;

    /* Get the block and fill the header */
    pointer = (struct bm_protocol_task_switch *) barman_protocol_get_block_and_fill_header(length, core, BM_PROTOCOL_RECORD_TASK_SWITCH, timestamp);
    if (pointer == BM_NULL) {
        return BM_FALSE;
    }

    /* fill the rest */
    pointer->task_id = task_id;
    pointer->reason = reason;

    /* commit the data */
    barman_datastore_commit_block(core, (bm_uint8* ) pointer);

    return BM_TRUE;
}

#endif

#if BM_NUM_CUSTOM_COUNTERS > 0

bm_bool barman_protocol_write_per_core_custom_counter(bm_uint64 timestamp, bm_uint32 core,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                                      bm_task_id_t task_id,
#endif
                                                      bm_uint32 counter_id, bm_uint64 value)
{
    const bm_datastore_block_length length = sizeof(struct bm_protocol_custom_counter_record);
    struct bm_protocol_custom_counter_record * pointer;

    /* Get the block and fill the header */
    pointer = (struct bm_protocol_custom_counter_record *) barman_protocol_get_block_and_fill_header(length, core, BM_PROTOCOL_RECORD_CUSTOM_COUNTER, timestamp);
    if (pointer == BM_NULL) {
        return BM_FALSE;
    }

    /* fill the rest */
#if BM_CONFIG_MAX_TASK_INFOS > 0
    pointer->task_id = task_id;
#endif
    pointer->counter = counter_id;
    pointer->value = value;

    /* commit the data */
    barman_datastore_commit_block(core, (bm_uint8 *) pointer);

    return BM_TRUE;
}

#endif

bm_bool barman_protocol_write_halt_event(bm_uint64 timestamp, bm_uint32 core, bm_bool entered_halt)
{
    const bm_datastore_block_length length = sizeof(struct bm_protocol_halting_event_record);
    struct bm_protocol_halting_event_record * pointer;

    /* Get the block and fill the header */
    pointer = (struct bm_protocol_halting_event_record *) barman_protocol_get_block_and_fill_header(length, core, BM_PROTOCOL_RECORD_HALT_EVENT, timestamp);
    if (pointer == BM_NULL) {
        return BM_FALSE;
    }

    /* fill the rest */
    pointer->entered_halt = entered_halt;

    /* commit the data */
    barman_datastore_commit_block(core, (bm_uint8 *) pointer);

    return BM_TRUE;
}

bm_bool barman_protocol_write_annotation(bm_uint64 timestamp, bm_uint32 core,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                                         bm_task_id_t task_id,
#endif
                                         bm_uint8 type, bm_uint32 channel, bm_uint32 group, bm_uint32 color, bm_uintptr data_length, const bm_uint8 * data)
{
    bm_datastore_block_length length = sizeof(struct bm_protocol_annotation_record) + data_length;
    struct bm_protocol_annotation_record * pointer;

    /* Get the block and fill the header */
    pointer = (struct bm_protocol_annotation_record *) barman_protocol_get_block_and_fill_header(length, core, BM_PROTOCOL_RECORD_ANNOTATION, timestamp);
    if (pointer == BM_NULL) {
        return BM_FALSE;
    }

    /* fill the rest */
#if BM_CONFIG_MAX_TASK_INFOS > 0
    pointer->task_id = task_id;
#endif
    pointer->data_length = data_length;
    pointer->channel = channel;
    pointer->group = group;
    pointer->color = color;
    pointer->type = type;

    /* add the data on the end */
    barman_memcpy(pointer + 1, data, data_length);

    /* commit the data */
    barman_datastore_commit_block(core, (bm_uint8 *) pointer);

    return BM_TRUE;
}

#if !BM_CONFIG_ENABLE_BUILTIN_MEMFUNCS

void * barman_memset(void * ptr, int value, bm_size_t num)
{
    bm_uint8 * data = (bm_uint8 *) ptr;

    if (num > 0) {
        do {
            *data++ = value;
        } while (--num > 0);
    }

    return ptr;
}

void * barman_memcpy(void * dest, const void * src, bm_size_t num)
{
    bm_size_t i;
    for (i = 0; i < num; ++i)
    {
        ((bm_uint8 *) dest)[i] = ((bm_uint8 *) src)[i];
    }
    return dest;
}

#endif

/**
 * @file
 * @brief   Contains an implementation of the barman-multicore.h interface for
 *          MPCore systems.
 * @details This implementation is suitable for implementations where the barman
 *          agent runs entirely in privileged mode and can use MPIDR register
 *          to uniquely identify a processor.
 */

/** @{ */
#define MPIDR_M_BIT         (BM_UINTPTR(1) << 31)
#define MPIDR_U_BIT         (BM_UINTPTR(1) << 30)
/** @} */

bm_uint32 barman_get_core_no(void)
{
    const bm_uintptr mpidr_value = barman_mpidr();

    if ((mpidr_value & MPIDR_M_BIT) && (mpidr_value & MPIDR_U_BIT)) {
        return 0;
    }

    return barman_ext_map_multiprocessor_affinity_to_core_no(mpidr_value);
}

#if BM_CONFIG_MAX_CORES == 1

#define BM_INVALID_MPIDR   ~BM_UINTPTR(0)

bm_uintptr first_mpidr = BM_INVALID_MPIDR;

bm_uint32 barman_ext_map_multiprocessor_affinity_to_core_no(bm_uintptr mpidr)
{
    bm_uintptr old_val = BM_INVALID_MPIDR;
    if (first_mpidr == mpidr || barman_atomic_cmp_ex_strong_pointer(&first_mpidr, &old_val, mpidr))
    {
        return 0;
    }
    else if (old_val == mpidr)
    {
        return 0;
    }
    else
    {
        return 1; /* An invalid value, doesn't matter what as long as not 0 */
    }
}

bm_uint32 barman_ext_map_multiprocessor_affinity_to_cluster_no(bm_uintptr mpidr)
{
    return 0;
}

#endif /* BM_CONFIG_MAX_CORES == 1 */
#if BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_CIRCULAR_RAM_BUFFER

/**
 * @file    data-store/barman-circular-ram-buffer.c
 * @brief   Implementation of a lock-free circular buffer
 * @details The buffer is implemented as a sort of list of arbitrary length blocks with `head_offset` indicating the offset to the base of the allocated data
 *          (the read offset), and `tail_offset` indicating the offset to the end of the allocated data (the write offset).
 *
 *          The real offsets into the buffer are `head_offset % buffer_length` and `tail_offset % buffer_length`; that is to say that the algorithm will only
 *          ever increment the offsets.
 *
 *          When a user requests a new block, the algorithm will first copy `tail_offset` into `reserved_tail_offset[core]`; this marks the current value of
 *          `tail_offset` as reserved for a given core. This is used to prevent other threads from freeing blocks beyond `tail_offset`; i.e. from trampling on
 *          data that is currently being used by a given core.
 *
 *          The algorithm will then ensure it has enough space to allocate the block (allowing for alignment in the case that the block would otherwise overlap
 *          the end of the buffer as each block returned must be contiguous). It will do this by freeing blocks from `head_offset` upto
 *          `MIN(reserved_tail_offset[0] ... reserved_tail_offset[MAX_CORES - 1], tail_offset)` (excluding any cores that are not reserving any data).
 *
 *          So long as enough space was freed it will then attempt to atomically CAS `tail_offset` with `tail_offset + required_length + alignment_length`. If
 *          the CAS fails, some other thread reserved the space first and the algorithm will restart. If the CAS succeeds it will fill in the appropriate parts
 *          of the block and return the pointer.
 *
 *          When the block is commited some time later, the algorith will update the header with the appropriate value for `write_offset` and will reset
 *          `reserved_tail_offset[core]` to some marker value (`BM_NO_RESERVED_TAIL`) indicating that it is not reserving any space. The algorithm is free to
 *          overwrite the data in the buffer previously reserved by the block at this point.
 *
 * @note    `barman_circular_ram_buffer_get_block` is free to fail if it cannot find enough free space for a block. This will usually happen if
 *          `MIN(reserved_tail_offset[0] ... reserved_tail_offset[MAX_CORES - 1], tail_offset)` is such a value that it cannot free enough space. The rate of
 *          failure is therefore a function of the number of cores accessing the buffer concurrently, the rate at which the blocks are requested and then
 *          commited, and the overall size of the buffer.
 *
 * @note    If a low priority thread were to successfully call `barman_circular_ram_buffer_get_block` and then subsequently be preempted and prevented from
 *          running before calling `barman_circular_ram_buffer_commit_block`, whilst other threads were using the buffer, the buffer would eventually reach a
 *          point where no more blocks could be allocated until the low priority thread is resumed. This situation will not lead to deadlock, but will instead
 *          cause all subsequent get requests to fail.
 */

/* *********************************** */

/**
 * @brief   Defines the data
 * @ingroup bm_data_store_circular_ram_buffer
 */
struct barman_circular_ram_buffer_configuration
{
    /** Header data */
    struct bm_datastore_header_data * header_data;
    /** Current commited write offset for each core */
    bm_atomic_uint64 reserved_tail_offset[BM_CONFIG_MAX_CORES];
    /** Buffer read offset */
    bm_atomic_uint64 head_offset;
    /** Buffer write offset */
    bm_atomic_uint64 tail_offset;
    /** Closed flag */
    bm_atomic_bool closed;
};

/** The configuration settings */
static struct barman_circular_ram_buffer_configuration barman_circular_ram_buffer_configuration = { BM_NULL, { 0 }, 0, 0, BM_TRUE };

/** Marker to indicate that no data is reserved */
#define BM_NO_RESERVED_TAIL     (~0ul)

/* *********************************** */

/**
 * @brief   Align a block size to a multiple of `sizeof(bm_datastore_block_length)`
 * @param   length  the length to align
 * @return  The aligned length
 */
static BM_INLINE bm_datastore_block_length barman_circular_ram_buffer_align_block_size(bm_datastore_block_length length)
{
    return ((length + (sizeof(bm_datastore_block_length) - 1)) & ~((bm_datastore_block_length) (sizeof(bm_datastore_block_length) - 1)));
}

/**
 * @brief   Find the lowest reserved tail value for all cores
 * @param   tail_offset     The current tail offset
 * @param   excluding_core  A core to exclude from the result, or BM_CONFIG_MAX_CORES to include all cores
 * @return  The lowest reserved tail value, or `tail_offset` if no tail values are reserved
 */
static BM_INLINE bm_datastore_header_length barman_circular_ram_buffer_get_reserved_tail(const bm_atomic_uint64 tail_offset, bm_uint32 excluding_core)
{
    bm_uint32 core;
    bm_atomic_uint64 result = tail_offset;

    for (core = 0; core < BM_CONFIG_MAX_CORES; ++core) {
        if (core != excluding_core) {
            const bm_atomic_uint64 offset = barman_atomic_load(&(barman_circular_ram_buffer_configuration.reserved_tail_offset[core]));

            if (offset != BM_NO_RESERVED_TAIL) {
                result = BM_MIN(result, offset);
            }
        }
    }

    return result;
}

/**
 * @brief   Free a single previously allocated block
 * @param   base_pointer    The base address of the buffer
 * @param   buffer_length   The length of the buffer
 * @param   head_offset_ptr [IN/OUT] Contains the current head_offset on entry, and will be updated to contain the new value of head_offset if it changes
 * @param   limit_offset    The limit offset whereby if the block were to move the head_offset past this point, the free fails.
 * @return  BM_TRUE if the block was freed, BM_FALSE if not
 */
BM_NONNULL((1, 3))
static bm_bool barman_circular_ram_buffer_free_block(bm_uint8 * const base_pointer, const bm_datastore_header_length buffer_length,
                                                     bm_atomic_uint64 * head_offset_ptr, const bm_atomic_uint64 limit_offset)
{
    const bm_datastore_header_length real_head_offset = *head_offset_ptr % buffer_length;
    bm_datastore_block_length * const block_pointer = BM_ASSUME_ALIGNED_CAST(bm_datastore_block_length, base_pointer + real_head_offset);

    /* read the length of the block */
    const bm_datastore_block_length user_length = BM_DATASTORE_GET_LENGTH_VALUE(*block_pointer);
    const bm_datastore_block_length block_length = user_length + sizeof(bm_datastore_block_length);

    /* validate we can free the block */
    const bm_atomic_uint64 new_head_offset = (block_length + *head_offset_ptr);
    if (new_head_offset > limit_offset) {
        return BM_FALSE;
    }

    /* try to free the block */
    if (barman_atomic_cmp_ex_strong_pointer(&barman_circular_ram_buffer_configuration.head_offset, head_offset_ptr, new_head_offset)) {
        *head_offset_ptr = new_head_offset;

        /* move the header pointer */
        barman_atomic_store(&barman_circular_ram_buffer_configuration.header_data->read_offset, *head_offset_ptr);
    }

    return BM_TRUE;
}

/**
 * @brief   Free all blocks between head_offset and tail_offset
 * @param   buffer_length   The length of the buffer
 * @param   base_pointer    The buffer base address
 * @param   tail_offset     The limit to free upto
 * @return  BM_TRUE if blocks were freed up to tail_offset, BM_FALSE on error
 */
BM_NONNULL((2))
static bm_bool barman_circular_ram_buffer_free_to_tail(const bm_datastore_header_length buffer_length, bm_uint8 * const base_pointer,
                                                       bm_atomic_uint64 tail_offset)
{
    const bm_atomic_uint64 reserved_tail_pointer = barman_circular_ram_buffer_get_reserved_tail(tail_offset, BM_CONFIG_MAX_CORES);

    if (reserved_tail_pointer < tail_offset) {
        return BM_FALSE;
    }
    else {
        bm_atomic_uint64 head_offset = barman_atomic_load(&barman_circular_ram_buffer_configuration.head_offset);

        /* loop freeing blocks until head_offset == tail_offset */
        while (head_offset < tail_offset) {
            if (!barman_circular_ram_buffer_free_block(base_pointer, buffer_length, &head_offset, tail_offset)) {
                return BM_FALSE;
            }
        }

        /* success */
        return BM_TRUE;
    }
}

/**
 * @brief   Free blocks until there is sufficient space
 * @param   buffer_length   The length of the buffer
 * @param   base_pointer    The buffer base address
 * @param   head_offset_out [OUT] Will contain the final value of head_offset
 * @param   tail_offset     The current value of tail_offset
 * @param   required_length The required amount of space to free
 * @return  BM_TRUE if blocks were freed up to enough free space, BM_FALSE on error
 */
BM_NONNULL((2, 3))
static bm_bool barman_circular_ram_buffer_ensure_free(const bm_datastore_header_length buffer_length, bm_uint8 * const base_pointer,
                                                      bm_atomic_uint64 * head_offset_out, bm_atomic_uint64 tail_offset,
                                                      const bm_datastore_block_length required_length)
{
    const bm_atomic_uint64 reserved_tail_offset = barman_circular_ram_buffer_get_reserved_tail(tail_offset, BM_CONFIG_MAX_CORES);

    bm_atomic_uint64 head_offset = barman_atomic_load(&barman_circular_ram_buffer_configuration.head_offset);

    /* loop freeing blocks until there is enough free space, or until we meet the reserved limit */
    while ((buffer_length - (tail_offset - head_offset)) < required_length) {
        if (!barman_circular_ram_buffer_free_block(base_pointer, buffer_length, &head_offset, reserved_tail_offset)) {
            return BM_FALSE;
        }
    }

    /* success */
    *head_offset_out = head_offset;
    return BM_TRUE;
}

/**
 * @brief   Commit the contents of some block, either a real block or an alignment block
 * @param   core            The core affected
 * @param   block_pointer   The data pointer
 * @param   length          The length of the data
 */
BM_NONNULL((2))
static void barman_circular_ram_buffer_write_commit(bm_uint32 core, bm_uint8 * block_pointer, bm_datastore_block_length length)
{
    bm_datastore_header_length old_write_offset = barman_atomic_load(&(barman_circular_ram_buffer_configuration.header_data->write_offset));
    bm_datastore_header_length new_write_offset;

    /* Clean the cache lines that contain the data */
    barman_cache_clean(block_pointer, length);

    /* increment total */
    barman_atomic_fetch_add(&(barman_circular_ram_buffer_configuration.header_data->total_written), length);

    /* Update the header write_offset */
    do {
        new_write_offset = barman_circular_ram_buffer_get_reserved_tail(barman_atomic_load(&barman_circular_ram_buffer_configuration.tail_offset), core);
    } while (!barman_atomic_cmp_ex_strong_pointer(&(barman_circular_ram_buffer_configuration.header_data->write_offset), &old_write_offset, new_write_offset));

    /* cache clean the header */
    barman_cache_clean(barman_circular_ram_buffer_configuration.header_data, sizeof(*barman_circular_ram_buffer_configuration.header_data));

    /* Clear the commit offset */
    barman_atomic_store(&(barman_circular_ram_buffer_configuration.reserved_tail_offset[core]), BM_NO_RESERVED_TAIL);
}

/* *********************************** */

bm_bool barman_circular_ram_buffer_initialize(struct bm_datastore_header_data * header_data)
{
    bm_uint32 i;

    /* Can only change the settings if closed */
    if (!barman_atomic_cmp_ex_strong_value(&barman_circular_ram_buffer_configuration.closed, BM_TRUE, BM_FALSE)) {
        return BM_FALSE;
    }

    /* Update configuration */
    barman_circular_ram_buffer_configuration.header_data = header_data;
    barman_circular_ram_buffer_configuration.head_offset = 0;
    barman_circular_ram_buffer_configuration.tail_offset = 0;
    barman_circular_ram_buffer_configuration.header_data->read_offset = 0;
    barman_circular_ram_buffer_configuration.header_data->write_offset = 0;
    barman_circular_ram_buffer_configuration.header_data->total_written = 0;
    for (i = 0; i < BM_CONFIG_MAX_CORES; ++i) {
        barman_circular_ram_buffer_configuration.reserved_tail_offset[i] = BM_NO_RESERVED_TAIL;
    }

    /* make sure length is aligned to multiple of sizeof(bm_datastore_block_length) */
    barman_circular_ram_buffer_configuration.header_data->buffer_length &= ~((bm_datastore_header_length) (sizeof(bm_datastore_block_length) - 1));

    barman_dsb();

    return BM_TRUE;
}

bm_uint8 * barman_circular_ram_buffer_get_block(bm_uint32 core, bm_datastore_block_length user_length)
{
    const bm_datastore_block_length aligned_length = barman_circular_ram_buffer_align_block_size(user_length);
    const bm_datastore_block_length required_length = aligned_length + sizeof(bm_datastore_block_length);
    bm_datastore_header_length buffer_length;
    bm_atomic_uint64 tail_offset;
    bm_uint8 * base_pointer;
    bm_uint8 * block_pointer = BM_NULL;

    /* Check length */
    if (aligned_length <= 0) {
        return BM_NULL;
    }

    /* Check core number */
    if (core >= BM_CONFIG_MAX_CORES) {
        return BM_NULL;
    }

    /* Check not already closed */
    if (barman_atomic_load(&barman_circular_ram_buffer_configuration.closed)) {
        return BM_NULL;
    }

    /* validate length */
    base_pointer = barman_circular_ram_buffer_configuration.header_data->base_pointer;
    buffer_length = barman_circular_ram_buffer_configuration.header_data->buffer_length;
    if (required_length > buffer_length) {
        return BM_NULL;
    }

    /* get tail offset */
    tail_offset = barman_atomic_load(&barman_circular_ram_buffer_configuration.tail_offset);

    /* check not already got uncommited block */
    if (!barman_atomic_cmp_ex_strong_value(&(barman_circular_ram_buffer_configuration.reserved_tail_offset[core]), BM_NO_RESERVED_TAIL, tail_offset) != 0) {
        return BM_NULL;
    }

    /*
     * To allocate a block of contiguous memory from the buffer as follows:
     *
     *  1  Reserve some limit point in which no thread can free past
     *  2  Ensure that there is enough space for the new block by freeing records up to the lowest reserve point for any core
     *  3  Adjust the tail to the new pointer
     *
     * Uses atomic CAS to move the tail pointer and will retry if another thread moves the tail in the mean time.
     */

    do {
        bm_atomic_uint64 head_offset, new_tail_offset;
        const bm_datastore_header_length real_tail_offset = tail_offset % buffer_length;
        const bm_datastore_header_length remaining_until_wrap = buffer_length - real_tail_offset;
        const bm_datastore_block_length alignment_size = (remaining_until_wrap >= required_length ? 0 : remaining_until_wrap);
        bm_datastore_block_length * length_pointer;

        /* mark the reserved tail */
        barman_atomic_store(&(barman_circular_ram_buffer_configuration.reserved_tail_offset[core]), tail_offset);

        /* if we need to align to the wrap point, then do that first and 'commit' it as if it were a complete block */
        if (alignment_size > 0) {
            /* just align up if necessary so that (head % buffer_length) is not between [real_tail_offset, buffer_length) */
            if (!barman_circular_ram_buffer_free_to_tail(buffer_length, base_pointer, tail_offset - real_tail_offset)) {
                /* reserved limit met; cannot free enough items */
                barman_atomic_store(&(barman_circular_ram_buffer_configuration.reserved_tail_offset[core]), BM_NO_RESERVED_TAIL);
                return BM_NULL;
            }

            /* at this point there should be enough free space, we increment the tail to include the alignment block */
            new_tail_offset = tail_offset + alignment_size;
            if (!barman_atomic_cmp_ex_strong_pointer(&barman_circular_ram_buffer_configuration.tail_offset, &tail_offset, new_tail_offset)) {
                /* failed; try again */
                continue;
            }

            tail_offset = new_tail_offset;

            /* write the padding block if required to align past the end of the buffer */
            if (alignment_size >= sizeof(bm_datastore_block_length)) {
                /* write a padding block */
                length_pointer = BM_ASSUME_ALIGNED_CAST(bm_datastore_block_length, base_pointer + real_tail_offset);
                *length_pointer = BM_DATASTORE_ENCODE_PADDING_BLOCK(alignment_size - sizeof(bm_datastore_block_length), BM_TRUE);
            }
            else {
                /* too small, just fill with zeros */
                bm_uint8 * pointer = (bm_uint8 *) (base_pointer + real_tail_offset);
                barman_memset(pointer, 0, alignment_size);
            }

            /* commit the alignment block */
            barman_circular_ram_buffer_write_commit(core, (bm_uint8 *) (base_pointer + real_tail_offset), alignment_size);
        }
        /* create our data block */
        else {
            /* ensure we have enough free space */
            if (!barman_circular_ram_buffer_ensure_free(buffer_length, base_pointer, &head_offset, tail_offset, required_length)) {
                /* reserved limit met; cannot free enough items */
                barman_atomic_store(&(barman_circular_ram_buffer_configuration.reserved_tail_offset[core]), BM_NO_RESERVED_TAIL);
                return BM_NULL;
            }

            /* at this point there should be enough free space, we increment the tail */
            new_tail_offset = tail_offset + required_length;
            if (!barman_atomic_cmp_ex_strong_pointer(&barman_circular_ram_buffer_configuration.tail_offset, &tail_offset, new_tail_offset)) {
                /* failed; try again */
                continue;
            }

            /* write the data block, mark as padding for now */
            length_pointer = BM_ASSUME_ALIGNED_CAST(bm_datastore_block_length, base_pointer + ((real_tail_offset + alignment_size) % buffer_length));
            *length_pointer = BM_DATASTORE_ENCODE_PADDING_BLOCK(aligned_length, BM_TRUE);
            block_pointer = ((bm_uint8 *) (length_pointer)) + sizeof(bm_datastore_block_length);
        }
    } while (block_pointer == BM_NULL);

    return block_pointer;
}

void barman_circular_ram_buffer_commit_block(bm_uint32 core, bm_uint8 * user_pointer)
{
    /* Check core number */
    if (core >= BM_CONFIG_MAX_CORES) {
        return;
    }

    /* Check not already closed */
    else if (barman_atomic_load(&barman_circular_ram_buffer_configuration.closed)) {
        return;
    }

    /* Pre tests are valid */
    else {
        const bm_datastore_header_length buffer_length = barman_circular_ram_buffer_configuration.header_data->buffer_length;
        bm_uint8 * const base_pointer = barman_circular_ram_buffer_configuration.header_data->base_pointer;
        bm_uint8 * const block_pointer = user_pointer - sizeof(bm_datastore_block_length);
        bm_uint8 * const buffer_end = base_pointer + buffer_length;
        bm_datastore_block_length * length_pointer = BM_ASSUME_ALIGNED_CAST(bm_datastore_block_length, block_pointer);
        const bm_atomic_uint64 reserved_tail = barman_atomic_load(&(barman_circular_ram_buffer_configuration.reserved_tail_offset[core]));
        bm_datastore_block_length user_length;
        bm_datastore_block_length required_length;

        /* Check pointer in bounds */
        if (block_pointer < base_pointer) {
            return;
        }

        user_length = BM_DATASTORE_GET_LENGTH_VALUE(*length_pointer);
        required_length = user_length + sizeof(bm_datastore_block_length);

        if ((user_length == 0) || ((block_pointer + required_length) > buffer_end)) {
            return;
        }

        /* Check has something to commit */
        if (reserved_tail == BM_NO_RESERVED_TAIL) {
            return;
        }

        /* Mark the length as valid */
        *length_pointer = BM_DATASTORE_ENCODE_PADDING_BLOCK(user_length, BM_FALSE);

        /* commit the data block */
        barman_circular_ram_buffer_write_commit(core, block_pointer, required_length);
    }
}

void barman_circular_ram_buffer_close(void)
{
    barman_atomic_store(&barman_circular_ram_buffer_configuration.closed, BM_TRUE);
}
#endif

#if BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ETM

/**
 * Write as ETM frame
 *
 * @param data    Data to write in the frame
 * @param length  Length of the frame
 */
void barman_etm_write_frame(const bm_uint8 * data, bm_uintptr length);

volatile bm_uint8 barman_write_index[256 + 4] BM_ALIGN(256) __attribute__((visibility("default")));

void barman_etm_write_frame(const bm_uint8 * buffer, bm_uintptr length)
{
    bm_uint8 length_l = (bm_uint8) length;
    bm_uint8 length_h = (bm_uint8) (length >> 8);
    bm_uint16 index;

    if (length > (BM_UINTPTR(1) << 16))
    {
        return;
    }

    barman_write_index[256 + 0] = length_l;
    barman_write_index[length_l] = 0;
    barman_write_index[256 + 1] = length_h;
    barman_write_index[length_h] = 1;

    for (index = 0 ; index < length; ++index) {
        unsigned char data = buffer[index];
        barman_write_index[data] = data;
    }

    barman_write_index[256 + 2] = length_l;
    barman_write_index[length_l] = 2;
    barman_write_index[256 + 3] = length_h;
    barman_write_index[length_h] = 3;
}
#endif

#if BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ITM

#define BM_READ_DEMCR(x)     x = BM_MEMORY_MAPPED_REGISTER_32(0xE000EDFC)
#define BM_WRITE_DEMCR(x)    BM_MEMORY_MAPPED_REGISTER_32(0xE000EDFC) = x

#define BM_DEMCR_TRCENA_BIT      (BM_UINT32(1) << 24)

/**
 * @defgroup    bm_itm     Macros for using ITM
 * @{ */

#define BM_READ_ITM_TER(port_block, x)    x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(itm_base) + 0xE00 + (port_block) * 0x04)
#define BM_READ_ITM_TPR(x)                x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(itm_base) + 0xE40)
#define BM_READ_ITM_TCR(x)                x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(itm_base) + 0xE80)
#define BM_READ_ITM_LSR(x)                x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(itm_base) + 0xFB4)

/* Note we don't use BM_MEMORY_MAPPED_REGISTER here because that is volatile and our atomic
 * functions don't have volatile parameters. However, we make the assumption they handle the
 * address in a volatile way, i.e., they only make exactly one write to the address per call
 * (if the CAS is successful, zero writes otherwise)
 */
#define BM_WRITE_ITM_STIM_WHEN_READY(port, value, bits) \
    while (!barman_atomic_cmp_ex_weak_value((bm_uint ## bits *) ((bm_uintptr)(itm_base) + (port) * 0x04), BM_ITM_STIM_FIFOREADY_BIT, value))

#define BM_WRITE_ITM_STIM_8(port, value)  BM_WRITE_ITM_STIM_WHEN_READY(port, value, 8)
#define BM_WRITE_ITM_STIM_16(port, value) BM_WRITE_ITM_STIM_WHEN_READY(port, value, 16)
#define BM_WRITE_ITM_STIM_32(port, value) BM_WRITE_ITM_STIM_WHEN_READY(port, value, 32)

#define BM_WRITE_ITM_TER(port_block, x)   BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(itm_base) + 0xE00 + (port_block) * 0x04) = x
#define BM_WRITE_ITM_TPR(x)               BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(itm_base) + 0xE40) = x
#define BM_WRITE_ITM_TCR(x)               BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(itm_base) + 0xE80) = x
#define BM_WRITE_ITM_LAR(x)               BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(itm_base) + 0xFB0) = x

/** @} */

#define BM_ITM_STIM_FIFOREADY_BIT    (BM_UINT32(1) << 0)

#define BM_ITM_TCR_BUSY_BIT          (BM_UINT32(1) << 23)
#define BM_ITM_TCR_TRACEID_MASK      BM_UINT32(0x007f0000)
#define BM_ITM_TCR_TRACEID_SHIFT     16
#define BM_ITM_TCR_GTSFREQ_MASK      BM_UINT32(0xc00)
#define BM_ITM_TCR_GTSFREQ_SHIFT     10
#define BM_ITM_TCR_TSPrescale_MASK   BM_UINT32(0x300)
#define BM_ITM_TCR_TSPrescale_SHIFT  8
#define BM_ITM_TCR_SWOENA_BIT        (BM_UINT32(1) << 4)
#define BM_ITM_TCR_TXENA_BIT         (BM_UINT32(1) << 3)
#define BM_ITM_TCR_SYNCENA_BIT       (BM_UINT32(1) << 2)
#define BM_ITM_TCR_TSENA_BIT         (BM_UINT32(1) << 1)
#define BM_ITM_TCR_ITMENA_BIT        (BM_UINT32(1) << 0)

#ifndef BM_ITM_TRACE_ID
#define BM_ITM_TRACE_ID -1
#endif

/** Address of the ITM registers */
static void * itm_base = BM_NULL;

static void enable_ports(void)
{
    bm_uint32 ter;

    const bm_uint8 max_port_block = (BM_CONFIG_ITM_NUMBER_OF_PORTS + BM_CONFIG_ITM_MIN_PORT_NUMBER) / 32;
    const bm_uint8 min_port_block = BM_CONFIG_ITM_MIN_PORT_NUMBER / 32;
    const bm_uint8 max_port_bit = (BM_CONFIG_ITM_NUMBER_OF_PORTS + BM_CONFIG_ITM_MIN_PORT_NUMBER) % 32;
    const bm_uint8 min_port_bit = BM_CONFIG_ITM_MIN_PORT_NUMBER % 32;

    if (min_port_block == max_port_block)
    {
        BM_READ_ITM_TER(min_port_block, ter);
        ter |= ~(~BM_UINT32(0) << (max_port_bit - min_port_bit)) << min_port_bit;
        BM_WRITE_ITM_TER(min_port_block, ter);
    }
    else
    {
        int i;

        BM_READ_ITM_TER(min_port_block, ter);
        ter |= ~BM_UINT32(0) << min_port_bit;
        BM_WRITE_ITM_TER(min_port_block, ter);

        for (i = min_port_block + 1; i < max_port_block; ++i)
        {
            BM_WRITE_ITM_TER(i, ~BM_UINT32(0));
        }

        if (max_port_bit > 0)
        {
            BM_READ_ITM_TER(max_port_block, ter);
            ter |= BM_UINT32(1) << (max_port_bit - 1);
            BM_WRITE_ITM_TER(max_port_block, ter);
        }
    }
}

static void enable_unprivileged_access(void)
{
    bm_uint32 tpr;

    const bm_uint8 num_priv_bits = (BM_CONFIG_ITM_NUMBER_OF_PORTS + 7) / 8;
    const bm_uint8 min_priv_bit = BM_CONFIG_ITM_MIN_PORT_NUMBER / 8;

    BM_READ_ITM_TPR(tpr);
    tpr &= ~(~(~BM_UINT32(0) << num_priv_bits) << min_priv_bit);
    BM_WRITE_ITM_TPR(tpr);
}

bm_bool barman_itm_init(struct bm_datastore_config_itm config)
{
    bm_uint32 tcr;

    itm_base = config.registers;

    if (itm_base == BM_NULL)
    {
        return BM_FALSE;
    }

#if BM_ARM_ARCH_PROFILE == 'M'
    {
        /* Enable trace */
        bm_uint32 demcr;
        BM_READ_DEMCR(demcr);
        demcr |= BM_DEMCR_TRCENA_BIT;
        BM_WRITE_DEMCR(demcr);
    }
#endif

    /* Unlock the configuration registers */
    BM_WRITE_ITM_LAR(0xC5ACCE55);

    /* disable */
    BM_READ_ITM_TCR(tcr);
    BM_WRITE_ITM_TCR(tcr & ~BM_ITM_TCR_ITMENA_BIT);

    /* and wait till not busy */
    do BM_READ_ITM_TCR(tcr);
    while (tcr & BM_ITM_TCR_BUSY_BIT);

    enable_ports();
    enable_unprivileged_access();

    /* Set the trace ID if needed */
#if BM_ITM_TRACE_ID >= 0
    tcr &= ~BM_ITM_TCR_TRACEID_MASK;
    tcr |= (BM_ITM_TRACE_ID << BM_ITM_TCR_TRACEID_SHIFT) & BM_ITM_TCR_TRACEID_MASK;
#endif

    /* Disable global timestamps */
    tcr &= ~BM_ITM_TCR_GTSFREQ_MASK;

    /* Don't prescale local timestamps */
    tcr &= ~BM_ITM_TCR_TSPrescale_MASK;

    /* Use processor clock for local timestamps */
    tcr &= ~BM_ITM_TCR_SWOENA_BIT;

    /* Enable the DWT packets */
    tcr |= BM_ITM_TCR_TXENA_BIT;

    /* Enable the synchronization packets */
    tcr |= BM_ITM_TCR_SYNCENA_BIT;

    /* Enable local timestamps */
    tcr |= BM_ITM_TCR_TSENA_BIT;

    /* Enable the ITM */
    tcr |= BM_ITM_TCR_ITMENA_BIT;

    BM_WRITE_ITM_TCR(tcr);

    return BM_TRUE;
}

void barman_itm_write_frame(const bm_uint8 * data, bm_uintptr length, bm_uint16 port, bm_bool flush)
{
    const bm_uintptr word_start_address = (((bm_uintptr)data + 3) & ~0x3ul);
    const bm_uintptr word_end_address = ((bm_uintptr)data + length) & ~0x3ul;

    const bm_uint8 * start_bytes = data;
    const bm_uintptr num_start_bytes = word_start_address - (bm_uintptr)data;

    const bm_uint32 * aligned_words = (const bm_uint32 *) word_start_address;
    const bm_uintptr num_aligned_words = (word_end_address - word_start_address) / 4;

    const bm_uint8 * end_bytes = (const bm_uint8 *) word_end_address;
    const bm_uintptr num_end_bytes = length - 4 * num_aligned_words - num_start_bytes;

    bm_uintptr i;

    /* Write the data out to ITM using a 16 bit packet to mark the start of a frame */
    BM_WRITE_ITM_STIM_16(port, 0xffffu);

    /* Write out the rest */

    for (i = 0; i < num_start_bytes; ++i)
    {
        BM_WRITE_ITM_STIM_8(port, start_bytes[i]);
    }

    for (i = 0; i < num_aligned_words; ++i)
    {
        bm_uint32 word = aligned_words[i];
#if BM_BIG_ENDIAN
        /* Our decoder assumes the packets are little endian */
        word = BM_SWAP_ENDIANESS_32(word);
#endif

        BM_WRITE_ITM_STIM_32(port, word);
    }

    for (i = 0; i < num_end_bytes; ++i)
    {
        BM_WRITE_ITM_STIM_8(port, end_bytes[i]);
    }

    if (flush)
    {
        /*
         * Writing a 16 bit packet ends the frame without having to wait
         * for the start of the next frame.
         */
        BM_WRITE_ITM_STIM_16(port, 0xffffu);
    }
}
#endif

#if BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_LINEAR_RAM_BUFFER

/* *********************************** */

/**
 * @brief   Defines the data
 * @ingroup bm_data_store_linear_ram_buffer
 */
struct barman_linear_ram_buffer_configuration
{
    /** Header data */
    struct bm_datastore_header_data * header_data;
    /** Current commited write offset for each core */
    bm_datastore_header_length commited_offset[BM_CONFIG_MAX_CORES];
    /** Current write offset */
    bm_datastore_header_length write_offset;
    /** Closed flag */
    bm_atomic_bool closed;
};

/** The configuration settings */
static struct barman_linear_ram_buffer_configuration barman_linear_ram_buffer_configuration = { BM_NULL, { 0 }, 0, BM_TRUE };

/** Marker to indicate that a call to get_block was made on a core but is not yet complete */
#define BM_CORE_IS_BUSY     (~0ul)

/* *********************************** */

/**
 * @brief   Align a block size to a multiple of `sizeof(bm_datastore_block_length)`
 * @param   length  the length to align
 * @return  The aligned length
 */
static BM_INLINE bm_datastore_block_length barman_linear_ram_buffer_align_block_size(bm_datastore_block_length length)
{
    return ((length + (sizeof(bm_datastore_block_length) - 1)) & ~((bm_datastore_block_length) (sizeof(bm_datastore_block_length) - 1)));
}

/* *********************************** */

bm_bool barman_linear_ram_buffer_initialize(struct bm_datastore_header_data * header_data)
{
    /* Can only change the settings if closed */
    if (!barman_atomic_cmp_ex_strong_value(&barman_linear_ram_buffer_configuration.closed, BM_TRUE, BM_FALSE)) {
        return BM_FALSE;
    }

    /* Update configuration */
    barman_linear_ram_buffer_configuration.header_data = header_data;
    barman_linear_ram_buffer_configuration.write_offset = 0;
    barman_linear_ram_buffer_configuration.header_data->read_offset = 0;
    barman_linear_ram_buffer_configuration.header_data->write_offset = 0;
    barman_linear_ram_buffer_configuration.header_data->total_written = 0;

    barman_dsb();

    return BM_TRUE;
}

bm_uint8 * barman_linear_ram_buffer_get_block(bm_uint32 core, bm_datastore_block_length user_length)
{
    const bm_datastore_block_length aligned_length = barman_linear_ram_buffer_align_block_size(user_length);
    const bm_datastore_block_length required_length = aligned_length + sizeof(bm_datastore_block_length);
    bm_datastore_header_length old_write_offset = barman_atomic_load(&barman_linear_ram_buffer_configuration.write_offset);
    bm_datastore_header_length new_write_offset;
    bm_uint8 * block_pointer;

    /* Check length */
    if (aligned_length <= 0) {
        return BM_NULL;
    }

    /* Check core number */
    if (core >= BM_CONFIG_MAX_CORES) {
        return BM_NULL;
    }

    /* Check not already closed */
    if (barman_atomic_load(&barman_linear_ram_buffer_configuration.closed)) {
        return BM_NULL;
    }

    /* check not already got uncommited block */
    if (!barman_atomic_cmp_ex_strong_value(&(barman_linear_ram_buffer_configuration.commited_offset[core]), 0, BM_CORE_IS_BUSY) != 0) {
        return BM_NULL;
    }

    /* Update the write offset atomically */
    do {
        /* NB: old_write_offset is modified by barman_atomic_cmp_ex_weak_pointer if it fails */
        new_write_offset = old_write_offset + required_length;
    } while (!barman_atomic_cmp_ex_weak_pointer(&barman_linear_ram_buffer_configuration.write_offset, &old_write_offset, new_write_offset));

    /* Validate the new write offset */
    if (new_write_offset > barman_linear_ram_buffer_configuration.header_data->buffer_length) {
        /* It is safe to subtract the length we previously added to revert the header since any subsequent calls to
         * get_block will have only incremented the write_offset futher past the end */
        barman_atomic_sub_fetch(&barman_linear_ram_buffer_configuration.write_offset, required_length);
        return BM_NULL;
    }

    /* Configure the result */
    block_pointer = (barman_linear_ram_buffer_configuration.header_data->base_pointer + old_write_offset);

    /* Write the length value, but mark the block invalid */
    *BM_ASSUME_ALIGNED_CAST(bm_datastore_block_length, block_pointer) = BM_DATASTORE_ENCODE_PADDING_BLOCK(aligned_length, BM_TRUE);

    /* Write the commit offset */
    barman_atomic_store(&(barman_linear_ram_buffer_configuration.commited_offset[core]), new_write_offset);

    return block_pointer + sizeof(bm_datastore_block_length);
}

void barman_linear_ram_buffer_commit_block(bm_uint32 core, bm_uint8 * user_pointer)
{
    /* Check core number */
    if (core >= BM_CONFIG_MAX_CORES) {
        return;
    }

    /* Check not already closed */
    else if (barman_atomic_load(&barman_linear_ram_buffer_configuration.closed)) {
        return;
    }

    /* Pre tests are valid */
    else {
        bm_uint8 * const block_pointer = user_pointer - sizeof(bm_datastore_block_length);
        bm_datastore_block_length * length_pointer = BM_ASSUME_ALIGNED_CAST(bm_datastore_block_length, block_pointer);
        const bm_datastore_header_length new_write_offset = barman_atomic_load(&(barman_linear_ram_buffer_configuration.commited_offset[core]));
        bm_uint8 * const buffer_end = (barman_linear_ram_buffer_configuration.header_data->base_pointer
                + barman_atomic_load(&barman_linear_ram_buffer_configuration.write_offset));
        bm_datastore_header_length old_write_offset = barman_atomic_load(&barman_linear_ram_buffer_configuration.header_data->write_offset);
        bm_datastore_block_length user_length;
        bm_datastore_block_length required_length;

        /* Check pointer in bounds */
        if (block_pointer < barman_linear_ram_buffer_configuration.header_data->base_pointer) {
            return;
        }

        user_length = BM_DATASTORE_GET_LENGTH_VALUE(*length_pointer);
        required_length = user_length + sizeof(bm_datastore_block_length);

        if ((user_length == 0) || ((block_pointer + required_length) > buffer_end)) {
            return;
        }

        /* Check has something to commit */
        if ((new_write_offset == 0) || (new_write_offset == BM_CORE_IS_BUSY)) {
            return;
        }

        /* Write the length value, now marked as valid */
        *length_pointer = BM_DATASTORE_ENCODE_PADDING_BLOCK(user_length, BM_FALSE);

        /* Clean the cache lines that contain the data */
        barman_cache_clean(block_pointer, required_length);

        /* increment total */
        barman_atomic_fetch_add(&(barman_linear_ram_buffer_configuration.header_data->total_written), required_length);

        /* Update the header write_offset, only if it is not already greater than the per core offset.
         * This ensures that if the user stops the core using the debugger to read the data, then the header has a
         * marginally better chance that the last entry in the buffer is not invalid data as compared to if we
         * updated the header write_offset in get_block. */
        do {
            /* NB: old_write_offset is modified by barman_atomic_cmp_ex_weak_pointer if it fails */
            /* Do not update if the write offset is already greater */
            if (old_write_offset >= new_write_offset) {
                break;
            }
        } while (!barman_atomic_cmp_ex_weak_pointer(&barman_linear_ram_buffer_configuration.header_data->write_offset, &old_write_offset, new_write_offset));

        /* cache clean the header */
        barman_cache_clean(barman_linear_ram_buffer_configuration.header_data, sizeof(*barman_linear_ram_buffer_configuration.header_data));

        /* Clear the commit offset */
        barman_atomic_store(&(barman_linear_ram_buffer_configuration.commited_offset[core]), 0);
    }
}

void barman_linear_ram_buffer_close(void)
{
    barman_atomic_store(&barman_linear_ram_buffer_configuration.closed, BM_TRUE);
}
#endif

#if BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_STM

/**
 * @defgroup    bm_stm     Macros for using STM
 * @{ */

#define BM_WRITE_STM_EXTENDED_STIMULUS(base_address, channel, trace_packet_type, value, bits) \
    BM_MEMORY_MAPPED_REGISTER_ ## bits ((bm_uintptr)(base_address) + (channel) * 0x100 + trace_packet_type) = value

#define BM_WRITE_STM_EXTENDED_STIMULUS_8(base_address, channel, trace_packet_type, value) \
    BM_WRITE_STM_EXTENDED_STIMULUS(base_address, channel, trace_packet_type, value, 8)

#define BM_WRITE_STM_EXTENDED_STIMULUS_16(base_address, channel, trace_packet_type, value) \
    BM_WRITE_STM_EXTENDED_STIMULUS(base_address, channel, trace_packet_type, value, 16)

#define BM_WRITE_STM_EXTENDED_STIMULUS_32(base_address, channel, trace_packet_type, value) \
    BM_WRITE_STM_EXTENDED_STIMULUS(base_address, channel, trace_packet_type, value, 32)

#define BM_WRITE_STM_EXTENDED_STIMULUS_64(base_address, channel, trace_packet_type, value) \
    BM_WRITE_STM_EXTENDED_STIMULUS(base_address, channel, trace_packet_type, value, 64)

/* Guaranteed data accesses */
#define BM_STM_G_DMTS   0x00 /* Data, marked with timestamp, guaranteed */
#define BM_STM_G_DM     0x08 /* Data, marked, guaranteed */
#define BM_STM_G_DTS    0x10 /* Data, with timestamp, guaranteed */
#define BM_STM_G_D      0x18 /* Data, guaranteed */
/* Guaranteed non-data accesses */
#define BM_STM_G_FLAGTS 0x60 /* Flag with timestamp, guaranteed */
#define BM_STM_G_FLAG   0x68 /* Flag, guaranteed */
#define BM_STM_G_TRIGTS 0x70 /* Trigger with timestamp, guaranteed */
#define BM_STM_G_TRIG   0x78 /* Trigger, guaranteed */
/* Invariant Timing data accesses */
#define BM_STM_I_DMTS   0x80 /* Data, marked with timestamp, invariant timing */
#define BM_STM_I_DM     0x88 /* Data, marked, invariant timing */
#define BM_STM_I_DTS    0x90 /* Data, with timestamp, invariant timing */
#define BM_STM_I_D      0x98 /* Data, invariant timing */
/* Invariant Timing non-data accesses */
#define BM_STM_I_FLAGTS 0xE0 /* Flag with timestamp, invariant timing */
#define BM_STM_I_FLAG   0xE8 /* Flag, invariant timing */
#define BM_STM_I_TRIGTS 0xF0 /* Trigger with timestamp, invariant timing */
#define BM_STM_I_TRIG   0xF8 /* Trigger, invariant timing */

/* Stimulus Port Control Registers */
#define BM_READ_STMSPER(control_block, x)         x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE00)
#define BM_READ_STMSPTER(control_block, x)        x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE20)
#define BM_READ_STMPRIVMASKR(control_block, x)    x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE40)
#define BM_READ_STMSPSCR(control_block, x)        x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE60)
#define BM_READ_STMSPMSCR(control_block, x)       x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE64)
#define BM_READ_STMSPOVERIDER(control_block, x)   x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE68)
#define BM_READ_STMSPMOVERIDER(control_block, x)  x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE6C)
#define BM_READ_STMSPTRIGCSR(control_block, x)    x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE70)

#define BM_WRITE_STMSPER(control_block, x)        BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE00) = x
#define BM_WRITE_STMSPTER(control_block, x)       BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE20) = x
#define BM_WRITE_STMPRIVMASKR(control_block, x)   BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE40) = x
#define BM_WRITE_STMSPSCR(control_block, x)       BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE60) = x
#define BM_WRITE_STMSPMSCR(control_block, x)      BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE64) = x
#define BM_WRITE_STMSPOVERIDER(control_block, x)  BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE68) = x
#define BM_WRITE_STMSPMOVERIDER(control_block, x) BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE6C) = x
#define BM_WRITE_STMSPTRIGCSR(control_block, x)   BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE70) = x

/* Primary Control and Status Registers */
#define BM_READ_STMTCSR(control_block, x)         x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE80)
  /* write-only STMTSSTIMR */
#define BM_READ_STMTSFREQR(control_block, x)      x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE8C)
#define BM_READ_STMSYNCR(control_block, x)        x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE90)
#define BM_READ_STMAUXCR(control_block, x)        x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE94)

#define BM_WRITE_STMTCSR(control_block, x)        BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE80) = x
#define BM_WRITE_STMTSSTIMR(control_block, x)     BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE84) = x
#define BM_WRITE_STMTSFREQR(control_block, x)     BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE8C) = x
#define BM_WRITE_STMSYNCR(control_block, x)       BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE90) = x
#define BM_WRITE_STMAUXCR(control_block, x)       BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xE94) = x

/* Identification Registers */
#define BM_READ_STMFEAT1R(control_block, x)       x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xEA0)
#define BM_READ_STMFEAT2R(control_block, x)       x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xEA4)
#define BM_READ_STMFEAT3R(control_block, x)       x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xEA8)

/* CoreSight Management Registers */
#define BM_READ_STMITCTRL(control_block, x)       x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xF00)
#define BM_READ_STMCLAIMSET(control_block, x)     x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xFA0)
#define BM_READ_STMCLAIMCLR(control_block, x)     x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xFA4)
  /* write-only STMLAR */
#define BM_READ_STMLSR(control_block, x)          x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xFB4)
#define BM_READ_STMAUTHSTATUS(control_block, x)   x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xFB8)
#define BM_READ_STMDEVARCH(control_block, x)      x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xFBC)
#define BM_READ_STMDEVID(control_block, x)        x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xFC8)
#define BM_READ_STMDEVTYPE(control_block, x)      x = BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xFCC)

#define BM_WRITE_STMITCTRL(control_block, x)      BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xF00) = x
#define BM_WRITE_STMCLAIMSET(control_block, x)    BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xFA0) = x
#define BM_WRITE_STMCLAIMCLR(control_block, x)    BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xFA4) = x
#define BM_WRITE_STMLAR(control_block, x)         BM_MEMORY_MAPPED_REGISTER_32((bm_uintptr)(control_block) + 0xFB0) = x

/** @} */

#define BM_STMTCSR_TRACEID_SHIFT (16)
#define BM_STMTCSR_TRACEID_MASK  (0x7f0000)
#define BM_STMTCSR_EN_BIT        (0x1)

#ifndef BM_STM_TRACE_ID
#define BM_STM_TRACE_ID -1
#endif

/** Address of the STM extended stimulus ports */
static void * extended_stimulus_ports = BM_NULL;

bm_bool barman_stm_init(struct bm_datastore_config_stm config)
{
    bm_uint32 tcsr;

    extended_stimulus_ports = config.extended_stimulus_ports;

    if (extended_stimulus_ports == BM_NULL)
    {
        return BM_FALSE;
    }

    if (config.configuration_registers != BM_NULL)
    {
        /* Unlock the configuration registers */
        BM_WRITE_STMLAR(config.configuration_registers, 0xC5ACCE55);

        /* Enable all ports; we assume the user would've passed NULL
         * and configured it them selves if they didn't want that */
        BM_WRITE_STMSPER(config.configuration_registers, 0xFFFFFFFF);

        BM_READ_STMTCSR(config.configuration_registers, tcsr);

        /* Set the trace ID if needed */
#if BM_STM_TRACE_ID >= 0
        tcsr &= ~BM_STMTCSR_TRACEID_MASK;
        tcsr |= (BM_STM_TRACE_ID << BM_STMTCSR_TRACEID_SHIFT) & BM_STMTCSR_TRACEID_MASK;
#endif

        /* Enable STM */
        tcsr |= BM_STMTCSR_EN_BIT;

        BM_WRITE_STMTCSR(config.configuration_registers, tcsr);
    }

    return BM_TRUE;
}

void barman_stm_write_frame(const bm_uint8 * data, bm_uintptr length, bm_uint16 channel, bm_bool flush)
{
    const bm_uintptr word_start_address = (((bm_uintptr)data + 3) & ~0x3ul);
    const bm_uintptr word_end_address = ((bm_uintptr)data + length) & ~0x3ul;

    const bm_uint8 * start_bytes = data;
    const bm_uintptr num_start_bytes = word_start_address - (bm_uintptr)data;

    const bm_uint32 * aligned_words = (const bm_uint32 *) word_start_address;
    const bm_uintptr num_aligned_words = (word_end_address - word_start_address) / 4;

    const bm_uint8 * end_bytes = (const bm_uint8 *) word_end_address;
    const bm_uintptr num_end_bytes = length - 4 * num_aligned_words - num_start_bytes;

    bm_uintptr i;

    /* Write the data out to STM using a flag to mark the start of a frame */
    BM_WRITE_STM_EXTENDED_STIMULUS_8(extended_stimulus_ports, channel, BM_STM_G_FLAG, 0);

    /* Write out the rest without markers */

    for (i = 0; i < num_start_bytes; ++i)
    {
        BM_WRITE_STM_EXTENDED_STIMULUS_8(extended_stimulus_ports, channel, BM_STM_G_D, start_bytes[i]);
    }

    for (i = 0; i < num_aligned_words; ++i)
    {
        bm_uint32 word = aligned_words[i];
#if BM_BIG_ENDIAN
        /* Our decoder assumes the packets are little endian */
        word = BM_SWAP_ENDIANESS_32(word);
#endif

        BM_WRITE_STM_EXTENDED_STIMULUS_32(extended_stimulus_ports, channel, BM_STM_G_D, word);
    }

    for (i = 0; i < num_end_bytes; ++i)
    {
        BM_WRITE_STM_EXTENDED_STIMULUS_8(extended_stimulus_ports, channel, BM_STM_G_D, end_bytes[i]);
    }

    if (flush)
    {
        /* Writing a flag ends the frame without having to wait for the start of the next frame */
        BM_WRITE_STM_EXTENDED_STIMULUS_8(extended_stimulus_ports, channel, BM_STM_G_FLAG, 0);
    }
}

#endif

#if BM_DATASTORE_USES_STREAMING_INTERFACE

/**
 * Initialize the backend
 *
 * @param config    Pointer to some configuration data
 * @return          True if successful
 */
bm_bool barman_ext_streaming_backend_init(void * config);

/**
 * Write data as a frame
 *
 * @param data    Data to write in the frame
 * @param length  Length of the frame
 * @param channel Channel to write the frame on
 * @param flush   Whether to flush the channel after writing the frame (may have some overhead)
 */
void barman_ext_streaming_backend_write_frame(const bm_uint8 * data, bm_uintptr length, bm_uint16 channel, bm_bool flush);

/**
 * Shutdown the backend
 */
void barman_ext_streaming_backend_close(void);

/**
 * Get the channel bank
 *
 * If banked by core this is just the core number
 * If not banked this should always be 0
 *
 * @return The bank
 */
bm_uint32 barman_ext_streaming_backend_get_bank(void);

#if BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ITM
#define BM_NUMBER_OF_STREAMING_CHANNELS                                    BM_CONFIG_ITM_NUMBER_OF_PORTS
#define BM_NUMBER_OF_STREAMING_BANKS                                       1
#define barman_streaming_backend_get_bank()                                0
#define barman_streaming_backend_init(config)                              barman_itm_init(config)
#define barman_streaming_backend_write_frame(data, length, channel, flush) barman_itm_write_frame(data, length, (channel) + BM_CONFIG_ITM_MIN_PORT_NUMBER, flush)
#define barman_streaming_backend_close()

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_STM
#define BM_NUMBER_OF_STREAMING_CHANNELS                                    BM_CONFIG_STM_NUMBER_OF_CHANNELS
/* STM is banked by master ID which could be per core or cluster but lets assume not banked to cover all cases */
#define BM_NUMBER_OF_STREAMING_BANKS                                       1
#define barman_streaming_backend_get_bank()                                0
#define barman_streaming_backend_init(config)                              barman_stm_init(config)
#define barman_streaming_backend_write_frame(data, length, channel, flush) barman_stm_write_frame(data, length, (channel) + BM_CONFIG_STM_MIN_CHANNEL_NUMBER, flush)
#define barman_streaming_backend_close()

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ETM
/* ETM is banked by core and does not have separate channels */
#define BM_NUMBER_OF_STREAMING_CHANNELS                                    1
#define BM_NUMBER_OF_STREAMING_BANKS                                       BM_CONFIG_MAX_CORES
#define barman_streaming_backend_get_bank()                                barman_get_core_no()
#define barman_streaming_backend_init(config)                              (*((bm_uint8 *) &config) != 0)
#define barman_streaming_backend_write_frame(data, length, channel, flush) do { (void)(channel); (void)(flush); barman_etm_write_frame(data, length); } while (0)
#define barman_streaming_backend_close()

#elif BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_STREAMING_USER_SUPPLIED
#ifndef BM_CONFIG_STREAMING_DATASTORE_USER_SUPPLIED_NUMBER_OF_CHANNELS
#error "BM_CONFIG_STREAMING_DATASTORE_USER_SUPPLIED_NUMBER_OF_CHANNELS must be defined"
#endif
#define BM_NUMBER_OF_STREAMING_CHANNELS                                    BM_CONFIG_STREAMING_DATASTORE_USER_SUPPLIED_NUMBER_OF_CHANNELS
#ifndef BM_CONFIG_STREAMING_DATASTORE_USER_SUPPLIED_NUMBER_OF_BANKS
#error "BM_CONFIG_STREAMING_DATASTORE_USER_SUPPLIED_NUMBER_OF_BANKS must be defined"
#endif
#define BM_NUMBER_OF_STREAMING_BANKS                                       BM_CONFIG_STREAMING_DATASTORE_USER_SUPPLIED_NUMBER_OF_BANKS
#define barman_streaming_backend_get_bank()                                barman_ext_streaming_backend_get_bank()
#define barman_streaming_backend_init(config)                              barman_ext_streaming_backend_init(config)
#define barman_streaming_backend_write_frame(data, length, channel, flush) barman_ext_streaming_backend_write_frame(data, length, channel, flush)
#define barman_streaming_backend_close()                                   barman_ext_streaming_backend_close()

#else /* Define some dummy values so it always compiles without warnings */
#define BM_NUMBER_OF_STREAMING_CHANNELS                                    1
#define BM_NUMBER_OF_STREAMING_BANKS                                       1
#define barman_streaming_backend_get_bank()                                0
#define barman_streaming_backend_init(config)                              (*((bm_uint8 *) &config) != 0)
#define barman_streaming_backend_write_frame(data, length, channel, flush) do { (void)(data); (void)(length); (void)(channel); (void)(flush);} while (0)
#define barman_streaming_backend_close()

#endif

#define BM_STREAMING_BUFFER_SIZE 0x100

#define BM_NUMBER_OF_STREAMING_BUFFERS    (BM_NUMBER_OF_STREAMING_BANKS * BM_NUMBER_OF_STREAMING_CHANNELS)

/* *********************************** */

/**
 * @brief   Defines the data
 * @ingroup bm_data_store_streaming_interface
 */
struct barman_streaming_interface_configuration
{
    /** status flag */
    bm_atomic_uintptr status;
    /** A flag for each buffer to indicate it's busy */
    bm_atomic_bool buffer_is_busy[BM_NUMBER_OF_STREAMING_BUFFERS];
};

#define BM_STATUS_CLOSED    0
#define BM_STATUS_OPEN      1
#define BM_STATUS_CHANGING  2

/** The configuration settings */
static struct barman_streaming_interface_configuration barman_streaming_interface_configuration = { BM_STATUS_CLOSED, { BM_FALSE } };

/** The temporary buffer to store data that is to be written out via STREAMING */
static bm_uint8 buffers[BM_STREAMING_BUFFER_SIZE * BM_NUMBER_OF_STREAMING_BUFFERS] BM_ALIGN(16);

/**
 * Gets an available buffer
 *
 * @return A free buffer (0 based) or -1 if they are all busy
 */
static bm_int32 get_a_buffer()
{
    bm_uint16 buffer_no;
    const bm_uint16 bank_start = barman_streaming_backend_get_bank() * BM_NUMBER_OF_STREAMING_CHANNELS;

    if (bank_start >= BM_NUMBER_OF_STREAMING_BUFFERS)
    {
        BM_ERROR("barman_streaming_backend_get_bank() returned value >= BM_NUMBER_OF_STREAMING_BUFFERS");
        return -1;
    }

    /* Find a buffer_no not already in use */
    for (buffer_no = bank_start; buffer_no < bank_start + BM_NUMBER_OF_STREAMING_CHANNELS; ++buffer_no)
    {
        if (barman_atomic_cmp_ex_strong_value(&(barman_streaming_interface_configuration.buffer_is_busy[buffer_no]), BM_FALSE, BM_TRUE)) {
            break;
        }
    }

    if (buffer_no == bank_start + BM_NUMBER_OF_STREAMING_CHANNELS)
    {
        /* Couldn't find a spare buffer */
        return -1;
    }

    return buffer_no;
}

/**
 * Returns a buffer allowing it to be reused
 *
 * @param buffer_no    The buffer to return (0 based)
 */
static void return_buffer(bm_uint16 buffer_no)
{
    /* Clear buffer busy flag */
    barman_atomic_store(&(barman_streaming_interface_configuration.buffer_is_busy[buffer_no]), BM_FALSE);
}

/* *********************************** */

bm_bool barman_streaming_interface_initialize(bm_datastore_config datastore_config)
{
    /* Can only change the settings if closed */
    if (!barman_atomic_cmp_ex_strong_value(&barman_streaming_interface_configuration.status, BM_STATUS_CLOSED, BM_STATUS_CHANGING)) {
        return BM_FALSE;
    }

    if (barman_streaming_backend_init(datastore_config))
    {
        barman_atomic_store(&(barman_streaming_interface_configuration.status), BM_STATUS_OPEN);
        return BM_TRUE;
    }

    /* Failed to initialize, so close again */
    barman_atomic_store(&(barman_streaming_interface_configuration.status), BM_STATUS_CLOSED);
    return BM_FALSE;
}

bm_uint8 * barman_streaming_interface_get_block(bm_datastore_block_length user_length)
{
    const bm_datastore_block_length required_length = user_length + sizeof(bm_datastore_block_length);
    bm_uint8 * block_pointer;
    bm_int32 buffer_no;

    /* Check length */
    if (user_length == 0 || required_length > BM_STREAMING_BUFFER_SIZE) {
        return BM_NULL;
    }

    /* Check not already closed */
    if (barman_atomic_load(&barman_streaming_interface_configuration.status) != BM_STATUS_OPEN) {
        return BM_NULL;
    }

    buffer_no = get_a_buffer();

    if (buffer_no == -1)
    {
        return BM_NULL;
    }

    /* Configure the result */
    block_pointer = buffers + BM_STREAMING_BUFFER_SIZE * buffer_no;

    /* Write the length value */
    *BM_ASSUME_ALIGNED_CAST(bm_datastore_block_length, block_pointer) = user_length;

    return block_pointer + sizeof(bm_datastore_block_length);
}

void barman_streaming_interface_commit_block(bm_uint8 * user_pointer)
{
    bm_uint8 * const block_pointer = user_pointer - sizeof(bm_datastore_block_length);
    bm_uintptr const byte_offset = block_pointer - buffers;
    const bm_uint16 buffer_no = byte_offset / BM_STREAMING_BUFFER_SIZE;
    const bm_uint16 bank = buffer_no / BM_NUMBER_OF_STREAMING_CHANNELS;
    const bm_uint16 channel = buffer_no % BM_NUMBER_OF_STREAMING_CHANNELS;

    /* Check pointer and buffer is valid */
    if ((byte_offset % BM_STREAMING_BUFFER_SIZE) != 0 ||
        buffer_no >= BM_NUMBER_OF_STREAMING_BUFFERS ||
        bank != barman_streaming_backend_get_bank() ||
        !barman_atomic_load(&barman_streaming_interface_configuration.buffer_is_busy[buffer_no])) {
        return;
    }

    /* buffer is now thought to be valid */

    /* Check not already closed */
    if (barman_atomic_load(&barman_streaming_interface_configuration.status) == BM_STATUS_OPEN) {
        /* Pre tests are valid */
        bm_datastore_block_length * length_pointer = BM_ASSUME_ALIGNED_CAST(bm_datastore_block_length, block_pointer);
        bm_datastore_block_length user_length;
        bm_datastore_block_length required_length;

        user_length = BM_DATASTORE_GET_LENGTH_VALUE(*length_pointer);
        required_length = user_length + sizeof(bm_datastore_block_length);

        if ((user_length != 0) && (required_length <= BM_STREAMING_BUFFER_SIZE - sizeof(bm_datastore_block_length))) {
            barman_streaming_backend_write_frame(user_pointer, user_length, channel, BM_FALSE);
        }

    }

    return_buffer(buffer_no);
}

void barman_streaming_interface_close(void)
{
    barman_atomic_store(&barman_streaming_interface_configuration.status, BM_STATUS_CHANGING);
    barman_streaming_backend_close();
    barman_atomic_store(&barman_streaming_interface_configuration.status, BM_STATUS_CLOSED);
}

BM_NONNULL((2))
void barman_streaming_interface_notify_header_updated(bm_uint64 timestamp, const void * header, bm_uintptr length)
{
    /* We don't actually need the buffer but we need to reserve the associated channel */
    const bm_int32 buffer_no = get_a_buffer();
    if (buffer_no != -1)
    {
        const bm_uint16 channel = buffer_no % BM_NUMBER_OF_STREAMING_CHANNELS;
        barman_streaming_backend_write_frame((const bm_uint8 *)header, length, channel, BM_TRUE);
        return_buffer(buffer_no);
    }
}
#endif
#if !BM_CONFIG_USER_SUPPLIED_PMU_DRIVER
#if BM_ARM_64BIT_STATE

void barman_cache_clean(void * pointer, bm_uintptr length)
{
    const bm_uintptr last_address = ((bm_uintptr) pointer) + length;

    bm_uintptr ctr_val;
    bm_uintptr cache_line_size;
    bm_uintptr aligned_address;

    /* validate args */
    if (length == 0) {
        return;
    }

    /* read cache type */
    asm volatile("MRS %0, CTR_EL0" : "=r" (ctr_val));

    /* get minimum data cache line */
    cache_line_size = 4 << ((ctr_val >> 16) & 15);
    if (cache_line_size == 0) {
        return;
    }

    /* align the starting address */
    aligned_address = ((bm_uintptr) pointer) & ~(cache_line_size - 1);

    /* clean cache lines */
    while (aligned_address < last_address) {
        asm volatile ("DC CVAC, %0" :: "r" (aligned_address));
        aligned_address += cache_line_size;
    }

    barman_dsb();
}

BM_WEAK bm_uintptr barman_ext_disable_interrupts_local(void)
{
    bm_uintptr result;

    asm volatile("MRS %0, DAIF" : "=r" (result));
    asm volatile("MSR DAIFSET, #2");

    return result;
}

BM_WEAK void barman_ext_enable_interrupts_local(bm_uintptr previous_state)
{
    asm volatile("MSR DAIF, %0" :: "r" (previous_state));
}
#elif BM_ARM_32BIT_STATE

void barman_cache_clean(void * pointer, bm_uintptr length)
{
    const bm_uintptr last_address = ((bm_uintptr) pointer) + length;

    bm_uintptr ctr_val;
    bm_uintptr cache_line_size;
    bm_uintptr aligned_address;

    /* validate args */
    if (length == 0) {
        return;
    }

    /* read cache type */
    BM_READ_SYS_REG(0, 0, 0, 1, ctr_val); /* CTR */

    /* get minimum data cache line */
    cache_line_size = 4 << ((ctr_val >> 16) & 15);
    if (cache_line_size == 0) {
        return;
    }

    /* align the starting address */
    aligned_address = ((bm_uintptr) pointer) & ~(cache_line_size - 1);

    /* clean cache lines */
    while (aligned_address < last_address) {
        BM_WRITE_SYS_REG(0, 7, 10, 1, aligned_address); /* DCCMVAC */
        aligned_address += cache_line_size;
    }

    barman_dsb();
}

BM_WEAK bm_uintptr barman_ext_disable_interrupts_local(void)
{
    bm_uintptr result;

#if BM_ARM_ARCH_PROFILE == 'M'
    BM_READ_SPECIAL_REG(FAULTMASK, result);
    asm ("CPSID f");
#else
    BM_READ_SPECIAL_REG(CPSR, result);
    asm ("CPSID if");
#endif

    return result;
}

BM_WEAK void barman_ext_enable_interrupts_local(bm_uintptr previous_state)
{
#if BM_ARM_ARCH_PROFILE == 'M'
    BM_WRITE_SPECIAL_REG(FAULTMASK, previous_state);
#else
    BM_WRITE_SPECIAL_REG(CPSR_c, previous_state);
#endif
}

#if BM_CONFIG_USE_DATASTORE == BM_CONFIG_USE_DATASTORE_ITM
#define BM_USER_SUPPLIED_TIMESTAMP 0
#else
#define BM_USER_SUPPLIED_TIMESTAMP 1
#endif

#if !BM_USER_SUPPLIED_TIMESTAMP
BM_WEAK bm_uint64 barman_ext_get_timestamp(void)
{
    /* arbitrary unused value */
    return 0;
}
#endif
#endif

/* We define our own to reduce dependency on external headers, and to avoid
 * conflicts with any definitions in other headers where barman headers
 * are included */

/**
 * @defgroup    bm_pmu_intrinsics   Macros for reading and writing PMU registers
 * @{
 *
 * @def         BM_PMU_AT_LEAST_V2
 * @brief       Defines whether targeting PMUv2 or later
 *
 * @def         BM_PMU_AT_LEAST_V3
 * @brief       Defines whether targeting PMUv3 or later
 *
 * @def         BM_PMU_AT_LEAST_V3_1
 * @brief       Defines whether targeting PMUv3 with ARMv8.1 extension or later
 *
 * @def         BM_READ_PMxx(out)
 * @brief       Read from PMU register PMxx (or register that architecturally maps to it)
 * @param       out Type "bm_uinptr"
 *              L-value to write the result into.
 *
 * @def         BM_WRITE_PMxx(in)
 * @brief       Write to PMU register PMxx (or register that architecturally maps to it)
 * @param       in Type "bm_uinptr"
 *              Value to write into the register.
 *
 * @def         BM_READ_PMxx_64(out)
 * @brief       Read 64 bits from PMU register PMxx (or register that architecturally maps to it)
 * @param       out Type "bm_uint64"
 *              L-value to write the result into.
 *
 * @def         BM_WRITE_PMxx_64(in)
 * @brief       Write 64 bits to PMU register PMxx (or register that architecturally maps to it)
 * @param       in Type "bm_uint64"
 *              Value to write into the register.
 *
 * @}
 */

/** @{ */

#define BM_PMU_AT_LEAST_V2   1
#define BM_PMU_AT_LEAST_V3   (BM_ARM_TARGET_ARCH >= 8)
#define BM_PMU_AT_LEAST_V3_1 (BM_ARM_TARGET_ARCH >= 801)

#if BM_PMU_AT_LEAST_V2

#if BM_ARM_32BIT_STATE

#define BM_READ_PMCR(x)          BM_READ_SYS_REG(0,  9, 12, 0, x)
#define BM_READ_PMCNTENSET(x)    BM_READ_SYS_REG(0,  9, 12, 1, x)
#define BM_READ_PMCNTENCLR(x)    BM_READ_SYS_REG(0,  9, 12, 2, x)
#define BM_READ_PMOVSR(x)        BM_READ_SYS_REG(0,  9, 12, 3, x)
  /* WRITE ONLY PMSWINC */
#define BM_READ_PMSELR(x)        BM_READ_SYS_REG(0,  9, 12, 5, x)
#define BM_READ_PMCEID0(x)       BM_READ_SYS_REG(0,  9, 12, 6, x)
#define BM_READ_PMCEID1(x)       BM_READ_SYS_REG(0,  9, 12, 7, x)

#define BM_READ_PMCCNTR(x)       BM_READ_SYS_REG(0,  9, 13, 0, x)
#define BM_READ_PMXEVTYPER(x)    BM_READ_SYS_REG(0,  9, 13, 1, x)
#define BM_READ_PMXEVCNTR(x)     BM_READ_SYS_REG(0,  9, 13, 2, x)

#define BM_READ_PMUSERENR(x)     BM_READ_SYS_REG(0,  9, 14, 0, x)
#define BM_READ_PMINTENSET(x)    BM_READ_SYS_REG(0,  9, 14, 1, x)
#define BM_READ_PMINTENCLR(x)    BM_READ_SYS_REG(0,  9, 14, 2, x)
#define BM_READ_PMOVSSET(x)      BM_READ_SYS_REG(0,  9, 14, 3, x)

#define BM_WRITE_PMCR(x)         BM_WRITE_SYS_REG(0,  9, 12, 0, x)
#define BM_WRITE_PMCNTENSET(x)   BM_WRITE_SYS_REG(0,  9, 12, 1, x)
#define BM_WRITE_PMCNTENCLR(x)   BM_WRITE_SYS_REG(0,  9, 12, 2, x)
#define BM_WRITE_PMOVSR(x)       BM_WRITE_SYS_REG(0,  9, 12, 3, x)
#define BM_WRITE_PMSWINC(x)      BM_WRITE_SYS_REG(0,  9, 12, 4, x)
#define BM_WRITE_PMSELR(x)       BM_WRITE_SYS_REG(0,  9, 12, 5, x)
    /* READ ONLY PMCEID0 */
    /* READ ONLY PMCEID1 */

#define BM_WRITE_PMCCNTR(x)      BM_WRITE_SYS_REG(0,  9, 13, 0, x)
#define BM_WRITE_PMXEVTYPER(x)   BM_WRITE_SYS_REG(0,  9, 13, 1, x)
#define BM_WRITE_PMXEVCNTR(x)    BM_WRITE_SYS_REG(0,  9, 13, 2, x)

#define BM_WRITE_PMUSERENR(x)    BM_WRITE_SYS_REG(0,  9, 14, 0, x)
#define BM_WRITE_PMINTENSET(x)   BM_WRITE_SYS_REG(0,  9, 14, 1, x)
#define BM_WRITE_PMINTENCLR(x)   BM_WRITE_SYS_REG(0,  9, 14, 2, x)
#define BM_WRITE_PMOVSSET(x)     BM_WRITE_SYS_REG(0,  9, 14, 3, x)

#elif BM_ARM_64BIT_STATE

#define BM_READ_PMCR(x)          BM_READ_SYS_REG_NAMED(PMCR_EL0,       x)
#define BM_READ_PMCNTENSET(x)    BM_READ_SYS_REG_NAMED(PMCNTENSET_EL0, x)
#define BM_READ_PMCNTENCLR(x)    BM_READ_SYS_REG_NAMED(PMCNTENCLR_EL0, x)
#define BM_READ_PMOVSR(x)        BM_READ_SYS_REG_NAMED(PMOVSCLR_EL0,   x)
  /* WRITE ONLY PMSWINC */
#define BM_READ_PMSELR(x)        BM_READ_SYS_REG_NAMED(PMSELR_EL0,     x)
#define BM_READ_PMCEID0(x)       BM_READ_SYS_REG_NAMED(PMCEID0_EL0,    x)
#define BM_READ_PMCEID1(x)       BM_READ_SYS_REG_NAMED(PMCEID1_EL0,    x)

#define BM_READ_PMCCNTR(x)       BM_READ_SYS_REG_NAMED(PMCCNTR_EL0,    x)
#define BM_READ_PMXEVTYPER(x)    BM_READ_SYS_REG_NAMED(PMXEVTYPER_EL0, x)
#define BM_READ_PMXEVCNTR(x)     BM_READ_SYS_REG_NAMED(PMXEVCNTR_EL0,  x)

#define BM_READ_PMUSERENR(x)     BM_READ_SYS_REG_NAMED(PMUSERENR_EL0,  x)
#define BM_READ_PMINTENSET(x)    BM_READ_SYS_REG_NAMED(PMINTENSET_EL1, x)
#define BM_READ_PMINTENCLR(x)    BM_READ_SYS_REG_NAMED(PMINTENCLR_EL1, x)
#define BM_READ_PMOVSSET(x)      BM_READ_SYS_REG_NAMED(PMOVSSET_EL0,   x)

#define BM_WRITE_PMCR(x)         BM_WRITE_SYS_REG_NAMED(PMCR_EL0,       x)
#define BM_WRITE_PMCNTENSET(x)   BM_WRITE_SYS_REG_NAMED(PMCNTENSET_EL0, x)
#define BM_WRITE_PMCNTENCLR(x)   BM_WRITE_SYS_REG_NAMED(PMCNTENCLR_EL0, x)
#define BM_WRITE_PMOVSR(x)       BM_WRITE_SYS_REG_NAMED(PMOVSCLR_EL0,   x)
#define BM_WRITE_PMSWINC(x)      BM_WRITE_SYS_REG_NAMED(PMSWINC_EL0,    x)
#define BM_WRITE_PMSELR(x)       BM_WRITE_SYS_REG_NAMED(PMSELR_EL0,     x)
    /* READ ONLY PMCEID0 */
    /* READ ONLY PMCEID1 */

#define BM_WRITE_PMCCNTR(x)      BM_WRITE_SYS_REG_NAMED(PMCCNTR_EL0,    x)
#define BM_WRITE_PMXEVTYPER(x)   BM_WRITE_SYS_REG_NAMED(PMXEVTYPER_EL0, x)
#define BM_WRITE_PMXEVCNTR(x)    BM_WRITE_SYS_REG_NAMED(PMXEVCNTR_EL0,  x)

#define BM_WRITE_PMUSERENR(x)    BM_WRITE_SYS_REG_NAMED(PMUSERENR_EL0,  x)
#define BM_WRITE_PMINTENSET(x)   BM_WRITE_SYS_REG_NAMED(PMINTENSET_EL1, x)
#define BM_WRITE_PMINTENCLR(x)   BM_WRITE_SYS_REG_NAMED(PMINTENCLR_EL1, x)
#define BM_WRITE_PMOVSSET(x)     BM_WRITE_SYS_REG_NAMED(PMOVSSET_EL0,   x)

#endif

/* NOT reentrant */
#define BM_READ_PMEVCNTR_NR(n, x)    do { BM_WRITE_PMSELR(n);  barman_isb(); BM_READ_PMXEVCNTR(x);   } while (0)
#define BM_READ_PMEVTYPER_NR(n, x)   do { BM_WRITE_PMSELR(n);  barman_isb(); BM_READ_PMXEVTYPER(x);  } while (0)
#define BM_READ_PMCCFILTR_NR(x)      do { BM_WRITE_PMSELR(31); barman_isb(); BM_READ_PMXEVTYPER(x);  } while (0)
#define BM_WRITE_PMEVCNTR_NR(n, x)   do { BM_WRITE_PMSELR(n);  barman_isb(); BM_WRITE_PMXEVCNTR(x);  } while (0)
#define BM_WRITE_PMEVTYPER_NR(n, x)  do { BM_WRITE_PMSELR(n);  barman_isb(); BM_WRITE_PMXEVTYPER(x); } while (0)
#define BM_WRITE_PMCCFILTR_NR(x)     do { BM_WRITE_PMSELR(31); barman_isb(); BM_WRITE_PMXEVTYPER(x); } while (0)

#endif /* BM_PMU_AT_LEAST_V2 */

#if BM_PMU_AT_LEAST_V3

#if BM_ARM_32BIT_STATE

#define BM_READ_PMEVCNTR0(x)      BM_READ_SYS_REG(0, 14,  8, 0, x)
#define BM_READ_PMEVCNTR1(x)      BM_READ_SYS_REG(0, 14,  8, 1, x)
#define BM_READ_PMEVCNTR2(x)      BM_READ_SYS_REG(0, 14,  8, 2, x)
#define BM_READ_PMEVCNTR3(x)      BM_READ_SYS_REG(0, 14,  8, 3, x)
#define BM_READ_PMEVCNTR4(x)      BM_READ_SYS_REG(0, 14,  8, 4, x)
#define BM_READ_PMEVCNTR5(x)      BM_READ_SYS_REG(0, 14,  8, 5, x)
#define BM_READ_PMEVCNTR6(x)      BM_READ_SYS_REG(0, 14,  8, 6, x)
#define BM_READ_PMEVCNTR7(x)      BM_READ_SYS_REG(0, 14,  8, 7, x)

#define BM_READ_PMEVCNTR8(x)      BM_READ_SYS_REG(0, 14,  9, 0, x)
#define BM_READ_PMEVCNTR9(x)      BM_READ_SYS_REG(0, 14,  9, 1, x)
#define BM_READ_PMEVCNTR10(x)     BM_READ_SYS_REG(0, 14,  9, 2, x)
#define BM_READ_PMEVCNTR11(x)     BM_READ_SYS_REG(0, 14,  9, 3, x)
#define BM_READ_PMEVCNTR12(x)     BM_READ_SYS_REG(0, 14,  9, 4, x)
#define BM_READ_PMEVCNTR13(x)     BM_READ_SYS_REG(0, 14,  9, 5, x)
#define BM_READ_PMEVCNTR14(x)     BM_READ_SYS_REG(0, 14,  9, 6, x)
#define BM_READ_PMEVCNTR15(x)     BM_READ_SYS_REG(0, 14,  9, 7, x)

#define BM_READ_PMEVCNTR16(x)     BM_READ_SYS_REG(0, 14, 10, 0, x)
#define BM_READ_PMEVCNTR17(x)     BM_READ_SYS_REG(0, 14, 10, 1, x)
#define BM_READ_PMEVCNTR18(x)     BM_READ_SYS_REG(0, 14, 10, 2, x)
#define BM_READ_PMEVCNTR19(x)     BM_READ_SYS_REG(0, 14, 10, 3, x)
#define BM_READ_PMEVCNTR20(x)     BM_READ_SYS_REG(0, 14, 10, 4, x)
#define BM_READ_PMEVCNTR21(x)     BM_READ_SYS_REG(0, 14, 10, 5, x)
#define BM_READ_PMEVCNTR22(x)     BM_READ_SYS_REG(0, 14, 10, 6, x)
#define BM_READ_PMEVCNTR23(x)     BM_READ_SYS_REG(0, 14, 10, 7, x)

#define BM_READ_PMEVCNTR24(x)     BM_READ_SYS_REG(0, 14, 11, 0, x)
#define BM_READ_PMEVCNTR25(x)     BM_READ_SYS_REG(0, 14, 11, 1, x)
#define BM_READ_PMEVCNTR26(x)     BM_READ_SYS_REG(0, 14, 11, 2, x)
#define BM_READ_PMEVCNTR27(x)     BM_READ_SYS_REG(0, 14, 11, 3, x)
#define BM_READ_PMEVCNTR28(x)     BM_READ_SYS_REG(0, 14, 11, 4, x)
#define BM_READ_PMEVCNTR29(x)     BM_READ_SYS_REG(0, 14, 11, 5, x)
#define BM_READ_PMEVCNTR30(x)     BM_READ_SYS_REG(0, 14, 11, 6, x)

#define BM_READ_PMEVTYPER0(x)     BM_READ_SYS_REG(0, 14, 12, 0, x)
#define BM_READ_PMEVTYPER1(x)     BM_READ_SYS_REG(0, 14, 12, 1, x)
#define BM_READ_PMEVTYPER2(x)     BM_READ_SYS_REG(0, 14, 12, 2, x)
#define BM_READ_PMEVTYPER3(x)     BM_READ_SYS_REG(0, 14, 12, 3, x)
#define BM_READ_PMEVTYPER4(x)     BM_READ_SYS_REG(0, 14, 12, 4, x)
#define BM_READ_PMEVTYPER5(x)     BM_READ_SYS_REG(0, 14, 12, 5, x)
#define BM_READ_PMEVTYPER6(x)     BM_READ_SYS_REG(0, 14, 12, 6, x)
#define BM_READ_PMEVTYPER7(x)     BM_READ_SYS_REG(0, 14, 12, 7, x)

#define BM_READ_PMEVTYPER8(x)     BM_READ_SYS_REG(0, 14, 13, 0, x)
#define BM_READ_PMEVTYPER9(x)     BM_READ_SYS_REG(0, 14, 13, 1, x)
#define BM_READ_PMEVTYPER10(x)    BM_READ_SYS_REG(0, 14, 13, 2, x)
#define BM_READ_PMEVTYPER11(x)    BM_READ_SYS_REG(0, 14, 13, 3, x)
#define BM_READ_PMEVTYPER12(x)    BM_READ_SYS_REG(0, 14, 13, 4, x)
#define BM_READ_PMEVTYPER13(x)    BM_READ_SYS_REG(0, 14, 13, 5, x)
#define BM_READ_PMEVTYPER14(x)    BM_READ_SYS_REG(0, 14, 13, 6, x)
#define BM_READ_PMEVTYPER15(x)    BM_READ_SYS_REG(0, 14, 13, 7, x)

#define BM_READ_PMEVTYPER16(x)    BM_READ_SYS_REG(0, 14, 14, 0, x)
#define BM_READ_PMEVTYPER17(x)    BM_READ_SYS_REG(0, 14, 14, 1, x)
#define BM_READ_PMEVTYPER18(x)    BM_READ_SYS_REG(0, 14, 14, 2, x)
#define BM_READ_PMEVTYPER19(x)    BM_READ_SYS_REG(0, 14, 14, 3, x)
#define BM_READ_PMEVTYPER20(x)    BM_READ_SYS_REG(0, 14, 14, 4, x)
#define BM_READ_PMEVTYPER21(x)    BM_READ_SYS_REG(0, 14, 14, 5, x)
#define BM_READ_PMEVTYPER22(x)    BM_READ_SYS_REG(0, 14, 14, 6, x)
#define BM_READ_PMEVTYPER23(x)    BM_READ_SYS_REG(0, 14, 14, 7, x)

#define BM_READ_PMEVTYPER24(x)    BM_READ_SYS_REG(0, 14, 15, 0, x)
#define BM_READ_PMEVTYPER25(x)    BM_READ_SYS_REG(0, 14, 15, 1, x)
#define BM_READ_PMEVTYPER26(x)    BM_READ_SYS_REG(0, 14, 15, 2, x)
#define BM_READ_PMEVTYPER27(x)    BM_READ_SYS_REG(0, 14, 15, 3, x)
#define BM_READ_PMEVTYPER28(x)    BM_READ_SYS_REG(0, 14, 15, 4, x)
#define BM_READ_PMEVTYPER29(x)    BM_READ_SYS_REG(0, 14, 15, 5, x)
#define BM_READ_PMEVTYPER30(x)    BM_READ_SYS_REG(0, 14, 15, 6, x)
#define BM_READ_PMCCFILTR(x)      BM_READ_SYS_REG(0, 14, 15, 7, x)

#define BM_WRITE_PMEVCNTR0(x)     BM_WRITE_SYS_REG(0, 14,  8, 0, x)
#define BM_WRITE_PMEVCNTR1(x)     BM_WRITE_SYS_REG(0, 14,  8, 1, x)
#define BM_WRITE_PMEVCNTR2(x)     BM_WRITE_SYS_REG(0, 14,  8, 2, x)
#define BM_WRITE_PMEVCNTR3(x)     BM_WRITE_SYS_REG(0, 14,  8, 3, x)
#define BM_WRITE_PMEVCNTR4(x)     BM_WRITE_SYS_REG(0, 14,  8, 4, x)
#define BM_WRITE_PMEVCNTR5(x)     BM_WRITE_SYS_REG(0, 14,  8, 5, x)
#define BM_WRITE_PMEVCNTR6(x)     BM_WRITE_SYS_REG(0, 14,  8, 6, x)
#define BM_WRITE_PMEVCNTR7(x)     BM_WRITE_SYS_REG(0, 14,  8, 7, x)

#define BM_WRITE_PMEVCNTR8(x)     BM_WRITE_SYS_REG(0, 14,  9, 0, x)
#define BM_WRITE_PMEVCNTR9(x)     BM_WRITE_SYS_REG(0, 14,  9, 1, x)
#define BM_WRITE_PMEVCNTR10(x)    BM_WRITE_SYS_REG(0, 14,  9, 2, x)
#define BM_WRITE_PMEVCNTR11(x)    BM_WRITE_SYS_REG(0, 14,  9, 3, x)
#define BM_WRITE_PMEVCNTR12(x)    BM_WRITE_SYS_REG(0, 14,  9, 4, x)
#define BM_WRITE_PMEVCNTR13(x)    BM_WRITE_SYS_REG(0, 14,  9, 5, x)
#define BM_WRITE_PMEVCNTR14(x)    BM_WRITE_SYS_REG(0, 14,  9, 6, x)
#define BM_WRITE_PMEVCNTR15(x)    BM_WRITE_SYS_REG(0, 14,  9, 7, x)

#define BM_WRITE_PMEVCNTR16(x)    BM_WRITE_SYS_REG(0, 14, 10, 0, x)
#define BM_WRITE_PMEVCNTR17(x)    BM_WRITE_SYS_REG(0, 14, 10, 1, x)
#define BM_WRITE_PMEVCNTR18(x)    BM_WRITE_SYS_REG(0, 14, 10, 2, x)
#define BM_WRITE_PMEVCNTR19(x)    BM_WRITE_SYS_REG(0, 14, 10, 3, x)
#define BM_WRITE_PMEVCNTR20(x)    BM_WRITE_SYS_REG(0, 14, 10, 4, x)
#define BM_WRITE_PMEVCNTR21(x)    BM_WRITE_SYS_REG(0, 14, 10, 5, x)
#define BM_WRITE_PMEVCNTR22(x)    BM_WRITE_SYS_REG(0, 14, 10, 6, x)
#define BM_WRITE_PMEVCNTR23(x)    BM_WRITE_SYS_REG(0, 14, 10, 7, x)

#define BM_WRITE_PMEVCNTR24(x)    BM_WRITE_SYS_REG(0, 14, 11, 0, x)
#define BM_WRITE_PMEVCNTR25(x)    BM_WRITE_SYS_REG(0, 14, 11, 1, x)
#define BM_WRITE_PMEVCNTR26(x)    BM_WRITE_SYS_REG(0, 14, 11, 2, x)
#define BM_WRITE_PMEVCNTR27(x)    BM_WRITE_SYS_REG(0, 14, 11, 3, x)
#define BM_WRITE_PMEVCNTR28(x)    BM_WRITE_SYS_REG(0, 14, 11, 4, x)
#define BM_WRITE_PMEVCNTR29(x)    BM_WRITE_SYS_REG(0, 14, 11, 5, x)
#define BM_WRITE_PMEVCNTR30(x)    BM_WRITE_SYS_REG(0, 14, 11, 6, x)

#define BM_WRITE_PMEVTYPER0(x)    BM_WRITE_SYS_REG(0, 14, 12, 0, x)
#define BM_WRITE_PMEVTYPER1(x)    BM_WRITE_SYS_REG(0, 14, 12, 1, x)
#define BM_WRITE_PMEVTYPER2(x)    BM_WRITE_SYS_REG(0, 14, 12, 2, x)
#define BM_WRITE_PMEVTYPER3(x)    BM_WRITE_SYS_REG(0, 14, 12, 3, x)
#define BM_WRITE_PMEVTYPER4(x)    BM_WRITE_SYS_REG(0, 14, 12, 4, x)
#define BM_WRITE_PMEVTYPER5(x)    BM_WRITE_SYS_REG(0, 14, 12, 5, x)
#define BM_WRITE_PMEVTYPER6(x)    BM_WRITE_SYS_REG(0, 14, 12, 6, x)
#define BM_WRITE_PMEVTYPER7(x)    BM_WRITE_SYS_REG(0, 14, 12, 7, x)

#define BM_WRITE_PMEVTYPER8(x)    BM_WRITE_SYS_REG(0, 14, 13, 0, x)
#define BM_WRITE_PMEVTYPER9(x)    BM_WRITE_SYS_REG(0, 14, 13, 1, x)
#define BM_WRITE_PMEVTYPER10(x)   BM_WRITE_SYS_REG(0, 14, 13, 2, x)
#define BM_WRITE_PMEVTYPER11(x)   BM_WRITE_SYS_REG(0, 14, 13, 3, x)
#define BM_WRITE_PMEVTYPER12(x)   BM_WRITE_SYS_REG(0, 14, 13, 4, x)
#define BM_WRITE_PMEVTYPER13(x)   BM_WRITE_SYS_REG(0, 14, 13, 5, x)
#define BM_WRITE_PMEVTYPER14(x)   BM_WRITE_SYS_REG(0, 14, 13, 6, x)
#define BM_WRITE_PMEVTYPER15(x)   BM_WRITE_SYS_REG(0, 14, 13, 7, x)

#define BM_WRITE_PMEVTYPER16(x)   BM_WRITE_SYS_REG(0, 14, 14, 0, x)
#define BM_WRITE_PMEVTYPER17(x)   BM_WRITE_SYS_REG(0, 14, 14, 1, x)
#define BM_WRITE_PMEVTYPER18(x)   BM_WRITE_SYS_REG(0, 14, 14, 2, x)
#define BM_WRITE_PMEVTYPER19(x)   BM_WRITE_SYS_REG(0, 14, 14, 3, x)
#define BM_WRITE_PMEVTYPER20(x)   BM_WRITE_SYS_REG(0, 14, 14, 4, x)
#define BM_WRITE_PMEVTYPER21(x)   BM_WRITE_SYS_REG(0, 14, 14, 5, x)
#define BM_WRITE_PMEVTYPER22(x)   BM_WRITE_SYS_REG(0, 14, 14, 6, x)
#define BM_WRITE_PMEVTYPER23(x)   BM_WRITE_SYS_REG(0, 14, 14, 7, x)

#define BM_WRITE_PMEVTYPER24(x)   BM_WRITE_SYS_REG(0, 14, 15, 0, x)
#define BM_WRITE_PMEVTYPER25(x)   BM_WRITE_SYS_REG(0, 14, 15, 1, x)
#define BM_WRITE_PMEVTYPER26(x)   BM_WRITE_SYS_REG(0, 14, 15, 2, x)
#define BM_WRITE_PMEVTYPER27(x)   BM_WRITE_SYS_REG(0, 14, 15, 3, x)
#define BM_WRITE_PMEVTYPER28(x)   BM_WRITE_SYS_REG(0, 14, 15, 4, x)
#define BM_WRITE_PMEVTYPER29(x)   BM_WRITE_SYS_REG(0, 14, 15, 5, x)
#define BM_WRITE_PMEVTYPER30(x)   BM_WRITE_SYS_REG(0, 14, 15, 6, x)
#define BM_WRITE_PMCCFILTR(x)     BM_WRITE_SYS_REG(0, 14, 15, 7, x)

#define BM_READ_PMCCNTR_64(x)     BM_READ_SYS_REG_64(0, 9, x)
#define BM_WRITE_PMCCNTR_64(x)    BM_WRITE_SYS_REG_64(0, 9, x)

#elif BM_ARM_64BIT_STATE

#define BM_READ_PMEVCNTR0(x)     BM_READ_SYS_REG_NAMED(PMEVCNTR0_EL0,   x)
#define BM_READ_PMEVCNTR1(x)     BM_READ_SYS_REG_NAMED(PMEVCNTR1_EL0,   x)
#define BM_READ_PMEVCNTR2(x)     BM_READ_SYS_REG_NAMED(PMEVCNTR2_EL0,   x)
#define BM_READ_PMEVCNTR3(x)     BM_READ_SYS_REG_NAMED(PMEVCNTR3_EL0,   x)
#define BM_READ_PMEVCNTR4(x)     BM_READ_SYS_REG_NAMED(PMEVCNTR4_EL0,   x)
#define BM_READ_PMEVCNTR5(x)     BM_READ_SYS_REG_NAMED(PMEVCNTR5_EL0,   x)
#define BM_READ_PMEVCNTR6(x)     BM_READ_SYS_REG_NAMED(PMEVCNTR6_EL0,   x)
#define BM_READ_PMEVCNTR7(x)     BM_READ_SYS_REG_NAMED(PMEVCNTR7_EL0,   x)

#define BM_READ_PMEVCNTR8(x)     BM_READ_SYS_REG_NAMED(PMEVCNTR8_EL0,   x)
#define BM_READ_PMEVCNTR9(x)     BM_READ_SYS_REG_NAMED(PMEVCNTR9_EL0,   x)
#define BM_READ_PMEVCNTR10(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR10_EL0,  x)
#define BM_READ_PMEVCNTR11(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR11_EL0,  x)
#define BM_READ_PMEVCNTR12(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR12_EL0,  x)
#define BM_READ_PMEVCNTR13(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR13_EL0,  x)
#define BM_READ_PMEVCNTR14(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR14_EL0,  x)
#define BM_READ_PMEVCNTR15(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR15_EL0,  x)

#define BM_READ_PMEVCNTR16(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR16_EL0,  x)
#define BM_READ_PMEVCNTR17(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR17_EL0,  x)
#define BM_READ_PMEVCNTR18(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR18_EL0,  x)
#define BM_READ_PMEVCNTR19(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR19_EL0,  x)
#define BM_READ_PMEVCNTR20(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR20_EL0,  x)
#define BM_READ_PMEVCNTR21(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR21_EL0,  x)
#define BM_READ_PMEVCNTR22(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR22_EL0,  x)
#define BM_READ_PMEVCNTR23(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR23_EL0,  x)

#define BM_READ_PMEVCNTR24(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR24_EL0,  x)
#define BM_READ_PMEVCNTR25(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR25_EL0,  x)
#define BM_READ_PMEVCNTR26(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR26_EL0,  x)
#define BM_READ_PMEVCNTR27(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR27_EL0,  x)
#define BM_READ_PMEVCNTR28(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR28_EL0,  x)
#define BM_READ_PMEVCNTR29(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR29_EL0,  x)
#define BM_READ_PMEVCNTR30(x)    BM_READ_SYS_REG_NAMED(PMEVCNTR30_EL0,  x)

#define BM_READ_PMEVTYPER0(x)    BM_READ_SYS_REG_NAMED(PMEVTYPER0_EL0,  x)
#define BM_READ_PMEVTYPER1(x)    BM_READ_SYS_REG_NAMED(PMEVTYPER1_EL0,  x)
#define BM_READ_PMEVTYPER2(x)    BM_READ_SYS_REG_NAMED(PMEVTYPER2_EL0,  x)
#define BM_READ_PMEVTYPER3(x)    BM_READ_SYS_REG_NAMED(PMEVTYPER3_EL0,  x)
#define BM_READ_PMEVTYPER4(x)    BM_READ_SYS_REG_NAMED(PMEVTYPER4_EL0,  x)
#define BM_READ_PMEVTYPER5(x)    BM_READ_SYS_REG_NAMED(PMEVTYPER5_EL0,  x)
#define BM_READ_PMEVTYPER6(x)    BM_READ_SYS_REG_NAMED(PMEVTYPER6_EL0,  x)
#define BM_READ_PMEVTYPER7(x)    BM_READ_SYS_REG_NAMED(PMEVTYPER7_EL0,  x)

#define BM_READ_PMEVTYPER8(x)    BM_READ_SYS_REG_NAMED(PMEVTYPER8_EL0,  x)
#define BM_READ_PMEVTYPER9(x)    BM_READ_SYS_REG_NAMED(PMEVTYPER9_EL0,  x)
#define BM_READ_PMEVTYPER10(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER10_EL0, x)
#define BM_READ_PMEVTYPER11(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER11_EL0, x)
#define BM_READ_PMEVTYPER12(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER12_EL0, x)
#define BM_READ_PMEVTYPER13(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER13_EL0, x)
#define BM_READ_PMEVTYPER14(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER14_EL0, x)
#define BM_READ_PMEVTYPER15(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER15_EL0, x)

#define BM_READ_PMEVTYPER16(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER16_EL0, x)
#define BM_READ_PMEVTYPER17(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER17_EL0, x)
#define BM_READ_PMEVTYPER18(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER18_EL0, x)
#define BM_READ_PMEVTYPER19(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER19_EL0, x)
#define BM_READ_PMEVTYPER20(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER20_EL0, x)
#define BM_READ_PMEVTYPER21(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER21_EL0, x)
#define BM_READ_PMEVTYPER22(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER22_EL0, x)
#define BM_READ_PMEVTYPER23(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER23_EL0, x)

#define BM_READ_PMEVTYPER24(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER24_EL0, x)
#define BM_READ_PMEVTYPER25(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER25_EL0, x)
#define BM_READ_PMEVTYPER26(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER26_EL0, x)
#define BM_READ_PMEVTYPER27(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER27_EL0, x)
#define BM_READ_PMEVTYPER28(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER28_EL0, x)
#define BM_READ_PMEVTYPER29(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER29_EL0, x)
#define BM_READ_PMEVTYPER30(x)   BM_READ_SYS_REG_NAMED(PMEVTYPER30_EL0, x)
#define BM_READ_PMCCFILTR(x)     BM_READ_SYS_REG_NAMED(PMCCFILTR_EL0,   x)

#define BM_WRITE_PMEVCNTR0(x)    BM_WRITE_SYS_REG_NAMED(PMEVCNTR0_EL0,   x)
#define BM_WRITE_PMEVCNTR1(x)    BM_WRITE_SYS_REG_NAMED(PMEVCNTR1_EL0,   x)
#define BM_WRITE_PMEVCNTR2(x)    BM_WRITE_SYS_REG_NAMED(PMEVCNTR2_EL0,   x)
#define BM_WRITE_PMEVCNTR3(x)    BM_WRITE_SYS_REG_NAMED(PMEVCNTR3_EL0,   x)
#define BM_WRITE_PMEVCNTR4(x)    BM_WRITE_SYS_REG_NAMED(PMEVCNTR4_EL0,   x)
#define BM_WRITE_PMEVCNTR5(x)    BM_WRITE_SYS_REG_NAMED(PMEVCNTR5_EL0,   x)
#define BM_WRITE_PMEVCNTR6(x)    BM_WRITE_SYS_REG_NAMED(PMEVCNTR6_EL0,   x)
#define BM_WRITE_PMEVCNTR7(x)    BM_WRITE_SYS_REG_NAMED(PMEVCNTR7_EL0,   x)

#define BM_WRITE_PMEVCNTR8(x)    BM_WRITE_SYS_REG_NAMED(PMEVCNTR8_EL0,   x)
#define BM_WRITE_PMEVCNTR9(x)    BM_WRITE_SYS_REG_NAMED(PMEVCNTR9_EL0,   x)
#define BM_WRITE_PMEVCNTR10(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR10_EL0,  x)
#define BM_WRITE_PMEVCNTR11(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR11_EL0,  x)
#define BM_WRITE_PMEVCNTR12(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR12_EL0,  x)
#define BM_WRITE_PMEVCNTR13(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR13_EL0,  x)
#define BM_WRITE_PMEVCNTR14(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR14_EL0,  x)
#define BM_WRITE_PMEVCNTR15(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR15_EL0,  x)

#define BM_WRITE_PMEVCNTR16(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR16_EL0,  x)
#define BM_WRITE_PMEVCNTR17(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR17_EL0,  x)
#define BM_WRITE_PMEVCNTR18(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR18_EL0,  x)
#define BM_WRITE_PMEVCNTR19(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR19_EL0,  x)
#define BM_WRITE_PMEVCNTR20(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR20_EL0,  x)
#define BM_WRITE_PMEVCNTR21(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR21_EL0,  x)
#define BM_WRITE_PMEVCNTR22(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR22_EL0,  x)
#define BM_WRITE_PMEVCNTR23(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR23_EL0,  x)

#define BM_WRITE_PMEVCNTR24(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR24_EL0,  x)
#define BM_WRITE_PMEVCNTR25(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR25_EL0,  x)
#define BM_WRITE_PMEVCNTR26(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR26_EL0,  x)
#define BM_WRITE_PMEVCNTR27(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR27_EL0,  x)
#define BM_WRITE_PMEVCNTR28(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR28_EL0,  x)
#define BM_WRITE_PMEVCNTR29(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR29_EL0,  x)
#define BM_WRITE_PMEVCNTR30(x)   BM_WRITE_SYS_REG_NAMED(PMEVCNTR30_EL0,  x)

#define BM_WRITE_PMEVTYPER0(x)   BM_WRITE_SYS_REG_NAMED(PMEVTYPER0_EL0,  x)
#define BM_WRITE_PMEVTYPER1(x)   BM_WRITE_SYS_REG_NAMED(PMEVTYPER1_EL0,  x)
#define BM_WRITE_PMEVTYPER2(x)   BM_WRITE_SYS_REG_NAMED(PMEVTYPER2_EL0,  x)
#define BM_WRITE_PMEVTYPER3(x)   BM_WRITE_SYS_REG_NAMED(PMEVTYPER3_EL0,  x)
#define BM_WRITE_PMEVTYPER4(x)   BM_WRITE_SYS_REG_NAMED(PMEVTYPER4_EL0,  x)
#define BM_WRITE_PMEVTYPER5(x)   BM_WRITE_SYS_REG_NAMED(PMEVTYPER5_EL0,  x)
#define BM_WRITE_PMEVTYPER6(x)   BM_WRITE_SYS_REG_NAMED(PMEVTYPER6_EL0,  x)
#define BM_WRITE_PMEVTYPER7(x)   BM_WRITE_SYS_REG_NAMED(PMEVTYPER7_EL0,  x)

#define BM_WRITE_PMEVTYPER8(x)   BM_WRITE_SYS_REG_NAMED(PMEVTYPER8_EL0,  x)
#define BM_WRITE_PMEVTYPER9(x)   BM_WRITE_SYS_REG_NAMED(PMEVTYPER9_EL0,  x)
#define BM_WRITE_PMEVTYPER10(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER10_EL0, x)
#define BM_WRITE_PMEVTYPER11(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER11_EL0, x)
#define BM_WRITE_PMEVTYPER12(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER12_EL0, x)
#define BM_WRITE_PMEVTYPER13(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER13_EL0, x)
#define BM_WRITE_PMEVTYPER14(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER14_EL0, x)
#define BM_WRITE_PMEVTYPER15(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER15_EL0, x)

#define BM_WRITE_PMEVTYPER16(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER16_EL0, x)
#define BM_WRITE_PMEVTYPER17(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER17_EL0, x)
#define BM_WRITE_PMEVTYPER18(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER18_EL0, x)
#define BM_WRITE_PMEVTYPER19(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER19_EL0, x)
#define BM_WRITE_PMEVTYPER20(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER20_EL0, x)
#define BM_WRITE_PMEVTYPER21(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER21_EL0, x)
#define BM_WRITE_PMEVTYPER22(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER22_EL0, x)
#define BM_WRITE_PMEVTYPER23(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER23_EL0, x)

#define BM_WRITE_PMEVTYPER24(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER24_EL0, x)
#define BM_WRITE_PMEVTYPER25(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER25_EL0, x)
#define BM_WRITE_PMEVTYPER26(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER26_EL0, x)
#define BM_WRITE_PMEVTYPER27(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER27_EL0, x)
#define BM_WRITE_PMEVTYPER28(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER28_EL0, x)
#define BM_WRITE_PMEVTYPER29(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER29_EL0, x)
#define BM_WRITE_PMEVTYPER30(x)  BM_WRITE_SYS_REG_NAMED(PMEVTYPER30_EL0, x)
#define BM_WRITE_PMCCFILTR(x)    BM_WRITE_SYS_REG_NAMED(PMCCFILTR_EL0,   x)

#define BM_READ_PMCCNTR_64(x)    BM_READ_PMCCNTR(x)
#define BM_WRITE_PMCCNTR_64(x)   BM_WRITE_PMCCNTR(x)

#endif

#define BM_READ_PMEVCNTR_LITERAL(n,x)     BM_READ_PMEVCNTR ##   n(x)
#define BM_READ_PMEVTYPER_LITERAL(n,x)    BM_READ_PMEVTYPER ##  n(x)
#define BM_WRITE_PMEVCNTR_LITERAL(n,x)    BM_WRITE_PMEVCNTR ##  n(x)
#define BM_WRITE_PMEVTYPER_LITERAL(n,x)   BM_WRITE_PMEVTYPER ## n(x)

#define BM_CALL_WITH_COUNTER_LITERAL_1(function, default_function, counter, arg1) \
    switch (counter)                        \
    {                                       \
        case  0: function( 0, arg1); break; \
        case  1: function( 1, arg1); break; \
        case  2: function( 2, arg1); break; \
        case  3: function( 3, arg1); break; \
        case  4: function( 4, arg1); break; \
        case  5: function( 5, arg1); break; \
        case  6: function( 6, arg1); break; \
        case  7: function( 7, arg1); break; \
        case  8: function( 8, arg1); break; \
        case  9: function( 9, arg1); break; \
        case 10: function(10, arg1); break; \
        case 11: function(11, arg1); break; \
        case 12: function(12, arg1); break; \
        case 13: function(13, arg1); break; \
        case 14: function(14, arg1); break; \
        case 15: function(15, arg1); break; \
        case 16: function(16, arg1); break; \
        case 17: function(17, arg1); break; \
        case 18: function(18, arg1); break; \
        case 19: function(19, arg1); break; \
        case 20: function(20, arg1); break; \
        case 21: function(21, arg1); break; \
        case 22: function(22, arg1); break; \
        case 23: function(23, arg1); break; \
        case 24: function(24, arg1); break; \
        case 25: function(25, arg1); break; \
        case 26: function(26, arg1); break; \
        case 27: function(27, arg1); break; \
        case 28: function(28, arg1); break; \
        case 29: function(29, arg1); break; \
        case 30: function(30, arg1); break; \
        default: default_function(arg1);    \
    }

#define BM_DO_NOTHING_1(x)        (void)(x)
#define BM_SET_TO_ZERO_1(x)       (x) = 0

#define BM_READ_PMEVCNTR(n,   x)   BM_CALL_WITH_COUNTER_LITERAL_1(BM_READ_PMEVCNTR_LITERAL,   BM_SET_TO_ZERO_1, n, x)
#define BM_READ_PMEVTYPER(n,  x)   BM_CALL_WITH_COUNTER_LITERAL_1(BM_READ_PMEVTYPER_LITERAL,  BM_SET_TO_ZERO_1, n, x)
#define BM_WRITE_PMEVCNTR(n,  x)   BM_CALL_WITH_COUNTER_LITERAL_1(BM_WRITE_PMEVCNTR_LITERAL,  BM_DO_NOTHING_1,  n, x)
#define BM_WRITE_PMEVTYPER(n, x)   BM_CALL_WITH_COUNTER_LITERAL_1(BM_WRITE_PMEVTYPER_LITERAL, BM_DO_NOTHING_1,  n, x)

#endif /* BM_PMU_AT_LEAST_V3 */

#if BM_PMU_AT_LEAST_V3_1

#if BM_ARM_32BIT_STATE

#define BM_READ_PMCEID2(x)    /* PMCEID0_EL0[63:32] */ BM_READ_SYS_REG(0,  9, 14, 4, x)
#define BM_READ_PMCEID3(x)    /* PMCEID1_EL0[63:32] */ BM_READ_SYS_REG(0,  9, 14, 5, x)

#define BM_READ_PMCEID0_64(x) /* PMCEID0_EL0        */       \
    do {                                                     \
        bm_uint32 _bm_pmceid0;                               \
        bm_uint32 _bm_pmceid2;                               \
        BM_READ_PMCEID0(_bm_pmceid0);                        \
        BM_READ_PMCEID2(_bm_pmceid2);                        \
        x = (((bm_uint64) _bm_pmceid2) << 32) | _bm_pmceid0; \
    } while (0)
#define BM_READ_PMCEID1_64(x) /* PMCEID1_EL0        */       \
    do {                                                     \
        bm_uint32 _bm_pmceid1;                               \
        bm_uint32 _bm_pmceid3;                               \
        BM_READ_PMCEID1(_bm_pmceid1);                        \
        BM_READ_PMCEID3(_bm_pmceid3);                        \
        x = (((bm_uint64) _bm_pmceid3) << 32) | _bm_pmceid1; \
    } while (0)

#elif BM_ARM_64BIT_STATE

#define BM_READ_PMCEID2(x)    /* PMCEID0_EL0[63:32] */ do { BM_READ_PMCEID0(x); x >> 32; } while (0)
#define BM_READ_PMCEID3(x)    /* PMCEID1_EL0[63:32] */ do { BM_READ_PMCEID1(x); x >> 32; } while (0)

#define BM_READ_PMCEID0_64(x) /* PMCEID0_EL0        */ BM_READ_PMCEID0(x)
#define BM_READ_PMCEID1_64(x) /* PMCEID1_EL0        */ BM_READ_PMCEID1(x)

#endif

#endif /* BM_PMU_AT_LEAST_V3_1 */

/** @} */

/* ************************************* */

/* The architecture limit on the number of counters */
/** @{ */
#define ARM_PMU_MAX_COUNTERS      32
#define ARM_PMU_COUNTER_MASK      (ARM_PMU_MAX_COUNTERS - 1)
/** @} */

/* PMCR */
/** @{ */
#define ARM_PMU_PMCR_E            (1 << 0)
#define ARM_PMU_PMCR_P            (1 << 1)
#define ARM_PMU_PMCR_C            (1 << 2)
#define ARM_PMU_PMCR_LC           (1 << 6)
#define ARM_PMU_PMCR_N_SHIFT      11
#define ARM_PMU_PMCR_N_MASK       0x1f
#ifdef BM_READ_PMCCNTR_64
#define ARM_PMU_PMCR_RESET_VALUE  ARM_PMU_PMCR_LC
#else
#define ARM_PMU_PMCR_RESET_VALUE  0
#endif
/** @} */

/* PMXEVTYPER */
/** @{ */
#define ARM_PMU_EVTYPE_MASK       0x0000ffff
#define ARM_PMU_EVTYPE_NSH_BIT    (1u << 27)
/** @} */

/* PMUSERENR */
/** @{ */
#define ARM_PMU_USERENR_EN        (1 << 0)
#define ARM_PMU_USERENR_SW        (1 << 1)
#define ARM_PMU_USERENR_CR        (1 << 2)
#define ARM_PMU_USERENR_ER        (1 << 3)
/** @} */

/** @{ */
#define CYCLE_COUNTER_NO    0
#define CYCLE_COUNTER_HW_NO 31
/** @} */
/* ************************************* */

/**
 * Stores the per-core PMU configuration data
 */
struct barman_arm_pmu_configuration_data
{
    /** Stores the 64-bit accumulated value for each 32-bit counter */
    bm_atomic_uint64 event_counter_values[ARM_PMU_MAX_COUNTERS];
    /** The number of events that were enabled */
    bm_uint32 n_events_enabled;
    /** Indicates PMU is properly configured and can be used */
    bm_bool is_initialized;
};

/** Per core configuration data */
static struct barman_arm_pmu_configuration_data barman_arm_pmu_configuration_data[BM_CONFIG_MAX_CORES];

/* ************************************* */

static BM_INLINE bm_uint32 barman_arm_pmu_get_number_of_counters(void)
{
    bm_uintptr pmcr;
    BM_READ_PMCR(pmcr);
    return ((pmcr >> ARM_PMU_PMCR_N_SHIFT) & ARM_PMU_PMCR_N_MASK);
}

static BM_INLINE bm_uint32 barman_arm_pmu_get_and_validate_counter_hw_no(bm_uint32 counter_no)
{
    const bm_uint32 n_counters = barman_arm_pmu_get_number_of_counters();

    if (counter_no == CYCLE_COUNTER_NO) {
        return CYCLE_COUNTER_HW_NO;
    }

    counter_no--;
    if (counter_no < n_counters) {
        return counter_no;
    }

    return -1u;
}

static BM_INLINE void barman_arm_pmu_configure_cycle_counter(bm_bool enable_interrupts)
{
#ifdef BM_WRITE_PMCCFILTR
    BM_WRITE_PMCCFILTR((bm_uintptr) ARM_PMU_EVTYPE_NSH_BIT);
#else
    /*
     * If we don't have the reentrant PMCCFILTR write available,
     * then we need to disable interrupts while we write it.
     */
    const bm_uintptr interrupt_status = barman_ext_disable_interrupts_local();

    /* Write the filter bits non-reentrantly */
    BM_WRITE_PMCCFILTR_NR((bm_uintptr) ARM_PMU_EVTYPE_NSH_BIT);

    barman_ext_enable_interrupts_local(interrupt_status);
#endif

    if (enable_interrupts) {
        BM_WRITE_PMINTENSET(BM_BIT(CYCLE_COUNTER_HW_NO));
    }

    BM_WRITE_PMCNTENSET(BM_BIT(CYCLE_COUNTER_HW_NO));
}

static BM_INLINE void barman_arm_pmu_configure_counter(bm_bool enable_interrupts, bm_uint32 counter_hw_no, bm_uint32 event_id)
{
#ifdef BM_WRITE_PMEVTYPER
    BM_WRITE_PMEVTYPER(counter_hw_no, (bm_uintptr) (event_id & ARM_PMU_EVTYPE_MASK) | ARM_PMU_EVTYPE_NSH_BIT);
#else
    /*
     * If we don't have the reentrant event type write available,
     * then we need to disable interrupts while we write it.
     */
    const bm_uintptr interrupt_status = barman_ext_disable_interrupts_local();

    /* Write the event type non-reentrantly */
    BM_WRITE_PMEVTYPER_NR(counter_hw_no, (bm_uintptr) (event_id & ARM_PMU_EVTYPE_MASK) | ARM_PMU_EVTYPE_NSH_BIT);

    barman_ext_enable_interrupts_local(interrupt_status);
#endif

    if (enable_interrupts) {
        BM_WRITE_PMINTENSET(BM_BIT(counter_hw_no));
    }

    BM_WRITE_PMCNTENSET(BM_BIT(counter_hw_no));
}

static BM_INLINE void barman_arm_pmu_reset(void)
{
    BM_WRITE_PMINTENCLR(0xfffffffful);
    BM_WRITE_PMCNTENCLR(0xfffffffful);
    barman_isb();
    BM_WRITE_PMOVSR(0xfffffffful);
    BM_WRITE_PMCR((bm_uintptr) (ARM_PMU_PMCR_RESET_VALUE | ARM_PMU_PMCR_P | ARM_PMU_PMCR_C));
    barman_isb();
}

static BM_INLINE bm_uint64 barman_arm_pmu_get_current_counter_value(bm_uint32 current_core_no, bm_uint32 counter_no)
{
    return barman_atomic_load(&(barman_arm_pmu_configuration_data[current_core_no].event_counter_values[counter_no]));
}

static BM_INLINE bm_bool barman_arm_pmu_set_current_counter_value(bm_uint32 current_core_no, bm_uint32 counter_no, bm_uint64 * old_value,
                                                                     bm_uint64 new_value)
{
    /* NB: old_value is modified by barman_atomic_cmp_ex_strong_pointer if it fails */
    return barman_atomic_cmp_ex_strong_pointer(&(barman_arm_pmu_configuration_data[current_core_no].event_counter_values[counter_no]), old_value, new_value);
}

/* ************************************* */

bm_int32 barman_arm_pmu_init(bm_bool enable_el0_access, bm_bool enable_interrupts, bm_bool enable_cycle_counter,
                              bm_uint32 n_event_types, const bm_uint32 * event_types)
{
    const bm_uint32 current_core_no = barman_get_core_no();
    const bm_uint32 n_counters = barman_arm_pmu_get_number_of_counters();
    const bm_uint32 n_events_to_configure = BM_MIN(n_event_types, n_counters);
    bm_uint32 counter_no;

    /* Make sure we do nothing if the core no is invalid */
    if (current_core_no >= BM_CONFIG_MAX_CORES) {
        return 0;
    }

    /* Set un-initialized */
    barman_arm_pmu_configuration_data[current_core_no].is_initialized = BM_FALSE;

    /* Reset the PMU */
    barman_arm_pmu_reset();

    /* Enable the cycle counter */
    if (enable_cycle_counter) {
        barman_arm_pmu_configure_cycle_counter(enable_interrupts);
    }

    /* Enable the other counters */
    for (counter_no = 0; counter_no < ARM_PMU_MAX_COUNTERS; ++counter_no) {
        /* Reset 64-bit counter */
        barman_arm_pmu_configuration_data[current_core_no].event_counter_values[counter_no] = 0;

        /* Enable event */
        if (counter_no < n_events_to_configure) {
            barman_arm_pmu_configure_counter(enable_interrupts, counter_no, event_types[counter_no]);
        }
    }

    /* Configure EL0 access */
    BM_WRITE_PMUSERENR((bm_uintptr) (enable_el0_access ? ARM_PMU_USERENR_EN | ARM_PMU_USERENR_SW | ARM_PMU_USERENR_CR | ARM_PMU_USERENR_ER : 0));

    barman_isb();

    /* Save enabled event count */
    barman_arm_pmu_configuration_data[current_core_no].n_events_enabled = n_events_to_configure;
    /* Set initialized */
    barman_arm_pmu_configuration_data[current_core_no].is_initialized = BM_TRUE;

    /* Make sure data is written to configuration */
    barman_dsb();

    return n_events_to_configure + (enable_cycle_counter ? 1 : 0);
}

void barman_arm_pmu_start(void)
{
    /* Make sure we do nothing if the core no is invalid */
    const bm_uint32 current_core_no = barman_get_core_no();
    if (current_core_no >= BM_CONFIG_MAX_CORES) {
        return;
    }

    /* Make sure the PMU has been initialized */
    if (!barman_arm_pmu_configuration_data[current_core_no].is_initialized) {
        return;
    }

    /* Start counters */
    BM_WRITE_PMCR((bm_uintptr) (ARM_PMU_PMCR_RESET_VALUE | ARM_PMU_PMCR_E));
}

void barman_arm_pmu_stop(void)
{
    /* Make sure we do nothing if the core no is invalid */
    const bm_uint32 current_core_no = barman_get_core_no();
    if (current_core_no >= BM_CONFIG_MAX_CORES) {
        return;
    }

    /* Make sure the PMU has been initialized */
    if (!barman_arm_pmu_configuration_data[current_core_no].is_initialized) {
        return;
    }

    /* Stop counters */
    BM_WRITE_PMCR((bm_uintptr) (ARM_PMU_PMCR_RESET_VALUE & ~ARM_PMU_PMCR_E));
}

bm_uint64 barman_arm_pmu_read_counter(bm_uint32 counter_no)
{
    const bm_uint32 current_core_no = barman_get_core_no();
    const bm_uint32 counter_hw_no = barman_arm_pmu_get_and_validate_counter_hw_no(counter_no);
    bm_uint64 value64;

    /* Make sure we do nothing if the core no is invalid */
    if (current_core_no >= BM_CONFIG_MAX_CORES) {
        return BM_ARM_PMU_INVALID_COUNTER_VALUE;
    }

    /* Make sure the PMU has been initialized */
    if (!barman_arm_pmu_configuration_data[current_core_no].is_initialized) {
        return BM_ARM_PMU_INVALID_COUNTER_VALUE;
    }

#ifdef BM_READ_PMCCNTR_64
    /* read the cycle counter as 64 bits */
    if (counter_hw_no == CYCLE_COUNTER_HW_NO) {
        BM_READ_PMCCNTR_64(value64);
    }
    else
#endif
    if (counter_hw_no <= CYCLE_COUNTER_HW_NO) {
        bm_uint64 current_counter_value;
        bm_uintptr read_counter_value_32;
        bm_uintptr overflowed_reg_before, overflowed_reg_after;
        bm_uintptr read_counter;
        bm_bool updated_value = BM_FALSE;

        /* Do an atomic RMW on the stored 64-bit counter value to avoid locking */
        current_counter_value = barman_arm_pmu_get_current_counter_value(current_core_no, counter_no);

        do {
            /*
             * We need to read the counter value and the overflow flag as an atomic pair, but we cannot guarantee that the code
             * wont be preempted between the read of the counter and the overflow because we allow the counters to be read at PL0.
             * Instead we assume that the time between overflows should be so long that even if it was preempted such that the
             * overflow occurred between the read of the counter and the overflow flag, if we tried again a second time the overflow
             * could not happen twice. (we can't detect that anyway because of the nature of the overflow flag)
             *
             * We read the overflow before and after the counter is read. If the flag is different then we try again a second time
             * under the assumption that it is impossible to happen a second time. This ensures the overflow flag is valid for
             * the counter value (at the expense of a possible chance of small delay in read)
             */
            for (read_counter = 0; (read_counter < 2); ++read_counter) {
                /* read the overflow bit before */
                BM_READ_PMOVSR(overflowed_reg_before);

#ifndef BM_READ_PMCCNTR_64
                /* read the cycle counter as 32 bits */
                if (counter_hw_no == CYCLE_COUNTER_HW_NO) {
                    BM_READ_PMCCNTR(read_counter_value_32);
                }
                else
#endif
                /* an event counter instead */
                {
#ifdef BM_READ_PMEVCNTR
                BM_READ_PMEVCNTR(counter_hw_no, read_counter_value_32);
#else
                    /*
                     * If we don't have the reentrant counter read available,
                     * then we need to disable interrupts while we read it.
                     */
                    const bm_uintptr interrupt_status = barman_ext_disable_interrupts_local();

                    /* Read the counter non-reentrantly */
                    BM_READ_PMEVCNTR_NR(counter_hw_no, read_counter_value_32);

                    barman_ext_enable_interrupts_local(interrupt_status);
#endif
                }

                /* read the overflow bit after */
                BM_READ_PMOVSR(overflowed_reg_after);

                /* compare overflow before and after */
                if ((overflowed_reg_before & BM_BIT(counter_hw_no)) == (overflowed_reg_after & BM_BIT(counter_hw_no))) {
                    break;
                }
            }

            /* Check for an overflow in the lower 32-bits; event counters are 32 bits only but externally we expect counters to
             * be 64bits */
            if ((overflowed_reg_after & BM_BIT(counter_hw_no)) != 0) {
                /* Clear the overflow */
                BM_WRITE_PMOVSR(BM_BIT(counter_hw_no));
                barman_isb();

                /* Increment the top 32-bits */
                value64 = ((current_counter_value & 0xffffffff00000000ull) + 0x100000000ul) | read_counter_value_32;
            }
            else {
                /* Just insert the new bottom 32 bits */
                value64 = (current_counter_value & 0xffffffff00000000ull) | read_counter_value_32;
            }

            /* if value64 < current_counter_value then it means this thread was preempted between the read of
             * current_counter_value and the read of the counter and overflow bit by another thread that read and reset
             * an overflow. If the other thread updates current_counter_value then we fail and retry anyway, otherwise
             * if it was preempted by this thread again, we will write a wrong value. */
            if (value64 < current_counter_value) {
                value64 += 0x100000000;
            }

            /* Do the atomic update.
             * If value64 fails to write and the current_counter_value is less that value64, then retry writing
             * until we succeed, or until current_counter_value becomes >= value64 which would mean another thread wrote
             * a larger value in which case we retry */
            do {
                updated_value = barman_arm_pmu_set_current_counter_value(current_core_no, counter_no, &current_counter_value, value64);
            } while ((current_counter_value < value64) && (!updated_value));
        } while (!updated_value);
    }
    /* Not a valid counter */
    else {
        value64 = BM_ARM_PMU_INVALID_COUNTER_VALUE;
    }

    return value64;
}
#endif

#endif /* (!defined(BARMAN_DISABLED)) || (!BARMAN_DISABLED) */
