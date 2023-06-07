MIC的英语全称是 "Many Integrated Core"。
是英特尔（Intel）推出的一种并行计算架构，也被称为 Intel Xeon Phi。
可以通过 PCI-E（Peripheral Component Interconnect Express）总线与 CPU（中央处理器）进行通信。
MIC是英特尔推出的一种针对高性能计算和并行处理任务的加速器架构。
它是基于x86架构的众核处理器，具有大量的处理核心和高度并行的计算能力。
MIC卡？

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

