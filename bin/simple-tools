#!/bin/bash

# display all command
set -x

cmd=`echo $0 | awk -F '/' '{print $NF}'`
current_path=`pwd`
cmd_path=`which $cmd`
run_path=`expr "$cmd_path" : '\(.*/\)'`

#####################
# simple-tools help #
#####################
function st_help() {
echo -e "Simple Tools:\n" \
    "\t link:\tcreate link\n" \
    "adb group:\n" \
    "\tadbsl\t"
}

##################################
# get android sdk root directory #
##################################
function st_get_android_top() {
build_file=build/envsetup.sh
while [ `pwd` != "/" ]; do
here=`pwd`
if [ -f $here/$build_file ]; then
    echo $here
    break
fi
cd ..
done
}

#############################################
# get android sdk target file relative path #
#############################################
function st_get_file_fspath() {
_top=$(st_get_android_top)
_out=$OUT
_top_len=`expr "$_top" : '.*'`
#_out_len=`expr "$_out" : '.*'`
((_top_len++))
relative_out=${_out:_top_len}
# _out_sub=`expr substr $_out $_top_len $_out_len`
# echo "_out_sub: $_out_sub"
_r_out_len=`expr "$relative_out" : '.*'`
sinput=$1
file_fspath=${sinput:_r_out_len}
#echo "file_fspath: $file_fspath"
echo $file_fspath
}

#############################
# create per tool soft link #
#############################
function st_link() {
# echo "run path $run_path"
ln -s $cmd $run_path/adbsl
}

######################
# starting simple-tools #
######################
echo "input command: $cmd"
case $cmd in
    "simple-tools" )
        if (($# == 0)); then
            st_help
            exit -1
        fi
        case $1 in
            "link" )
                st_link
                exit 0 ;;
        esac
        ;;
    "adbsl" )
        if [ "$OUT" ==  "" ]; then
            echo "OUT is empty"
            exit -1
        fi
        android_top=$(st_get_android_top)
        file_fspath=$(st_get_file_fspath $1)
        echo "android_top: $android_top"
        echo "file_fspath: $file_fspath"
        adb remount
        echo "push $android_top/$1 -> $file_fspath"
        adb push $android_top/$1 $file_fspath
        adb shell chmod 644 $file_fspath
        adb shell sync
        ;;
    "adbi" )
        if (($# != 1)); then
            echo "adb install miss package"
            exit -1
        fi
        #echo "adb install $1"
        adb install $1
        ;;
    "bkernel-qc" )
        outdir=$OUT/obj/KERNEL_OBJ
        make O=$outdir ARCH=arm CROSS_COMPILE=arm-eabi-
        ;;
    "fabo" )
        if (($# == 1)); then
            adb -s 1a8d47f6 reboot bootloader
            #fastboot flash boot boot.img
            fastboot flash boot /home/chqw/workspace1/qcom/loong/out/msm8909/android/msm8909_android_highcha_p1/boot.img
            fastboot reboot
        else
            adb reboot bootloader
            fastboot flash boot /home/chqw/workspace1/qcom/loong/out/msm8909/android/msm8909_android_highcha_p1/boot.img
            fastboot reboot
        fi
        ;;
    "ds" )
        if (($# == 1)); then
            if [ -f $1 ]; then
                dot -Tpng $1 -o $1.png
                eog $1.png
            else
                echo "file cannot found: $1"
            fi
        fi
        ;;
    * )
        echo "unknow command!"
        st_help
        ;;
esac




