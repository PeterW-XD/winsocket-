Microsoft Visual Studio Community 2022 (64 位)：版本 17.3.6
使用方法：
1.运行Server，再运行Client；
2.若连接成功，Server显示Client的IP和Port；
3.三种通信模式选择 1发送文件 2接收文件 3聊天模式；
   有三种通信方式：Server发文件Client收文件；Server收文件Client发文件；Server和Client都选择CHAT模式；
4.发送文件请注意：发送文件名不能有空格，将文件拖入即可。另外，聊天模式是半双工的；
5.若进入一种模式通讯结束后想使用别的模式，请关闭Server和Client重新开始。

日志：
1）send 和 recv 的lenth参数不能用strlen 要用宏定义；switch里面输入要在前面加getchar()阻止意外退出；
2）CHAT通讯模式出现接收出错，修正：在每次接收前添加了清空buffer的语句，随即解决
3）发送时间计算出错，已经选用新的函数，精度在1s

程序还有很多不完善的地方，用户可用性还不太好
未来有时间，我还想实现公网上的网络通信，不过最近的学习和申请工作艰巨而重要，就在此留下一个小小的愿望！


