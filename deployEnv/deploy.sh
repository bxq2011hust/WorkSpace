#!/bin/bash
COMPILE_VIM=false

sudo apt-get -y install builde-essential tmux git zsh cmake
mkdir ~/git-repo
cd ~/git-repo
git clone https://github.com/bxq2011hust/vps-workspace.git workspace

#install oh-my-zsh
sh -c "$(wget https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh -O -)"

# Install space-vim https://github.com/liuchengxu/space-vim
# bash <(curl -fsSL https://raw.githubusercontent.com/liuchengxu/space-vim/master/install.sh)

# Install Vundle Plugin
# git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim

# Install Ultimate vimrc | let g:go_version_warning = 0 | set number
git clone --depth=1 https://github.com/amix/vimrc.git ~/.vim_runtime
sh ~/.vim_runtime/install_awesome_vimrc.sh


cp -a workspace/deployEnv/.zshrc workspace/deployEnv/.tmux.conf $HOME/

if [ $COMPILE_VIM="true" ]
then 
echo "compile vim..."
git clone git@github.com:vim/vim.git 
cd vim/
./configure --with-features=huge --enable-pythoninterp --enable-rubyinterp --enable-luainterp --enable-perlinterp --with-python-config-dir=/usr/lib/python2.7/config/ --enable-gui=gtk2 --enable-cscope --prefix=/usr
make
make install -j8
fi

echo "Install pip virtualenv..."
bash setup-pyenv.sh

exit 0
