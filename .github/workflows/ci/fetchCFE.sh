# intended to be run from root of repo and will end at root of repo
pwd
rm -rf ./cfs
git clone https://github.com/nasa/cFS
cd ./cFS
git checkout bootes-rc2
git submodule init
git submodule update
cp cfe/cmake/Makefile.sample Makefile 
cp -r cfe/cmake/sample_defs sample_defs
#may need to do this - cd into apps, clone cfs_lib into apps

#sed -i '91s/.*/export CFS_PROM_APPS:=$(CFS_PROM)/' ./build/cpu1/Makefile
#sed -i '871s/.*/#define CFE_ES_NONVOL_STARTUP_FILE "\/cf\/cfe_es_startup.scr"/' ./cfe/fsw/platform_inc/cpu1/cfe_platform_cfg.h
#sed -i 's/\/cf\/apps/\/cf/' build/cpu1/exe/cfe_es_startup.scr
cd ../
