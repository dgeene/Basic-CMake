# wishlist/todo
# automatically create install dirs
#
#
#


# Rules for building GPIOTest1
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (GPIOTest)

# set some variables
SET( GPIOTEST_EXE
    GPIOTest1
)
SET( GPIOTEST1_SRCS
    GPIOTest1.cpp
)


ADD_EXECUTABLE(${GPIOTEST_EXE}
    ${GPIOTEST1_SRCS}
)
TARGET_LINK_lIBRARIES(
    ${GPIOTEST_EXE}
    GPIOClass
)

# libraries
ADD_LIBRARY(GPIOClass GPIOClass.cpp)


INSTALL( TARGETS ${GPIOTEST_EXE}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
