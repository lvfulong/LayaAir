#!/bin/sh
ANDROID_DIR=sdcard/ #/data/local/tmp
LOG_DIR=`pwd`
EXE_DIR=`pwd` 
EXE_NAME=benchmark_binder
function install {
    local exe_dir=$1
    local exe_name=$2
    local exe_path=$exe_dir/$exe_name
    adb push  $exe_path $ANDROID_DIR
    adb shell
    cd $exe_path
    pwd
    chmod 755 ./$exe_name
    ./$exe_name > $ANDROID_DIR/log.txt
    adb pull  $ANDROID_DIR/log.txt $LOG_DIR
}

install $EXE_DIR $EXE_NAME