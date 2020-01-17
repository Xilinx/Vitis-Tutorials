<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>はじめてのプログラム</h1>
 </td>
 </tr>
</table>

# 2\. はじめてのホスト プログラムの記述

カーネルを記述したら、次はホスト アプリケーションについて学びます。ホスト アプリケーションは、OpenCL API 呼び出しを使用して C または C++ のいずれかで記述され、FPGA アクセラレータとの交信に使用されます。

C++ バージョンと C バージョンの両方を示すサンプル ホスト コードは `./reference-files/src` フォルダーに含まれています。このチュートリアルでは、C ++ バージョンのみを確認します。

- C++ バージョンのホスト コードは、[host.cpp](./reference-files/src/host.cpp) ファイルに含まれています。
- C バージョンのホスト コードは、[host.c](./reference-files/src/host.c) ファイルに含まれています。

通常、ホスト アプリケーションの構造は次の 3 つのセクションに分けることができます。

1. ハードウェアの設定。
2. カーネルの実行。
3. カーネル リターン後のハードウェア リソースのリリース。

このチュートリアルでは、それぞれの手順を実行します。

## ハードウェアの設定

アプリケーションは、FPGA を設定して初期化するところから開始する必要があります。これには、通常次の手順を実行します。

- 使用可能なザイリンクス プラットフォームのリストを読み出します。
- 各ザイリンクス プラットフォームでサポートされるデバイスのリストを読み出します。
- コンテキストを作成します。
- コンパイル済み FPGA バイナリ (xclbin) からプログラム オブジェクトを作成します。
- カーネル オブジェクトを作成します。

このセクションの手順を実行するには、[host.cpp](./reference-files/src/host.cpp) ファイルの手順 1 を参照してください。

> **ヒント:** この演習では C++ コードを使用しますが、C コードもリファレンス ファイルに含まれています。特定の OpenCL API 呼び出しに関する詳細は、[OpenCL リファレンス ページ](https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/)を参照してください。

1. アプリケーションは、ザイリンクス FPGA デバイスに含まれるプラットフォームを見つけるところから開始します。ザイリンクス プラットフォームが含まれているかどうかは、`cl::Platform::get` OpenCL API を使用するとわかります。この呼び出しを実行すると、システム内で使用可能なプラットフォームが返されます。

   ```Cpp
   cl::Platform::get(&platforms)
   ```

   使用可能なプラットフォームを読み出したら、ホストが特定ベンダーのプラットフォームを検証します。各プラットフォームにベンダーのインストールが含まれるので、システムにはさまざまなプラットフォームが混合している可能性があります。`cl::platform.getInfo` API 呼び出しは、使用可能な OpenCL プラットフォームに関する特定の情報を返します。このホスト コードでは、プラットフォームのベンダー情報を読み出して、それをユーザー入力の `XILINX` で確認します。

   ```Cpp
    platform.getInfo<CL_PLATFORM_NAME>(&err)
   ```

   次に、ホストが該当するプラットフォームから特定のデバイスを選択する必要があります。これには `cl::platform::getDevices` API を使用します。

   ```Cpp
   platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices)
   ```

   > **注記:** 現時点の C++ コードでは、上記の動作がまとめられて、`host.hpp` ファイル内のユーザー定義の `get_devices("Xilinx")` 内に含まれています。
   >
   > ```Cpp
   > get_devices("Xilinx")
   > ```

2. プラットフォームとデバイスを選択したら、コンテキストを作成する必要があります。コンテキストは、ランタイムがコマンド キューおよびカーネル オブジェクトなどのオブジェクトを管理するために使用されます。コンテキストは `cl::Context` OpenCLAPI を使用して作成します。

   ```Cpp
   cl::Context context(device, NULL, NULL, NULL, &err))
   ```

3. コンテキストを作成したら、コマンド キューを作成します。アプリケーションは、データ転送、カーネル実行、同期などの動作のコマンドをこのキューに含めます。この後、これらのコマンドがそのコンテキスト内で、デバイスでスケジューリングされます。コマンド キューは、`cl::CommandQueue` OpenCL API を使用して作成します。

   ```Cpp
   cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err)
   ```

4. 次に、プログラム オブジェクトを作成する必要があります。プログラム オブジェクトは、コンパイル済み FPGA バイナリ ファイル (xclbin) から作成されます。含まれるのは、ユーザー定義のカーネル関数のコレクションで、これが FPGA にプログラムされます。

   > **ヒント:** [アプリケーションのビルド](/docs/Pathway3/BuildingAnApplication.md)で説明したように、xclbin は作成したコンパイル済みのカーネル バイナリです。

   まず、アプリケーションが xclbin ファイルの内容を読み出す必要があります。このチュートリアルでは、ユーザー定義の関数 `read_binary_file` を使用します。この関数は、xclbin ファイルの内容へポインターを返します。

   ```Cpp
   fileBuf = read_binary_file(binaryFile, fileBufSize)
   ```

   この後、`cl::Program::Binaries` オブジェクトを作成して、xclbin バイナリ ファイルの内容を格納します。

   ```Cpp
   cl::Program::Binaries bins{{fileBuf, fileBufSize}}
   ```

   最後にプログラム オブジェクトを作成し、`bins` 変数に格納された xclbin バイナリの内容で初期化します。これには `cl::Program program` API を使用します。

   ```Cpp
   cl::Program program(context, devices, bins, NULL, &err)
   ```

   この手順では、FPGA を `bins` 変数に読み込んだバイナリを使用してプログラムします。問題なく終了したら、`CL_SUCCESS` と表示されます。

5. 次に、カーネル オブジェクトを作成する必要があります。カーネル オブジェクトは、どのソフトウェア アプリケーションを使用して、引数を実際のハードウェア カーネルに渡して実行するかを決めます。カーネル オブジェクトは `cl::Kernel` API を使用して作成されます。

   ```Cpp
   cl::Kernel krnl_vector_add(program,"vadd", &err)
   ```

   > **注記:** 説明した動作はほとんどのアプリケーションで同じなので、再利用できます。

## カーネルの実行

ハードウェアを設定したので、ホスト アプリケーションがデバイスに対してコマンドを発行し、カーネルと交信する準備ができました。これらのコマンドには、次が含まれます。

- FPGA デバイスのバッファー転送
- カーネル引数の設定
- FPGA でのカーネルの実行
- イベントの同期

このセクションの手順を実行するには、[host.cpp](./reference-files/src/host.cpp) ファイルの手順 2 を参照してください。

1. まず、グローバル メモリにバッファーを作成する必要があります。バッファーは、ホストとデバイス間のデータ転送に使用されます。カーネルはこれらのバッファーからデータを読み出し、処理して、バッファーに書き戻します。バッファー オブジェクトは `cl::Buffer` API を使用して作成します。

   ```Cpp
   cl::Buffer buffer_in1 (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes, source_in1.data(), &err)
   ```

   次のバッファーを作成します。

   - `buffer_in1`: source\_in1 を格納
   - `buffer_in2`: source\_in2 を格納
   - `buffer_output`: 結果 (source\_hw\_results) を格納

2. カーネルを実行する前に、その引数をそれぞれ設定する必要があります。カーネル引数は、スカラー値かバッファー オブジェクトのいずれかです。カーネル引数は `cl::Kernel::setArg` API を使用して設定します。

   ```Cpp
   krnl_vector_add.setArg(0, buffer_in1)
   ```

   これにより、カーネルに入力データの含まれる箇所、出力を格納する箇所、各バッファーのサイズなどを示すポインターが渡されます。次の引数が設定されています。

   - `in1` (入力): 入力ベクター 1
   - `in2` (入力): 入力ベクター 2
   - `out` (出力): 出力ベクター
   - `size` (入力): ベクターのサイズ (整数)

3. まず、`cl::CommandQueue::enqueueMigrateMemObjects` API を使用し、ホスト メモリからデバイス メモリ (グローバル メモリ) への入力データの転送を要求します。

   ```Cpp
   q.enqueueMigrateMemObjects({buffer_in1, buffer_in2},0/* 0 means from host*/)
   ```

4. 次に、`cl::CommandQueue::enqueueTask` API を使用してカーネルの実行を要求します。

   ```Cpp
   q.enqueueTask(krnl_vector_add)
   ```

5. この後、デバイスのグローバル メモリからホスト メモリへの出力結果の転送を要求します。これには `cl::CommandQueue::enqueueMigrateMemoryObjects` API を使用します。

   ```Cpp
   q.enqueueMigrateMemObjects({buffer_output},CL_MIGRATE_MEM_OBJECT_HOST)
   ```

6. 最後に、コマンド キューに含んだすべての要求が終了するのを待ちます。

   ```Cpp
   q.finish();
   ```

   エンキューした API 呼び出しは実際に指定したコマンドを実行するわけではなく、その実行を要求するだけです。エンキューした関数が返されても、コマンドが実際に実行されたわけではありません。コマンドの実行をスケジュールするのは、ランタイムです。このため、アプリケーションが同期手法を使用して、コマンドの終了するタイミングを知る必要があります。

## アプリケーション リターン後の FPGA のリリース

ホスト アプリケーションのビルドの最後の手順は、オブジェクトのリリースです。このセクションの手順を実行するには、[host.cpp](./reference-files/src/host.cpp) ファイルの手順 3 を参照してください。C++ ラッパーは、オブジェクトがスコープ外に渡されると、そのオブジェクトを自動的にリリースします。

## 次のステップ

次は、[アプリケーションおよびカーネルをコンパイル、リンク、実行](./building_application.md)します。</br>

<hr/>
<p align="center"><b><a href="../../docs/vitis-getting-started/">入門コースの初めに戻る</a> &mdash; <a href="./README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
