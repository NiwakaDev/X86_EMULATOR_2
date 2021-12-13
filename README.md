# X86_EMULATOR_2


<p>はりぼてOS</p>

![demo](https://user-images.githubusercontent.com/61189782/144729289-fe16a09d-d5dd-4b7d-8583-3b19e9381b3d.gif)

![スクリーンショット 2021-12-01 8.35.34.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/373549/39b0e3eb-4877-4f96-b63b-30673e38221f.png)

![富士山](https://user-images.githubusercontent.com/61189782/143998297-d17dd067-b59a-4762-8bde-caebc8ca174b.png)

<p>FreeDOS(REAL MODE)</p>

![FreeDOS](https://user-images.githubusercontent.com/61189782/145657637-3331b9cf-021e-4a1e-867c-b1b49b31a852.png)

<p>osz(https://github.com/neri/osz)</p>

![スクリーンショット 2021-12-12 21 33 31](https://user-images.githubusercontent.com/61189782/145712414-eed58221-06ef-42d8-9c6f-28fad542ae25.png)

<h2>1.ビルド</h2>

```
make
```

<h2>2.実行</h2>


はりぼてOSを起動
```
./x86 -i haribote.img
```

FreeDOSを起動
```
./x86 -i fd.img
```

oszを起動
```
./x86 -i full.img
```

<h2>3.言語とライブラリ</h2>
C++11, SDL2.0

<h2>4.マウス使用方法</h2>

<h4>4-1.マウスをエミュレータに取り込みたい時</h4>
エミュレータ上でマウスをクリック

<h4>4-2.エミュレータに取り込んだマウスを外に出したい時</h4>

```
left-ctrl+left-alt+g
```
と押す。
※left-ctrl+left-altを先に押してから、gを押してください。

<h2>5.エミュレータで動かせるOS</h2>
<h4><a href="https://www.amazon.co.jp/30%E6%97%A5%E3%81%A7%E3%81%A7%E3%81%8D%E3%82%8B-OS%E8%87%AA%E4%BD%9C%E5%85%A5%E9%96%80-%E5%B7%9D%E5%90%88-%E7%A7%80%E5%AE%9F/dp/4839919844/ref=pd_vtp_5/356-0188609-1667566?pd_rd_w=qrIxz&pf_rd_p=949e26f5-c2ef-4c96-bfde-49d7614d0317&pf_rd_r=FQQA4209JZRMHNABD541&pd_rd_r=2bdba9dc-9d7d-401a-b66c-053b098be917&pd_rd_wg=dpohq&pd_rd_i=4839919844&psc=1">30日でできる! OS自作入門</a></h4>
<h4>FreeDOS(REAL MODE)</h4>

<h4>osz(https://github.com/neri/osz)</h4>

<h2>参考</h2>

<h4>リアルモードの参考</h4>
https://bitbucket.org/7shi/8086run/
を参考にしました。

<h4>BIOSで使用するFONTについて</h4>
https://github.com/dhepper/font8x8
から拝借しました。ありがとうございます。

<h2>ビルドしても動かない場合</h2>
連絡をくれるとありがたいです。
