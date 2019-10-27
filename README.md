# NT-OS

<h2>NT-OSは、x86系CPUで動作するOSです。</h2>



<h3>現在実装した機能</h3>
<p>デバイスドライバ:</p>
<p>    フロッピードライバ(floppy.c)</p>
        LBA方式でセクタを読み書きできるように、read_lbaとread_writeを実装しました。
        DMAドライバ(dma.c)
フォント:
    0~9のフォントを用意
    A~Pのフォントを用意(残りは後日実装予定)

参考文献
1.  30日できる!OS自作入門
2.  はじめて読む486

<h2>使い方</h2>
</p>brew install gcc-elf-i386を実行した後に、kernelの下でmake run</p>

## License
MIT
