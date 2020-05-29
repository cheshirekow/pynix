============
Installation
============

.. default-role:: code

Install with pip
================

The easiest way to install `pynix` is from `pypi.org`_ using `pip`_. The
project name on `pypi.org` is `libpynix`, so the canonical insallation command
would be::

    pip install libpynix

However, depending on your distribution, you may need::

    sudo pip install libpynix

if you wish to in a system global location (e.g.
`/usr/local/lib/python3.6/dist-packages`), or::

    pip install --user libpynix

if you wish to install in a user global location (e.g.
`~/.local/lib/python3.6/site-packages`) which I would probably recommend for
most users.

.. _`pypi.org`: https://pypi.org/project/pynix/
.. _pip: https://pip.pypa.io/en/stable/

Install from source
===================

You can also install from source with pip. You can download a release package
from github__ or pypi__ and then install it directly with pip. For example::

  pip install libpynix-<version>.tar.gz

.. __: https://github.com/cheshirekow/pynix/releases
.. __: https://pypi.org/project/pynix/#files

Note that the release packages on github are automatically generated from git
tags which are the same commit used to generate the corresponding version
package on ``pypi.org``. So whether you install a particular version from
github or pypi shouldn't matter. They should be the exact same file.

Pip can also install directly from github. For example::

    pip install git+https://github.com/cheshirekow/pynix.git

If you wish to test a pre-release or dev package from a branch called
``foobar`` you can install it with::

    pip install "git+https://github.com/cheshirekow/pynix.git@foobar"

