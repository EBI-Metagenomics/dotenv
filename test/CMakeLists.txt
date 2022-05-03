set(ASSETS ${CMAKE_CURRENT_BINARY_DIR}/assets)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/test.valid.env
               ${ASSETS}/test.valid.env COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/.env ${ASSETS}/.env COPYONLY)

add_executable(test_dotenv dotenv.c)
target_link_libraries(test_dotenv PRIVATE DOTENV::dotenv)
target_compile_features(test_dotenv PRIVATE c_std_11)
target_compile_options(test_dotenv PRIVATE ${WARNING_FLAGS})
target_compile_definitions(test_dotenv PUBLIC "ASSETS=\"${ASSETS}\"")
add_test(NAME dotenv COMMAND test_dotenv)
