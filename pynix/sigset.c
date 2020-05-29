// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

#include <signal.h>

/* =============================================================================
 * pynix.SigSet()
 * ========================================================================== */

static int pynix_sigset_init(PynixSigset* self, PyObject* args,
                             PyObject* kwds) {
  static char* kwlist[] = {NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "", kwlist)) {
    return -1;
  }

  sigemptyset(&self->cobj);
  return 0;
}

static PyObject* pynix_sigset_empty(PynixSigset* self, PyObject* args) {
  sigemptyset(&self->cobj);
  Py_RETURN_NONE;
}

static PyObject* pynix_sigset_fill(PynixSigset* self, PyObject* args) {
  sigfillset(&self->cobj);
  Py_RETURN_NONE;
}

static PyObject* pynix_sigset_add(PynixSigset* self, PyObject* args) {
  int signo;
  if (!PyArg_ParseTuple(args, "i", &signo)) {
    Py_RETURN_NONE;
  }
  sigaddset(&self->cobj, signo);
  Py_RETURN_NONE;
}

static PyObject* pynix_sigset_delete(PynixSigset* self, PyObject* args) {
  int signo;
  if (!PyArg_ParseTuple(args, "i", &signo)) {
    Py_RETURN_NONE;
  }
  sigdelset(&self->cobj, signo);
  Py_RETURN_NONE;
}

static PyObject* pynix_sigset_ismember(PynixSigset* self, PyObject* args) {
  int signo;
  if (!PyArg_ParseTuple(args, "i", &signo)) {
    Py_RETURN_NONE;
  }
  if (sigismember(&self->cobj, signo)) {
    Py_RETURN_TRUE;
  } else {
    Py_RETURN_FALSE;
  }
}

static PyMethodDef g_sigset_methods[] = {
    {"empty", (PyCFunction)pynix_sigset_empty, METH_NOARGS},
    {"fill", (PyCFunction)pynix_sigset_fill, METH_NOARGS},
    {"add", (PyCFunction)pynix_sigset_add, METH_VARARGS},
    {"delete", (PyCFunction)pynix_sigset_delete, METH_VARARGS},
    {"ismember", (PyCFunction)pynix_sigset_ismember, METH_VARARGS},
    {NULL},
};

static const char g_sigset_doc[] =
    "Wraps `sigset_t`\n see: "
    "http://man7.org/linux/man-pages/man3/sigsetops.3.html";

static PyTypeObject g_sigset_type = {
    PyVarObject_HEAD_INIT(NULL, 0) /**/

        .tp_basicsize = sizeof(PynixSigset),
    .tp_doc = g_sigset_doc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_init = (initproc)pynix_sigset_init,
    .tp_itemsize = 0,
    .tp_methods = g_sigset_methods,
    .tp_name = "pynix.SigSet",
    .tp_new = PyType_GenericNew,
};

PyTypeObject* pynix_sigset_get_type() {
  return &g_sigset_type;
}

/* =============================================================================
 * module bindings
 * ========================================================================== */

int pynix_sigset_addobjects(PyObject* module) {
  Py_INCREF(&g_sigset_type);
  if (PyModule_AddObject(module, "SigSet", (PyObject*)&g_sigset_type) < 0) {
    Py_DECREF(&g_sigset_type);
    return -1;
  }

  return 0;
}
