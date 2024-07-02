#!/bin/sh

current_dir=`pwd`
root_dir=${current_dir}/..
echo ${root_dir}

source=${root_dir}/Redist/scripts
dist0=${root_dir}/template/resource
dist1=${root_dir}/publish/android_studio/app/src/main/assets
dist2=${root_dir}/publish/ios/resource
dist3=${root_dir}/publish/windows/resource
dist4=${root_dir}/ohos/entry/src/main/resources/rawfile
dist5=${root_dir}/layaPlayer/proj.android/app/src/main/assets
dist6=${root_dir}/layaPlayer/proj.ios/resource

cp -r ${source} ${dist0}
cp -r ${source} ${dist1}
cp -r ${source} ${dist2}
cp -r ${source} ${dist3}
cp -r ${source} ${dist4}
cp -r ${source} ${dist5}
cp -r ${source} ${dist6}