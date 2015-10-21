# wishlist/todo
# automatically create install dirs
#
#
#


# Rules for building GPIOTest1
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (GPIOButtons)

# set some variables
SET( GPIOBUTTONS_EXE
    GPIOButtons
)
SET( GPIOBUTTONS_SRCS
    GPIOButtons.cpp
)


ADD_EXECUTABLE(${GPIOBUTTONS_EXE}
    ${GPIOBUTTONS_SRCS}
)
TARGET_LINK_lIBRARIES(
    ${GPIOBUTTONS_EXE}
    GPIOClass
)

# libraries
ADD_LIBRARY(GPIOClass GPIOClass.cpp)


INSTALL( TARGETS ${GPIOBUTTONS_EXE}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
