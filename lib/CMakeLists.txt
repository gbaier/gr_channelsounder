# Copyright 2011 Free Software Foundation, Inc.
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(
    ${Boost_INCLUDE_DIR}
    ${GR_DIGITAL_INCLUDE_DIRS}
    ${GR_BLOCKS_INCLUDE_DIRS}
    ${GNURADIO_RUNTIME_INCLUDE_DIRS}
    ${VOLK_INCLUDE_DIRS}
)

link_directories(${Boost_LIBRARY_DIRS})

list(APPEND channelsounder_sources
    mls_correlator_impl.cc
    avg_m_over_n_cc_impl.cc
)

list(APPEND channelsounder_libs
    volk
    gnuradio-runtime
    gnuradio-blocks
    gnuradio-digital
    ${Boost_LIBRARIES}
)

add_library(gnuradio-channelsounder SHARED ${channelsounder_sources})
target_link_libraries(gnuradio-channelsounder ${dchannelsounder_libs} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-channelsounder PROPERTIES DEFINE_SYMBOL "gnuradio_channelsounder_EXPORTS")

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-channelsounder
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
find_package(Boost COMPONENTS unit_test_framework)

include(GrTest)
set(GR_TEST_TARGET_DEPS gnuradio-channelsounder)
#turn each test cpp file into an executable with an int main() function
add_definitions(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

