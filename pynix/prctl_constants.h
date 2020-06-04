#pragma once
// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>


#ifndef PR_SET_PDEATHSIG
#define PR_SET_PDEATHSIG 1
#endif

#ifndef PR_GET_PDEATHSIG
#define PR_GET_PDEATHSIG 2
#endif

#ifndef PR_GET_DUMPABLE
#define PR_GET_DUMPABLE 3
#endif

#ifndef PR_SET_DUMPABLE
#define PR_SET_DUMPABLE 4
#endif

#ifndef PR_GET_UNALIGN
#define PR_GET_UNALIGN 5
#endif

#ifndef PR_SET_UNALIGN
#define PR_SET_UNALIGN 6
#endif

#ifndef PR_UNALIGN_NOPRINT
#define PR_UNALIGN_NOPRINT 1
#endif

#ifndef PR_UNALIGN_SIGBUS
#define PR_UNALIGN_SIGBUS 2
#endif

#ifndef PR_GET_KEEPCAPS
#define PR_GET_KEEPCAPS 7
#endif

#ifndef PR_SET_KEEPCAPS
#define PR_SET_KEEPCAPS 8
#endif

#ifndef PR_GET_FPEMU
#define PR_GET_FPEMU 9
#endif

#ifndef PR_SET_FPEMU
#define PR_SET_FPEMU 10
#endif

#ifndef PR_FPEMU_NOPRINT
#define PR_FPEMU_NOPRINT 1
#endif

#ifndef PR_FPEMU_SIGFPE
#define PR_FPEMU_SIGFPE 2
#endif

#ifndef PR_GET_FPEXC
#define PR_GET_FPEXC 11
#endif

#ifndef PR_SET_FPEXC
#define PR_SET_FPEXC 12
#endif

#ifndef PR_FP_EXC_SW_ENABLE
#define PR_FP_EXC_SW_ENABLE 0x80
#endif

#ifndef PR_FP_EXC_DIV
#define PR_FP_EXC_DIV 0x010000
#endif

#ifndef PR_FP_EXC_OVF
#define PR_FP_EXC_OVF 0x020000
#endif

#ifndef PR_FP_EXC_UND
#define PR_FP_EXC_UND 0x040000
#endif

#ifndef PR_FP_EXC_RES
#define PR_FP_EXC_RES 0x080000
#endif

#ifndef PR_FP_EXC_INV
#define PR_FP_EXC_INV 0x100000
#endif

#ifndef PR_FP_EXC_DISABLED
#define PR_FP_EXC_DISABLED 0
#endif

#ifndef PR_FP_EXC_NONRECOV
#define PR_FP_EXC_NONRECOV 1
#endif

#ifndef PR_FP_EXC_ASYNC
#define PR_FP_EXC_ASYNC 2
#endif

#ifndef PR_FP_EXC_PRECISE
#define PR_FP_EXC_PRECISE 3
#endif

#ifndef PR_GET_TIMING
#define PR_GET_TIMING 13
#endif

#ifndef PR_SET_TIMING
#define PR_SET_TIMING 14
#endif

#ifndef PR_TIMING_STATISTICAL
#define PR_TIMING_STATISTICAL 0
#endif

#ifndef PR_TIMING_TIMESTAMP
#define PR_TIMING_TIMESTAMP 1
#endif

#ifndef PR_SET_NAME
#define PR_SET_NAME 15
#endif

#ifndef PR_GET_NAME
#define PR_GET_NAME 16
#endif

#ifndef PR_GET_ENDIAN
#define PR_GET_ENDIAN 19
#endif

#ifndef PR_SET_ENDIAN
#define PR_SET_ENDIAN 20
#endif

#ifndef PR_ENDIAN_BIG
#define PR_ENDIAN_BIG 0
#endif

#ifndef PR_ENDIAN_LITTLE
#define PR_ENDIAN_LITTLE 1
#endif

#ifndef PR_ENDIAN_PPC_LITTLE
#define PR_ENDIAN_PPC_LITTLE 2
#endif

#ifndef PR_GET_SECCOMP
#define PR_GET_SECCOMP 21
#endif

#ifndef PR_SET_SECCOMP
#define PR_SET_SECCOMP 22
#endif

#ifndef PR_CAPBSET_READ
#define PR_CAPBSET_READ 23
#endif

#ifndef PR_CAPBSET_DROP
#define PR_CAPBSET_DROP 24
#endif

#ifndef PR_GET_TSC
#define PR_GET_TSC 25
#endif

#ifndef PR_SET_TSC
#define PR_SET_TSC 26
#endif

#ifndef PR_TSC_ENABLE
#define PR_TSC_ENABLE 1
#endif

#ifndef PR_TSC_SIGSEGV
#define PR_TSC_SIGSEGV 2
#endif

#ifndef PR_GET_SECUREBITS
#define PR_GET_SECUREBITS 27
#endif

#ifndef PR_SET_SECUREBITS
#define PR_SET_SECUREBITS 28
#endif

#ifndef PR_SET_TIMERSLACK
#define PR_SET_TIMERSLACK 29
#endif

#ifndef PR_GET_TIMERSLACK
#define PR_GET_TIMERSLACK 30
#endif

#ifndef PR_TASK_PERF_EVENTS_DISABLE
#define PR_TASK_PERF_EVENTS_DISABLE 31
#endif

#ifndef PR_TASK_PERF_EVENTS_ENABLE
#define PR_TASK_PERF_EVENTS_ENABLE 32
#endif

#ifndef PR_MCE_KILL
#define PR_MCE_KILL 33
#endif

#ifndef PR_MCE_KILL_CLEAR
#define PR_MCE_KILL_CLEAR 0
#endif

#ifndef PR_MCE_KILL_SET
#define PR_MCE_KILL_SET 1
#endif

#ifndef PR_MCE_KILL_LATE
#define PR_MCE_KILL_LATE 0
#endif

#ifndef PR_MCE_KILL_EARLY
#define PR_MCE_KILL_EARLY 1
#endif

#ifndef PR_MCE_KILL_DEFAULT
#define PR_MCE_KILL_DEFAULT 2
#endif

#ifndef PR_MCE_KILL_GET
#define PR_MCE_KILL_GET 34
#endif

#ifndef PR_SET_MM
#define PR_SET_MM 35
#endif

#ifndef PR_SET_MM_START_CODE
#define PR_SET_MM_START_CODE 1
#endif

#ifndef PR_SET_MM_END_CODE
#define PR_SET_MM_END_CODE 2
#endif

#ifndef PR_SET_MM_START_DATA
#define PR_SET_MM_START_DATA 3
#endif

#ifndef PR_SET_MM_END_DATA
#define PR_SET_MM_END_DATA 4
#endif

#ifndef PR_SET_MM_START_STACK
#define PR_SET_MM_START_STACK 5
#endif

#ifndef PR_SET_MM_START_BRK
#define PR_SET_MM_START_BRK 6
#endif

#ifndef PR_SET_MM_BRK
#define PR_SET_MM_BRK 7
#endif

#ifndef PR_SET_MM_ARG_START
#define PR_SET_MM_ARG_START 8
#endif

#ifndef PR_SET_MM_ARG_END
#define PR_SET_MM_ARG_END 9
#endif

#ifndef PR_SET_MM_ENV_START
#define PR_SET_MM_ENV_START 10
#endif

#ifndef PR_SET_MM_ENV_END
#define PR_SET_MM_ENV_END 11
#endif

#ifndef PR_SET_MM_AUXV
#define PR_SET_MM_AUXV 12
#endif

#ifndef PR_SET_MM_EXE_FILE
#define PR_SET_MM_EXE_FILE 13
#endif

#ifndef PR_SET_MM_MAP
#define PR_SET_MM_MAP 14
#endif

#ifndef PR_SET_MM_MAP_SIZE
#define PR_SET_MM_MAP_SIZE 15
#endif

#ifndef PR_SET_PTRACER
#define PR_SET_PTRACER 0x59616d61
#endif

#ifndef PR_SET_PTRACER_ANY
#define PR_SET_PTRACER_ANY ((unsigned long)-1)
#endif

#ifndef PR_SET_CHILD_SUBREAPER
#define PR_SET_CHILD_SUBREAPER 36
#endif

#ifndef PR_GET_CHILD_SUBREAPER
#define PR_GET_CHILD_SUBREAPER 37
#endif

#ifndef PR_SET_NO_NEW_PRIVS
#define PR_SET_NO_NEW_PRIVS 38
#endif

#ifndef PR_GET_NO_NEW_PRIVS
#define PR_GET_NO_NEW_PRIVS 39
#endif

#ifndef PR_GET_TID_ADDRESS
#define PR_GET_TID_ADDRESS 40
#endif

#ifndef PR_SET_THP_DISABLE
#define PR_SET_THP_DISABLE 41
#endif

#ifndef PR_GET_THP_DISABLE
#define PR_GET_THP_DISABLE 42
#endif

#ifndef PR_MPX_ENABLE_MANAGEMENT
#define PR_MPX_ENABLE_MANAGEMENT 43
#endif

#ifndef PR_MPX_DISABLE_MANAGEMENT
#define PR_MPX_DISABLE_MANAGEMENT 44
#endif

#ifndef PR_SET_FP_MODE
#define PR_SET_FP_MODE 45
#endif

#ifndef PR_GET_FP_MODE
#define PR_GET_FP_MODE 46
#endif

#ifndef PR_FP_MODE_FR
#define PR_FP_MODE_FR (1 << 0)
#endif

#ifndef PR_FP_MODE_FRE
#define PR_FP_MODE_FRE (1 << 1)
#endif

#ifndef PR_CAP_AMBIENT
#define PR_CAP_AMBIENT 47
#endif

#ifndef PR_CAP_AMBIENT_IS_SET
#define PR_CAP_AMBIENT_IS_SET 1
#endif

#ifndef PR_CAP_AMBIENT_RAISE
#define PR_CAP_AMBIENT_RAISE 2
#endif

#ifndef PR_CAP_AMBIENT_LOWER
#define PR_CAP_AMBIENT_LOWER 3
#endif

#ifndef PR_CAP_AMBIENT_CLEAR_ALL
#define PR_CAP_AMBIENT_CLEAR_ALL 4
#endif

#ifndef PR_SVE_SET_VL
#define PR_SVE_SET_VL 50
#endif

#ifndef PR_SVE_SET_VL_ONEXEC
#define PR_SVE_SET_VL_ONEXEC (1 << 18)
#endif

#ifndef PR_SVE_GET_VL
#define PR_SVE_GET_VL 51
#endif

#ifndef PR_SVE_VL_LEN_MASK
#define PR_SVE_VL_LEN_MASK 0xffff
#endif

#ifndef PR_SVE_VL_INHERIT
#define PR_SVE_VL_INHERIT (1 << 17)
#endif

#ifndef PR_GET_SPECULATION_CTRL
#define PR_GET_SPECULATION_CTRL 52
#endif

#ifndef PR_SET_SPECULATION_CTRL
#define PR_SET_SPECULATION_CTRL 53
#endif

#ifndef PR_SPEC_STORE_BYPASS
#define PR_SPEC_STORE_BYPASS 0
#endif

#ifndef PR_SPEC_INDIRECT_BRANCH
#define PR_SPEC_INDIRECT_BRANCH 1
#endif

#ifndef PR_SPEC_NOT_AFFECTED
#define PR_SPEC_NOT_AFFECTED 0
#endif

#ifndef PR_SPEC_PRCTL
#define PR_SPEC_PRCTL (1UL << 0)
#endif

#ifndef PR_SPEC_ENABLE
#define PR_SPEC_ENABLE (1UL << 1)
#endif

#ifndef PR_SPEC_DISABLE
#define PR_SPEC_DISABLE (1UL << 2)
#endif

#ifndef PR_SPEC_FORCE_DISABLE
#define PR_SPEC_FORCE_DISABLE (1UL << 3)
#endif

#ifndef PR_GET_DISPLAY_LSM
#define PR_GET_DISPLAY_LSM 1000000
#endif

#ifndef PR_SET_DISPLAY_LSM
#define PR_SET_DISPLAY_LSM 1000001
#endif
