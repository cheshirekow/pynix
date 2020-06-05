// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

/* =============================================================================
 * pynix.sigprocmask()
 * ========================================================================== */

static PyObject* pynix_sigprocmask(PyObject* module, PyObject* args) {
  int how = 0;
  int err = 0;
  PynixSigset* set = NULL;
  PyObject* oldset_obj = NULL;
  sigset_t* oldset = NULL;

  if (!PyArg_ParseTuple(args, "iO!|O", &how, pynix_sigset_get_type(), &set,
                        &oldset_obj)) {
    Py_RETURN_NONE;
  }

  if (PyObject_IsInstance(oldset_obj, (PyObject*)pynix_sigset_get_type())) {
    oldset = &((PynixSigset*)oldset_obj)->cobj;
  } else if (oldset_obj != Py_None) {
#if PY_MAJOR_VERSION >= 3
    PyErr_WarnFormat(PyExc_RuntimeWarning, 1, "Ignoring `oldset` of type %s",
                     oldset_obj->ob_type->tp_name);
#else
    char msgbuf[64];
    snprintf(msgbuf, sizeof(msgbuf), "Ignoring `oldset` of type %s",
             oldset_obj->ob_type->tp_name);
    PyErr_WarnEx(PyExc_RuntimeWarning, msgbuf, 1);
#endif
  }

  err = sigprocmask(how, &(set->cobj), oldset);
  if (err == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }
  Py_RETURN_NONE;
}

/* =============================================================================
 * module bindings
 * ========================================================================== */

size_t pynix_sigprocmask_defmethods(PyMethodDef* defs) {
  PyMethodDef* def = defs;

  def->ml_name = "sigprocmask";
  def->ml_meth = pynix_sigprocmask;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_sigprocmask_docstr;
  def++;

  return (def - defs);
}

int pynix_sigprocmask_addobjects(PyObject* module) {
  EXPORT_CONST(SIG_BLOCK);
  EXPORT_CONST(SIG_UNBLOCK);
  EXPORT_CONST(SIG_SETMASK);
  return 0;
}
