<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>アクセラレーションした FPGA アプリケーションの最適化: たたみ込みの例</td>
 </tr>
</table>

# 6\. 順不同キューと複数の計算ユニットの使用

前の演習では、パイプラインおよびデータフローなどの手法を使用してカーネル内で並列処理を抽出する方法をお見せしました。FPGA の利点の 1 つは、カーネルのコピーである複数の計算ユニット (CU) を作成して、より多くの処理が並列で実行されるようにできることです。これらの CU を使用すると、複数画像を同時に処理したり、1 つの画像をより小さな領域に分割して、各フレームをより速く処理できるようになります。このチュートリアルでは、1 つの画像をより小さな領域に分割して、各フレームの計算速度を上げます。

複数 CU によるアクセラレーションの可能性を利用するには、ホスト アプリケーションが複数の並行リクエストを CU に送信して管理できるようにする必要があります。パフォーマンスを最大にするには、アプリケーションがすべての CU をビジー状態に維持するようにすることが重要です。データ転送や CU の開始に遅延があると、全体的なパフォーマンスが下がります。

この演習では、まず複数の CU を処理するようにホスト コードを変更してから、フレームの下位領域を処理するようにカーネルをアップデートします。

## キューの演算を順不同に実行

ホスト アプリケーションは OpenCL™ API を使用して FPGA 上のカーネルと通信します。これらのコマンドは、コマンド キュー オブジェクトを介して実行されます。デフォルトでは、コマンド キューは順番通りに処理されますが、特殊なフラグをコマンド キューに渡すと、どの順番でも動作を実行できるようになります。このタイプのキューは、リソースが使用可能になるとすぐに、実行準備のできた演算を実行します。

順不同キューを使用すると、メモリ転送およびカーネル呼び出しなどの複数の動作を同時に開始できます。OpenCL API イベントと待機リストを使用すると、タスクに依存性を追加できます。イベントは、特定タスクに関連付けられたオブジェクトで、通常は最後の引数として呼び出しに渡されます。動作が別のタスクに依存する場合は、そのイベントを待機リストに渡すことができます。動作は、待機リストのすべてのイベントが終了するのを待ってから実行されます。

> **ヒント:** すべてのホスト コードのソース ファイルは、`reference-files/multicu` フォルダーに含まれます。必要に応じて、リファレンスとしてご利用ください。

順不同キューとイベントの利点を生かすには、ホスト プログラムを変更します。

1. `src/multicu` から `convolve.cpp` ファイルを開いて、次の行を見つけます。

        cl::CommandQueue q(context, device, cl::QueueProperties::Profiling);

   これを次のように変更します。

        cl::CommandQueue q(context, device, cl::QueueProperties::Profiling | cl::QueueProperties::OutOfOrder);

   `cl::QueueProperties::OutOfOrder` 列挙型を `CommandQueue` コンストラクターに渡すことで、ランタイムにこのキューの動作を順不同で実行できることを伝えられます。

2. 順不同キューを使用する場合、読み出し、enqueueTask、書き込みの呼び出し間の順番を、コピー動作が終了する前にバッファーを読み出すことがないように変更する必要があります。`cl::Event` オブジェクトを作成して、それを `enqueueWriteBuffer` 関数の最後の引数として渡します。次の 95 行目を見つけます。

        q.enqueueWriteBuffer(buffer_input, CL_FALSE, 0, frame_bytes, inFrame.data());

   これを次のように変更します。

        cl::Event write_event;
        q.enqueueWriteBuffer(buffer_input, CL_FALSE, 0, frame_bytes, inFrame.data(), nullptr, &write_event);

   `write_event` オブジェクトを使用して、次のタスクへの動作の依存性を指定します。

3. `write_event` を `enqueueTask` 呼び出しに渡す必要があります。タスクが読み出し動作に渡されるよう、イベント オブジェクトを作成する必要もあります。前の呼び出しからの `write_event` オブジェクトは、ベクターへのポインターを介してこの呼び出しに渡される必要があります。96 行目の `enqueueTask` 呼び出しを次のように変更します。

        vector<cl::Event> iteration_events{write_event};
        cl::Event task_event;
        q.enqueueTask(convolve_kernel, &iteration_events, &task_event);

4. 読み出し呼び出しは、`convolve_kernel` の実行が終了してから実行する必要があります。前の動作と同様、イベントをこの関数の最後の引数として送信します。97 行目の `enqueueReadBuffer` 呼び出しを次のように変更します。

        iteration_events.push_back(task_event);
        cl::Event read_event;
        q.enqueueReadBuffer(buffer_output, CL_FALSE, 0, frame_bytes, outFrame.data(), &iteration_events, &read_event);
        iteration_events.push_back(read_event);

   ここでは、`iteration_events` ベクターの最後に `task_event` オブジェクトを追加しました。その後、`enqueueReadBuffer` 呼び出しへの最後から 2 つ目の引数として `iteration_events` を渡しました。`enqueueTask` 呼び出しは前の呼び出しに依存するので、新しいベクターを作成することもできました。

5. `ffmpeg` がデータをホストに転送し戻す前に出力ストリームに書き込まないようにする必要があります。`read_event` オブジェクトで待機呼び出しを呼び出すことで、スレッドが続行されないようにできます。`iteration_events` オブジェクトの `push_back` 関数呼び出しの後に次の行を追加します。

        read_event.wait();

## 複数の計算ユニットの使用

前の演習では、カーネルに 1 つの CU しか使用しませんでした。このセクションでは、複数の CU を使用して、各 CU が画像のより小さな領域を処理するようにデザインを変更します。これには、前の手順からの出力を使用して変更を加えていきます。

> **ヒント:** カーネル ソース ファイルは `reference-files/multicu` フォルダーに含まれています。必要に応じて、リファレンスとしてご利用ください。

では、カーネル コードを変更します。`src/multicu` から `convolve_fpga.cpp` ファイルを開いて、次のように変更します。

1. オフセットと各カーネルが処理する行数を受け取れるように、`convolve_fpga` カーネルのシグネチャを変更します。

   ```
           void convolve_fpga(const RGBPixel* inFrame, RGBPixel* outFrame, const float* coefficient, int coefficient_size, int img_width, int img_height, int line_offset, int num_lines) {
               ...
   ```

   画像サイズと CU 数によって、作業を均等に分割します。カーネルの開始位置はオフセットを使用して決定します。`line_offset` パラメーターは CU が処理する最初の行です。`num_lines` 引数は、各 CU で処理される行数を保持します。

   > **ヒント:** `kernels.h` の `convolve_fpga` 関数の宣言が `convolve_fpga.cpp` ファイルと同じになるようにします。

2. メイン カーネルを変更し、処理する各 CU のパディングおよびオフセットを計算できるようにします (123 行目)。

   ```
        int half = COEFFICIENT_SIZE / 2;

        hls::stream<RGBPixel> read_stream("read");
        hls::stream<RGBPixel> write_stream("write");

        int elements = img_width * num_lines;
        int offset = std::max(0, line_offset - half) * img_width;
        int top_padding = 0;
        int bottom_padding = 0;
        int padding = 0;
        if(line_offset == 0) {
            top_padding = half * img_width;
        } else {
            padding = img_width *  half;
        }
        if(line_offset + num_lines < img_height) {
            padding += img_width * half + COEFFICIENT_SIZE;
        }else {
            bottom_padding = img_width * (half) + COEFFICIENT_SIZE;
        }

        #pragma HLS dataflow
        read_dataflow(read_stream, inFrame + offset, img_width, elements + padding, half, top_padding, bottom_padding);
        compute_dataflow(write_stream, read_stream, coefficient, img_width, elements, half);
        write_dataflow(outFrame + line_offset * img_width, write_stream, elements);
   ```

   * `offset` 変数は、画像の最初から CU の読み出す最初のピクセルまでのオフセットを計算するために使用されます。
   * `top_padding` および `bottom_padding` 変数は、画像の一番上と一番下に追加する 0 のパディングを決定します。
   * 一方で、`padding` 変数は、たたみ込みウィンドウ周辺の領域も含めた読み出すピクセル数です。

3. 画像の一番上と一番下のパディング エリアに 0 を送信するように read\_dataflow カーネルを変更します。

   ```
        void read_dataflow(hls::stream<RGBPixel>& read_stream, const RGBPixel * in, int img_width, int elements, int half, int top_padding, int bottom_padding) {
            while(top_padding--) {
                read_stream << zero;
            }
            int pixel = 0;
            while(elements--) {
                read_stream << in[pixel++];
            }
            while(bottom_padding--) {
                read_stream << zero;
            }
        }
   ```

4. パディング ロジックは read\_dataflow モジュール内で処理するので、パディング エリアを 0 で埋める初期化ロジックは削除できます。compute\_dataflow から次の行 (45 行目) を削除します。

   ```
        while(line_idx < center) {
            for(int i = 0; i < img_width; i++) {
                window_mem[line_idx][i] = zero;
            }
                line_idx++;
        }
   ```

複数の CU を並列で開始するには、ホスト コードをさらに変更する必要があります。

## 複数の計算ユニット (CU) をサポートするようにホスト コードをアップデート

CU をサポートするためには、次の手順を実行する必要があります。

1. `convolve.cpp` を開いて、`frame_count` `for` ループの前に次の行を追加します。

   ```
        int compute_units = 4;
        int lines_per_compute_unit = height / compute_units;
   ```

   これらの変数は、バイナリに含まれる CU の数を定義します。この後、画像の行を CU 間で均等に分割します。このコードは、CU 間で画像を均等に分割できると想定したものです。

2. 1 つのタスクを開始するのではなく、作成した各 CU でタスクを開始します。次のコードを見つけます。

   ```
        cl::Event task_event;
        q.enqueueTask(convolve_kernel, &iteration_events, &task_event);
   ```

   これを次のように変更します。

   ```
        vector<cl::Event> task_events;
        for(int cu = 0; cu < compute_units; cu++) {
            cl::Event task_event;
            convolve_kernel.setArg(6, cu * lines_per_compute_unit);
            convolve_kernel.setArg(7, lines_per_compute_unit);
            q.enqueueTask(convolve_kernel, &iteration_events, &task_event);
            task_events.push_back(task_event);
        }
        copy(begin(task_events), end(task_events), std::back_inserter(iteration_events));
   ```

   この `for` ループは CU ごとに 1 つのタスクを開始します。各タスクにイベント オブジェクトを渡してから、それを `task_events` ベクターに追加します。ループの終わりまでは `iteration_events` に追加しないようになっています。これは、タスクを `enqueueWriteBuffer` 呼び出しにのみに依存させ、互いには依存させないようにするためです。

これでデザインをコンパイルして実行でき、次のような結果が表示されます。

## ハードウェア エミュレーションの実行 (複数の計算ユニットを使用した場合)

1. エミュレーションの実行前に、CU 数を 4 に設定する必要があります。これには、`design.cfg` を開いて `nk` オプションを次のように変更します。

   ```
   nk=convolve_fpga:4
   ```

   ビルド プロセスのリンク段階で生成されたカーネル インスタンス数または CU 数を指定するのに `nk` オプションが使用されています。この演習では、4 に設定します。

2. `makefile` ディレクトリに移動します。

3. 次のコマンドを使用してハードウェア エミュレーションを実行します。

   ```
   make run TARGET=hw_emu STEP=multicu SOLUTION=1 NUM_FRAMES=1
   ```

4 つの CU で実行されるこのカーネルの結果は、次のようになります。

```
Processed 0.08 MB in 42.810s (0.00 MBps)

INFO: [Vitis-EM 22] [Wall clock time: 01:34, Emulation time: 0.102462 ms] Data transfer between kernel(s) and global memory(s)
convolve_fpga_1:m_axi_gmem1-DDR[0]          RD = 24.012 KB              WR = 0.000 KB
convolve_fpga_1:m_axi_gmem2-DDR[0]          RD = 0.000 KB               WR = 20.000 KB
convolve_fpga_1:m_axi_gmem3-DDR[0]          RD = 0.035 KB               WR = 0.000 KB
convolve_fpga_2:m_axi_gmem1-DDR[0]          RD = 22.012 KB              WR = 0.000 KB
convolve_fpga_2:m_axi_gmem2-DDR[0]          RD = 0.000 KB               WR = 20.000 KB
convolve_fpga_2:m_axi_gmem3-DDR[0]          RD = 0.035 KB               WR = 0.000 KB
convolve_fpga_3:m_axi_gmem1-DDR[0]          RD = 24.012 KB              WR = 0.000 KB
convolve_fpga_3:m_axi_gmem2-DDR[0]          RD = 0.000 KB               WR = 20.000 KB
convolve_fpga_3:m_axi_gmem3-DDR[0]          RD = 0.035 KB               WR = 0.000 KB
convolve_fpga_4:m_axi_gmem1-DDR[0]          RD = 22.000 KB              WR = 0.000 KB
convolve_fpga_4:m_axi_gmem2-DDR[0]          RD = 0.000 KB               WR = 20.000 KB
convolve_fpga_4:m_axi_gmem3-DDR[0]          RD = 0.035 KB               WR = 0.000 KB
```

これで、ほぼ同じ時間量で 4 倍の仕事を実行できるようになりました。これで、グローバル メモリからさらにデータを転送できるようになりました。これは各 CU が周囲のパディング行を読み出す必要があるためです。

## ハードウェア エミュレーションのプロファイル サマリ レポートの表示

1. 次のコマンドを使用して、プロファイル サマリ レポートを表示します。

```
make view_run_summary TARGET=hw_emu STEP=multicu
```

4 つの CU のカーネル実行時間は約 0.065 ms です。

アップデートされた表は、次のようになります。

| 演習名| Image Size| Time (HW-EM)(ms)| Reads (KB)| Writes (KB)| Avg. Read (KB)| Avg. Write (KB)| BW (MBps)
|:----------|:----------|----------:|----------:|----------:|----------:|----------:|----------:
| baseline| 512x10| 3.903| 344| 20.0| 0.004| 0.004| 5.2
| localbuf| 512x10| 1.574 (2.48x)| 21 (0.12x)| 20.0| 0.064| 0.064| 13
| fixed-type data| 512x10| 0.46 (3.4x)| 21| 20.0| 0.064| 0.064| 44
| dataflow| 512x10| 0.059 (7.8x)| 21| 20.0| 0.064| 0.064| 347
| multi-CU| 512x40\*| 0.06 (0.98x)| 92 (4.3x)| 80.0 (4x)| 0.064| 0.064| 1365\*

> **注記:**
>
> * この複数 CU のバージョンは前のバージョンに比べると 4 倍のデータを処理します。各 CU の実行時間は変わりませんが、4 つの並列 CU によりシステム パフォーマンスがほぼ 4 倍増加します。
> * これは、4 x データ/時間で計算されます。この場合、データ転送時間は考慮されておらず、4 つの CU が並列で実行されているとしています。これは、ハードウェア実行ほど正確ではありませんが、最適化の効率性を計るためのリファレンスとして使用できます。

## 次のステップ

この段階では、順不同コマンド キューを使用して複数の CU を実行することで、ホスト コード最適化を実行しました。次は、[アクセラレータをハードウェアで実行](./RunOnHardware.md)するアプリケーションを作成します。

</br>

[hostopt_hwemu_profilesummary]: ./images/191_hostopt_hwemu_pfsummary_40_2.jpg "ホスト コード最適化バージョンのハードウェア エミュレーションのプロファイル サマリ"
<hr/>
<p align="center"><b><a href="../../docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="./README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
