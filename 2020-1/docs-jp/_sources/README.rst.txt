.. 
   Copyright 2020 Xilinx, Inc.

.. meta::
   :keywords: Vitis, チュートリアル, コア, 開発, tutorials, core, development
   :description: Vitis コア開発キットを使用したアクセラレーション アプリケーションの開発方法について説明します。
   :xlnxdocumentclass: Document
   :xlnxdocumenttypes: Tutorials
   

   

概要
###########

.. toctree::
   :maxdepth: 5
   :caption: English Version
   :hidden:

   Master <https://xilinx.github.io/Vitis-Tutorials/master/docs/README.html>
   
   
.. _入門コース:


入門コース
========================

`Vitis™ コア開発キットを使用したアクセラレーション アプリケーションの開発方法 <./vitis-getting-started/README.html>`__ について学びます。

|pathways|

.. |pathways| image:: ./vitis-getting-started/images/pathway.png
   :target: ./vitis-getting-started/README.html

.. toctree::
   :maxdepth: 5
   :caption: 入門コース
   :hidden:

   vitis-getting-started/README
   vitis-execution-model/README
   alveo-getting-started/README
   Pathway3/README
   my-first-program/README
   convolution-tutorial/README
   bloom/README



.. _中級チュートリアル:

中級チュートリアル
=============

.. list-table:: 
   :widths: 30 20 50
   :header-rows: 1

   * - チュートリアル
     - カーネル
     - 内容

   * - `RTL カーネル入門 <./getting-started-rtl-kernels/README.html>`__
     - RTL
     - Vitis コア開発キットを使用して RTL カーネルを FPGA にプログラムし、一般的な開発フローでハードウェア エミュレーションをビルドする方法を説明します。

   * - `Vitis HLS の解析および最適化 <./vitis_hls_analysis/README.html>`__
     - C
     - Vitis HLS ツールの GUI を使用して、ハードエア カーネルをビルド、解析、最適化する方法を説明します。

   * - `C と RTL の混合 <./mixing-c-rtl-kernels/README.html>`__
     - C および RTL
     - RTL カーネルと OpenCL™ カーネルを含むアプリケーションを使用して、Vitis コア開発キット フローとさまざまなデザイン解析機能を試してみます。

   * - `複数の計算ユニットの使用 <using-multiple-cu/README.html>`__
     - C および RTL
     - FPGA のカーネル インスタンス数を増加する柔軟なカーネル リンク プロセスを使用して、統合したホスト カーネル システムの並列処理を改善する方法を説明します。  

   * - `ホスト コードの最適化 <host-code-opt/README.html>`__
     - C および RTL
     - デザインにホスト コード最適化手法を適用する方法を説明します。

   * - `複数 DDR バンクの使用 <mult-ddr-banks/README.html>`__
     - C および RTL
     - 複数の DDR を使用してカーネルとグローバル メモリ間のデータ転送を向上する方法を説明します。  

.. toctree::
   :maxdepth: 5
   :caption: 中級チュートリアル
   :hidden:

   getting-started-rtl-kernels/README
   vitis_hls_analysis/README
   mixing-c-rtl-kernels/README
   using-multiple-cu/README
   host-code-opt/README
   mult-ddr-banks/README


.. _上級チュートリアル:

上級チュートリアル
=========


.. list-table:: 
   :widths: 30 20 50
   :header-rows: 1

   * - チュートリアル
     - カーネル
     - 内容

   * - `Vivado インプリメンテーションの制御 <./controlling-vivado-impl/README.html>`__
     - RTL
     - プロジェクトをインプリメントする際に Vivado® ツール フローを制御する方法を説明します。

.. toctree::
   :maxdepth: 2
   :caption: 上級チュートリアル
   :hidden:

   controlling-vivado-impl/README

.. _バージョン:

.. toctree::
   :maxdepth: 2
   :caption: バージョン
   :hidden:

   Master <https://xilinx.github.io/Vitis-Tutorials/2020-1/docs/README.html>
   
   2019.2 <https://github.com/Xilinx/Vitis-Tutorials/blob/Vitis-Tutorials-2019.2-Hotfix1/README.md>

この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。 日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。    
	 


   