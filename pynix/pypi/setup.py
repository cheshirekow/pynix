import io

from setuptools import setup, Extension

GITHUB_URL = 'https://github.com/cheshirekow/pynix'

VERSION = None
with io.open('pynix/__init__.py', encoding='utf-8') as infile:
  for line in infile:
    line = line.strip()
    if line.startswith('VERSION ='):
      VERSION = line.split('=', 1)[1].strip().strip("'\"")

assert VERSION is not None


with io.open('pynix/README.rst', encoding='utf-8') as infile:
  long_description = infile.read()

setup(
    author='Josh Bialkowski',
    author_email='josh.bialkowski@gmail.com',
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: GNU General Public License v3 (GPLv3)"
    ],
    description=(
        "Extension module which exposes several linux-specific system APIs"),
    download_url='{}/archive/{}.tar.gz'.format(GITHUB_URL, VERSION),
    ext_modules=[
        Extension(
            include_dirs=["."],
            name="_pynix",
            sources=[
                "pynix/chroot.c",
                "pynix/epoll.c",
                "pynix/gettid.c",
                "pynix/inotify.c",
                "pynix/mount.c",
                "pynix/prctl.c",
                "pynix/pynix.c",
                "pynix/resid.c",
                "pynix/signalfd.c",
                "pynix/sigprocmask.c",
                "pynix/sigset.c",
                "pynix/unshare.c",
            ])
    ],
    license="GPLv3",
    long_description=long_description,
    keywords=['linux', 'extension-module'],
    name='libpynix',
    packages=[
        'pynix',
    ],
    url=GITHUB_URL,
    version=VERSION,
)
