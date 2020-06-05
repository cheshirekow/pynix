#
# Generated source bits for sphinx documentation
#
set(_stage ${CMAKE_CURRENT_BINARY_DIR}/stage)
set(_genbits)

add_custom_command(
  OUTPUT ${_stage}/bits
  COMMAND ${CMAKE_COMMAND} -E make_directory ${_stage}/bits
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Creating directory ${_stage}/bits")

# copy sourcefiles from the sourcetree into the build tree so that they're all
# accessible from one tree
set(_copyfiles)
set(_docsources
    .readthedocs.yml
    bits/impl_list.rst
    bits/badges.rst
    changelog.rst
    conf.py
    index.rst
    installation.rst
    impl_list.rst
    modules.rst
    release_notes.rst)
foreach(docsource ${_docsources})
  add_custom_command(
    OUTPUT ${_stage}/${docsource}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${docsource}
            ${_stage}/${docsource}
    DEPENDS ${docsource} ${_stage}/bits)
  if(${docsource} MATCHES "bits/.*")
    list(APPEND _genbits ${_stage}/${docsource})
  else()
    list(APPEND _copyfiles ${_stage}/${docsource})
  endif()
endforeach()

# TODO(josh): remove this iteration for just one file.
set(_genfiles)
foreach(genfile index.rst impl_list.rst README.rst)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${genfile}.stamp
    COMMAND python -B ${TANGENT_TOOLING}/gendoc.py --bits ${_stage}/bits
            ${CMAKE_CURRENT_SOURCE_DIR}/${genfile}
    COMMAND ${CMAKE_COMMAND} -E touch
            ${CMAKE_CURRENT_BINARY_DIR}/${genfile}.stamp
    # TODO(josh): not every file depends on every bit, so this is overkill, but
    # whatever... it's fine for now. We'll need more complicated code if we want
    # to actually declare which bits each file depends on.
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${genfile} ${_genbits}
            ${TANGENT_TOOLING}/gendoc.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
  list(APPEND _genfiles ${CMAKE_CURRENT_BINARY_DIR}/${genfile}.stamp)

  add_custom_target(
    run.pynix-doc-verify-${genfile}
    COMMAND python -B ${TANGENT_TOOLING}/gendoc.py --verify --bits
            ${_stage}/bits ${CMAKE_CURRENT_SOURCE_DIR}/${genfile}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

  add_test(
    NAME pynix-doc-verify-${genfile}
    COMMAND python -B ${TANGENT_TOOLING}/gendoc.py --verify --bits
            ${_stage}/bits ${CMAKE_CURRENT_SOURCE_DIR}/${genfile}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endforeach()
add_custom_target(genbits-pynix-doc DEPENDS ${_genbits})
add_dependencies(test-deps genbits-pynix-doc)
add_custom_target(genfiles-pynix-doc DEPENDS ${_genfiles} ${_copyfiles})
add_dependencies(gen genfiles-pynix-doc)

# For the subproject build, also copy some of the common files into the build
# tree.
add_custom_command(
  OUTPUT ${_stage}/conf_common.py
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/doc/conf.py
          ${_stage}/conf_common.py)
list(APPEND _copyfiles ${_stage}/conf_common.py)

add_custom_command(
  OUTPUT ${_stage}/sphinx-static/css/cheshire_theme.css
  COMMAND
    ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/doc/sphinx-static/css/cheshire_theme.css
    ${_stage}/sphinx-static/css/cheshire_theme.css
  DEPENDS ${_stage}/bits
          ${CMAKE_SOURCE_DIR}/doc/sphinx-static/css/cheshire_theme.css)
list(APPEND _copyfiles ${_stage}/sphinx-static/css/cheshire_theme.css)

add_custom_command(
  OUTPUT ${_stage}/rtd-requirements.txt
  COMMAND
    python -B ${TANGENT_TOOLING}/gen_rtd_requirements.py --tag from-travis
    --version ${_src_version} -o ${_stage}/rtd-requirements.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/rtd-requirements.txt.in
  DEPENDS ${TANGENT_TOOLING}/gen_rtd_requirements.py
          ${CMAKE_CURRENT_SOURCE_DIR}/rtd-requirements.txt.in
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
list(APPEND _genbits ${_stage}/rtd-requirements.txt)

set(_sphinx_manifest ${_genbits} ${_copyfiles})
list(SORT _sphinx_manifest)

# NOTE(josh): cmake 3.10 does not have list(JOIN)
string(REPLACE ";" "\n" _manifest_text "${_sphinx_manifest}")

# NOTE(josh): don't use file(WRITE) because that wont create a dependency on the
# configure step to rerun cmake if the file is missing
configure_file(MANIFEST.in ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST
               NEWLINE_STYLE UNIX)

add_custom_target(
  clean-pynix-sphinx-stage
  DEPENDS ${_sphinx_manifest} ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST
  COMMAND python -B ${TANGENT_TOOLING}/clean_stage.py
          ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST ${_stage}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Cleaning pynix/doc/stage")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/html/index.html
  COMMAND env PYTHONPATH=${CMAKE_SOURCE_DIR}:$<TARGET_FILE_DIR:pynix> #
          sphinx-build -M html ${_stage} ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/html/index.html
  DEPENDS clean-pynix-sphinx-stage ${_sphinx_manifest}
          ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST ${CMAKE_SOURCE_DIR}/doc/conf.py
          pynix
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Building sphinx documentation for pynix")

add_custom_target(doc-pynix DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/html/index.html)

add_custom_target(
  showdoc-pynix
  COMMAND xdg-open ${CMAKE_CURRENT_BINARY_DIR}/html/index.html
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/html/index.html)

add_dependencies(doc doc-pynix)
set_property(GLOBAL APPEND PROPERTY global_doc_files ${_sphinx_manifest})

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
                                       ${_copyfiles} ${_genbits})

set(_deploykey)
if(IS_TRAVIS_CI)
  set(_encodedkey
      ${CMAKE_SOURCE_DIR}/tangent/tooling/deploy_keys/pynix-rtd.deploy_key.enc)
  set(_deploykey ${CMAKE_CURRENT_BINARY_DIR}/pynix-rtd.deploy_key)
  add_custom_command(
    OUTPUT ${_deploykey}
    COMMAND
      # cmake-format: off
      openssl aes-256-cbc
      -K $ENV{encrypted_4e5c65f25f5d_key}
      -iv $ENV{encrypted_4e5c65f25f5d_iv}
      -in ${_encodedkey}
      -out ${_deploykey}
      -d
      # cmake-format: on
    COMMAND chmod 0600 ${_deploykey}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Decrypting pynix-rtd.deploy_key")
endif()

add_custom_target(
  push-pynix-rtd-repo
  COMMAND
    # cmake-format: off
    python -B ${TANGENT_TOOLING}/github.py sync-doc-artifacts
      --doc-repo git@github.com:cheshirekow/pynix-rtd.git
      --repo-dir ${CMAKE_CURRENT_BINARY_DIR}/pynix-rtd.git
      --scratch-tree ${CMAKE_CURRENT_BINARY_DIR}/scratch-tree
      --stage ${_stage}
      --deploy-key "${_deploykey}"
      --tag from-travis
    # cmake-format: on
  DEPENDS clean-pynix-sphinx-stage ${_sphinx_manifest} ${_deploykey}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Pushing doc artifacts to the RTD repository")
