# Description
**gh4t**最近被软件安全作业橄榄了，因此他找到了你，希望你能够帮助他完成这样一个简单的小实验。

现在**gh4t**已经完成了PE文件的注入部分，你只需要帮他把项目中标有**TODO**的地方补充完整即可。

注：已知答案的格式为：**flag{xxx}**

# Difficulty
:star::star::star:

# Flag
`flag{gh4tgh4t_S1mplePEShellCode}`

# Platform (Optional)
Windows 11

# Hint (Optional)
本题需要**x86MSVC**对应编译器在环境变量中才可直接使用**build.ps1**，若有问题请自行思考解决方法。

你只需要修改**task.cpp**与**task_blowfish.cpp**中标有**TODO**的地方即可完成本题。

**build.ps1**运行提示成功后即可使用**run.bat**运行程序观察效果，你可以使用头文件中已经声明好的**MessageBoxA**来辅助调试。

# WriteUp
补全程序所缺的Blowfish加&解密算法后再爆破密钥即可。
由于已经提示所有字节都是异或的同一个值，而flag以f开头，直接爆破异或的值即可。