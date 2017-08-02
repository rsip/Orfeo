function(install_rule src_file)

  get_filename_component(src_file_EXT ${src_file} EXT)
  get_filename_component(src_file_NAME ${src_file} NAME)
  get_filename_component(src_file_NAME_WE ${src_file} NAME_WE)
  get_filename_component(src_file_PATH ${src_file} PATH)

  file(GLOB src_file_star "${src_file_PATH}/${src_file_NAME_WE}*")

  foreach(sfile ${src_file_star})
    #message( "sfile=${sfile}")
    get_filename_component(sfile_NAME ${sfile} NAME)
    string(TOLOWER "${sfile_NAME}" sfile_NAME_LOWER)
    
    get_filename_component(sfile_ABS "${sfile}" ABSOLUTE) #file_full -> sfile_ABS
    string(TOLOWER "${sfile_ABS}" sfile_ABS_LOWER) #file_full_lower -> sfile_ABS_LOWER
    
    if ("${sfile_NAME_LOWER}" MATCHES "^(otb|mvd)([a-z]+)(headertest|testdriver)")
      message( "SKIP INSTALL for ${sfile_NAME_LOWER}")
      continue()
    endif()    
    set(install_type)
    set(install_dir)
    if("${sfile_ABS_LOWER}" MATCHES "(\\.exe)$")
      set(install_type PROGRAMS)
      set(install_dir bin)
    elseif("${sfile_ABS_LOWER}" MATCHES "(\\.dll)$")
      set(install_type FILES)
      set(install_dir bin) 
    elseif("${sfile_ABS_LOWER}" MATCHES "(\\.lib)$")
      set(install_type FILES)
      set(install_dir lib)
    elseif("${sfile_ABS_LOWER}" MATCHES "(\\.pyd)$")
      #python bindings are installed in install_python_bindings
      continue()
    elseif("${sfile_ABS_LOWER}" MATCHES "\\.lnk$")
      #we don't install symlink on windows. issue a warning
      set(install_type "symlink")
      set(install_dir)
    elseif("${sfile_ABS_LOWER}" MATCHES "(\\.a)$")
      set(install_type FILES)
      set(install_dir lib)
    elseif("${sfile_ABS_LOWER}" MATCHES "\\.prl$")
      continue()
    elseif("${sfile_ABS_LOWER}" MATCHES "\\.la$")
      continue()
    elseif("${sfile_NAME_LOWER}" MATCHES "^otbapp_")
      set(install_dir "lib/otb/applications")
      set(install_type PROGRAMS)
      install_otbapp_wrapper_scripts(${sfile_NAME})
    else()
      if(UNIX)
	#the last else() loop where we run a 'file' command to find file type and directory
	set(install_type)
	set(install_dir)
	#message("sfile_ABS=${sfile_ABS}")
	#this is whole other story
	detect_using_file_command(${sfile_ABS} install_type install_dir)
      endif(UNIX)
    endif()

    if(install_type STREQUAL "symlink")
      #we don't install symlink on windows. issue a warning
      message(WARNING "${sfile_ABS} is a symbolic link and this will be excluded from package") 
      return()
    endif()
    
    if(NOT install_type OR NOT install_type)
      message(FATAL_ERROR "sfile_ABS=${sfile_ABS}.\ninstall_type=${install_dir}\ninstall_dir=${install_dir}")
      return()
    endif()

    install(${install_type} "${sfile}"
      DESTINATION
      "${PKG_STAGE_DIR}/${install_dir}"
      )
    
  
  endforeach()  
endfunction()   

