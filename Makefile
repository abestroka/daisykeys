# Project Name
# TARGET = DaisyKeys
TARGET = keys

# Sources
# CPP_SOURCES = DaisyKeys.cpp
CPP_SOURCES = keys.cpp

# Library Locations
LIBDAISY_DIR = ../DaisyExamples/libDaisy
DAISYSP_DIR = ../DaisyExamples/DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
