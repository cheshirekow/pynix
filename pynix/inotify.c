// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

#include <sys/inotify.h>

#include <structmember.h>

/* =============================================================================
 * pynix.inotify_init()
 * ========================================================================== */

static PyObject* pynix_inotify_init(PyObject* module, PyObject* args) {
  int flags = 0;

  if (!PyArg_ParseTuple(args, "|i", &flags)) {
    return NULL;
  }

  int infd = inotify_init1(flags);
  if (infd == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  return PyLong_FromLong(infd);
}

/* =============================================================================
 * pynix.inotify_add_watch()
 * ========================================================================== */

static PyObject* pynix_inotify_add_watch(PyObject* module, PyObject* args) {
  int infd = 0;
  const char* pathname = NULL;
  uint32_t mask;

  if (!PyArg_ParseTuple(args, "isI", &infd, &pathname, &mask)) {
    return NULL;
  }

  int wd = inotify_add_watch(infd, pathname, mask);
  if (wd == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  return PyLong_FromLong(wd);
}

/* =============================================================================
 * pynix.inotify_rm_watch()
 * ========================================================================== */

static PyObject* pynix_inotify_rm_watch(PyObject* module, PyObject* args) {
  int infd = 0;
  int wd = 0;

  if (!PyArg_ParseTuple(args, "ii", &infd, &wd)) {
    return NULL;
  }

  int result = inotify_rm_watch(infd, wd);
  if (result == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  Py_RETURN_NONE;
}

/* =============================================================================
 * pynix.InotifyEvent
 * ========================================================================== */

static PyMemberDef g_inotify_event_members[] = {
    {.name = "wd",
     .type = T_INT,
     .offset =
         offsetof(PynixInotifyEvent, cobj) + offsetof(struct inotify_event, wd),
     .flags = 0,
     .doc = "Watch descriptor"},
    {.name = "mask",
     .type = T_UINT,
     .offset = offsetof(PynixInotifyEvent, cobj) +
               offsetof(struct inotify_event, mask),
     .flags = 0,
     .doc = "Mask describing event"},
    {.name = "cookie",
     .type = T_UINT,
     .offset = offsetof(PynixInotifyEvent, cobj) +
               offsetof(struct inotify_event, cookie),
     .flags = 0,
     .doc = "Unique cookie associating related events (for rename(2))"},
    {.name = "len",
     .type = T_UINT,
     .offset = offsetof(PynixInotifyEvent, cobj) +
               offsetof(struct inotify_event, len),
     .flags = 0,
     .doc = "Size of the name field"},
    {.name = "name",
     .type = T_STRING,
     .offset = offsetof(PynixInotifyEvent, cobj) +
               offsetof(struct inotify_event, name),
     .flags = 0,
     .doc = "Optional null-terminated name"},
    {NULL}};

static PyMethodDef g_inotify_event_methods[] = {
    {NULL},
};

static PyTypeObject g_inotify_event_type = {
    PyVarObject_HEAD_INIT(NULL, 0) /**/
        .tp_name = "pynix.InotifyEvent",
    .tp_doc =
        "Wraps `struct inotify_event`\n see "
        "https://www.man7.org/linux/man-pages/man7/inotify.7.html",
    .tp_basicsize = sizeof(PynixInotifyEvent) + NAME_MAX + 1,
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_methods = g_inotify_event_methods,
    .tp_members = g_inotify_event_members,
};

PyTypeObject* pynix_inotify_event_get_type(void) {
  return &g_inotify_event_type;
}

/* =============================================================================
 * pynix.inotify_read()
 * ========================================================================== */

PyObject* pynix_inotify_read(PyObject* module, PyObject* args) {
  int fd;
  int read_result;

  PynixInotifyEvent* event;

  if (!PyArg_ParseTuple(args, "i", &fd)) {
    return NULL;
  }

  event = PyObject_New(PynixInotifyEvent, &g_inotify_event_type);
  if (!event) {
    return NULL;
  }

  read_result =
      read(fd, &event->cobj, sizeof(struct inotify_event) + NAME_MAX + 1);
  if (read_result < 0) {
    PyErr_SetFromErrno(PyExc_OSError);
    PyObject_DEL(event);
    return NULL;
  } else if (read_result == 0) {
    Py_RETURN_NONE;
  } else {
    return (PyObject*)event;
  }
}

/* =============================================================================
 * module bindings
 * ========================================================================== */

size_t pynix_inotify_defmethods(PyMethodDef* defs) {
  PyMethodDef* def = defs;

  def->ml_name = "inotify_init";
  def->ml_meth = pynix_inotify_init;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_inotify_init_docstr;
  def++;

  def->ml_name = "inotify_add_watch";
  def->ml_meth = pynix_inotify_add_watch;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_inotify_add_watch_docstr;
  def++;

  def->ml_name = "inotify_rm_watch";
  def->ml_meth = pynix_inotify_rm_watch;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_inotify_rm_watch_docstr;
  def++;

  def->ml_name = "inotify_read";
  def->ml_meth = pynix_inotify_read;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_inotify_read_docstr;
  def++;

  return (def - defs);
}

int pynix_inotify_addobjects(PyObject* module) {
  EXPORT_CONST(IN_ACCESS);
  EXPORT_CONST(IN_ATTRIB);
  EXPORT_CONST(IN_CLOEXEC);
  EXPORT_CONST(IN_CLOSE_NOWRITE);
  EXPORT_CONST(IN_CLOSE_WRITE);
  EXPORT_CONST(IN_CREATE);
  EXPORT_CONST(IN_DELETE);
  EXPORT_CONST(IN_DELETE_SELF);
  EXPORT_CONST(IN_MODIFY);
  EXPORT_CONST(IN_MOVED_FROM);
  EXPORT_CONST(IN_MOVED_TO);
  EXPORT_CONST(IN_MOVE_SELF);
  EXPORT_CONST(IN_NONBLOCK);
  EXPORT_CONST(IN_OPEN);
  return 0;
}
