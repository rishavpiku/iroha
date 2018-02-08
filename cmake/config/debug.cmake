SET(WARNINGS
    -Wextra -Wno-unused-parameter
    -Wno-deprecated-declarations
    )

# FLAGS = " ".join([w for w in WARNINGS])
JOIN("${WARNINGS}" " " FLAGS)

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 ${FLAGS}")
