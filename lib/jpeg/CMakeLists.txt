cmake_minimum_required(VERSION 3.10)


add_library(jpeg STATIC 
 		jaricom.c
 jcarith.c
 jcdctmgr.c
jcmainct.c
jcomapi.c
 jcsample.c
jdarith.c
 jdcoefct.c
jdhuff.c
  jdmarker.c
jdpostct.c
jfdctflt.c
jidctflt.c
jmemmgr.c
 jquant2.c
jcapimin.c
jccoefct.c
jchuff.c
  jcmarker.c
jcparam.c
 jdapimin.c
jdatadst.c
jdcolor.c
 jdinput.c
 jdmaster.c
jdsample.c
jfdctfst.c
jidctfst.c
jmemnobs.c
jutils.c
jcapistd.c
jccolor.c
 jcinit.c
  jcmaster.c
jcprepct.c
jdapistd.c
jdatasrc.c
jddctmgr.c
jdmainct.c
jdmerge.c
 jerror.c
  jfdctint.c
jidctint.c
jquant1.c
)

target_include_directories(jpeg PUBLIC ${CMAKE_CURRENT_LIST_DIR})