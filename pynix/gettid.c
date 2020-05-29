// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <structmember.h>

#include "pynix/docstr.h"

/* =============================================================================
 * pynix.gettid()
 * ========================================================================== */

static PyObject* pynix_gettid(PyObject* module, PyObject* args) {
#if (__GLIBC__ >= 2 && __GLIBC_MINOR__ >= 30)
  pid_t result = gettid();
#else
  long result = syscall(186);
#endif

  if (result == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }
  return PyLong_FromLong(result);
}

/* =============================================================================
 * module bindings
 * ========================================================================== */

size_t pynix_gettid_defmethods(PyMethodDef* defs) {
  PyMethodDef* def = defs;

  def->ml_name = "gettid";
  def->ml_meth = pynix_gettid;
  def->ml_flags = METH_NOARGS;
  def->ml_doc = g_gettid_docstr;
  def++;

  return (def - defs);
}
