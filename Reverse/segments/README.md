# Description
段错误（核心已转储）

# Difficulty
:star::star:

# Flag
`flag{3F6BC006-BA9F-DCE6-388A-0E338613E029}`

# Platform (Optional)
Windows 11

# Hint (Optional)
你会用obj文件的，对吧。你也知道怎么把obj文件编译成exe文件的，对吧。

# WriteUp
题目名字写的很清楚了，答案就在section中。
把每个段的名字按照guid格式拼接即可得到结果。
程序中采用的是CRC进行粗校验。