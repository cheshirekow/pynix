"""
Generate prctl preprocessor macros
"""

import argparse
import io
import os

import jinja2


def main():
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument("-o", "--out-dir", default=".")
  parser.add_argument("which", choices=["prctl", "clone"])
  args = parser.parse_args()

  tpldir = os.path.dirname(os.path.realpath(__file__))
  env = jinja2.Environment(loader=jinja2.FileSystemLoader(tpldir))
  tpl = env.get_template("constants.h.in")

  values = globals()[args.which.upper() + "_VALUES"]
  content = tpl.render(values=values)
  outfile_path = os.path.join(args.out_dir, args.which + "_constants.h")
  with io.open(outfile_path, "w", encoding="utf-8") as outfile:
    outfile.write(content)


PRCTL_VALUES = [
    ("PR_SET_PDEATHSIG", "1"),
    ("PR_GET_PDEATHSIG", "2"),
    ("PR_GET_DUMPABLE", "3"),
    ("PR_SET_DUMPABLE", "4"),
    ("PR_GET_UNALIGN", "5"),
    ("PR_SET_UNALIGN", "6"),
    ("PR_UNALIGN_NOPRINT", "1"),
    ("PR_UNALIGN_SIGBUS", "2"),
    ("PR_GET_KEEPCAPS", "7"),
    ("PR_SET_KEEPCAPS", "8"),
    ("PR_GET_FPEMU", "9"),
    ("PR_SET_FPEMU", "10"),
    ("PR_FPEMU_NOPRINT", "1"),
    ("PR_FPEMU_SIGFPE", "2"),
    ("PR_GET_FPEXC", "11"),
    ("PR_SET_FPEXC", "12"),
    ("PR_FP_EXC_SW_ENABLE", "0x80"),
    ("PR_FP_EXC_DIV", "0x010000"),
    ("PR_FP_EXC_OVF", "0x020000"),
    ("PR_FP_EXC_UND", "0x040000"),
    ("PR_FP_EXC_RES", "0x080000"),
    ("PR_FP_EXC_INV", "0x100000"),
    ("PR_FP_EXC_DISABLED", "0"),
    ("PR_FP_EXC_NONRECOV", "1"),
    ("PR_FP_EXC_ASYNC", "2"),
    ("PR_FP_EXC_PRECISE", "3"),
    ("PR_GET_TIMING", "13"),
    ("PR_SET_TIMING", "14"),
    ("PR_TIMING_STATISTICAL", "0"),
    ("PR_TIMING_TIMESTAMP", "1"),
    ("PR_SET_NAME", "15"),
    ("PR_GET_NAME", "16"),
    ("PR_GET_ENDIAN", "19"),
    ("PR_SET_ENDIAN", "20"),
    ("PR_ENDIAN_BIG", "0"),
    ("PR_ENDIAN_LITTLE", "1"),
    ("PR_ENDIAN_PPC_LITTLE", "2"),
    ("PR_GET_SECCOMP", "21"),
    ("PR_SET_SECCOMP", "22"),
    ("PR_CAPBSET_READ", "23"),
    ("PR_CAPBSET_DROP", "24"),
    ("PR_GET_TSC", "25"),
    ("PR_SET_TSC", "26"),
    ("PR_TSC_ENABLE", "1"),
    ("PR_TSC_SIGSEGV", "2"),
    ("PR_GET_SECUREBITS", "27"),
    ("PR_SET_SECUREBITS", "28"),
    ("PR_SET_TIMERSLACK", "29"),
    ("PR_GET_TIMERSLACK", "30"),
    ("PR_TASK_PERF_EVENTS_DISABLE", "31"),
    ("PR_TASK_PERF_EVENTS_ENABLE", "32"),
    ("PR_MCE_KILL", "33"),
    ("PR_MCE_KILL_CLEAR", "0"),
    ("PR_MCE_KILL_SET", "1"),
    ("PR_MCE_KILL_LATE", "0"),
    ("PR_MCE_KILL_EARLY", "1"),
    ("PR_MCE_KILL_DEFAULT", "2"),
    ("PR_MCE_KILL_GET", "34"),
    ("PR_SET_MM", "35"),
    ("PR_SET_MM_START_CODE", "1"),
    ("PR_SET_MM_END_CODE", "2"),
    ("PR_SET_MM_START_DATA", "3"),
    ("PR_SET_MM_END_DATA", "4"),
    ("PR_SET_MM_START_STACK", "5"),
    ("PR_SET_MM_START_BRK", "6"),
    ("PR_SET_MM_BRK", "7"),
    ("PR_SET_MM_ARG_START", "8"),
    ("PR_SET_MM_ARG_END", "9"),
    ("PR_SET_MM_ENV_START", "10"),
    ("PR_SET_MM_ENV_END", "11"),
    ("PR_SET_MM_AUXV", "12"),
    ("PR_SET_MM_EXE_FILE", "13"),
    ("PR_SET_MM_MAP", "14"),
    ("PR_SET_MM_MAP_SIZE", "15"),
    ("PR_SET_PTRACER", "0x59616d61"),
    ("PR_SET_PTRACER_ANY", "((unsigned long)-1)"),
    ("PR_SET_CHILD_SUBREAPER", "36"),
    ("PR_GET_CHILD_SUBREAPER", "37"),
    ("PR_SET_NO_NEW_PRIVS", "38"),
    ("PR_GET_NO_NEW_PRIVS", "39"),
    ("PR_GET_TID_ADDRESS", "40"),
    ("PR_SET_THP_DISABLE", "41"),
    ("PR_GET_THP_DISABLE", "42"),
    ("PR_MPX_ENABLE_MANAGEMENT", "43"),
    ("PR_MPX_DISABLE_MANAGEMENT", "44"),
    ("PR_SET_FP_MODE", "45"),
    ("PR_GET_FP_MODE", "46"),
    ("PR_FP_MODE_FR", "(1 << 0)"),
    ("PR_FP_MODE_FRE", "(1 << 1)"),
    ("PR_CAP_AMBIENT", "47"),
    ("PR_CAP_AMBIENT_IS_SET", "1"),
    ("PR_CAP_AMBIENT_RAISE", "2"),
    ("PR_CAP_AMBIENT_LOWER", "3"),
    ("PR_CAP_AMBIENT_CLEAR_ALL", "4"),
    ("PR_SVE_SET_VL", "50"),
    ("PR_SVE_SET_VL_ONEXEC", "(1 << 18)"),
    ("PR_SVE_GET_VL", "51"),
    ("PR_SVE_VL_LEN_MASK", "0xffff"),
    ("PR_SVE_VL_INHERIT", "(1 << 17)"),
    ("PR_GET_SPECULATION_CTRL", "52"),
    ("PR_SET_SPECULATION_CTRL", "53"),
    ("PR_SPEC_STORE_BYPASS", "0"),
    ("PR_SPEC_INDIRECT_BRANCH", "1"),
    ("PR_SPEC_NOT_AFFECTED", "0"),
    ("PR_SPEC_PRCTL", "(1UL << 0)"),
    ("PR_SPEC_ENABLE", "(1UL << 1)"),
    ("PR_SPEC_DISABLE", "(1UL << 2)"),
    ("PR_SPEC_FORCE_DISABLE", "(1UL << 3)"),
    ("PR_GET_DISPLAY_LSM", "1000000"),
    ("PR_SET_DISPLAY_LSM", "1000001"),
]

CLONE_VALUES = [
    ("CLONE_VM", "0x00000100"),
    ("CLONE_FS", "0x00000200"),
    ("CLONE_FILES", "0x00000400"),
    ("CLONE_SIGHAND", "0x00000800"),
    ("CLONE_NEWTIME", "0x00000080"),
    ("CLONE_PTRACE", "0x00002000"),
    ("CLONE_VFORK", "0x00004000"),
    ("CLONE_PARENT", "0x00008000"),
    ("CLONE_THREAD", "0x00010000"),
    ("CLONE_NEWNS", "0x00020000"),
    ("CLONE_SYSVSEM", "0x00040000"),
    ("CLONE_SETTLS", "0x00080000"),
    ("CLONE_PARENT_SETTID", "0x00100000"),
    ("CLONE_CHILD_CLEARTID", "0x00200000"),
    ("CLONE_DETACHED", "0x00400000"),
    ("CLONE_UNTRACED", "0x00800000"),
    ("CLONE_CHILD_SETTID", "0x01000000"),
    ("CLONE_NEWCGROUP", "0x02000000"),
    ("CLONE_NEWUTS", "0x04000000"),
    ("CLONE_NEWIPC", "0x08000000"),
    ("CLONE_NEWUSER", "0x10000000"),
    ("CLONE_NEWPID", "0x20000000"),
    ("CLONE_NEWNET", "0x40000000"),
    ("CLONE_IO", "0x80000000"),
]

if __name__ == "__main__":
  main()