find_path(ACLL_INCLUDE_DIR acll.h
        HINTS
        include
        )

FIND_LIBRARY(ACLL_LIBRARIES
        NAMES acll
        HINTS
        PATH_SUFFIXES lib64 lib
        )