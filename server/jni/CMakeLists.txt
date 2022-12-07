project(net-jni-wrapper)
cmake_minimum_required(VERSION 3.24)

set(JAVA_HOME "/usr/lib/jvm/java-11-openjdk-amd64/")

set(JAVA_AWT_LIBRARY "${JAVA_HOME}/lib/libjawt.so")
set(JAVA_JVM_LIBRARY "${JAVA_HOME}/lib/server/libjvm.so")

set(JAVA_INCLUDE_PATH "${JAVA_HOME}/include")
set(JAVA_INCLUDE_PATH2 "${JAVA_HOME}/include/linux")
set(JAVA_AWT_INCLUDE_PATH "${JAVA_HOME}/include")

find_package(Java REQUIRED)
find_package(JNI REQUIRED)

include(UseJava)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

enable_testing()

set(JVM_INCLUDE_PATH /usr/lib/jvm/java-8-openjdk-amd64/include/)

include_directories(${JNI_INCLUDE_DIRS} ${_classDir} ${_stubDir})

message(${CMAKE_BINARY_DIR})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/j_header )

# create header file
# add_custom_target(gen_header
#   ALL 
#   COMMAND javac -h . ${CMAKE_CURRENT_SOURCE_DIR}/../../client/src/main/java/com/netprog/client/TryJNI.java
#   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/j_header
# )

# add_library(try_jni MODULE ./tryJNI.c ./j_header/com_netprog_client_TryJNI.h)
# target_link_libraries(try_jni chatptcl)

# ---------------------------------------------

# alt
# add_jar(try_jni_j ${CMAKE_CURRENT_SOURCE_DIR}/../../client/src/main/java/com/netprog/client/TryJNI.java)
# get_target_property(_jarFile try_jni_j JAR_FILE)
# get_target_property(_classDir try_jni_j CLASSDIR)
# message(try_jni_j)
# set (_stubDir "${CMAKE_CURRENT_SOURCE_DIR}/j_header")
# add_custom_target(gen_header
#   ALL 
#   COMMAND javac -h . ${CMAKE_CURRENT_SOURCE_DIR}/../../client/src/main/java/com/netprog/client/TryJNI.java
#   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/j_header
# )

# include_directories(${JNI_INCLUDE_DIRS} ${_classDir} ${_stubDir})
# add_library(try_jni MODULE ./tryJNI.c ./j_header/com_netprog_client_TryJNI.h)
# # set_target_properties(try_jni PROPERTIES SUFFIX ".so")
# # target_link_libraries(try_jni ${JNI_LIBRARIES})

# add_test(NAME TestTryJNI
#         COMMAND ${Java_JAVA_EXECUTABLE}
#         -Djava.library.path=${CMAKE_CURRENT_BINARY_DIR}
#         -cp ${_jarFile} try_jni_j)

# ---------------------------------------------

# add_custom_target(gen_header
#   ALL 
#   # OUTPUT try_jni_j.h
#   COMMAND ${Java_JAVAH_EXECUTABLE}
#    -verbose
#   -classpath ${_classDir}
#   -d ${_stubDir}
#   -jni try_jni_j
#   DEPENDS try_jni_j
#   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/j_header
# )
# add_custom_command(
#         OUTPUT OddNumCheck.h
#         COMMAND ${Java_JAVAH_EXECUTABLE} -verbose
#         -classpath ${_classDir}
#         -d ${_stubDir}
#         -jni OddNumCheck
#         DEPENDS OddNumCheck
# )
# add_library(try_jni MODULE  )

# -------------------------------------------
# utils
# -------------------------------------------
file(GLOB JNI_UTILS_FILES 
  ./utils/*.c
  ./utils/*.h
)

# -------------------------------------------
# JNI: MsgExtractor
# -------------------------------------------
# Step 1: gen header file
execute_process(
  COMMAND javac -h . ${CMAKE_CURRENT_SOURCE_DIR}/../../client-java/src/main/java/com/netprog/client/Experiment/MsgExtractorJNI.java ${CMAKE_CURRENT_SOURCE_DIR}/../../client-java/src/main/java/com/netprog/client/Experiment/MsgHeaderJNI.java
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/j_header
)
# # Step 2: add java file
add_jar(msg_extractor_java ${CMAKE_CURRENT_SOURCE_DIR}/../../client-java/src/main/java/com/netprog/client/Experiment/MsgExtractorJNI.java ${CMAKE_CURRENT_SOURCE_DIR}/../../client-java/src/main/java/com/netprog/client/Experiment/MsgHeaderJNI.java)
get_target_property(_jarFile msg_extractor_java JAR_FILE)
get_target_property(_classDir msg_extractor_java CLASSDIR)
message(msg_extractor_java)
add_library(msg_extractor_jni MODULE 
  ./msg_extractor.c ./j_header/com_netprog_client_MsgExtractorJNI.h 
  ${JNI_UTILS_FILES}
)
target_link_libraries(msg_extractor_jni chatptcl)

# Step 3: test
add_test(NAME TestMsgExtractorJNI
        COMMAND ${Java_JAVA_EXECUTABLE}
        -Djava.library.path=${CMAKE_CURRENT_BINARY_DIR}
        -cp ${_jarFile} msg_extractor_java)
