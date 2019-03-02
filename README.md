# helloworld
* 启动游戏
* 运行Manager线程
* 运行Server线程，server线程自动接收所有发送到本机6789端口的数据包
* 运行Client线程，client线程自动发送心跳广播以及发送队列的内容
* 运行User线程
* User线程进入select loop状态，等待用户选择挑战对象
* user线程创建一个udpclient，client线程使用这个对象发起连接
* server线程收集到连接请求，则回应到user线程，出现询问
* user确认之后，将自动进入fight状态