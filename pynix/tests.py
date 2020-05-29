import errno
import os
import signal
import unittest

import pynix


class EpollTest(unittest.TestCase):

  def test_epoll_wait(self):
    (rfd, wfd) = os.pipe()
    epfd = pynix.epoll_create()

    pynix.epoll_ctl(epfd, pynix.EPOLL_CTL_ADD, rfd, pynix.EPOLLIN, None)
    epbuf = pynix.EpollEventBuf(10)
    self.assertEqual(10, epbuf.capacity)
    self.assertEqual(0, epbuf.size)

    pynix.epoll_wait(epfd, epbuf, 0)
    self.assertEqual(0, epbuf.size)

    os.write(wfd, b"hello")
    pynix.epoll_wait(epfd, epbuf, 0)
    self.assertEqual(1, epbuf.size)

    for idx, event in enumerate(epbuf):
      self.assertLess(idx, 1)
      self.assertTrue(
          event.events & pynix.EPOLLIN,
          "Expected events ({:0x}) to include EPOLLIN ({:0x})"
          .format(event.events, pynix.EPOLLIN))
      self.assertFalse(event.events & pynix.EPOLLOUT)
      self.assertIsNone(event.data)

    data = os.read(rfd, 3)
    pynix.epoll_wait(epfd, epbuf, 0)
    self.assertEqual(1, epbuf.size)
    data += os.read(rfd, 10)
    pynix.epoll_wait(epfd, epbuf, 0)
    self.assertEqual(0, epbuf.size)

    pynix.epoll_ctl(epfd, pynix.EPOLL_CTL_DEL, rfd)
    for fd in (epfd, rfd, wfd):
      os.close(fd)

  def test_epoll_pwait(self):
    (rfd, wfd) = os.pipe()
    epfd = pynix.epoll_create()
    sigmask = pynix.SigSet()

    pynix.epoll_ctl(epfd, pynix.EPOLL_CTL_ADD, rfd, pynix.EPOLLIN, None)
    epbuf = pynix.EpollEventBuf(10)
    self.assertEqual(10, epbuf.capacity)
    self.assertEqual(0, epbuf.size)

    pynix.epoll_pwait(epfd, epbuf, 0, sigmask)
    self.assertEqual(0, epbuf.size)

    os.write(wfd, b"hello")
    pynix.epoll_pwait(epfd, epbuf, 0, sigmask)
    self.assertEqual(1, epbuf.size)

    for idx, event in enumerate(epbuf):
      self.assertLess(idx, 1)
      self.assertTrue(
          event.events & pynix.EPOLLIN,
          "Expected events ({:0x}) to include EPOLLIN ({:0x})"
          .format(event.events, pynix.EPOLLIN))
      self.assertFalse(event.events & pynix.EPOLLOUT)
      self.assertIsNone(event.data)

    data = os.read(rfd, 3)
    pynix.epoll_pwait(epfd, epbuf, 0, sigmask)
    self.assertEqual(1, epbuf.size)
    data += os.read(rfd, 10)
    pynix.epoll_pwait(epfd, epbuf, 0, sigmask)
    self.assertEqual(0, epbuf.size)

    pynix.epoll_ctl(epfd, pynix.EPOLL_CTL_DEL, rfd)
    for fd in (epfd, rfd, wfd):
      os.close(fd)


class GetTidTest(unittest.TestCase):

  def test_gettid_matches_getpid(self):
    self.assertEqual(pynix.gettid(), os.getpid())


class PrctlTest(unittest.TestCase):

  def test_dumpable(self):
    pynix.prctl(pynix.PR_SET_DUMPABLE, 0)
    self.assertEqual(0, pynix.prctl(pynix.PR_GET_DUMPABLE))
    pynix.prctl(pynix.PR_SET_DUMPABLE, 1)
    self.assertEqual(1, pynix.prctl(pynix.PR_GET_DUMPABLE))

  def test_threadname(self):
    pynix.prctl(pynix.PR_SET_NAME, "pynixthread")
    self.assertEqual("pynixthread", pynix.prctl(pynix.PR_GET_NAME))

  def test_pdeathsig(self):
    self.assertEqual(0, pynix.prctl(pynix.PR_GET_PDEATHSIG))
    pynix.prctl(pynix.PR_SET_PDEATHSIG, signal.SIGUSR1)
    self.assertEqual(signal.SIGUSR1, pynix.prctl(pynix.PR_GET_PDEATHSIG))


class ResIdTest(unittest.TestCase):

  def test_functions_at_least_dont_throw(self):
    _ = pynix.getresuid()
    _ = pynix.getresgid()

  def test_functions_match_os_when_available(self):
    if hasattr(os, "getresuid"):
      self.assertEqual(pynix.getresuid(), getattr(os, "getresuid")())

    if hasattr(os, "getresgid"):
      self.assertEqual(pynix.getresgid(), getattr(os, "getresgid")())


class SignalFdTest(unittest.TestCase):

  def test_signalfd(self):
    procmask = pynix.SigSet()
    procmask.add(signal.SIGUSR1)
    pynix.sigprocmask(pynix.SIG_BLOCK, procmask, None)

    fdmask = pynix.SigSet()
    fdmask.add(signal.SIGUSR1)
    sigfd = pynix.signalfd(-1, fdmask, pynix.SFD_NONBLOCK | pynix.SFD_CLOEXEC)

    try:
      siginfo = pynix.signalfd_read(sigfd)
      self.fail("Expected signalfd_read to raise an exception")
    except OSError as err:
      self.assertEqual(err.errno, errno.EAGAIN)

    os.kill(os.getpid(), signal.SIGUSR1)
    siginfo = pynix.signalfd_read(sigfd)
    self.assertEqual(siginfo.ssi_signo, signal.SIGUSR1)


SIGNALS_RECEIVED = []


def signal_handler(signo, _stackframe):  # pylint: disable=W0613
  SIGNALS_RECEIVED.append(signo)


class SigProcMaskTest(unittest.TestCase):
  def setUp(self):
    signal.signal(signal.SIGUSR1, signal_handler)

  def test_sigprocmask(self):
    procmask = pynix.SigSet()
    procmask.add(signal.SIGUSR1)
    pynix.sigprocmask(pynix.SIG_BLOCK, procmask, None)

    os.kill(os.getpid(), signal.SIGUSR1)
    self.assertFalse(SIGNALS_RECEIVED)

    pynix.sigprocmask(pynix.SIG_UNBLOCK, procmask, None)
    self.assertTrue(SIGNALS_RECEIVED)
    self.assertEqual(SIGNALS_RECEIVED[0], signal.SIGUSR1)


def signo_range():
  for signo in range(1, 20):
    yield signo


class SigSetTest(unittest.TestCase):

  def test_initally_empty(self):
    sigset = pynix.SigSet()
    for signo in signo_range():
      self.assertFalse(sigset.ismember(signo))

  def test_fill_and_empty_fn(self):
    sigset = pynix.SigSet()
    sigset.fill()
    for signo in signo_range():
      message = "for signo {}".format(signo)
      self.assertTrue(sigset.ismember(signo), message)
    sigset.empty()
    for signo in signo_range():
      message = "for signo {}".format(signo)
      self.assertFalse(sigset.ismember(signo), message)

  def test_add_fn(self):
    sigset = pynix.SigSet()
    for signo in signo_range():
      self.assertFalse(sigset.ismember(signo))
      sigset.add(signo)
      self.assertTrue(sigset.ismember(signo))

  def test_delete_fn(self):
    sigset = pynix.SigSet()
    sigset.fill()
    for signo in signo_range():
      self.assertTrue(sigset.ismember(signo))
      sigset.delete(signo)
      self.assertFalse(sigset.ismember(signo))


if __name__ == "__main__":
  unittest.main()
