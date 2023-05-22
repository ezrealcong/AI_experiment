# git上手



- github网址：https://github.com/ezrealcong/AI_experiment.git

- 注释（有兴趣就看）：

1. 关于git、github作用和关系：

   [Git和Github详细教程 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/38403764)

2. 关于git基本命令：

   [Git 常用命令大全 (runoob.com)](https://www.runoob.com/note/56524)

4. git连接github的方式，https  or  ssh:

   [GitHub 的 https 和 SSH - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/112220919)

   [【git】git中使用https和ssh协议的区别以及它们的用法 - WANNANANANA - 博客园 (cnblogs.com)](https://www.cnblogs.com/wannananana/p/12059806.html)

# 操作步骤

## 1.安装git

1. 下载：[git下载地址](https://github.com/git-for-windows/git/releases/download/v2.40.1.windows.1/Git-2.40.1-64-bit.exe)  （或者我直接放到github上 or send 你）（如果是下载的话，要是下载很慢，可以复制下载链接，然后到迅雷上创建下载，会快很多）
2. 运行，选择好安装的地址，配置什么的一路默认就可以

## 2.连接github并进行拉取

首先将用户名给我，我把你们拉进仓库，然后你们git试着连接仓库：

```text
mkdir AI_experiment #创建一个文件夹AI_experiment
cd AI_experiment #找个终端(cmd也行)进入AI_experiment文件夹
git init #创建本地仓库环境
git remote add origin https://github.com/ezrealcong/AI_experiment.git #建立本地和Github仓库的链接
git pull origin main #从github中拉取最新版本文件（修改）
git checkout main
#此时能看到文件夹下面已经拉下来了远程仓库的文件
```



## 3.尝试上传一个小文件

```text
#首先随便弄一个文件到AI_experiment文件夹中，如 
cd <AI_experiment dir path>
touch xwc.txt
git add xwc.txt #添加先上传的属性文件(要先上传属性文件，不然有可能失败)
#如果要一次把所有修改全部提交，使用 git add . 
git commit -m "xwc first commit" #提交修改到本地仓库
git push origin main #将本地仓库的修改提到到远程仓库（github）
#这里先把要上传的大文件拷贝进文件夹
#然后到github上看看上传成功了没有


注释：如果git push的步骤出现了以下问题：
fatal: unable to access 'https://github.com/ezrealcong/AI_experiment.git/': Failed to connect to github.com port 443 after 21037 ms: Couldn't connect to server
则输入：git config --global --unset http.proxy
```

