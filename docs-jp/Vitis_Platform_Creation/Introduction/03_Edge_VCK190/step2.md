﻿<!--
# Copyright 2020 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
-->
<p align="right"><a href="../../../README.md">English</a> | <a>日本語</a></p>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Versal カスタム プラットフォーム作成チュートリアル</h1>
    </td>
 </tr>
</table>

## 手順 2: PetaLinux でのソフトウェア コンポーネントの作成

この手順では、Vitis プラットフォームの必須コンポーネントを含む PetaLinux プロジェクトを作成します。

### PetaLinux プロジェクトの作成

1. PetaLinux 環境を `source <petaLinux_tool_install_dir>/settings.sh` で設定します。

2. 前の手順で作成した XSA を使用して PetaLinux プロジェクトを作成します。

   ```bash
   petalinux-create -t project --template versal --force -n petalinux
   cd petalinux
   petalinux-config --get-hw-description=<path to xsa directory> --silentconfig
   ```

   - 作成される PetaLinux プロジェクト名は **petalinux** です。PetaLinux プロジェクト名は **petalinux-create -n** オプションで自由に変更できます。

3. VCK190 デバイス ツリーを適用します。

   - `petalinux-config` を実行します。
   - **\[DTG Settings]** をクリックします。
   - **MACHINE\_NAME** オプション (コンフィギュレーション オプション名は CONFIG\_SUBSYSTEM\_MACHINE\_NAME) に **versal-vck190-reva-x-ebm-02-reva** を入力します。

   **注記**: このプリセット デバイス設定により、デバイス ツリーにイーサネット PHY 情報が追加されます。[デバイス ツリーのソース コード][1]が PetaLinux プロジェクトに適用されます。VCK190 ボードのバージョンが異なる場合は、デバイス ツリーのソース コード ディレクトリにボードに対応するデバイス ツリーのバージョンがあるかどうかを確認し、PetaLinux プロジェクトで適用してください。git リポジトリをクローンしたり、GitHub のファイル検索機能を使用できます。

   ![GitHub デバイス ツリー リスト](images/step2/github_find_available_device_tree.png)

[1]: https://github.com/Xilinx/u-boot-xlnx/blob/master/arch/arm/dts/versal-vck190-revA-x-ebm-02-revA.dts ""
### ルート ファイル システム、カーネル、デバイス ツリー、および U-Boot のカスタマイズ

1. **\<your\_petalinux\_project\_dir>/project-spec/meta-user/conf/user-rootfsconfig** ファイルに下に示す CONFIG\_x 行を追加して、ユーザー パッケージを追加します。

   ***注記: この手順は必須ではありませんが、次の手順で必要なパッケージをすべて見つけやすくなります。***

   ベースの XRT サポート用のパッケージ (必須):

   ```
   CONFIG_packagegroup-petalinux-xrt
   ```

   - XRT パッケージ グループには、Vitis アクセラレーション ランタイム用のすべてのパッケージが含まれます。

   オンボード アクセラレーション アプリケーション コンパイル サポート用パッケージ (オプション):

   ```
   CONFIG_packagegroup-petalinux-xrt-dev
   ```

   - パッケージ名には `-dev` が最後に付き、Yocto で環境をコンパイルする際に必要なヘッダー ファイル、依存ライブラリ、ソフト リンクが含まれます。

   システム管理を容易にするパッケージ:

   ```
   CONFIG_dnf
   CONFIG_e2fsprogs-resize2fs
   CONFIG_parted
   CONFIG_auto-login
   ```

   - **dnf** はパッケージ管理ツールです。
   - **auto-login** はデフォルトでルートとしてログインします。開発やデバッグの際に便利です。プロダクション段階では削除してください。
   - **parted** および **e2fsprogs-resize2fs** は ext4 パーティションを拡張して残りの SD カードを使用できるようにします。

2. `petalinux-config -c rootfs` を実行して **user packages** を選択し、上記すべてのライブラリの rootfs の名前を選択し、保存して終了します。手順 1 を飛ばした場合は、検索機能で `/` を使用してこれらのパッケージを検索し、イネーブルにしてください。

   ![petalinux rootfs 設定](./images/step2/petalinux_user_packages.png)

3. rootfs 設定メニューで **\[Image Features]** を選択し、**\[package-management]** および **\[debug\_tweaks]** をオンにし、変更を保存して終了します  (推奨)。

   ![](./images/step2/petalinux_package_management.png)

4. システム ユーザー デバイス ツリーをアップデートします (必須)。

   **project-spec/meta-user/cetie-bsp/device-tree/files/system-user.dtsi** ファイルに次の内容を追加します。

   - **zyxclmm\_drm** ノードは、XRT の一部である zocl ドライバーに必要です。
   - **axi\_intc\_0** ノードは、32 の割り込み入力を定義します。これは、Vivado のハードウェア設定では推論できませんので、手動でここに追加する必要があります。
   - **注記**: サンプル ファイルは **ref\_files/step2\_petalinux/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi** に含まれます。

   ```
   &amba {
       zyxclmm_drm {
           compatible = "xlnx,zocl-versal";
           status = "okay";
           interrupt-parent = <&axi_intc_0>;
           interrupts = <0  4>, <1  4>, <2  4>, <3  4>,
                    <4  4>, <5  4>, <6  4>, <7  4>,
                    <8  4>, <9  4>, <10 4>, <11 4>,
                    <12 4>, <13 4>, <14 4>, <15 4>,
                    <16 4>, <17 4>, <18 4>, <19 4>,
                    <20 4>, <21 4>, <22 4>, <23 4>,
                    <24 4>, <25 4>, <26 4>, <27 4>,
                    <28 4>, <29 4>, <30 4>, <31 4>;
       };
   };

   &axi_intc_0 {
         xlnx,kind-of-intr = <0x0>;
         xlnx,num-intr-inputs = <0x20>;
   };
   ```

5. EXT4 rootfs サポートを追加します (推奨)。

   PetaLinux は、デフォルトで rootfs に **initrd** 形式を使用します。この形式は、DDR メモリ内の rootfs を抽出します。つまり、実行時に使用可能な DDR メモリが減り、再起動後に rootfs の変更を保持できないことを意味します。ルート ファイル システムで変更が保持できるようにするには、SD カード上の 2 つ目のパーティションとして rootfs に EXT4 形式を使用し、1 つ目のパーティション FAT32 にはほかのブートファイルを保存します。

   - `petalinux-config` を実行し、**\[Image Packaging Configuration]** に移動し、**\[Root File System Type]** に **\[EXT4]** を選択し、**\[Root File System Formats]** に `ext4` を追加します。

   ![](./images/step2/petalinux_root_filesystem_type.png)

   - bootargs をアップデートして、ext4 パーティションを system-user.dtsi のルート ファイル システムとして使用します。

     ```
     / {
         chosen {
            stdout-path = "serial0:115200";
            bootargs = "console=ttyAMA0 earlycon=pl011,mmio32,0xFF000000,115200n8 clk_ignore_unused root=/dev/mmcblk1p2 rw rootwait rootfs=ext4";
         };
     };
     ```

   **注記**: `root=/dev/mmcblk1p2` は rootfs パスです。`rootfs=ext4` は rootfs タイプを定義します。[ref\_files/step2\_petalinux/system-user.dtsi](ref_files/step2_petalinux/system-user.dtsi) の完全な system-user.dtsi を参照してください。

### PetaLinux イメージのビルド

1. PetaLinux プロジェクト内の任意のディレクトリから、PetaLinux プロジェクトをビルドします。

   ```
   petalinux-build
   ```

   生成された U-Boot イメージと Linux イメージは、**images/linux** ディレクトリに保存されます。

2. sysroot をビルドしてインストールします。

   ```
   petalinux-build --sdk
   ```

   sysroot **sdk.sh** は **images/linux** ディレクトリに生成されます。これは、次の手順で抽出します。

### スクリプトを使用した実行

PetaLinux プロジェクトを再作成し、出力を生成するスクリプトが提供されています。これらのスクリプトを使用するには、次の手順を実行します。

1. ビルドを実行します。

   ```
   # cd to the step directory, e.g.
   cd step2_petalinux
   make
   ```

2. 生成されたファイルをクリーンアップするには、次を実行します。

   ```bash
   make clean
   ```

**注記: ハードウェア プラットフォームとソフトウェア プラットフォームがすべて生成されます。次に、[Vitis プラットフォームをパッケージ](./step3.md)します。**

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
