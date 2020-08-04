<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>RTL カーネル入門</h1>
 </td>
 </tr>
</table>

# 概要

このチュートリアルでは、RTL IP をアクセラレーション カーネルとしてパッケージして、Vitis コア開発キットで使用する方法を 4 つの手順で説明します。

1. Vivado™ Design Suite IP として RTL ブロックをパッケージ。
2. カーネルを記述した XML ファイルを作成。
3. RTL カーネルをザイリンクス オブジェクト (XO) ファイルにパッケージ。
4. Vitis 統合設計環境 (IDE) を使用して RTL カーネルを FPGA にプログラムし、ハードウェアまたはハードウェア エミュレーションで実行。

# チュートリアルの概要

特定のソフトウェアおよびハードウェア インターフェイス要件に従った RTL デザインは、ザイリンクス オブジェクト (XO) ファイルにパッケージできます。このファイルをバイナリ コンテナーにリンクすると、xclbin ファイル (ホスト アプリケーションがカーネルを FPGA にプログラムする際に使用) を作成できます。

このチュートリアルには、次のリファレンスファイルが含まれます。

- `B[i] = A[i]+B[i]` 演算を実行する単純なベクター累算の例。

- ホスト アプリケーション (カーネルとの通信には OpenCL API を使用):

  - ホストが読み出し/書き込みバッファーを作成して、ホストと FPGA 間のデータを転送。
  - ホストは RTL カーネルをエンキュー (FPGA で実行)。具体的には、DDR のバッファーが読み込まれ、`B[i] = A[i]+B[i]` が実行され、結果が DDR に書き戻されます。
  - ホストがデータを読み戻して、結果を比較。

これらのリファレンス ファイルを使用して、Vitis™ IDE プロジェクトを作成する最初の手順から、プロジェクトをビルドして実行する最後の手順まで説明します。

# 開始前の注意点

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド
* 2019.2 Vitis コア開発キット リリースおよび xilinx\_u200\_xdma\_201830\_2 プラットフォーム。  
必要であれば、その他のバージョンおよびプラットフォームも使用できます。

> **重要:**
>
> * 例を実行する前に、[インストール](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/vhc1571429852245.html)に示すように Vitis コア開発キットをインストールしておく必要があります。
> * ザイリンクス Alveo™ データセンター アクセラレータ カードでアプリケーションを実行する場合は、『Alveo データセンター アクセラレータ カード入門ガイド』 ([UG1301](https://japan.xilinx.com/support/documentation/boards_and_kits/accelerator-cards/ug1301-getting-started-guide-alveo-accelerator-cards.pdf)) で説明されるように、カードとソフトウェア ドライバーを正しくインストールしてください。

## チュートリアル リファレンス ファイルの入手

1. リファレンス ファイルを入手するには、ターミナルに `git clone https://github.com/Xilinx/Vitis-Tutorials` と入力します。
2. `getting-started-rtl-kernels` ディレクトリに移動し、`reference-files` ディレクトリにアクセスします。

# RTL カーネルとして RTL デザインを使用するための要件

Vitis IDE 内で RTL カーネルを使用するには、Vitis コア開発キット実行モデルの要件とハードウェア インターフェイス要件の両方を満たす必要があります。

## カーネルの実行モデル

RTL カーネルは、C/C++ カーネルと同じソフトウェア インターフェイスおよび実行モデルを使用します。RTL カーネルは、ホスト アプリケーションでは void 戻り値、スカラー引数、およびポインター引数を持つ関数として認識されます。次はその例です。

```C
void vadd_A_B(int *a, int *b, int scalar)
```

この例からは、RTL カーネルの実行モデルがソフトウェア関数のものと類似していることがわかります。

- 呼び出されたときに開始。
- 必要な結果を処理。
- 処理が終了したら、通知を送信。

Vitis コア開発キット実行モデルは、特に次の機能および条件に従っている必要があります。

- スカラー引数は AXI4-Lite スレーブ インターフェイスを介してカーネルに渡されます。
- ポインター引数はグローバル メモリ (DDR、HBM または PLRAM) を介して転送されます。
- ポインター引数のベース アドレスは、その AXI4-Lite スレーブ インターフェイスを介してカーネルに渡されます。
- カーネルは 1 つまたは複数の AXI4 マスター インターフェイスを介して、グローバル メモリ内のポインター引数にアクセスします。
- カーネルは AXI4-Lite インターフェイスを介してホスト アプリケーションにより開始されます。
- カーネルは演算が終了したら、AXI4-Lite インターフェイスまたは特別な割り込み信号を介してホスト アプリケーションに通知する必要があります。

## ハードウェア インターフェイス要件

この実行モデルを使用するには、次の特定のハードウェア インターフェイス要件を満たしている必要があります。

- プログラマブル レジスタ (制御レジスタ、スカラー引数、およびポインターのベース アドレス) へのアクセスに使用するインターフェイスは AXI4-Lite スレーブ インターフェイス 1 つのみ。

  - オフセット `0x00` - 制御レジスタ: カーネル ステータスを制御および表示。
    - ビット `0`: **start** 信号: カーネルがデータ処理を開始できるようになるとホスト アプリケーションによりアサート。**done** 信号がアサートされるとクリアされる必要あり。
    - ビット `1`: **done** 信号: 演算を終了したらアサート。読み出しでクリア。
    - ビット `2`: **idle** 信号: どのデータも処理されていない場合にアサート。**done** 信号がアサートされるのと同時に Low から High に遷移。
  - オフセット `0x04`: グローバル割り込みイネーブル レジスタ - ホストへの割り込みをイネーブルにするために使用。
  - オフセット `0x08`: IP 割り込みイネーブル レジスタ - 割り込みを生成するのに使用する IP 生成信号を制御。
  - オフセット `0x0C`- IP 割り込みステータス レジスタ: 割り込みステータスを提供。
  - オフセット `0x10` およびそれ以上 - カーネル引数レジスタ: スカラー パラメーターおよびポインターのベース パラメーター用のレジスタ。

- 次のインターフェイスの 1 つまたは複数:

  - グローバル メモリと通信するための AXI4 マスター インターフェイス。
    - AXI4 マスター インターフェイスにはすべて 64 ビット アドレスが必要です。
    - グローバル メモリ空間の分割はカーネル開発者が実行します。グローバル メモリの各パーティションがカーネル引数になります。各パーティションのベース アドレス (メモリ オフセット) は、AXI4-Lite スレーブ インターフェイスを介してプログラム可能な制御レジスタにより設定されます。
    - AXI4 マスターには WRAP または FIXED タイプのバースト、またはサイズが満たない (サブサイズ) バーストは使用できませんので、AxSIZE は AXI データ バスの幅と同じである必要があります。
    - 上記の要件を満たさないユーザー ロジックまたは RTL コードは、ラップしておくかブリッジさせておく必要があります。
  - その他のカーネルとの通信用の AXI4-Stream インターフェイス。

変更前の RTL デザインが別の実行モデルまたはハードウェア インターフェイスを使用する場合、デザインがこの方法で実行できて、インターフェイス要件に従うように、ロジックを追加する必要があります。

### その他の情報

Vitis コア開発キット カーネルのインターフェイス要件の詳細は、[RTL カーネルの要件](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/Chunk851038901.html?hl=requirements%2Crtl#qbh1504034323531)を参照してください。

## Vector-Accumulate RTL IP

`B[i]=A[i]+B[i]` を実行する Vector-Accumulate RTL IP は、上記の要件すべてを満たしているほか、次のような特性を持っています。

- 2 つの AXI4 メモリ マップド インターフェイス:
  - A を読み出すインターフェイス
  - B を読み出して書き込むインターフェイス
  - このデザインで使用される AXI4 マスターは WRAP、FIXED、または NARROW バースト タイプを使用しません。
- 1 つの AXI4-Lite スレーブ制御インターフェイス:
  - 制御レジスタはオフセット `0x00`
  - カーネル引数レジスタをオフセット `0x10` にすると、ホストがスカラー値をカーネルに渡すことができます。
  - カーネル引数レジスタをオフセット `0x18` にすると、ホストがグローバル メモリの A のベース アドレスをカーネルに渡すことができます。
  - カーネル引数レジスタをオフセット `0x24` にすると、ホストがグローバル メモリの B のベース アドレスをカーネルに渡すことができます。

これらは RTL カーネル ウィザードへの入力に使用されます。RTL カーネル ウィザードはこの情報を使用して次を生成します。

- RTL デザインを Vitis コア開発キット カーネルの XO ファイルとしてパッケージするのに必要な XML ファイル
- `A[i] = A[i] + 1` を実行するサンプル カーネル (RTL コード、テストベンチ、およびホスト コード)
- カーネル用の Vivado Design Suite プロジェクト

チュートリアルの後半部分では、このサンプル カーネルを既存の Vector-Accumulate デザインに置き換えて、XO ファイルとしてパッケージします。

# Vitis IDE プロジェクトの作成

1. Linux ターミナル ウィンドウで `vitis` コマンドを入力して Vitis IDE を起動します。  
\[Workspace Launcher] ダイアログ ボックスが表示されます。  
![error: missing image](images/192_vitis_launcher.png)

2. ワークスペースのプロジェクト ディレクトリを選択します。

3. **\[Launch]** をクリックします。  
Vitis IDE が開きます。

4. メニュー バーから **\[File]** → **\[New]** → **\[Application Project]** をクリックします。  
\[New Vitis Application Project] ウィンドウが開きます。  
![error: missing image](images/192_vitis_newapplicationproject.png)

5. Vitis IDE プロジェクトの新規作成:

   1. `rtl_ke_t2` などのプロジェクト名を入力します。
   2. **\[Use default location]** をオンにします。
   3. **\[Next]** をクリックします。  
\[Platform] ページが表示されます。表示されるプラットフォームのリストは、システム設定によって異なります。  
![error: missing image](./images/192_vitis_hardware_platform_dialog.png)

6. `xilinx_u200_xdma_201830_2` を選んで、**\[Next]** をクリックします。  
\[Templates] ページが開き、Vitis IDE プロジェクトの作成に使用可能なテンプレートがリストされます。ほかの Vitis IDE サンプルをダウンロードしていない場合は、\[Empty Application] と \[Vector Addition] のみが使用可能なテンプレートとして表示されます。  
![error: missing image](./images/192_vitis_example_empty.png)

   選択したプラットフォームによって、プロジェクトが Vitis IDE プロジェクトとして定義されます。

7. **\[Empty Application]** を選択し、**\[Finish]** をクリックします。  
ウィザードが閉じて、Vitis IDE のメイン ウィンドウに戻ります。

8. Vitis IDE の一番上のメニュー バーから **\[Xilinx]** → **\[RTL Kernel Wizard]** をクリックして RTL Kernel ウィザードを起動します。

   次のセクションでは、RTL Kernel ウィザードを設定して RTL カーネルを作成します。

# RTL Kernel ウィザードの設定

RTL Kernel ウィザードでは、RTL カーネルのインターフェイス機能を指定できます。このウィザードを使用すると、Vitis IDE でシステムに統合可能な有効なカーネルに RTL IP をパッケージできます。また、RTL IP をカーネルにパッケージするために必要なタスクがいくつか自動化することもできます。

## \[General Settings] ページ

- **\[Kernel Identification]**: IP の VLNV (Vendor:Library:Name:Version) とも呼ばれるベンダー、カーネル名、およびライブラリなどを指定します。カーネル名は、RTL カーネルに使用する IP の最上位モジュール名と同じにする必要があります。
- **\[Kernel Options]**: デザイン タイプを指定します。
  - **\[RTL]** (デフォルト): 関連ファイルを Verilog 形式で生成します。
  - **\[Block design]**: Vivado ツールの IP インテグレーター用のブロック デザインを生成します。ブロック デザインには、制御レジスタをエミュレートするため、ブロック RAM 交換メモリを使用する MicroBlaze™ サブシステムが含まれます。
- **\[Clock and Reset Options]**: カーネルで使用されるクロック数およびカーネルに最上位リセット ポートが必要かどうかを指定します。

1. \[Kernel Identification] には、カーネル名を `Vadd_A_B` と指定します。

2. 残りのオプションはデフォルトのままにして、**\[Next]** をクリックします。  
![Missing Image: P1.PNG](./images/192_vitis_P1.PNG)

## \[Scalars] ページ

スカラー引数は、ホスト アプリケーションからカーネルへ入力パラメーターを渡すために使用されます。指定した \[Number of scalar kernel input arguments] の数に応じて、ソフトウェアからハードウェアに引数を渡すための制御レジスタが作成されます。各引数には ID 値が付けられ、ホスト アプリケーションからその引数にアクセスするために使用されます。この ID 値は、ウィザードのサマリ ページに表示されます。

- **\[Argument name]**: 引数の名前。
- **\[Argument type]**: ネイティブ C/C++ データ型として記述されるスカラー引数型。たとえば、(u)char、(u)short、(u)int などがあります。

1. デフォルト値のままにして、**\[Next]** をクリックします。  
![scalar.PNG](./images/scalar.PNG)

## \[Global Memory] ページ

グローバル メモリは、ホストとカーネル間、カーネルとその他のカーネル間で大型のデータ セットを渡すため使用されます。このメモリには、カーネルから AXI4 マスター インターフェイスを介してアクセスできます。AXI4 マスター インターフェイスごとに、インターフェイス名、データ幅、関連する引数の数を指定できます。

- **\[Number of AXI master interfaces]**: カーネルの AXI インターフェイス数を指定します。

- **\[AXI master definition]**: インターフェイス名、データ幅 (バイト)、各 AXI インターフェイスの引数の数を指定します。

- **\[Argument definition]**: 各 AXI インターフェイスに割り当てるポインター引数を指定します。各引数には ID 値が付けられ、ホスト アプリケーションからその引数にアクセスするために使用されます。この ID 値の割り当ては、ウィザードのサマリ ページに表示されます。

  ![Missing Image: maxi.PNG](./images/maxi.PNG)

1. Vector-Accumulate カーネルには 2 つの AXI4 インターフェイスがあるので、\[Number of AXI master interfaces] は **2** にします。

2. \[AXI master definition] セクションは次のように設定します。

   1. インターフェイス名は変更しません。
   2. 幅は変更しません。
   3. AXI4 インターフェイスはそれぞれ 1 つのポインター引数専用なので、\[Number of arguments] は **1** にします。

3. \[Argument definition] セクションの \[Argument name] は次のように設定します。

   1. \[m00\_axi] には `A` と入力します。  
データセット A にはこの AXI4 インターフェイスを介してアクセスされます。

   2. \[m01\_axi] には `B` と入力します。  
データセット B にはこの AXI4 インターフェイスを介してアクセスされます。

      設定は、上記のスクリーン キャプチャのようになるはずです。

4. **\[Next]** をクリックします。

## \[Streaming Interfaces] ページ

このデザインには、デザイン ストリーミング インターフェイスは必要ありません。デフォルト値のままにして、**\[Next]** をクリックします。

## \[Summary] ページ

![Missing Image: summary.PNG](./images/192_vitis_summary.PNG)

- **\[Target platform]**: RTL カーネルがコンパイルされるプラットフォームを表示します。別のプラットフォームがサポートされるようにするには、RTL カーネルをリコンパイルする必要があります。
- **\[Function prototype]**: カーネル呼び出しが C 関数でどのように記述されるかを示します。
- **\[Register map]**: ホスト ソフトウェア ID、引数名、ハードウェア レジスタ オフセット、データ型、関連する AXI インターフェイス間の関係を表示します。

1. カーネルを生成する前に、この \[Summary] ページを確認してください。

   RTL カーネル ウィザードは、さまざまな段階で指定したものを \[Summary] ページにまとめて、次を生成します。

   - カーネルを記述した XML ファイル。

     - [kernel.xml](./reference-files/src/xml/kernel.xml) ファイルには、レジスタ マップなど、ウィザードで定義したランタイムおよび Vitis コア開発キットフローで必要となる属性が指定されます。

   - 次を含む、`A[i]=A[i]+1` をインプリメントする VADD というサンプル カーネル。

     - RTL コード
     - 検証テストベンチ
     - ホスト コード
     - VADD サンプルカーネル用の Vivado Design Suite プロジェクト

2. **\[OK]** をクリックし、Vivado Design Suite を起動して RTL IP をパッケージしてカーネルを作成します。

# Vivado Design Suite — RTL デザイン

Vivado Design Suite IDE が開くと、\[Sources] ウィンドウには RTL カーネル ウィザードで自動的に生成されたソース ファイルが表示されます。

## サンプル ファイルの削除

デザイン ファイルをプロジェクトに追加する前に、RTL Kernel ウィザードで生成されたサンプル デザインを削除する必要があります。

1. Vivado Design Suite IDE の \[Sources] ウィンドウで **\[Compile Order]** → **\[Synthesis]** をクリックし、\[Design Sources] ツリーを展開表示します。

2. 8 つすべてのソース ファイルを選択して右クリックし、**\[Remove File from Project]** をクリックします。

3. \[Remove Sources] ダイアログ ボックスで **\[OK]** をクリックして、プロジェクトからファイルを削除します。

   \[Invalid Top Module] ダイアログ ボックスが開きます。

4. **\[OK]** をクリックします。

   > **注記**: RTL カーネル プロジェクトから自動生成された RTL ファイルを削除したら、ユーザー自身の RTL IP ファイルをプロジェクトに戻すことができるようになります。このチュートリアルでは RTL IP ファイルは提供済みですが、実際にはご自身の RTL IP ファイルとサポートされるファイル階層を挿入してください。

5. 同じウィンドウでシミュレーションするソースを変更して、`Vadd_A_B_tb.sv` ファイルのみを削除します。

6. 手順 3 と 4 を繰り返します。

## プロジェクトへの RTL ソースの追加

1. **\[Design Sources]** を右クリックし、**\[Add Sources]** をクリックします。  
\[Add Sources] ウィンドウが開きますます。

2. **\[Add or create design sources]** をクリックし、**\[Next]** をクリックします。

3. **\[Add Directories]** をクリックし、`reference-files` の `src` ディレクトリの `IP` ディレクトリ (RTL ソースを含む) を指定します。

   > **注記:** ユーザーの RTL IP を追加するには、その必要なフォルダーまたはファイルを指定します。

4. **\[Add Files]** をクリックし、`testbench` に含まれる **\[Vadd\_A\_B\_tb.sv]** を選択します。  
![Missing Image: add\_design\_sources.PNG](./images/add_design_sources.PNG)

5. **\[Finish]** をクリックして、現在のプロジェクトにファイルを追加します。

6. プロジェクトの階層を確認するには、\[Sources] ウィンドウで **\[Hierarchy]** タブをクリックします。

   > **重要:** テストベンチが最上位デザイン ファイルとして選択されています。テストベンチは IP を含むので技術的には問題ありませんが、テストベンチは RTL カーネルの-最上位にするべきではありません。

7. **\[Vadd\_A\_B\_tb.sv]** を右クリックし、**\[Move to Simulation Sources]** をクリックします。

   これでテストベンチがシミュレーションで使用できるように定義され、Vivado Design Suite で `Vadd_A_B.v` がデザインの新しい最上位ファイルとして認識されるようになります。この RTL IP には、Vitis コア開発キット内で RTL カーネルの要件と互換性のあるインターフェイスが含まれます。  
次のコードに示すように、これは module `Vadd_A_B` 定義部分に表示されます。

   ```verilog
   module Vadd_A_B #(
     parameter integer C_S_AXI_CONTROL_ADDR_WIDTH = 12 ,
     parameter integer C_S_AXI_CONTROL_DATA_WIDTH = 32 ,
     parameter integer C_M00_AXI_ADDR_WIDTH       = 64 ,
     parameter integer C_M00_AXI_DATA_WIDTH       = 512,
     parameter integer C_M01_AXI_ADDR_WIDTH       = 64 ,
     parameter integer C_M01_AXI_DATA_WIDTH       = 512
   )
   (
     input  wire                                    ap_clk               ,
     output wire                                    m00_axi_awvalid      ,
     input  wire                                    m00_axi_awready      ,
     output wire [C_M00_AXI_ADDR_WIDTH-1:0]         m00_axi_awaddr       ,
     output wire [8-1:0]                            m00_axi_awlen        ,
     output wire                                    m00_axi_wvalid       ,
     input  wire                                    m00_axi_wready       ,
     output wire [C_M00_AXI_DATA_WIDTH-1:0]         m00_axi_wdata        ,
     output wire [C_M00_AXI_DATA_WIDTH/8-1:0]       m00_axi_wstrb        ,
     output wire                                    m00_axi_wlast        ,
     input  wire                                    m00_axi_bvalid       ,
     output wire                                    m00_axi_bready       ,
     output wire                                    m00_axi_arvalid      ,
     input  wire                                    m00_axi_arready      ,
     output wire [C_M00_AXI_ADDR_WIDTH-1:0]         m00_axi_araddr       ,
     output wire [8-1:0]                            m00_axi_arlen        ,
     input  wire                                    m00_axi_rvalid       ,
     output wire                                    m00_axi_rready       ,
     input  wire [C_M00_AXI_DATA_WIDTH-1:0]         m00_axi_rdata        ,
     input  wire                                    m00_axi_rlast        ,
     // AXI4 master interface m01_axi
     output wire                                    m01_axi_awvalid      ,
     input  wire                                    m01_axi_awready      ,
     output wire [C_M01_AXI_ADDR_WIDTH-1:0]         m01_axi_awaddr       ,
     output wire [8-1:0]                            m01_axi_awlen        ,
     output wire                                    m01_axi_wvalid       ,
     input  wire                                    m01_axi_wready       ,
     output wire [C_M01_AXI_DATA_WIDTH-1:0]         m01_axi_wdata        ,
     output wire [C_M01_AXI_DATA_WIDTH/8-1:0]       m01_axi_wstrb        ,
     output wire                                    m01_axi_wlast        ,
     input  wire                                    m01_axi_bvalid       ,
     output wire                                    m01_axi_bready       ,
     output wire                                    m01_axi_arvalid      ,
     input  wire                                    m01_axi_arready      ,
     output wire [C_M01_AXI_ADDR_WIDTH-1:0]         m01_axi_araddr       ,
     output wire [8-1:0]                            m01_axi_arlen        ,
     input  wire                                    m01_axi_rvalid       ,
     output wire                                    m01_axi_rready       ,
     input  wire [C_M01_AXI_DATA_WIDTH-1:0]         m01_axi_rdata        ,
     input  wire                                    m01_axi_rlast        ,
     // AXI4-Lite slave interface
     input  wire                                    s_axi_control_awvalid,
     output wire                                    s_axi_control_awready,
     input  wire [C_S_AXI_CONTROL_ADDR_WIDTH-1:0]   s_axi_control_awaddr ,
     input  wire                                    s_axi_control_wvalid ,
     output wire                                    s_axi_control_wready ,
     input  wire [C_S_AXI_CONTROL_DATA_WIDTH-1:0]   s_axi_control_wdata  ,
     input  wire [C_S_AXI_CONTROL_DATA_WIDTH/8-1:0] s_axi_control_wstrb  ,
    input  wire                                    s_axi_control_arvalid,
    output wire                                    s_axi_control_arready,
    input  wire [C_S_AXI_CONTROL_ADDR_WIDTH-1:0]   s_axi_control_araddr ,
    output wire                                    s_axi_control_rvalid ,
    input  wire                                    s_axi_control_rready ,
    output wire [C_S_AXI_CONTROL_DATA_WIDTH-1:0]   s_axi_control_rdata  ,
    output wire [2-1:0]                            s_axi_control_rresp  ,
    output wire                                    s_axi_control_bvalid ,
    input  wire                                    s_axi_control_bready ,
    output wire [2-1:0]                            s_axi_control_bresp  ,
    output wire                                    interrupt
   )
   ```

   これで XML ファイルと RTL ソースを使用して、カーネルを XO ファイルにパッケージできるようになりました。

## RTL 検証

RTL デザインをカーネル XO ファイルとしてパッケージする前に、検証 IP、ランダムおよびプロトコル チェッカーなどの標準 RTL 検証方法を使用して、完全に検証しておきます。

このチュートリアルでは、Vector-Accumulate カーネルの RTL コードは既に検証済みです。

この手順を飛ばして RTL カーネル IP のパッケージを開始する場合は、次のセクションへ進んでください。

> **注記:** Vivado IP カタログに含まれる AXI Verification IP (AXI VIP) は、AXI インターフェイスを検証するのに役立ちます。このチュートリアルに含まれるテストベンチには、この AXI VIP が組み込まれています。

このテストベンチを使用して Vector Addition カーネルを検証する手順は、次のとおりです。

1. Flow Navigator で **\[Run Simulation]** を右クリックし、**\[Simulation Settings]** をクリックします。

2. \[Settings] ダイアログ ボックスで **\[Simulation]** をクリックします。

3. 次の図のように **\[xsim.simulate.runtime]** 値を `all` に変更します。

   ![Missing Image: simulation\_settings.PNG](./images/simulation_settings.PNG)

4. **\[OK]** をクリックします。

5. Flow Navigator で **\[Run Simulation]** → **\[Run Behavioral Simulation]** をクリックします。

   Vivado シミュレーションが実行されて終了したら、\[Tcl Console] ウィンドウに次のようなメッセージが表示されます。エラー メッセージはすべて無視します。

   `Test Completed Successfully` が表示されたら、カーネルが問題なく検証されています。

   > **注記:** このメッセージを確認するために、\[Tcl Console] ウィンドウをスクロールアップする必要があることもあります。

## RTL カーネル IP のパッケージ

これで Vivado Design Suite プロジェクトをカーネル XO ファイルとしてパッケージして、Vitis IDE で使用する準備ができました。

1. Flow Navigator で **\[Generate RTL Kernel]** をクリックします。次の図のように \[Generate RTL Kernel] ダイアログ ボックスが開きます。  
![Missing Image: generate\_rtl\_kernel.png](./images/192_vitis_generate_rtl_kernel.PNG)

   > **パッケージ オプションの詳細**
   >
   > - **\[Sources-only kernel]**: RTL デザイン ソースを直接使用してカーネルをパッケージします。
   > - **\[Pre-synthesized kernel]**: RTL デザイン ソースと、合成済みのキャッシュされた出力 (後でリンキング フローで使用すると、カーネルの再合成をしなくてすむので、ランタイムが改善されます) を含めてカーネルをパッケージします。
   > - **\[Netlist (DCP) based kernel]**: カーネルの合成済み出力により生成されたネットリストを使用して、カーネルをブラック ボックスとしてパッケージします。この出力はオプションで暗号化して、IP を保護しつつ、サードパーティに提供することも可能です。
   > - **\[Software Emulation Sources]** (オプション): ソフトウェア エミュレーションで使用可能なソフトウェア モデルを使用して、RTL カーネルをパッケージします。このチュートリアルでは、ソフトウェア モデルを使用しないので、このオプションは空のままにします。

2. このチュートリアルでは、**\[Sources-only]** を選択します。

3. **\[OK]** をクリックします。  
Vivado ツールで `package_xo` コマンドを使用して、XO ファイルを生成します。

   ```
   package_xo -xo_path Vadd_A_B.xo -kernel_name Vadd_A_B -kernel_xml kernel.xml -ip_directory ./ip/
   ```

   プロセスの記録は、\[Tcl Console] ウィンドウを参照してください。

4. プロンプトが表示されたら、Vivado Design Suite を終了します。

   Vivado ツールが閉じ、Vitis IDE に戻ります。RTL カーネルが開いている Vitis IDE プロジェクトにインポートされ、カーネル ウィザードのダイアログ ボックスが表示されます。

5. **\[OK]** をクリックします。

# Vitis IDE プロジェクトでの RTL カーネルの使用

## ホスト コードの削除とインポート

Vivado ツールを終了したら、次のファイルが Vitis IDE 環境の \[Project Explorer] ビューに追加されます。

- `Vadd_A_B.xo`: コンパイル済みのカーネル オブジェクト ファイル。
- `host_example.cpp`: サンプルのホスト アプリケーション ファイル。

1. \[Project Explorer] ビューに次の図のように `src` を展開します。  
![Missing Image: project\_explorer.png](./images/192_vitis_project_explorer.PNG)

   > **注記:** `Vadd_A_B.xo` に稲妻アイコンが付きます。Vitis IDE では、これはハードウェア関数であることを示しています。IDE で RTL カーネルが認識され、アクセラレーションされた関数としてマークされます。

2. `host_example.cpp` を選択して削除します。  
この段階で、チュートリアル用に用意されたホスト アプリケーションをインポートします。

3. このホスト アプリケーション プロジェクトをインポートするには、\[Project Explorer] ビューでチュートリアル プロジェクトを右クリックし、**\[Import Sources]** をクリックします。

4. `Vitis-Tutorials/docs/getting-started-rtl-kernels/reference-files/src/host` から **\[Browse]** をクリックして `reference-files/src/host` を指定して、**\[OK]** をクリックします。

5. フォルダー内で **\[Browse]** をクリックして `rtl_ke_t2/src/vitis_rtl_kernel/Vadd_A_B` を指定し、**\[OK]** をクリックします。

6. ホスト アプリケーション コードをプロジェクトに追加するには、`host.cpp` を選択して、`Into folder` タブ用に `Vadd_A_B` ディレクトリを選択します。

7. **\[Finish]** をクリックします。

   `host.cpp` ファイルが `src/vitis_rtl_kernel/Vadd_A_B` フォルダーに追加されます。

8. `host.cpp` をダブルクリックすると、\[Code Editor] ウィンドウが開きます。

## ホスト コードの構造

ホスト コードの構造は、次の 3 つのセクションに分けられます。

1. OpenCL ランタイム環境の設定
2. カーネルの実行
3. FPGA デバイスのポストプロセスとリリース

次は、ホスト アプリケーションと FPGA のやり取りを可能にする重要な OpenCL API 呼び出しの一部です。

- カーネルへのハンドルを作成します (239 行目)。

  ```C
    clCreateKernel(program, "Vadd_A_B", &err);
  ```

- ホストと FPGA 間のデータ転送用にバッファーを作成します (256 行目)。

  ```C
  clCreateBuffer(context, CL_MEM_READ_WRITE,sizeof(int) * number_of_words, NULL, NULL);
  ```

- 値 (A および B) がバッファーに書き込まれて、そのバッファーが FPGA に転送されます (266 および 274 行目)。

  ```C
  clEnqueueWriteBuffer(commands, dev_mem_ptr, CL_TRUE, 0,sizeof(int) * number_of_words, host_mem_ptr, 0, NULL, NULL);
  ```

- A と B がデバイスに転送されたら、カーネルが実行できるようになります (299 行目)。

  ```C
  clEnqueueTask(command_queue, kernel, 0, NULL, NULL);
  ```

- カーネルが終了したら、ホスト アプリケーションが新しい B の値を含むバッファーを読み戻します (312 行目)。

  ```C
  clEnqueueReadBuffer(command_queue, dev_mem_ptr, CL_TRUE, 0, sizeof(int)*number_of_words,host_mem_output_ptr, 0, NULL, &readevent );
  ```

ホスト アプリケーションの構造および要件の詳細は、[アプリケーションの開発](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/lhv1569273988420.html)を参照してください。

## プロジェクトのビルド

ホスト アプリケーション コード (`vadd.cpp`) および RTL カーネル コード (`Vadd_A_B.xo`) をプロジェクトに追加したら、プロジェクトをビルドして実行できます。

1. バイナリ コンテナーを作成するには、ハードウェア関数として RTL カーネルを選択します。\[Hardware Functions] ウィンドウでハードウェア関数をプロジェクトに追加する小さな稲妻アイコンをクリックし、Vadd\_A\_B 関数を選択します。

   > **注記:** \[Software Emulation] の場合は、RTL カーネル フローにそのカーネルの C/C++ ソフトウェア モデルが必要となります。このチュートリアルには、そのようなモデルは含まれていませんので、\[Software Emulation] ビルドを実行することはできません。

2. \[Vitis Application Project Settings] で **\[Active build configuration]** を **\[Emulation-HW]** に変更します。  
\[Hardware Emulation] は、次のような場合に使用します。

   - FPGA に含まれるロジックの機能を検証。
   - アクセラレータの最初のパフォーマンス見積もりとホスト アプリケーションを取得。

3. \[Run] ボタンをクリックし、**\[Run Configurations]** をクリックします。

   ![Missing Image: rtl\_kernel.png](./images/rtl_kernel.PNG)

   rtl\_ke\_t2-Default コンフィギュレーションが既にシステムで作成されています。ホスト コードに xclbin ファイルを読み込んで、引数リストの入力引数として提供する必要があります。![Missing Image: xclbin.png](./images/xclbin.PNG)

4. **\[Automatically add binary container(s) to arguments]** をオンにします。  
オンにすると、xclbin が自動的に検索されて含まれます。ホスト コードを実行する際に 2 つ目の入力引数として、xclbin ファイル名の後に `xilinx_u200_xdma_201830_2` を追加します。

5. ハードウェア エミュレーション コンフィギュレーションをビルドして実行し、結果を検証します。

### (オプション) ハードウェア プラットフォームでのシステムのビルドと実行

1. \[Vitis Application Project Settings] で **\[Active build configuration]** を **\[System]** に変更します。  
\[System] コンフィギュレーションでは、カーネル コードが FPGA にインプリメントされるので、選択したプラットフォーム カードで実行されるバイナリが生成されます。

2. 使用可能なハードウェア プラットフォームがある場合は、システムをビルドして実行してから、結果を検証してください。

# まとめ

1. Vitis IDE から RTL Kernel ウィザードを使用し、既存 RTL IP に基づいて、新しい RTL カーネルの名前とインターフェイスを指定しました。

   - RTL Kernel ウィザードは指定した仕様で XML テンプレートを作成し、これらの仕様に合わせてサンプルの RTL カーネル用のファイルを自動的に生成してから、Vivado Design Suite を起動しました。

2. Vivado Design Suite でサンプルの RTL ファイルを削除して、ユーザー自身の RTL IP ファイルを追加しました。

3. テストベンチを使用して IP をシミュレーションし、AXI Verification IP (AXI VIP) を組み込みました。

4. RTL IP プロジェクトを Vitis IDE で必要とされるコンパイル済み XO ファイルにパッケージしました。

5. RTL カーネルをホスト アプリケーションに追加し、\[Hardware Emulation] コンフィギュレーションをビルドしました。

   - Vitis IDE で、XO ファイルを使用してバイナリ コンテナーを作成し、xclbin ファイルをコンパイルしました。</br>

<hr/>
<p align= center><b><a href="../../README.md">メイン ページに戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>

この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。

