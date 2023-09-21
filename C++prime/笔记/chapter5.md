




## 异常处理机制

**当程序某个部分检测到一个无法处理的问题，需要用到异常处理**


* 异常处理机制
    * 异常检测：throw表达式
        * 发出异常信号，表示程序遇到故障，无法继续运行
        * 发完异常信号，检测部分完成任务
        * 不关心不需要知道故障在哪里得到处理
    * 异常处理
        * try语句
        * catch子句
    * 异常处理过程
        * 寻找处理函数的过程和函数调用链条相反
        * 如果没有合适的try语句和catch子句处理异常，将调用terminate标准库函数，导致程序非正常退出

* 编写异常安全的代码很困难：异常中断了程序的正常流程
    * 异常发生时，部分计算可能已经完成，另一部分可能没有完成
    * 对象可能处于无效或者未完成的状态，或者资源没有释放

* 标准异常
    * 头文件
        * exception
        * stdexception
            * exception
            * runtime_error 运行时错误
            * range_error
            * overflow_error 计算上溢
            * underflow_error 计算下溢
            * logic_error 逻辑错误
            * domain_error
            * invalid_argument无效参数
            * length_error 超出最长范围
            * out_of_range 超出有效范围
        * new
            * bad_alloc
        * type_info
            * bad_cast
    * exception/bad_cast/bad_alloc只能默认初始化，不能提供初始值
    * 其他异常类型必须提供C风格字符串初始值，不允许默认初始化
    * what成员函数：没有参数，返回C风格字符串，提示关于异常的一些文本信息