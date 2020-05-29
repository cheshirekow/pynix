// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

#include <structmember.h>

/* =============================================================================
 * pynix.signalfd()
 * ========================================================================== */

static PyObject* pynix_signalfd(PyObject* module, PyObject* args) {
  int fd;
  PyObject* mask;
  int flags;
  if (!PyArg_ParseTuple(args, "iO!i", &fd, pynix_sigset_get_type(), &mask,
                        &flags)) {
    Py_RETURN_NONE;
  }

  int outfd = signalfd(fd, &((PynixSigset*)mask)->cobj, flags);
  if (outfd == -1) {
    PyErr_SetFromErrno(PyExc_OSError);
    Py_RETURN_NONE;
  }
  return PyLong_FromLong(outfd);
}

/* =============================================================================
 * pynix.SigInfo
 * ========================================================================== */

static PyMemberDef g_siginfo_members[] = {
    {.name = "ssi_signo",
     .type = T_UINT,
     .offset = offsetof(PynixSiginfo, cobj) +
               offsetof(struct signalfd_siginfo, ssi_signo),
     .flags = 0,
     .doc = "Signal number"},
    {.name = "ssi_errno",
     .type = T_INT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_errno),
     .doc = "Error number (unused)"},
    {.name = "ssi_code",
     .type = T_INT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_code),
     .doc = "Signal code"},
    {.name = "ssi_pid",
     .type = T_UINT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_pid),
     .doc = "PID of sender"},
    {.name = "ssi_uid",
     .type = T_UINT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_uid),
     .doc = "Real UID of sender"},
    {.name = "ssi_fd",
     .type = T_INT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_fd),
     .doc = "File descriptor (SIGIO)"},
    {.name = "ssi_tid",
     .type = T_UINT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_tid),
     .doc = "Kernel timer ID (POSIX timers)"},
    {.name = "ssi_band",
     .type = T_UINT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_band),
     .doc = "Band event (SIGIO)"},
    {.name = "ssi_overrun",
     .type = T_UINT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_overrun),
     .doc = "POSIX timer overrun count"},
    {.name = "ssi_trapno",
     .type = T_UINT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_trapno),
     .doc = "Trap number that caused signal"},
    {.name = "ssi_status",
     .type = T_INT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_status),
     .doc = "Exit status or signal (SIGCHLD)"},
    {.name = "ssi_int",
     .type = T_INT,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_int),
     .doc = "Integer sent by sigqueue(3)"},
    {.name = "ssi_ptr",
     .type = T_ULONGLONG,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_ptr),
     .doc = "Pointer sent by sigqueue(3)"},
    {.name = "ssi_utime",
     .type = T_ULONGLONG,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_utime),
     .doc = "User CPU time consumed (SIGCHLD"},
    {.name = "ssi_stime",
     .type = T_ULONGLONG,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_stime),
     .doc = "System CPU time consumed (SIGCHLD)"},
    {.name = "ssi_stime",
     .type = T_ULONGLONG,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_stime),
     .doc = "System CPU time consumed (SIGCHLD)"},
    {.name = "ssi_addr",
     .type = T_ULONGLONG,
     .offset = 0,
     .flags = offsetof(PynixSiginfo, cobj) +
              offsetof(struct signalfd_siginfo, ssi_addr),
     .doc = "Address that generated signal (for hardware-generated signals)"},
    // {.name = "ssi_addr_lsb",
    //  .type = T_USHORT,
    //  .offset = 0,
    //  .flags = offsetof(PynixSiginfo, cobj) +
    //           offsetof(struct signalfd_siginfo, ssi_addr_lsb),
    //  .doc = "Least significant bit of address (SIGBUS; since Linux 2.6.37)"},
    {NULL},
};

static PyMethodDef g_siginfo_methods[] = {
    {NULL},
};

static PyTypeObject g_siginfo_type = {
    PyVarObject_HEAD_INIT(NULL, 0) /**/
        .tp_name = "pynix.SigInfo",
    .tp_doc = "Wraps `struct signalfd_siginfo`",
    .tp_basicsize = sizeof(PynixSiginfo),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_methods = g_siginfo_methods,
    .tp_members = g_siginfo_members,
};

PyTypeObject* pynix_siginfo_get_type() {
  return &g_siginfo_type;
}

/* =============================================================================
 * pynix.signalfd_read()
 * ========================================================================== */

PyObject* pynix_signalfd_read(PyObject* module, PyObject* args) {
  int fd;
  int read_result;

  PynixSiginfo* siginfo;

  if (!PyArg_ParseTuple(args, "i", &fd)) {
    Py_RETURN_NONE;
  }

  siginfo = PyObject_New(PynixSiginfo, &g_siginfo_type);
  if (!siginfo) {
    Py_RETURN_NONE;
  }

  read_result = read(fd, &siginfo->cobj, sizeof(struct signalfd_siginfo));
  if (read_result < 0) {
    PyErr_SetFromErrno(PyExc_OSError);
    PyObject_DEL(siginfo);
    return NULL;
  } else if (read_result == 0) {
    Py_RETURN_NONE;
  } else {
    return (PyObject*)siginfo;
  }
}

/* =============================================================================
 * module bindings
 * ========================================================================== */

size_t pynix_signalfd_defmethods(PyMethodDef* defs) {
  PyMethodDef* def = defs;

  def->ml_name = "signalfd";
  def->ml_meth = pynix_signalfd;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_signalfd_docstr;
  def++;

  def->ml_name = "signalfd_read";
  def->ml_meth = pynix_signalfd_read;
  def->ml_flags = METH_VARARGS;
  def->ml_doc = g_signalfd_read_docstr;
  def++;

  return (def - defs);
}

int pynix_signalfd_addobjects(PyObject* module) {
  PyModule_AddObject(module, "SFD_NONBLOCK", PyLong_FromLong(SFD_NONBLOCK));
  PyModule_AddObject(module, "SFD_CLOEXEC", PyLong_FromLong(SFD_CLOEXEC));

  Py_INCREF(&g_siginfo_type);
  if (PyModule_AddObject(module, "SigInfo", (PyObject*)&g_siginfo_type) < 0) {
    Py_DECREF(&g_siginfo_type);
    return -1;
  }

  return 0;
}
