# Linux系统构成


* Linux内核
内核：管理和分配计算机资源的的核心软件
内核的位置：`/boot/vmlinuz`
内核功能：
* 内存管理：虚拟内存管理机制：给系统一种拥有更大内存的幻觉
    * 只需要将进程的一部分内存页加载进内存
    * 进程的其他部分页位于实际物理内存外的交换区
    * 在需要时把页从交换区送入内存
    * 物理内存吃紧时把暂时不需要的页放入交换区
* 进程管理：进程的调度/进程创建/进程终止/进程挂起等
* 文件系统：提供对磁盘上文件的管理
    * 能够支持通过不同类型的文件系统从硬盘中读写数据
    * 实现：使用虚拟文件系统VFS作为和每个文件系统交互的接口，为Linux内核同任何类型文件系统通信提供了一个标准接口
* 设备管理：实现内核和设备的通信
    * 实现：通过设备驱动程序作为中间人允许内核与设备之间交换数据
    * 内核中插入驱动的办法
        * 将设备驱动代码编译到内核：每次添加设备都有编译内核，很麻烦
        * 使用内核模块：随用随插，无需重新编译
    * 设备类型
        * 块设备 e.g. 硬盘
        * 字符设备 e.g. 终端
        * 网络设备 e.g. 网卡和回环设备
* 应用程序系统调用接口：用户进程利用系统调用请求内核执行各种任务


* 应用软件
    * 桌面软件系统：KDE/GNOME/Unity
    * GNU coreutils：一些程序要借助Shell执行
        * shell：允许你输入文本命令，然后解释命令，并在内核中执行
            * 单个命令执行
                * 执行内置shell命令
                * 执行非内置shell命令，查找并执行对应程序
            * 多个命令执行：制作shell脚本
            * 系统自带多种shell：zsh/ash/tcsh等，Linux会采用一个作为默认
        * 二进制命令程序：ls/rm/kill/cat/chmod等
    * 其他应用软件：办公软件/游戏软件

