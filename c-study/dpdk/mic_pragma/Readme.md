MIC的英语全称是 "Many Integrated Core"。
是英特尔（Intel）推出的一种并行计算架构，也被称为 Intel Xeon Phi。
可以通过 PCI-E（Peripheral Component Interconnect Express）总线与 CPU（中央处理器）进行通信。
MIC是英特尔推出的一种针对高性能计算和并行处理任务的加速器架构。
它是基于x86架构的众核处理器，具有大量的处理核心和高度并行的计算能力。
MIC卡？

## pragma offload

offload target(mic)
意思是将下面这一段程序offload到MIC卡去执行
    #pragma offload target(mic) in(arr_a:length(n)) in(arr_b:length(n)) out(arr_c:length(n))
    {
        __m512i m_a, m_b, m_c;
        m_a = _mm512_load_epi32(arr_a);
        m_b = _mm512_load_epi32(arr_b);
        m_c = _mm512_add_epi32(m_a, m_b);
        _mm512_store_epi32(arr_c, m_c);
    }

## pragma once
与头文件中的#ifndef #define #endif作用相同，仅包含一次

## pragma message

    #if _M_IX86 >= 500
    #pragma message("_M_IX86 >= 500")
    #endif

## pragma warning
如果有一个不能顺利在4级警告下编译的头文件，下面的代码改变警告等级到3，然后在头文件的结束时恢复到原来的警告等级。

#pragma warning(push, 3)
// Declarations/ definitions //要书写的代码
#pragma warning(pop)
