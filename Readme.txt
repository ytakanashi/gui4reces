_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
【 ソフト名 】　gui4reces
【バージョン】　0.0.1.0
【 製作者名 】　x@rgs
【 動作環境 】　Windows XP以降
【 製作言語 】　C++
【ｿﾌﾄｳｪｱ種別】　フリーソフトウェア
【 配布条件 】　NYSL Version 0.9982
【  連絡先  】	Y.R.Takanashi@gmail.com
【  サイト  】	http://www16.atpages.jp/rayna/index.html
_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
●はじめに
 この度は、「gui4reces」(以下、本ソフトとも)をダウンロードしていただき、ありがとうございます。
 以下、稚拙な文章が続きますが、もしかしたら重要な情報が含まれるかもしれませんので、目を通していただけると幸いです。



●説明
 「gui4reces」はウインドウにドラッグ&ドロップするだけで、
 様々な書庫の「再圧縮」「圧縮」「解凍」を行うソフトウェアです。
 統合アーカイバライブラリの他、書庫用Susie Plug-inにも対応しています。
 ...その実態は初心者向け再圧縮ソフトを装った、recesのGUIフロントエンドです。



●特徴
 [対応書庫]
 ・zip、rar、7z、lzhファイルなど主要な書庫をサポート。
 ・Susie Plug-inで様々な書庫に対応可能。
 ・UNICODE文字、4GB以上のファイルに対応。 *1
 ・ヘッダ暗号化(ファイル名暗号化)の7zやrarをサポート。

 [書庫処理]
 ・LMZIP32.dllを使用することで、不正な書庫でファイル名の文字化けが起きにくい。
 ・プログレスバーを自前で表示し、無駄なウインドウを表示させない。
 ・ディレクトリのタイムスタンプ復元が可能。 *2
 ・ファイルの分割/結合が出来る。
 ・二重フォルダ防止機能あり。
 ・基底ディレクトリを含まない圧縮が可能。
 ・処理後、元ファイルをごみ箱へ送ることが出来る。

 [その他機能]
 ・設定をプロファイルとして保存することが出来る。
 ・指定した文字、サイズ、日付などの各種フィルタを指定することが出来る。
 ・パスワードリストを用いた総当たり解凍が出来る。
 ・レジストリは一切使用しない。 *3


    *1 書庫Dllが対応している場合に限ります。
    *2 勿論タイムスタンプが保持されている場合に限ります。
      保持されていない場合、配下のディレクトリやファイルから取得します。(reces Ver.0.00r16以上が必要)
    *3 ライブラリ側が使用する場合もありますのでご注意下さい。



●ファイル構成
 cfg/                    ----- プロファイル保存ディレクトリ
 gui4reces.cfg           ----- gui4reces設定ファイル(本体終了時作成されます)
 gui4reces.exe           ----- gui4reces本体
 gui4recesHelp.chm       ----- gui4recesヘルプファイル
 NYSL_withfaq.TXT        ----- ライセンスファイル
 Readme.txt              ----- このファイル
 reces.exe               ----- reces Ver.0.00r21
 Src.7z                  ----- gui4recesソースファイル
 unrar32.dll             ----- RuRuRu氏によるunrar32.dll x64/ユニコード対応版




●インストール
 1.「gui4reces0010.zip」を適当なディレクトリに解凍して下さい。
 2.以下の各書庫操作ライブラリをパスの通ったディレクトリへコピーして下さい。

   7-zip32.dll(再圧縮、圧縮、解凍)
   tar32.dll(再圧縮、圧縮、解凍)
   UNLHA32.DLL(再圧縮、圧縮、解凍)
   unrar.dll,unrar32.dll(解凍)
   UnIso32.dll(解凍)
   XacRett.dll(解凍)
   b2e32.dll(直接操作のみ)
   LMZIP32.dll(解凍)
   amzip.spi(解凍)
   ax7z_s.spi+7z.dll(解凍)
   その他*.spi(解凍)

   XacRett.dllとLMZIP32.dll以外の統合アーカイバライブラリは、
      統合アーカイバプロジェクト(http://www.csdinc.co.jp/archiver/)
   よりダウンロードすることが出来ますが、
   Claybird氏「LhaForge」付属の「caldix」を使用するとより簡単にインストールすることが出来ます。
   XacRett.dllは、
      Bonty's HomePage(http://hp.vector.co.jp/authors/VA030681/index.htm)
   LMZIP32.dllは、
      Yanagi's Home Page(http://homepage2.nifty.com/NYanagi/index.html)
   unrar.dllは、
      RARLAB(http://www.rarlab.com/)
   amzip.spiは、
      amzip - Yet Another ZIP Extract Plug-in for Susie32(http://www.nk2.org/amzip/)
   ax7z_s.spi+7zは、
      物置(http://yak3.myhome.cx:8080/junks/)
   よりダウンロードすることが出来ます。

   上記ライブラリをすべて導入すると、以下の拡張子に対応することが出来ます。
   (Susie Plug-inの導入により更に対応させることが出来ます。)
     lzh,lha,lzs
     rar
     zip,7z,jar
     tar,tgz,gz,tar.gz,bz2,tbz,tbz2,tar.bz2,xz,txz,tar.xz,lzma,tlz,tlzma,tar.lzma
     Z,taz,tz,tar.Z,cpio,cpgz,a,lib,rpm,deb
     iso
     ace,arc,arg,arj,asd,b2a,b64,bel,bin,boo,bza,gza,C,cab,cpt,dmg,dz,F,xF,gca,hqx,imp,ish,jak,ntx,pit,pak,wad,pff,rez,sp,spl,uue,xxe,zac,zoo

 3.「gui4reces.exe」を実行、「バージョン情報」をクリックし、
   各種ライブラリが正しくインストールされているか確認します。



●アンインストール
 レジストリは一切使用しないため、ディレクトリごと、
 若しくはコピーしたファイル(作成したショートカット含む)を削除して下さい。


●使用方法
 gui4recesHelp.chmをご覧下さい。


●注意
 ・バグだらけです。


●開発環境
 [Main]
 OS:Microsoft Windows 7 Home Premium 64-bit (6.1, Build 7600)
 CPU:Intel(R) Core(TM) i5 CPU M 460 @ 2.53GHz (4 CPUs), ~2.5GHz
 memory:4096MB RAM
 compiler:Microsoft Visual C++ 2010 Express/gcc version 4.7.1 (tdm-1)
 debugger:Microsoft Visual C++ 2010 Express/gdb 7.3 with Code::Blocks 10.05
 editor:xyzzy version 0.2.2.235/ResEdit 1.6.2 Unicode build.

 [Sub]
 OS:Microsoft Windows XP Home Edition Build 2600 SP3
 CPU:Intel(R) Atom(TM) CPU N270@1.60GHz,1600MHz(4x400)
 memory:1016MB
 compiler:Microsoft Visual C++ 2010 Express/gcc version 4.7.1 (tdm-1)
 debugger:Microsoft Visual C++ 2010 Express/gdb 7.3 with Code::Blocks 10.05
 editor:xyzzy version 0.2.2.235/ResEdit 1.5.11 Unicode build.


●謝辞
 本ソフト開発にあたり参考/使用させて頂いたソフトウェアや、
 各書庫操作ライブラリの作者様方...
   k.inaba氏(Noah,UnDller)
   Claybird氏(LhaForge,b2e32.dll)
   鬼束 裕之氏(Explzh)
   Bonty氏(Archon2,XacRett.dll)
   高田 謙氏(undll.exe,UnIso32.dll)
   秋田 稔氏(7-zip32.dll)
   吉岡 恒夫氏/Claybird氏(tar32.dll)
   Ｍｉｃｃｏ氏(UNLHA32.dll)
   Alexander Roshal氏(unrar.dll)
   RuRuRu氏(unrar32.dll x64/ユニコード対応版)
   N.Yanagi氏(LMZIP32.dll)
   Nobuyuki Honda氏(amzip.spi)
   Yak!氏(ax7z_s.spi,7z.dll)
 ...他多数!!!
 大変可愛らしいアイコンを提供していただいた「なつだ」氏、
 そしてユーザの皆様方に、
 この場を借りて御礼申し上げます。


●ライセンス
 本ソフトはNYSL Version 0.9982に準拠して配布されています。
 ライセンスの条文については同梱の「NYSL_withfaq.TXT」をご覧下さい。
 各種書庫操作ライブラリについてはそれぞれのライセンスに従います。


●その他
 本ソフトの派生作品を配布される、または本ソフトについてWeb、雑誌などの媒体にて掲載されると言った場合には、
 サイトにて紹介させていただきますので、お気軽にご連絡下さい。
 サポート(ご意見、ご感想、不具合報告、要望等)は
 Y.R.Takanashi@gmail.com
 若しくは
 BBS(以下URLよりアクセス可能)

 最新版は
 http://www16.atpages.jp/rayna/index.html
 でお願いします。


●開発履歴
 〇Ver.0.0.1.0 - 2014/08/31
 ・reces Ver.0.00r22に対応。
 ・リストビューで選択中のファイル数を表示するように。その他、リストビューに関する幾つかの修正。
 ・XPでのバージョン情報表示でアイコンが表示されない不具合を修正。
 ・バージョン情報表示を高速化。
 ・ウインドウのリサイズが可能に。

 〇Ver.0.0.0.9 - 2014/07/27
 ・reces Ver.0.00r21に対応。
 ・ファイルリストへの追加が降順で行われる不具合を修正。
 ・「文字化け」タブを削除。
 ・プロファイル未選択時もrecesへのショートカット作成が出来るように。
 ・出力先やspiのあるディレクトリの指定が行えない不具合を修正。
 ・「再圧縮/圧縮」タブに「自動リネーム」オプションを追加。(recesの/oor)
 ・「解凍」タブに「共通パスを除外」オプションを追加。(recesの/eb[level|x])
 ・「その他」タブに「使用するライブラリを指定」及び「設定」ボタンを追加。
 ・バージョン情報ダイアログに指定されたディレクトリ内のSusie Plug-inも列挙するように。
 ・選択している動作によって、「実行」ボタンの文字列を変更するように。
 ・「その他」タブに「作業ディレクトリ」オプションを追加。
 ・「ヘルプ」ボタンを追加。
 ・除外フィルタでサイズ指定が出来ない不具合を修正。
 ・「除外フィルタ」タブを「フィルタ」タブに統合。

 〇Ver.0.0.0.8 - 2014/01/03
 ・reces Ver.0.00r15に対応。
 ・「文字化け」タブを追加。ロケールの指定、Unicodeエスケープシーケンスのデコードを行うかを設定できるように
 ・「recesのウインドウを閉じない」を「recesのウインドウを閉じる」に変更。
 ・「gui4recesを終了する」を追加。
 ・動作に「リスト」を追加。
 ・出力ファイル名をアイテムごとに指定出来るように。
 ・ファイル名指定が無効に出来ない不具合を修正。

 〇Ver.0.0.0.7 - 2013/12/08
 ・reces Ver.0.00r14に対応。
 ・ソースコードを全て書き直しました。
 ・reces.exe、unrar32.dllをgui4reces.exeと同じ階層に配置するように。
 ・設定をプロファイルとして複数保存出来るように。
 ・recesへは一部引数を除き、リストファイルと設定ファイルのパスのみを送る様に。
 ・コンパイラをMicrosoft Visual C++ 2010 Expressに変更。

 ○Ver.0.0.0.6 - 2013/01/21
 ・reces Ver.0.00r11に対応。
 ・対応するrecesを同梱するように。
 ・設定をcfgファイルに保存するように。
 ・製作言語をHot Soup Processor ver3.21からgcc version 4.6.1 (tdm-1)に変更。
 ・作者HNを変更。
 ・NYSL Version 0.9982を適用

 ○Ver.0.0.0.5 - 2010/10/03
 ・reces Ver.0.00r9に対応。(/de,/dfオプションの廃止、/d,/tr,/tnオプションの追加等)
 ・/aオプションを使用せず、独自にコマンドを作成するように。これにより大量のファイルを一度に処理する事が出来ます。
 ・「使用するDLLを指定する」オプションを追加。
 ・「処理終了後ダイアログを表示する」オプションを追加。
 ・DnDされたら直ぐに処理を開始せず、リストに追加するように。
 ・ログを別ウインドウで表示するように。
 ・reces.exeに送られるコマンドラインを表示しない様に。

 ○Ver.0.0.0.4 - 2010/05/08
 ・直接送信モードで、送信できる文字列数を増加した。

 ○Ver.0.0.0.3 - 2010/04/06
 ・reces Ver.0.00r7に対応。(/aオプションの追加によるコマンドライン分割の廃止。)
 ・exeに直接DnDできないように。
 ・オブジェクトの配置を一部変更。

 ○Ver.0.0.0.2 - 2010/02/11
 ・reces Ver.0.00r6に対応。(/dオプションの廃止、/de,/dfオプションを追加等。)
 ・reces.exeに送られるコマンドラインを表示する様に。
 ・オブジェクトの配置を一部変更。

 ○Ver.0.0.0.1 - 2010/01/21
 ・公開。
 ・アイコンを設定。(Special thx! なつだ氏)
 ・reces本体がバージョンアップされない限り、これを安定バージョンとします。


_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
This Readme file made by x@rgs
