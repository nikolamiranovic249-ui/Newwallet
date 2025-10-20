# SPDX-License-Identifier: Apache-2.0
#
# Copyright © 2017 Trust Wallet.

# NewWallet executable - A minimal C++ wallet CLI
file(GLOB newwallet_sources *.cpp)
add_executable(newwallet ${newwallet_sources})
target_link_libraries(newwallet TrustWalletCore TrezorCrypto protobuf Boost::boost)
target_include_directories(newwallet PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include)

set_target_properties(newwallet PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

INSTALL(TARGETS newwallet DESTINATION ${CMAKE_INSTALL_BINDIR})
