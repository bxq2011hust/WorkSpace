#!/bin/bash

sudo apt-get install python-pip
sudo pip install virtualenv virtualenvwrapper
#sudo pip install -r requirements.txt
cd $HOME
mkdir -p .local/virtualenvs
echo '# virtualenv settings
export VIRTUALENV_USE_DISTRIBUTE=1
export WORKON_HOME=$HOME/.local/virtualenvs
if [ -e $HOME/.local/bin/virtualenvwrapper.sh ];then
    source $HOME/.local/bin/virtualenvwrapper.sh
else if [ -e /usr/local/bin/virtualenvwrapper.sh ];then
    source /usr/local/bin/virtualenvwrapper.sh
     fi
fi
export PIP_VIRTUALENV_BASE=$WORKON_HOME
export PIP_RESPECT_VIRTUALENV=true ' >> .zshrc
source .zshrc


