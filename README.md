# ds5_keras_mnist
Keras MNIST to ARM Cortex-A9 Baremetal porting using DS-5 - CQ ARM MCU Workshop 2017 
---
本プロジェクトは2017/7/28に開催されたCQ出版社主催のARMマイコン・ワークショップ2017にてセッション A-05 組み込みでディープラーニングの発表で用いたサンプルプログラムです。  
  
---
## ds5_keras_mnist/jupyter
Keras MNIST CNN example (François Chollet)をベースにネットワークサイズ削減のため畳み込み層のフィルターサイズとチャンネル数を調整しています。
<https://github.com/fchollet/keras/blob/master/examples/mnist_cnn.py>
### CQseminar_MNIST_CNN.ipynb
ニューラルネットワークのトレーニングを行いパラメータ（重み、バイアス）のArrayをjsonでファイルに保存します。
### CQseminar_MNIST_CNN_2.ipynb
ニューラルネットワークの各層の出力Arrayの構成を理解するためにArrayをprintで表示します。行列の演算では要素の並びが異なると計算結果が一致しません。
### 環境
* ホストPC : Windows 64bit
* Anaconda
* Tensorflow
* numpy, scipy
* Keras
* matplotlib
* pillow
* h5py

---
## ds5_keras_mnist/ds5/TrustZone_with_NEON
`ds5_keras_mnist/jupyter/CQseminar_MNIST_CNN.ipynb`で出力した学習済みニューラルネットワークのパラメータファイルをARM Cortex-A9ベアメタルターゲットに読み込みMNISTの推論を行います。  
学習済みニューラルネットワークと推論処理はTrustZoneのSecure worldに配置されるため、コストをかけて学習したニューラルネットワークを保護することができます。
### 環境
* ホストPC : Windows 64bit
* DS-5 Version 5.27.1  
<https://developer.arm.com/products/software-development-tools/ds-5-development-studio/downloads>

### Exampleプロジェクト本体
本プロジェクトファイルはDS-5のExampleである`TrustZone Example for Cortex-A9 and ARM Compiler 5 - ARM®DS-5™`をベースにMNIST CNNの処理を追加したものです。その他の変更点としてARM Compilerによるループの自動並列化のためにデバイス初期化コードでNEONの有効化を行っています。  
### 学習済みパラメータ
Kerasから出力されたパラメータファイルはサイズが大きいため組み込みデバイスで利用するにはストレージなどの大容量メモリが必要になります。本サンプルではニューラルネットワークの処理に集中するためDS-5のJythonデバッガスクリプトを利用したスタブを構築しています。  

* Kerasから出力したパラメータファイル  
`ds5_keras_mnist/ds5/TrustZone_with_NEON/scripts/mnist_cnn_train121_params_layer0.json`
`ds5_keras_mnist/ds5/TrustZone_with_NEON/scripts/mnist_cnn_train121_params_layer2.json`
`ds5_keras_mnist/ds5/TrustZone_with_NEON/scripts/mnist_cnn_train121_params_layer6.json`
`ds5_keras_mnist/ds5/TrustZone_with_NEON/scripts/mnist_cnn_train121_params_layer8.json`
* デバイスのメモリへ展開するスクリプト  
`ds5_keras_mnist/ds5/TrustZone_with_NEON/scripts/cnn_import.py`  
パラメータをデバイスのメモリへ展開した後、ds5_params.binにバイナリファイルとして保存します。同名のファイルが既に存在する場合は保存に失敗しますので、あらかじめ削除しておいてください。  
* デバイスのメモリへ展開されたパラメータを保存したバイナリファイル  
`ds5_keras_mnist/ds5/TrustZone_with_NEON/scripts/ds5_params.bin`  
デバッガ接続時に下記コマンドで一括展開されます。  
`restore "${workspace_loc:/TrustZone_with_NEON/scripts/ds5_params.bin}" binary S:0x80300000`
### 推論対象の入力画像
推論対象となる手書き数字の画像を組み込みデバイスに入力するには一般的にタッチパネル付きLCDなどのペリフェラルが必要になります。本サンプルではニューラルネットワークの処理に集中するためDS-5のJythonデバッガスクリプトを利用したスタブを構築しています。  

* JPEGファイル  
`ds5_keras_mnist/ds5/TrustZone_with_NEON/scripts/test.jpg`
* JPEGファイルをメモリに展開するスクリプト  
デバッガ接続時に呼び出されます。  
`ds5_keras_mnist/ds5/TrustZone_with_NEON/scripts/image_import.py`


---
## ご注意
* 本プロジェクトは組み込みデバイスで畳み込みニューラルネットワーク実装の実験を目的としています。十分な検証、例外処理は行われていません。また、処理速度、メモリサイズ共にソースコードレベルの最適化は行われていません。
* 本プロジェクトに何らかの不具合が発生したとしても修正の義務を負いません。
* 本プロジェクトの利用により何らかの損害が発生したとしても一切の責任を負いません。
