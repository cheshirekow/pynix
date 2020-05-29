// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

#include <sys/prctl.h>

#include <structmember.h>

/* =============================================================================
 * pynix.prctl()
 * ========================================================================== */

static int get_option_from_args(PyObject* args) {
  if (!PyTuple_Check(args)) {
    PyErr_SetString(PyExc_SystemError,
                    "pynix_prctl somehow called with non-tuple as args");
    return -1;
  }

  if (PyTuple_Size(args) < 1) {
    PyErr_SetString(PyExc_TypeError,
                    "pynix_prctl requires at least one argument");
    return -1;
  }

  PyObject* arg0 = PyTuple_GetItem(args, 0);
  if (!PyLong_Check(arg0)) {
    PyErr_SetString(PyExc_TypeError,
                    "The first argument to pynix_prctl should be an integer");
    return -1;
  }

  return PyLong_AsLong(arg0);
}

static PyObject* pynix_prctl(PyObject* module, PyObject* args) {
  int option;
  unsigned long arg2 = 0;
  unsigned long arg3 = 0;
  unsigned long arg4 = 0;
  unsigned long arg5 = 0;
  int result = 0;

  option = get_option_from_args(args);
  if (option < 0) {
    return NULL;
  }

  switch (option) {
    case PR_SET_NAME: {
      const char* setname;
      if (!PyArg_ParseTuple(args, "is", &option, &setname)) {
        return NULL;
      }
      result = prctl(option, (unsigned long)setname, 0, 0, 0);
      if (result == -1) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
      }
      Py_RETURN_NONE;
    }

    case PR_GET_NAME: {
      char namebuf[16];
      if (!PyArg_ParseTuple(args, "i", &option)) {
        return NULL;
      }
      result = prctl(option, (unsigned long)namebuf, 0, 0, 0);
      if (result == -1) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
      }
      return Py_BuildValue("s", namebuf);
    }

    case PR_GET_CHILD_SUBREAPER:
    case PR_GET_FPEMU:
    case PR_GET_FPEXC:
    case PR_GET_PDEATHSIG: {
      if (!PyArg_ParseTuple(args, "i", &option)) {
        return NULL;
      }

      int value = 0;
      result = prctl(option, (unsigned long)&value, 0, 0, 0);
      if (result == -1) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
      }
      return PyLong_FromLong(value);
    }

    case PR_GET_UNALIGN: {
      unsigned int value = 0;
      result = prctl(option, (unsigned long)&value, 0, 0, 0);
      if (result == -1) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
      }
      return PyLong_FromUnsignedLong(value);
    }

    default:
      break;
  }

  if (!PyArg_ParseTuple(args, "i|kkkk", &option, &arg2, &arg3, &arg4, &arg5)) {
    return NULL;
  }

  result = prctl(option, arg2, arg3, arg4, arg5);
  if (result == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }
  return PyLong_FromLong(result);
}

/* =============================================================================
 * module bindings
 * ========================================================================== */

size_t pynix_prctl_defmethods(PyMethodDef* defs) {
  PyMethodDef* def = defs;

  def->ml_name = "prctl";
  def->ml_meth = pynix_prctl;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_prctl_docstr;
  def++;

  return (def - defs);
}

#define EXPORT_CONST(name) \
  PyModule_AddObject(module, #name, PyLong_FromLong(name));

int pynix_prctl_addobjects(PyObject* module) {
  EXPORT_CONST(PR_CAP_AMBIENT);
  EXPORT_CONST(PR_CAPBSET_DROP);
  EXPORT_CONST(PR_CAPBSET_READ);
  EXPORT_CONST(PR_ENDIAN_BIG);
  EXPORT_CONST(PR_ENDIAN_LITTLE);
  EXPORT_CONST(PR_ENDIAN_PPC_LITTLE);
  EXPORT_CONST(PR_FPEMU_NOPRINT);
  EXPORT_CONST(PR_FPEMU_SIGFPE);
  EXPORT_CONST(PR_FP_EXC_ASYNC);
  EXPORT_CONST(PR_FP_EXC_DISABLED);
  EXPORT_CONST(PR_FP_EXC_DIV);
  EXPORT_CONST(PR_FP_EXC_INV);
  EXPORT_CONST(PR_FP_EXC_NONRECOV);
  EXPORT_CONST(PR_FP_EXC_OVF);
  EXPORT_CONST(PR_FP_EXC_PRECISE);
  EXPORT_CONST(PR_FP_EXC_RES);
  EXPORT_CONST(PR_FP_EXC_SW_ENABLE);
  EXPORT_CONST(PR_FP_EXC_UND);
  EXPORT_CONST(PR_FP_MODE_FR);
  EXPORT_CONST(PR_FP_MODE_FRE);
  EXPORT_CONST(PR_GET_CHILD_SUBREAPER);
  EXPORT_CONST(PR_GET_DUMPABLE);
  EXPORT_CONST(PR_GET_ENDIAN);
  EXPORT_CONST(PR_GET_FPEMU);
  EXPORT_CONST(PR_GET_FPEXC);
  EXPORT_CONST(PR_GET_FP_MODE);
  EXPORT_CONST(PR_GET_KEEPCAPS);
  EXPORT_CONST(PR_GET_NAME);
  EXPORT_CONST(PR_GET_NO_NEW_PRIVS);
  EXPORT_CONST(PR_GET_PDEATHSIG);
  EXPORT_CONST(PR_GET_SECCOMP);
  EXPORT_CONST(PR_GET_SECUREBITS);
  EXPORT_CONST(PR_GET_SPECULATION_CTRL);
  EXPORT_CONST(PR_GET_THP_DISABLE);
  EXPORT_CONST(PR_GET_TID_ADDRESS);
  EXPORT_CONST(PR_GET_TIMERSLACK);
  EXPORT_CONST(PR_GET_TIMING);
  EXPORT_CONST(PR_GET_TSC);
  EXPORT_CONST(PR_GET_UNALIGN);
  EXPORT_CONST(PR_MCE_KILL);
  EXPORT_CONST(PR_MCE_KILL_CLEAR);
  EXPORT_CONST(PR_MCE_KILL_DEFAULT);
  EXPORT_CONST(PR_MCE_KILL_EARLY);
  EXPORT_CONST(PR_MCE_KILL_GET);
  EXPORT_CONST(PR_MCE_KILL_LATE);
  EXPORT_CONST(PR_MCE_KILL_SET);
  EXPORT_CONST(PR_MPX_DISABLE_MANAGEMENT);
  EXPORT_CONST(PR_MPX_ENABLE_MANAGEMENT);
  EXPORT_CONST(PR_SET_CHILD_SUBREAPER);
  EXPORT_CONST(PR_SET_DUMPABLE);
  EXPORT_CONST(PR_SET_ENDIAN);
  EXPORT_CONST(PR_SET_FPEMU);
  EXPORT_CONST(PR_SET_FPEXC);
  EXPORT_CONST(PR_SET_FP_MODE);
  EXPORT_CONST(PR_SET_KEEPCAPS);
  EXPORT_CONST(PR_SET_MM);
  EXPORT_CONST(PR_SET_MM_ARG_END);
  EXPORT_CONST(PR_SET_MM_ARG_START);
  EXPORT_CONST(PR_SET_MM_AUXV);
  EXPORT_CONST(PR_SET_MM_BRK);
  EXPORT_CONST(PR_SET_MM_END_CODE);
  EXPORT_CONST(PR_SET_MM_END_DATA);
  EXPORT_CONST(PR_SET_MM_ENV_END);
  EXPORT_CONST(PR_SET_MM_ENV_START);
  EXPORT_CONST(PR_SET_MM_EXE_FILE);
  EXPORT_CONST(PR_SET_MM_MAP);
  EXPORT_CONST(PR_SET_MM_MAP_SIZE);
  EXPORT_CONST(PR_SET_MM_START_BRK);
  EXPORT_CONST(PR_SET_MM_START_CODE);
  EXPORT_CONST(PR_SET_MM_START_DATA);
  EXPORT_CONST(PR_SET_MM_START_STACK);
  EXPORT_CONST(PR_SET_NAME);
  EXPORT_CONST(PR_SET_NO_NEW_PRIVS);
  EXPORT_CONST(PR_SET_PDEATHSIG);
  EXPORT_CONST(PR_SET_PTRACER);
  EXPORT_CONST(PR_SET_PTRACER_ANY);
  EXPORT_CONST(PR_SET_SECCOMP);
  EXPORT_CONST(PR_SET_SECUREBITS);
  EXPORT_CONST(PR_SET_SPECULATION_CTRL);
  EXPORT_CONST(PR_SET_THP_DISABLE);
  EXPORT_CONST(PR_SET_TIMERSLACK);
  EXPORT_CONST(PR_SET_TIMING);
  EXPORT_CONST(PR_SET_TSC);
  EXPORT_CONST(PR_SET_UNALIGN);
  EXPORT_CONST(PR_SPEC_DISABLE);
  EXPORT_CONST(PR_SPEC_ENABLE);
  EXPORT_CONST(PR_SPEC_FORCE_DISABLE);
  EXPORT_CONST(PR_SPEC_PRCTL);
  EXPORT_CONST(PR_SPEC_STORE_BYPASS);
  EXPORT_CONST(PR_TASK_PERF_EVENTS_DISABLE);
  EXPORT_CONST(PR_TASK_PERF_EVENTS_ENABLE);
  EXPORT_CONST(PR_TIMING_STATISTICAL);
  EXPORT_CONST(PR_TIMING_TIMESTAMP);
  EXPORT_CONST(PR_TSC_ENABLE);
  EXPORT_CONST(PR_TSC_SIGSEGV);
  EXPORT_CONST(PR_UNALIGN_NOPRINT);
  EXPORT_CONST(PR_UNALIGN_SIGBUS);

  return 0;
}
