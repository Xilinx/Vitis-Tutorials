<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>複数の計算ユニットの使用</h1>
 </td>
 </tr>
</table>

# 概要

このチュートリアルでは、FPGA 上のカーネル インスタンスの数を増やすための柔軟なカーネル リンク プロセスを示します。カーネルの各インスタンスは、計算ユニット (CU) とも呼ばれます。CU 数を増加するこのプロセスを使用すると、統合されたホスト/カーネル システムの並列処理が向上します。

# チュートリアルの概要

デフォルトでは、Vitis™ コア開発キットはカーネルごとに CU を 1 つ作成します。ホスト プログラムは、異なるデータ セットに対して同じカーネルを複数回使用できます。この場合、カーネルに対して複数の CU を生成して、これらの CU を同時実行すると、システム全体のパフォーマンスを向上できます。

詳細は、[アプリケーションのビルドおよび実行](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/xia1553473418160.html)の[カーネルの複数インスタンスの作成](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/Chunk595166594.html#yzb1524519238289)を参照してください。

このチュートリアルでは、次を実行します。

1. ハードウェア エミュレーションを実行し、エミュレーション レポートを参照して、カーネルが順に複数回実行されることを確認します。
2. コマンドを順不同に実行できるようにホスト コードを変更します。
3. カーネル リンク プロセスを変更し、同じカーネルの CUを複数作成します。
4. ハードウェア エミュレーションを再実行し、CU が同時実行されることを確認します。

このチュートリアルでは、画像フィルター例を使用して複数 CU の利点を示します。ホスト アプリケーションは、画像を処理して Y、U、および V プレーンを抽出し、カーネルを 3 回実行して画像の各プレーンをフィルター処理します。デフォルトでは、FPGA にはカーネルの CU が 1 つしか含まれないので、これら 3 つのカーネルは同じハードウェア リソースを使用して順次実行されます。このチュートリアルでは、CU の数を増加し、このカーネル実行を並列で実行する方法を示します。

# 開始前の注意点

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド
* 2019.2 Vitis コア開発キット リリースおよび xilinx\_u200\_xdma\_201830\_2 プラットフォーム。必要であれば、その他のバージョンおよびプラットフォームも使用できます。

> **重要:**
>
> * 例を実行する前に、[インストール](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/vhc1571429852245.html)に示すように Vitis コア開発キットをインストールしておく必要があります。
> * ザイリンクス Alveo™ データセンター アクセラレータ カードでアプリケーションを実行する場合は、『Alveo データセンター アクセラレータ カード入門ガイド ([UG1301](https://japan.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=j_ug1301-getting-started-guide-alveo-accelerator-cards.pdf)) で説明されるように、カードとソフトウェア ドライバーを正しくインストールしてください。

## チュートリアル リファレンス ファイルの入手

1. リファレンス ファイルを入手するには、ターミナルに `git clone http://github.com/Xilinx/Vitis-Tutorials` と入力します。
2. `using-multiple-cu` ディレクトリに移動し、`reference-files` ディレクトリにアクセスします。

# makefile フロー

このチュートリアルで使用する makefile は、`using-multiple-cu/reference-files/Makefile` に含まれます。最上位設定には、次が含まれます。

* **VPP**: カーネル コードをコンパイルする Vitis コンパイラ パス。

* **EMCONFIGUTIL**: エミュレーション コンフィギュレーション ファイル `emconfig.json` を作成するユーティリティ ファイルのパス。

* **DSA**: ターゲット プラットフォーム。

* **LFLAGS**: ホスト コード リンカー オプション用に OpenCV™ ライブラリを使用するリンカー オプション。

  ```
  -Wl,-rpath,${XILINX_VIVADO}/lnx64/tools/opencv/opencv_gcc -L${XILINX_VIVADO}/lnx64/tools/opencv/opencv_gcc -lopencv_core -lopencv_highgui
  ```

* **EXE\_OPT**: コマンド ライン引数として渡されるランタイム オプション: コンパイル済みカーネル xclbin ファイル, 入力画像。

## ハードウェア エミュレーションの実行

次のコマンドでハードウェア エミュレーションを実行します。

```
make check MODE=hw_emu
```

ハードウェア エミュレーション (`hw_emu`) では、カーネル コードがハードウェア モデル (RTL) にコンパイルされ、専用シミュレータで実行されますが、残りのシステムでは C シミュレータが使用されます。ビルドおよび実行にかかる時間は長くなりますが、詳細でサイクル精度の高いカーネル アクティビティが表示されます。このターゲットは、FPGA に含まれるロジックの機能をテストして、最初のパフォーマンス見積もりを取得する場合に便利です。

> **注記:** ホスト ソフトウェアおよびハードウェアのビルド方法は、[アプリケーションのビルド](/docs/Pathway3/BuildingAnApplication.md)演習を参照してください。

## ホスト コードの確認

1. エミュレーション run を実行中、別のターミナルで `src/host/host.cpp` ファイルを開きます。

2. 255 ～ 257 行目を確認してください。Y、U、および V チャネルを処理するためにフィルター関数が 3 回呼び出されています。

   ```
   request[xx*3+0] = Filter(coeff.data(), y_src.data(), width, height, stride, y_dst.data());
   request[xx*3+1] = Filter(coeff.data(), u_src.data(), width, height, stride, u_dst.data());
   request[xx*3+2] = Filter(coeff.data(), v_src.data(), width, height, stride, v_dst.data());
   ```

   この関数は、80 行目から記述されています。下の抜粋部分で、カーネル引数が設定され、カーネルが `clEnqueueTask` コマンドにより実行されます。

   ```
    // Set the kernel arguments
    clSetKernelArg(mKernel, 0, sizeof(cl_mem),       &mSrcBuf[0]);
    clSetKernelArg(mKernel, 1, sizeof(cl_mem),       &mSrcBuf[1]);
    clSetKernelArg(mKernel, 2, sizeof(unsigned int), &width);
    clSetKernelArg(mKernel, 3, sizeof(unsigned int), &height);
    clSetKernelArg(mKernel, 4, sizeof(unsigned int), &stride);
    clSetKernelArg(mKernel, 5, sizeof(cl_mem),       &mDstBuf[0]);

   // Schedule the writing of the inputs
   clEnqueueMigrateMemObjects(mQueue, 2, mSrcBuf, 0, 0, nullptr,  &req->mEvent[0]);

   // Schedule the execution of the kernel
   clEnqueueTask(mQueue, mKernel, 1,  &req->mEvent[0], &req->mEvent[1]);
   ```

   これら 3 つの `clEnqueueTask` コマンドは、1 つの順序どおりのコマンド キューを使用してキューに追加されます (75 行目)。この結果、すべてのコマンドがキューに追加された順序で実行されます。

   ```
   Filter2DDispatcher(
           cl_device_id     &Device,
           cl_context       &Context,
           cl_program       &Program )
     {
           mKernel  = clCreateKernel(Program, "Filter2DKernel", &mErr);
           mQueue   = clCreateCommandQueue(Context, Device, CL_QUEUE_PROFI   LING_ENABLE, &mErr);
           mContext = Context;
           mCounter = 0;
     }
   ```

## エミュレーション結果

1. まず、生成されたプロファイル サマリ レポート (`profile_summary.csv`) を確認します。

   ```
   vitis_analyzer ./profile_summary.csv
   ```

   * このレポートには、アプリケーションがどうのように実行されたかに関連するデータが表示されます。
   * 「Top Kernel Execution」セクションを見ると、カーネルが 3 回実行されていることがわかります。

   > **注記:** run ディレクトリには、`xrt.ini` というファイルが含まれます。このファイルには、プロファイル サマリ レポートおよびタイムライン トレースなどの追加レポートを生成するランタイム オプションが含まれています。

2. 次にタイムライン トレース レポート (`timeline_trace.csv`) を確認します。

   ```
   vitis_analyzer ./timeline_trace.csv
   ```

   アプリケーション タイムライン レポートは、ホストとデバイスのイベント情報を収集し、共通のタイムラインに表示します。これは、システムの全体的な状態とパフォーマンスを視覚的に表示して理解するのに役立ちます。

   * タイムラインの最下部に、ホストからキューに追加された各カーネルに 1 本ずつ、合計 3 本の青いバーがあります。1 つの順序どおりのコマンド キューが使用されているので、ホストはカーネル実行を順にキューに追加します。
   * 青いバーの下に、各カーネル実行に 1 本ずつ、合計 3 本の緑色のバーがあります。これらは、FPGA で順に実行されます。 ![順序通りのカーネル エンキュー](./images/serial_kernel_enqueue_vitis.JPG)

## カーネルを同時にキューに追加するためのホスト コードの改善

1. 75 行目の `src/host/host.cpp` ホスト ファイルを変更します。  
これはコマンド キューを順不同コマンド キューとして宣言します。

   コード変更前:

   ```
   mQueue   = clCreateCommandQueue(Context, Device, CL_QUEUE_PROFILING_ENABLE, &mErr);
   ```

   コード変更後:

   ```
   mQueue   = clCreateCommandQueue(Context, Device, CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &mErr);
   ```

2. **(オプション)** 変更したホスト コードでハードウェア エミュレーションを実行します。

   ハードウェア エミュレーションを実行する場合、タイムライン トレースを使用して、順不同コマンド キューを使用することによりカーネルがほぼ同時に実行されるように要求できます (青のバーはホストでスケジュールされるカーネル エンキュー リクエストを示しています)。

   ホストはこれらの実行を同時にスケジューリングできますが、FPGA 上には 1 つしか CU がないので、2 つ目および 3 つ目の実行要求が遅れます (FPGA ではカーネルはまだ順次実行されるので)。  
![シーケンシャル カーネル](./images/sequential_kernels_2.JPG)  
次の手順では、FPGA 上の CU の数を増やして、ホスト カーネルを同時に実行できるようにします。

## CU 数の増加

次は、同じカーネルの CU を 3 つ生成するようにリンク手順を変更して、カーネル xclbin をビルドし直します。

`link.cfg` を開いて `nk` 設定を変更します。

```
nk = Filter2DKernel:3
```

## ハードウェア エミュレーションの実行と変更の確認

1. xclbin ファイルを生成し直します。`make clean` および `make` を実行して 1 つの CU を含む既存の xclbin を削除してから、3 つのカーネル CU を含む新しい xclbin を作成する必要があります。

   ```
   make clean
   make check MODE=hw_emu
   ```

2. Vitis アナライザーで新しい `timeline_trace.csv` を表示します。

   ```
   vitis_analyzer ./timeline_trace.csv
   ```

   アプリケーションが 3 つの CU の利点を生かし、カーネル実行がオーバーラップして並列で実行されるようになって、アプリケーション全体の速度が上がるようになりました。![](./images/overlapping_kernels_vitis_2.JPG)

# まとめ

カーネル リンク プロセスを変更して、FPGA 上の同じカーネル インスタンスを同時に実行する方法を学びました。</br>

<hr/>
<p align= center><b><a href="../../README.md">メイン ページに戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>

この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。
