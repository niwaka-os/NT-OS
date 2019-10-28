# NT-OS

<h2>NT-OSは、x86系CPUで動作するOSです。</h2>

<h2>開発きっかけ</h2>
自分でも作ってみたくなった。

<h2>注意!!</h2>
このOSは、現在進行形で作っているソフトウェアなので、ところどころおかしな記述が見られますがお許しください。
参考になるファイルは、floppy.cとdma.cかと思います。
<p><strong>kernelフォルダの中のソースコードを参考にしてください!!</strong></p>
<h3>現在実装した機能</h3>
<p>デバイスドライバ:</p>
<p>    フロッピードライバ(floppy.c)</p>
        LBA方式でセクタを読み書きできるように、read_lbaとread_writeを実装しました。
<p>DMAドライバ(dma.c)</p>
フロッピーディスクの読み書きの制御を行うドライバです。       
        
      
<p>フォント:</p>
    <p>0~9のフォントを用意</p>
    <p>A~Pのフォントを用意(残りは後日実装予定)</p>
    <p>ASCII対応の配列を用意しました。font_ASCII配列を利用していただければ、ASCII_CODE指定での描画が可能です。</p>
    <p>1から全てフォントを作成する予定のため、全て揃えるのにもう少し時間がかかります。</p>
    
<h3>現在実装中の機能</h3>
    <p>コンソール</p>
    <p>ファイルシステム(floppy disk formatted on FAT12)</p>
    <p>ページング</p>
    ファイルシステム(fs.c)ではなくconsole.cの方にファイルシステムの実装を行なっていますが、動作が確認できたらfs.cに移行を進めるつもりです。
    
<p>以下のURLは、僕が書いたフロッピディスクのデバイスドライバ入門記事です。参考にしてくれるとありがたいです。</p>
<p><a href="https://qiita.com/tetutetuman/items/50731863c08ddc307742">フロッピーデバイスドライバ入門</a></p>

参考文献
1.  30日でできる!OS自作入門
2.  はじめて読む486

<h2>使い方</h2>
<p>環境→MacOS, qemu</p>
</p>brew install gcc-elf-i386を実行した後に、kernelの下でmake run</p>

![スクショ1]https://github.com/niwaka-os/NT-OS/tree/master/NT-OS1.png

## License
MIT
