<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td align="center"><h1>アクセラレーション アプリケーションのビルドおよび実行の基本概念</h1>
 </td>
 </tr>
</table>

# 1\. アプリケーションのビルド

## 概要

[FPGA アクセラレーション アプリケーションの実行モデル](../vitis-execution-model/README.md)に示すように、アクセラレーションされたアプリケーションには、x86 サーバーで実行されるソフトウェア プログラムと、Alveo データセンター アクセラレータ カードまたはザイリンクス デバイスで実行されるアクセラレーション カーネルが含まれます。これらソースは、別々にビルド (コンパイルおよびリンク) する必要があります。詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[アクセラレーション用 Vitis 環境の概要](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=chunkbreaker1.html)を参照してください。

このチュートリアルでは、`g++` コンパイラおよび Vitis コンパイラ (`v++`) を使用して、デザインのソフトウェア部分とハードウェア部分の両方をビルドする方法を説明します。ターゲット プラットフォームを指定する方法やハードウェアまたはソフトウェア エミュレーションをビルドする方法など、さまざまなコマンド ライン オプションについて説明します。

このチュートリアルで使用するリファレンス ファイルは、`reference-files` ディレクトリに含まれます

> **重要:** サンプル コマンドを実行する前に、[Vitis 環境の設定](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=rbk1547656041291.html)に示すように、次のコマンドを実行して Vitis コア開発キットをインストールしておく必要があります。
>
> ```bash
>  #setup Xilinx Vitis tools. XILINX_VITIS and XILINX_VIVADO will be set in this step.
>  source <VITIS_install_path>/settings64.sh
>  #Setup Xilinx runtime. XILINX_XRT will be set in this step.
>  source <XRT_install_path>/setup.sh
> ```

**ヒント:** `reference-files` には、システム (`make build`) のビルドに使用可能な makefile やホスト (`make exe`) および `xclbin` (`make xclbin`) などのビルドの要素が含まれます。`make clean` を使用すると、ビルド データをクリーンアップして新たに開始できます。

## ソフトウェアのビルド

ソフトウェア プログラムは、C/C++ で記述され、OpenCL™ API 呼び出しを使用してアクセラレーションされたカーネルとの通信および制御を実行します。標準 GCC コンパイラまたは GCC のラッパーである `g++` コンパイラを使用してビルドされます。各ソース ファイルはオブジェクト ファイル (`.o`) にコンパイルされ、ザイリンクス ランタイム (XRT) 共有ライブラリとリンクされて、実行ファイルが作成されます。GCC および関連するコマンド ライン オプションの詳細は、[GNU コンパイラ コレクション (GCC) の使用](https://gcc.gnu.org/onlinedocs/gcc/)を参照してください。

1. **ソフトウェア プログラムのコンパイル**

   `./reference_files/run` フォルダーに移動します。

   ```
   cd <Vitis-Tutorials>/docs/Pathway3/reference_files/run
   ```

   ホスト アプリケーションをコンパイルするには、`-c` オプションを使用してホスト ソース ファイルのリストを指定します。

   オプションで、`-o` オプションを使用して出力オブジェクト ファイル名を指定することもできます。

   ```bash
   g++ ... -c <source_file_name1> ... <source_file_nameN> -o <object_file_name> -g
   ```

2. **ソフトウェア プログラムのリンク**

   生成したオブジェクト ファイルをリンクするには、次のように `-l` オプションとオブジェクト入力ファイルを指定します。

   ```bash
   g++ ... -l <object_file1.o> ... <object_fileN.o> -o <output_file_name>
   ```

   > **ヒント:** ホスト コンパイルおよびリンクは、1 つにまとめることができます。その場合、`-c` および `-l` オプションは必要ありません。必要なのは、次の例に示すように、ソース入力ファイルのみです。
   >
   > `g++ ... <source_file_name1> ... <source_file_nameN> ... -o <output_file_name>`

3. **必須のフラグ**

   XRT および Vivado ツールのインクルード パスとライブラリ パスを指定する必要があります。

   1. `-I` オプションを使用してインクルード ディレクトリを `-I$XILINX_XRT/include -I$XILINX_VIVADO/include` と指定します。
   2. `-L` オプションを使用して `-l` ライブラリを検索するディレクトリを `-L$XILINX_XRT/lib` と指定します。
   3. `-l` オプションを使用してリンク中に使用されるライブラリを `-lOpenCL -lpthread -lrt -lstdc++` と指定します。

4. **完全なコマンド**

   ホスト プログラムを `./reference_files/run` フォルダーから 1 つのステップでコンパイルおよびリンクするコマンドは、次のとおりです。次のコマンドを実行します。

   ```bash
   g++ -I$XILINX_XRT/include/ -I$XILINX_VIVADO/include/ -Wall -O0 -g -std=c++11 \
   ../src/host.cpp  -o 'host'  -L$XILINX_XRT/lib/ -lOpenCL -lpthread -lrt -lstdc++
   ```

   > **コマンド オプションおよび説明**
   >
   > * `-I../libs`、`-I$XILINX_XRT/include/`、および `-I$XILINX_VIVADO/include/`: インクルード ディレクトリ
   > * `-Wall`: 警告をすべてイネーブル
   > * `-O0`: 最適化オプション (最低限の最適化を実行)
   > * `-g`: デバッグ情報を生成
   > * `-std=c++11`: 言語規格 (インクルード ディレクトリではなく C++ 規格を定義)
   > * `../src/host.cpp`: ソース ファイル
   > * `-o 'host'`: 出力名
   > * `-L$XILINX_XRT/lib/`: XRT ライブラリを検索
   > * `-lOpenCL`、`-lpthread`、`-lrt`、および `-lstdc++`: リンク中に指定のライブラリを検索

## ハードウェアのビルド

次に、ハードウェア アクセラレータ カードで実行されるカーネルをビルドする必要があります。ホスト アプリケーションと同様、カーネルのビルドにもコンパイルおよびリンクが必要です。ハードウェア カーネルは C/C++、OpenCL C、または RTL で記述できます。C/C++ および OpenCL C カーネルは、Vitis コンパイラを使用してコンパイルされますが、RTL で記述されたカーネルはザイリンクスの `package_xo` ユーティリティを使用してコンパイルされます。

`v++` および `package_xo` の詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis 環境リファレンス資料](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=yxl1556143111967.html)を参照してください。カーネルのコンパイル方法にかかわらず、どちらの方法でもザイリンクス オブジェクト ファイル (`.xo`) が生成されます。

オブジェクト ファイルは、FPGA バイナリ ファイルまたは `xclbin` ファイルを作成するために、Vitis コンパイラを使用してシェル (ハードウェア プラットフォーム) とリンクされます。

次の図に、さまざまなタイプのカーネルのコンパイルおよびリンク フローを示します。

![compiling\_and\_linking\_flow](images/compiling_and_linking_flow.png)

このチュートリアルでは、`v++` コンパイルについてのみ説明し、RTL カーネルについては説明しません。RTL カーネルのビルド方法については、[RTL カーネル入門](../getting-started-rtl-kernels/README.md)チュートリアルを参照してください。

### ハードウェア コンパイル

ハードウェア コンパイルでは、`v++` コマンドの `-c` オプションを使用してハードウェア カーネルのソース ファイルをコンパイルします。`v++` コマンドには多くのコマンド オプションがありますが、最低でもソース ファイル、ターゲット プラットフォーム、ビルド ターゲットを指定する必要があります。`v++` コマンド オプションのリストは、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis コンパイラ コマンド](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vitiscommandcompiler.html;a=wrj1504034328013)を参照してください。

`-k` または `--kernel` 引数を使用すると、コンパイルするカーネル ソース ファイル内のカーネル名を指定できます。

`v++ … -k <kernel_name> <kernel_source_file> … <kernel_source_file>`

ターゲット プラットフォームは、コンパイルとリンクの両方の段階で指定する必要があります。これは、次のように `--platform` オプションを使用して指定します。

`v++ … --platform <platform_target>`

最後に、`-t` オプションを使用してビルド ターゲットを `v++ … -t <build_target>` のように指定します。

ビルド ターゲットには、デバッグおよび検証に使用する 2 つのエミュレーション ターゲット、および FPGA で実行する必要のある実際のバイナリを生成するのに使用されるハードウェア ターゲット 1 つの合計 3 つがあります。

これらのビルド ターゲットの設定オプションは、`sw_emu`、`hw_emu`、および `hw` です。

* ソフトウェア エミュレーション (`sw_emu`) では、ホスト アプリケーション コードとカーネル コードの両方が x86 プロセッサで実行できるようコンパイルされます。これにより、ビルドと実行を高速に繰り返して反復アルゴリズムで調整していくことができます。このターゲットは、構文エラーを特定し、アプリケーションと共に実行されるカーネル コードをソース レベルでデバッグし、システムの動作を検証するのに便利です。  RTL カーネルでは、C モデルが関連付けられている場合にソフトウェア エミュレーションがサポートされます。C モデルがない場合は、ハードウェア エミュレーションを使用してカーネル コードをデバッグする必要があります。

* ハードウェア エミュレーション (`hw_emu`) では、カーネル コードはハードウェア モデル (RTL) にコンパイルされ、ハードウェア シミュレータで実行されますが、システムの残りの部分には C シミュレータが使用されます。ビルドおよび実行にかかる時間は長くなりますが、詳細でサイクル精度のカーネル アクティビティが表示されます。このターゲットは、FPGA で実行されるロジックの機能をテストして、初期パフォーマンス見積もりを取得する場合に便利です。

* ターゲットがハードウェア (`hw`) に設定すると、カーネル コードが合成されてコンパイルされ、FPGA で実行するバイナリが生成されます。

Vitis コンパイラでは、コマンド ラインでコンフィギュレーション ファイルがサポートされます。これにより、さまざまなオプションを指定した長いコマンド ラインを管理し、関連するオプションをまとめることができます。この演習では、`design.cfg` ファイルを使用し、必要なすべてのオプションをこのファイルに含めます。`design.cfg` ファイルを開き、内容を確認します。

```
platform=xilinx_u200_xdma_201830_2
debug=1
save-temps=1

[connectivity]
nk=mmult:1:mmult_1
```

次のコマンドを使用し、`/reference-files/run` フォルダーからカーネルをソフトウェア エミュレーション ターゲットにコンパイルします。

```bash
v++ -t sw_emu --config design.cfg -c -k mmult -I'../src' -o mmult.sw_emu.xilinx_u200_xdma_201830_2.xo ../src/mmult.cpp
```

> **コマンドおよびコンフィギュレーション ファイルのオプションと説明**
>
> * `-t sw_emu`: ソフトウェア エミュレーションをターゲットに指定
> * `--config design.cfg`: 使用するコンフィギュレーション ファイルを指定
> * `platform=xilinx_u200_xdma_201830_2`: xilinx\_u250 プラットフォームをターゲットに指定
> * `debug=1`: デバッグ情報を生成
> * `save-temps=1`: ビルド プロセス中に生成された一時ファイルを保存
> * `nk=mmult:1:mmult_1`: インスタンシエートするカーネル インスタンス (CU) の数を定義
> * `-c`: カーネルをコンパイル
> * `-k mmult`: コンパイルするカーネルを指定
> * `../src/mmult.cpp`: ソース ファイルを指定
> * `-o mmult.sw_emu.xilinx_u200_xdma_201830_2.xo`: .xo 出力パスおよびファイル名を指定

`v++` コマンドでは、Vitis アナライザーで関連レポートを確認するために表示可能な `compile_summary` ファイルも生成されます。次のコマンドを使用すると、Vitis アナライザーを開いてコンパイル サマリを確認できます。

```
vitis_analyzer mmult.sw_emu.xilinx_u200_xdma_201830_2.xo.compile_summary
```

### ハードウェアのリンク

ハードウェアのリンクでは、1 つまたは複数のカーネルをプラットフォームにリンクし、出力バイナリ コンテナー (`.xclbin`) ファイルを作成します。ハードウェア カーネルをリンクするには、`v++` コマンドに `-l` オプションを使用します。コンパイルと同様、リンクでは複数のオプション (入力 .xo オブジェクト ファイル、ターゲット プラットフォーム、およびビルド ターゲットの指定など) が必要です。使用可能なリンク オプションについては、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis コンパイラ コマンド](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vitiscommandcompiler.html;a=wrj1504034328013)を参照してください。リンク中に使用されるプラットフォームおよびビルド ターゲット オプションは、コンパイル中に使用されるものと同じである必要があります。

`.xo` オブジェクト ファイルは、オブジェクト ファイルを直接リストして `v++` コマンドで指定します。複数のオブジェクト ファイルを追加できます。

`v++ … <kernel_1.xo> … <kernel_2.xo>`

リンク段階で指定できるオプションは多数あります。`--nk` はその 1 つです。リンク プロセス中に作成されるカーネル インスタンス、計算ユニット (CU) 数を指定するのに使用されます。使用方法は、次のとおりです。

`v++ ... --nk <kernel_name>:<compute_units>:<kernel_name1>:…:<kernel_nameN>`

> **ヒント:** インスタンス名 \(`kernel\_name1...`) はオプションで、指定しない場合は自動的に定義されます。

`-o` オプションを使用して生成される出力ファイルの名前を指定することもできます。リンク段階の出力ファイルは `xclbin` ファイルで、それに応じた名前を付ける必要があります。

`v++ ... -o <xclbin_name>.xclbin`

ここで、ハードウェアをリンクします。ハードウェア コンパイル段階のターゲットに一致するように、プラットフォームとターゲットを指定する必要があります。

```bash
v++ -t sw_emu --config design.cfg -l -g -o mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin mmult.sw_emu.xilinx_u200_xdma_201830_2.xo
```

> **コマンド オプションおよび説明**
>
> * `-l`: カーネルをリンク
> * `-g`: デバッグ情報を生成
> * `-t sw_emu`: ソフトウェア エミュレーションをターゲットに指定
> * `--config design.cfg`: コンパイル中に使用されるコンフィギュレーション ファイルを指定
> * `nk=mmult:1:mmult_1`: mmult\_1 という 1 つの CU を作成。これはコンフィギュレーション ファイルで定義できるようになりました。
> * `mmult.sw_emu.xilinx_u200_xdma_201830_2.xo`: 入力オブジェクト ファイル
> * `-o mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin`: xclbin ファイルの名前を指定

`v++` コマンドでは、Vitis アナライザーで使用可能な `link_summary` ファイルを生成して、関連するレポートを表示することもできます。次のコマンドを実行して、link\_summary を表示します。

```
vitis_analyzer mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin.link_summary
```

### ハードウェア エミュレーションのビルド

ハードウェア エミュレーション用のハードウェアをビルド、または Alveo データセンター アクセラレータ カード (ハードウェア) をターゲットにする場合、\<*build\_target*> を定義する `-t` オプションを、ハードウェア エミュレーションの場合は **sw\_emu** から **hw\_emu** に、アクセラレータ カード用にビルドする場合は **hw** に変更します。

次のコード例に、ビルド ターゲットがハードウェア エミュレーションの場合のコンパイルおよびリンクの `v++` コマンドを示します。

```
v++ -t hw_emu --config design.cfg -c -k mmult -I'../src' -o mmult.hw_emu.xilinx_u200_xdma_201830_2.xo ../src/mmult.cpp
v++ -t hw_emu --config design.cfg -l -o mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin mmult.hw_emu.xilinx_u200_xdma_201830_2.xo
```

### Vitis アナライザーでのハードウェア エミュレーション ビルド レポートの確認

Vitis ツールでは、ビルド プロセスの各段階 (コンパイル、リンク、実行段階) のサマリと、その段階に関連する一連のレポートが生成されます。このサマリには、ビルドのステータスの簡単な概要と、それを作成するのに使用されたコマンド ラインが表示されます。これらのサマリは Vitis アナライザー ツールで確認できます。サマリを開くと、Vitis アナライザーでそのサマリに関連するレポートが自動的に開きます。これらのレポートには、次の内容が含まれます。

* Vitis ツールで生成されたガイダンスには、複数の重要度レベル (エラー、アドバイザリ、警告、クリティカル警告) があります。これらは、ソフトウェア エミュレーション、ハードウェア エミュレーション、ハードウェア ビルドのコンパイル、リンク、実行段階で生成されます。

* ハードウェア エミュレーションおよびハードウェア ビルドでは、カーネル (`.xo`) およびシステム (`xclbin`) のパフォーマンス見積もりも表示されます。見積もりレポートは、コンパイルとリンク段階の両方で自動的に生成され、FPGA リソース使用量の見積もりと、ハードウェアでアクセラレーションされたカーネルの周波数の見積もりが示されます。

* HLS 合成レポートには、Vitis ツールからの合成結果がサブ関数とループの説明と共に表示されます。レイテンシは、クロック サイクルと時間でレポートされます。

さまざまなレポートの詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[アプリケーションのプロファイリング](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=profilingapplication.html;a=xmv1511400547463)を参照してください。

次のコマンドを使用して、ハードウェア エミュレーション (hw\_emu) のリンク サマリを開きます。

```
vitis_analyzer hw_emu/mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin.link_summary
```

次の図に、hw\_emu リンク段階で生成されたサマリ レポートの例を示します。Vitis アナライザー ツールの左側の `link_summary` の下には、開いているレポートがリストされます。これには、`compile_summary` も含まれ、link コマンドに含まれる各カーネルごとに開きます。リストされるレポートをクリックすると、Vitis アナライザーで開きます。詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis アナライザーの使用](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=jfn1567005096685.html)を参照してください。

![vitis\_analyzer\_link\_summary.png](images/vitis_analyzer_link_summary.png)

Vitis アナライザーでは、ビルド プロセス中に生成されたファイルを使用してそのレポート ビューが表示されます。このため、コマンド ラインに `--save-temps` オプションを追加するか、このチュートリアルで実行したようにコンフィギュレーション ファイルを追加する必要があります。ただし、保存したレポートは、ビルド プロセスを再実行したり、新しいターゲットをビルドしたりすると上書きされます。つまり、ソフトウェア エミュレーション ビルド レポートはハードウェア エミュレーション ビルド レポートで上書きされます。上書きされないようにするには、コマンド ラインで出力を新しい `--temp_dir` ディレクトリに変更するか、`archive_summary` コマンドで新しいビルドの実行前に現在のビルドのレポート アーカイブを作成します。

ソフトウェア エミュレーション ビルドまたはハードウェア エミュレーション ビルドを実行したら、次のコマンドを使用してアーカイブを作成します。

```
archive_summary mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin.link_summary mmult.hw_emu.link_summary.archive
```

`archive_summary -h` を使用すると、このコマンドとそのオプションの詳細を確認できます。次のビルドを実行する前にアーカイブを作成して、サマリ レポートを保存しておくと、この後の実行と比較できます。link\_summary をアーカイブすると、link\_summary が参照する compile\_summary も自動的に保存されます。

### ハードウェアのビルド

ハードウェア ビルド ターゲットのコンパイルおよびリンクの `v++` コマンドは、次のコード例を参照してください。

> **重要:** ハードウェア ターゲットのコンパイルおよびリンクには、FPGA バイナリ ファイルの合成およびインプリメンテーションのため、数時間かかることがあります。

```
v++ -t hw --config design.cfg -c -k mmult -I'../src' -o mmult.hw.xilinx_u200_xdma_201830_2.xo ../src/mmult.cpp
v++ -t hw --config design.cfg -l -o mmult.hw.xilinx_u200_xdma_201830_2.xclbin mmult.hw.xilinx_u200_xdma_201830_2.xo
```

### 手順のサマリ

この演習のソース ファイルを使用して、異なるビルドをターゲットにしたソフトウェアおよびハードウェア両方をビルドする方法を次にまとめます。

1. Vitis コア開発キットを設定します。

   ```bash
   #setup Xilinx Vitis tools, XILINX_VITIS and XILINX_VIVADO will be set in this step. source <VITIS install path>/settings64.sh. for example:
   source <Vitis_install_dir>/Vitis/2020.1/settings64.sh
   #Setup runtime. XILINX_XRT will be set in this step
   source /opt/xilinx/xrt/setup.sh
   #change to the working directory
   cd ./reference-files/run
   ```

2. ホスト ソフトウェアをビルドします。

   ```bash
   g++ -I$XILINX_XRT/include/ -I$XILINX_VIVADO/include/ -Wall -O0 -g -std=c++11 ../src/host.cpp  -o 'host'  -L$XILINX_XRT/lib/ -lOpenCL -lpthread -lrt -lstdc++
   ```

3. ハードウェアをビルドします。ターゲット (ソフトウェア エミュレーション、ハードウェア エミュレーション、またはハードウェア) を選択して、関連のコマンドを実行します。

   * ソフトウェア エミュレーションをターゲットにした場合

     ```bash
     v++ -t sw_emu --config design.cfg -c -k mmult -I'../src' --temp_dir sw_emu -o 'sw_emu/mmult.sw_emu.xilinx_u200_xdma_201830_2.xo' '../src/mmult.cpp'
     v++ -t sw_emu --config design.cfg -l --temp_dir sw_emu -o 'sw_emu/mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin' sw_emu/mmult.sw_emu.xilinx_u200_xdma_201830_2.xo
     ```

   * ハードウェア エミュレーションをターゲットにした場合

     ```bash
     v++ -t hw_emu --config design.cfg -c -k mmult -I'../src' --temp_dir hw_emu -o 'hw_emu/mmult.hw_emu.xilinx_u200_xdma_201830_2.xo' '../src/mmult.cpp'
     v++ -t hw_emu --config design.cfg -l --temp_dir hw_emu -o 'hw_emu/mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin' hw_emu/mmult.hw_emu.xilinx_u200_xdma_201830_2.xo
     ```

   * システムをターゲットにした場合

     > **重要:** この手順は数時間かかるので、オプションです。

     ```bash
     v++ -t hw --config design.cfg -c -k mmult -I'../src' --temp_dir hw -o 'hw/mmult.hw.xilinx_u200_xdma_201830_2.xo' '../src/mmult.cpp'
     v++ -t hw --config design.cfg -l --temp_dir hw -o 'hw/mmult.hw.xilinx_u200_xdma_201830_2.xclbin'   hw/mmult.hw.xilinx_u200_xdma_201830_2.xo
     ```

4. ビルドの link\_summary および compile\_summary をアーカイブします。

   ```
   archive_summary mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin.link_summary mmult.hw_emu.link_summary.archive
   ```

## 次の手順

デザインをビルドできたら、エミュレーションを実行してデバッグおよび最適化します。

* [ソフトウェアおよびハードウェア エミュレーションの実行](./Emulation.md)演習では、ソフトウェアおよびハードウェア エミュレーションの実行方法を説明します。
* Alveo データセンター アクセラレータ カードがある場合は、[ハードウェアでの実行](./HardwareExec.md)演習に進んで、ハードウェアで直接アプリケーションを実行することもできます。</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="../../docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="./README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
