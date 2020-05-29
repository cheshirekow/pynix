.. symbol: chroot

chroot() changes the root directory of the calling process to that
specified in path.  This directory will be used for pathnames
beginning with /.  The root directory is inherited by all children of
the calling process.

Only a privileged process (Linux: one with the CAP_SYS_CHROOT
capability in its user namespace) may call chroot().

This call changes an ingredient in the pathname resolution process
and does nothing else.  In particular, it is not intended to be used
for any kind of security purpose, neither to fully sandbox a process
nor to restrict filesystem system calls.  In the past, chroot() has
been used by daemons to restrict themselves prior to passing paths
supplied by untrusted users to system calls such as open(2).
However, if a folder is moved out of the chroot directory, an
attacker can exploit that to get out of the chroot directory as well.
The easiest way to do that is to chdir(2) to the to-be-moved
directory, wait for it to be moved out, then open a path like
../../../etc/passwd.

A slightly trickier variation also works under some circumstances if
chdir(2) is not permitted.  If a daemon allows a "chroot directory"
to be specified, that usually means that if you want to prevent
remote users from accessing files outside the chroot directory, you
must ensure that folders are never moved out of it.

This call does not change the current working directory, so that
after the call '.' can be outside the tree rooted at '/'.  In
particular, the superuser can escape from a "chroot jail" by doing:

    mkdir foo; chroot foo; cd ..

This call does not close open file descriptors, and such file
descriptors may allow access to files outside the chroot tree.


Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/chroot.2.html

.. symbol: epoll_create

epoll_create() creates a new epoll(7) instance.  Since Linux 2.6.8,
the size argument is ignored, but must be greater than zero; see
NOTES.

epoll_create() returns a file descriptor referring to the new epoll
instance.  This file descriptor is used for all the subsequent calls
to the epoll interface.  When no longer required, the file descriptor
returned by epoll_create() should be closed by using close(2).  When
all file descriptors referring to an epoll instance have been closed,
the kernel destroys the instance and releases the associated
resources for reuse.

epoll_create1()
    If flags is 0, then, other than the fact that the obsolete size
    argument is dropped, epoll_create1() is the same as epoll_create().
    The following value can be included in flags to obtain different
    behavior:

    EPOLL_CLOEXEC
          Set the close-on-exec (FD_CLOEXEC) flag on the new file
          descriptor.  See the description of the O_CLOEXEC flag in
          open(2) for reasons why this may be useful.


Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/epoll_create.2.html

.. symbol: epoll_ctl

int epoll_ctl(int epfd, int op, int fd, uint32_t events, PyObject* data);

This system call is used to add, modify, or remove entries in the
interest list of the epoll(7) instance referred to by the file
descriptor epfd.  It requests that the operation op be performed for
the target file descriptor, fd.

Valid values for the op argument are:

EPOLL_CTL_ADD
      Add an entry to the interest list of the epoll file
      descriptor, epfd.  The entry includes the file descriptor, fd,
      a reference to the corresponding open file description (see
      epoll(7) and open(2)), and the settings specified in event.

EPOLL_CTL_MOD
      Change the settings associated with fd in the interest list to
      the new settings specified in event.

EPOLL_CTL_DEL
      Remove (deregister) the target file descriptor fd from the
      interest list.  The event argument is ignored and can be NULL
      (but see BUGS below).

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/epoll_ctl.2.html

NOTE: If you provide a data object, it will be associated with the epoll
file descriptor and returned on events, but it's reference count will not be
incremented. You will need to hold your own reference.

.. symbol: epoll_wait

int epoll_wait(int epfd, pynix.EpollEventBuf* buf, int timeout);

The epoll_wait() system call waits for events on the epoll(7)
instance referred to by the file descriptor epfd.  The buffer pointed
to by events is used to return information from the ready list about
file descriptors in the interest list that have some events
available.  Up to maxevents are returned by epoll_wait().  The
maxevents argument must be greater than zero.

The timeout argument specifies the number of milliseconds that
epoll_wait() will block.  Time is measured against the
CLOCK_MONOTONIC clock.

A call to epoll_wait() will block until either:

* a file descriptor delivers an event;
* the call is interrupted by a signal handler; or
* the timeout expires.

Note that the timeout interval will be rounded up to the system clock
granularity, and kernel scheduling delays mean that the blocking
interval may overrun by a small amount.  Specifying a timeout of -1
causes epoll_wait() to block indefinitely, while specifying a timeout
equal to zero cause epoll_wait() to return immediately, even if no
events are available.

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/epoll_wait.2.html

NOTE: If you provide a data object, it will be associated with the epoll
file descriptor and returned on events, but it's reference count will not be
incremented. You will need to hold your own reference.

.. symbol: epoll_pwait

int epoll_pwait(
  int epfd, pynix.EpollEventBuf buf, int timeout, pynix.SigSet sigset);

The relationship between epoll_wait() and epoll_pwait() is analogous
to the relationship between select(2) and pselect(2): like
pselect(2), epoll_pwait() allows an application to safely wait until
either a file descriptor becomes ready or until a signal is caught.

The following epoll_pwait() call:

    ready = epoll_pwait(epfd, &events, maxevents, timeout, &sigmask);

is equivalent to atomically executing the following calls:

    sigset_t origmask;

    pthread_sigmask(SIG_SETMASK, &sigmask, &origmask);
    ready = epoll_wait(epfd, &events, maxevents, timeout);
    pthread_sigmask(SIG_SETMASK, &origmask, NULL);

The sigmask argument may be specified as NULL, in which case
epoll_pwait() is equivalent to epoll_wait().

.. symbol: gettid

gettid() returns the caller's thread ID (TID).  In a single-threaded
process, the thread ID is equal to the process ID (PID, as returned
by getpid(2)).  In a multithreaded process, all threads have the same
PID, but each one has a unique TID.  For further details, see the
discussion of CLONE_THREAD in clone(2).

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/gettid.2.html



.. symbol: inotify_init

inotify_init() initializes a new inotify instance and returns a file
descriptor associated with a new inotify event queue.

If flags is 0, then inotify_init1() is the same as inotify_init().
The following values can be bitwise ORed in flags to obtain different
behavior:

IN_NONBLOCK
      Set the O_NONBLOCK file status flag on the open file
      description (see open(2)) referred to by the new file
      descriptor.  Using this flag saves extra calls to fcntl(2) to
      achieve the same result.

IN_CLOEXEC
      Set the close-on-exec (FD_CLOEXEC) flag on the new file
      descriptor.  See the description of the O_CLOEXEC flag in
      open(2) for reasons why this may be useful.

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/inotify_init.2.html


.. symbol: inotify_add_watch

inotify_add_watch() adds a new watch, or modifies an existing watch,
for the file whose location is specified in pathname; the caller must
have read permission for this file.  The fd argument is a file
descriptor referring to the inotify instance whose watch list is to
be modified.  The events to be monitored for pathname are specified
in the mask bit-mask argument.  See inotify(7) for a description of
the bits that can be set in mask.

A successful call to inotify_add_watch() returns a unique watch
descriptor for this inotify instance, for the filesystem object
(inode) that corresponds to pathname.  If the filesystem object was
not previously being watched by this inotify instance, then the watch
descriptor is newly allocated.  If the filesystem object was already
being watched (perhaps via a different link to the same object), then
the descriptor for the existing watch is returned.

The watch descriptor is returned by later read(2)s from the inotify
file descriptor.  These reads fetch inotify_event structures (see
inotify(7)) indicating filesystem events; the watch descriptor inside
this structure identifies the object for which the event occurred.

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/inotify_add_watch.2.html

.. symbol: inotify_rm_watch

inotify_rm_watch() removes the watch associated with the watch
descriptor wd from the inotify instance associated with the file
descriptor fd.

Removing a watch causes an IN_IGNORED event to be generated for this
watch descriptor.  (See inotify(7).)

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/inotify_rm_watch.2.html

.. symbol: inotify_read

To determine what events have occurred, an application read(2)s from
the inotify file descriptor.  If no events have so far occurred,
then, assuming a blocking file descriptor, read(2) will block until
at least one event occurs (unless interrupted by a signal, in which
case the call fails with the error EINTR; see signal(7)).

Each successful read(2) returns a buffer containing one or more of
the following structures:

    struct inotify_event {
        int      wd;       /* Watch descriptor */
        uint32_t mask;     /* Mask describing event */
        uint32_t cookie;   /* Unique cookie associating related
                              events (for rename(2)) */
        uint32_t len;      /* Size of name field */
        char     name[];   /* Optional null-terminated name */
    };

wd identifies the watch for which this event occurs.  It is one of
the watch descriptors returned by a previous call to
inotify_add_watch(2).

mask contains bits that describe the event that occurred (see below).

cookie is a unique integer that connects related events.  Currently,
this is used only for rename events, and allows the resulting pair of
IN_MOVED_FROM and IN_MOVED_TO events to be connected by the applica‐
tion.  For all other event types, cookie is set to 0.

The name field is present only when an event is returned for a file
inside a watched directory; it identifies the filename within the
watched directory.  This filename is null-terminated, and may include
further null bytes ('\0') to align subsequent reads to a suitable
address boundary.

The len field counts all of the bytes in name, including the null
bytes; the length of each inotify_event structure is thus
sizeof(struct inotify_event)+len.

The behavior when the buffer given to read(2) is too small to return
information about the next event depends on the kernel version: in
kernels before 2.6.21, read(2) returns 0; since kernel 2.6.21,
read(2) fails with the error EINVAL.  Specifying a buffer of size

    sizeof(struct inotify_event) + NAME_MAX + 1

will be sufficient to read at least one event.

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man7/inotify.7.html

.. symbol: mount

mount() attaches the filesystem specified by source (which is often a
pathname referring to a device, but can also be the pathname of a
directory or file, or a dummy string) to the location (a directory or
file) specified by the pathname in target.

Appropriate privilege (Linux: the CAP_SYS_ADMIN capability) is
required to mount filesystems.

Values for the filesystemtype argument supported by the kernel are
listed in /proc/filesystems (e.g., "btrfs", "ext4", "jfs", "xfs",
"vfat", "fuse", "tmpfs", "cgroup", "proc", "mqueue", "nfs", "cifs",
"iso9660").  Further types may become available when the appropriate
modules are loaded.

The data argument is interpreted by the different filesystems.
Typically it is a string of comma-separated options understood by
this filesystem.  See mount(8) for details of the options available
for each filesystem type.

A call to mount() performs one of a number of general types of
operation, depending on the bits specified in mountflags.  The choice
of which operation to perform is determined by testing the bits set
in mountflags, with the tests being conducted in the order listed
here:

*  Remount an existing mount: mountflags includes MS_REMOUNT.

*  Create a bind mount: mountflags includes MS_BIND.

*  Change the propagation type of an existing mount: mountflags
  includes one of MS_SHARED, MS_PRIVATE, MS_SLAVE, or MS_UNBINDABLE.

*  Move an existing mount to a new location: mountflags includes
  MS_MOVE.

*  Create a new mount: mountflags includes none of the above flags.

Each of these operations is detailed later in this page.  Further
flags may be specified in mountflags to modify the behavior of
mount(), as described below.


Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/mount.2.html

.. symbol: prctl

prctl() is called with a first argument describing what to do (with
values defined in <linux/prctl.h>), and further arguments with a
significance depending on the first one.  The first argument can be:

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/signalfd.2.html

.. symbol: setresuid

setresuid() sets the real user ID, the effective user ID, and the
saved set-user-ID of the calling process.

An unprivileged process may change its real UID, effective UID, and
saved set-user-ID, each to one of: the current real UID, the current
effective UID or the current saved set-user-ID.

A privileged process (on Linux, one having the CAP_SETUID capability)
may set its real UID, effective UID, and saved set-user-ID to
arbitrary values.

If one of the arguments equals -1, the corresponding value is not
changed.

Regardless of what changes are made to the real UID, effective UID,
and saved set-user-ID, the filesystem UID is always set to the same
value as the (possibly new) effective UID.

Completely analogously, setresgid() sets the real GID, effective GID,
and saved set-group-ID of the calling process (and always modifies
the filesystem GID to be the same as the effective GID), with the
same restrictions for unprivileged processes.


Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/setresuid.2.html


.. symbol: getresuid

getresuid() returns the real UID, the effective UID, and the saved
set-user-ID of the calling process, in the arguments ruid, euid, and
suid, respectively.  getresgid() performs the analogous task for the
process's group IDs.

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/getresuid.2.html

.. symbol: signalfd

signalfd() creates a file descriptor that can be used to accept
signals targeted at the caller.  This provides an alternative to the
use of a signal handler or sigwaitinfo(2), and has the advantage that
the file descriptor may be monitored by select(2), poll(2), and
epoll(7).

The mask argument specifies the set of signals that the caller wishes
to accept via the file descriptor.  This argument is a signal set
whose contents can be initialized using the macros described in
sigsetops(3).  Normally, the set of signals to be received via the
file descriptor should be blocked using sigprocmask(2), to prevent
the signals being handled according to their default dispositions.
It is not possible to receive SIGKILL or SIGSTOP signals via a
signalfd file descriptor; these signals are silently ignored if
specified in mask.

If the fd argument is -1, then the call creates a new file descriptor
and associates the signal set specified in mask with that file
descriptor.  If fd is not -1, then it must specify a valid existing
signalfd file descriptor, and mask is used to replace the signal set
associated with that file descriptor.

Starting with Linux 2.6.27, the following values may be bitwise ORed
in flags to change the behavior of signalfd():

SFD_NONBLOCK  Set the O_NONBLOCK file status flag on the open file
              description (see open(2)) referred to by the new file
              descriptor.  Using this flag saves extra calls to
              fcntl(2) to achieve the same result.

SFD_CLOEXEC   Set the close-on-exec (FD_CLOEXEC) flag on the new file
              descriptor.  See the description of the O_CLOEXEC flag
              in open(2) for reasons why this may be useful.

In Linux up to version 2.6.26, the flags argument is unused, and must
be specified as zero.

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/signalfd.2.html

.. symbol: signalfd_read

If one or more of the signals specified in mask is pending for
the process, then the buffer supplied to read(2) is used to
return one or more signalfd_siginfo structures (see below)
that describe the signals.  The read(2) returns information
for as many signals as are pending and will fit in the
supplied buffer.  The buffer must be at least sizeof(struct
signalfd_siginfo) bytes.  The return value of the read(2) is
the total number of bytes read.

As a consequence of the read(2), the signals are consumed, so
that they are no longer pending for the process (i.e., will
not be caught by signal handlers, and cannot be accepted using
sigwaitinfo(2)).

If none of the signals in mask is pending for the process,
then the read(2) either blocks until one of the signals in
mask is generated for the process, or fails with the error
EAGAIN if the file descriptor has been made nonblocking.

Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/signalfd.2.html

.. symbol: sigprocmask

sigprocmask() is used to fetch and/or change the signal mask of the
calling thread.  The signal mask is the set of signals whose delivery
is currently blocked for the caller (see also signal(7) for more
details).

The behavior of the call is dependent on the value of how, as
follows.

SIG_BLOCK
      The set of blocked signals is the union of the current set and
      the set argument.

SIG_UNBLOCK
      The signals in set are removed from the current set of blocked
      signals.  It is permissible to attempt to unblock a signal
      which is not blocked.

SIG_SETMASK
      The set of blocked signals is set to the argument set.

If oldset is non-NULL, the previous value of the signal mask is
stored in oldset.

If set is NULL, then the signal mask is unchanged (i.e., how is
ignored), but the current value of the signal mask is nevertheless
returned in oldset (if it is not NULL).

A set of functions for modifying and inspecting variables of type
sigset_t ("signal sets") is described in sigsetops(3).

The use of sigprocmask() is unspecified in a multithreaded process;
see pthread_sigmask(3).

.. symbol: unshare

unshare() allows a process (or thread) to disassociate parts of its
execution context that are currently being shared with other
processes (or threads).  Part of the execution context, such as the
mount namespace, is shared implicitly when a new process is created
using fork(2) or vfork(2), while other parts, such as virtual memory,
may be shared by explicit request when creating a process or thread
using clone(2).

The main use of unshare() is to allow a process to control its shared
execution context without creating a new process.

The flags argument is a bit mask that specifies which parts of the
execution context should be unshared.  This argument is specified by
ORing together zero or more of the following constants:


Text from the linux manpage project:
https://www.man7.org/linux/man-pages/man2/unshare.2.html
