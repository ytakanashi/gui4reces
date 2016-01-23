_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
【 ソフト名 】　gui4reces
【バージョン】　0.0.1.7
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
 様々な書庫の「再圧縮」「圧縮」「解凍」「内容一覧表示」「検査」「内容削除」「リネーム」を行うソフトウェアです。
 文字化け防止機能やフィルタ(指定文字列、正規表現、更新日時)機能等々を備えています。
 統合アーカイバライブラリの他、書庫用Susie Plug-inやTotal Commander Pluginにも対応しています。
 ...その実態は初心者向け再圧縮ソフトを装った、recesのGUIフロントエンドです。



●特徴
 [対応書庫]
 ・zip、rar、7z、lzhファイルなど主要な書庫をサポート。
 ・Susie Plug-in(*.spi)で様々な書庫に対応可能。
 ・Total Commander Plugin(*.wcx)で更に多くの書庫に対応可能。
 ・b2eスクリプト(*.b2e)で書庫操作以外の処理も可能。
 ・UNICODE文字、4GB以上のファイルに対応。 *1
 ・ヘッダ暗号化(ファイル名暗号化)の7zやrarをサポート。

 [書庫処理]
 ・ファイル名の文字化けが起きにくい。
 ・zipファイルのコードページを指定して解凍することが出来る。
 ・プログレスバーを自前で表示し、無駄なウインドウを表示させない。
 ・ディレクトリのタイムスタンプ復元が可能。 *2
 ・ファイルの分割/結合が出来る。
 ・二重フォルダ防止機能あり。
 ・基底ディレクトリを含まない圧縮が可能。
 ・処理後、元ファイルをごみ箱へ送ることが出来る。

 [その他機能]
 ・設定をプロファイルとして保存することが出来る。
 ・指定した文字、正規表現、サイズ、日付などの各種フィルタを指定することが出来る。
 ・パスワードリストを用いた総当たり解凍が出来る。
 ・レジストリは一切使用しない。 *3


    *1 対応している書庫ライブラリに限ります。
    *2 タイムスタンプが保持され、かつ取得出来る場合に限ります。
       出来ない場合、配下のディレクトリやファイルから取得します。(「ディレクトリのタイムスタンプを復元」オプション使用)
    *3 ライブラリ側が使用する場合もありますのでご注意下さい。



●ファイル構成
 cfg/                    ----- プロファイル保存ディレクトリ
 7-zip32.dll             ----- 7-zip32.dll文字化け対策版
 gui4reces.cfg           ----- gui4reces設定ファイル(本体終了時作成されます)
 gui4reces.exe           ----- gui4reces本体(32bit版)ファイル
 gui4recesHelp.chm       ----- gui4recesヘルプファイル
 NYSL_withfaq.TXT        ----- ライセンスファイル
 Readme.txt              ----- このファイル
 reces.exe               ----- reces(32bit版)ファイル
 Src.7z                  ----- gui4recesソースファイル

 ./x64
    cfg/                 ----- プロファイル保存ディレクトリ
    7-zip64.dll          ----- 7-zip64.dll文字化け対策版
    b2e64.dll            ----- b2e32.dll(64bit版)ファイル
    gui4reces.cfg        ----- gui4reces設定ファイル(本体終了時作成されます
    gui4reces.exe        ----- gui4reces本体(64bit版)ファイル
    reces.exe            ----- reces(64bit版)ファイル

 ./b2e
 ./x64/b2e
    exe.msi.b2e          ----- Universal Extractorを使用し、インストーラを解凍するb2eスクリプトファイル
    mp3.b2e              ----- LAMEを使用し、mp3ファイルのエンコード・デコードを行うb2eスクリプトファイル



●インストール
 1.「gui4reces0017.zip」を適当なディレクトリに解凍して下さい。
 2.以下の内、必要な各書庫操作ライブラリをパスの通ったディレクトリへコピーして下さい。

    == ライブラリ ================================================================================
   ||    32bit版        |       64bit版        ||            対応            ||      入手先      ||
   ||-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*||-*-*-*-*-*-*-*-*-*||
   ||    7-zip32.dll    |     7-zip64.dll      ||  再圧縮、圧縮、解凍、削除  ||       同梱       ||
   ||    tar32.dll      |     tar64.dll        ||  再圧縮、圧縮、解凍        ||  *1 64bit版は *2 ||
   ||    UNLHA32.DLL    |     ------------     ||  再圧縮、圧縮、解凍、削除  ||        *1        ||
   ||    unrar32.dll    |     unrar64j.dll     ||  解凍                      ||  *1 64bit版は *6 ||
   ||    UnIso32.dll    |     ------------     ||  解凍                      ||        *1        ||
   ||    XacRett.dll    |     ------------     ||  解凍                      ||        *3        ||
   ||  *.b2e+b2e32.dll  |   *.b2e+b2e64.dll    ||  再圧縮、圧縮、解凍        ||        *1        ||
   ||       *.spi       |        *.sph         ||  解凍                      ||                  ||
   ||                   |  *.spi+ZBYPASSA.SPH  ||                            ||        *5        ||
   ||       *.wcx       |    *.wcx,*.wcx64     ||  解凍                      ||                  ||
   ||      -------      |     UNBYPASS.DLL     ||  再圧縮、圧縮、解凍        ||        *5        ||
    ==============================================================================================

   *1
      統合アーカイバプロジェクト( http://www.csdinc.co.jp/archiver/ )
   *2
      綾川的趣味之接続集( http://homepage1.nifty.com/Ayakawa/index.html )
   *3
      Bonty's HomePage( http://hp.vector.co.jp/authors/VA030681/index.htm )
   *4
      Yanagi's Home Page( http://homepage2.nifty.com/NYanagi/index.html )
   *5
      TORO's Library( http://homepage1.nifty.com/toro/index.html )
   *6
      Ru^3 Honpo( http://www.vesta.dti.ne.jp/~tsato/index.html )

 ・7-zip32.dllについて、現在統合アーカイバプロジェクトで配布されている9.22.00.01は、
   一部ファイル名が文字化けしてしまいます。
   そこでgui4recesでは、「7-zip32.dll/7-zip64.dll文字化け対策版」を同梱しています。
   最新版はFrost Moon Project( http://www16.atpages.jp/rayna/index.html )にて入手することが出来ます。

 ・rar書庫を取り扱うにあたり、必ず「unrar.dll」「unrar64.dll」を最新版にバージョンアップしてください。
   RARLAB( http://www.rarlab.com/ )の「RAR」->「Extras」->「UNRAR.dll」からダウンロードすることが出来ます。

 ・「7z.dll対応版7-zip32.dll/7-zip64.dll」( http://www16.atpages.jp/rayna/index.html )に対応しています。
   gui4reces x64版ではUNLHA32.DLLやXacRett.dllが使用できないため、導入をお勧めします。
   1.「7-Zip x64版」( http://www.7-zip.org/ )をインストール。
   2.「7-zip32.dll/7-zip64.dll/7z.dll文字化け対策版」( http://www16.atpages.jp/rayna/index.html )をダウンロード。
   3.「x64\7z.dll」を7-Zipインストールディレクトリにコピー。
   4.「7-zip32-full\7-zip64.dll」をパスの通ったディレクトリにコピー。
   以下の形式に対応しています。
      7z, XZ, BZIP2, GZIP, TAR, ZIP
      AR, ARJ, CAB, CHM, CPIO, CramFS, DMG, EXT, FAT, GPT, HFS, IHEX, ISO, LZH, LZMA, MBR, MSI, NSIS, NTFS,
      QCOW2, RAR, RPM, SquashFS, UDF, UEFI, VDI, VHD, VMDK, WIM, XAR, Z

 ・ライブラリの優先順位は、
      UNLHA32.dll / UNLHA32.dll+UNBYPASS.DLL
      unrar32.dll / unrar64j.dll
      tar32.dll   / tar64.dll
      7-zip32.dll / 7-zip64.dll
      UnIso32.dll
      XacRett.dll
      Susie Plug-in(*.spi / *.sph, *.spi+ZBYPASSA.SPH)
      Total Commander Plugin(*.wcx / *.wcx64)
      b2eスクリプト(*.b2e) + b2e32.dll
   です。

   上記統合アーカイバライブラリをすべて導入すると、以下の拡張子に対応することが出来ます。
   (Susie Plug-inやTotal Commander Plugin、b2eスクリプトの導入により更に対応させることが出来ます。)
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
 compiler/debugger:Microsoft Visual C++ 2015(Microsoft Visual Studio Community 2015)
                   Microsoft Visual C++ 2010 Express
 editor:xyzzy version 0.2.2.235/ResEdit 1.6.6 Unicode build.

 [Sub]
 OS:Microsoft Windows XP Home Edition Build 2600 SP3
 CPU:Intel(R) Atom(TM) CPU N270@1.60GHz,1600MHz(4x400)
 memory:1016MB
 compiler/debugger:Microsoft Visual C++ 2010 Express
 editor:xyzzy version 0.2.2.235/ResEdit 1.6.6 Unicode build.


●謝辞
 本ソフト開発にあたり参考/使用させて頂いたソフトウェアや、
 各書庫操作ライブラリの作者様方...
   k.inaba氏(Noah,UnDller)
   Claybird氏(LhaForge,b2e32.dll,tar32.dll,unrar32.dll)
   鬼束 裕之氏(Explzh)
   Bonty氏(Archon2,XacRett.dll)
   高田 謙氏(undll.exe,UnIso32.dll)
   秋田 稔氏(7-zip32.dll)
   市丸 剛氏(7-zip64.dll,tar64.dll)
   吉岡 恒夫氏(tar32.dll)
   Ｍｉｃｃｏ氏(UNLHA32.dll)
   Alexander Roshal氏(unrar.dll/unrar64.dll)
   亀井 哲弥氏(unrar32.dll)
   RuRuRu氏(unrar32.dll x64/ユニコード対応版)
   TORO氏(UNBYPASS.DLL,ZBYPASSA.SPH)
   Nozomu Katô氏(SRELL)
   Kuro氏(Mery)
 ...他多数!!!
 大変可愛らしいアイコンを提供していただいた「なつだ」氏、
 そしてユーザの皆様方に、
 この場を借りて御礼申し上げます。


●ライセンス
 ・本ソフトはNYSL Version 0.9982に準拠して配布されています。
   ライセンスの詳細は同梱の「NYSL_withfaq.TXT」をご覧下さい。
 ・「reces」( http://www16.atpages.jp/rayna/index.html )はNYSL Version 0.9982に準拠して配布されています。
   ライセンスの詳細は「reces」に同梱されている「NYSL_withfaq.TXT」をご覧ください。
 ・「7-zip32.dll/7-zip64.dll文字化け対策版」( http://www16.atpages.jp/rayna/index.html )は
   GNU Lesser General Public License (LGPL)の下で配布されています。
   ライセンスの詳細は下記URLをご覧下さい。
      http://www.gnu.org/copyleft/lesser.ja.html
 ・「b2e64.dll」はClaybird氏による「b2e32.dll」を64bit化したものです。
   「b2e32.dll」はNYSL ver 0.9982が適用されています。
   ライセンスの詳細は「b2e32.dll」に同梱されている「b2e32.txt」をご覧ください。


●その他
 サポート(ご意見、ご感想、不具合報告、要望等)は
 Y.R.Takanashi@gmail.com
 若しくは
 BBS(以下URLよりアクセス可能)にて、

 最新版は
 http://www16.atpages.jp/rayna/index.html
 でお願いします。


●開発履歴
 ○Ver.0.0.1.7 - 2016/01/23
 ・reces Ver.0.00r31に対応。
 ・「動作」に「リネーム」を追加。
 ・プロファイルの保存ができない不具合を修正。
 ・7-zip32.dll/7-zip64.dll文字化け対策版 Ver.15.14.00.01 betaに更新。

 ○Ver.0.0.1.6 - 2015/10/21
 ・reces Ver.0.00r30に対応。
 ・「その他」タブ「文字コード」の「自動選択」を「指定しない」に変更。
 ・「再圧縮/圧縮」タブ「書庫作成」に「拡張子を保持」を追加。
 ・マルチスレッド(/MT)でビルドするように。

 ○Ver.0.0.1.5 - 2015/08/18
 ・reces Ver.0.00r29に対応。
 ・unrar32.dll/unrar64j.dllの同梱を中止。
 ・7-zip32.dll/7-zip64.dll文字化け対策版 Ver.15.06.00.02 betaに更新。
 ・「パスワード」に「ファイルを開く」を追加。
 ・「出力」に「実行時に指定する」を追加。
 ・ディレクトリ選択ダイアログをSHBrowseForFolder()に変更。
 ・「ディレクトリを再帰的に追加」を追加。
 ・「ディレクトリ」タブ追加。「その他」タブの項目の一部を移動。
 ・「その他」タブに「文字コード」「b2eスクリプト」を追加。
 ・「b2e64.dll」を同梱するように。
 ・「b2e/exe.msi.b2e」「b2e/mp3.b2e」を同梱するように。
 ・コンパイラをMicrosoft Visual C++ 2015に変更。

 ○Ver.0.0.1.4 - 2015/05/15
 ・reces Ver.0.00r27に対応。
 ・「フィルタ」でサブディレクトリ以下が検索されない不具合を修正。
 ・「動作」の一部を「動作詳細」タブに移動。
 ・「動作」に「テスト」「削除」を追加。
 ・「動作詳細」タブに「ログウインドウを表示」を追加。
 ・「圧縮」タブ「基底ディレクトリを除外」と「解凍」タブ「共通パスを除外」の変更を同期するように。

 ○Ver.0.0.1.3 - 2015/03/14
 ・reces Ver.0.00r26に対応。
 ・「フィルタ」タブ、処理対象除外の「EmptyDir」が設定ファイルから反映されない不具合を修正。
 ・「正規表現を使用する」を追加。
 ・「ファイル名のみを対象」を追加。
 ・リストビューにチェックボックスを追加。
 ・リストビューに右クリックメニューを追加。

 〇Ver.0.0.1.2 - 2015/02/09
 ・reces Ver.0.00r25に対応。
 ・64bit版を同梱するように。
 ・7-zip32.dll/7-zip64.dll文字化け対策版を同梱するように。
 ・ヘルプファイルの容量を削減。
 ・アイコンファイルの容量を削減。
 ・「バージョン情報」ボタンを「V」、「ヘルプ」ボタンを「H」に改称し移動。
 ・「すぐに開始」オプションを追加。
 ・リスビューのファイル名を編集できるように。

 〇Ver.0.0.1.1 - 2014/10/13
 ・reces Ver.0.00r23に対応。
 ・バージョン情報ダイアログにスクロールバーを表示するように。
 ・「バックグラウンド」オプション追加。
 ・ショートカット作成で、ウインドウ最大化・最小化の選択ができるように。

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
