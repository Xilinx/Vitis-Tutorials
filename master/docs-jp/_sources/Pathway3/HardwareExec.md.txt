<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 4\. ハードウェアでの実行

> **重要:** この演習では、コンパイル済みカーネル ライブラリ `.xclbin` を実行するため、Alveo データセンター アクセラレータ カードを使用する必要があります。Alveo カードをインストールしたマシンにアクセスがない場合、この演習を終了できません。

## 概要

ハードウェア実行では、ホスト プログラムがホスト マシンで実行され、カーネル コードが Alveo アクセラレータ カードでザイリンクス デバイスを使用して実行されます。ハードウェア実行のパフォーマンス データは、実際のハードウェアからの実際のパフォーマンスになります。前の演習で説明したプロファイル サマリおよびタイムライン トレース レポートを含む実行サマリも使用できます。

> **ヒント:** システム ビルドのデバイス プロファイリングオンにすると (`-t hw`)、全体的なアプリケーションのパフォーマンスに悪影響を与える可能性があります。この機能は、ハードウェア上でのパフォーマンス デバッグにのみ使用し、プロダクション ビルドではオフにする必要があります。

## 開始前の確認事項

この演習を実行する前に、[アプリケーションのビルド](./BuildingAnApplication.md)、[ソフトウェアおよびハードウェア エミュレーションの実行](./Emulation.md)、および[プロファイルおよびトレース レポートの生成](./ProfileAndTraceReports.md)演習を確認して実行しておきます。この演習では、これらの演習と同じソース ファイルおよびオプションを使用します。ホストおよびハードウェア プラットフォームをビルドするのに必要なコマンドをすべて表示します。

## デザインのビルド

ザイリンクス ボードまたはデバイスでアプリケーションを実行する前に、`v++` コマンドに `-t hw` オプションを使用してデザインをビルドする必要があります。

ホスト ソフトウェアおよびハードウェアのビルド方法および \<*build\_target*> の指定方法は、[アプリケーションのビルド](./BuildingAnApplication.md)演習を参照してください。

> **重要:** サンプルを実行する前に、ランタイム スクリプトを読み込むようにしてください。
>
> ```bash
> #Setup runtime. XILINX_XRT will be set in this step
> #Note that it's only necessary to setup runtime before deployment on hardware.
> source /opt/xilinx/xrt/setup.sh
> ```
>
> **ヒント:** また、XCL\_EMULATION\_MODE 環境は設定しないようにします。前の演習では、これを `sw_emu` または `hw_emu` に設定しました。

次のコマンドを実行します。

```bash
unset XCL_EMULATION_MODE
```

## アプリケーションのビルド

ホスト プログラムおよびハードウェア プラットフォーム (`xclbin`) を使用すると、アプリケーションをザイリンクス ボードまたはデバイスで実行できます。ホスト プログラムおよびハードウェア プラットフォームをまだビルドしていない場合は、次のコマンドを使用します。

> **重要:** FPGA バイナリ ファイルの合成およびプリメンテーションのため、ハードウェア ターゲットのコンパイルいおよびリンクにはかなり時間がかかることがあります。

```bash
source /opt/Xilinx/Vitis/2019.2/settings64.sh
source /opt/xilinx/xrt/setup.sh
#make sure working path is in reference-files/run
g++ -I$XILINX_XRT/include/ -I$XILINX_VIVADO/include/ -Wall -O0 -g -std=c++11 ../src/host.cpp  -o 'host'  -L$XILINX_XRT/lib/ -lOpenCL -lpthread -lrt -lstdc++
v++ -t hw --config design.cfg -c -k mmult -I'../src' -o 'mmult.hw.xilinx_u200_xdma_201830_2.xo' '../src/mmult.cpp'
v++ -t hw --config design.cfg -l -o 'mmult.hw.xilinx_u200_xdma_201830_2.xclbin' mmult.hw.xilinx_u200_xdma_201830_2.xo
```

## アプリケーションの実行

```bash
./host mmult.hw.xilinx_u200_xdma_201830_2.xclbin
```

アプリケーションが問題なく実行されると、コンソールに次のように表示されます。

```
Loading: 'mmult.hw.xilinx_u200_xdma_201830_2.xclbin'
TEST PASSED
```

## xbutil を使用したボード ステータスの確認

`xbutil` の使用はオプションですが、ザイリンクス ボードに関する解析およびデバッグに便利なツールです。たとえば、次のコマンドを実行してボード ステータスを確認できます。

```bash
xbutil query
```

ボード ステータス、計算ユニット ステータスなどが表示されます。`xbutil` の詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis 環境リファレンス資料](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=yxl1556143111967.html)を参照してください。

## 次の手順

これで、Vitis コア開発キットを使用してアプリケーションをビルドして実行するために必要な手順を理解できたはずです。これには、ソフトウェア プログラムおよびカーネルのコード記述、アプリケーションのビルド、ソフトウェアおよびハードウェア エミュレーションの実行、レポートの確認、アクセラレータ カードでのアプリケーションの実行が含まれます。次の手順では、これを踏まえた上で[最初のプログラムをビルド](../my-first-program/README.md)します。</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="./README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
