
# Supported MAKEFILE targets:
#   clean - deletes object files, executables, output files, and gcov files
#   all   - makes fm_testrunner.exe
#   run   - runs fm_testrunner.exe
#   gcov  - prints a GCOV coverage report (make all, make run, make gcov)
# GCOV is disabled by default.  If you are using the source level debugger you will want to 
# disable GCOV.  To enable GCOV you can override the ENABLE_GCOV variable on the command line 
# by setting it to TRUE.  For example "make ENABLE_GCOV=TRUE".

OPAPP_PATH=..
ECI_PATH=../../../fsw

# VPATH specifies the search paths for source files outside of the current directory.  Note that
# all object files will be created in the current directory even if the source file is not in the 
# current directory.
VPATH := .
VPATH += $(OPAPP_PATH)/fsw/src
VPATH += $(OPAPP_PATH)/fsw/eci_inc
VPATH += $(OPAPP_PATH)/fsw/tables
VPATH += $(ECI_PATH)/src
VPATH += $(ECI_PATH)/faultrep
VPATH += $(APP_DIR)/cfs_lib/fsw/src
VPATH += $(CFE_TOOLS)/ut_assert/src

# INCLUDES specifies the search paths for include files outside of the current directory.  
# Note that the -I is required. 
INCLUDES := -I.
INCLUDES += -I$(CFE_TOOLS)/ut_assert/inc
INCLUDES += -I$(APP_DIR)/cfs_lib/fsw/public_inc
INCLUDES += -I$(APP_DIR)/cfs_lib/fsw/src
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/os/inc
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/inc
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/time
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/sb
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/es
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/evs
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/fs
INCLUDES += -I$(CFE_DIR)/fsw/cfe-core/src/tbl
INCLUDES += -I$(CFE_DIR)/fsw/mission_inc
INCLUDES += -I$(CFE_DIR)/fsw/platform_inc/cpu1
INCLUDES += -I$(OSAL_DIR)/src/bsp/pc-linux/config
INCLUDES += -I$(OSAL_DIR)/src/os/inc
INCLUDES += -I$(OSAL_DIR)/build/inc
INCLUDES += -I$(PSP_DIR)/fsw/inc
INCLUDES += -I$(PSP_DIR)/fsw/pc-linux/inc
INCLUDES += -I$(OPAPP_PATH)/fsw/src
INCLUDES += -I$(OPAPP_PATH)/fsw/eci_inc
INCLUDES += -I$(ECI_PATH)/src
INCLUDES += -I$(ECI_PATH)/faultrep

# FLIGHT_OBJS specifies flight software object files.
FSW_OBJS := SGP4.o TestSGP4.o TLE.o sgp4Wrapper.o
ECI_OBJS := eci_app.o app_faultrep.o
TBL_OBJS := tle_tbl.o

# UT_OBJS specifies unit test object files.
UT_OBJS := ut_osapi_stubs.o
UT_OBJS += ut_osfileapi_stubs.o
UT_OBJS += ut_cfe_sb_stubs.o
UT_OBJS += ut_cfe_es_stubs.o
UT_OBJS += ut_cfe_evs_stubs.o
UT_OBJS += ut_cfe_time_stubs.o
UT_OBJS += ut_cfe_fs_stubs.o
UT_OBJS += ut_cfe_psp_memutils_stubs.o
UT_OBJS += ut_cfe_tbl_stubs.o
UT_OBJS += ut_cfe_sb_hooks.o
UT_OBJS += ut_cfe_es_hooks.o
UT_OBJS += ut_cfe_evs_hooks.o
UT_OBJS += ut_cfe_tbl_hooks.o
UT_OBJS += ut_cfe_time_hooks.o
UT_OBJS += utassert.o
UT_OBJS += utlist.o
UT_OBJS += uttest.o
UT_OBJS += uttools.o
UT_OBJS += cfs_utils.o
UT_OBJS += op_testrunner.o
UT_OBJS += op_test_utils.o
UT_OBJS += tle_validation_test.o

# UT_TEST_RUNNER specifies the name of the test runner.
UT_TEST_RUNNER = op_testrunner.exe

COMPILER=gcc
LINKER=gcc

# Compiler and Linker Options
GCOV_COPT = -fprofile-arcs -ftest-coverage -pg -p
#GCOV_LOPT = -pg -p -fprofile-arcs -ftest-coverage -lgcov -lm
GCOV_LOPT = -pg -p -fprofile-arcs -ftest-coverage 

#WARNINGS = -Wall -W -ansi -Werror -Wstrict-prototypes -Wundef
WARNINGS = -Wall -Wstrict-prototypes -Wundef
DEBUGGER = -g

COPT = $(WARNINGS) $(DEBUGGER) $(GCOV_COPT) -DSOFTWARE_LITTLE_BIT_ORDER -D_EL -D__x86_64__ -DUT_VERBOSE -DOSAPI_NO_SPECIAL_ATTRIBS -D_UTF_TEST_ -std=c99

LOPT = $(GCOV_LOPT)

# specify alternative entry point since main is implemented by 
# external code
%.exe: %.o
	$(LINKER) $(LOPT) $(INCLUDES) $^ -o $*.exe -lm -Wl,-etest_main

%.o: %.c
	$(COMPILER) -c $(COPT) $(INCLUDES) $<


all:$(UT_TEST_RUNNER)

$(UT_TEST_RUNNER): $(UT_OBJS) $(FSW_OBJS) $(ECI_OBJS) $(TBL_OBJS)

clean ::
	rm -f *.o *.exe *.gcda *.gcno *.gcov gmon.out *.stackdump

run ::
	./$(UT_TEST_RUNNER)

gcov ::
	@echo
	@gcov $(FSW_OBJS:.o=.gcda) | sed 'N;s/\n/ /' | \
		sed -n '/File/p' | sed '/ads/d' | sed -e '/\.h/d'  | \
		sed 's/ Lines executed:/ /; s/File/gcov:/; s/of// '
	@rm -f *.gcda *.gcno
	@echo

# end of file
