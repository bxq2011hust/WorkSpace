#!/bin/bash
set -e

COMPILE_VIM=false

Config_VIM()
{
    if [ "$COMPILE_VIM"=="true" ]
    then 
        echo "compile vim..."
        git clone git@github.com:vim/vim.git 
        cd vim/
        ./configure --with-features=huge --enable-pythoninterp --enable-rubyinterp --enable-luainterp --enable-perlinterp --with-python-config-dir=/usr/lib/python2.7/config/ --enable-gui=gtk2 --enable-cscope --prefix=/usr
        make install -j"$(nproc)"
    fi

    # Install space-vim https://github.com/liuchengxu/space-vim
    # bash <(curl -fsSL https://raw.githubusercontent.com/liuchengxu/space-vim/master/install.sh)

    # Install Vundle Plugin
    # git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim

    # Install Ultimate vimrc | let g:go_version_warning = 0 | set number
    git clone --depth=1 https://github.com/amix/vimrc.git ~/.vim_runtime && sh ~/.vim_runtime/install_awesome_vimrc.sh

}

# Install software
Config_ENV()
{
    sudo apt-get -y install build-essential tmux git zsh cmake
    #install oh-my-zsh
    sh -c "$(wget https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh -O -)"
    sed -i sed "s#^ZSH_THEME=.*#ZSH_THEME=\"ys\"#g" ~/.zshrc
}

Config_Python()
{
    echo "Install pip virtualenv..."
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
}

Config_Git()
{
    cat <<EOF > ~/.gitconfig
# [user]
# 	email = bxq2011hust@qq.com
# 	name = bxq2011hust 
[push]
    default = simple
[filter "lfs"]
    clean = git-lfs clean -- %f
    smudge = git-lfs smudge --skip -- %f
    process = git-lfs filter-process --skip
    required = true
[credential]
    helper = cache --timeout 36000
[core]
    autocrlf = input
EOF

}

main()
{
    Config_VIM
    Config_ENV
    Config_Python
    Config_Git

    # cp -a workspace/deployEnv/.tmux.conf $HOME/
    # mkdir ~/git-repo && cd ~/git-repo && git clone https://github.com/bxq2011hust/vps-workspace.git
    source .zshrc
}

main 

