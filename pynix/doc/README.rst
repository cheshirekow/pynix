===========================
Linux System APIs in Python
===========================

.. dynamic: badges-begin


.. image:: https://travis-ci.com/cheshirekow/pynix.svg?branch=master
    :target: https://travis-ci.com/cheshirekow/pynix

.. image:: https://readthedocs.org/projects/pynix/badge/?version=latest
    :target: https://pynix.readthedocs.io

.. image:: https://img.shields.io/badge/chat-on%20disord-green.svg?logo=discord
    :target: https://discord.com/channels/572162369283948594
.. dynamic: badges-end

The `pynix` python package provides access to a number of linux system APIs
that are not part of the python standard library, such as:

.. dynamic: impl_list-begin

* epoll_
* gettid_
* inotify_
* mount_
* prctl_
* `getres[ug]id`__
* `setres[ug]id`__
* signalfd_
* sigprocmask_
* sigset_
* unshare_

.. _epoll: https://www.man7.org/linux/man-pages/man7/epoll.7.html
.. _gettid: https://www.man7.org/linux/man-pages/man2/gettid.2.html
.. _inotify: https://www.man7.org/linux/man-pages/man7/inotify.7.html
.. _mount: https://www.man7.org/linux/man-pages/man2/mount.2.html
.. _prctl: https://www.man7.org/linux/man-pages/man2/prctl.2.html
.. __: https://www.man7.org/linux/man-pages/man2/getresuid.2.html
.. __: https://www.man7.org/linux/man-pages/man2/setresuid.2.html
.. _signalfd: https://www.man7.org/linux/man-pages/man2/signalfd.2.html
.. _sigprocmask: https://www.man7.org/linux/man-pages/man2/sigprocmask.2.html
.. _sigset: https://www.man7.org/linux/man-pages/man2/sigset.2.html
.. _unshare: https://www.man7.org/linux/man-pages/man2/unshare.2.html

.. dynamic: impl_list-end
