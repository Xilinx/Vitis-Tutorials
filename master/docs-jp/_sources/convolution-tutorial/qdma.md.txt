<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 7\. 複数の計算ユニットを使用した QDMA ストリーミングの使用

このセクションでは、前の演習の結果に基づいて、ストリーミング機能に複数の CU を使用するようデザインを変更します。

> **ヒント:** 完成したカーネルのソース ファイルは `reference-files/qdma` フォルダーにあります。

> **重要:** コードを変更する前に、ホストとカーネル コードの関数/構造体定義を分離するため、一部のヘッダー ファイル (`types.h`、`kernel.h`、`xcl2.hpp`、`xcl2.cpp`、および新規 `kernel_types.h` ファイル) が前の手順から追加/変更されています。

この手順では、ホスト コードとカーネル コードの両方を変更する必要があります。

`src/qdma` ディレクトリにある `convolve.cpp` ホスト コードファイルを開き、次のように変更します。

前の手順で、画像を分割し、各 CU が個別の部分を並列に処理するカーネル コードを開発しました。この手順では、各 CU をストリーム インターフェイスに関連付けるので、ホスト上の画像を分割し、データをそれに応じてデバイスに送信できるようにします。

## デバイスを初期化

この手順では、ストリーム クラスを設定および作成します。このセクションの手順の実行方法は、ホスト コードの手順 1 を参照してください。これには、通常次の手順を実行します。

- カーネル ストリーミング クラスおよびストリーミング API を宣言します。
- 各 CU のカーネル オブジェクトを作成します。
- ストリーミング クラスを初期化します。
- CU のストリームを作成します。

1. `mem_ext_ptr` のクラスとザイリンクス ストリーミング API にバインドするカスタム ストリームを宣言します。

   ```
   #include "CL/cl_ext_xilinx.h"

   decltype(&clCreateStream) xcl::Stream::createStream = nullptr;
   decltype(&clReleaseStream) xcl::Stream::releaseStream = nullptr;
   decltype(&clReadStream) xcl::Stream::readStream = nullptr;
   decltype(&clWriteStream) xcl::Stream::writeStream = nullptr;
   decltype(&clPollStreams) xcl::Stream::pollStreams = nullptr;
   ```

2. 各ストリーミング インターフェイスを各 CU の固有のカーネル `cl_kernel` オブジェクトに関連付けます。

   ```
   std::string krnl_name = args.kernel_name;
   std::vector<cl::Kernel> convolve_kernel(NCU);

   for (int i = 0; i < NCU; i++)
   {
               cu_id = std::to_string(i + 1);
           auto krnl_name_full = krnl_name +":{" + "convolve_fpga_" + cu_id +"}" ;
               printf("Creating a kernel [%s] for CU(%d)\n",
                       krnl_name_full.c_str(),
                       i);
               convolve_kernel[i] = cl::Kernel(
                               program, krnl_name_full.c_str(), &err);
   }
   ```

3. ホスト上でストリーミング インターフェイスを使用する前に、ストリーミング クラスをプラットフォームで初期化する必要があります。デバイス情報は `device.getInfo` から取得できます。

   ```
   auto platform_id = device.getInfo<CL_DEVICE_PLATFORM>(&err);

   //Initialization of streaming class is needed before using it.
   xcl::Stream::init(platform_id);
   ```

4. カーネル インターフェイスを見ると、AXI にマップされている 3 つの関数引数をストリームに変換できます。各 CU に 3 つのストリームを作成します。

   各ストリームはコマンド キューを使用しないので、直接 OpenCL デバイス オブジェクトに接続する必要があります。ストリーム自体はデータを特定の方向 (ホストからカーネルまたはカーネルからホスト) に渡すだけのコマンド キューです。

   - 最初の手順では、5 つの引数を入力する `clcreatestream` を使用してストリームを作成します。

     - **`device_id`**: ストリームを作成するデバイス ハンドル。
     - **`flags`**: カーネル プログラムの視点から、適切なフラグを使用してストリームを XCL_STREAM_READ_ONLY または XCL_STREAM_WRITE_ONLY と指定する必要があります。
     - **`attributes`**: 要求されたストリームの属性。
     - **`ext`**: ストリームをデバイスにどのように接続するかを指定するには、拡張ポインター オブジェクト (cl_mem_ext_ptr_t) を使用して、カーネルとストリームを関連付けるカーネル引数を指定します。`ext.flag` は、対応するカーネル引数と通信するために使用します。
       - **`ext.param`**: カーネル名を指定します。
       - **`ext.flags`**: ストリームを関連付けるカーネル引数を指定します。
       - **`ext.obj`**: ヌルに設定します。
     - **`errcode_ret`**: 戻り値 (CL\_SUCCESS など)。

     ```
     // Streams
     std::vector<cl_stream> write_stream_a(NCU);
     std::vector<cl_stream> write_stream_b(NCU);
     std::vector<cl_stream> read_stream(NCU);
     cl_int ret;

     for (int i = 0; i < NCU; i++) {
             // Device Connection specification of the stream through extension pointer
             cl_mem_ext_ptr_t ext;
             ext.param = convolve_kernel[i].get();
             ext.obj = NULL;
             // The .flag should be used to denote the kernel argument
             // Create write stream for argument 0 and 1 of kernel
             ext.flags = 0;
                 write_stream_a[i] = xcl::Stream::createStream(
                     device.get(), XCL_STREAM_READ_ONLY, CL_STREAM, &ext, &ret);
             ext.flags = 2;
                 write_stream_b[i] = xcl::Stream::createStream(
                     device.get(), XCL_STREAM_READ_ONLY, CL_STREAM, &ext, &ret);

             //Create read stream for argument 2 of kernel
             ext.flags = 1;
                 read_stream[i] = xcl::Stream::createStream(
                     device.get(), XCL_STREAM_WRITE_ONLY, CL_STREAM, &ext, &ret);
         }
     ```

## カーネルの実行

ハードウェアのストリーム クラスを設定したので、この手順ではデータをデバイス上のカーネルに送信します。このセクションの手順の実行方法は、ホスト コードの手順 2 を参照してください。

- ストリーミングでないカーネル引数を設定します。
- カーネルを実行します。
- オフセットを設定して画像を各 CU に分割します。
- CU ごとに読み出しおよび書き込みストリーム転送を開始します。
- ストリームをプールします。

1. 残りのストリーミングでないインターフェイス引数を設定し、次の標準 API `setarg` および `enqueueTask` を使用してカーネルをエンキューします。

   ```
   int lines_per_compute_unit = args.height / compute_units;

   for (int i = 0; i < NCU; i++) {
           convolve_kernel[i].setArg(3, coefficient_size);
           convolve_kernel[i].setArg(4, args.width);
           convolve_kernel[i].setArg(5, args.height);
           convolve_kernel[i].setArg(6, i * lines_per_compute_unit);
           convolve_kernel[i].setArg(7, lines_per_compute_unit);
           q.enqueueTask(convolve_kernel[i]);

   }
       int img_width = args.width;
       int img_height = args.height;
   ```

2. 読み出しおよび書き込み転送を開始する前に、画像にアクセスするため、画像を正しいオフセットを使用して分割する必要があります。

   * `line_offset` および `offset` 変数は、画像の最初から CU が読み出す最初のピクセルまでのオフセットを計算するために使用されます。

   * `padding` 変数は、たたみ込みウィンドウ周辺の領域を含む読み出すピクセル数です。

   * `num_lines` 変数は CU ごとのライン数です。

   * `elements` は、CU ごとに計算されるバイトの総数です。

     ```
     for(int cu = 0; cu < compute_units; cu++) {
         int line_offset = cu*lines_per_compute_unit;
         int num_lines = lines_per_compute_unit;
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
     ```

3. `clReadStream` および `clWriteStream` コマンドを使用して、CU ごとの読み出しおよび書き込みストリーム転送を開始します。読み出しおよび書き込みストリームには、5 つの引数を指定します。

   - **`stream`**: デバイスに読み出す/書き込む必要のあるストリームを指定します。
   - **`ptr`**: 入力/出力引数の場所。
   - **`size`**: 書き込むバイト数。
   - **`req_type`**: 読み出しおよび書き込み要求に関連する CL\_STREAM\_XFER\_REQ 属性の使用方法。req\_type には、次の引数を指定する必要があります。
     - **`flag`**: 転送メカニズムを示します。
       - **`CL_STREAM_NONBLOCKING`**: デフォルトでは、読み出し転送および書き込み転送はブロッキングです。ノンブロッキング転送の場合は、CL\_STREAM\_NONBLOCKING を設定する必要があります。
       - **`.priv_data`**: タグ付けのために転送に関連付ける文字列を指定します。これにより、ストリーミングの終了をポーリングする際に特定の転送の完了を検出できます。ノンブロッキング バージョンの API を使用する場合にのみ必要です。
   - **`errcode_ret`**: 戻り値 (CL\_SUCCESS など)。

```
int vector_size_bytes = sizeof(RGBPixel)* (elements + padding);
       int coeff_size = sizeof(float)* COEFFICIENT_SIZE*COEFFICIENT_SIZE ;
        cl_stream_xfer_req rd_req{0};
        cl_stream_xfer_req wr_req{0};

        rd_req.flags = CL_STREAM_EOT |CL_STREAM_NONBLOCKING;
        wr_req.flags = CL_STREAM_EOT |CL_STREAM_NONBLOCKING;

        auto write_tag_a = "write_a_" + std::to_string(cu);
        wr_req.priv_data = (void *)write_tag_a.c_str();
        RGBPixel *p = inFrame.data();
        p = p+ offset;
        std::cout << "\n Writing Stream write_stream_a[" << cu << "]";
                  xcl::Stream::writeStream(write_stream_a[cu],
                                           (p),
                                           vector_size_bytes,
                                           &wr_req,
                                           &ret);
        auto write_tag_b = "write_b_" + std::to_string(cu);
        wr_req.priv_data = (void *)write_tag_b.c_str();
 std::cout << "\n Writing Stream write_stream_b[" << cu << "]";
                   xcl::Stream::writeStream(write_stream_b[cu],
                                            (filter_coeff.data()),
                                           coeff_size,
                                           &wr_req,
                                           &ret);
       auto read_tag = "read_" + std::to_string(cu);
        rd_req.priv_data = (void *)read_tag.c_str();
        RGBPixel *out_base = outFrame.data();
       out_base = out_base + line_offset*img_width;
       std::cout << "\n Reading Stream read_stream[" << cu << "]";
                        xcl::Stream::readStream(read_stream[cu],
                                          (outFrame.data()+line_offset*img_width),
                                          (elements)*sizeof(int),
                                          &rd_req,
                                          &ret);
```

4. すべてのストリームの終了をポーリングします。

   * ノンブロッキング転送には、読み出し/書き込み転送の完了を確認するためのポーリング API が提供されています。
   * ブロッキング バージョンの API には、ポーリングは必要ありません。ポーリング結果は cl\_streams\_poll\_req\_completions 配列に格納され、ストリーミング イベント結果を検証および確認するのに使用できます。

   `clPollStreams` はブロッキング API です。すべてのストリーム要求が完了したことを示す通知を受け取るか、指定したタイムアウトになると、実行をホスト コードに戻します。

   ```
   int num_compl = 3 * NCU;

       // Checking the request completions
       cl_streams_poll_req_completions *poll_req;
       poll_req = (cl_streams_poll_req_completions *)malloc(
           sizeof(cl_streams_poll_req_completions) * num_compl);
       memset(poll_req, 0, sizeof(cl_streams_poll_req_completions) * num_compl);
       print(
           "\n clPollStreams for (%d) events (CU: %d, axis_in: 2, axis_out: 1)\n",
           num_compl,
           NCU);
   xcl::Stream::pollStreams(device.get(),
                                       poll_req,
                                       num_compl,
                                       num_compl,
                                       &num_compl,
                                       50000,
                                       &ret);
   ```

## ストリームの解放

ポーリング要求が正しく完了したら、`releaseStream` を使用してストリームを解放する必要があります。

```
for (int i = 0; i < NCU; i++) {
    xcl::Stream::releaseStream(write_stream_a[i]);
    xcl::Stream::releaseStream(write_stream_b[i]);
    xcl::Stream::releaseStream(read_stream[i]);
  }
```

これで、ホスト コードに必要な変更は完了です。

## カーネル コードの変更

次に、カーネル コードを変更します。カーネル コードの変更は、次の 4 つの段階に分けることができます。

- ホストからカーネルへのストリーミングに使用されるクラス。
- QDMA のデータ型の制限。
- ポインターを hls::stream に変更。
- データ ムーバーの変更。

### ホストからカーネルへのストリーミング

Vitis HLS には、ストリーミング データ構造を記述するための C++ テンプレート クラス hls::stream\<> が含まれます。hls::stream\<> クラスを使用してインプリメントされるストリームには、次の属性があります。

- `qdma_axis<D,0,0,0>` 型を使用する必要があります。`qdma_axis` データは、`ap_axi_sdata.h` ヘッダー ファイルに含まれます。

- `qdma_axis` 型には、カーネル コード内で使用する必要のある 3 つの変数が含まれます。

  * **`data`**: qdma\_axis 型には ap\_int <D> が含まれますが、これには .get\_data() および .set\_data() メソッドでアクセスする必要があります。D は 8、16、32、64、128、256、または 512 ビット幅である必要があります。
  * **`last`**: 入力ストリームおよび出力ストリームの最後の値を示します。入力ストリームから読み出される際、`last` でストリームの末尾が検出されます。同様に、カーネルがホストに転送する出力ストリームを書き込む際にも、`last` を設定してストリームの末尾を示す必要があります。

  - **`get_last/set_last`**: ストリームの末尾を示すのに使用される last 変数を取得/設定します。
  - **`keep`**: 特殊な状況で、last データを切り捨ててバイト数を減らすために使用できます。keep はストリームの last データ以外には使用できません。そのため、ほとんどの場合はカーネルからのすべての出力データに対して keep を 1 に設定する必要があります。
  - **`get_keep/set_keep`**: `keep` 変数を取得/設定します。last データの前のデータすべてに対して `keep` を 1 に設定し、データのすべてのバイトが有効であることを示す必要があります。last データでは、カーネルで少ないバイトを送信できます。たとえば 4 バイトを転送する際、次の set\_keep() 関数を使用すると、last データで 1 バイト、2 バイト、または 3 バイトを送信できます。
    - last データが 1 バイト ≥ `.set_keep(1)`
    - last データが 2 バイト ≥ `.set_keep(3)`
    - last データが 3 バイト ≥ `.set_keep(7)`
    - last データが 4 バイト (last 以外のすべてのデータと同様) ≥ `set_keep(-1)`

### QDMA のデータ型の制限

現在のところ、QDMA クラスでは `ap_uint` のみがサポートされ、オーバーロードすることはできません。このデザインでは、`floats` および `RGBPixel` をデータ型として使用します。struct 関数を使用して `ap_int` 型を `RGB` 型に分離および連結します。

```
#ifdef KERNEL_HEADERS
#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#define DWIDTH 32

typedef qdma_axis<DWIDTH, 0, 0, 0> pkt;
#endif


struct RGBPixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
//    unsigned char a;
#ifdef KERNEL_HEADERS

void init(ap_int<32> d )
{
#pragma HLS INLINE
    r = d.range(7,0);
    g = d.range(15,8);
    b = d.range(23,16);

}
ap_int<32> func1()
{
#pragma HLS INLINE
  ap_int<32> c = (ap_int<8>(b) , ap_int<8>(g), ap_int<8>(r));
    return c;
}

#endif

}__attribute__((aligned(4)));
```

`coeff` 引数にも同じ制限が適用されるので、union を使用して `int` を `float` に変換します。これを `convolve_fpga.cpp` に配置します。

```
union test{
int x;
float y;
};
```

### カーネル インターフェイスの変更

次のカーネル インターフェイスをメモリ マップドから `hls::stream` に変更します。

```
void convolve_fpga( hls::stream<pkt>& inFrame, hls::stream<pkt>& outFrame,
		hls::stream<pkt>& coefficient,
		int coefficient_size, int img_width, int img_heightint line_offset, int num_lines)
```

### データ ムーバーの変更

前の手順では、カーネル コードは読み出し、計算、書き込み関数の 3 つの段階に分割しました。読み出し/書き込み関数は DDR に対するデータ ムーバーです。この手順では、配列ポインターを使用するこれらのデータ ムーバーを `hls::stream` オブジェクトに変更します。前述のとおり、この `hls::stream` は `ap_axiu` オブジェクトを使用します。この関数は、前述のメソッドを使用し、last パケットが受信されるまで `do-while` ループを使用してデータを読み出します。

```
 void read_dataflow(hls::stream<RGBPixel>& read_stream, hls::stream<pkt>& in,
                   int img_width, int elements, int half,
                   int top_padding, int bottom_padding) {
     while(top_padding--) {
         read_stream << zero;
     }
    RGBPixel a_temp;
     int pixel = 0;
     ap_int<1> last;
     bool eos = false;
     do{

    pkt a_t = in.read();
    ap_int<32> in1 = a_t.get_data();
    last = a_t.get_last();
    if(last)
    {
        eos = true;
    }
      a_temp.init(in1);
       RGBPixel a;
       a = a_temp;
         read_stream << a_temp;

     }while(eos ==false);

        while(bottom_padding--) {
         read_stream << zero;
     }
 }


 void write_dataflow(hls::stream<pkt>& outFrame, hls::stream<RGBPixel>& write_stream,
                     int elements)
 {
     int pixel = 0;
     pkt t_out;
     ap_int<32> a_out;
     RGBPixel tmpout;
     int i=0;
     while(elements--) {

        write_stream >> tmpout;
        a_out = tmpout.func1();
       t_out.set_data(a_out);
        i++;
        if(elements ==0)
        {
            t_out.set_last(1);
        }
        else
        {
            t_out.set_last(0);
           t_out.set_keep(-1);
             outFrame.write(t_out);
        }
     }
  }
```

## 複数の計算ユニットを使用するストリーミング インターフェイスのハードウェア エミュレーションの実行

1. エミュレーションの実行前に、CU 数を 4 に設定する必要があります。これには、`design.cfg` を開き、`nk` オプションを次のように変更します。

   ```
   nk=convolve_fpga:4
   ```

   `nk` オプションは、ビルド プロセスのリンク段階で生成するカーネル インスタンス (CU) の数を指定するために使用します。

2. `makefile` ディレクトリに移動します。

3. 次のコマンドを使用してハードウェア エミュレーションを実行します。

   ```
   make run TARGET=hw_emu STEP=qdma SOLUTION=1 NUM_FRAMES=1
   ```

   このカーネルを 4 つの CU で実行した場合の結果は、次のようになります。

   ```
   Data transfer on stream interfaces
   HOST-->convolve_fpga_1:coefficient           0.035 KB        
   HOST-->convolve_fpga_3:inFrame               24.012 KB       
   convolve_fpga_3:outFrame-->HOST              20.000 KB       
   HOST-->convolve_fpga_4:coefficient           0.035 KB        
   HOST-->convolve_fpga_4:inFrame               22.000 KB       
   convolve_fpga_4:outFrame-->HOST              20.000 KB       
   HOST-->convolve_fpga_1:inFrame               22.012 KB       
   convolve_fpga_1:outFrame-->HOST              20.000 KB       
   HOST-->convolve_fpga_2:coefficient           0.035 KB        
   HOST-->convolve_fpga_2:inFrame               24.012 KB       
   convolve_fpga_2:outFrame-->HOST              20.000 KB       
   HOST-->convolve_fpga_3:coefficient            0.035 KB        
   ```

   これで、ほぼ同じ時間で 4 倍の処理を実行できるようになりました。各 CU がパディングされた周辺のラインも読み出す必要があるので、グローバル メモリから転送されるデータ量は増加します。

## ハードウェア エミュレーションのプロファイル サマリ レポートの表示

次のコマンドを使用して、プロファイル サマリ レポートを表示します。

```
make view_run_summary TARGET=hw_emu STEP=qdma
```

4 つの CU のカーネル実行時間は、それぞれ約 0.135061 ms です。

アップデートされた表は、次のようになります。

| 手順| 画像サイズ| 時間 (HW-EM) (ms)| 読み出し (KB)| 書き込み (KB)| 読み出し平均 (KB)| 書き込み平均 (KB)| 帯域幅 (Mbps)
|:----------|:----------|----------:|----------:|----------:|----------:|----------:|----------:
| baseline| 512x10| 3.903| 344| 20.0| 0.004| 0.004| 5.2
| localbuf| 512x10| 1.574 (2.48x)| 21 (0.12x)| 20.0| 0.064| 0.064| 13
| fixedpoint| 512x10| 0.46 (3.4x)| 21| 20.0| 0.064| 0.064| 44
| dataflow| 512x10| 0.059 (7.8x)| 21| 20.0| 0.064| 0.064| 347
| multi-CU| 512x40\*| 0.358| 92| 80.0 (4x)| 0.064| 0.064| 1365\*
| Stream-multi-CU| 512x40\*| 0.130561 (約 3 倍)| 96.188 (4.3 倍)| 80.0| 22.540| 0.036| 1200

> **注記:**
>
> * Stream-multi-CU バージョンは、前のバージョンの 4 倍のデータを処理します。各 CU の実行時間は変わらなくても、4 つの CU を並列処理することによりシステム パフォーマンスがほぼ 4 倍になります。
> * これは、4 x データ/時間で計算されます。ここでは、データ転送時間は考慮されておらず、4 つの CU が並列で実行されると想定しています。これはハードウェア実行ほど正確ではありませんが、最適化の効果を評価するために使用します。

## 次の手順

この手順では、ホスト コードおよびカーネル コードを変更して複数ストリームの CU を生成しました。次の手順では、[アクセラレータをハードウェアで実行](./RunOnHardware.md)します。

</br>

[hostopt_hwemu_profilesummary]: ./images/191_hostopt_hwemu_pfsummary_40_2.jpg "ホスト コード最適化バージョンのハードウェア エミュレーションのプロファイル サマリ"
<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="/docs/convolution-tutorial/README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
