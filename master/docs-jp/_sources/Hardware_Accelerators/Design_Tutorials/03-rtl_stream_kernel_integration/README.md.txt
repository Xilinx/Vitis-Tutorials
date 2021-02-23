<p align="right"><a href="https://xilinx.github.io/Vitis-Tutorials/master/docs/index.html">English</a> | <a>日本語</a></p>

# AXI-Stream と Vitis を使用した混合カーネル デザインのチュートリアル

このチュートリアルでは、RTL カーネルと HLS C カーネルの両方、および Vitis ビジョン ライブラリを含む、混合カーネルのハードウェア デザイン例のデザイン フローを示します。このデザインはリアルタイム クロックの画像を生成し、サイズを変更してから、グローバル メモリ内の入力画像とアルファ ミックスし、最後に結果の画像をグローバル メモリへ出力します。AXI-Stream インターフェイスは、カーネル間の接続に使用されます。

<div align="center"><img src="./doc/images/alpha_mixing.jpg" alt="アルファ ミキシング" ></div>
<br/><br/>

ハードウェア デザインには、 rtc\_gen、alpha\_mix、strm\_dump の 3 つのカーネルが含まれます。これらのカーネルは AXI-Stream リンクを使用して直接接続されます。次の図は、デザインのトポロジを示しています。

<div align="center"><img src="./doc/images/topo.png" alt="トポロジ" ></div>
デザインは、次のソフトウェア/ハードウェア環境およびツール チェーン バージョンで検証されています。

* オペレーティング システム
  * Redhat/CentOS 7.4 - 7.9
  * Ubuntu 16.04/18.04
  * OpenCV \& OpenCL ライブラリ (必須)
* Vitis: 2020.2
* XRT: 2.8.726
* ハードウェアとプラットフォーム (運用プラットフォームと開発プラットフォームの両方が必要)
  * Alveo U200 - xilinx\_u200\_xdma\_201830\_2、xilinx\_u200\_gen3x16\_xdma\_1\_1\_202020\_1
  * Alveo U250 - xilinx\_u250\_xdma\_201830\_2、xilinx\_u250\_gen3x16\_xdma\_3\_1\_202020\_1
  * Alveo U50  - xilinx\_u50\_gen3x16\_xdma\_201920\_3
  * Alveo U280 - xilinx\_u280\_xdma\_201920\_3

ディレクトリ構造と簡単な説明は、次のとおりです。

~~~
├── doc/                            # documents
├── hw/                             # Hardware build working directory
│   ├── alpha_mix.cpp               # HLS C source code for alpha_mix kernel
│   ├── build_rtc_gen_xo.sh         # shell script to call Vivado to package rtc_gen kernel IP to xo file
│   ├── config_gen.mk               # Makefile sub-module to generate Vitis linking configuration files
│   ├── include/                    # Vision Vision library include file for HLS C
│   ├── Makefile                    # Makefile for hardware building
│   ├── package_rtc_gen.tcl         # Vivado tcl script to package rtc_gen kernel IP to xo file
│   ├── rtc_gen_ip/                 # IP directory for rtc_gen kernel, including all the RTL source codes
│   ├── rtc_gen_kernel.xml          # Kernel desctiption file for rtc_gen kernel
│   └── strm_dump.cpp               # HLS C source for strm_dump kernel
├── README.md
├── rtc_gen                         # working directory for rtc_gen kernel development
│   ├── font_sim_data.txt           # text format font library file for RTL simulation
│   └── src/                        # RTL source code for rtc_gen kernel
└── sw/                             # Test program directory
    ├── build/                      # Software build working directory
    │   ├── font.dat                # Font library file including 11 characters
    │   ├── setup_emu.sh            # setup script for emulation mode
    │   └── xrt.ini                 # XRT configuration for emulation and debug
    ├── CMakeLists.txt              # cmake configuration file
    ├── media/                      # Media files for test program
    └── src/                        # Test program source codes
        ├── rtc_alpha_tb.cpp        # Test program for the whole design
        ├── rtc_gen_test.cpp        # Test program for rtc_gen kernel
        └── xcl2/                   # Xilinx OpenCL include files
~~~

## RTL カーネル: rtc\_gen (XO)

rtc\_gen は、Verilog HDL で記述されたリアルタイムのクロック桁の画像を生成するカーネルです。rtc\_gen には、クロック分周器付きの AXI バス クロックで駆動される内部の常時稼働リアルタイム クロックが含まれます。時間の値は、カーネル引数を使用してホストによって設定できます。カーネルはまず、0 ～ 9 桁のフォント画像ライブラリをグローバル メモリからオンチップ バッファーに読み込んでから、AXI-Stream ポートを介してリアルタイムのクロック桁の画像を出力します。ユーザーは、内部の常時稼働タイム カウンターから時間の値を読み出すこともできます。

フォント ライブラリの文字サイズは 240 (高さ) × 160 (幅) ピクセルで、11 文字 (数字 0 ～ 9 およびコロン) が含まれます。フォント ライブラリの内容については、次の図を参照してください。

<div align="center"><img src="./doc/images/font_lib.png" alt="フォント ライブラリ" ></div><br/><br/>
フォント ライブラリ内の各ピクセルは 4 ビットで表されます。これは、実際には各ピクセルの不透明度の値になります。AXI-Stream ポートを介して出力する場合、4 ビットの不透明度の値は、左にシフトして 4 ビット追加されるので、8 ビットに拡張され、さらに 15 が追加されます (つまり、0xB が 0xBF に拡張されます)。不透明度の値は、ダウンストリームのアルファ ミキシング カーネルで使用され、カラー設定を含むタイム桁が生成されます。単一の文字のフォント画像のデータ サイズは、次のとおりです。

~~~
  240 x 160 x 4 = 153,600 bits = 19,200 bytes
~~~

フォント画像ライブラリの合計サイズは、次のとおりです。

~~~
  19,200 x 11 = 211,200 bytes
~~~

rtc\_gen は、センチ秒ありの形式 (11 文字 を含む HOUR:MIN:SEC:CENTISEC) とセンチ秒なし (8 文字を含む HOUR:MIN:SEC) の 2 つの時間形式をサポートします。時間形式はカーネル引数で設定されます。2 つの出力時間形式の例は、次の図を参照してください。

<div align="center"><img src="./doc/images/time_format.png" alt="時間形式" ></div>
rtc\_gen カーネルには、次の 3 つのバス インタフェースがあります。

+ カーネル引数および制御用の AXI4-Lite スレーブ インターフェイス
+ フォント ライブラリ データ読み込み用の AXI4-Lite マスター インターフェイス
+ クロック桁の画像出力用の AXI-Stream マスター インターフェイス

カーネルは、AXI マスター ベースのフォント ライブラリ読み出し用の rtc\_gen\_axi\_read\_master と、AXI スレーブ ベースのカーネル引数と制御用の rtc\_gen\_control\_s\_axi、コア カーネル関数および AXI-Stream 出力用の rtc\_gen\_core の 3 つのブロックで構成されています。rtc\_gen\_axi\_read\_master は、Vitis/Vivado RTL Kernel Wizard で生成される標準ブロックです。rtc\_gen\_control\_s\_axi も生成されるブロックですが、時間値の読み出し機能を追加するため、いくつか変更を加える必要があります。

<div align="center"><img src="./doc/images/rtc_gen_block.png" alt="rtc_gen ブロック" ></div>
カーネルは、ホストによりトリガーされると、内部リアルタイム クロックから時間値を読み取り、その時間値に対応する時間画像のフレームを出力します。

次の表に、rtc\_gen カーネルで使用される引数を示します。

| 番号| 引数| 幅| 説明
|----------|----------|----------|----------
| 0| work\_mode| 1| \[0]: カーネルの動作モードを決定します。<br> 0 = AXI 読み出しマスターを介してグローバル メモリからオンチップ SRAM までのフォントを読み込みます。<br>1 - AXI-Stream マスターを介して RTC 桁の図を出力します。
| 1| cs\_count| 32| \[21:0]: センチ秒カウンター。たとえば、システム クロックが 200MHz の場合、cs\_count は 2,000,000 に設定する必要があります。
| 2| time\_format| 1| \[0]: センチ秒が出力桁の画像に含まれているかどうかを確認します。<br> 0 - センチ秒の出力をディスエーブルにします。<br> 1 - センチ秒の出力をディスエーブルにします。
| 3| time\_set\_val| 32| 内部フリーランニング クロックの時間値を設定します。<br> \[31:24] - 時間 <br> \[23:16] - 分 <br> \[15:8] - 秒 <br> \[7:0] - センチ秒
| 4| time\_set\_en| 1| \[0]: このビットに 1 を書き込むと、time\_set\_value が内部フリーランニング クロックに読み込まれます。
| 5| time\_val| 32| 内部リアルタイム クロック時間値の読み取り専用レジスタ: <br> \[31:24] - 時間 <br> \[23:16] - 分 <br> \[15:8] - 秒 <br> \[7:0] - センチ秒
| 6| read\_addr| 64| AXI マスター ポインター。これは、フォント ライブラリ用の FPGA デバイス バッファー アドレスです。

<br/><br/>

RTL カーネル rtc\_gen およびこの RTL カーネルを作成するための手順を追ったガイドラインの詳細は、[RTL カーネル (rtc\_gen) の作成](./doc/rtc_gen_tutorial.md)を参照してください。

## HLS C カーネル: alpha\_mix (XO)

カーネル alpha\_mix は、次のタスクを順番に実行します。

* AXI-Stream ポートを介して rtc\_gen カーネルからクロック桁の画像を受信します。
* Vitis ビジョン ライブラリのリサイズ関数を使用して、クロック桁の画像サイズを変更します。
* グローバル メモリから背景画像を読み込んで、クロック桁の画像とのアルファ ミキシングを実行します
* AXI-Stream ポートを介して混合した画像を送信します。

<div align="center"><img src="./doc/images/alpha_flow.png" alt="alpha_mix フロー" ></div>
alpha\_mix カーネルには次の 4 つのバス インタフェースがあります。

* 制御用の AXI4-Lite スレーブ インターフェイス
* 背景画像読み込み用の AXI4-Lite マスター インターフェイス
* クロック桁の画像受信用の AXI-Stream スレーブ インターフェイス
* 混合画像出力用の AXI-Stream マスター インターフェイス

次の表に、alpha\_mix カーネルで使用される引数の概要を示します。カーネルは XF\_NPPC8 モードを使用しているので、クロック サイクルごとに 8 ピクセルが処理されます。背景画像の幅とサイズ変更された時間画像の幅が 8 の整数倍であることを確認してください。整数倍でないと、カーネルが停止する可能性があります。

| 番号| 引数| 幅| 説明|
|----------|----------|----------|----------|----------
| 0| reserved| \-| \-|
| 1| bgr\_img\_input| 64| AXI マスター ポインター、入力背景画像用の FPGA デバイス バッファー|
| 2| reserved| \-| \-|
| 3| time\_img\_rows\_in| 32| 入力時間画像の高さ|
| 4| time\_img\_cols\_in| 32| 入力時間画像の幅|
| 5| time\_img\_rows\_rsz| 32| サイズ変更した時間画像の高さ|
| 6| time\_img\_cols\_rsz| 32| サイズ変更した時間画像の幅|
| 7| time\_img\_pos\_row| 32| 0 から開始する時間画像の垂直座標|
| 8| time\_img\_pos\_col| 32| 0 から開始する時間画像の水平座標。|
| 9| time\_char\_color| 32| 時間画像の色、\[RGB] にはビット範囲 \[23:0] を使用|
| 10| time\_bgr\_color| 32| 時間背景色、\[RGB] にはビット範囲 \[23:0] を使用|
| 11| time\_bgr\_opacity| 32| 時間背景の不透明度、\[7:0] を使用、値の範囲は 0 ～ 255|
| 12| bgr\_img\_rows| 32| 背景画像の高さ|
| 13| bgr\_img\_cols| 32| 背景画像の幅|

カーネル引数の意味については、次の図を参照してください。

<div align="center"><img src="./doc/images/alpha_arg.png" alt="alpha_mix カーネル引数" ></div>
HLS C カーネル alpha\_mix の詳細は、[HLS C カーネル (alpha\_mix) の作成](./doc/alpha_mix_tutorial.md)を参照してください。

## HLS C カーネル: strm\_dump (XO)

strm\_dump は、AXI4-Lite マスターを介して入力 AXI-Stream をグローバル メモリにダンプする単純な HLS カーネルです。

次の表に strm\_dump カーネルで使用される引数を示します

| 番号| 引数| 幅| 説明
|----------|----------|----------|----------
| 0| reserved| \-| \-
| 1| output\_addr| 64| AXI マスター ポインター。これは出力画像の FPGA デバイス バッファー アドレスです。
| 2| byte\_size| 32| 出力されるデータ量 (バイト単位)。これは、時間形式と色の深さに基づいて計算できます。

## ビットストリーム インプリメンテーション (XCLBIN)

### rtc\_gen\_test\_hw.xclbin / rtc\_gen\_test\_hw\_emu.xclbin

これは rtc\_gen カーネル用の単純なテスト システムで、AXI-Stream バスを使用して接続された rtc\_gen と strm\_dump の 2 つのカーネルを統合します。次の U50 プラットフォームの接続図を参照してください。ビルド ターゲット (hw または hw\_emu) によって、2 つの XCLBIN ファイルが生成されます。

<div align="center"><img src="./doc/images/rtc_gen_test_diagram.png" alt="rtc_gen_test の図" ></div>
### rtc\_alpha\_hw.xclbin / rtc\_alpha\_hw\_emu.xclbin

これは完全にインプリメント済みのシステムで、3 つのカーネル (rtc\_gen、alpha\_mix および strm\_dump) をすべて統合したもので、AXI-Stream バスを介して接続されます。strm\_dump カーネルの関数は、alpha\_mix カーネルに簡単に統合できることに注意してください。ここでは、カーネル間の AXI-Stream 接続機能を示すために、strm\_dump カーネルを分けました。U50 プラットフォームの次の接続図を参照してください。ビルド ターゲット (hw または hw\_emu) によって、2 つの XCLBIN ファイルが生成されます。

<div align="center"><img src="./doc/images/rtc_alpha_diagram.png" alt="rtc_alpha の図" ></div>
## テスト プログラム

### rtc\_gen\_test.cpp

このプログラムは、まず環境変数 XCL\_EMULATION\_MODE に従って実行モードを判断し、次にバイナリ ファイル rtc\_gen\_test\_hw.xclbin または rtc\_gen\_test\_hw\_emu.xclbin を選択して、RTL カーネル rtc\_gen のテストを終了します。8 桁と 11 桁のクロック形式の両方をテストし、生成されたクロック画像を直接表示します。また、このプログラムは XRT 下位レベルの API である xclRegRead を使用して、rtc\_gen カーネルの time\_val レジスタの値、つまり内部ハードウェア タイム カウンターの値を読み取り、表示します。time\_val の値は、画像表示の更新を制御するためにも使用されます。xclRegRead 関数が正しく動作するようにするには、実行ディレクトリで xrt.ini ファイルを作成または変更して、次の行を追加します。

~~~
[Runtime]
exclusive_cu_context=true
~~~

### rtc\_alpha\_tb.cpp

このプログラムは、まず環境変数 XCL\_EMULATION\_MODE に従って実行モードを判断し、次にバイナリ ファイル rtc\_alpha\_hw.xclbin または rtc\_alpha\_hw\_emu.xclbin を選択して、生成されたリアルタイム クロック画像を背景画像に混合します。ユーザーはコマンド パラメーターを使用して、背景画像を選択したり、時間形式を設定したり、クロック時間の設定できます。また、プログラムのソースコードを変更して、クロック画像の色、サイズ、位置を変更することもできます。このテスト プログラムは、xclRegRead API を使用して rtc\_gen カーネルの time\_val レジスタの値を読み取り、その値を使用して画像表示の更新を制御します。

<br/>
## このリポジトリの使用方法

次の手順を実行する前に、XRT および Vitis 設定ファイルを必ず読み込んでください。

~~~
source /opt/xilinx/xrt/setup.sh
source /opt/xilinx/Vitis/2020.2/settings64.sh
~~~

2 つのテスト プログラムが画像を表示する必要があります。そのため、リモート サーバーを使用している場合は、VNC デスクトップを使用するか、X11 転送とローカル X11 サーバーを使用した ssh 接続を使用してください。

### ハードウェアのビルド

./hw ディレクトリに移動し、**make** コマンドを使用して 3 つの XO ファイルと XCLBIN ファイルのビルドを終了します。使用可能なすべての make コマンド オプションは、次のとおりです。

~~~
make
      Display help information

make all TARGET=<hw/hw_emu> PLATFORM=<FPGA PLATFORM>
      Command to build all the rtc_gen_test and rtc_alpha xclbin and necessary kernel files (xo) for  
      specified target and platform. By default, TARGET=hw, PLATFORM=xilinx_u200_gen3x16_xdma_1_1_202020_1

make all_xo TARGET=<hw/hw_emu> PLATFORM=<FPGA PLATFORM>
      Command to build all the kernel files (xo), including rtc_gen.xo, alpha_mix.xo and strm_dump.xo
      By default, TARGET=hw, PLATFORM=xilinx_u200_gen3x16_xdma_1_1_202020_1

make clean
      Command to remove all the generated files.
~~~

make コマンド オプションでは、TARGET は hw または hw\_emu にします。rtc\_gen カーネルはソフトウェア エミュレーション モデルを提供しないためsw\_emu モードは使用できません。TARGET が hw の場合、XCLBIN ファイルと XO ファイルには \_hw が最後に付き、TARGET が hw\_emu の場合は \_hw\_emu が付きます。RTL カーネル rtc\_gen は、hw または hw\_emu オプションの影響を受けないので、XO ファイル rtc\_gen.xo のみが存在することに注意してください。

PLATFORM は、xilinx\_u200\_gen3x16\_xdma\_1\_1\_202020\_1、xilinx\_u200\_xdma\_201830\_2、xilinx\_u250\_gen3x16\_xdma\_3\_1\_202020\_1、xilinx\_u250\_xdma\_201830\_2、xilinx\_u50\_gen3x16\_xdma\_201920\_3 および xilinx\_u280\_xdma\_201920\_3 の 6 つのいずれかになります。これらの Alveo カードがインストールされているかどうかにかかわらず、システムに開発プラットフォーム パッケージ (deb または rpm パッケージ) をインストールしていれば、プラットフォームをビルド プラットフォームとして使用できます。/opt/xinxi/platform ディレクトリを参照するか、またはコマンド platforminfo -l を使用すると、どのプラットフォームがインストールされているか確認できます。最終的に生成された XCLBIN ファイルと XO ファイルは、make コマンドが正常に実行された後、./hw ディレクトリに保存されます。

たとえば、すべての XO ファイルと XCLBIN ファイルを U50 カードを使用してハードウェア エミュレーション モードでビルドする場合は、次のように入力します。

~~~
make all TARGET=hw_emu PLATFORM=xilinx_u50_gen3x16_xdma_201920_3
~~~

ハードウェア ターゲット用の XCLBIN ファイルのビルドには時間がかかるので、時間を節約するために、サポートされる各プラットフォーム用に事前にビルドされた XCLBIN ファイル (rtc\_gen\_test\_hw.xclbin および rtc\_alpha\_hw.xclbin) も用意しています。これらは target=hw オプションでビルドされているので、hw\_emu モードでは使用できません。hw\_emu ターゲットの XCLBIN ファイルの場合は、ビルドとシステム依存性が非常に速いので、ご自身でビルドしてください。

プリビルドされた XCLBIN ファイルは、次のリンクからダウンロードできます。 <https://japan.xilinx.com/bin/public/openDownload?filename=rtl_stream_kernel_xclbin_2020.2.tgz>

**プリビルドされた xclbin ファイルを使用するには、ターゲット プラットフォームに該当する 2 つの xclbin ファイルを ./hw ディレクトリにコピーしてください。このディレクトリは、ダウンストリーム手順で直接使用されます。**

### ソフトウェアのビルドと実行

* 手順 1: makefileを生成

./sw/debuild ディレクトリに移動し、**cmake ..** または **cmake3 ..** コマンドを入力します。これにより、ソフトウェア ビルド用の makefile が生成され、./hw ディレクトリ内の 2 つの XCLBIN ファイルが ./sw/build ディレクトリにリンクされます。

~~~
cd ./sw/build
cmake ..
~~~

* 手順 2: プログラムをコンパイル

**make** コマンドを入力すると、2 つの C++ プログラムがコンパイルされます。これにより、rtc\_gen\_test と rtc\_alpha\_tb の 2 つの実行可能ファイルが生成されます。

~~~
make
~~~

テスト プログラムでは XRT の下位レベル API の xclRegRead が使用されるため、ハードウェア モードとハードウェア エミュレーション モードには異なるリンク ライブラリ セットがあります。コンパイルが問題なく終了すると、rtc\_alpha\_tb、rtc\_alpha\_tb\_emu、rtc\_gen\_test、rtc\_gen\_test\_emu の 4 つの実行可能ファイルが生成されます。ハードウェアまたはハードウェア エミュレーション モードには、正しい実行可能ファイルを使用してください。

* 手順 3: 実行モード (ハードウェアまたはハードウェア エミュレーション) を設定

実行モードを設定するスクリプト setup\_emu.sh は が提供されています。

**ハードウェア モードで実行**

エミュレーション モードを入力しなかった場合は、rtc\_gen\_test および rtc\_alpha\_ab の実行ファイルを実行して、ハードウェア モードで実行します。ハードウェア エミュレーションモードを入力し、実際のハードウェア モードに戻る場合は、次のコマンドを使用してから、実行ファイルを実行します。

~~~
source setup_emu.sh -s off
~~~

**ハードウェア エミュレーション モードで実行**

テスト プログラムをハードウェア エミュレーション モードで試すには、rtc\_gen\_test\_emu および rtc\_alpha\_tb\_emu の実行可能ファイルを使用する必要があります。これらを実行する前に、まず次のコマンドを実行してください。

~~~
source setup_emu.sh -s on -p PLATFORM_NAME
~~~

PLATFORM\_NAME は、サポートされている 6 つのプラットフォームの 1 つです。次のコマンドを実行すると、ヘルプ情報を取得できます。

~~~
source setup_emu.sh
~~~

たとえば、U50 プラットフォームで実行ファイルをハードウェア エミュレーション モードで実行する場合は、次のように入力します。

~~~
source setup_emu.sh -s on -p xilinx_u50_gen3x16_xdma_201920_3
~~~

setup\_emu.sh は、必要なコンフィギュレーション ファイルを生成し、環境を設定します。

**注記:** ここで入力する platform\_name は、./sw/duild ディレクトリ内の XCLBIN ファイルと同じである必要があります。

このデザイン例のハードウェア エミュレーションの詳細は、[エミュレーション チュートリアル](./doc/hw_emu_tutorial.md)を参照してください。

* 手順 4: 実行可能ファイル **rtc\_gen\_test** または **rtc\_gen\_test\_emu** を実行

実行可能な rtc\_gen\_test または rtc\_gen\_test\_emu を実行して、ハードウェアまたはハードウェア エミュレーション モードで実行中のプログラムを終了します。最初に 8 桁のクロックが表示されます。画像ウィンドウを手前にして、Esc キーを押すと、2 つ目の 11 桁のクロックが表示されます。画像ウィンドウを手前にしたまま、もう一度 Esc キーを押してプログラムを終了します。また、カーネルの time\_val レジスタの値も読み出して表示します。**rtc\_gen\_test\_emu を実行する前に、必ず実行モードをハードウェア エミュレーションに設定してください。**

~~~
./rtc_gen_test

or

./rtc_gen_test_emu
~~~

プログラムはまず実行モード (hw または hw\_emu) を判別し、次に ./sw/build/rtc\_gen\_test\_hw.xclbin または .sw/build/rtc\_gen\_test\_hw\_emu.xclbin ファイルを探して解析し、使用しているプラットフォームを取得してから、インストールしたカードと比較します。不一致が検出されると、エラー情報をレポートして、プログラムを終了します。

**注記:** ハードウェア エミュレーション モードで実行すると、RTL シミュレーションが実際に実行されるため、時間がかかることがあります。

* 手順 5: 実行ファイル rtc\_alpha\_tb または rtc\_alpha\_tb\_emu を実行

実行ファイルrtc\_alpha\_tb または rtc\_alpha\_tb\_emu を実行し、ハードウェアまたはハードウェア エミュレーション モードで実行中のプログラムを終了します。実行ファイルにはいくつかのコマンド パラメーターがあります。使用方法は次のとおりです。

~~~
rtc_alpha_tb [-i BACK_IMAGE] [-f] [-s] [-h]

  -i BACK_IMAGE: set path to the background image, default is ../media/alveo.jpg
  -f : set to use eleven-digit clock, default is eight-digit
  -s : use system time to set the clock, default don't set the clock
  -h : print help information

~~~

./sw/media ディレクトリには、alveo.jpg、vitis.jpg、victor.jpg の 3 つの画像が含まれますが、ほかの画像も使用できます。画像は 3 チャンネル形式 (透明度なしの RGB) である必要があります。また、十分な大きさの画像を使用する必要があります。そうしない場合は、プログラムのソース コードを変更して、クロック画像のサイズまたは位置を調整してください。

次に、実行コマンド ラインの例を示します。

~~~
rtc_alpha_tb
      Mix the clock image with ../media/alveo.jpg and display, don't sync the kernel internal real-time-clock
      with Linux system clock, and use 8-digit format.

rtc_alpha_tb -i ../media/vitis.jpg -f -s
      Mix the clock image with ../media/vitis.jpg and display, sync the kernel internal real-time-clock with
      Linux system clock, and use 11-digit format.
~~~

プログラムを終了するには、画像ウィンドウを手前にして、**Esc** キーを押します。

プログラムはまず実行モード (hw または hw\_emu) を判別し、次に ./sw/build/rtc\_alpha\_hw.xclbin または .sw/build/rtc\_alpha\_hw\_emu.xclbin ファイルを探して解析し、使用しているプラットフォームを取得してから、インストールしたカードと比較します。不一致が検出されると、エラー情報をレポートして、プログラムを終了します。

./sw/src/rtc\_alpha\_tb.cpp ファイルの先頭にある #define セクションを変更して、クロック画像の色、サイズ、位置、不透明度を調整したら、**手順 2** を繰り返してプログラムを再コンパイルおよび実行して、結果を確認できます。背景画像の幅とサイズ変更されたクロック画像は、必ず 8 の整数倍にしてください。

~~~c++
// position of clock image, top-left corner is (0,0)
#define RTC_POSITION_ROW 64
#define RTC_POSITION_COL 400

// resized clock image size for 8-digit font digit size
// ensure RTC_IMG_WIDTH is integer multiple of 8
#define RTC_IMG_WIDTH_8D 480
#define RTC_IMG_HEIGHT_8D 90

// resized clock image size for 11-digit font digit size
// ensure RTC_IMG_WIDTH is integer multiple of 8
#define RTC_IMG_WIDTH_11D 528
#define RTC_IMG_HEIGHT_11D 72

// clock image font color
#define FONT_COLOR_R 255
#define FONT_COLOR_G 255
#define FONT_COLOR_B 255

// clock image background color
#define BGR_COLOR_R 80
#define BGR_COLOR_G 80
#define BGR_COLOR_B 80

// clock image background opacity
#define BGR_OPA 100
~~~

**注記:** ハードウェア エミュレーション モードで実行すると、RTL シミュレーションが実際に実行されるため、時間がかかることがあります。より小さな背景画像を使用すると、実行時間を短縮できます。この場合は、上記で説明したサイズと位置のパラメーターを変更してください。

* 手順 6: rtc\_gen\_test および rtc\_alpha\_tb プログラムを使用して、Vitis プロファイリング関数を試します。

Vitis は、パフォーマンス、帯域幅の使用状況、デザインのボトルネックなどを詳細に把握できる強力なプロファイリング機能を備えています。詳細は、[アプリケーションのプロファイリング](./doc/profile_tutorial.md)を参照してください。

---------------------------------------


<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align= center class="sphinxhide"><b><a href="../../../README.md">メイン ページに戻る</a> &mdash; <a href="../../README.md/">ハードウェア アクセラレータ チュートリアルの初めに戻る</a></b></p></br>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
