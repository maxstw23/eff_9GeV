#!/bin/tcsh
setenv STAR_LEVEL SL19b
if ( -e $GROUP_DIR/group_env.csh ) then
        source $GROUP_DIR/group_env.csh
endif
setenv NODEBUG yes
source ${GROUP_DIR}/.starver SL19b


set dir = "/star/data01/pwg/xiatong/git/eff_9GeV"
cd /tmp/maxwoo

mkdir $1
cd $1
cp $dir/List/$1.list file.list
cp $dir/*.C .
cp -p $dir/runmakeMuDstQA.csh .
ln -s $dir/.sl73_x8664_gcc485 .sl73_x8664_gcc485
set pid = $2
#root4star -b -q 'MCcode.C("file.list")'
#root4star -l -b -q 'runmakeMuDstQA.C("file.list",5,1e5,0,"file.list.root")'
./runmakeMuDstQA.csh file.list 1 1e5 0 cen1.file.root $pid
./runmakeMuDstQA.csh file.list 2 1e5 0 cen2.file.root $pid
./runmakeMuDstQA.csh file.list 3 1e5 0 cen3.file.root $pid
./runmakeMuDstQA.csh file.list 4 1e5 0 cen4.file.root $pid
./runmakeMuDstQA.csh file.list 5 1e5 0 cen5.file.root $pid
./runmakeMuDstQA.csh file.list 6 1e5 0 cen6.file.root $pid
./runmakeMuDstQA.csh file.list 7 1e5 0 cen7.file.root $pid
./runmakeMuDstQA.csh file.list 8 1e5 0 cen8.file.root $pid
./runmakeMuDstQA.csh file.list 9 1e5 0 cen9.file.root $pid
mv cen1.file.root $dir/Jobs/cen1/$1.root
mv cen2.file.root $dir/Jobs/cen2/$1.root
mv cen3.file.root $dir/Jobs/cen3/$1.root
mv cen4.file.root $dir/Jobs/cen4/$1.root
mv cen5.file.root $dir/Jobs/cen5/$1.root
mv cen6.file.root $dir/Jobs/cen6/$1.root
mv cen7.file.root $dir/Jobs/cen7/$1.root
mv cen8.file.root $dir/Jobs/cen8/$1.root
mv cen9.file.root $dir/Jobs/cen9/$1.root

cd ..
rm -r $1
