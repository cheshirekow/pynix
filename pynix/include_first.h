#pragma once
// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <signal.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <sys/signalfd.h>

#include "pynix/docstr.h"

typedef struct {
  PyObject_HEAD;
  uint32_t events;
  PyObject* data;
} PynixEpollEvent;

PyTypeObject* pynix_epoll_event_get_type();

typedef struct {
  PyObject_HEAD;
  struct epoll_event* events;
  Py_ssize_t capacity;
  Py_ssize_t size;
  Py_ssize_t stride;
} PynixEpollEventBuf;

PyTypeObject* pynix_epoll_eventbuf_get_type();

typedef struct {
  PyObject_HEAD;

  PynixEpollEventBuf* buf;
  Py_ssize_t idx;
} PynixEpollEventBufIter;

PyTypeObject* pynix_epoll_eventbufiter_get_type();

typedef struct {
  PyObject_HEAD;
  struct inotify_event cobj;
} PynixInotifyEvent;

PyTypeObject* pynix_inotify_event_get_type();

typedef struct {
  PyObject_HEAD;
  struct signalfd_siginfo cobj;
} PynixSiginfo;

PyTypeObject* pynix_siginfo_get_type();

typedef struct {
  PyObject_HEAD;
  sigset_t cobj;
} PynixSigset;

PyTypeObject* pynix_sigset_get_type();

size_t pynix_chroot_defmethods(PyMethodDef* defs);
size_t pynix_epoll_defmethods(PyMethodDef* defs);
size_t pynix_gettid_defmethods(PyMethodDef* defs);
size_t pynix_inotify_defmethods(PyMethodDef* defs);
size_t pynix_mount_defmethods(PyMethodDef* defs);
size_t pynix_prctl_defmethods(PyMethodDef* defs);
size_t pynix_resid_defmethods(PyMethodDef* defs);
size_t pynix_signalfd_defmethods(PyMethodDef* defs);
size_t pynix_sigprocmask_defmethods(PyMethodDef* defs);
size_t pynix_unshare_defmethods(PyMethodDef* defs);

int pynix_epoll_addobjects(PyObject* module);
int pynix_inotify_addobjects(PyObject* module);
int pynix_mount_addobjects(PyObject* module);
int pynix_prctl_addobjects(PyObject* module);
int pynix_signalfd_addobjects(PyObject* module);
int pynix_sigprocmask_addobjects(PyObject* module);
int pynix_sigset_addobjects(PyObject* module);
int pynix_unshare_addobjects(PyObject* module);
