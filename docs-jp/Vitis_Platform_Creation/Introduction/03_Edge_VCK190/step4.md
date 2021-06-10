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

## 手順 4: プラットフォームのテスト

### テスト 1: プラットフォーム情報の読み込み

Vitis 環境設定では、**platforminfo** ツールで XPFM プラットフォーム情報をレポートできます。

ハードウェア コンフィギュレーション (クロック、メモリ) およびソフトウェア コンフィギュレーション (ドメイン) が正しく設定されていることを確認できます。

<details>
  <summary><b>ログを表示</b></summary>

```bash
 # Find the XPFM file
 cd pfm
 find . -name "*.xpfm"
 cd <xpfm directory>  
 # Report Platform Info
 platforminfo vck190_custom.xpfm
 ==========================
 Basic Platform Information
 ==========================
 Platform:           vck190_custom
 File:               /scratch/rickys/work/vck190_platform/merge/Tutorial-VCK190_Custom/ref_files/step3_pfm/platform_repo/vck190_custom/export/vck190_custom/vck190_custom.xpfm
 Description:        
 A custom platform VCK190 platform


 =====================================
 Hardware Platform (Shell) Information
 =====================================
 Vendor:                           xilinx.com
 Board:                            xilinx_vck190_base
 Name:                             xilinx_vck190_base
 Version:                          1.0
 Generated Version:                2020.2
 Hardware:                         1
 Software Emulation:               1
 Hardware Emulation:               1
 Hardware Emulation Platform:      0
 FPGA Family:                      versal
 FPGA Device:                      xcvc1902
 Board Vendor:                     xilinx.com
 Board Name:                       xilinx.com:vck190_es:1.1
 Board Part:                       xcvc1902-vsva2197-2MP-e-S-es1

 =================
 Clock Information
 =================
 Default Clock Index: 0
 Clock Index:         0
    Frequency:         149.998499
 Clock Index:         1
    Frequency:         99.999000
 Clock Index:         2
    Frequency:         299.996999

 ==================
 Memory Information
 ==================
 Bus SP Tag: DDR

 =============================
 Software Platform Information
 =============================
 Number of Runtimes:            1
 Default System Configuration:  vck190_custom
 System Configurations:
 System Config Name:                      vck190_custom
 System Config Description:               vck190_custom
 System Config Default Processor Group:   xrt
 System Config Default Boot Image:        standard
 System Config Is QEMU Supported:         1
 System Config Processor Groups:
    Processor Group Name:      aiengine
    Processor Group CPU Type:  ai_engine
    Processor Group OS Name:   aie_runtime
    Processor Group Name:      xrt
    Processor Group CPU Type:  cortex-a72
    Processor Group OS Name:   linux
 System Config Boot Images:
    Boot Image Name:           standard
    Boot Image Type:           
    Boot Image BIF:            vck190_custom/boot/linux.bif
    Boot Image Data:           vck190_custom/xrt/image
    Boot Image Boot Mode:      sd
    Boot Image RootFileSystem:
    Boot Image Mount Path:     /mnt
    Boot Image Read Me:        vck190_custom/boot/generic.readme
    Boot Image QEMU Args:      vck190_custom/qemu/pmc_args.txt:vck190_custom/qemu/qemu_args.txt
    Boot Image QEMU Boot:      
    Boot Image QEMU Dev Tree:  
 Supported Runtimes:
 Runtime: OpenCL


```

</details>

### テスト 2: PL アクセラ アプリケーションの実行

プラットフォームの機能を検証するために、PL カーネルを使用したアクセラレーション プロジェクトを作成します。このテストには、Vitis にビルトインされているベクター加算のテンプレートを使用します。

1. Vitis を起動します。

   ```
   cd ref_files/step4_verify
   vitis -workspace ./ &
   ```

   起動したら \[Welcome] タブを閉じます。

2. カスタム プラットフォームを追加します。

   - \[Xilinx] → \[Add custom platform] をクリックします。
   - \[Add] ボタンをクリックします。
   - 手順 3 で生成したディレクトリを選択します  (例: ref\_files/step3\_pfm/platform\_repo ディレクトリ)。
   - \[OK] をクリックします。

3. カスタム プラットフォームでベクター加算アプリケーションを作成します。

   - **\[File] → \[New] → \[Application Project]** をクリックします。
   - \[Welcome] ページで **\[Next]** をクリックします。
   - \[Platform] ページで、リストから **\[vck190\_custom]** プラットフォームを選択します。**\[Next]** をクリックします。
   - アプリケーション名を **vadd** に、ターゲット プロセッサを **psv\_cortexa72\_SMP** に設定します。**\[Next]** をクリックします。
   - sysroot パス、rootfs パス、およびカーネル イメージを入力します。**\[Next]** をクリックします。
   - **\[Vector Addition]** テンプレートを選択します。**\[Finish]** をクリックします。

4. ハードウェア用のベクター加算アプリケーションをビルドします。

   - **vadd\_system** プロジェクトを選択します。
   - ツールバーの **\[Build]** (ハンマー アイコン) のドロップダウンをクリックし、**\[Hardware]** を選択します。この手順は、**\[Active Build Configuration]** に **\[Hardware]** を選択し、**\[Build]** アイコンをクリックしても実行できます。

5. (オプション) ハードウェア エミュレーション用のベクター加算アプリケーションをビルドします。

   - **vadd\_system** プロジェクトを選択します。
   - ツールバーの **\[Build]** (ハンマー アイコン) のドロップダウンをクリックし、**\[Emulation-HW]** を選択します。この手順は、**\[Active Build Configuration]** に **\[Emulation-HW]** を選択し、**\[Build]** アイコンをクリックしても実行できます。
   - プロジェクトをクリーンアップするかどうか尋ねるダイアログ ボックスが表示されたら、**\[Don't clean]** を選択します。

#### ハードウェアでのアプリケーションのテスト

1. ローカルに `vadd_system/Hardware/package/sd_card.img` をコピーします。

2. sd\_card.img を SD カードにプログラムします。

3. SD カードを挿入し、SD ブートモード (SW6\[4:1]=0001) で VCK190 を起動します。

4. UART コンソールへ接続します。

5. テスト アプリケーションを起動します。

   ```
   cd /mnt/sd-mmcblk1p1
   ./vadd binary_container_1.xclbin
   ```

6. UART コンソールに次のように表示されます。

<details>
  <summary><b>ログを表示</b></summary>

```
root@petalinux:/mnt/sd-mmcblk1p1# ./vadd binary_container_1.xclbin
[   34.747622] [drm] Pid 770 opened device
[   34.751501] [drm] Pid 770 closed device
[   34.759710] [drm] Pid 770 opened device
[   34.763568] [drm] Pid 770 closed device
[   34.767554] [drm] Pid 770 opened device
Loading: 'binary_container_1.xclbin'
[   35.023095] [drm] get section AIE_METADATA err: -22
[   35.023119] [drm] zocl_xclbin_read_axlf 1ec78909-b5e7-4db2-9fe9-22fd362b09a4 ret: 0
[   35.029555] [drm] bitstream 1ec78909-b5e7-4db2-9fe9-22fd362b09a4 locked, ref=1
[   35.037397] [drm] No ERT scheduler on MPSoC, using KDS
[   35.049806] [drm] 9 non-zero interrupt-id CUs out of 10 CUs
[   35.049852] [drm] scheduler config ert(0)
[   35.055426] [drm]   cus(1)
[   35.059435] [drm]   slots(16)
[   35.062132] [drm]   num_cu_masks(1)
[   35.065095] [drm]   cu_shift(16)
[   35.068578] [drm]   cu_base(0xa4010000)
[   35.071799] [drm]   polling(0)
[   35.075658] [drm] bitstream 1ec78909-b5e7-4db2-9fe9-22fd362b09a4 unlocked, ref=0
TEST PASSED
[   35.079775] [drm] bitstream 1ec78909-b5e7-4db2-9fe9-22fd362b09a4 locked, ref=1
[   35.099312] [drm] bitstream 1ec78909-b5e7-4db2-9fe9-22fd362b09a4 unlocked, ref=0
[   35.116279] [drm] Pid 770 closed device
```

</details>

**注記**: `[ 35.116279] [drm]` メッセージは、XRT からデバッグ目的に表示されます。表示されるのは UART の場合のみです。これは ssh の場合も表示されるようになりました。このデバッグ メッセージは、system dmesg のレポート レベルを下げるとオフにできます。

```
echo "4" > /proc/sys/kernel/printk
```

#### (オプション) ハードウェア エミュレーションでのアプリケーションのテスト

1. PS 用のエミュレーターを起動します。

   - \[Xilinx] → \[Start/Stop Emulator] をクリックします。
   - \[Project] は plaie\_system、\[Configuration] は Emulation-HW にします。

   ![Launch Emulator for Vadd Project](images/step4/vitis_emulation_vadd.png)

   - \[Start] をクリックします。
   - \[Emulation Console] に表示さあれます。
   - Linux が起動するまで待ちます。Linux の起動が正しく検出されると、待機ウィンドウは表示されなくなります。

2. PL エミュレーションを起動します。

   - **\[add\_system]** を右クリックして **\[Run as]** → **\[Run Configurations]** をクリックします。
   - **vadd\_system -Launch** を選択します
   - **\[Build Configuration]** を **\[Emulation-HW]** に変更します。
   - **\[Remote Working Directory]** を **/mnt/sd-mmcblk1p1** に変更します。

   ![Vitis Emulation Vadd コンフィギュレーション](images/step4/vitis_emulation_vadd_configuration.png)

   - \[Apply] をクリックします。
   - \[Run] をクリックします。
   - 実行結果を確認します。

   ![Vitis Emulation Vadd の結果](images/step4/vitis_emulation_vadd_result.png)

3. エミュレーターを停止します。

   - \[Xilinx] → \[Start/Stop Emulator] をクリックします。
   - \[Stop] ボタンをクリックします。

**ここまでの結果**

Vitis は、XSIM の QEMU、PL、AIE エミュレーションで PS エミュレーションを実行します。これらは相互に通信できます。エミュレーションを実行すると、Vitis は実行ファイルと xclbin を \[Remote Working Directory] で指定したディレクトリにダウンロードして、起動します。

### テスト 3: AIE ＋ PL アクセラレーション アプリケーション プロジェクトの実行

プラットフォームの機能を検証するために、AIE + PL カーネルと PS アプリケーションを含むプロジェクトを作成し、VCK190 ボード上で実行します。

1. カスタム プラットフォームでベクター加算アプリケーションを作成します。

   - **\[File] → \[New] → \[Application Project]** をクリックします。
   - \[Welcome] ページで **\[Next]** をクリックします。
   - \[Platform] ページで、リストから **\[vck190\_custom]** プラットフォームを選択します。**\[Next]** をクリックします。
   - アプリケーション名を **plaie** に、ターゲット プロセッサを **psv\_cortexa72\_SMP** に設定します。**\[Next]** をクリックします。
   - sysroot パス、rootfs パス、およびカーネル イメージを入力します。**\[Next]** をクリックします。
   - **\[AI Engine System Design Examples]** → **\[AI Engine, PL and PS System Design]** テンプレートを選択します。**\[Finish]** をクリックします。

2. ハードウェア用のベクター加算アプリケーションをビルドします。

   - **plaie\_system** プロジェクトを選択します。
   - ツールバーの **\[Build]** (ハンマー アイコン) のドロップダウンをクリックし、**\[Hardware]** を選択します。この手順は、**\[Active Build Configuration]** に **\[Hardware]** を選択し、**\[Build]** アイコンをクリックしても実行できます。

3. (オプション) ハードウェア エミュレーション用のベクター加算アプリケーションをビルドします。

   - **plaie\_system** プロジェクトを選択します。
   - ツールバーの **\[Build]** (ハンマー アイコン) のドロップダウンをクリックし、**\[Emulation-HW]** を選択します。この手順は、**\[Active Build Configuration]** に **\[Emulation-HW]** を選択し、**\[Build]** アイコンをクリックしても実行できます。
   - プロジェクトをクリーンアップするかどうか尋ねるダイアログ ボックスが表示されたら、**\[Don't clean]** を選択します。

4. ビルドが正常に完了すると、**sd\_card.img** が `plaie_system/Hardware/package` および `plaie_system/Hardware/package_no_aie_debug` ディレクトリに生成されます。これには、1 つ目の FAT32 パーティションの sd\_card ディレクトリにあるすべてのファイルと、2 つ目の EXT4 パーティションの rootfs.ext4 の内容が含まれます。相違点は、**package** ディレクトリの **sd\_card.img** には `enable_aie_debug=1` パッケージ パラメーターがあることです。これはデバッグに使用されます。**package\_no\_aie\_debug** のものはフリーランニングに使用できます。

#### ハードウェアでのアプリケーションのテスト

1. プロジェクトがサーバーまたは仮想マシンで実行される場合は、plaie\_system/Hardware/package\_no\_aie\_debug ディレクトリから **sd\_card.img** をローカルにコピーします。

2. **sd\_card.img** を SD カードにプログラムします。

3. SD カードを挿入し、SD ブートモード (SW6\[4:1]=0001) で VCK190 ボードをブートします。

4. XRT ランタイム環境を設定し、テスト アプリケーションを開始します。

   ```
   cd /mnt/sd-mmcblk1p1
   ./plaie binary_container_1.xclbin
   ```

5. UART コンソールに次のように表示されます。

<details>
  <summary><b>ログを表示</b></summary>

```
root@petalinux:/mnt/sd-mmcblk1p1# ./plaie binary_container_1.xclbin
[  381.642589] [drm] Pid 693 opened device
[  381.646455] [drm] Pid 693 closed device
[  381.654748] [drm] Pid 693 opened device
[  381.658589] [drm] Pid 693 closed device
[  381.662601] [drm] Pid 693 opened device
Loading: 'binary_container_1.xclbin'
[  381.928588] [drm] zocl_xclbin_read_axlf 8ff25a1d-3722-4718-bae4-e65ef3313a0f ret: 0
[  381.934195] [drm] bitstream 8ff25a1d-3722-4718-bae4-e65ef3313a0f locked, ref=1
[  381.941892] [drm] No ERT scheduler on MPSoC, using KDS
[  381.954244] [drm] Interrupt is not enabled for at least one kernel. Fall back to polling mode.
[  381.954419] [drm] 12 non-zero interrupt-id CUs out of 13 CUs
[  381.963051] [drm] CU ffffff02 is free-running.
[  381.968711] [drm] scheduler config ert(0)
[  381.973149] [drm]   cus(3)
[  381.977152] [drm]   slots(16)
[  381.979853] [drm]   num_cu_masks(1)
[  381.982813] [drm]   cu_shift(16)
[  381.986292] [drm]   cu_base(0xa4010000)
[  381.989515] [drm]   polling(1)
[  381.993375] [drm] bitstream 8ff25a1d-3722-4718-bae4-e65ef3313a0f unlocked, ref=0
TEST PASSED
[  381.998058] [drm] bitstream 8ff25a1d-3722-4718-bae4-e65ef3313a0f locked, ref=1
[  382.022624] [drm] bitstream 8ff25a1d-3722-4718-bae4-e65ef3313a0f unlocked, ref=0
[  382.045158] [drm] Pid 693 closed device
```

</details>

#### (オプション) ハードウェア エミュレーションでのアプリケーションのテスト

1. PS 用のエミュレーターを起動します。

   - \[Xilinx] → \[Start/Stop Emulator] をクリックします。
   - \[Project] は plaie\_system、\[Configuration] は Emulation-HW にします。
   - \[Start] をクリックします。
   - \[Emulation Console] に表示さあれます。
   - Linux が起動するまで待ちます。Linux の起動が正しく検出されると、待機ウィンドウは表示されなくなります。

2. PL および AIE エミュレーションを起動します。

   - \[plaie\_system] を右クリックして \[Run as] → \[Run Configurations] をクリックします。
   - plaie\_system-Launch を選択します。
   - **\[Remote Working Directory]** を **/mnt/sd-mmcblk1p1** に変更します。
   - \[Apply] をクリックします。
   - \[Run] をクリックします。

   ![PLAIE プロジェクトのエミュレーション結果](images/step4/vitis_emulation_plaie_result.png)

3. エミュレーターを停止します。

   - **\[Xilinx]** → **\[Start/Stop Emulator]** をクリックします。
   - **\[Stop]** ボタンをクリックします。

これで、PL アクセラレーションカーネルと AIE アプリケーションが動作するカスタム プラットフォームが作成できました。

[次の章](./faq.md)では、よく発生する問題とよくある質問について説明します。プラットフォーム作成の詳細は、そちらをご覧ください。

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
