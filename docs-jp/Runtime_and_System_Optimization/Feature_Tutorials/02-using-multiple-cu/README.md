<p align="right"><a href="../../../../README.md">English</a> | <a>日本語</a>
<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.1">2020.1 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 
</table>

# 複数の計算ユニットの使用

## 概要

このチュートリアルでは、FPGA 上のカーネル インスタンスの数を増やすための柔軟なカーネル リンク プロセスを示します。カーネルの各インスタンスは、計算ユニット (CU) とも呼ばれます。CU 数を増加するこのプロセスを使用すると、統合されたホスト/カーネル システムの並列処理が向上します。

## チュートリアルの概要

Vitis™ コア開発キットでは、デフォルトでカーネルごとに CU が 1 つずつ作成されます。ホスト プログラムは、異なるデータ セットに対して同じカーネルを複数回使用できます。この場合、カーネルに対して複数の CU を生成して、これらの CU を同時実行すると、システム全体のパフォーマンスを向上できます。

詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[複数のカーネル インスタンスの作成](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.2;t=vitis+doc;d=buildingdevicebinary.html;a=yzb1524519238289)を参照してください。

このチュートリアルでは、次を実行します。

1. ハードウェア エミュレーションを実行し、エミュレーション レポートを参照して、カーネルが順に複数回実行されることを確認します。
2. コマンドを順不同に実行できるようにホスト コードを変更します。
3. カーネル リンク プロセスを変更し、同じカーネルの CU を複数作成します。
4. ハードウェア エミュレーションを再実行し、CU が同時実行されることを確認します。

このチュートリアルでは、画像フィルター例を使用して複数 CU の利点を示します。ホスト アプリケーションは、画像を処理して Y、U、および V プレーンを抽出し、カーネルを 3 回実行して画像の各プレーンをフィルター処理します。デフォルトでは、FPGA にはカーネルの CU が 1 つしか含まれないので、これら 3 つのカーネルは同じハードウェア リソースを使用して順次実行されます。このチュートリアルでは、CU の数を増加し、このカーネルを並列実行する方法を示します。

## 開始前の確認事項

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド
* 2020.2 Vitis コア開発キット リリースおよび xilinx\_u200\_xdma\_201830\_2 プラットフォーム。必要であれば、ほかのバージョンおよびプラットフォームを使用するように変更することもできます。

> **重要:**
>
> * 例を実行する前に、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[インストール](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/acceleration_installation.html#vhc1571429852245)の手順に従って、Vitis コア開発キットをインストールしておく必要があります。
> * ザイリンクス Alveo™ データセンター アクセラレータ カードでアプリケーションを実行する場合は、[Alveo ポートフォリオ](https://japan.xilinx.com/products/boards-and-kits/alveo.html) ページの手順に従って、カードとソフトウェア ドライバーを正しくインストールしてください。
> * このチュートリアル モジュールには、gcc-6.2.0 でコンパイルされたコンパイル済みの OpenCV™ ライブラリが含まれており、gcc/g++ バージョン 5.5 以降が必要です。バージョンが古い場合、ホスト コードのコンパイル中にエラーが返されます。また、gcc-6 に関連する必要なランタイム ライブラリが選択されるようにするため、次のコードを使用して LD\_LIBRARY\_PATH を設定する必要があります。
>   ```
>   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$XILINX_VITIS/lib/lnx64.o/Default
>   ```

### チュートリアル リファレンス ファイルの入手

1. リファレンス ファイルを入手するには、ターミナルに `git clone http://github.com/Xilinx/Vitis-Tutorials` と入力します。
2. `Runtime_and_System_Optimization/Feature_Tutorials/02-using-multiple-cu` ディレクトリに移動し、`reference-files` ディレクトリにアクセスします。

## makefile フロー

このチュートリアルで使用する makefile は、`reference-files/Makefile` に含まれます。最上位設定には、次が含まれます。

* **VPP**: カーネル コードをコンパイルする Vitis コンパイラ パス。

* **EMCONFIGUTIL**: エミュレーション コンフィギュレーション ファイル `emconfig.json` を作成するユーティリティのパス。

* **DSA**: ターゲット プラットフォーム。

* **LFLAGS**: ホスト コード リンカー オプション用に OpenCV ライブラリを使用するリンカー オプション。

  ```
  -Wl,-rpath,./opencv/opencv_gcc -L./opencv/opencv_gcc -lopencv_core -lopencv_highgui
  ```

* **EXE\_OPT**: コマンド ライン引数として渡されるランタイム オプション: コンパイル済みカーネル `xclbin` ファイル、入力画像。

### ハードウェア エミュレーションの実行

次のコマンドでハードウェア エミュレーションを実行します。

```
make check MODE=hw_emu
```

ハードウェア エミュレーション (`hw_emu`) では、カーネル コードはハードウェア モデルにコンパイルされ、ハードウェア シミュレータで実行されますが、システムの残りの部分には C シミュレータが使用されます。ビルドおよび実行にかかる時間は長くなりますが、詳細でサイクル精度のカーネル アクティビティが表示されます。このターゲットは、FPGA で実行されるロジックの機能をテストして、初期パフォーマンス見積もりを取得する場合に便利です。

> **注記:** ホスト ソフトウェアおよびハードウェアのビルド方法は、[Vitis 入門](https://github.com/Xilinx/Vitis-Tutorials/tree/master/Getting_Started)チュートリアルを参照してください。

### ホスト コードの確認

1. エミュレーション run を実行中、別のターミナルで `src/host/host.cpp` ファイルを開きます。

2. 255 ～ 257 行目を確認します。Y、U、および V チャネルを処理するためにフィルター関数が 3 回呼び出されています。

   ```
   request[xx*3+0] = Filter(coeff.data(), y_src.data(), width, height, stride, y_dst.data());
   request[xx*3+1] = Filter(coeff.data(), u_src.data(), width, height, stride, u_dst.data());
   request[xx*3+2] = Filter(coeff.data(), v_src.data(), width, height, stride, v_dst.data());
   ```

   この関数は、80 行目から記述されています。次の抜粋部分で、カーネル引数が設定され、カーネルが `clEnqueueTask` コマンドにより実行されます。

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

### エミュレーション結果

生成されたタイムライン トレース レポート (`timeline_trace.csv`) を確認します。

```
vitis_analyzer filter2d.hw_emu.xclbin.run_summary
```

> **注記:** run ディレクトリには、`xrt.ini` というファイルが含まれます。このファイルには、プロファイル サマリ レポートおよびタイムライン トレースなどの追加レポートを生成するランタイム オプションが含まれています。

Vitis アナライザーが開いたら、左パネルの **\[Application Timeline]** をクリックします。アプリケーション タイムライン レポートは、ホストとデバイスのイベント情報を収集し、共通のタイムラインに表示します。これは、システムの全体的な状態とパフォーマンスを視覚的に表示して理解するのに役立ちます。

* タイムラインの最下部に、ホストからキューに追加された各カーネルに 1 本ずつ、合計 3 本の青いバーがあります。1 つの順序どおりのコマンド キューが使用されているので、ホストはカーネル実行を順にキューに追加します。
* 青いバーの下に、各カーネル実行に 1 本ずつ、合計 3 本の緑色のバーがあります。これらは、FPGA で順に実行されます。 ![順序通りのカーネル エンキュー](./images/serial_kernel_enqueue_vitis.JPG)

### カーネルを同時にエンキューするためのホスト コードの改善

1. `src/host/host.cpp` ホスト ファイルの 75 行目を変更します。この行を変更してコマンド キューを順不同コマンド キューとして宣言します。

   コード変更前:

   ```
   mQueue   = clCreateCommandQueue(Context, Device, CL_QUEUE_PROFILING_ENABLE, &mErr);
   ```

   コード変更後:

   ```
   mQueue   = clCreateCommandQueue(Context, Device, CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &mErr);
   ```

2. (オプション) 変更したホスト コードでハードウェア エミュレーションを実行します。

   ハードウェア エミュレーションを実行する場合、タイムライン トレースを使用して、順不同コマンド キューを使用することによりカーネルがほぼ同時に実行されるように要求できまることを確認します (青のバーはホストでスケジュールされるカーネル エンキュー要求)。

   ホストはこれらの実行を同時にスケジューリングできますが、FPGA 上には 1 つしか CU がなく、カーネルは順次実行されるので、2 つ目および 3 つ目の実行要求は遅れます。  
![シーケンシャル カーネル](./images/sequential_kernels_2.JPG)  
次の手順では、FPGA 上の CU の数を増やして、ホスト カーネルを同時に実行できるようにします。

### CU 数の増加

次に、同じカーネルの CU を 3 つ生成するようにリンク手順を変更して、カーネル xclbin をビルドし直します。

`link.cfg` を開いて `nk` 設定を変更します。

```
nk = Filter2DKernel:3
```

### ハードウェア エミュレーションの実行と変更の確認

1. `xclbin` ファイルを生成し直します。`make clean` を実行して 1 つの CU を含む既存の xclbin を削除してから、`make` を実行して 3 つのカーネル CU を含む新しい xclbin を作成する必要があります。

   ```
   make clean
   make check MODE=hw_emu
   ```

2. Vitis アナライザーで新しい `timeline_trace.csv` を表示します。

アプリケーションで 3 つの CU が使用され、カーネル実行が並列実行されるようになったので、アプリケーションの実行速度が上がりました。![](./images/overlapping_kernels_vitis_2.JPG)

## まとめ

カーネル リンク プロセスを変更して、FPGA 上の同じカーネル関数を同時に実行する方法を学びました。</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">メイン ページに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 1 月 22 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
