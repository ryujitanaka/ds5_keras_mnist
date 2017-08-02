/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.

 Simple CNN Application for Inference only
==================================================================
*/
// スキャッタロードファイルでSECURE_BUFFERとして確保済みの領域(0x80300000　to 0x804FFFFF)
#define SECURE_BUFFER	0x80300000

// keras_lay[0]
// 入力(Channel:1, Figure rows:28, Figure columns:28)
// 出力(Channel:16, Figure rows:24, Figure columns:24)
// 畳み込み1(Activation:ReLU, Channel:16, Filter rows:5, Filter columns:5)
//  層間パラメータ: 重み(Array[rows:5][columns:5][input_channel:1][output_channel:16]), バイアス(Array[output_channel:16])
//
// keras_lay[1]
// 入力(Channel:16, Figure rows:24, Figure columns:24)
// 出力(Channel:16, Figure rows:12, Figure columns:12)
// 最大プーリング1(Channel:16, Figure rows:12, Figure columns:12)
//  層間パラメータ: なし
//
// keras_lay[2]
// 入力(Channel:16, Figure rows:12, Figure columns:12)
// 出力(Channel:32, Figure rows:8, Figure columns:8)
// 畳み込み2(Activation:ReLU, Channel:32, Filter rows:5, Filter columns:5)
//  層間パラメータ: 重み(Array[rows:5][columns:5][input_channel:16][output_channel:32]), バイアス(Array[output_channel:32])
//
// keras_lay[3]
// 入力(Channel:32, Figure rows:8, Figure columns:8)
// 出力(Channel:32, Figure rows:4, Figure columns:4)
// 最大プーリング2(Channel:32, Figure rows:4, Figure columns:4)
//  層間パラメータ: なし
//
// keras_lay[4]
// ドロップアウト(Dropout rate:0.25)
// データフォーマット変化なし
//  層間パラメータ: なし
//
// keras_lay[5]
// 入力(Channel:32, Figure rows:4, Figure columns:4)
// 出力(Channel:512)
// 平滑化(Channel:512)
//  層間パラメータ: なし
//
// keras_lay[6]
// 入力(Channel:512)
// 出力(Channel:128)
// 全結合1(Activation:ReLU, Channel:128)
//  層間パラメータ: 重み(Array[input_channel:512][output_channel:128]), バイアス(Array[output_channel:128])
//
// keras_lay[7]
// ドロップアウト(Dropout rate:0.5, Channel:128)
// データフォーマット変化なし
//  層間パラメータ: なし
//
// keras_lay[8]
// 入力(Channel:128)
// 出力(Channel:10)
// 全結合2(Activation:Softmax, Channel:10)
//  層間パラメータ: 重み(Array[input_channel:128][output_channel:10]), バイアス(Array[output_channel:10])
//

//--- 層間パラメータ格納先アドレス ---
// keras_lay[0]
// biases{Array[16]}, weights{Array[5][5][1][16]}
//  biases  S:0x80300000 - S:0x80300040
//  weights S:0x80300040 - S:0x80300680
#define KERASLAYER0_BIASES 0x80300000
#define KERASLAYER0_WEIGHTS 0x80300040
// keras_lay[2]
// biases{Array[32]}, weights{Array[5][5][16][32]}
//  biases  S:0x80300680 - S:0x80300700
//  weights S:0x80300700 - S:0x8030cf00
#define KERASLAYER2_BIASES 0x80300680
#define KERASLAYER2_WEIGHTS 0x80300700
// keras_lay[6]
// biases{Array[128]}, weights{Array[512][128]}
//  biases  S:0x8030cf00 - S:0x8030d100
//  weights S:0x8030d100 - S:0x8034d100
#define KERASLAYER6_BIASES 0x8030cf00
#define KERASLAYER6_WEIGHTS 0x8030d100
// keras_lay[8]
// biases{Array[10]}, weights{Array[128][10]}
//  biases  S:0x8034d100 - S:0x8034d128
//  weights S:0x8034d128 - S:0x8034e528
#define KERASLAYER8_BIASES 0x8034d100
#define KERASLAYER8_WEIGHTS 0x8034d128

//--- 層データ格納先アドレス ---
// ※各層向けに独立した領域を確保しているがシーケンシャルな処理であるため本来は使いまわしても構わない
// 入力層
// サイズ(Channel:1, Figure rows:28, Figure columns:28)
#define INPUTLAYER 0x80350000	// 0x80350000 to 0x80350C40 (size 0xC40)
// 隠れ層1 (畳み込み1)
// サイズ(Figure rows:24, Figure columns:24, Channel:16)
#define HIDDENLAYER1 0x80351000	// 0x80351000 to 0x8035A000 (size 0x9000)
// 隠れ層2 (最大プーリング1)
// サイズ(Figure rows:12, Figure columns:12, Channel:16)
#define HIDDENLAYER2 0x8035B000	// 0x8035B000 to 0x8035D400 (size 0x2400)
// 隠れ層3 (畳み込み2)
// サイズ(Figure rows:8, Figure columns:8, Channel:32)
#define HIDDENLAYER3 0x8035E000	// 0x8035E000 to 0x80360000 (size 0x2000)
// 隠れ層4 (最大プーリング2)
// サイズ(Figure rows:4, Figure columns:4, Channel:32)
#define HIDDENLAYER4 0x80360000	// 0x80360000 to 0x80360800 (size 0x800)
// 隠れ層5 (全結合1/入力は隠れ層4を平滑化したChannel:512)
// サイズ(Channel:128)
#define HIDDENLAYER5 0x80361000	// 0x80361000 to 0x80361200 (size 0x200)
// 出力層 (全結合2)
// サイズ(Channel:10)
#define OUTPUTLAYER 0x80361300	// 0x80361300 to 0x80361328 (size 0x28)

// テスト画像サイズ
#define IMAGE_ROWS		28
#define IMAGE_COLUMNS	28
#define TESTDATA 0x80370000		// 0x80370000 to 0x80370C40 (size 0xC40)

int mnist_cnn_eval(
		unsigned int *test,			// 入力：推論イメージ test[IMAGE_ROWS][IMAGE_COLUMNS]
		unsigned int *result		// 出力：推論結果
);
