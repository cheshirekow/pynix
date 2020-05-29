// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

 #include <unistd.h>

/* =============================================================================
 * pynix.chroot()
 * ========================================================================== */

static PyObject* pynix_chroot(PyObject* module, PyObject* args) {
  const char* path;

  if (!PyArg_ParseTuple(args, "s", &path)) {
    return NULL;
  }

  int err = chroot(path);
  if (err == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  Py_RETURN_NONE;
}

/* =============================================================================
 * module bindings
 * ========================================================================== */

size_t pynix_chroot_defmethods(PyMethodDef* defs) {
  PyMethodDef* def = defs;

  def->ml_name = "chroot";
  def->ml_meth = pynix_chroot;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_chroot_docstr;
  def++;

  return (def - defs);
}
