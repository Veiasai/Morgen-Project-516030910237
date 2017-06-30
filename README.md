# Morgen-Project-516030910237
sort程序使用手册：
1.可选项：unit time
鉴于要实现插入order，查看状态，双线程工作，所以quote读取一直运行。
假如重复读取，仍会因为速度太快而导致订单总是在瞬间被填满，无法有效查看过程。
所以选择模拟，输入时可以设置模拟时间，即读取一行quote所用时间，单位ms。
2.文件录入
正确输入文件路径即可。
3.可选项：输出文件
输入文件名即可输出，enter可以选择不输出到文件。
4.运行前的order设置。
注意保证格式正确： 用逗号分隔
5.开始运行（配置有生成好的两个windows可执行文件）：
如果unit time设置不为零，会进入双线程模式，
可选操作按命令提示即可。
cout << "input 1: display the order status\n";
cout << "input 2: display the book\n";
cout << "input 3: insert a order\n";
cout << "input 4: pause quote reading\n";
cout << "input 5: exit\n";
6.附注：
在设置时间为0，且不输出文件时，读取速度为最大。
经测试，10000条quotes读取时间24ms, 即 2.4μs/q。
（Win7，VS2015，release）
输出到文件时。总时长约267ms，即26.7μs/q。
7.附注（重要）：
本程序不限制交易所数量，股票数量。
即：格式保证相同，修改交易所名称，股票名称，
增加交易所，增加股票，运行是正常的。
（动态存储）

iceberg:
#输出见文件icebergout.txt
#因为没有完整的时间与价格对应表
#切分操作是简单平均操作
#切分后时间点分散，要对应价格模拟购买数据不足，做一些处理后，结果意义也不大。
#icebergout输出的结果为各个时间点应当购买的百分比。
#以此便可将任意大订单切分。
