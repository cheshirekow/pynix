// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

#include <sched.h>

#include "pynix/clone_constants.h"

/* =============================================================================
 * pynix.unshare()
 * ========================================================================== */

static PyObject* pynix_unshare(PyObject* module, PyObject* args) {
  int flags;

  if (!PyArg_ParseTuple(args, "i", &flags)) {
    return NULL;
  }

  int err = unshare(flags);
  if (err == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  Py_RETURN_NONE;
}

/* =============================================================================
 * module bindings
 * ========================================================================== */

size_t pynix_unshare_defmethods(PyMethodDef* defs) {
  PyMethodDef* def = defs;

  def->ml_name = "unshare";
  def->ml_meth = pynix_unshare;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_unshare_docstr;
  def++;

  return (def - defs);
}

int pynix_unshare_addobjects(PyObject* module) {
  EXPORT_CONST(CLONE_FILES);
  EXPORT_CONST(CLONE_FS);
  EXPORT_CONST(CLONE_NEWCGROUP);
  EXPORT_CONST(CLONE_NEWIPC);
  EXPORT_CONST(CLONE_NEWNET);
  EXPORT_CONST(CLONE_NEWNS);
  EXPORT_CONST(CLONE_NEWPID);
  EXPORT_CONST(CLONE_NEWTIME);
  EXPORT_CONST(CLONE_NEWUSER);
  EXPORT_CONST(CLONE_NEWUTS);
  EXPORT_CONST(CLONE_SYSVSEM);
  EXPORT_CONST(CLONE_THREAD);
  EXPORT_CONST(CLONE_SIGHAND);
  EXPORT_CONST(CLONE_VM);
  return 0;
}
