/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.

 Simple CNN Application for Prediction only
==================================================================
*/
#include "cnn.h"

// MNIST image[image_num][IMAGE_ROWS][IMAGE_COLUMNS]
typedef struct {
	unsigned int input_channel, input_rows, input_columns;
	unsigned int filter_rows, filter_columns;
	unsigned int output_channel, output_rows, output_columns;
	char relu_activation;
} layer_structure;

//--- 必要な処理リスト ---
// 前処理(入力正規化)
// 畳み込み
// 最大プーリング
// 全結合
// 後処理(最大スコアの要素取得)
// ReLU活性化関数

//--- 推論のみなので不要な処理リスト ---
// model compile時のoptimizer=keras.optimizers.Adadelta()は学習用の最適化手法なので不要
// Dropoutは過学習抑制のためであり学習を行わないのであれば不要
// Flattenはオフセット計算だけなので関数化の必要なし
// MNISTでは１つの正解を導き出せばよいため直前の全結合層のスコアを求めるだけでSoftmaxによる正規化は不要（推論の自信を返す必要がある場合は後処理で行う）

//--- ReLU活性化関数 ---
float relu(float value)
{
	if (value < 0.0f) {
		return 0.0f;
	}
	return value;
}

//--- 畳み込み ---
//
// keras_lay[0]
// 入力(Figure rows:28, Figure columns:28, Channel:1)
// 出力(Figure rows:24, Figure columns:24, Channel:16)
// 畳み込み1(Activation:ReLU, Filter rows:5, Filter columns:5, Channel:16)
//  層間パラメータ: 重み(Array[rows:5][columns:5][input_channel:1][output_channel:16]), バイアス(Array[output_channel:16])
//
// keras_lay[2]
// 入力(Figure rows:12, Figure columns:12, Channel:16)
// 出力(Figure rows:8, Figure columns:8, Channel:32)
// 畳み込み2(Activation:ReLU, Filter rows:5, Filter columns:5, Channel:32)
//  層間パラメータ: 重み(Array[rows:5][columns:5][input_channel:16][output_channel:32]), バイアス(Array[output_channnel:32])
//
// Keras Conv2Dで明示的に指定していないデフォルト値(https://keras.io/ja/layers/convolutional/)
//	strides=(1, 1)  : ストライド1
//	padding='valid' : パディングしない(画像端の特徴には弱いがMNISTであれば大丈夫)
//
int convolution(
		layer_structure *lay,
		float *inputs,	// 入力配列 inputs[lay->input_rows][lay->input_columns][lay->input_channel]
		float *outputs,	// 出力配列 outputs[lay->output_rows][lay->output_columns][lay->output_channel]
		float *weights,	// 重み配列 weights[lay->filter_rows][lay->filter_columns][lay->input_channel][lay->output_channel]
		float *biases	// バイアス配列 biases[lay->output_channnel]
) {
	unsigned int out_ch;		// 出力チャンネルインデックス
	unsigned int in_ch;			// 入力チャンネルインデックス
	unsigned int stride_row;	// ストライド行インデックス
	unsigned int stride_col;	// ストライド列インデックス
	unsigned int filter_row;	// フィルター行インデックス
	unsigned int filter_col;	// フィルター列インデックス
	float current_weight;		// 現在の重み値
	float current_biase;		// 現在のバイアス値
	float current_input;		// 現在の入力値
	float current_result;		// 現在の計算結果
	float kernel_result;		// カーネル単位の計算結果

	// 出力配列の初期化
	for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {	// ストライド行方向ループ
		for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {	// ストライド列方向ループ
			for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {	// 出力チャンネルループ
				((float*)outputs)[((stride_row * lay->output_columns * lay->output_channel)	// [x][ ][ ]
												+(stride_col * lay->output_channel)			// [ ][x][ ]
												+ out_ch)]                       			// [ ][ ][x]
				= 0.0f;
			}
		}
	}

	// 重みの演算
	for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {	// ストライド行方向ループ
		for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {	// ストライド列方向ループ
			for (filter_row = 0; filter_row < lay->filter_rows; filter_row++) {	// フィルター行方向ループ
				for (filter_col = 0; filter_col < lay->filter_columns; filter_col++) {	// フィルター列方向ループ
					for (in_ch = 0; in_ch < lay->input_channel; in_ch++) {	// 入力チャンネルループ
						// 現在の入力値取得
						// 入力チャンネルおよびストライド計算後の先頭アドレスは解決済み
						current_input = ((float*)inputs)[  ((stride_row + filter_row) * lay->input_columns * lay->input_channel)	// [x][ ][ ]
														 + ((stride_col + filter_col)                      * lay->input_channel)	// [ ][x][ ]
														 + in_ch];																	// [ ][ ][x]
						for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {	// 出力チャンネルループ
							// 現在の重みパラメータ取得
							current_weight = ((float*)weights)[  (filter_row * lay->filter_columns * lay->input_channel * lay->output_channel)	// [x][ ][ ][ ]
															   + (filter_col                       * lay->input_channel * lay->output_channel)	// [ ][x][ ][ ]
															   + (in_ch                                                 * lay->output_channel)	// [ ][ ][x][ ]
															   + out_ch];																		// [ ][ ][ ][x]
							// 入力 * 重み
							current_result = current_input * current_weight;
							// 出力先に加算
							((float*)outputs)[  (stride_row * lay->output_columns * lay->output_channel)	// [x][ ][ ]
									          + (stride_col                       * lay->output_channel)	// [ ][x][ ]
											  + out_ch]                       								// [ ][ ][x]
							+= current_result;
						}
					}

				}
			}
			// 出力
		}
	}

	// バイアスを加算
	for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {	// ストライド行方向ループ
		for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {	// ストライド列方向ループ
			for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {	// 出力チャンネルループ
				// 現在のバイアス値を取得
				current_biase = ((float*)biases)[out_ch];
				// 出力先にバイアスを加算
				((float*)outputs)[  (stride_row * lay->output_columns * lay->output_channel)	// [x][ ][ ]
						          + (stride_col                       * lay->output_channel)	// [ ][x][ ]
								  + out_ch]                       								// [ ][ ][x]
				+= current_biase;
				// 活性化関数
				if (lay->relu_activation == 1) {
					kernel_result = ((float*)outputs)[  (stride_row * lay->output_columns * lay->output_channel)	// [x][ ][ ]
													  + (stride_col                       * lay->output_channel)	// [ ][x][ ]
													  + out_ch];                       								// [ ][ ][x]
					((float*)outputs)[  (stride_row * lay->output_columns * lay->output_channel)	// [x][ ][ ]
							          + (stride_col                       * lay->output_channel)	// [ ][x][ ]
									  + out_ch]                       								// [ ][ ][x]
					= relu(kernel_result);
				}
			}
		}
	}

	return 0;
}

//--- MAXプーリング ---
//
// keras_lay[1]
// 入力(Figure rows:24, Figure columns:24, Channel:16)
// 出力(Figure rows:12, Figure columns:12, Channel:16)
// 最大プーリング1(Channel:16, Figure rows:12, Figure columns:12)
//  層間パラメータ: なし
//
// keras_lay[3]
// 入力(Figure rows:8, Figure columns:8, Channel:32)
// 出力(Figure rows:4, Figure columns:4, Channel:32)
// 最大プーリング2(Channel:32, Figure rows:4, Figure columns:4)
//  層間パラメータ: なし
//
int max_pooling(
		layer_structure *lay,
		float *inputs,	// 入力配列 inputs[lay->input_rows][lay->input_columns][lay->input_channel]
		float *outputs	// 出力配列 outputs[lay->output_rows][lay->output_columns][lay->output_channel]
) {
	unsigned int ch;			// チャンネルインデックス
	unsigned int input_row;		// 入力行インデックス
	unsigned int input_col;		// 入力列インデックス
	unsigned int output_row;	// 出力行インデックス
	unsigned int output_col;	// 出力列インデックス
	unsigned int filter_row;	// フィルター行インデックス
	unsigned int filter_col;	// フィルター列インデックス
	float current_max;			// 現在の最大値
	float current_value;		// 現在の値

	for (ch = 0; ch < lay->input_channel; ch++) {	// チャンネル
		output_row = 0;
		for (input_row = 0; input_row < lay->input_rows; input_row += lay->filter_rows) {	// 行 (フィルターサイズ刻み)
			output_col = 0;
			for (input_col = 0; input_col < lay->input_columns; input_col += lay->filter_columns) {	// 列 (フィルターサイズ刻み)

				// フィルター内の最大値取得
				for (filter_row = 0; filter_row < lay->filter_rows; filter_row++) {	// フィルター行
					for (filter_col = 0; filter_col < lay->filter_columns; filter_col++) {	// フィルター列
						// 現在の入力値取得
						current_value = ((float*)inputs)[((input_row + filter_row) * lay->input_columns * lay->input_channel)
														+ ((input_col + filter_col) * lay->input_channel)
														+ ch];
						if (filter_row == 0 && filter_col == 0) {	// 初回(負の値もあり得るため)
							current_max = current_value;
						}
						else if (current_max < current_value) {		// ２回目以降最大値更新
							current_max = current_value;
						}
					}
				}

				// 出力配列へ格納
				((float*)outputs)[(output_row * lay->output_columns * lay->output_channel)
								  + (output_col * lay->output_channel)
								  + ch] = current_max;

				output_col++;
			}
			output_row++;
		}
	}

	return 0;
}

//--- 全結合 ---
//
// keras_lay[6]
// 入力(Channel:512)
// 出力(Channel:128)
// 全結合(Activation:ReLU, Channel:128)
//  層間パラメータ: 重み(Array[input_channel:512][output_channel:128]), バイアス(Array[output_channel:128])
//
// keras_lay[8]
// 入力(Channel:128)
// 出力(Channel:10)
// 全結合(Activation:Softmax, Channel:10)
//  層間パラメータ: 重み(Array[input_channel:128][output_channel:10]), バイアス(Array[output_channel:10])
//
int fully_connected(
		layer_structure *lay,
		float *inputs,	// 入力配列 inputs[lay->input_channel]
		float *outputs,	// 出力配列 outputs[lay->output_channel]
		float *weights,	// 重み配列 weights[lay->filter_rows][lay->filter_columns]
		float *biases	// バイアス配列 biases[lay->output_channnel]
) {
	unsigned int o;				// 出力配列インデックス
	unsigned int i;				// 入力配列インデックス
	float current_weight;		// 現在の重み値
	float current_biase;		// 現在のバイアス値
	float current_input;		// 現在の入力値
	float current_out;			// 現在の出力値

	for (o = 0; o < lay->output_channel; o++) {	// 出力配列(keras_lay[6]=128, keras_lay[8]=10)
		// 現在のバイアス値取得
		current_biase = ((float*)biases)[o];
		current_out = 0.0f;
		for (i = 0; i < lay->input_channel; i++) {	// 入力配列(keras_lay[6]=512, keras_lay[8]=128)
			// 現在の入力値取得
			current_input = ((float*)inputs)[i];
			// 現在の重み値取得
			current_weight = ((float*)weights)[(i * lay->output_channel) + o];
			// 入力 * 重み
			current_out += (current_input * current_weight);
		}
		// 合計値 + バイアス
		current_out +=  current_biase;
		// 活性化関数
		if (lay->relu_activation == 1) {
			current_out = relu(current_out);
		}
		// 出力配列へ格納
		((float*)outputs)[o] = current_out;
	}

	return 0;
}

// 前処理
int pre_proc(
		unsigned int *test_images,	// test_images[IMAGE_ROWS][IMAGE_COLUMNS]
		float *outputs				// output[IMAGE_ROWS][IMAGE_COLUMNS]
) {
	unsigned int current_input;
	float normalized;
	unsigned int row, col;

	// 画像データの値を256階調グレースケールから0.0～1.0に正規化
	for (row = 0; row < IMAGE_ROWS; row++) {
		for (col = 0; col < IMAGE_COLUMNS; col++) {
			current_input = ((unsigned int*)test_images)[row * IMAGE_COLUMNS + col];
			normalized = (float)current_input / 255.0;
			((float*)outputs)[(row * IMAGE_COLUMNS + col)] = normalized;
		}
	}

	return 0;
}

// 後処理
int post_proc(
		float *outlay,			// 出力層 outlay[channel]
		unsigned int channel	// 出力層チャンネル数
) {
	unsigned int idx, idx_max;
	float conf, conf_max;

	// 出力層から最もスコアが高いインデックスを検索
	idx_max = 0;
	conf_max = 0.0f;
	for( idx = 0; idx < channel; idx++ ){
		conf = ((float*)outlay)[idx];
		if(conf_max < conf){
			idx_max = idx;
			conf_max = conf;
		}
	}

	return idx_max;
}

int mnist_cnn_eval(
		unsigned int *test_images,	// 入力：推論イメージ test_images[IMAGE_ROWS][IMAGE_COLUMNS]
		unsigned int *result		// 出力：推論結果
) {
	static layer_structure lay;

	// 前処理
	pre_proc(test_images, (float*)INPUTLAYER);

	// keras_lay[0]
	// 入力(Figure rows:28, Figure columns:28, Channel:1)
	// 出力(Figure rows:24, Figure columns:24, Channel:16)
	// 畳み込み1(Activation:ReLU, Filter rows:5, Filter columns:5, Channel:16)
	//  層間パラメータ: 重み(Array[rows:5][columns:5][input_channel:1][output_channel:16]), バイアス(Array[output_channel:16])
	lay.input_channel = 1;
	lay.input_rows = 28;
	lay.input_columns = 28;
	lay.filter_rows = 5;
	lay.filter_columns = 5;
	lay.output_channel = 16;
	lay.output_rows = 24;
	lay.output_columns = 24;
	lay.relu_activation = 1;	// Activation:ReLU
	convolution(&lay,
			(float*)INPUTLAYER,
			(float*)HIDDENLAYER1,
			(float*)KERASLAYER0_WEIGHTS,
			(float*)KERASLAYER0_BIASES);

	// keras_lay[1]
	// 入力(Figure rows:24, Figure columns:24, Channel:16)
	// 出力(Figure rows:12, Figure columns:12, Channel:16)
	// 最大プーリング1(Figure rows:12, Figure columns:12, Channel:16)
	//  層間パラメータ: なし
	lay.input_channel = 16;
	lay.input_rows = 24;
	lay.input_columns = 24;
	lay.filter_rows = 2;
	lay.filter_columns = 2;
	lay.output_channel = 16;
	lay.output_rows = 12;
	lay.output_columns = 12;
	lay.relu_activation = 0;
	max_pooling(&lay, (float*)HIDDENLAYER1, (float*)HIDDENLAYER2);

	// keras_lay[2]
	// 入力(Figure rows:12, Figure columns:12, Channel:16)
	// 出力(Figure rows:8, Figure columns:8, Channel:32)
	// 畳み込み2(Activation:ReLU, Filter rows:5, Filter columns:5, Channel:32)
	//  層間パラメータ: 重み(Array[rows:5][columns:5][input_channel:16][output_channel:32]), バイアス(Array[output_channnel:32])
	lay.input_channel = 16;
	lay.input_rows = 12;
	lay.input_columns = 12;
	lay.filter_rows = 5;
	lay.filter_columns = 5;
	lay.output_channel = 32;
	lay.output_rows = 8;
	lay.output_columns = 8;
	lay.relu_activation = 1;	// Activation:ReLU
	convolution(&lay,
			(float*)HIDDENLAYER2,
			(float*)HIDDENLAYER3,
			(float*)KERASLAYER2_WEIGHTS,
			(float*)KERASLAYER2_BIASES);

	// keras_lay[3]
	// 入力(Figure rows:8, Figure columns:8, Channel:32)
	// 出力(Figure rows:4, Figure columns:4, Channel:32)
	// 最大プーリング2(Figure rows:4, Figure columns:4, Channel:32)
	//  層間パラメータ: なし
	lay.input_channel = 32;
	lay.input_rows = 8;
	lay.input_columns = 8;
	lay.filter_rows = 2;
	lay.filter_columns = 2;
	lay.output_channel = 32;
	lay.output_rows = 4;
	lay.output_columns = 4;
	lay.relu_activation = 0;
	max_pooling(&lay, (float*)HIDDENLAYER3, (float*)HIDDENLAYER4);

	// keras_lay[4]
	// ドロップアウト(Dropout rate:0.25)
	// データフォーマット変化なし
	//  層間パラメータ: なし

	// keras_lay[5]
	// 入力(Figure rows:4, Figure columns:4, Channel:32)
	// 出力(Channel:512)
	// 平滑化(Channel:512)
	//  層間パラメータ: なし

	// keras_lay[6]
	// 入力(Channel:512)
	// 出力(Channel:128)
	// 全結合1(Activation:ReLU, Channel:128)
	//  層間パラメータ: 重み(Array[input_channel:512][output_channel:128]), バイアス(Array[output_channel:128])
	lay.input_channel = 512;
	lay.input_rows = 0;
	lay.input_columns = 0;
	lay.filter_rows = 0;
	lay.filter_columns = 0;
	lay.output_channel = 128;
	lay.output_rows = 0;
	lay.output_columns = 0;
	lay.relu_activation = 1;	// Activation:ReLU
	fully_connected(&lay,
			(float*)HIDDENLAYER4,
			(float*)HIDDENLAYER5,
			(float*)KERASLAYER6_WEIGHTS,
			(float*)KERASLAYER6_BIASES);

	// keras_lay[7]
	// ドロップアウト(Dropout rate:0.5, Channel:128)
	// データフォーマット変化なし
	//  層間パラメータ: なし

	// keras_lay[8]
	// 入力(Channel:128)
	// 出力(Channel:10)
	// 全結合2(Activation:Softmax, Channel:10)
	//  層間パラメータ: 重み(Array[input_channel:128][output_channel:10]), バイアス(Array[output_channel:10])
	lay.input_channel = 128;
	lay.input_rows = 0;
	lay.input_columns = 0;
	lay.filter_rows = 0;
	lay.filter_columns = 0;
	lay.output_channel = 10;
	lay.output_rows = 0;
	lay.output_columns = 0;
	lay.relu_activation = 0;
	fully_connected(&lay,
			(float*)HIDDENLAYER5,
			(float*)OUTPUTLAYER,
			(float*)KERASLAYER8_WEIGHTS,
			(float*)KERASLAYER8_BIASES);

	// 後処理
	// 最もスコアが高いインデックスを返す
	*result = post_proc((float*)OUTPUTLAYER, lay.output_channel);

	return 0;
}
