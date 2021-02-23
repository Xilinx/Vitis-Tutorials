<p align="right"><a href="../../../../../../README.md">English</a> | <a>日本語</a></p>

**モジュール 1** のコード、ファイル、および手順

> **このモジュールの内容:**

_1> ホスト/カーネルの実例の OpenCL API を確認  
2> Vitis を GUI から実行、またはソフトウェアおよびハードウェア エミュレーションを実行する make を実行  
3> Vitis アナライザーを実行してアプリケーション タイムラインを確認  
4> Vitis HLS を実行してスループットのボトルネックを判別  
5> HLS にテストベンチを挿入してすばやくイテレーションおよびカーネル コードの変更を検証_

## ホストとカーネルを使用したコード設定

各モジュールのソースコードは、ローカルの <code>./src</code> ディレクトリの下にあります。

+ ホスト コードを確認すると、複数の API が使用されていることがわかります。データがカーネル間をどのように転送されるかに注目してください。実行モデルでは、次が実行されます。
  + ホスト プログラムが、Alveo データセンター アクセラレータ カード上の PCIe インターフェイスまたはエンベデッド プラットフォームの AXI バスを介して、カーネルで必要なデータをグローバル メモリに書き込みます。
  + ホスト プログラムが、入力パラメーターを使用してカーネルを設定します。
  + ホスト プログラムが FPGA のカーネル関数の実行をトリガーします。
  + カーネルが、必要に応じてグローバル メモリからのデータを読み出しながら、計算を実行します。
  + カーネルがグローバル メモリにデータを書き込み、ホストにタスクが終了したことを通知します。
  + ホスト プログラムがグローバル メモリからホスト メモリにデータを読み出し、必要に応じて処理を続けます。
+ カーネル コードを見てください。  このコードは、Vitis ツールでコンパイルされ、ハードウェア記述 (Vivado ツールがザイリンクス デバイスにインプリメント) に変換されます。ホスト コードとカーネル コードは独立して開発およびコンパイルされるので、ヘッダー ファイルで extern "C" リンクを使用してカーネル関数宣言を囲むか、カーネル コードで関数全体を囲みます。

```cpp

  extern "C" {
               void kernel_function(int *in, int *out, int size);
             }
  ```

***


## Vitis での作成およびエミュレーション

このチュートリアルの各モジュールでは、GUI またはコマンド ライン (このチュートリアルの内容ではより効率的) を使用して Vitis を実行できます。

<details>
  <summary><b>クリックして展開 (<code>Vitis GUI</code> の手順)</b></summary>

### Vitis を GUI で使用

ヒント: 次の画像を表示する場合、右クリックで別のタブやウィンドウで開くようにすると、内容を表示されたままにできます。

1. ターミナルを開きます
2. Vitis を設定して起動します
3. \[File] → \[Import] をクリックします <a href="../images/gui1_import.png">(画像)</a>
4. Vitis プロジェクトでエクスポートした ZIP ファイルのデフォルトをそのまま使用し、\[Next] をクリックします <a href="../images/gui2_next.png">(画像)</a>
5. 次のウィンドウで \[Browse] をクリックし、./docs/module1\_baseline/project に移動します <a href="../images/gui3_browse.png">(画像)</a>
6. vitis\_export\_archive.ide.zip を選択し、\[OK] をクリックします <a href="../images/gui4_zip.png">(画像)</a>
7. 次のウィンドウで \[Finish] をクリックします <a href="../images/gui5_finish.png">(画像)</a>

### プラットフォームの設定

1. 中央のウィンドウ ペインで、プラットフォーム リンクの直後にある 3 つのドット アイコン <code>\[...]</code> をクリックします <a href="../images/gui6_platform.png">(画像)</a>
2. システムからアクセス可能なプラットフォームを指定します

### デザインのビルドとエミュレーション

\[Assistant] ウィンドウに 3 つのメイン フローが表示されます。

1. <code>Emulation-SW</code>: 論理レベルでデザインを検証します
2. <code>Emulation-HW</code>: カーネルをサイクル精度表記にコンパイルして、メトリクスをより正確に測定します
3. <code>Hardware</code>: フル コンパイルを開始し、ビットストリームを生成します

ソフトウェア エミュレーションを実行します (数分かかります)

ハードウェア エミュレーション実行します (約 10 ～ 20 分かかります)

ハードウェア エミュレーションが完了したら、次の Vitis アナライザーのセクションに進みます。

</details>

<br>

***または***

<details>
  <summary><b>クリックして展開 (<code>make</code> の手順)</b></summary>
### **make** の使用

1. ターミナルを開きます。
2. Vitis を設定します。
3. `./build` に移動します。
4. `make run TARGET=sw_emu` を実行 (論理エミュレーションの場合)
5. `make run TARGET=hw_emu` を実行 (より詳細なエミュレーションの場合。10 分以上かかります)
   * 現時点では "hw" ターゲットを実行すると 1 時間以上かかってしまうので、実行しないようにしてください。

</details>

***


## Vitis アナライザーを使用したアプリケーションのエンド ツー エンドのタイムライン解析

Vitis アナライザーは、システム全体からカーネルの詳細まで、デザインのさまざまな側面を参照できるグラフィカル ツールです。

<details>
  <summary><b>クリックして展開 (<code>Vitis Analyzer</code></b> の手順)</summary>

1. ターミナル、Vitis を設定します

2. <code>vitis\_analyzer \&</code> を実行します

3. \[File] → \[Open Summary] をクリックします

4. <code>./build</code> を参照します

5. cholesky\_kernel\_hw\_emu\_xclbin\_<b>run</b>\_summary (青色の play アイコンが前に付いています)

6. Vitis アナライザーの使用方法は、この 45 秒の [looping gif](../images/analyzer_anim.gif) を参照してください。

   次を確認してください。

   1. プロファイル サマリ
   2. ガイダンス レポート - 改善点を示します
   3. アプリケーション タイムライン - 詳細は次を参照してください

アプリケーション タイムラインの構造は次のとおりです。

* *ホスト*

  * **\[OpenCL API Calls]**: すべての OpenCL API 呼び出しがここでトレースされます。アクティビティ時間はホストの視点から測定されます。

  * **\[General]**: clCreateProgramWithBinary、clCreateContext、および clCreateCommandQueue などの一般的な OpenCL API 呼び出しがここでトレースされます。

  * **\[Queue]**: 特定のコマンド キューに関連する OpenCL API 呼び出しがここでトレースされます。これには clEnqueueMigrateMemObjects および clEnqueueNDRangeKernel などのコマンドが含まれます。ユーザー アプリケーションで複数のコマンド キューが作成された場合は、このセクションにすべてのキューとそのアクティビティが表示されます。

  * **\[Data Transfer]**: このセクションでは、ホストからデバイス メモリまでの DMA 転送がトレースされます。OpenCL ランタイムにインプリメントされる DMA スレッドは複数あり、通常は同数の DMA チャネルがあります。DMA 転送は clEnqueueMigrateMemObjects などの OpenCLAPI を呼び出して、ユーザー アプリケーションにより開始されます。これらの DMA 要求がランタイムに転送され、スレッドの 1 つに割り当てられます。ホストからデバイスまでのデータ転送は \[Write] の下、デバイスからホストまでのデータ転送は \[Read] の下に表示されます。

  * **\[Kernel Enqueues]**: ホスト プログラムによりエンキューされたカーネルが表示されます。ここに示されるカーネルを、デバイスのカーネル/CU と混同しないようにしてください。この「カーネル」は NDRangeKernels と、OpenCL コマンドの clEnqueueNDRangeKernels および clEnqueueTask で作成されるタスクのことです。これらはホストの視点から測定された時間に対してプロットされます。複数のカーネルが同時に実行されるようにスケジュールでき、実行がスケジュールされた時点からカーネル実行の終了までがトレースされます。重複するカーネル実行の数に応じて、複数のエントリが異なる行に表示されます。

* **\[Device "name"]**

  **\[Binary Container “name”]**: バイナリ コンテナー名。

  * **\[Accelerator "name"]**: FPGA 上の計算ユニット (アクセラレータ) の名前。

</details

***


## Vitis HLS を使用したカーネル最適化

デバイス LUT およびフロップにインプリメントされるようになっている C++ カーネル (別名「ファブリック」) は、高位合成ツールの Vitis HLS で自動的にコンパイルされます。このチュートリアルでは、Vitis HLS を手動で実行して、基盤となる合成テクノロジとコレスキー カーネル アルゴリズムに関する追加情報を示します。

<details>
  <summary><b>クリックして展開(<code>Vitis HLS</code> の手順)</b></summary>

1. ターミナル、Vitis を設定します
2. <code>./build/cholesky\_kernel\_hw\_emu/cholesky\_kernel</code> に移動します。
   * その階層には、もう 1 つ cholesky\_kernel ディレクトリがあるはずです。
3. <code>vitis\_hls -p cholesky\_kernel \&</code> を実行して、Vitis 高位合成 GUI を起動します。
4. Vitis HLS で高位合成レポートが表示されるようになりました。
5. GUI で **\[Synthesis Summary Report]** ウィンドウを展開します。
6. **\[Performance \& Resources]** セクションでループと関数を展開します。
7. このクリップの **\[II violation]** を右クリックして、コード内の II 違反を見つけます。[**50s HLS looping GIF**](../images/HLS_anim.gif)

注記: 元の Vitis HLS ウィンドウ レイアウトに戻すには、\[Windows] メニューから \[Reset Perspective] をクリックします。

#### 開始間隔 (II)

この関数では、2 つのループに対する II 違反が 8 になっています。その 1 つは次のようになります。

```cpp
// Loop only takes one element every 8 clock cycles!!!
// We expected one every cycle (II of 1)
for (int k = 0; k < j; k++)
{
    tmp += dataA[j][k] * dataA[j][k];
}
```

このバージョンのアルゴリズムでは、累算ありの double データ型を使用しているので、操作が実行されて終了するまでシリコンが 300MHz で 8 サイクルで動作する必要があります。そのため、サンプルは 8 サイクルの間隔で 1 つずつしか計算できません。  これは、1 つ目のボトルネックです (次のモジュールで対処します)。

#### カーネル レイテンシ

それでは、レイテンシについて見てみましょう。

`cholesky_kernel/solution/syn/report/cholesky_kernel_csynth.rpt`

    * Loop:
    +--------------------+--------+---------+-------------+-----------+-----------+------------+----------+
    |                    | Latency (cycles) |  Iteration  |  Initiation Interval  |    Trip    |          |
    |       Loop Name    |  min   |   max   |   Latency   |  achieved |   target  |    Count   | Pipelined|
    +--------------------+--------+---------+-------------+-----------+-----------+------------+----------+
    |- VITIS_LOOP_32_..  |       ?|        ?|            3|          1|          1|           ?|    yes   |
    |- Loop_first_col    |       ?|        ?|           34|          1|          1|           ?|    yes   |
    |- Loop_col          |       ?|        ?|            ?|          -|          -|           ?|    no    |
    | + Loop_diag        |      17|  2097161|           18|          8|          1| 1 ~ 262144 |    yes   |
    | + Loop_row         |       ?|        ?| 61 ~ 2097205|          -|          -|           ?|    no    |
    |  ++ Loop_vec_mul   |      17|  2097161|           18|          8|          1| 1 ~ 262144 |    yes   |
    |- VITIS_LOOP_67_..  |       ?|        ?|            4|          1|          1|           ?|    yes   |
    +--------------------+--------+---------+-------------+-----------+-----------+------------+----------+

次の点に注意してください。

- <code>VITIS</code> が名前の先頭に付いたループ: Vitis HLS で自動的にラベル付けされたループで、ソース コードにはラベルは付いていません。  表に含まれているそれ以外のループにはラベルが付いています。
- クエスチョン マーク (?) は、関数へのスカラー入力に依存するために計算できないメトリクスを表します。この例では、行列サイズはコンフィギュレーション可能であり、レイテンシはそのサイズによって異なります。
- 最後の Pipelined 列は、各サイクルで入力を処理するようにループが制約されているかどうかを示します。単純なループまたはほとんどの入れ子のループは、通常ツールによって自動的にパイプライン処理されます。

コレスキー関数への入力として、ユーザーが行列 N のサイズを渡します (この例の場合、64 でした)。

最初のループでは、開始間隔 II=1 で N 回の繰り返しが必要で、II=3 なので、終了するのに N x 3 かかります。<code>Loop\_first\_col</code> ループは N x 34 かかり、<code>Loop\_col</code> ループは N 回 ((<code>Loop\_diag</code> は N \* 18) + (<code>Loop\_row</code> は N \* (N + 18)) 実行されます。最後のループは、最初のループと同様に N 回の繰り返しを必要とします。

大体の所要時間は <code> N(18N+N(18N+residual1)+residual2) = 18N<sup>3</sup> + (18+residual1)N<sup>2</sup> + residual2.N と見積もられます。 </code>

このため、基本的にアルゴリズムのレイテンシは、行列のサイズである N の 3 乗で計算できます。

#### カーネル用の C++ テストベンチの追加

このチュートリアルでは、カーネルをラップして Vitis HLS 環境でシミュレーションするあらかじめ作成された C++ の main プログラムを用意しています。

**使用法:**

1. ターミナルの <code>docs</code> ディレクトリから次を実行します。

       cp -r ./hls_tb ./module1_baseline/build/cholesky_kernel_hw_emu/cholesky_kernel
       cp ./module1_baseline/src/cholesky_kernel.hpp ./module1_baseline/build/cholesky_kernel_hw_emu/cholesky_kernel/hls_tb

2. Vitis HLS GUI が閉じている場合は、開き直します。

       cd ./module1_baseline/build/cholesky_kernel_hw_emu/cholesky_kernel
       vitis_hls -p cholesky_kernel &

3. GUI の \[Explorer] ウィンドウの左ペインで \[Source] の下の \[Test Bench] を見つけます。右クリックして \[Add file] を選択し、test\_hls.cpp を選択します。この操作を ./hls\_tb/tb\_data の matrix\_input.dat と golden\_result.dat の 2 つのデータ ファイルに対して繰り返します。

4. メインメニューで \[Project] → \[Run C simulation] をクリックします。これにより、「Csim」と呼ばれる純粋な論理シミュレーションが実行されます。HLS の合成するものは使用されません。

5. \[Project] → \[Run C simulation] をクリックします。

6. \[Solution] → \[Run C Synthesis ] -> \[Active Solution] をクリックします。

7. \[Solution] → \[Run C/RTL Cosimulation] をクリックします。  ポップアップ ウィンドウで \[OK] をクリックします。

Vitis HLS 協調シミュレーションは、クロック サイクルで実際レイテンシを示すサイクル精度の RTL シミュレーションを実行します。  テストベンチでは、行列は 16x16 です。

</details>

***


## モジュール 1 のまとめ

**重要ポイント:**

- Alveo カードのアルゴリズムをアクセラレーションするには、プログラムにホストとカーネル デザイン ユニットが必要です。
- Vitis はアプリケーションのビルドをしやすくし、ホスト カーネル通信をイネーブルにするドライバーを提供します。
- Vitis には操作のシーケンスを理解するのに役立つ解析ツールが含まれます。
- Vitis HLS は C コード アルゴリズムをハードウェア言語に変換して、ザイリンクス デバイスにインプリメントするコンパイラ テクノロジです。

***


#### 次の手順

次は[**モジュール 2**](../module2_pipeline/README.md) に進みます。

***


<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align= center class="sphinxhide"><b><a href="../../../../../README.md">メイン ページに戻る</a> &mdash; <a href="../../../../README.md">ハードウェア アクセラレータ チュートリアルの初めに戻る</a></b></p></br>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
