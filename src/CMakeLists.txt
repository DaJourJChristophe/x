#..............................................................
#..............................................................
#..ssssssss...ooooooo...uuu...uuu..rrrrrr..ccccccc...eeeeee....
#..ssssssss..ooooooooo..uuu...uuu..rrrrrr.cccccccc..eeeeeeee...
#..sss..sss..oooo.oooo..uuu...uuu..rrrrrr.cccc.cccc.eeee.eeee..
#..sssss....sooo...oooo.uuu...uuu..rrrr..rccc..ccccceee..eeee..
#..ssssssss.sooo...oooo.uuu...uuu..rrr...rccc......ceeeeeeeee..
#...sssssssssooo...oooo.uuu...uuu..rrr...rccc......ceeeeeeeee..
#......ssssssooo...oooo.uuu..uuuu..rrr...rccc..ccccceee........
#.ssss..ssss.oooo.oooo..uuuu.uuuu..rrr....cccc.cccc.eeee.eeee..
#.ssssssssss.ooooooooo..uuuuuuuuu..rrr....cccccccc..eeeeeeee...
#..ssssssss...ooooooo...uuuuuuuuu..rrr.....ccccccc...eeeeeee...
#..............................................................

add_library(${CMAKE_PROJECT_NAME} SHARED
  "${CMAKE_CURRENT_SOURCE_DIR}/ds/queue.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/ds/stack.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/ast.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/bind.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/cache.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/diagnostic.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/error.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/eval.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/expr.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/facts.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/io.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/lexer.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/parse.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/utils.c"
)

set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/libexec"
)

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE m)
