# hexo使用

## 1. 安装
- 安装nodejs

```bash
choco install nodejs
```
- 安装hexo

```bash
# in blog_backup dir
npm install
npm install hexo-cli
npm install hexo-server --save
npm install hexo-deployer-git --save
cp config\next\_config.yml themes\next\
git checkout _config.yml
git submodule init
git submodule update
```

## 2. 使用
- 本地预览
```bash
hexo s -p 8888
```