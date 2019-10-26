# NT-OS

NT-OSは、x86系CPUで動作するOSです。



デバイスドライバ:
  フロッピードライバ(floppy.c)
    LBA方式でセクタを読み書きできるように、read_lbaとread_writeを実装しました。
  
  DMAドライバ(dma.c)

参考文献
1.  30日できる!OS自作入門
2.  はじめて読む486

## License
MIT
