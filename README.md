# NT-OS

<h2>NT-OSは、x86系CPUで動作するOSです。</h2>

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
    
<h3>現在実装中の機能</h3>
    <p>コンソール</p>
    <p>ファイルシステム</p>
    
<p>以下のURLは、僕が書いたフロッピディスクのデバイスドライバ入門記事です。参考にしてくれるとありがたいです。</p>
<p><a href="https://qiita.com/tetutetuman/items/50731863c08ddc307742">フロッピーデバイスドライバ入門</a></p>

参考文献
1.  30日できる!OS自作入門
2.  はじめて読む486

<h2>使い方</h2>
</p>brew install gcc-elf-i386を実行した後に、kernelの下でmake run</p>

## License
MIT
