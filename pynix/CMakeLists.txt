# NOTE(josh): create a .o with three symbols containing the contents of a file.
# Note that a terminating null character is not appended, so the file should
# contain this before executing objectcopy.
#
# The object file contains the following symbols: _binary_FILENAME_start
# _binary_FILENAME_end _binary_FILENAME_size
#
# Consider this as an option for authoring long docstrings in a separate
# reStructuredText document. Then write a python script to split the strings,
# execute objcopy, then `ar` them back together into a static library.
#
# add_custom_command( OUTPUT signalfd_doc.o COMMAND objcopy -I binary -O
# elf32-little --rename-section .data=.rodata signalfd_doc.rst signalfd_doc.o)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/docstr.h
         ${CMAKE_CURRENT_SOURCE_DIR}/docstr.c
  COMMAND
    python -B ${CMAKE_CURRENT_SOURCE_DIR}/docstr/docdb_gen.py --out-dir
    ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/docstr/docdb.rst
  DEPENDS docstr/docdb.rst docstr/docdb_gen.py)

set_source_files_properties(docstr.h docstr.c PROPERTIES GENERATED TRUE)
set(_genfiles docstr.h docstr.c)
add_custom_target(genfiles-pynix DEPENDS ${_genfiles})

check_call(
  COMMAND python-config --includes
  OUTPUT_VARIABLE _pydev_includes
  OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE " +" ";" _pydev_includes "${_pydev_includes}")
string(REGEX REPLACE "-I" "" _pydev_includes "${_pydev_includes}")

check_call(
  COMMAND python-config --libs
  OUTPUT_VARIABLE _pydev_libs
  OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE " +" ";" _pydev_libs "${_pydev_libs}")

check_call(
  COMMAND python-config --extension-suffix
  OUTPUT_VARIABLE _pydev_suffix
  OUTPUT_STRIP_TRAILING_WHITESPACE)

cc_library(
  pynix
  SHARED
  docstr.c
  chroot.c
  epoll.c
  gettid.c
  inotify.c
  mount.c
  prctl.c
  pynix.c
  resid.c
  signalfd.c
  sigprocmask.c
  sigset.c
  unshare.c
  PROPERTIES OUTPUT_NAME "_pynix" #
             SUFFIX ${_pydev_suffix}
  INC PRIVATE ${_pydev_includes}
  DEPS ${_pydev_libs})

# NOTE(josh): we can't include this in the cc_library() call above because the
# "empty string" argument will get chomped during arguments forwarding
set_target_properties(pynix PROPERTIES PREFIX "")

add_test(
  NAME pynix-sigset-test
  COMMAND
    ${CMAKE_COMMAND} -E #
    env PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR} #
    python -Bm pynix.tests SigSetTest
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_test(
  NAME pynix-signalfd-test
  COMMAND
    ${CMAKE_COMMAND} -E #
    env PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR} #
    python -Bm pynix.tests SignalFdTest
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_test(
  NAME pynix-sigprocmask-test
  COMMAND
    ${CMAKE_COMMAND} -E #
    env PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR} #
    python -Bm pynix.tests SigProcMaskTest
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_test(
  NAME pynix-epoll-test
  COMMAND
    ${CMAKE_COMMAND} -E #
    env PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR} #
    python -Bm pynix.tests EpollTest
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

execute_process(
  COMMAND python -Bm pynix --version
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  RESULT_VARIABLE _resultcode
  OUTPUT_VARIABLE _src_version
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT _resultcode EQUAL 0)
  message(FATAL_ERROR "Failed to get pynix version number from __init__.py")
endif()

check_call(
  COMMAND
    python -B ${TANGENT_TOOLING}/get_wheel_name.py --name libpynix
    --version-from=${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
    --has-extension-module
  OUTPUT_VARIABLE _wheel_filename
  OUTPUT_STRIP_TRAILING_WHITESPACE)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/.egg
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/.egg
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

set(_distdir ${CMAKE_CURRENT_BINARY_DIR}/dist)
add_custom_command(
  OUTPUT ${_distdir}/libpynix-${_src_version}.tar.gz
         ${_distdir}/${_wheel_filename}
  COMMAND
    # cmake-format: off
    python -B pynix/pypi/setup.py
    build --build-base ${CMAKE_CURRENT_BINARY_DIR}
    egg_info --egg-base ${CMAKE_CURRENT_BINARY_DIR}/.egg
    bdist_wheel
      --bdist-dir ${CMAKE_CURRENT_BINARY_DIR}/bdist
      --dist-dir ${_distdir}
    sdist --dist-dir ${_distdir}
    # cmake-format: on
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.egg
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_target(pynix-wheel DEPENDS ${_distdir}/${_wheel_filename})
add_dependencies(wheels pynix-wheel)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/release_notes-${_src_version}.rst
  COMMAND
    python -B ${TANGENT_TOOLING}/get_release_notes.py
    pynix/doc/release_notes.rst \$\${TRAVIS_TAG} -o
    ${CMAKE_CURRENT_BINARY_DIR}/release_notes-${_src_version}.rst
  DEPENDS doc/release_notes.rst
          ${CMAKE_SOURCE_DIR}/tangent/tooling/get_release_notes.py
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/release_notes-${_src_version}.md
  COMMAND
    pandoc -s -o ${CMAKE_CURRENT_BINARY_DIR}/release_notes-${_src_version}.md
    ${CMAKE_CURRENT_BINARY_DIR}/release_notes-${_src_version}.rst
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/release_notes-${_src_version}.rst
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_subdirectory(doc)

add_custom_target(
  push-pynix-github-release
  COMMAND
    # cmake-format: off
    python -B ${TANGENT_TOOLING}/github.py push-release
      --message ${CMAKE_CURRENT_BINARY_DIR}/release_notes-${_src_version}.md
      --repo-slug cheshirekow/pynix
      --tag from-travis
      ${_distdir}/libpynix-${_src_version}.tar.gz
      ${_distdir}/${_wheel_filename}
    # cmake-format: on
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/release_notes-${_src_version}.md
          ${_distdir}/libpynix-${_src_version}.tar.gz
          ${_distdir}/${_wheel_filename}
  COMMENT "Uploading release artifacts"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
