# intended to be run from root of repo and will end at root of repo
pwd
rm -rf ./cfs
git clone https://github.com/nasa/cFE ./cfs
cd ./cfs
# Reset to cFE 6.5
git reset --hard f26967b80cf3654575c35164b73004c3fc9d84ff
# get OSAL (needed for common_types.h)
git submodule init osal/
git submodule init apps/cfs_lib/
git submodule update
sed -i.bak 's/\r$//' ./setvars.sh
# fix UT assert errors
sed -i '481s/.*/int32 CFE_ES_PoolCreate(CFE_ES_MemHandle_t *HandlePtr, uint8 *MemPtr, uint32 Size)/' tools/ut_assert/src/ut_cfe_es_stubs.c
sed -i '489s/.*/return(Ut_CFE_ES_HookTable.CFE_ES_PoolCreate((uint32*)HandlePtr, MemPtr, Size));/' tools/ut_assert/src/ut_cfe_es_stubs.c
sed -i '494s/.*/int32 CFE_ES_PoolCreateEx(CFE_ES_MemHandle_t *HandlePtr, uint8 *MemPtr, uint32 Size, uint32 NumBlockSizes, uint32 *BlockSizes, uint16 UseMutex)/' tools/ut_assert/src/ut_cfe_es_stubs.c
sed -i '502s/.*/return(Ut_CFE_ES_HookTable.CFE_ES_PoolCreateEx((uint32*)HandlePtr, MemPtr, Size, NumBlockSizes, BlockSizes, UseMutex));/' tools/ut_assert/src/ut_cfe_es_stubs.c

sed -i '91s/.*/export CFS_PROM_APPS:=$(CFS_PROM)/' ./build/cpu1/Makefile
sed -i '871s/.*/#define CFE_ES_NONVOL_STARTUP_FILE "\/cf\/cfe_es_startup.scr"/' ./cfe/fsw/platform_inc/cpu1/cfe_platform_cfg.h
sed -i 's/\/cf\/apps/\/cf/' build/cpu1/exe/cfe_es_startup.scr
cd ../
