/*
==================================================================
 Copyright ARM Ltd 2017. All rights reserved.

 Simple CNN Application for Inference only
==================================================================
*/
// Buffer for trained comvolutional neural network parameters
// from 0x80300000 size 0x00070C40
#define SECURE_BUFFER	0x80300000

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

//--- Addresses for Layer ---
//
// Input Layer
//  Layer size(Channel:1, Figure rows:28, Figure columns:28)
#define INPUTLAYER 0x80350000	// 0x80350000 to 0x80350C40 (size 0xC40)
// Hidden layer 1(Convolutional layer 1)
//  Layer size(Figure rows:24, Figure columns:24, Channel:16)
#define HIDDENLAYER1 0x80351000	// 0x80351000 to 0x8035A000 (size 0x9000)
// Hidden layer 2(Max Pooling layer 1)
//  Layer size(Figure rows:12, Figure columns:12, Channel:16)
#define HIDDENLAYER2 0x8035B000	// 0x8035B000 to 0x8035D400 (size 0x2400)
// Hidden layer 3(Convolutional layer 2)
//  Layer size(Figure rows:8, Figure columns:8, Channel:32)
#define HIDDENLAYER3 0x8035E000	// 0x8035E000 to 0x80360000 (size 0x2000)
// Hidden layer 4(Max Pooling layer 2)
//  Layer size(Figure rows:4, Figure columns:4, Channel:32)
#define HIDDENLAYER4 0x80360000	// 0x80360000 to 0x80360800 (size 0x800)
// Hidden layer 5(Fully connected layer 1/Input is Channel:512 data which has flatten hidden layer 4)
//  Layer size(Channel:128)
#define HIDDENLAYER5 0x80361000	// 0x80361000 to 0x80361200 (size 0x200)
// Output(Fully connected layer 2)
//  Lyaer size(Channel:10)
#define OUTPUTLAYER 0x80361300	// 0x80361300 to 0x80361328 (size 0x28)

// Inference target image size
#define IMAGE_ROWS		28
#define IMAGE_COLUMNS	28
#define TESTDATA 0x80370000		// 0x80370000 to 0x80370C40 (size 0xC40)

int mnist_cnn_eval(
		unsigned int *test,			// Input: Inference target image test[IMAGE_ROWS][IMAGE_COLUMNS]
		unsigned int *result		// Output: Inference result
);
