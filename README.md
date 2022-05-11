# X86_EMULATOR_2


<p>X86_EMULATOR_2 is a x86 emulator written in C++ .</p>

<p><a href="https://www.amazon.co.jp/30%E6%97%A5%E3%81%A7%E3%81%A7%E3%81%8D%E3%82%8B-OS%E8%87%AA%E4%BD%9C%E5%85%A5%E9%96%80-%E5%B7%9D%E5%90%88-%E7%A7%80%E5%AE%9F-ebook/dp/B00IR1HYI0">Haribote OS</a></p>

![demo](https://user-images.githubusercontent.com/61189782/144729289-fe16a09d-d5dd-4b7d-8583-3b19e9381b3d.gif)

![スクリーンショット 2021-12-01 8.35.34.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/373549/39b0e3eb-4877-4f96-b63b-30673e38221f.png)

![富士山](https://user-images.githubusercontent.com/61189782/143998297-d17dd067-b59a-4762-8bde-caebc8ca174b.png)

<p>FreeDOS(REAL MODE)</p>

![FreeDOS](https://user-images.githubusercontent.com/61189782/145657637-3331b9cf-021e-4a1e-867c-b1b49b31a852.png)

<p><a href="https://github.com/neri/osz">osz</a></p>

![スクリーンショット 2021-12-12 21 33 31](https://user-images.githubusercontent.com/61189782/145712414-eed58221-06ef-42d8-9c6f-28fad542ae25.png)

<h2><a href="https://github.com/barotto/test386.asm">test386</a></h2>
 <table border="1">
    <tr>
      <th>diagnostic code</th>
      <th>Status</th>
    </tr>
    <tr>
      <td>0x01(Conditional jumps and loops)</td>
      <td>passed</td>
    </tr>
    <tr>
      <td>0x02(Quick tests of unsigned 32-bit multiplication and division)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x03(Move segment registers in real mode)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x04(Store, move, scan, and compare string data in real mode)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x05(Calls in real mode)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x06(Load full pointer in real mode)</td>
      <td>passed</td>
    </tr>
    <tr>
      <td>0x08(GDT, LDT, PDT, and PT setup, enter protected mode)</td>
      <td>passed</td>
    </tr>
    <tr>
      <td>0x09(Stack functionality)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x0A(Test user mode (ring 3) switching)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x0B(Moving segment registers)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x0A(Test user mode (ring 3) switching)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x0C(Zero and sign-extension)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x0D(16-bit addressing modes (LEA))</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x0E(32-bit addressing modes (LEA))</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x0F(Access memory using various addressing modes)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x10(Store, move, scan, and compare string data in protected mode)</td>
      <td>passed</td>
    </tr>
     <tr>
      <td>0x11(Page faults and PTE bits)</td>
      <td>failed</td>
    </tr>

  </table>
  
<h2>Build</h2>

```
make
```

<h2>Run</h2>


Run Haribote OS
```
./x86 -i haribote.img
```

Run FreeDOS
```
./x86 -i fd.img
```

Run <a href="https://github.com/neri/osz">osz</a>
```
./x86 -i full.img
```

<h2>Language and Library</h2>
C++14, <a href="https://www.libsdl.org/">SDL2</a>

<h2>How to use mouse</h2>

<h3>Mouse into the emulator</h3>

```
Click mouse on the emulator 
```

<h3>Mouse out of the emulator</h3>

```
left-ctrl+left-alt+g
```

※Press left-ctrl+left-alt first, then g .

<h2>Test</h2>

```
./test.sh
```

or

```
make clean
make x86_test
./x86_test
```


<h2>Reference</h2>

<h4>Real Mode</h4>
<a href="https://bitbucket.org/7shi/8086run/">https://bitbucket.org/7shi/8086run/</a>

<h4>FONT</h4>
<a href="https://github.com/dhepper/font8x8">https://github.com/dhepper/font8x8</a>

<h2>Contact</h2>
Feel free to contact me .

<h2>フロッピーディスクの音源について</h2>
<strong>※これは廃止になりました。</strong>

<p>OtoLogicの素材を使用(https://otologic.jp/)</p>
<p>フロッピーディスクの音源ファイル:Floppy_Disk_Drive02-1(Operating_Noise).mp3</p>
