/*******************************************************************************
Vendor: Xilinx
Associated Filename: fir.h
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
#ifndef LDW7NS_H_
#define LDW7NS_H_

#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_fixed.h>

#define HDMI_COLUMN 1280
#define HDMI_ROW 720
#define HDMI_HALF 640

typedef ap_ufixed<24,24,AP_TRN_ZERO,AP_SAT> uint24_data;
typedef ap_ufixed<16,16,AP_TRN_ZERO,AP_SAT> uint16_data;
typedef ap_ufixed<8,8,AP_TRN_ZERO,AP_SAT> uint8_data;
typedef ap_ufixed<8,1,AP_TRN_ZERO,AP_SAT> rgb_coeff;

struct my_stream
{
	uint24_data rgb;
	bool user;
	bool last;

};

void LDW7ns(hls::stream<my_stream> &ARM2IP, hls::stream<my_stream> &IP2ARM);











#endif




//#define WEBCAM_COLUMN 640
//#define WEBCAM_HALF 320
//#define WEBCAM_ROW 480
//
//typedef ap_ufixed<32,32,AP_TRN_ZERO,AP_SAT> uint32_data;
//struct AXI_VAL
//{
//	uint32_data intensity;
//	bool last;
//};
//
//
//void cvt(hls::stream<AXI_VAL> &ARM2IP, hls::stream<AXI_VAL> &IP2ARM);
