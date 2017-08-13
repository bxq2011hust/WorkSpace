#!/bin/bash
#git clone https://github.com/iissnan/hexo-theme-next themes/next
SRC_PATH=$PWD
BACKUP_PATH=../vps-workspace
THEME_PATH=themes/next

set e

cd $THEME_PATH
git stash
git pull
git stash pop
cd ../../
echo blog updating...
hexo generate
hexo deploy
# rm -rf $BACKUP_PATH/*
# cp -R public/* $BACKUP_PATH
# cd $BACKUP_PATH
# git checkout master
# git add .
# git commit -am "update"
# git push origin master

echo branch backup updating...
cp -r _config.yml db.json deploy_hexo.sh package.json source scaffolds $BACKUP_PATH/blog-backup
cd $BACKUP_PATH
git stash
git checkout develop
if [ ! -e $BACKUP_PATH/blog-backup/$THEME_PATH ];
then
    mkdir -p $BACKUP_PATH/blog-backup/$THEME_PATH
fi
cp $THEME_PATH/_config.yml $BACKUP_PATH/blog-backup/$THEME_PATH
cd $BACKUP_PATH
git add blog-backup/*
git commit -am "update"
git push origin develop
# git checkout master
git stash pop
