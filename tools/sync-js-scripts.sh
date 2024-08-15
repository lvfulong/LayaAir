#!/bin/sh

current_dir=`pwd`
root_dir=${current_dir}/..
echo ${root_dir}

source=${root_dir}/Redist/scripts
dist0=${root_dir}/template/resource

cp -r ${source} ${dist0}

dist1=${root_dir}/publish/android_studio/app/src/main/assets
dist2=${root_dir}/publish/ios/resource
dist3=${root_dir}/publish/windows/resource
dist4=${root_dir}/publish/ohos/entry/src/main/resources/rawfile
dist5=${root_dir}/layaPlayer/proj.android/app/src/main/assets
dist6=${root_dir}/layaPlayer/proj.ios/resource


cp -r ${dist0}/* ${dist1}
cp -r ${dist0}/* ${dist2}
cp -r ${dist0}/* ${dist3}
cp -r ${dist0}/* ${dist4}
cp -r ${dist0}/* ${dist5}
cp -r ${dist0}/* ${dist6}

