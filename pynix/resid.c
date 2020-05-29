// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

#include <unistd.h>

/* =============================================================================
 * pynix.setresuid()
 * ========================================================================== */

static PyObject* pynix_setresuid(PyObject* module, PyObject* args) {
  uid_t ruid;
  uid_t euid;
  uid_t suid;

  if (!PyArg_ParseTuple(args, "III", &ruid, &euid, &suid)) {
    return NULL;
  }

  int err = setresuid(ruid, euid, suid);
  if (err == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  Py_RETURN_NONE;
}

/* =============================================================================
 * pynix.setresgid()
 * ========================================================================== */

static PyObject* pynix_setresgid(PyObject* module, PyObject* args) {
  gid_t rgid;
  gid_t egid;
  gid_t sgid;

  if (!PyArg_ParseTuple(args, "III", &rgid, &egid, &sgid)) {
    return NULL;
  }

  int err = setresgid(rgid, egid, sgid);
  if (err == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  Py_RETURN_NONE;
}

/* =============================================================================
 * pynix.getresuid()
 * ========================================================================== */

static PyObject* pynix_getresuid(PyObject* module, PyObject* args) {
  uid_t ruid;
  uid_t euid;
  uid_t suid;

  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }

  int err = getresuid(&ruid, &euid, &suid);
  if (err == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  return Py_BuildValue("III", ruid, euid, suid);
}

/* =============================================================================
 * pynix.getresgid()
 * ========================================================================== */

static PyObject* pynix_getresgid(PyObject* module, PyObject* args) {
  gid_t rgid;
  gid_t egid;
  gid_t sgid;

  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }

  int err = getresgid(&rgid, &egid, &sgid);
  if (err == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  return Py_BuildValue("III", rgid, egid, sgid);
}

/* =============================================================================
 * module bindings
 * ==========================================================================
 */

size_t pynix_resid_defmethods(PyMethodDef* defs) {
  PyMethodDef* def = defs;

  def->ml_name = "setresuid";
  def->ml_meth = pynix_setresuid;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_setresuid_docstr;
  def++;

  def->ml_name = "setresgid";
  def->ml_meth = pynix_setresgid;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_setresuid_docstr;
  def++;

  def->ml_name = "getresuid";
  def->ml_meth = pynix_getresuid;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_getresuid_docstr;
  def++;

  def->ml_name = "getresgid";
  def->ml_meth = pynix_getresgid;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_getresuid_docstr;
  def++;

  return (def - defs);
}
