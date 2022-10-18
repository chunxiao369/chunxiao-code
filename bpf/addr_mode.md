  Addressing mode  Syntax               Description

   0               x/%x                 Register X
   1               [k]                  BHW at byte offset k in the packet
   2               [x + k]              BHW at the offset X + k in the packet
   3               M[k]                 Word at offset k in M[]
   4               #k                   Literal value stored in k
   5               4*([k]&0xf)         Lower nibble * 4 at byte offset k in the packet
   6               L                    Jump label L
   7               #k,Lt,Lf             Jump to Lt if true, otherwise jump to Lf
   8               x/%x,Lt,Lf           Jump to Lt if true, otherwise jump to Lf
   9               #k,Lt                Jump to Lt if predicate is true
  10               x/%x,Lt              Jump to Lt if predicate is true
  11               a/%a                 Accumulator A
  12               extension            BPF extension

