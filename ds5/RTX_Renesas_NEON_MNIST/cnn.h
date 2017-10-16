/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.

 Simple CNN Application for Inference only
==================================================================
*/
// RW_DATAの後半に配置(0x20650000　to 0x206FFFFF)
#define NN_BUFFER	0x20650000

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
//  biases  0x20650000 - 0x20650040 (size 0x40)
//  weights 0x20650040 - 0x20650680 (size 0x640)
#define KERASLAYER0_BIASES NN_BUFFER
#define KERASLAYER0_WEIGHTS (KERASLAYER0_BIASES + 0x40)
// keras_lay[2]
// biases{Array[32]}, weights{Array[5][5][16][32]}
//  biases  0x20650680 - 0x20650700 (size 0x80)
//  weights 0x20650700 - 0x2065CF00 (size 0xC800)
#define KERASLAYER2_BIASES (KERASLAYER0_WEIGHTS + 0x640)
#define KERASLAYER2_WEIGHTS (KERASLAYER2_BIASES + 0x80)
// keras_lay[6]
// biases{Array[128]}, weights{Array[512][128]}
//  biases  0x2065CF00 - 0x2065D100 (size 0x200)
//  weights 0x2065D100 - 0x2069D100 (size 0x40000)
#define KERASLAYER6_BIASES (KERASLAYER2_WEIGHTS + 0xC800)
#define KERASLAYER6_WEIGHTS (KERASLAYER6_BIASES + 0x200)
// keras_lay[8]
// biases{Array[10]}, weights{Array[128][10]}
//  biases  0x2069D100 - 0x2069D128 (size 0x28)
//  weights 0x2069D128 - 0x2069E528 (size 0x1400)
#define KERASLAYER8_BIASES (KERASLAYER6_WEIGHTS + 0x40000)
#define KERASLAYER8_WEIGHTS (KERASLAYER8_BIASES + 0x28)

//--- 層データ格納先アドレス ---
// ※各層向けに独立した領域を確保しているがシーケンシャルな処理であるため本来は使いまわしても構わない
// 入力層
// サイズ(Channel:1, Figure rows:28, Figure columns:28)
#define INPUTLAYER (KERASLAYER8_WEIGHTS + 0x1400)	// 0x206A0000 - 0x206A0C40 (size 0xC40)
// 隠れ層1 (畳み込み1)
// サイズ(Figure rows:24, Figure columns:24, Channel:16)
#define HIDDENLAYER1 (INPUTLAYER + 0xC40)	// 0x206A1000 - 0x206AA000 (size 0x9000)
// 隠れ層2 (最大プーリング1)
// サイズ(Figure rows:12, Figure columns:12, Channel:16)
#define HIDDENLAYER2 (HIDDENLAYER1 + 0x9000)	// 0x206AB000 - 0x206AD400 (size 0x2400)
// 隠れ層3 (畳み込み2)
// サイズ(Figure rows:8, Figure columns:8, Channel:32)
#define HIDDENLAYER3 (HIDDENLAYER2 + 0x2400)	// 0x206AE000 - 0x206B0000 (size 0x2000)
// 隠れ層4 (最大プーリング2)
// サイズ(Figure rows:4, Figure columns:4, Channel:32)
#define HIDDENLAYER4 (HIDDENLAYER3 + 0x2000)	// 0x206B0000 - 0x206B0800 (size 0x800)
// 隠れ層5 (全結合1/入力は隠れ層4を平滑化したChannel:512)
// サイズ(Channel:128)
#define HIDDENLAYER5 (HIDDENLAYER4 + 0x800)	// 0x206B1000 - 0x206B1200 (size 0x200)
// 出力層 (全結合2)
// サイズ(Channel:10)
#define OUTPUTLAYER (HIDDENLAYER5 + 0x200)	// 0x206B1300 - 0x206B1328 (size 0x28)

// テスト画像サイズ
#define IMAGE_ROWS		28
#define IMAGE_COLUMNS	28
#define TESTDATA 0x206C0000		// 0x206C0000 - 0x206C0C40 (size 0xC40)

int mnist_cnn_eval(
		unsigned int *test,			// 入力：推論イメージ test[IMAGE_ROWS][IMAGE_COLUMNS]
		unsigned int *result		// 出力：推論結果
);
