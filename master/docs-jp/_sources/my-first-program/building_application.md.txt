<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 3\. アプリケーションのコンパイル、リンク、実行

## 概要

Vitis コア開発キットのアプリケーションは、ソフトウェア プログラムと FPGA バイナリの 2 つのコンポーネントを含むヘテロジニアス アプリケーションです。これらの 2 つのコンポーネントは、専用のコンパイル チェーンを使用してビルドします。この演習では、コマンド ラインから VADD サンプルをコンパイル、リンク、および実行する方法について説明します。

## アプリケーションのビルド

### ホスト プログラムのビルド

OpenCL™ API 呼び出しを使用して C/C++ で記述されたホスト アプリケーションは、標準 CGG コンパイラを使用してビルドできます。各ソース ファイルはオブジェクト ファイル (O) にコンパイルされ、ザイリンクス ランタイム (XRT) 共有ライブラリとリンクされて、実行ファイル (EXE) が作成されます。GCC および関連するコマンド ライン オプションの詳細は、[GNU コンパイラ コレクション (GCC) の使用](https://gcc.gnu.org/onlinedocs/gcc/)を参照してください。

ホスト コードをビルドするには、`./reference-files/src` ファイルに次の XCPP コマンドを使用します。

```bash
g++ -I$XILINX_XRT/include/ -I$XILINX_VIVADO/include/ -Wall -O0 -g -std=c++11 ./src/host.cpp  -o 'host'  -L$XILINX_XRT/lib/ -lOpenCL -lpthread -lrt -lstdc++
```

このコマンドは、XRT、Vivado ツール、OpenCL API に必要なライブラリおよびインクルード ファイルを指定します。ホストのビルド方法の詳細は、「アクセラレーションされたアプリケーションをビルドして実行するための基本的な概念」チュートリアルの[アプリケーションのビルド](../Pathway3/BuildingAnApplication.md)演習を参照してください。

> **ヒント**: ここに示すコマンドを実行するには、[アプリケーションのビルド](../Pathway3/BuildingAnApplication.md)で説明するように、Vitis コア開発キットの環境を設定する必要があります。

### FPGA バイナリのビルド

「アクセラレーションされたアプリケーションをビルドして実行するための基本的な概念」チュートリアルの[アプリケーションのビルド](../Pathway3/BuildingAnApplication.md)演習で説明するように、ハードウェアのビルドには、ソフトウェア アプリケーションのビルドと同様、Vitis コンパイラを使用してコンパイルおよびリンクする必要があります。

次のコマンドのビルド ターゲットはソフトウェア エミュレーションです。「アクセラレーションされたアプリケーションをビルドして実行するための基本的な概念」チュートリアルの[アプリケーションのビルド](../Pathway3/BuildingAnApplication.md)演習で説明したように、`hw_emu` および `hw` などの別のビルド ターゲットを指定することもできます。

* 次のコマンドを使用して VADD サンプルのハードウェア カーネルをコンパイルします。

  ```
  v++ -t sw_emu --config design.cfg -c -k vadd -I'../src' -o 'vadd.xilinx_u200_xdma_201830_2.xo' './src/vadd.cpp'
  ```

* 次のコマンドを使用して VADD サンプルのハードウェア カーネルをリンクします。

  ```
  v++ -t sw_emu --config design.cfg -l -o'vadd.xilinx_u200_xdma_201830_2.xclbin' vadd.xilinx_u200_xdma_201830_2.xo
  ```

## エミュレーションの実行

ハードウェアでデザインを実行する前に、[ソフトウェアおよびハードウェア エミュレーションの実行](../Pathway3/Emulation.md)モードでデザインを実行して、機能を検証しておいてください。エミュレーション モードでのアプリケーションの実行は、次の 2 段階のプロセスです。

1. エミュレーション コンフィギュレーション ファイルを生成します。
2. アプリケーションを実行します。

### ソフトウェア エミュレーション

1. ソフトウェア エミュレーション モードで実行するには、エミュレーション コンフィギュレーション ファイルを生成し、XCL\_EMULATION\_MODE 環境変数を設定します。

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   export XCL_EMULATION_MODE=sw_emu  
   ```

2. コンフィギュレーション ファイル (`emconfig.json`) を生成して XCL\_EMULATION\_MODE を設定したら、次のコマンドを使用してホスト プログラムとカーネルをソフトウェア エミュレーション モードで実行します。

   ```bash
   ./host vadd.xilinx_u200_xdma_201830_2.xclbin
   ```

3. エミュレーションの実行結果を確認します。

   ```
   Found Platform
   Platform Name: Xilinx
   INFO: Reading vadd.xilinx_u200_xdma_201830_2.xclbin
   Loading: 'vadd.xilinx_u200_xdma_201830_2.xclbin'
   TEST PASSED
   ```

> **重要:** RTL カーネルでは、C モデルが関連付けられている場合にソフトウェア エミュレーションがサポートされます。C モデルがない場合は、ハードウェア エミュレーションを使用してカーネル コードをデバッグする必要があります。

### ハードウェア エミュレーション

1. ハードウェア エミュレーションには、`-t hw_emu` オプションを使用してカーネルをビルドし直します。

   ```bash
   v++ -t hw_emu --config design.cfg -c -k vadd -I'../src' -o 'vadd.xilinx_u200_xdma_201830_2.xo' './src/vadd.cpp'
   v++ -t hw_emu --config design.cfg -l -o 'vadd.xilinx_u200_xdma_201830_2.xclbin' vadd.xilinx_u200_xdma_201830_2.xo
   ```

2. ハードウェア エミュレーション モードで実行するには、エミュレーション コンフィギュレーション ファイルを生成し、XCL\_EMULATION\_MODE 環境変数を設定します。

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   export XCL_EMULATION_MODE=hw_emu
   ```

3. 次のコマンドを使用し、ホスト プログラムとカーネルをハードウェア エミュレーション モードで実行します。

   ```
   ./host vadd.xilinx_u200_xdma_201830_2.xclbin
   ```

4. エミュレーションの実行結果を確認します。

   ```
   Found Platform
   Platform Name: Xilinx
   INFO: Reading vadd.xilinx_u200_xdma_201830_2.xclbin
   Loading: 'vadd.xilinx_u200_xdma_201830_2.xclbin'
   INFO: [HW-EM 01] Hardware emulation runs simulation underneath. Using a large data set will result in long simulation times. It is recommended that a small dataset is used for faster execution. The flow uses approximate models for DDR memory and interconnect and hence the performance data generated is approximate.
   TEST PASSED
   INFO: [Vitis-EM 22] [Wall clock time: 16:39, Emulation time: 0.108767 ms] Data transfer between kernel(s) and global memory(s)
   vadd_1:m_axi_gmem-DDR[1]          RD = 32.000 KB              WR = 16.000 KB
   ```

## ハードウェアでの実行

エミュレーション モードを使用して機能の正しさとホストとカーネルの統合を検証したら、ザイリンクス FPGA の Alveo データセンター アクセラレータ カードをターゲットにします (`-t hw`)。

### ハードウェア実行

1. ハードウェア実行では、`-t hw` オプションを使用してカーネルをビルドし直します。

   ```bash
   v++ -t hw --config design.cfg -c -k vadd -I'../src' -o'vadd.xilinx_u200_xdma_201830_2.xo' './src/vadd.cpp'
   v++ -t hw --config design.cfg -l -o 'vadd.xilinx_u200_xdma_201830_2.xclbin' vadd.xilinx_u200_xdma_201830_2.xo
   ```

2. XCL\_EMULATION\_MODE 環境変数が設定されていないことを確認します。

   ```
   unset XCL_EMULATION_MODE
   ```

   この段階では、デザインを Vivado ツールで合成して FPGA ハードウェアに読み込むビットストリームにパッケージする必要があるので、ハードウェア ビルドに時間がかかる可能性があります。

3. ビルドが準備でき、環境変数を正しく設定したら、デザインを Alveo データセンター アクセラレータ カードで実行します。

   ```
   ./host vadd.xilinx_u200_xdma_201830_2.xclbin
   ```

4. エミュレーションの実行結果を確認します。

   ```
   Found Platform
   Platform Name: Xilinx
   INFO: Reading vadd.xilinx_u200_xdma_201830_2.xclbin
   Loading: 'vadd.xilinx_u200_xdma_201830_2.xclbin'
   TEST PASSED
   ```

## makefile の使用

makefile を使用すると、コマンドおよびオプションを整理でき、必要に応じてビルドを再実行しやすくなります。`./reference-files` に含まれる `makefile` には、上記に定義したコマンドが含まれており、ビルドをクリーンアップし、プロジェクトをビルドし直すための構造も含まれます。

この `makefile` を使用するには、使用しているホスト コードのビルド ターゲット (TARGET)、ハードウェア (DEVICE)、およびバージョン (VER) を指定する必要があります。

* ソフトウェア エミュレーションをターゲットにする場合は、`reference_files` ディレクトリで次の `make` コマンドを使用します。

  ```
    make check TARGET=sw_emu DEVICE=xilinx_u200_xdma_201830_2 VER=host_cpp
  ```

* ハードウェア エミュレーションをターゲットにする場合は、`reference_files` ディレクトリで次の `make` コマンドを使用します。

  ```
  make check TARGET=hw_emu DEVICE=xilinx_u200_xdma_201830_2 VER=host_cpp
  ```

* ハードウェアをターゲットにするには、`reference_files` ディレクトリで次の `make` コマンドを使用します。

  ```
  make check TARGET=hw DEVICE=xilinx_u200_xdma_201830_2 VER=host_cpp
  ```

## 次の手順

次の手順では、[アプリケーションのプロファイリング](./profile_debug.md)を実行します。</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="../../docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="./README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
