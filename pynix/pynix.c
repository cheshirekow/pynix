// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>
#include "pynix/include_first.h"

enum { NUM_MODULE_METHODS = 30 };

static PyMethodDef g_method_defs[NUM_MODULE_METHODS] = {{NULL, NULL, 0}};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef g_pynix_def = {  //
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "pynix",
    .m_doc = "pynix provides thin wrappers around linux specific system APIs",
    .m_size = 0,
    .m_methods = g_method_defs,
    .m_slots = NULL,
    .m_traverse = NULL,
    .m_clear = NULL,
    .m_free = NULL};
#endif

static PyObject* g_pynix_error;

#if PY_MAJOR_VERSION >= 3
#define RETURN_ERROR return NULL
#else
#define RETURN_ERROR return
#endif

#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit__pynix() {
#else
void init_pynix() {
#endif
  PyObject* module;
  size_t method_idx = 0;

  PyTypeObject* pynix_types[] = {pynix_epoll_event_get_type(),
                                 pynix_epoll_eventbuf_get_type(),
                                 pynix_epoll_eventbufiter_get_type(),
                                 pynix_inotify_event_get_type(),
                                 pynix_sigset_get_type(),
                                 pynix_siginfo_get_type(),
                                 NULL};
  for (PyTypeObject** typeobj = &pynix_types[0]; *typeobj; typeobj++) {
    if (PyType_Ready(*typeobj) < 0) {
      RETURN_ERROR;
    }
  }

  method_idx += pynix_chroot_defmethods(&g_method_defs[method_idx]);
  method_idx += pynix_epoll_defmethods(&g_method_defs[method_idx]);
  method_idx += pynix_gettid_defmethods(&g_method_defs[method_idx]);
  method_idx += pynix_inotify_defmethods(&g_method_defs[method_idx]);
  method_idx += pynix_mount_defmethods(&g_method_defs[method_idx]);
  method_idx += pynix_prctl_defmethods(&g_method_defs[method_idx]);
  method_idx += pynix_resid_defmethods(&g_method_defs[method_idx]);
  method_idx += pynix_signalfd_defmethods(&g_method_defs[method_idx]);
  method_idx += pynix_sigprocmask_defmethods(&g_method_defs[method_idx]);
  method_idx += pynix_unshare_defmethods(&g_method_defs[method_idx]);
  memset(&g_method_defs[method_idx++], 0, sizeof(PyMethodDef));
  if (method_idx > NUM_MODULE_METHODS) {
    PyErr_Format(PyExc_SystemError,
                 "pynix has attempted to register %d methods, but only "
                 "reserved space for %d",
                 (int)method_idx, (int)NUM_MODULE_METHODS);
    RETURN_ERROR;
  }

#if PY_MAJOR_VERSION >= 3
  module = PyModule_Create(&g_pynix_def);
#else
  module = Py_InitModule("pynix", g_method_defs);
#endif

  if (module == NULL) {
    RETURN_ERROR;
  }

  g_pynix_error = PyErr_NewException("pynix.error", NULL, NULL);
  Py_XINCREF(g_pynix_error);
  if (PyModule_AddObject(module, "error", g_pynix_error) < 0) {
    Py_XDECREF(g_pynix_error);
    Py_CLEAR(g_pynix_error);
    Py_DECREF(module);
    RETURN_ERROR;
  }

  pynix_epoll_addobjects(module);
  pynix_inotify_addobjects(module);
  pynix_mount_addobjects(module);
  pynix_prctl_addobjects(module);
  pynix_sigset_addobjects(module);
  pynix_sigprocmask_addobjects(module);
  pynix_signalfd_addobjects(module);
  pynix_unshare_addobjects(module);

#if PY_MAJOR_VERSION >= 3
  return module;
#endif
}
