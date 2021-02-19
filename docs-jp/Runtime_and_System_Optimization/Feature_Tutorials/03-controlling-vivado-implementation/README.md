<p align="right"><a href="../../../../README.md">English</a> | <a>日本語</a>
<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.1">2020.1 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>

</table>

# Vivado インプリメンテーションの制御

## 概要

Vitis™ コンパイラは、ソース コードからカーネル オブジェクトを作成し、そのカーネルをターゲット シェルにリンクし、Vivado® ツールのインプリメンテーション フローを使用してアセンブルされたデザインを実行します。また、FPGA ベースのアクセラレーション カードをプログラムするのに必要なプラットフォーム ファイル (`xclbin`) を生成します。適切な結果を達成するため、タイミング クロージャを含む Vivado 合成およびインプリメンテーションのアドバンス オプションを使用することが必要となる場合があります。ここで説明されるトピックの詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vivado 合成およびインプリメンテーション結果の管理](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.2;t=vitis+doc;d=buildingdevicebinary.html;a=hnw1523048617934)を参照してください。

## チュートリアルの概要

このチュートリアルでは、プロジェクトをインプリメントする際に Vivado フローを制御する方法を GUI で手順ごとに示します。

Vitis コア開発キットには、Vivado ツール フローを制御する 2 つの方法があります。

1. ハードウェア ビルド用にシステムをリンクする際、`v++` コマンド ラインまたはコンフィギュレーション ファイルを使用して、Vivado 合成およびインプリメンテーション オプションを渡すことができます。

2. アプリケーションが既にコンパイル済みの場合は、次を実行できます。

   * Vivado Design Suite を使用してデザインを最適化します。
   * 新しい配線済みチェックポイントを生成します。
   * このチェックポイントを再利用して、新しいデバイスのバイナリ ファイル (`.xclbin`) を生成します。

> **ヒント:** このチュートリアルで使用するデザインは、GitHub の [Vitis Accel Examples](https://github.com/Xilinx/Vitis_Accel_Examples/tree/master/ocl_kernels/cl_gmem_2banks/src) の `ocl_kernels` カテゴリにある `cl_gmem_2banks` 例からのものです。

## 開始前の確認事項

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド
* 2020.2 Vitis コア開発キット リリースおよび xilinx\_u200\_xdma\_201830\_2 プラットフォーム。必要であれば、ほかのバージョンおよびプラットフォームを使用するように変更することもできます。

> **重要:**
>
> * 例を実行する前に、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[インストール](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/acceleration_installation.html#vhc1571429852245)の手順に従って、Vitis コア開発キットをインストールしてください。
> * ザイリンクス Alveo™ データセンター アクセラレータ カードでアプリケーションを実行する場合は、[Alveo ポートフォリオ](https://japan.xilinx.com/products/boards-and-kits/alveo.html) ページの手順に従って、カードとソフトウェア ドライバーを正しくインストールしてください。
> * この演習を実行する前に、[Vitis 入門](https://github.com/Xilinx/Vitis-Tutorials/tree/master/Getting_Started/Vitis)チュートリアルを理解しておく必要があります。

### チュートリアル リファレンス ファイルの入手

1. リファレンス ファイルを入手するには、ターミナルに `git clone https://github.com/Xilinx/Vitis-Tutorials` と入力します。
2. `Runtime_and_System_Optimization/Feature_Tutorials/03-controlling-vivado-implementation` ディレクトリに移動し、`reference-files/run` ディレクトリにアクセスします。

### Vitis 環境の設定

Vitis コア開発キットを設定するには、次のコマンドを実行します。

> ```bash
>  #setup Xilinx Vitis tools. XILINX_VITIS and XILINX_VIVADO will be set in this step.
>  source <VITIS_install_path>/settings64.sh
>  #Setup Xilinx runtime. XILINX_XRT will be set in this step.
>  source <XRT_install_path>/setup.sh
> ```

## Vitis コンパイラからの Vivado 合成およびインプリメンテーションの制御

> **注記:** このチュートリアルでは、すべての手順を `reference-files/run` ディレクトリから実行します。

`–-vivado` オプションは、Vivado ツールを設定するプロパティまたはパラメーターに対応しています。たとえば、`--vivado` オプションで最適化、配置、およびタイミングを設定したり、エミュレーションおよびコンパイル オプションを設定したりできます。コマンド ライン フローでは、プロパティは `--vivado.prop <object_type>.<object_name>.<prop_name>` と指定されます。

* `prop`: プロパティを指定することを示す必須のキーワード。
* `object_type`: プロパティを割り当てるファースト クラス オブジェクトを指定します。
* `object_name`: 割り当てるオブジェクト インスタンスを指定します。
* `prop_name`: プロパティ名と値のペアを指定します。

`v++` コマンド ラインで `--vivado` オプションを使用することもできますが、一部は `--config` オプションで指定するコンフィギュレーション ファイルで指定するのが合理的です。これについては、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis コンパイラのコンフィギュレーション ファイル](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.2;t=vitis+doc;d=vitiscommandcompiler.html;a=pni1524163195211)を参照してください。このチュートリアルでは、次のオプションを例として使用して、Vivado 合成およびインプリメンテーションを制御するフローを示します。

* `FLATTEN_HIERARCHY=full` プロパティを指定して、RTL 合成で階層を完全にフラット化します。

  * `--vivado.prop run.my_rm_synth_1.{STEPS.SYNTH_DESIGN.ARGS.FLATTEN_HIERARCHY}={full}`

* `STEPS.ROUTE_DESIGN` プロパティを使用して、Vivado 配線段階で `NoTimingRelaxation` 指示子を指定します。

  * `--vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={NoTimingRelaxation}`

このチュートリアルでは、`-–vivado` コマンド オプションは、ハードウェア ビルド プロセスで使用される `design.cfg` ファイルで定義されています。

> **ヒント:** Vivado 合成およびインプリメンテーションを実行するので、ハードウェア ビルド プロセス (`t=hw`) にはかなりの時間がかかります。

1. 次のコマンド オプションを使用して、Vitis コンパイラを使用してカーネルをコンパイルしてプラットフォーム ファイルにリンクし、デバイス バイナリ (`.xclbin`) を生成します。

   ```bash
   v++ -t hw --config design.cfg -c -k apply_watermark -o apply_watermark.hw.xilinx_u200_xdma_201830_2.xo ../src/krnl_watermarking.cl
   v++ -t hw -s --config design.cfg -R2 -l -o apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin apply_watermark.hw.xilinx_u200_xdma_201830_2.xo
   ```

2. `design.cfg` ファイルを開いて、さまざまなオプションを確認します。次のリストでは、コマンド オプションの一部を説明しています。

   > **コマンド オプションの説明**
   >
   > * `-t hw`: ビルド ターゲットをハードウェアに指定します。
   > * `platform=xilinx_u200_xdma_201830_2`: xilinx\_u200 プラットフォームを指定します。
   > * `debug=1`: デバッグ情報を生成します。
   > * `max_memory_ports=apply_watermark`: 各ポートに個別の AXI4 インターフェイスを生成します。
   > * `-c`: カーネルをコンパイルします。
   > * `-k apply_watermark`: カーネル名を指定します。
   > * `../src/krnl_watermarking.cl`: ソース ファイルを指定します。
   > * `sp=apply_watermark_1.m_axi_gmem0:DDR[0]`: DDR バンク接続を指定します。
   > * `--config design.cfg`: Vivado 合成およびインプリメンテーション オプションを設定します。
   > * `-R2`: レポート レベルを 2 に設定して、各インプリメンテーション段階ごとに DCP を生成します。
   > * `-l`: カーネルをリンクします。
   > * `nk=apply_watermark:1:apply_watermark_1`: カーネル インスタンスの数と名前を指定します。

3. Vivado 合成およびインプリメンテーション オプションが適用されたことを確認するには、`_x/logs/link/vivado.log` ファイルで「flatten\_hierarchy」および「NoTimingRelaxation」を検索します。オプションが実行されたかどうかは、次の行からわかります。

   * **コマンド**: `synth_design -top pfm_dynamic -part xcu200-fsgd2104-2-e -flatten_hierarchy full -mode out_of_context`
   * **コマンド**: `route_design -directive NoTimingRelaxation`

Vivado Design Suite での合成およびインプリメンテーションの結果を制御するためのプロパティを適用できました。

## Vivado ツールでのデザインの最適化

タイミング クロージャを達成するなどデザインの目標を達成、またはパフォーマンスを向上するため、さまざまな最適化手法を使用して Vivado インプリメンテーション結果を最適化します。Vivado ツールでデザインを最適化したら、配線後のデザインからデザイン チェックポイント (DCP) を生成し、Vitis コンパイラのリンク プロセス (`v++ --link`) で再利用します。

> **重要:** Vitis ツールと Vivado ツールのバージョンが同じであることを確認してください。

Vivado ツールの使用方法および最適化については、『Vivado Design Suite ユーザー ガイド: インプリメンテーション』 (UG904: [英語版](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=latest;d=ug904-vivado-implementation.pdf)、[日本語版](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=latest;d=ug904-vivado-implementation.pdf)) および『UltraFast 設計手法ガイド (Vivado Design Suite 用)』 (UG949: [英語版](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=latest;d=ug949-vivado-design-methodology.pdf)、[日本語版](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=latest;d=ug949-vivado-design-methodology.pdf)) を参照してください。

Vivado ツールは、Tcl スクリプトを使用してバッチ モードで実行するか、対話形式で実行できます。このチュートリアルでは、対話形式で実行し、ツール内で Tcl スクリプトを読み込んで最適化を達成します。これにより、Vivado IDE の使用に慣れることができます。

1. `/reference-file/run` フォルダーから Vivado IDE を起動します。

   ```bash
   vivado
   ```

2. Vivado IDE が開いたら、\[Tcl Console] ウィンドウに次のコマンドを入力します。

   ```bash
   source ./opt.tcl
   ```

   `opt.tcl` ファイルには、次の Tcl コマンドが含まれます。

   ```bash
   #DCP files in different stages of Vivado have been written by v++ linker automatically with option "-R2"
   open_checkpoint ./_x/link/vivado/vpl/prj/prj.runs/impl_1/pfm_top_wrapper_routed.dcp
   #Run post-route physical optimization
   phys_opt_design -directive AggressiveExplore
   write_checkpoint -force ./_x/link/vivado/routed.dcp
   ```

   `opt.tcl` スクリプトが完了したら、Vivado IDE を使い慣れていない場合は、時間をとってツールを見てみてください。IDE の下部にある \[Tcl Console] ウィンドウに表示されている実行されたコマンドを確認してください。

3. **\[File] → \[Exit]** をクリックします。

### 最適化されたチェックポイントを再利用してデバイス バイナリを作成

次に、`opt.tcl` スクリプトで生成された `routed.dcp` チェックポイント ファイルを再利用して新しいプラットフォーム ファイル (`.xclbin`) を生成します。これには、`v++` コマンドに `--reuse_impl` オプションを追加します。これにより、インプリメント済み Vivado デザイン チェックポイント ファイル (`.dcp`) を使用して `--link` プロセスが再実行されます。最初のハードウェア ビルドよりも、かなり短時間で実行されます。

```bash
v++ -t hw --config design.cfg -l -o 'apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin' apply_watermark.hw.xilinx_u200_xdma_201830_2.xo --reuse_impl ./_x/link/vivado/routed.dcp
```

コマンド シェルに、インプリメントしたデザインの生成をスキップしてビットストリームの生成が開始されたことが表示されます。

```bash
INFO: [VPL 60-423]   Target device: xilinx_u200_xdma_201830_2
[11:22:58] Run vpl: Step interactive: Started
Starting FPGA bitstream generation.
```

## 手順のサマリ

次に、`v++` コマンドを使用して Vivado インプリメンテーションを制御し、Vivado ツールを使用してデザインを最適化し、インプリメント済み DCP を再利用してプラットフォーム ファイルを生成する方法をまとめます。コマンドのクイック リファレンスとしてご利用ください。

1. コア開発キットおよび作業ディレクトリを設定します。

   ```bash
    #setup Xilinx Vitis tools. XILINX_VITIS and XILINX_VIVADO will be set in this step.
    source <VITIS_install_path>/settings64.sh
    #Setup Xilinx runtime. XILINX_XRT will be set in this step.
    source <XRT_install_path>/setup.sh
   ```

2. `v++` コマンドを使用してプラットフォーム ファイル (`xclbin`) を生成します。

   ```bash
   v++ -t hw --config design.cfg -c -k apply_watermark -o apply_watermark.hw.xilinx_u200_xdma_201830_2.xo ../src/krnl_watermarking.cl
   v++ -t hw -s --config design.cfg -R2 -l -o apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin apply_watermark.hw.xilinx_u200_xdma_201830_2.xo
   ```

3. Vivado ツールを使用してデザインを最適化し、配線後の DCP を記述し直します。

   ```bash
   vivado -mode batch -source opt.tcl
   ```

4. 配線済み DCP を再利用して、デバイス バイナリ (`.xclbin`) を生成します。

   ```bash
   v++ -t hw --config design.cfg -l -o apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin apply_watermark.hw.xilinx_u200_xdma_201830_2.xo --reuse_impl ./_x/link/vivado/routed.dcp
   ```

5. このチュートリアルには、さまざまなフローを実行するためのホスト コードが含まれています。次のコマンドは、ホストをビルドしてからアプリケーションを実行します。

   ```bash
   #Generate the host executable.
   g++ -I$XILINX_XRT/include/ -I$XILINX_VIVADO/include/ -Wall -O0 -g -std=c++11 -L$XILINX_XRT/lib/ -lOpenCL -lpthread -lrt -lstdc++ -o 'host' '../src/host.cpp'
   #Please set correct XCL_EMULATION_MODE manually if running sw_emu and hw_emu modes
   ./host apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin ../src/inputImage.bmp ../src/golden.bmp
   ```

## まとめ

このチュートリアルでは、Vivado ツールでデザインを最適化する単純な例を使用して、`v++` コマンドの `--vivado` オプションで Vivado 合成およびインプリメンテーションを制御する方法を示しました。また、インプリメント済み DCP を再利用してデバイス バイナリ (`.xclbin`) を生成し、最適化された Vivado インプリメンテーションから Vitis アクセラレーション アプリケーションをビルドしました。</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">メイン ページに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 1 月 22 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
