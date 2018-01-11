/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.

 Simple CNN Application for Inference only
==================================================================
*/
// Buffer for trained comvolutional neural network parameters
// 0x20300000 to 0x20370C40 size 0x00070C40
#define NN_BUFFER 0x20300000

// keras_lay[0]
// Input(Channel:1, Figure rows:28, Figure columns:28)
// Output(Channel:16, Figure rows:24, Figure columns:24)
// Convolutional layer 1(Activation:ReLU, Channel:16, Filter rows:5, Filter columns:5)
//  Parameter: Weights(Array[rows:5][columns:5][input_channel:1][output_channel:16]), Biases(Array[output_channel:16])
//
// keras_lay[1]
// Input(Channel:16, Figure rows:24, Figure columns:24)
// Output(Channel:16, Figure rows:12, Figure columns:12)
// Max Pooling layer 1(Channel:16, Figure rows:12, Figure columns:12)
//  Parameters: None
//
// keras_lay[2]
// Input (Channel:16, Figure rows:12, Figure columns:12)
// Output(Channel:32, Figure rows:8, Figure columns:8)
// Convolutional layer 2(Activation:ReLU, Channel:32, Filter rows:5, Filter columns:5)
//  Parameters: Weights(Array[rows:5][columns:5][input_channel:16][output_channel:32]), Biases(Array[output_channel:32])
//
// keras_lay[3]
// Input(Channel:32, Figure rows:8, Figure columns:8)
// Output(Channel:32, Figure rows:4, Figure columns:4)
// Max Pooling layer 2(Channel:32, Figure rows:4, Figure columns:4)
//  Parameters: None
//
// keras_lay[4]
// Dropout(Dropout rate:0.25)
// No data format conversion
//  Parameters: None
//
// keras_lay[5]
// Input(Channel:32, Figure rows:4, Figure columns:4)
// Output(Channel:512)
// Flatten layer(Channel:512)
//  Parameters: None
//
// keras_lay[6]
// Input(Channel:512)
// Output(Channel:128)
// Fully connected layer 1(Activation:ReLU, Channel:128)
//  Parameters: Weights(Array[input_channel:512][output_channel:128]), Biases(Array[output_channel:128])
//
// keras_lay[7]
// Dropout(Dropout rate:0.5, Channel:128)
// No data format conversion
//  Parameters: None
//
// keras_lay[8]
// Input(Channel:128)
// Output(Channel:10)
// Fully connected layer 2(Activation:Softmax, Channel:10)
//  Parameters: Weights(Array[input_channel:128][output_channel:10]), Biases(Array[output_channel:10])
//

//--- Addresses for Parameter ---
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

//--- Addresses for Layer ---
// 
// Input Layer
//  Layer size(Channel:1, Figure rows:28, Figure columns:28)
#define INPUTLAYER (KERASLAYER8_WEIGHTS + 0x1400)	// 0x206A0000 - 0x206A0C40 (size 0xC40)
// Hidden layer 1(Convolutional layer 1)
//  Layer size(Figure rows:24, Figure columns:24, Channel:16)
#define HIDDENLAYER1 (INPUTLAYER + 0xC40)	// 0x206A1000 - 0x206AA000 (size 0x9000)
// Hidden layer 2(Max Pooling layer 1)
//  Layer size(Figure rows:12, Figure columns:12, Channel:16)
#define HIDDENLAYER2 (HIDDENLAYER1 + 0x9000)	// 0x206AB000 - 0x206AD400 (size 0x2400)
// Hidden layer 3(Convolutional layer 2)
//  Layer size(Figure rows:8, Figure columns:8, Channel:32)
#define HIDDENLAYER3 (HIDDENLAYER2 + 0x2400)	// 0x206AE000 - 0x206B0000 (size 0x2000)
// Hidden layer 4(Max Pooling layer 2)
//  Layer size(Figure rows:4, Figure columns:4, Channel:32)
#define HIDDENLAYER4 (HIDDENLAYER3 + 0x2000)	// 0x206B0000 - 0x206B0800 (size 0x800)
// Hidden layer 5(Fully connected layer 1/Input is Channel:512 data which has flatten hidden layer 4)
//  Layer size(Channel:128)
#define HIDDENLAYER5 (HIDDENLAYER4 + 0x800)	// 0x206B1000 - 0x206B1200 (size 0x200)
// Output(Fully connected layer 2)
//  Lyaer size(Channel:10)
#define OUTPUTLAYER (HIDDENLAYER5 + 0x200)	// 0x206B1300 - 0x206B1328 (size 0x28)

// Inference target image size
#define IMAGE_ROWS		28
#define IMAGE_COLUMNS	28

// Inference target image buffer 0x20400000 - 0x20400C40 (size 0xC40)
#define TESTDATA 0x20400000

int mnist_cnn_eval(
		unsigned int *test,			// Input: Inference target image test[IMAGE_ROWS][IMAGE_COLUMNS]
		unsigned int *result		// Output: Inference result
);
