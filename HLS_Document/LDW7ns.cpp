/*******************************************************************************
Vendor: Xilinx
Associated Filename: fir.c
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
#include "LDW7ns.h"
using namespace std;
#include <ap_fixed.h>


void LDW7ns(hls::stream<my_stream> &ARM2IP, hls::stream<my_stream> &IP2ARM)
{
#pragma HLS INTERFACE axis port=IP2ARM
#pragma HLS INTERFACE axis port=ARM2IP
#pragma HLS INTERFACE ap_ctrl_none port=return

	const float CoeffR = 0.299;
	const float CoeffB = 0.114;
	const float CoeffG = 0.587;

	my_stream arm2ip;
	my_stream ip2arm;



	int InCount = 0;
	int RowPos = 0;
	int ColPos = 0;
	uint24_data rbg;
	float r;
	float g;
	float b;
	uint8_data gray_tmp;
	uint8_data GrayBuff[3] = {0,0,0};
#pragma HLS ARRAY_RESHAPE variable=GrayBuff complete dim=1
	bool ShiftRegStatus = 0;

	uint8_data EdgeDetOut = 0;
	uint24_data DataOut = 0;


	for (int i = 0; i < (HDMI_COLUMN * HDMI_ROW); i++)
	{
#pragma HLS PIPELINE
		//read in and shift
		arm2ip = ARM2IP.read();
		rbg = arm2ip.rgb;


		ColPos = InCount%HDMI_COLUMN;//column position of the new data
		RowPos = (InCount-ColPos)/HDMI_COLUMN;//row position of the new data

		r = rbg.range(23,16);
		b = rbg.range(15,8);
		g = rbg.range(7,0);

		gray_tmp = r * CoeffR + b * CoeffB + g * CoeffG;

		//processing data
		//first 2 elements of each row
		if(ColPos == 0 || ColPos == 1)
		{
			//cannot calculate since the shift register is not full
			ShiftRegStatus = 0;
			//shift the current shift register and add in new data
			GrayBuff[0] = GrayBuff[1];
			GrayBuff[1] = GrayBuff[2];
			GrayBuff[2] = gray_tmp;
			//output is 0

			DataOut = 0;

			if (RowPos == 0 && ColPos == 0)
			{
				//start of the frame
				ip2arm.user = 1;
				ip2arm.last = 0;
				ip2arm.rgb = DataOut;
				IP2ARM.write(ip2arm);
			}
			else
			{
				//not start of the frame
				ip2arm.user = 0;
				ip2arm.last = 0;
				ip2arm.rgb = DataOut;
				IP2ARM.write(ip2arm);
			}

		}
		else if (ColPos == 1279)
		{
			//last element of the line
			ShiftRegStatus = 1;
			GrayBuff[0] = GrayBuff[1];
			GrayBuff[1] = GrayBuff[2];
			GrayBuff[2] = gray_tmp;

			EdgeDetOut = -GrayBuff[0] + GrayBuff[2];
			DataOut = EdgeDetOut * 65793;

			ip2arm.user = 0;
			ip2arm.last = 1;
			ip2arm.rgb = DataOut;
			IP2ARM.write(ip2arm);

			//initialise the array again
			//the next 2 data should be invalid
			GrayBuff[0] = 0;
			GrayBuff[1] = 0;
			GrayBuff[2] = 0;


		}
		else
		{
			//other elements in between
			ShiftRegStatus = 1;
			GrayBuff[0] = GrayBuff[1];
			GrayBuff[1] = GrayBuff[2];
			GrayBuff[2] = gray_tmp;

			EdgeDetOut = -GrayBuff[0] + GrayBuff[2];
			DataOut = EdgeDetOut * 65793;

			ip2arm.user = 0;
			ip2arm.last = 0;
			ip2arm.rgb = DataOut;
			IP2ARM.write(ip2arm);

		}

		InCount ++;
//		if (RowPos == 0 && ColPos == 0)
//		{
//			//start of the frame
//			ip2arm.user = 1;
//			ip2arm.last = 0;
//		}
//
//		else if (ColPos == 1279)
//		{
//			//end of the line
//			ip2arm.user = 0;
//			ip2arm.last = 1;
//
//		}
//
//		else
//		{
//			//others
//			ip2arm.user = 0;
//			ip2arm.last = 0;
//		}
//
//		ip2arm.rgb = gray_tmp * 65793;
//		IP2ARM.write(ip2arm);

//		cout << "(" << RowPos << "," << ColPos << ")" << "\t";
//		if (ColPos == 1279)
//		{
//			cout << endl;
//		}






	}



}

//rgb2gray
/*
 *
void LDW7ns(hls::stream<my_stream> &ARM2IP, hls::stream<my_stream> &IP2ARM)
{
#pragma HLS INTERFACE axis port=IP2ARM
#pragma HLS INTERFACE axis port=ARM2IP
#pragma HLS INTERFACE ap_ctrl_none port=return

	const float CoeffR = 0.299;
	const float CoeffB = 0.114;
	const float CoeffG = 0.587;

	my_stream arm2ip;
	my_stream ip2arm;



	int InCount = 0;
	int RowPos = 0;
	int ColPos = 0;
	uint24_data rbg;
	float r;
	float g;
	float b;
	uint8_data gray;

	uint8_data EdgeDetOut = 0;
	uint24_data DataOut = 0;


	for (int i = 0; i < (HDMI_COLUMN * HDMI_ROW); i++)
	{
#pragma HLS PIPELINE
		//read in and shift
		arm2ip = ARM2IP.read();
		rbg = arm2ip.rgb;


		ColPos = InCount%HDMI_COLUMN;//column position of the new data
		RowPos = (InCount-ColPos)/HDMI_COLUMN;//row position of the new data

		r = rbg.range(23,16);
		b = rbg.range(15,8);
		g = rbg.range(7,0);

		gray = r * CoeffR + b * CoeffB + g * CoeffG;


		ip2arm.rgb = gray * 65793;
		if (RowPos == 0 && ColPos == 0)
		{
			//start of the frame
			ip2arm.user = 1;
			ip2arm.last = 0;
		}

		else if (ColPos == 1279)
		{
			//end of the line
			ip2arm.user = 0;
			ip2arm.last = 1;

		}

		else
		{
			//others
			ip2arm.user = 0;
			ip2arm.last = 0;
		}

		IP2ARM.write(ip2arm);

//		cout << "(" << RowPos << "," << ColPos << ")" << "\t";
//		if (ColPos == 1279)
//		{
//			cout << endl;
//		}

		InCount ++;




	}



}
 */



