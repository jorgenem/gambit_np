set(CMAKE_Fortran_VERBOSE_FLAG "-v")
set(CMAKE_Fortran_FORMAT_FIXED_FLAG "-fixed")
set(CMAKE_Fortran_FORMAT_FREE_FLAG "-free")

set(CMAKE_Fortran_COMPILE_OPTIONS_PIC "-KPIC")
set(CMAKE_SHARED_LIBRARY_Fortran_FLAGS "-KPIC")
set(CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS "-G")
set(CMAKE_SHARED_LIBRARY_RUNTIME_Fortran_FLAG "-R")
set(CMAKE_SHARED_LIBRARY_RUNTIME_Fortran_FLAG_SEP ":")
set(CMAKE_SHARED_LIBRARY_SONAME_Fortran_FLAG "-h")
set(CMAKE_EXECUTABLE_RUNTIME_Fortran_FLAG "-R")

set(CMAKE_Fortran_FLAGS_INIT "")
set(CMAKE_Fortran_FLAGS_DEBUG_INIT "-g")
set(CMAKE_Fortran_FLAGS_MINSIZEREL_INIT "-xO2 -xspace -DNDEBUG")
set(CMAKE_Fortran_FLAGS_RELEASE_INIT "-xO3 -DNDEBUG")
set(CMAKE_Fortran_FLAGS_RELWITHDEBINFO_INIT "-g -xO2 -DNDEBUG")
set(CMAKE_Fortran_MODDIR_FLAG "-moddir=")
set(CMAKE_Fortran_MODPATH_FLAG "-M")

set(CMAKE_Fortran_CREATE_PREPROCESSED_SOURCE "<CMAKE_Fortran_COMPILER> <DEFINES> <FLAGS> -F <SOURCE> -o <PREPROCESSED_SOURCE>")
set(CMAKE_Fortran_CREATE_ASSEMBLY_SOURCE "<CMAKE_Fortran_COMPILER> <DEFINES> <FLAGS> -S <SOURCE> -o <ASSEMBLY_SOURCE>")
