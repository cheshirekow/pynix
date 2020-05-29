// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

#include <sys/mount.h>

/* =============================================================================
 * pynix.mount()
 * ========================================================================== */

static PyObject* pynix_mount(PyObject* module, PyObject* args) {
  const char* source;
  const char* target;
  const char* filesystemtype;
  unsigned long mountflags = 0;
  const char* data = NULL;

  if (!PyArg_ParseTuple(args, "sss|ks", &source, &target, &filesystemtype,
                        &mountflags, &data)) {
    return NULL;
  }

  int err = mount(source, target, filesystemtype, mountflags, data);
  if (err == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  Py_RETURN_NONE;
}

/* =============================================================================
 * module bindings
 * ========================================================================== */

size_t pynix_mount_defmethods(PyMethodDef* defs) {
  PyMethodDef* def = defs;

  def->ml_name = "mount";
  def->ml_meth = pynix_mount;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_mount_docstr;
  def++;

  return (def - defs);
}

#define EXPORT_CONST(name) \
  PyModule_AddObject(module, #name, PyLong_FromLong(name));

int pynix_mount_addobjects(PyObject* module) {
  EXPORT_CONST(MS_BIND);
  EXPORT_CONST(MS_REMOUNT);
  EXPORT_CONST(MS_SHARED);
  EXPORT_CONST(MS_PRIVATE);
  EXPORT_CONST(MS_SLAVE);
  EXPORT_CONST(MS_UNBINDABLE);
  EXPORT_CONST(MS_MOVE);

  EXPORT_CONST(MS_DIRSYNC);
#ifdef MS_LAZYTIME
  EXPORT_CONST(MS_LAZYTIME);
#endif
  EXPORT_CONST(MS_MANDLOCK);
  EXPORT_CONST(MS_NOATIME);
  EXPORT_CONST(MS_NODEV);
  EXPORT_CONST(MS_NODIRATIME);
  EXPORT_CONST(MS_NOEXEC);
  EXPORT_CONST(MS_NOSUID);
  EXPORT_CONST(MS_RDONLY);
  EXPORT_CONST(MS_REC);
  EXPORT_CONST(MS_RELATIME);
  EXPORT_CONST(MS_SILENT);
  EXPORT_CONST(MS_STRICTATIME);
  EXPORT_CONST(MS_SYNCHRONOUS);

  return 0;
}
