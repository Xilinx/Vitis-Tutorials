<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>アクセラレーション FPGA アプリケーションの最適化: ブルーム フィルターの例</td>
 </tr>
</table>

# 3\. メモリ転送の最適化

先ほどの演習では、ドキュメントを 100 個使用してアプリケーションのベースライン パフォーマンスを確立し、メモリのボトルネックを特定しました。この演習では冗長メモリ アクセスを削除し、カーネルとグローバル メモリ間のデータ転送を最適化します。

`gmem3` に大きなメモリ ボトルネックがあって関数でストールが発生していることがわかっています。ソース コードを確認すると、どの反復にもそのメモリにランダム アクセスがあって、これが原因でシステムが最高 50% ストールしています。

これは、比較的新しい CPU では問題にならないかもしれません。というのも、新しいプロセッサのほとんどにはハードウェア制御されたキャッシュがあって、非常に高速なローカル キャッシュに自動的にデータが格納されるようになっているからです。ただし FPGA 上では、データを作成し、そのデータをハードウェア カーネルのローカル配列にコピーして、明確に設定しておく必要があります。

次に、グローバル メモリの場合と同じように、ローカル配列から読み出しが実行されます。グローバル メモリからローカル メモリへデータが呼び出されれば、グローバル メモリにアクセスする必要はありません。グローバル メモリ アクセスがボトルネックになっている場合は、ローカル メモリの方がグローバル メモリよりも帯域幅が非常に高いため、システムのスループットを改善できるのが利点です。

`bloom_filter` 配列のサイズは 64 KB なので、グローバル メモリからローカル メモリへコピーして、`bloom_filter` 配列からのメモリ アクセスを最適化できます。また `profile_weights` 配列と `input_words` 配列にも、メモリ ボトルネックがあります。

`profile_weights` のサイズは **(2\^24)\*8B=128 MB** です。このサイズはデバイス内の使用可能なメモリのサイズよりも大きいので、ローカル メモリにはこのデータをコピーできません。ブルーム フィルターでワードが検出されるかどうかによりますが、この配列へのアクセス数は少ないので、ローカル メモリへはコピーしません。

`input_doc_words` 配列の各ワードにはカーネルから一度だけアクセスがあり、またこの配列でバースト推論が実行されます。したがって、アプリケーション側ではデータが再利用されないので、できればこの配列をローカル メモリにコピーしないようにしてください。

## ホスト コードの変更

1. 最初のカーネルへのエンキュー呼び出しでのみ、グローバル メモリからローカル メモリへ `bloom_filter` 配列を転送できるよう、58 行目にブール型の引数 `flag` を追加します。

   ```
   bool flag=false;
   ```

   `bloom_filter` 配列のデータは定数なので、データを一度送るだけで転送効率を改善できます。

2. 84 行目の最初の反復でのみ `flag` を `true` に設定し、カーネル引数を設定します。

   ```
    if(iter==0)
    flag=true;
   kernel.setArg(6,flag);
   ```

## カーネル コードの変更

1. `runOnCPU` 関数にブール型引数 `load_weights` を追加します。

   カーネルを複数回呼び出しているので、カーネルへの最初の呼び出しでのみ `bloom_filter` 配列をローカル メモリにコピーするのがより効率的です。この引数は DDR からローカル メモリへ `bloom_filter` を読み込むべきかどうかを示すために使用します。

   ```
   void runOnfpga (unsigned int* doc_sizes,unsigned int* input_doc_words,unsigned int* bloom_filter,unsigned long* profile_weights,unsigned long* fpga_profileScore,unsigned int total_num_docs)
   ```

   上記の行を次のように変更します。

   ```
   void runOnfpga (unsigned int* doc_sizes,unsigned int* input_doc_words,unsigned int* bloom_filter,unsigned long* profile_weights,unsigned long* fpga_profileScore,unsigned int total_num_docs,bool load_weights)
   ```

2. `load_weights` 引数に `s_axilite` pragma を追加します。

   ```
   #pragma HLS INTERFACE s_axilite port=load_weights bundle=control
   ```

3. 関数本文にある最後の pragma の後、次の行を追加します。

   ```
   static unsigned int bloom_filter_local[bloom_filter];
   if(load_weights==true)
   memcpy(bloom_filter_local,bloom_filter,bloom_size*sizeof(unsigned int));
   ```

   上記の行により `bloom_filter_local` 配列が作成されます。この配列は、ホスト コードで `load_weights` が TRUE に設定され、イネーブルになっている場合にのみ、フィルター係数を格納します。この `bloom_filter_local` 配列はブロック RAM リソースを使用して FPGA 上で合成されます。ブロック RAM へアクセスするときのレイテンシは、オンボードの DDR メモリへ合うセスするときよりも大幅に低くなります。

4. キャッシュされた `bloom_filter_local` 配列を作成できたので、次は、その配列を使用するためコードの残りの部分を変更します。内部ループにある次の行を変更します (59 行目と 60 行目)。

   ```
    bool inh1 = bloom_filter[ hash1 >> 5 ] & ( 1 << (hash1 & 0x1f));
    bool inh2 = bloom_filter[ hash2 >> 5 ] & ( 1 << (hash2 & 0x1f));
   ```

   上記の行を次のように変更します。

   ```
   bool inh1 = bloom_filter_local[ hash1>> 5 ] & ( 1 << (hash1 & 0x1f));
   bool inh2 = bloom_filter_local[ hash2 >> 5 ] & ( 1 << (hash2 & 0x1f));
   ```

5. このコードを見ると累積和を格納するために、`fpga_profileScore` に繰り返しアクセスがあることがわかります。何度もこの `fpga_profileScore` にアクセスするのではなく、ローカル変数を使用してこの和を計算し `fpga_profile_score` に一度だけその和を格納します。こうすることでグローバル メモリへのアクセス数を緩和できます。

   52 行目の `for` ループの後に、ローカル変数 `ans` を作成します。

   ```
   unsigned long ans=0;
   ```

6. 76 行目に `fpga_profile_score[doci]` の代わりに `ans` を挿入します。

   ```
   fpga_profile_score[doci] += profile_weights[word_id] * (unsigned long)frequency;
                 }
             }
   ```

   上記の行を次のように変更します。

   ```
   ans += profile_weights[word_id] * (unsigned long)frequency;
                }
            }
             fpga_profile_score[doc]=ans;
   ```

これですべての変更を完了させました。最適化されたコードを使ってハードウェア エミュレーションを実行してください。

## ハードウェア エミュレーションの実行

`makefile` ディレクトリに移動し、次のコマンドを使用してハードウェア エミュレーションを実行します。

```
make run TARGET=hw_emu STEP=localbuf SOLUTION=1 NUM_DOCS=100
```

## ハードウェア エミュレーションのレポートの生成

次のコマンドを使用して、プロファイル サマリとタイムライン トレースのレポートを生成します。

```
make view_report TARGET=hw_emu STEP=localbuf
```

## ハードウェア エミュレーションのプロファイル サマリ

1. Vitis 解析で `Profile Summary` レポートをクリックすると、次が表示されます。

   ![](./images/profile_localbuf.PNG)

2. プロファイル サマリ レポートからパフォーマンス データを取り込んで、次の表に追加します。

| 演習                            | Number of Documents   | Average Document Size(kB) | Time (Hardware) (ms) | Throughput (MBps) |
 | :-----------------------        | :----------- | ------------: | ------------------: | ----------------: |
 | CPU                       |     100 |           16 |              11.23 |   124.67        |
 | baseline                   |     100 |           16 |             38|  157.236            |
 | localbuf                 | 100 | 16| 1.67 | 838.32
 ---------------------------------------

## ハードウェア エミュレーションのタイムライン トレース

Vitis 解析で `Timeline Trace` レポートをクリックすると、次が表示されます。

![](./images/profile_localbuf.PNG)

データ転送がすべて完了した後にのみ演算が実行され、実行時間のほとんどがカーネルにあてられているのがわかります。

![](./images/new_tutorial_bloom_localbuf_timeline_hw_emu.PNG)

## ハードウェア エミュレーションのカーネル詳細トレース レポートの確認

カーネル詳細トレース レポートはハードウェア エミュレーション実行の一部として自動的に生成されます。カーネル詳細トレースの次の図は、メモリ転送が最適化されている CU ストールを示しています。

![](images/new_tutorial_bloom_localbuf.PNG)

CU ストールは以前と比べると緩和されています。カーネル詳細トレース レポートを見ればわかるように、ストールは主に `gmem4` とオーバーラップしています。これは、グローバル メモリから `h_profile_weights` 配列がランダムにアクセスされているためです。

データ量が大きいのでグローバル メモリをローカル メモリには転送できませんし、またアクセスがランダムであるため、バースト アクセスを使ってデータ転送を最適化することもできません。アクセスの数が少ないので、このストールが比較的少ないことが原因でパフォーマンスは影響を受けます。

## 次のステップ

次のセクションでは、[データフローベースのストリーミング](./dataflow.md)を使用して、グローバル メモリからデータ転送をストリーミングすることにより、パフォーマンスを改善する方法を説明します。</br>

<hr/>
<p align="center"><b><a href="../../docs/vitis-getting-started/README.md">入門ガイドの最初に戻る</a> &mdash; <a href="./README.md">チュートリアルの最初に戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
