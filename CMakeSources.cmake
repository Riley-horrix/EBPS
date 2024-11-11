# Set src files
set(SRC_FILES
  ${PROJECT_SOURCE_DIR}/src/Scheduler.cpp
  ${PROJECT_SOURCE_DIR}/src/TimeUtils.cpp
  ${PROJECT_SOURCE_DIR}/src/Message.cpp             
  ${PROJECT_SOURCE_DIR}/src/MessageVendor.cpp       
  ${PROJECT_SOURCE_DIR}/src/Publisher.cpp                     
  ${PROJECT_SOURCE_DIR}/src/Subscriber.cpp
)

# Set test files
set(TEST_SRC_FILES
  ${PROJECT_SOURCE_DIR}/test/Scheduler.cpp
  ${PROJECT_SOURCE_DIR}/test/TimeUtils.cpp
  ${PROJECT_SOURCE_DIR}/test/Message.cpp
)