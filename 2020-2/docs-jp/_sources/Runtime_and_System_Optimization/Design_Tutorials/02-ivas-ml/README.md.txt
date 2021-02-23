<p align="right"><a href="../../../../README.md">English</a> | <a>日本語</a>

# IVAS ZCU104 ML アクセラレーション リファレンス リリース

**必要なツール バージョン: 2020.1 と Vitis AI 1.2**

このリポジトリには、マルチチャネル機械学習システムでザイリンクス IVAS フレームワークを使用するためのリファレンス デザインが含まれています。次のものが含まれます。

- **hw\_src**: Vitis プロジェクトをビルドするために必要なハードウェア ソース ファイルが含まれます。

- **hls\_tests**: 個々のカーネルを検証するための HLS スクリプトおよびテスト データが含まれます。

- **sw\_src**: 個々のテスト アプリケーションと gstreamer パイプライン両方のソース コードが含まれます。

- **platform**: プラットフォーム ソースが含まれます。

- **vitis\_ai\_library**: Vitis AI ライブラリの内容のオーバーライドがいくつか含まれます。

- **vitis\_patch**: Vitis 用の SD カード パッケージ パッチが含まれます。

**注意: Vitis パッチが必要:** このデザインには大きな rootfs が含まれており、Vitis 2020.1 では 2 GB を超える ext4 パーティションを含む SD カード イメージをパッケージできません。このパッチは、パッケージ フローを変更し、初期 rootfs サイズを、ext4 パーティション サイズを超える 512 MB の最初の倍数に丸めます。インストールするには、次のコマンドを使用します。

```bash
$ cp ./vitis_patch/mkfsImage.sh ${XILINX_VITIS}/scripts/vitis/util
```

## ビルド手順

#### 1\.  リポジトリのクローン (サブモジュールを含む)

```bash
$ git clone --recurse-submodules https://github.com/Xilinx/Vitis-In-Depth-Tutorial
```

***注記: このリポジトリのクローンを作成するパスが NFS マウントされていないことを確認してください。***

#### 2\.  ハードウェア プラットフォームのビルド

source コマンドを使用して Vitis と PetaLinux の設定ファイルを読み込み、platform/dev/zcu104\_vcu ディレクトリに移動して **make** を実行します。プラットフォーム ソースが platform/repa/xinxy\_zcu104\_VCU\_2020202020\_1 に出力されます。プラットフォームをビルドしたら、sysroot を抽出してインストールします。

```bash
$ cd platform/dev/zcu104_vcu

$ make

$ cd ../../repo/xilinx_zcu104_vcu_202010_1/sysroot

$ ./sdk.sh -d `pwd` -y
```

#### 3\.  Vitis デザインのビルド

XRT のインストール エリアから source コマンドを使用して **setup.sh** スクリプトを実行します。このプラットフォームは、2020.1 リリースの XRT バージョン 2.6.655 でビルドされています。バージョン 2.7.766 もサポートされています。これらのバージョンのいずれかを使用する必要があります。ほかのバージョンの XRT でのビルドはサポートされていません。

```bash
source /opt/xilinx/xrt/setup.sh
```

hw\_src ディレクトリのパッチを Vitis ビジョン ライブラリに適用する必要があります。これは 1 回だけの操作です。

```bash
$ cd hw_src/Vitis_Libraries

$ patch -p1 < ../vision_lib_area_resize_ii_fix.patch
```

パッチを適用したら、ハードウェア デザインをビルドします。

```bash
$ cd hw_src

$ make
```

***注記: これ以降の操作には、新しいターミナルを開始することを強くお勧めします。***

#### 4\.  source コマンドを使用してクロスコンパイル ツールチェーンを読み込み

```bash
$ cd platform/repo/xilinx_zcu104_vcu_202010_1/sysroot/

$ source ./environment-setup-aarch64-xilinx-linux
```

#### 5\.  ターゲット ソフトウェアのビルド

これには cmake と meson を組み合わせて使用しますが、まずクロスコンパイル環境を設定する必要があります。ビルド プロセスには、異なるビルド システムを使用した複数の概念サブモジュールが含まれているので、これを自動化し、さまざまなプロセスの明確にするのに役立つスクリプトが提供されています。

このスクリプトは、次の処理を自動的に実行します。

- pkfconfig のクロスコンパイル rootfs に XRT コンポーネントが存在しない場合は、追加します。
- Vitis AI クロスコンパイル コンポーネントを rootfs に展開します。
- ターゲットの meson クロスコンパイル ファイルを設定します。
- すべてのソース リファレンスをビルドします。

***注記: ml\_models フォルダーを作成し、生成された densebbox および reid モデルをその中に保持してください。***

モデルのコンパイルについては、Vitis AI 1.2 の資料を参照してください。

```bash
- ml_models/dpu_densebox_640_360.elf
- ml_models/dpu_reid.elf
```

```bash
$ cd sw_src

$ ./go.sh
```

## ブート用の SD カード イメージの準備

#### 1\.  マイクロ SD カードへのイメージの書き込み

dd、Win32DiskImager、Balena Etcher などの SD カード書き込みツールを使用して、SD カードにイメージ `hw_src/sd_card_zcu104/sd_card.img` を書き込み、ボードを起動します。

このバージョンでは、SD カードに保存されている不揮発性ルートファイル システムを使用します。インストール後に必要な設定がいくつかあります。特に記載がない限り、次の手順はターゲットのブート後に実行します。

#### 2\.  sd\_card ユーティリティのコピー

- ターゲットの /home/root に `sd_card_utils/*` をコピーします。次の手順で、これらのスクリプトと JSON ファイルを使用します。ホストで次のコマンドを実行します。

```bash
$ cd sd_card_utils

$ scp -r * root@<zcu104 board ip>:~

$ scp -r .bashrc root@<zcu104 board ip>:~
```

#### 3\.  ソフトウェア スタックのコピー

- SSH などを使用して、`sw_src/install` からの update.tar.gz をボードのルート (/) ディレクトリにコピーします。

```bash
$ cd <project_root>/sw_src/install

$ scp update.tar.gz root@<zcu104 board ip>:/
```

***注記: 残りの手順はすべてターゲットで実行します。***

#### 4\. rootfs のサイズ変更

SD カードの残りのスペースを使用するようにルート ファイル システム パーティションのサイズを変更します。`df -h` を使用すると、前後に使用済みのディスク スペースを確認できます。

```bash
$ cd ~/scripts

$ ./ext4_auto_resize.sh
```

#### 5\.  ソフトウェア ライブラリと Vitis AI のインストール

```bash
$ cd scripts

$ ./update.sh

$ ./install_vai.sh
```

***注記: Vitis AI インストール スクリプトを実行するには、ボードにインターネット接続が必要です。スクリプトは wget を使用してダウンロードするので、プロキシ設定を指定できる場合があります。これが不可能な場合は、スクリプト ソースを確認し、手順を手動で実行してください。***

- 追加のテスト データ (ビデオなど) を同じホーム/ルート ディレクトリにコピーします。ビデオはこのリポジトリの一部ではないことに注意してください。

***この時点で、すべての設定変更を有効にするため、ボードをリブートする必要があります。`shutdown -h now`、`reboot` などを実行せずに電源を切ることはしないでください。***

## アプリケーションの実行

```bash
$ cd /home/root/scripts

$ ./1ch_crop_reid.sh
```

- **オプション:** Vitis 2020.1 から、ターゲット用のさまざまなツールをインストールするためのパッケージ フィードが提供されています。使用するパッケージ マネージャーは dnf です。このコマンド ライン構文は、apt や yum と同様です。これには、ネットワーク接続と場合によってはプロキシ設定が必要です。このパッケージ フィードを設定し、`vim` ユーティリティなどをインストールするには、次の手順を実行します。

```bash
$ cd /home/root/scripts

$ ./package_feed_setup.sh

$ dnf install vim
```

DNF パッケージ フィードのセットアップ中に警告が表示される場合がありますが、これは無視してもかまいません。

<p align="center" class="sphinxhide"><b><a href="/README.md">メイン ページに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 1 月 22 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
