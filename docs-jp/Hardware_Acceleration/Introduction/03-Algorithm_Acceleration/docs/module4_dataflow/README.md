<p align="right"><a href="../../../../../../README.md">English</a> | <a>日本語</a></p>

**モジュール 4** のコードおよびファイル ([モジュール 1](../module1_baseline) と同じ手順で Vitis、Vitis アナライザーおよび Vitis HLS を設定)

> **このモジュールの内容:**   
1> テンプレート化された関数を使用し、適用されたプログラマブル係数で計算ループを複製  
2> Use the Vitis HLS `dataflow` プラグマを使用  
3> フル コンパイルを実行してカードをプログラム

<details>
  <summary><b>クリックして展開 (<code>dataflow</code> プラグマの詳細</b>)</summary>

<code>DATAFLOW</code> プラグマは、タスク レベルのパイプライン処理をイネーブルにして関数およびループをオーバーラップできるようにし、レジスタ トランスファー レベル (RTL) インプリメンテーションでの同時実行性を増加してデザイン全体のスループットを向上します。

C 記述では、すべての演算が順次に実行されます。pragma HLS allocation などのリソースを制限する指示子を指定しない場合は、Vivado 高位合成 (HLS) ではレイテンシを最小限に抑え、同時実行性を向上するように処理されます。ただし、データ依存性のためにこれが制限されることがあります。たとえば、配列にアクセスする関数またはループは、完了する前に配列への読み出し/書き込みアクセスをすべて終了する必要があります。そのため、そのデータを消費する次の関数またはループの演算を開始できません。<code>DATAFLOW</code> 最適化を使用すると、前の関数またはループがすべての演算を完了する前に、次の関数またはループの演算を開始できるようになります。

<code>DATAFLOW</code> プラグマを指定した場合、HLS ツールで順次関数またはループ間のデータフローが解析され、プロデューサー関数またはループが完了する前にコンシューマー関数またはループの演算を開始できるように、ピンポン RAM または FIFO に基づいてチャネルが作成されます。これにより関数またはループを並列実行でき、レイテンシが削減されて RTL のスループットが向上します。

開始間隔 (II) (関数またはループの開始から次の関数またはループの開始までのサイクル数) が指定されていない場合は、HLS ツールで開始間隔が最小になるようにし、データが使用可能になったらすぐに演算を開始できるようにすることが試みられます。

ヒント: config\_dataflow コマンドは、<code>dataflow</code> 最適化で使用されるデフォルトのメモリ チャネルと FIFO の深さを指定します。詳細は、『Vivado Design Suite ユーザー ガイド: 高位合成』 (UG902) の config\_dataflow に関する説明を参照してください。<code>DATAFLOW</code> 最適化が機能するようにするには、デザイン内でデータが 1 つのタスクから次のタスクに流れる必要があります。次のコーディング スタイルを使用すると、HLS ツールで <code>DATAFLOW</code> 最適化が実行されなくなります。

+ シングル プロデューサー コンシューマー違反
+ タスクのバイパス
+ タスク間のフィードバック
+ タスクの条件付き実行
+ 複数の exit 条件を持つループ

**重要**: これらのコーディング スタイルのいずれかが使用されている場合、HLS ツールでメッセージが表示されます。<code>DATAFLOW</code> 最適化は実行されません。

<code>STABLE</code> プラグマを使用して <code>DATAFLOW</code> 領域内の変数を安定とマークすると、変数が同時に読み出しまたは書き込みされるのを回避できます。

最後に、<code>DATAFLOW</code> 最適化には階層インプリメンテーションはありません。サブ関数またはループに最適化が有益な可能性のあるタスクが含まれる場合、最適化をそのループまたはサブ関数に適用するか、サブ関数をインライン展開する必要があります。

**構文**

C ソースの領域、関数、またはループ内に配置します。

```cpp
#pragma HLS DATAFLOW
```

**例**

次の例では、wr\_loop\_j ループ内で <code>DATAFLOW</code> 最適化を指定しています。

```cpp
wr_loop_j: for (int j = 0; j < TILE_PER_ROW; ++j) {
#pragma HLS DATAFLOW
   wr_buf_loop_m: for (int m = 0; m < HEIGHT; ++m) {
      wr_buf_loop_n: for (int n = 0; n < WIDTH; ++n) {
      #pragma HLS PIPELINE
      // should burst WIDTH in WORD beat
         outFifo >> tile[m][n];
      }
   }
   wr_loop_m: for (int m = 0; m < HEIGHT; ++m) {
      wr_loop_n: for (int n = 0; n < WIDTH; ++n) {
      #pragma HLS PIPELINE
         outx[HEIGHT*TILE_PER_ROW*WIDTH*i+TILE_PER_ROW*WIDTH*m+WIDTH*j+n] = tile[m][n];
      }
   }
}
```

</details>

#### コレスキー カーネルのコード変更

このモジュール 4 では、アルゴリズムのコードを <code>cholesky\_kernel.hpp</code> ヘッダーファイルに移動します。

並列処理と並列計算の数は明示的に指定されるようになっています。これは、`NCU` (`cholesky_kernel.cpp` で `#define NCU 16` に設定された定数) で決まります。

`NCU` は、テンプレート パラメーターとして `chol_col_wrapper` に渡されます (次を参照)。  `DATAFLOW` プラグマが、16 回呼び出される `chol_col` ループに適用されます。

```cpp
template <typename T, int N, int NCU>
void chol_col_wrapper(int n, T dataA[NCU][(N + NCU - 1) / NCU][N], T dataj[NCU][N], T tmp1, int j)
{
#pragma HLS DATAFLOW

Loop_row:
    for (int num = 0; num < NCU; num++)
    {
#pragma HLS unroll factor = NCU
        chol_col<T, N, NCU>(n, dataA[num], dataj[num], tmp1, num, j);
    }
}
```

`DATAFLOW` が確実に適用されるように、dataA は複数の `NCU` 部分に分割されます。

最後に、ループは `NCU` 係数を使用して展開されます。この係数は、データのチャンクが処理されるたびに、`chol_col` のコピーが `NCU` (たとえば 16) 個作成されることを意味します。

#### デザインの実行

モジュール 1 と同じ手順:

+ ハードウェア エミュレーションを実行します。
+ Vitis アナライザーを実行します。
+ Vitis HLS を実行し、結果をデータフロー ビューアーを確認します。これは、合成サマリ レポートから dataflow が適用される関数 (<code>chol\_col\_wrapper</code>) を右クリックすると表示できます。[**このアニメーション**](../images/HLS_dataflow_anim.gif)を参照して、アクセス方法および複製が適用されたことを確認する方法を確認してください。

#### 結果のサマリ

CPU<sup>(\*)</sup> での実行を含む、すべてのモジュールの実行を比較した実際の結果は、次のようになります。

| モジュール| CPU| モジュール 1| モジュール 2| モジュール 3| モジュール 4 (NCU = 16)
|----------|----------:|----------:|----------:|----------:|----------:
| 実行時間 (µs)| 21461| 793,950| 793,732| 536,784| 11,698
| 速度アップ (CPU 基準)| 1| 0.03x| 0.03x| 0.04x| 1.83x
| 速度アップ| 該当なし| 1| 1| 1.48x| 68x

(\*): 基準 CPU は、Intel® Xeon® Processor E5-2640 v3 (Nimbix で利用可能) です。

#### まとめ

これで、この Vitis ハードウェア アクセラレータの入門チュートリアルは終了です。その他の Vitis のデザイン最適化チュートリアルを参照するには、[ここ](../../../../README.md)をクリックしてください。

***

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align= center class="sphinxhide"><b><a href="../../../../../README.md">メイン ページに戻る</a> &mdash; <a href="../../../../README.md">ハードウェア アクセラレータ チュートリアルの初めに戻る</a></b></p></br>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
