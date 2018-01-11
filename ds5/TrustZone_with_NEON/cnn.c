/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.

 Simple CNN Application for Inference only
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

//--- Required processes for inference ---
// Pre-process(Input data normalization)
// Convolution
// Max pooling
// Fully connection
// Post-process(Get inference result)
// ReLU activation

//--- ReLU activation function ---
float relu(float value)
{
	if (value < 0.0f) {
		return 0.0f;
	}
	return value;
}

//--- Convolution ---
//
// keras_lay[0]
// Input(Channel:1, Figure rows:28, Figure columns:28)
// Output(Channel:16, Figure rows:24, Figure columns:24)
// Convolutional layer 1(Activation:ReLU, Channel:16, Filter rows:5, Filter columns:5)
//  Parameter: Weights(Array[rows:5][columns:5][input_channel:1][output_channel:16]), Biases(Array[output_channel:16])
//
// keras_lay[2]
// Input (Channel:16, Figure rows:12, Figure columns:12)
// Output(Channel:32, Figure rows:8, Figure columns:8)
// Convolutional layer 2(Activation:ReLU, Channel:32, Filter rows:5, Filter columns:5)
//  Parameters: Weights(Array[rows:5][columns:5][input_channel:16][output_channel:32]), Biases(Array[output_channel:32])
//
// Keras Conv2D default values(https://keras.io/ja/layers/convolutional/)
//	strides=(1, 1)  : Stride 1
//	padding='valid' : No padding
//
int convolution(
		layer_structure *lay,
		float *inputs,	// Input array: inputs[lay->input_rows][lay->input_columns][lay->input_channel]
		float *outputs,	// Output array: outputs[lay->output_rows][lay->output_columns][lay->output_channel]
		float *weights,	// Weights array: weights[lay->filter_rows][lay->filter_columns][lay->input_channel][lay->output_channel]
		float *biases	// Biases array: biases[lay->output_channnel]
) {
	unsigned int out_ch;		// Index for output channel
	unsigned int in_ch;			// Index for input channel
	unsigned int stride_row;	// Index for row of stride
	unsigned int stride_col;	// Index for column of stride
	unsigned int filter_row;	// Index for row of filter
	unsigned int filter_col;	// Index for column of filter
	float current_weight;		// Current weight value
	float current_biase;		// Current bias value
	float current_input;		// Current input value
	float current_result;		// Current result
	float kernel_result;		// Current result for kernel

	// Initialize output array
	for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {	// Loop for stride row
		for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {	// Loop for stride column
			for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {	// Loop for output channel
				((float*)outputs)[((stride_row * lay->output_columns * lay->output_channel)	// [x][ ][ ]
												+(stride_col * lay->output_channel)			// [ ][x][ ]
												+ out_ch)]                       			// [ ][ ][x]
				= 0.0f;
			}
		}
	}

	// Calculate weights
	for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {	// Loop for stride row
		for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {	// Loop for stride column
			for (filter_row = 0; filter_row < lay->filter_rows; filter_row++) {	// Loop for filter row
				for (filter_col = 0; filter_col < lay->filter_columns; filter_col++) {	// Loop for filter column
					for (in_ch = 0; in_ch < lay->input_channel; in_ch++) {	// Loop for input channnel
						// Get current input value
						current_input = ((float*)inputs)[  ((stride_row + filter_row) * lay->input_columns * lay->input_channel)	// [x][ ][ ]
														 + ((stride_col + filter_col)                      * lay->input_channel)	// [ ][x][ ]
														 + in_ch];																	// [ ][ ][x]
						for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {	// Loop for output channel
							// Get current weight value
							current_weight = ((float*)weights)[  (filter_row * lay->filter_columns * lay->input_channel * lay->output_channel)	// [x][ ][ ][ ]
															   + (filter_col                       * lay->input_channel * lay->output_channel)	// [ ][x][ ][ ]
															   + (in_ch                                                 * lay->output_channel)	// [ ][ ][x][ ]
															   + out_ch];																		// [ ][ ][ ][x]
							// Input * weight
							current_result = current_input * current_weight;
							// Add to outputs
							((float*)outputs)[  (stride_row * lay->output_columns * lay->output_channel)	// [x][ ][ ]
									          + (stride_col                       * lay->output_channel)	// [ ][x][ ]
											  + out_ch]                       								// [ ][ ][x]
							+= current_result;
						}
					}

				}
			}
		}
	}

	// Add biases
	for (stride_row = 0; stride_row < lay->output_rows; stride_row++) {	// Loop for stride row
		for (stride_col = 0; stride_col < lay->output_columns; stride_col++) {	// Loop for stride column
			for (out_ch = 0; out_ch < lay->output_channel; out_ch++) {	// Loop for output channel
				// Get current bias value
				current_biase = ((float*)biases)[out_ch];
				// Add current bias value to output
				((float*)outputs)[  (stride_row * lay->output_columns * lay->output_channel)	// [x][ ][ ]
						          + (stride_col                       * lay->output_channel)	// [ ][x][ ]
								  + out_ch]                       								// [ ][ ][x]
				+= current_biase;
				// Activation function
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

//--- Max pooling ---
//
// keras_lay[1]
// Input(Channel:16, Figure rows:24, Figure columns:24)
// Output(Channel:16, Figure rows:12, Figure columns:12)
// Max Pooling layer 1(Channel:16, Figure rows:12, Figure columns:12)
//  Parameters: None
//
// keras_lay[3]
// Input(Channel:32, Figure rows:8, Figure columns:8)
// Output(Channel:32, Figure rows:4, Figure columns:4)
// Max Pooling layer 2(Channel:32, Figure rows:4, Figure columns:4)
//  Parameters: None
//
int max_pooling(
		layer_structure *lay,
		float *inputs,	// Input array: inputs[lay->input_rows][lay->input_columns][lay->input_channel]
		float *outputs	// Output array: outputs[lay->output_rows][lay->output_columns][lay->output_channel]
) {
	unsigned int ch;			// Offset for channel
	unsigned int input_row;		// Offset for row of input
	unsigned int input_col;		// Offset for column of input
	unsigned int output_row;	// Offset for row of output
	unsigned int output_col;	// Offset for column of output
	unsigned int filter_row;	// Offset for row of filter
	unsigned int filter_col;	// Offset for column of filter
	float current_max;			// Current maximum value
	float current_value;		// Current value

	for (ch = 0; ch < lay->input_channel; ch++) {	// Loop for channel
		output_row = 0;
		for (input_row = 0; input_row < lay->input_rows; input_row += lay->filter_rows) {	// Loop for row of input(Including filter size)
			output_col = 0;
			for (input_col = 0; input_col < lay->input_columns; input_col += lay->filter_columns) {	// Loop for column of input(Including filter size)

				// Get maximum value of the filter
				for (filter_row = 0; filter_row < lay->filter_rows; filter_row++) {	// Row of filter
					for (filter_col = 0; filter_col < lay->filter_columns; filter_col++) {	// Column of filter
						// Get current input value
						current_value = ((float*)inputs)[((input_row + filter_row) * lay->input_columns * lay->input_channel)
														+ ((input_col + filter_col) * lay->input_channel)
														+ ch];
						if (filter_row == 0 && filter_col == 0) {	// First time(for the negative number)
							current_max = current_value;
						}
						else if (current_max < current_value) {		// Update the maximum value
							current_max = current_value;
						}
					}
				}

				// Store current maximum value to output
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

//--- Fully connected layer ---
//
// keras_lay[6]
// Input(Channel:512)
// Output(Channel:128)
// Fully connected layer 1(Activation:ReLU, Channel:128)
//  Parameters: Weights(Array[input_channel:512][output_channel:128]), Biases(Array[output_channel:128])
//
// keras_lay[8]
// Input(Channel:128)
// Output(Channel:10)
// Fully connected layer 2(Activation:Softmax, Channel:10)
//  Parameters: Weights(Array[input_channel:128][output_channel:10]), Biases(Array[output_channel:10])
//
int fully_connected(
		layer_structure *lay,
		float *inputs,	// Input array: inputs[lay->input_channel]
		float *outputs,	// Output array: outputs[lay->output_channel]
		float *weights,	// Weights array: weights[lay->filter_rows][lay->filter_columns]
		float *biases	// Biases array: biases[lay->output_channnel]
) {
	unsigned int o;				// Offset for output
	unsigned int i;				// Offset for input
	float current_weight;		// Current weight value
	float current_biase;		// Current bias value
	float current_input;		// Current input value
	float current_out;			// Current output value

	for (o = 0; o < lay->output_channel; o++) {	// Loop for output array(keras_lay[6]=128, keras_lay[8]=10)
		// Get current bias value
		current_biase = ((float*)biases)[o];
		current_out = 0.0f;
		for (i = 0; i < lay->input_channel; i++) {	// Loop for input array(keras_lay[6]=512, keras_lay[8]=128)
			// Get current input value
			current_input = ((float*)inputs)[i];
			// Get current weight value
			current_weight = ((float*)weights)[(i * lay->output_channel) + o];
			// Input * Weight
			current_out += (current_input * current_weight);
		}
		// sum + bias
		current_out +=  current_biase;
		// Activation function
		if (lay->relu_activation == 1) {
			current_out = relu(current_out);
		}
		// Store current output value to output array
		((float*)outputs)[o] = current_out;
	}

	return 0;
}

// Pre process
int pre_proc(
		unsigned int *test_images,	// test_images[IMAGE_ROWS][IMAGE_COLUMNS]
		float *outputs				// output[IMAGE_ROWS][IMAGE_COLUMNS]
) {
	unsigned int current_input;
	float normalized;
	unsigned int row, col;

	// Nomalize input image data from 0.0 to 1.0
	for (row = 0; row < IMAGE_ROWS; row++) {
		for (col = 0; col < IMAGE_COLUMNS; col++) {
			current_input = ((unsigned int*)test_images)[row * IMAGE_COLUMNS + col];
			normalized = (float)current_input / 255.0;
			((float*)outputs)[(row * IMAGE_COLUMNS + col)] = normalized;
		}
	}

	return 0;
}

// Post process
int post_proc(
		float *outlay,			// Output layer: outlay[channel]
		unsigned int channel	// Output layer channnel number
) {
	unsigned int idx, idx_max;
	float conf, conf_max;

	// Get the index for maximum score of output layer
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
		unsigned int *test_images,	// Input(Inference target image): test_images[IMAGE_ROWS][IMAGE_COLUMNS]
		unsigned int *result		// Output(Inference result)
) {
	static layer_structure lay;

	// Pre process
	pre_proc(test_images, (float*)INPUTLAYER);

	// keras_lay[0]
	// Input(Channel:1, Figure rows:28, Figure columns:28)
	// Output(Channel:16, Figure rows:24, Figure columns:24)
	// Convolutional layer 1(Activation:ReLU, Channel:16, Filter rows:5, Filter columns:5)
	//  Parameter: Weights(Array[rows:5][columns:5][input_channel:1][output_channel:16]), Biases(Array[output_channel:16])
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
	// Input(Channel:16, Figure rows:24, Figure columns:24)
	// Output(Channel:16, Figure rows:12, Figure columns:12)
	// Max Pooling layer 1(Channel:16, Figure rows:12, Figure columns:12)
	//  Parameters: None
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
	// Input (Channel:16, Figure rows:12, Figure columns:12)
	// Output(Channel:32, Figure rows:8, Figure columns:8)
	// Convolutional layer 2(Activation:ReLU, Channel:32, Filter rows:5, Filter columns:5)
	//  Parameters: Weights(Array[rows:5][columns:5][input_channel:16][output_channel:32]), Biases(Array[output_channel:32])
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
	// Input(Channel:32, Figure rows:8, Figure columns:8)
	// Output(Channel:32, Figure rows:4, Figure columns:4)
	// Max Pooling layer 2(Channel:32, Figure rows:4, Figure columns:4)
	//  Parameters: None
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
	// Dropout(Dropout rate:0.25)
	// No data format conversion
	//  Parameters: None

	// keras_lay[5]
	// Input(Channel:32, Figure rows:4, Figure columns:4)
	// Output(Channel:512)
	// Flatten layer(Channel:512)
	//  Parameters: None

	// keras_lay[6]
	// Input(Channel:512)
	// Output(Channel:128)
	// Fully connected layer 1(Activation:ReLU, Channel:128)
	//  Parameters: Weights(Array[input_channel:512][output_channel:128]), Biases(Array[output_channel:128])
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
	// Dropout(Dropout rate:0.5, Channel:128)
	// No data format conversion
	//  Parameters: None

	// keras_lay[8]
	// Input(Channel:128)
	// Output(Channel:10)
	// Fully connected layer 2(Activation:Softmax, Channel:10)
	//  Parameters: Weights(Array[input_channel:128][output_channel:10]), Biases(Array[output_channel:10])
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

	// Post process
	*result = post_proc((float*)OUTPUTLAYER, lay.output_channel);

	return 0;
}
