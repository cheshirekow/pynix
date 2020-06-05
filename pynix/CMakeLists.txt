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
unset(_genfiles)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/docstr.h
         ${CMAKE_CURRENT_SOURCE_DIR}/docstr.c
  COMMAND python -B ${CMAKE_CURRENT_SOURCE_DIR}/codegen/gen_docstr.py --out-dir
          ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND clang-format-8 -style=File -i ${CMAKE_CURRENT_SOURCE_DIR}/docstr.h
          ${CMAKE_CURRENT_SOURCE_DIR}/docstr.c
  DEPENDS codegen/docdb.rst codegen/gen_docstr.py codegen/docstr.h.in
          codegen/docstr.c.in)
list(APPEND _genfiles docstr.h docstr.c)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/prctl_constants.h
  COMMAND python -B ${CMAKE_CURRENT_SOURCE_DIR}/codegen/gen_constants.py
          --out-dir ${CMAKE_CURRENT_SOURCE_DIR} prctl
  COMMAND clang-format-8 -style=File -i
          ${CMAKE_CURRENT_SOURCE_DIR}/prctl_constants.h
  DEPENDS codegen/gen_constants.py codegen/constants.h.in)
list(APPEND _genfiles prctl_constants.h)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/clone_constants.h
  COMMAND python -B ${CMAKE_CURRENT_SOURCE_DIR}/codegen/gen_constants.py
          --out-dir ${CMAKE_CURRENT_SOURCE_DIR} clone
  COMMAND clang-format-8 -style=File -i
          ${CMAKE_CURRENT_SOURCE_DIR}/clone_constants.h
  DEPENDS codegen/gen_constants.py codegen/constants.h.in)
list(APPEND _genfiles clone_constants.h)

set_source_files_properties(${_genfiles} PROPERTIES GENERATED TRUE)
add_custom_target(genfiles-pynix DEPENDS ${_genfiles})

check_call(
  COMMAND python-config --cflags
  OUTPUT_VARIABLE _pydev_cflags_raw
  OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE " +" ";" _pydev_cflags_raw "${_pydev_cflags_raw}")
unset(_pydev_cflags)
foreach(cflag ${_pydev_cflags_raw})
  if("${cflag}" MATCHES "-(I|(isystem)|(iquote)).*")
    # pass
  elseif("${cflag}" MATCHES "-isystem.*")
    # pass
  else()
    list(APPEND _pydev_cflags)
  endif()
endforeach()

check_call(COMMAND python-config --includes
           OUTPUT_VARIABLE _pydev_includes OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE " +" ";" _pydev_includes "${_pydev_includes}")
string(REGEX REPLACE "-I" "" _pydev_includes "${_pydev_includes}")

check_call(COMMAND python-config --ldflags
           OUTPUT_VARIABLE _pydev_ldflags_raw OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE " +" ";" _pydev_ldflags_raw "${_pydev_ldflags_raw}")
unset(_pydev_libdirs)
unset(_pydev_ldflags)
foreach(ldflag ${_pydev_ldflags_raw})
  if("${ldflag}" MATCHES "-L.*")
    list(APPEND _pydev_libdirs "${ldflag}")
  elseif("${ldflag}" MATCHES "-l.*")
    # pass
  else()
    list(APPEND _pydev_ldflags)
  endif()
endforeach()

check_call(COMMAND python-config --libs
           OUTPUT_VARIABLE _pydev_libs OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE " +" ";" _pydev_libs "${_pydev_libs}")

execute_process(
  COMMAND python-config --extension-suffix
  OUTPUT_VARIABLE _pydev_suffix
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE _exitcode
  ERROR_QUIET)
if(NOT "${_exitcode}" EQUAL 0)
  set(_pydev_suffix ".so")
endif()

set(_hdrs prctl_constants.h include_first.h clone_constants.h)
set(_srcs
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
    unshare.c)

cc_library(
  pynix SHARED
  SRCS ${_srcs}
  PROPERTIES OUTPUT_NAME "_pynix" #
             SUFFIX ${_pydev_suffix}
  INC PRIVATE ${_pydev_includes}
  LIBDIRS PRIVATE ${_pydev_libdirs}
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

file(READ __init__.py _pynix_init_content)
string(REGEX MATCH "VERSION = \"([^\"]+)\"" _version_defn
             "${_pynix_init_content}")
if(NOT _version_defn)
  message(FATAL_ERROR "Failed to get pynix version number from __init__.py")
endif()
set(_src_version ${CMAKE_MATCH_1})

check_call(
  COMMAND
    python -B ${TANGENT_TOOLING}/get_wheel_name.py --name libpynix
    --version-from=${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
    --has-extension-module
  OUTPUT_VARIABLE _wheel_filename OUTPUT_STRIP_TRAILING_WHITESPACE)

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
add_custom_target(pynix-wheel-localenv DEPENDS ${_distdir}/${_wheel_filename})

check_call(COMMAND id -u OUTPUT_VARIABLE _user_id
                                         OUTPUT_STRIP_TRAILING_WHITESPACE)

check_call(COMMAND id -g OUTPUT_VARIABLE _group_id
                                         OUTPUT_STRIP_TRAILING_WHITESPACE)

unset(_wheels)
set(_manylinux_names "cp27-cp27m" "cp27-cp27mu" "cp35-cp35m" "cp36-cp36m"
                     "cp37-cp37m" "cp38-cp38")
foreach(pyslug ${_manylinux_names})
  set(_platform manylinux2010_x86_64)
  set(_linux_wheelfile libpynix-${_src_version}-${pyslug}-linux_x86_64.whl)
  set(_manylinux_wheelfile libpynix-${_src_version}-${pyslug}-${_platform}.whl)

  set(_docker_run
      # cmake-format: off
      docker run --rm #
      -v ${CMAKE_SOURCE_DIR}:/io #
      -v ${CMAKE_BINARY_DIR}/pybuild-${pyslug}:/io/build #
      -v ${_distdir}:/io/dist #
      -w /io #
      -u ${_user_id}:${_group_id} #
      quay.io/pypa/${_platform} #
      # cmake-format: on
  )

  add_custom_command(
    OUTPUT ${_distdir}/${_linux_wheelfile}
    COMMAND ${CMAKE_COMMAND} -E make_directory
            ${CMAKE_BINARY_DIR}/pybuild-${pyslug}
    COMMAND ${_docker_run} #
            /opt/python/${pyslug}/bin/python pynix/pypi/setup.py bdist_wheel
    DEPENDS ${_hdrs} ${_srcs} pypi/setup.py)
  list(APPEND _wheels ${_distdir}/${_linux_wheelfile})

  add_custom_command(
    OUTPUT ${_distdir}/${_manylinux_wheelfile}
    COMMAND ${_docker_run} #
            auditwheel show dist/${_linux_wheelfile}
    COMMAND
      ${_docker_run} #
      auditwheel repair dist/${_linux_wheelfile} --plat "${_platform}" #
      -w /io/dist
    DEPENDS ${_distdir}/${_linux_wheelfile})
  list(APPEND _wheels ${_distdir}/${_manylinux_wheelfile})

  add_custom_target(pynix-wheel-${pyslug}
                    DEPENDS ${_distdir}/${_manylinux_wheelfile})
endforeach()

add_custom_target(pynix-wheels-manylinux DEPENDS ${_wheels})
add_dependencies(wheels pynix-wheel-localenv pynix-wheels-manylinux)

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
  push-pynix-github-pseudorelease-tag
  COMMAND python -B ${TANGENT_TOOLING}/github.py create-pseudorelease-tag
          cheshirekow/pynix \$\${TRAVIS_BRANCH}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Pushing psuedo-release tag")

add_custom_target(
  push-pynix-github-release
  COMMAND
    # cmake-format: off
    python -B ${TANGENT_TOOLING}/github.py push-release
      --message ${CMAKE_CURRENT_BINARY_DIR}/release_notes-${_src_version}.md
      --repo-slug cheshirekow/pynix
      --tag from-travis
      ${_distdir}/libpynix-${_src_version}.tar.gz
      ${_wheels}
    # cmake-format: on
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/release_notes-${_src_version}.md
          ${_distdir}/libpynix-${_src_version}.tar.gz ${_wheels}
  COMMENT "Uploading release artifacts"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
