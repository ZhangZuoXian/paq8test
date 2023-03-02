这个版本是单次预训练的版本，命令行有两个参数，第一个参数是训练数据占总数据的百分比(0~100)，第二个参数是待压缩的文件
注意，这个版本虽然写了分块相关的东西，但是没用到，整体还是无分块的
把无用的时间统计备份给删了，这部分占到近一半的CPU时间

decompress:
在pre-training的基础上添加解压部分

dev:
decompress的dev分支
压缩命令格式：paq8test [分块大小] [文件名] [训练量]
解压命令格式：paq8test -d [文件名]

2023.3.1
重新写这个部分
以前：训练完成后将参数保存，静态压缩时重新生成预测器并读取参数，在静态压缩的过程中还在更新参数。该方案没有真正的静态压缩，只是在压缩每一个数据块时重置了参数。
现在：
1. 首先动态压缩更新参数，达到训练量后将整个预测器的状态和混合器中的参数固定，不再更新。静态压缩时不新生成预测器，直接使用静态的预测器。需要重训练时重置整个预测器和混合器的状态。
2. 添加多文件输入
3. 添加解压部分

TODO：
1. 动态压缩和静态压缩的区别只有一个updateState，除此之外应该都一样
2. 每压缩完一个块都应该保存块压缩前、压缩后的长度，前者用于验证解压的正确性，后者用于解压时判断终止
3. 重训练时应当完全reset，包括share、映射表、混合器
4. 能够处理混合数据，即压缩时将多个文件打包~~，解压时在合适的时候开始下一个文件~~（解压没必要做，做压缩只是为了获得测试数据说明重训练的必要性，解压直接用paq8px的就行了）（暂时不管多文件输入，先做单文件，反正最后一起改了）
5. 设置合理的输出控制，在合理的时候输出每一块的压缩信息

需要注意的问题：
1. 最后一个数据块可能不满，这个数据块可以是动态的也可以是静态的
2. 动态压缩时将全部数据认作是一个数据块，所以虽然指定了数据块的大小，在实际压缩、解压时数据块还是可能是不定长的。同时，末尾的数据块也有可能不是指定长度
