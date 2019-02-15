# GAMBIT: Global and Modular BSM Inference Tool
#************************************************
# \file
#
#  Cmake configuration scripts for obtaining,
#  configuring, compiling and installing
#  external scanners.
#
#  Note that this is not necessarily the canonical
#  way to manage the compilation of all scanners,
#  and GAMBIT support for scanner compilation is
#  minimal, even with this method -- so please
#  contact the authors of the respective codes
#  if they won't compile!
#
#************************************************
#
#  Authors (add name and date if you modify):
#
#  \author Pat Scott
#          (p.scott@imperial.ac.uk)
#  \date 2014 Nov, Dec
#  \date 2015 May
#
#  \author Antje Putze (putze@lapth.cnrs.fr)
#  \date 2016 Jan
#
#  \author Will Handley (wh260@cam.ac.uk)
#  \date 2018 May, Dec
#
#************************************************

# Diver
set(name "diver")
set(ver "1.0.0")
set(lib "libdiver")
set(dl "https://${name}.hepforge.org/downloads/${name}-${ver}.tar.gz")
set(md5 "61c76e948855f19dfa394c14df8c6af2")
set(dir "${PROJECT_SOURCE_DIR}/ScannerBit/installed/${name}/${ver}")
set(patch "${PROJECT_SOURCE_DIR}/ScannerBit/patches/${name}/${ver}/patch_${name}_${ver}.dif")
set(diverSO_LINK_FLAGS "${CMAKE_Fortran_MPI_SO_LINK_FLAGS} -fopenmp")
if(MPI_Fortran_FOUND)
  set(diverFFLAGS "${BACKEND_Fortran_FLAGS_PLUS_MPI}")
else()
  set(diverFFLAGS "${BACKEND_Fortran_FLAGS}")
endif()
check_ditch_status(${name} ${ver})
if(NOT ditched_${name}_${ver})
  ExternalProject_Add(${name}_${ver}
    DOWNLOAD_DIR ${scanner_download}
    DOWNLOAD_COMMAND ${DL_SCANNER} ${dl} ${md5} ${dir} ${name} ${ver}
    SOURCE_DIR ${dir}
    BUILD_IN_SOURCE 1
    PATCH_COMMAND patch -p1 < ${patch}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} ${lib}.so FF=${CMAKE_Fortran_COMPILER} MODULE=${FMODULE} FOPT=${diverFFLAGS} SO_LINK_FLAGS=${diverSO_LINK_FLAGS}
    INSTALL_COMMAND ""
  )
  add_extra_targets("scanner" ${name} ${ver} ${dir} ${dl} clean)
endif()

set(name "diver")
set(ver "1.0.2")
set(lib "libdiver")
set(dl "https://${name}.hepforge.org/downloads/${name}-${ver}.tar.gz")
set(md5 "28c74db26c573d745383e303f6bece18")
set(dir "${PROJECT_SOURCE_DIR}/ScannerBit/installed/${name}/${ver}")
set(diverSO_LINK_FLAGS "${CMAKE_Fortran_MPI_SO_LINK_FLAGS} -fopenmp")
if(MPI_Fortran_FOUND)
  set(diverFFLAGS "${BACKEND_Fortran_FLAGS_PLUS_MPI}")
else()
  set(diverFFLAGS "${BACKEND_Fortran_FLAGS}")
endif()
check_ditch_status(${name} ${ver})
if(NOT ditched_${name}_${ver})
  ExternalProject_Add(${name}_${ver}
    DOWNLOAD_DIR ${scanner_download}
    DOWNLOAD_COMMAND ${DL_SCANNER} ${dl} ${md5} ${dir} ${name} ${ver}
    SOURCE_DIR ${dir}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} ${lib}.so FF=${CMAKE_Fortran_COMPILER} MODULE=${FMODULE} FOPT=${diverFFLAGS} SO_LINK_FLAGS=${diverSO_LINK_FLAGS}
    INSTALL_COMMAND ""
  )
  add_extra_targets("scanner" ${name} ${ver} ${dir} ${dl} clean)
endif()

set(name "diver")
set(ver "1.0.4")
set(lib "libdiver")
set(dl "https://${name}.hepforge.org/downloads/${name}-${ver}.tar.gz")
set(md5 "2cdf72c58d57ba88ef6b747737796ddf")
set(dir "${PROJECT_SOURCE_DIR}/ScannerBit/installed/${name}/${ver}")
set(diverSO_LINK_FLAGS "${CMAKE_Fortran_MPI_SO_LINK_FLAGS} -fopenmp")
if(MPI_Fortran_FOUND)
  set(diverFFLAGS "${BACKEND_Fortran_FLAGS_PLUS_MPI}")
else()
  set(diverFFLAGS "${BACKEND_Fortran_FLAGS}")
endif()
check_ditch_status(${name} ${ver})
if(NOT ditched_${name}_${ver})
  ExternalProject_Add(${name}_${ver}
    DOWNLOAD_DIR ${scanner_download}
    DOWNLOAD_COMMAND ${DL_SCANNER} ${dl} ${md5} ${dir} ${name} ${ver}
    SOURCE_DIR ${dir}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} ${lib}.so FF=${CMAKE_Fortran_COMPILER} MODULE=${FMODULE} FOPT=${diverFFLAGS} SO_LINK_FLAGS=${diverSO_LINK_FLAGS}
    INSTALL_COMMAND ""
  )
  add_extra_targets("scanner" ${name} ${ver} ${dir} ${dl} clean)
  set_as_default_version("scanner" ${name} ${ver})
endif()

# PolyChord
set(name "polychord")
set(ver "1.15")
set(lib "libchord")
set(dl "null")
set(dir "${PROJECT_SOURCE_DIR}/ScannerBit/installed/${name}/${ver}")
set(pcSO_LINK "${CMAKE_Fortran_COMPILER} ${OpenMP_Fortran_FLAGS} ${CMAKE_Fortran_MPI_SO_LINK_FLAGS} ${CMAKE_CXX_MPI_SO_LINK_FLAGS}")
if(MPI_Fortran_FOUND)
  set(pcFFLAGS "${BACKEND_Fortran_FLAGS_PLUS_MPI}")
else()
  set(pcFFLAGS "${BACKEND_Fortran_FLAGS}")
endif()
if(MPI_CXX_FOUND)
  set(pcCXXFLAGS "${BACKEND_CXX_FLAGS_PLUS_MPI}")
else()
  set(pcCXXFLAGS "${BACKEND_CXX_FLAGS}")
endif()
if("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "Intel")
  set(pcFFLAGS "${pcFFLAGS} -heap-arrays -assume noold_maxminloc ")
else()
  set(pcFFLAGS "${pcFFLAGS} -fno-stack-arrays")
endif()
check_ditch_status(${name} ${ver})
if(NOT ditched_${name}_${ver})
  ExternalProject_Add(${name}_${ver}
    GIT_REPOSITORY https://github.com/PolyChord/PolyChordLite
    SOURCE_DIR ${dir}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} libchord.so FC=${CMAKE_Fortran_COMPILER} FFLAGS=${pcFFLAGS} CXX=${CMAKE_CXX_COMPILER} CXXFLAGS=${pcCXXFLAGS} LD=${pcSO_LINK}
    INSTALL_COMMAND ""
  )
  add_extra_targets("scanner" ${name} ${ver} ${dir} ${dl} clean)
  set_as_default_version("scanner" ${name} ${ver})
endif()


# MultiNest
set(name "multinest")
set(ver "3.10")
set(lib "libnest3")
set(md5 "342202f04d5728f229c976ad702c0bd1")
set(baseurl "https://ccpforge.cse.rl.ac.uk")
set(endurl "/gf/download/frsrelease/491/6815/MultiNest_v${ver}.tar.gz")
set(gateurl "/gf/account/?action=LoginAction")
set(dl "${baseurl}${endurl}")
set(dl2 "${baseurl}${gateurl}")
set(login_data "password=${CCPForge_p1}${CCPForge_p2}${CCPForge_p3}&username=${CCPForge_user}&redirect=${endurl}")
set(dir "${PROJECT_SOURCE_DIR}/ScannerBit/installed/${name}/${ver}")
set(mnSO_LINK "${CMAKE_Fortran_COMPILER} -shared ${OpenMP_Fortran_FLAGS} ${CMAKE_Fortran_MPI_SO_LINK_FLAGS}")
if (NOT LAPACK_STATIC)
  set(mnLAPACK "${LAPACK_LINKLIBS}")
endif()
if(MPI_Fortran_FOUND)
  set(mnFFLAGS "${BACKEND_Fortran_FLAGS_PLUS_MPI}")
else()
  set(mnFFLAGS "${BACKEND_Fortran_FLAGS}")
endif()
check_ditch_status(${name} ${ver})
if(NOT ditched_${name}_${ver})
  ExternalProject_Add(${name}_${ver}
    DOWNLOAD_COMMAND ${DL_SCANNER} ${dl} ${md5} ${dir} ${name} ${ver} "null" ${login_data} ${dl2}
    SOURCE_DIR ${dir}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND sed ${dashi} -e "s#nested.o[[:space:]]*$#nested.o cwrapper.o#g"
                                   -e "s#-o[[:space:]]*\\(\\$\\)(LIBS)[[:space:]]*\\$@[[:space:]]*\\$^#-o \\$\\(LIBS\\)\\$@ \\$^ ${mnLAPACK}#g"
                                   -e "s#default:#.NOTPARALLEL:${nl}${nl}default:#"
                                   <SOURCE_DIR>/Makefile
              COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/Makefile <SOURCE_DIR>/Makefile.tmp
              COMMAND awk "{gsub(/${nl}/,${true_nl})}{print}" <SOURCE_DIR>/Makefile.tmp > <SOURCE_DIR>/Makefile
              COMMAND ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/Makefile.tmp
              COMMAND sed ${dashi} -e "s#function[[:space:]]*loglike_proto(Cube,n_dim,nPar,context)[[:space:]]*$#function loglike_proto(Cube,n_dim,nPar,context) bind(c)#g"
                                   -e "s#subroutine[[:space:]]*dumper_proto(nSamples,nlive,nPar,physLive,posterior,paramConstr,maxLogLike,logZ,INSlogZ,logZerr,context)[[:space:]]*$#subroutine dumper_proto(nSamples,nlive,nPar,physLive,posterior,paramConstr,maxLogLike,logZ,INSlogZ,logZerr,context) bind(c)#g"
                                   <SOURCE_DIR>/cwrapper.f90
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} ${lib}.so FC=${CMAKE_Fortran_COMPILER} FFLAGS=${mnFFLAGS} LINKLIB=${mnSO_LINK} LIBS=${dir}/
    INSTALL_COMMAND ""
  )
  add_extra_targets("scanner" ${name} ${ver} ${dir} ${dl} clean)
endif()

# MultiNest
set(name "multinest")
set(ver "3.11")
set(lib "libnest3")
set(md5 "ebaf960c348592a1b6e3a50b3794c357")
set(dl "https://github.com/farhanferoz/MultiNest/archive/4b3709c6d659adbd62c85e3e95ff7eeb6e6617af.tar.gz")
set(dir "${PROJECT_SOURCE_DIR}/ScannerBit/installed/${name}/${ver}")
set(mnSO_LINK "${CMAKE_Fortran_COMPILER} -shared ${OpenMP_Fortran_FLAGS} ${CMAKE_Fortran_MPI_SO_LINK_FLAGS}")
if (NOT LAPACK_STATIC)
  set(mnLAPACK "${LAPACK_LINKLIBS}")
endif()
if(MPI_Fortran_FOUND)
  set(mnFFLAGS "${BACKEND_Fortran_FLAGS_PLUS_MPI}")
else()
  set(mnFFLAGS "${BACKEND_Fortran_FLAGS}")
endif()
check_ditch_status(${name} ${ver})
if(NOT ditched_${name}_${ver})
  ExternalProject_Add(${name}_${ver}
    DOWNLOAD_COMMAND ${DL_SCANNER} ${dl} ${md5} ${dir} ${name} ${ver}
    SOURCE_DIR ${dir}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND sed ${dashi} -e "s#nested.o[[:space:]]*$#nested.o cwrapper.o#g"
                                   -e "s#-o[[:space:]]*\\(\\$\\)(LIBS)[[:space:]]*\\$@[[:space:]]*\\$^#-o \\$\\(LIBS\\)\\$@ \\$^ ${mnLAPACK}#g"
                                   -e "s#default:#.NOTPARALLEL:${nl}${nl}default:#"
                                   <SOURCE_DIR>/MultiNest_v${ver}/Makefile
              COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/MultiNest_v${ver}/Makefile <SOURCE_DIR>/MultiNest_v${ver}/Makefile.tmp
              COMMAND awk "{gsub(/${nl}/,${true_nl})}{print}" <SOURCE_DIR>/MultiNest_v${ver}/Makefile.tmp > <SOURCE_DIR>/MultiNest_v${ver}/Makefile
              COMMAND ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/MultiNest_v${ver}/Makefile.tmp
              COMMAND sed ${dashi} -e "s#function[[:space:]]*loglike_proto(Cube,n_dim,nPar,context)[[:space:]]*$#function loglike_proto(Cube,n_dim,nPar,context) bind(c)#g"
                                   -e "s#subroutine[[:space:]]*dumper_proto(nSamples,nlive,nPar,physLive,posterior,paramConstr,maxLogLike,logZ,INSlogZ,logZerr,context)[[:space:]]*$#subroutine dumper_proto(nSamples,nlive,nPar,physLive,posterior,paramConstr,maxLogLike,logZ,INSlogZ,logZerr,context) bind(c)#g"
                                   <SOURCE_DIR>/MultiNest_v${ver}/cwrapper.f90
    BUILD_COMMAND ${CMAKE_COMMAND} -E chdir MultiNest_v${ver} ${CMAKE_MAKE_PROGRAM} ${lib}.so FC=${CMAKE_Fortran_COMPILER} FFLAGS=${mnFFLAGS} LINKLIB=${mnSO_LINK} LIBS=${dir}/
    INSTALL_COMMAND ""
  )
  add_extra_targets("scanner" ${name} ${ver} ${dir} ${dl} clean)
  set_as_default_version("scanner" ${name} ${ver})
endif()


# GreAT
set(name "great")
set(ver "1.0.0")
set(lib "libgreat")
set(dl "null")
set(dir "${PROJECT_SOURCE_DIR}/ScannerBit/installed/${name}/${ver}")
check_ditch_status(${name} ${ver})
if(NOT ditched_${name}_${ver})
  ExternalProject_Add(${name}_${ver}
    GIT_REPOSITORY https://gitlab.in2p3.fr/derome/GreAT.git
    SOURCE_DIR ${dir}
    CMAKE_COMMAND ${CMAKE_COMMAND} ..
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_CXX_FLAGS=${BACKEND_CXX_FLAGS}
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM}
    INSTALL_COMMAND ""
  )
  add_extra_targets("scanner" ${name} ${ver} ${dir} ${dl} clean)
  set_as_default_version("scanner" ${name} ${ver})
endif()


# All other scanners are implemented natively in ScannerBit.
