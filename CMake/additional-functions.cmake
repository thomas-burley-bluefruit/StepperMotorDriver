function(find_directories_recursive FOUND_DIRECTORIES SEARCH_PATH)
  file(GLOB_RECURSE new_list ${SEARCH_PATH})
  set(dir_list "")

  foreach(file_path ${new_list})
    get_filename_component(dir_path ${file_path} PATH)
    set(dir_list ${dir_list} ${dir_path})
  endforeach()

  list(REMOVE_DUPLICATES dir_list)
  set(${FOUND_DIRECTORIES} ${dir_list} PARENT_SCOPE)
endfunction()

function(add_additional_outputs TARGET_NAME)
  if(PLATFORM_ARM AND PLATFORM_COMPILER_GNU)
    set(SIZES_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.sizes)
    set(SYMBOL_TABLE_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.sym)

    # set(DUMP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.disasm)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET_NAME}> > ${SIZES_FILE} COMMENT "Dumping sizes to ${SIZES_FILE}")
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_NM} -ClnS $<TARGET_FILE:${TARGET_NAME}> > ${SYMBOL_TABLE_FILE} COMMENT "Dumping symbol table to ${SYMBOL_TABLE_FILE}")

    # add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_OBJDUMP} -S --disassemble $<TARGET_FILE:${TARGET_NAME}> > ${DUMP_FILE} COMMENT "Disassembling to ${DUMP_FILE}")
    get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)

    if(TARGET_TYPE STREQUAL EXECUTABLE)
      # set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.bin)
      # set(HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex)
      set(SREC_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.s19)

      # add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${TARGET_NAME}> ${BIN_FILE} COMMENT "Producing ${BIN_FILE}")
      # add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${TARGET_NAME}> ${HEX_FILE} COMMENT "Producing ${HEX_FILE}")
      add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Osrec $<TARGET_FILE:${TARGET_NAME}> ${SREC_FILE} COMMENT "Producing ${SREC_FILE}")
    endif()
  endif()
endfunction()
