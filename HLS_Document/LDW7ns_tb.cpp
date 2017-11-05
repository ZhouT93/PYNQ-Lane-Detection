/*******************************************************************************
Vendor: Xilinx
Associated Filename: fir_tes.c
Purpose: Vivado HLS Tutorial Example
Device: All
Revision History: May 30, 2008 - initial release

*******************************************************************************
Copyright 2008 - 2012 Xilinx, Inc. All rights reserved.

This file contains confidential and proprietary information of Xilinx, Inc. and
is protected under U.S. and international copyright and other intellectual
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials
distributed herewith. Except as otherwise provided in a valid license issued to
you by Xilinx, and to the maximum extent permitted by applicable law:
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY,
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether
in contract or tort, including negligence, or under any other theory of
liability) for any loss or damage of any kind or nature related to, arising under
or in connection with these materials, including for any direct, or any indirect,
special, incidental, or consequential loss or damage (including loss of data,
profits, goodwill, or any type of loss or damage suffered as a result of any
action brought by a third party) even if such damage or loss was reasonably
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any
application requiring fail-safe performance, such as life-support or safety
devices or systems, Class III medical devices, nuclear facilities, applications
related to the deployment of airbags, or any other applications that could lead
to death, personal injury, or severe property or environmental damage
(individually and collectively, "Critical Applications"). Customer assumes the
sole risk and liability of any use of Xilinx products in Critical Applications,
subject only to applicable laws and regulations governing limitations on product
liability.

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT
ALL TIMES.

*******************************************************************************/
#include <iostream>
#include "LDW7ns.h"
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_fixed.h>

#include <opencv2/core/core.hpp>
#include <hls_opencv.h>


using namespace std;

#define ROADIN "C:\\Users\\tz1115\\Downloads\\LDW7ns0815\\LDW7ns_prj\\road1280.jpg"
#define FRAME_OUT "C:\\Users\\tz1115\\Downloads\\LDW7ns0815\\LDW7ns_prj\\frame_out.jpg"

//void saveImage(const std::string path,cv::InputArray inArr)
//{
//	double min;
//	double max;
//	cv::minMaxIdx(inArr,&min,&max);
//	cv::Mat adjMap;
//	cv::convertScaleAbs(inArr,adjMap,255/max);
//	cv::imwrite(path,adjMap);
//}

int main ()
{

	cv::Mat camIn;
		cout << "Reading in the frame: " << ROADIN << endl;
		camIn = cv::imread(ROADIN);
		cout << "Finishes frame reading in: " << ROADIN << endl;
		const int column = camIn.cols;
		const int row = camIn.rows;
		cout << "No of rows: " << row <<"\t\tNo of columns: " << column <<endl;

		uint8_data r;
		uint8_data g;
		uint8_data b;
		uint24_data concat;
		hls::stream<my_stream> tb2ip;
		my_stream StreamIn;
		hls::stream<my_stream> ip2tb;
		my_stream StreamOut;
		uint24_data img[1280];

		int in_count = 0;//
		int out_count = 0;//


		for (int i = 0; i < row; i++)
		{

			for (int j = 0; j < column; j++)
			{
				r =  camIn.at<cv::Vec3b>(i,j)[2];
				g =  camIn.at<cv::Vec3b>(i,j)[1];
				b =  camIn.at<cv::Vec3b>(i,j)[0];

				concat = r*65536 + b*256 + g;

				//cout << concat << "\t";//
				StreamIn.rgb = concat;

				if (i == 0 && j == 0)
				{
					StreamIn.user = 1;
					StreamIn.last = 0;
				}

				else if (j == (column - 1))
				{
					StreamIn.user = 0;
					StreamIn.last = 1;
				}

				else
				{
					StreamIn.user = 0;
					StreamIn.last = 0;
				}

				//cout << StreamIn.rgb << "\t\t" << StreamIn.user << "\t\t" << StreamIn.last << endl;

				tb2ip << StreamIn;
				in_count ++;
			}
			//cout << endl;
		}

		//cout << "HLS::STREAM Pop in count: " << in_count << endl;//

		LDW7ns(tb2ip, ip2tb);

		cout << "tb Reading out: " << endl;//

		for (int i = 0; i < row; i++)
		{
			//cout << (i + 1) << ":\t";//
			for (int j = 0; j < column; j++)
			{
				ip2tb.read(StreamOut);
				out_count++;
				img[j] = StreamOut.rgb;
				cout << img[j]/65793 << "\t";//
			}

			cout << endl;//
		}

		cout << "HLS::STREAM Read out count: " << out_count << endl;//



		return 0;





}

//==============================================================================================
//	uint8_data r = camIn.at<cv::Vec3b>(0,0)[2];
//	uint8_data g = camIn.at<cv::Vec3b>(0,0)[1];
//	uint8_data b = camIn.at<cv::Vec3b>(0,0)[0];
//
//	cout << r << "\t\t" << b <<"\t\t" << g<< endl;

//	tmp = r*65536 + b*256 +g;
//
//	cout << tmp << endl;
//==============================================================================================
//	uint24_data max;
//	uint24_data min;
//	uint24_data in;
//
//	max = 100000000000000000000;
//	min = -123;
//	in = 15772175;
//
//	uint8_data r = in.range(23,16);
//	uint8_data b = in.range(15,8);
//	uint8_data g = in.range(7,0);
//
//
//	cout << "Max: " <<max << endl;//Max: 1.67772e+07
//	cout << "Min: " << min << endl;//Min: 0
//	cout << "In: " << in <<endl;//In: 1.57722e+07
//
//	cout << "R: " << r <<endl;//R: 240
//	cout << "B: " << b <<endl;//B: 170
//	cout << "G: " << g <<endl;//G: 15
//==============================================================================================

//		hls::stream<AXI_VAL> tb2ip;
//		hls::stream<AXI_VAL> ip2tb;
//
//
//		AXI_VAL AXI_in;
//		AXI_VAL AXI_out;
//		uint32_data tmp_intensity;
//		bool tmp_last;
//
//
//		//read in the gray scale image and get No of rows and columns
//		cv::Mat camIn;
//		cout << "Reading in the frame: " << WEBCAM_GRAYIN << endl;
//		camIn = cv::imread(WEBCAM_GRAYIN,0);
//		cout << "Finishes frame reading in: " << WEBCAM_GRAYIN << endl;
//		const int column = camIn.cols;
//		const int row = camIn.rows;
//		cout << "No of rows: " << row <<"\t\tNo of columns: " << column <<endl;
//
//		uint32_data frame_out[row][column];
//		char frame_display[row][column];//store the output image
//		cv::Mat out (cv::Size(column,row),CV_8UC1,frame_display,cv::Mat::AUTO_STEP); //
//
//		for (int i = 0; i < WEBCAM_ROW; i++)
//		{
//			//read in data
//			//cout << "reading in data: " << endl;
//			//cout << "TB Row " << (i + 1) << ": ";
//			for (int j = 0; j < WEBCAM_COLUMN; j++)
//			{
//				AXI_in.intensity = (unsigned int) camIn.at<unsigned char>(i,j);
//				tb2ip << AXI_in;
//				//cout <<  AXI_in.intensity << "\t";
//			}
//			//cout << endl;
//		}
//
//		cvt(tb2ip, ip2tb);
//
//		for (int i = 0; i < WEBCAM_ROW; i++)
//		{
//			cout << "TB Row " << (i + 1) << ": ";
//			for (int k = 0; k < WEBCAM_COLUMN; k++)
//			{
//
//				ip2tb.read(AXI_out);
//				tmp_intensity = AXI_out.intensity;
//				tmp_last = AXI_out.last;
//
//				cout << tmp_intensity << "\t";
//				frame_out[i][k] = tmp_intensity;
//				frame_display[i][k] = frame_out[i][k];
//
//			}
//
//			cout << endl;
//
//		}
//
//
//
//
//	saveImage(std::string(FRAME_OUT),out);
//==============================================================================================

