// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

#include <structmember.h>
#include <sys/epoll.h>

/* =============================================================================
 * pynix.epoll_create()
 * ========================================================================== */

static PyObject* pynix_epoll_create(PyObject* module, PyObject* args) {
  int flags = 0;

  if (!PyArg_ParseTuple(args, "|i", &flags)) {
    return NULL;
  }

  int epfd = epoll_create1(flags);
  if (epfd == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  return PyLong_FromLong(epfd);
}

/* =============================================================================
 * pynix.epoll_ctl()
 * ========================================================================== */

static PyObject* pynix_epoll_ctl(PyObject* module, PyObject* args) {
  int epfd;
  int op;
  int fd;
  uint32_t events = 0;
  PyObject* data = NULL;

  if (!PyArg_ParseTuple(args, "iii|IO", &epfd, &op, &fd, &events, &data)) {
    return NULL;
  }

  struct epoll_event event = {.events = events, .data = {.ptr = data}};

  int err = epoll_ctl(epfd, op, fd, &event);
  if (err == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  Py_RETURN_NONE;
}

/* =============================================================================
 * pynix.EpollEvent
 * ========================================================================== */

static PyMemberDef g_epoll_event_members[] = {
    {.name = "events",
     .type = T_UINT,
     .offset = offsetof(PynixEpollEvent, events),
     .flags = 0,
     .doc = "bitmask of available events"},
    {.name = "data",
     .type = T_OBJECT,
     .offset = offsetof(PynixEpollEvent, data),
     .flags = 0,
     .doc = "user data stored with event entry"},
    {NULL}};

static PyMethodDef g_epoll_event_methods[] = {
    {NULL},
};

static PyTypeObject g_epoll_event_type = {
    PyVarObject_HEAD_INIT(NULL, 0) /**/
        .tp_name = "pynix.EpollEvent",
    .tp_doc =
        "Wraps `struct epoll_event`\n see "
        "https://www.man7.org/linux/man-pages/man7/epoll.7.html",
    .tp_basicsize = sizeof(PynixEpollEvent),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_methods = g_epoll_event_methods,
    .tp_members = g_epoll_event_members,
};

PyTypeObject* pynix_epoll_event_get_type() {
  return &g_epoll_event_type;
}

/* =============================================================================
 * pynix.EpollEventBuf
 * ========================================================================== */

static PyMemberDef g_epoll_eventbuf_members[] = {
    {.name = "capacity",
     .type = T_PYSSIZET,
     .offset = offsetof(PynixEpollEventBuf, capacity),
     .flags = READONLY,
     .doc = "number of epollevents slots that can be filled"},
    {.name = "size",
     .type = T_PYSSIZET,
     .offset = offsetof(PynixEpollEventBuf, size),
     .flags = 0,
     .doc = "number of epoll events filled"},
    {NULL}};

static PyMethodDef g_epoll_eventbuf_methods[] = {
    {NULL},
};

static int epoll_eventbuf_init(PynixEpollEventBuf* eventbuf, PyObject* args,
                               PyObject* kwds) {
  static char* kwlist[] = {"capacity", NULL};
  int capacity = 10;

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &capacity)) {
    return -1;
  }

  eventbuf->events = calloc(capacity, sizeof(struct epoll_event));
  eventbuf->size = 0;
  eventbuf->capacity = capacity;
  eventbuf->stride = sizeof(struct epoll_event);
  return 0;
}

static void epoll_eventbuf_destroy(PynixEpollEventBuf* eventbuf) {
  if (eventbuf->events) {
    free(eventbuf->events);
  }
}

static PyObject* epoll_eventbuf_iter(PynixEpollEventBuf* eventbuf) {
  PynixEpollEventBufIter* iter =
      PyObject_New(PynixEpollEventBufIter, pynix_epoll_eventbufiter_get_type());

  Py_INCREF(eventbuf);
  iter->buf = eventbuf;
  iter->idx = 0;

  return (PyObject*)iter;
}

static int epoll_eventbuf_getbuffer(PynixEpollEventBuf* eventbuf,
                                    Py_buffer* view, int flags) {
  if (flags & PyBUF_WRITABLE) {
    PyErr_SetString(
        (PyObject*)eventbuf,
        "Request for writable view into pynix.EpollEventBuf, which is not"
        " allowed");
    return -1;
  }

  Py_INCREF(eventbuf);
  view->obj = (PyObject*)eventbuf;
  view->buf = eventbuf->events;
  view->len = eventbuf->size * sizeof(struct epoll_event);
  view->readonly = 1;
  view->itemsize = sizeof(struct epoll_event);
  view->ndim = 1;
  view->shape = &eventbuf->size;
  view->strides = &eventbuf->stride;
  if (flags & PyBUF_FORMAT) {
    view->format = "IO";
  }
  return 0;
}

static PyBufferProcs g_epoll_eventbuf_as_buffer = {
    .bf_getbuffer = (getbufferproc)epoll_eventbuf_getbuffer};

static const char g_epoll_eventbuf_doc[] =
    "pynix.EpollEvent(int capacity)\n"
    "Stores an array of `struct epoll_event`\n see "
    "https://www.man7.org/linux/man-pages/man7/epoll.7.html";

static PyTypeObject g_epoll_eventbuf_type = {
    PyVarObject_HEAD_INIT(NULL, 0) /**/
        .tp_as_buffer = &g_epoll_eventbuf_as_buffer,
    .tp_basicsize = sizeof(PynixEpollEventBuf),
    .tp_dealloc = (destructor)epoll_eventbuf_destroy,
    .tp_doc = g_epoll_eventbuf_doc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_init = (initproc)epoll_eventbuf_init,
    .tp_itemsize = 0,
    .tp_iter = (getiterfunc)epoll_eventbuf_iter,
    .tp_name = "pynix.EpollEventBuf",
    .tp_new = PyType_GenericNew,
    .tp_members = g_epoll_eventbuf_members,
    .tp_methods = g_epoll_eventbuf_methods,

};

PyTypeObject* pynix_epoll_eventbuf_get_type() {
  return &g_epoll_eventbuf_type;
}

/* =============================================================================
 * pynix.EpollEventBufIter
 * ========================================================================== */

static int epoll_eventbufiter_init(PynixEpollEventBufIter* iter, PyObject* args,
                                   PyObject* kwds) {
  static char* kwlist[] = {"eventbuf", NULL};
  PyObject* eventbuf = NULL;
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                   &g_epoll_eventbuf_type, &eventbuf)) {
    return -1;
  }

  Py_XINCREF(eventbuf);
  iter->buf = (PynixEpollEventBuf*)eventbuf;
  iter->idx = 0;
  return 0;
}

static void epoll_eventbufiter_destroy(PynixEpollEventBufIter* iter) {
  Py_XDECREF(iter->buf);
}

static PyObject* epoll_eventbufiter_iter(PyObject* iter) {
  Py_INCREF(iter);
  return iter;
}

static PyObject* epoll_eventbufiter_iternext(PynixEpollEventBufIter* iter) {
  if (iter->idx >= iter->buf->size) {
    return NULL;
  }

  PynixEpollEvent* event = PyObject_New(PynixEpollEvent, &g_epoll_event_type);
  if (!event) {
    return NULL;
  }
  event->events = iter->buf->events[iter->idx].events;
  event->data = (PyObject*)iter->buf->events[iter->idx].data.ptr;
  iter->idx++;

  return (PyObject*)event;
}

static const char g_epoll_eventbufiter_doc[] =
    "pynix.EpollEventIter(pynix.EpollEventBuf buffer)\n"
    "Iterator over events in an eventbuf`\n see "
    "https://www.man7.org/linux/man-pages/man7/epoll.7.html";

static PyTypeObject g_epoll_eventbufiter_type = {
    PyVarObject_HEAD_INIT(NULL, 0) /**/
        .tp_basicsize = sizeof(PynixEpollEventBufIter),
    .tp_dealloc = (destructor)epoll_eventbufiter_destroy,
    .tp_doc = g_epoll_eventbufiter_doc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_init = (initproc)epoll_eventbufiter_init,
    .tp_itemsize = 0,
    .tp_name = "pynix.EpollEventBufIter",
    .tp_new = PyType_GenericNew,
    .tp_iter = epoll_eventbufiter_iter,
    .tp_iternext = (iternextfunc)epoll_eventbufiter_iternext,
};

PyTypeObject* pynix_epoll_eventbufiter_get_type() {
  return &g_epoll_eventbufiter_type;
}

/* =============================================================================
 * pynix.epoll_wait()
 * ========================================================================== */

static PyObject* pynix_epoll_wait(PyObject* module, PyObject* args) {
  int epfd;
  PyObject* eventbuf_obj;
  int timeout;

  if (!PyArg_ParseTuple(args, "iO!i", &epfd, pynix_epoll_eventbuf_get_type(),
                        &eventbuf_obj, &timeout)) {
    return NULL;
  }

  PynixEpollEventBuf* eventbuf = (PynixEpollEventBuf*)eventbuf_obj;

  int numready =
      epoll_wait(epfd, eventbuf->events, eventbuf->capacity, timeout);
  if (numready == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  eventbuf->size =
      (numready <= eventbuf->capacity) ? numready : eventbuf->capacity;
  return PyLong_FromLong(numready);
}

/* =============================================================================
 * pynix.epoll_pwait()
 * ========================================================================== */

static PyObject* pynix_epoll_pwait(PyObject* module, PyObject* args) {
  int epfd;
  PyObject* eventbuf_obj;
  int timeout;
  PyObject* sigmask_obj;

  if (!PyArg_ParseTuple(args, "iO!iO!", &epfd, pynix_epoll_eventbuf_get_type(),
                        &eventbuf_obj, &timeout, pynix_sigset_get_type(),
                        &sigmask_obj)) {
    return NULL;
  }

  PynixEpollEventBuf* eventbuf = (PynixEpollEventBuf*)eventbuf_obj;
  PynixSigset* sigmask = (PynixSigset*)sigmask_obj;

  int numready = epoll_pwait(epfd, eventbuf->events, eventbuf->capacity,
                             timeout, &sigmask->cobj);
  if (numready == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    return NULL;
  }

  eventbuf->size =
      (numready <= eventbuf->capacity) ? numready : eventbuf->capacity;
  return PyLong_FromLong(numready);
}

/* =============================================================================
 * module bindings
 * ========================================================================== */

size_t pynix_epoll_defmethods(PyMethodDef* defs) {
  PyMethodDef* def = defs;

  def->ml_name = "epoll_create";
  def->ml_meth = pynix_epoll_create;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_epoll_create_docstr;
  def++;

  def->ml_name = "epoll_ctl";
  def->ml_meth = pynix_epoll_ctl;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_epoll_ctl_docstr;
  def++;

  def->ml_name = "epoll_wait";
  def->ml_meth = pynix_epoll_wait;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_epoll_wait_docstr;
  def++;

  def->ml_name = "epoll_pwait";
  def->ml_meth = pynix_epoll_pwait;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_epoll_pwait_docstr;
  def++;

  return (def - defs);
}

#define EXPORT_CONST(name) \
  PyModule_AddObject(module, #name, PyLong_FromLong(name));

int pynix_epoll_addobjects(PyObject* module) {
  EXPORT_CONST(EPOLL_CLOEXEC);
  EXPORT_CONST(EPOLL_CTL_ADD);
  EXPORT_CONST(EPOLL_CTL_MOD);
  EXPORT_CONST(EPOLL_CTL_DEL);

  EXPORT_CONST(EPOLLIN);
  EXPORT_CONST(EPOLLOUT);
  EXPORT_CONST(EPOLLRDHUP);
  EXPORT_CONST(EPOLLPRI);
  EXPORT_CONST(EPOLLERR);
  EXPORT_CONST(EPOLLHUP);
  EXPORT_CONST(EPOLLET);
  EXPORT_CONST(EPOLLONESHOT);
#ifdef EPOLLWAKEUP
  EXPORT_CONST(EPOLLWAKEUP);
#endif
#ifdef EPOLLEXCLUSIVE
  EXPORT_CONST(EPOLLEXCLUSIVE);
#endif

  Py_INCREF(&g_epoll_event_type);
  if (PyModule_AddObject(module, "EpollEvent", (PyObject*)&g_epoll_event_type) <
      0) {
    Py_DECREF(&g_epoll_event_type);
    return -1;
  }

  Py_INCREF(&g_epoll_eventbuf_type);
  if (PyModule_AddObject(module, "EpollEventBuf",
                         (PyObject*)&g_epoll_eventbuf_type) < 0) {
    Py_DECREF(&g_epoll_eventbuf_type);
    return -1;
  }

  Py_INCREF(&g_epoll_eventbufiter_type);
  if (PyModule_AddObject(module, "EpollEventBufIter",
                         (PyObject*)&g_epoll_eventbufiter_type) < 0) {
    Py_DECREF(&g_epoll_eventbufiter_type);
    return -1;
  }

  return 0;
}
