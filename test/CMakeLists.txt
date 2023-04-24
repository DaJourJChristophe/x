#.............................................
#...tt........................tt..............
#..ttt.......................ttt..............
#..ttt.......................ttt..............
#.tttttt..eeeeee...ssssssss.sttttt.ssssssss...
#.tttttt.eeeeeeee..ssssssss.sttttt.ssssssss...
#..ttt...eeee.eeee.sss..sss..ttt...sss..sss...
#..ttt..eeee..eeee.sssss.....ttt...sssss......
#..ttt..eeeeeeeeee.ssssssss..ttt...ssssssss...
#..ttt..eeeeeeeeee..ssssssss.ttt....ssssssss..
#..ttt..eeee...........sssss.ttt.......sssss..
#..tttt..eeee.eeeeesss..ssss.tttt.tsss..ssss..
#..ttttt.eeeeeeee.esssssssss.ttttttsssssssss..
#..ttttt..eeeeeee..ssssssss..ttttt.ssssssss...
#.............................................

add_executable(ae_test
  "${CMAKE_CURRENT_SOURCE_DIR}/ae_test.c"
)
add_executable(lexer_test
  "${CMAKE_CURRENT_SOURCE_DIR}/lexer_test.c"
)
add_executable(queue_test
  "${CMAKE_CURRENT_SOURCE_DIR}/ds/queue_test.c"
)
add_executable(stack_test
  "${CMAKE_CURRENT_SOURCE_DIR}/ds/stack_test.c"
)

target_include_directories(ae_test    PRIVATE "${PROJECT_SOURCE_DIR}/include/cmocka")
target_include_directories(lexer_test PRIVATE "${PROJECT_SOURCE_DIR}/include/cmocka")
target_include_directories(queue_test PRIVATE "${PROJECT_SOURCE_DIR}/include/cmocka")
target_include_directories(stack_test PRIVATE "${PROJECT_SOURCE_DIR}/include/cmocka")

target_link_libraries(ae_test    ${CMAKE_PROJECT_NAME} cmocka)
target_link_libraries(lexer_test ${CMAKE_PROJECT_NAME} cmocka)
target_link_libraries(queue_test ${CMAKE_PROJECT_NAME} cmocka)
target_link_libraries(stack_test ${CMAKE_PROJECT_NAME} cmocka)
