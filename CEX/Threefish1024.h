// The GPL version 3 License (GPLv3)
// 
// Copyright (c) 2017 vtdev.com
// This file is part of the CEX Cryptographic library.
// 
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.

#ifndef _CEX_THREEFISH1024_H
#define _CEX_THREEFISH1024_H

#include "CexDomain.h"
#include "IntUtils.h"

NAMESPACE_DIGEST

/**
* \internal
*/
class Threefish1024
{
private:
	static const size_t BLOCK_SIZE = 128;

	static ulong GetParity(const std::vector<ulong> &Key)
	{
		ulong parity = 0x1BD11BDAA9FC1A22;

		for (size_t i = 0; i < Key.size(); i++)
			parity ^= Key[i];

		return parity;
	}

	static inline void Mix(ulong &A, ulong &B, uint R)
	{
		A += B;
		B = Utility::IntUtils::RotL64(B, R) ^ A;
	}

	static inline void Inject(ulong &A, ulong &B, uint R, ulong K0, ulong K1)
	{
		B += K1;
		A += B + K0;
		B = Utility::IntUtils::RotL64(B, R) ^ A;
	}

public:

	template <typename T>
	static void Transfrom128(std::vector<ulong> &Input, size_t InOffset, T &Output)
	{
		// cache the block, key, and tweak
		ulong B0 = Input[0];
		ulong B1 = Input[1];
		ulong B2 = Input[2];
		ulong B3 = Input[3];
		ulong B4 = Input[4];
		ulong B5 = Input[5];
		ulong B6 = Input[6];
		ulong B7 = Input[7];
		ulong B8 = Input[8];
		ulong B9 = Input[9];
		ulong B10 = Input[10];
		ulong B11 = Input[11];
		ulong B12 = Input[12];
		ulong B13 = Input[13];
		ulong B14 = Input[14];
		ulong B15 = Input[15];
		ulong K0 = Output.S[0];
		ulong K1 = Output.S[1];
		ulong K2 = Output.S[2];
		ulong K3 = Output.S[3];
		ulong K4 = Output.S[4];
		ulong K5 = Output.S[5];
		ulong K6 = Output.S[6];
		ulong K7 = Output.S[7];
		ulong K8 = Output.S[8];
		ulong K9 = Output.S[9];
		ulong K10 = Output.S[10];
		ulong K11 = Output.S[11];
		ulong K12 = Output.S[12];
		ulong K13 = Output.S[13];
		ulong K14 = Output.S[14];
		ulong K15 = Output.S[15];
		ulong K16 = GetParity(Output.S);
		ulong T0 = Output.T[0];
		ulong T1 = Output.T[1];
		ulong T2 = Output.T[0] ^ Output.T[1];

		// 80 rounds
		Inject(B0, B1, 24, K0, K1);
		Inject(B2, B3, 13, K2, K3);
		Inject(B4, B5, 8, K4, K5);
		Inject(B6, B7, 47, K6, K7);
		Inject(B8, B9, 8, K8, K9);
		Inject(B10, B11, 17, K10, K11);
		Inject(B12, B13, 22, K12, K13 + T0);
		Inject(B14, B15, 37, K14 + T1, K15);
		Mix(B0, B9, 38);
		Mix(B2, B13, 19);
		Mix(B6, B11, 10);
		Mix(B4, B15, 55);
		Mix(B10, B7, 49);
		Mix(B12, B3, 18);
		Mix(B14, B5, 23);
		Mix(B8, B1, 52);
		Mix(B0, B7, 33);
		Mix(B2, B5, 4);
		Mix(B4, B3, 51);
		Mix(B6, B1, 13);
		Mix(B12, B15, 34);
		Mix(B14, B13, 41);
		Mix(B8, B11, 59);
		Mix(B10, B9, 17);
		Mix(B0, B15, 5);
		Mix(B2, B11, 20);
		Mix(B6, B13, 48);
		Mix(B4, B9, 41);
		Mix(B14, B1, 47);
		Mix(B8, B5, 28);
		Mix(B10, B3, 16);
		Mix(B12, B7, 25);
		Inject(B0, B1, 41, K1, K2);
		Inject(B2, B3, 9, K3, K4);
		Inject(B4, B5, 37, K5, K6);
		Inject(B6, B7, 31, K7, K8);
		Inject(B8, B9, 12, K9, K10);
		Inject(B10, B11, 47, K11, K12);
		Inject(B12, B13, 44, K13, K14 + T1);
		Inject(B14, B15, 30, K15 + T2, K16 + 1);
		Mix(B0, B9, 16);
		Mix(B2, B13, 34);
		Mix(B6, B11, 56);
		Mix(B4, B15, 51);
		Mix(B10, B7, 4);
		Mix(B12, B3, 53);
		Mix(B14, B5, 42);
		Mix(B8, B1, 41);
		Mix(B0, B7, 31);
		Mix(B2, B5, 44);
		Mix(B4, B3, 47);
		Mix(B6, B1, 46);
		Mix(B12, B15, 19);
		Mix(B14, B13, 42);
		Mix(B8, B11, 44);
		Mix(B10, B9, 25);
		Mix(B0, B15, 9);
		Mix(B2, B11, 48);
		Mix(B6, B13, 35);
		Mix(B4, B9, 52);
		Mix(B14, B1, 23);
		Mix(B8, B5, 31);
		Mix(B10, B3, 37);
		Mix(B12, B7, 20);
		Inject(B0, B1, 24, K2, K3);
		Inject(B2, B3, 13, K4, K5);
		Inject(B4, B5, 8, K6, K7);
		Inject(B6, B7, 47, K8, K9);
		Inject(B8, B9, 8, K10, K11);
		Inject(B10, B11, 17, K12, K13);
		Inject(B12, B13, 22, K14, K15 + T2);
		Inject(B14, B15, 37, K16 + T0, K0 + 2);
		Mix(B0, B9, 38);
		Mix(B2, B13, 19);
		Mix(B6, B11, 10);
		Mix(B4, B15, 55);
		Mix(B10, B7, 49);
		Mix(B12, B3, 18);
		Mix(B14, B5, 23);
		Mix(B8, B1, 52);
		Mix(B0, B7, 33);
		Mix(B2, B5, 4);
		Mix(B4, B3, 51);
		Mix(B6, B1, 13);
		Mix(B12, B15, 34);
		Mix(B14, B13, 41);
		Mix(B8, B11, 59);
		Mix(B10, B9, 17);
		Mix(B0, B15, 5);
		Mix(B2, B11, 20);
		Mix(B6, B13, 48);
		Mix(B4, B9, 41);
		Mix(B14, B1, 47);
		Mix(B8, B5, 28);
		Mix(B10, B3, 16);
		Mix(B12, B7, 25);
		Inject(B0, B1, 41, K3, K4);
		Inject(B2, B3, 9, K5, K6);
		Inject(B4, B5, 37, K7, K8);
		Inject(B6, B7, 31, K9, K10);
		Inject(B8, B9, 12, K11, K12);
		Inject(B10, B11, 47, K13, K14);
		Inject(B12, B13, 44, K15, K16 + T0);
		Inject(B14, B15, 30, K0 + T1, K1 + 3);
		Mix(B0, B9, 16);
		Mix(B2, B13, 34);
		Mix(B6, B11, 56);
		Mix(B4, B15, 51);
		Mix(B10, B7, 4);
		Mix(B12, B3, 53);
		Mix(B14, B5, 42);
		Mix(B8, B1, 41);
		Mix(B0, B7, 31);
		Mix(B2, B5, 44);
		Mix(B4, B3, 47);
		Mix(B6, B1, 46);
		Mix(B12, B15, 19);
		Mix(B14, B13, 42);
		Mix(B8, B11, 44);
		Mix(B10, B9, 25);
		Mix(B0, B15, 9);
		Mix(B2, B11, 48);
		Mix(B6, B13, 35);
		Mix(B4, B9, 52);
		Mix(B14, B1, 23);
		Mix(B8, B5, 31);
		Mix(B10, B3, 37);
		Mix(B12, B7, 20);
		Inject(B0, B1, 24, K4, K5);
		Inject(B2, B3, 13, K6, K7);
		Inject(B4, B5, 8, K8, K9);
		Inject(B6, B7, 47, K10, K11);
		Inject(B8, B9, 8, K12, K13);
		Inject(B10, B11, 17, K14, K15);
		Inject(B12, B13, 22, K16, K0 + T1);
		Inject(B14, B15, 37, K1 + T2, K2 + 4);
		Mix(B0, B9, 38);
		Mix(B2, B13, 19);
		Mix(B6, B11, 10);
		Mix(B4, B15, 55);
		Mix(B10, B7, 49);
		Mix(B12, B3, 18);
		Mix(B14, B5, 23);
		Mix(B8, B1, 52);
		Mix(B0, B7, 33);
		Mix(B2, B5, 4);
		Mix(B4, B3, 51);
		Mix(B6, B1, 13);
		Mix(B12, B15, 34);
		Mix(B14, B13, 41);
		Mix(B8, B11, 59);
		Mix(B10, B9, 17);
		Mix(B0, B15, 5);
		Mix(B2, B11, 20);
		Mix(B6, B13, 48);
		Mix(B4, B9, 41);
		Mix(B14, B1, 47);
		Mix(B8, B5, 28);
		Mix(B10, B3, 16);
		Mix(B12, B7, 25);
		Inject(B0, B1, 41, K5, K6);
		Inject(B2, B3, 9, K7, K8);
		Inject(B4, B5, 37, K9, K10);
		Inject(B6, B7, 31, K11, K12);
		Inject(B8, B9, 12, K13, K14);
		Inject(B10, B11, 47, K15, K16);
		Inject(B12, B13, 44, K0, K1 + T2);
		Inject(B14, B15, 30, K2 + T0, K3 + 5);
		Mix(B0, B9, 16);
		Mix(B2, B13, 34);
		Mix(B6, B11, 56);
		Mix(B4, B15, 51);
		Mix(B10, B7, 4);
		Mix(B12, B3, 53);
		Mix(B14, B5, 42);
		Mix(B8, B1, 41);
		Mix(B0, B7, 31);
		Mix(B2, B5, 44);
		Mix(B4, B3, 47);
		Mix(B6, B1, 46);
		Mix(B12, B15, 19);
		Mix(B14, B13, 42);
		Mix(B8, B11, 44);
		Mix(B10, B9, 25);
		Mix(B0, B15, 9);
		Mix(B2, B11, 48);
		Mix(B6, B13, 35);
		Mix(B4, B9, 52);
		Mix(B14, B1, 23);
		Mix(B8, B5, 31);
		Mix(B10, B3, 37);
		Mix(B12, B7, 20);
		Inject(B0, B1, 24, K6, K7);
		Inject(B2, B3, 13, K8, K9);
		Inject(B4, B5, 8, K10, K11);
		Inject(B6, B7, 47, K12, K13);
		Inject(B8, B9, 8, K14, K15);
		Inject(B10, B11, 17, K16, K0);
		Inject(B12, B13, 22, K1, K2 + T0);
		Inject(B14, B15, 37, K3 + T1, K4 + 6);
		Mix(B0, B9, 38);
		Mix(B2, B13, 19);
		Mix(B6, B11, 10);
		Mix(B4, B15, 55);
		Mix(B10, B7, 49);
		Mix(B12, B3, 18);
		Mix(B14, B5, 23);
		Mix(B8, B1, 52);
		Mix(B0, B7, 33);
		Mix(B2, B5, 4);
		Mix(B4, B3, 51);
		Mix(B6, B1, 13);
		Mix(B12, B15, 34);
		Mix(B14, B13, 41);
		Mix(B8, B11, 59);
		Mix(B10, B9, 17);
		Mix(B0, B15, 5);
		Mix(B2, B11, 20);
		Mix(B6, B13, 48);
		Mix(B4, B9, 41);
		Mix(B14, B1, 47);
		Mix(B8, B5, 28);
		Mix(B10, B3, 16);
		Mix(B12, B7, 25);
		Inject(B0, B1, 41, K7, K8);
		Inject(B2, B3, 9, K9, K10);
		Inject(B4, B5, 37, K11, K12);
		Inject(B6, B7, 31, K13, K14);
		Inject(B8, B9, 12, K15, K16);
		Inject(B10, B11, 47, K0, K1);
		Inject(B12, B13, 44, K2, K3 + T1);
		Inject(B14, B15, 30, K4 + T2, K5 + 7);
		Mix(B0, B9, 16);
		Mix(B2, B13, 34);
		Mix(B6, B11, 56);
		Mix(B4, B15, 51);
		Mix(B10, B7, 4);
		Mix(B12, B3, 53);
		Mix(B14, B5, 42);
		Mix(B8, B1, 41);
		Mix(B0, B7, 31);
		Mix(B2, B5, 44);
		Mix(B4, B3, 47);
		Mix(B6, B1, 46);
		Mix(B12, B15, 19);
		Mix(B14, B13, 42);
		Mix(B8, B11, 44);
		Mix(B10, B9, 25);
		Mix(B0, B15, 9);
		Mix(B2, B11, 48);
		Mix(B6, B13, 35);
		Mix(B4, B9, 52);
		Mix(B14, B1, 23);
		Mix(B8, B5, 31);
		Mix(B10, B3, 37);
		Mix(B12, B7, 20);
		Inject(B0, B1, 24, K8, K9);
		Inject(B2, B3, 13, K10, K11);
		Inject(B4, B5, 8, K12, K13);
		Inject(B6, B7, 47, K14, K15);
		Inject(B8, B9, 8, K16, K0);
		Inject(B10, B11, 17, K1, K2);
		Inject(B12, B13, 22, K3, K4 + T2);
		Inject(B14, B15, 37, K5 + T0, K6 + 8);
		Mix(B0, B9, 38);
		Mix(B2, B13, 19);
		Mix(B6, B11, 10);
		Mix(B4, B15, 55);
		Mix(B10, B7, 49);
		Mix(B12, B3, 18);
		Mix(B14, B5, 23);
		Mix(B8, B1, 52);
		Mix(B0, B7, 33);
		Mix(B2, B5, 4);
		Mix(B4, B3, 51);
		Mix(B6, B1, 13);
		Mix(B12, B15, 34);
		Mix(B14, B13, 41);
		Mix(B8, B11, 59);
		Mix(B10, B9, 17);
		Mix(B0, B15, 5);
		Mix(B2, B11, 20);
		Mix(B6, B13, 48);
		Mix(B4, B9, 41);
		Mix(B14, B1, 47);
		Mix(B8, B5, 28);
		Mix(B10, B3, 16);
		Mix(B12, B7, 25);
		Inject(B0, B1, 41, K9, K10);
		Inject(B2, B3, 9, K11, K12);
		Inject(B4, B5, 37, K13, K14);
		Inject(B6, B7, 31, K15, K16);
		Inject(B8, B9, 12, K0, K1);
		Inject(B10, B11, 47, K2, K3);
		Inject(B12, B13, 44, K4, K5 + T0);
		Inject(B14, B15, 30, K6 + T1, K7 + 9);
		Mix(B0, B9, 16);
		Mix(B2, B13, 34);
		Mix(B6, B11, 56);
		Mix(B4, B15, 51);
		Mix(B10, B7, 4);
		Mix(B12, B3, 53);
		Mix(B14, B5, 42);
		Mix(B8, B1, 41);
		Mix(B0, B7, 31);
		Mix(B2, B5, 44);
		Mix(B4, B3, 47);
		Mix(B6, B1, 46);
		Mix(B12, B15, 19);
		Mix(B14, B13, 42);
		Mix(B8, B11, 44);
		Mix(B10, B9, 25);
		Mix(B0, B15, 9);
		Mix(B2, B11, 48);
		Mix(B6, B13, 35);
		Mix(B4, B9, 52);
		Mix(B14, B1, 23);
		Mix(B8, B5, 31);
		Mix(B10, B3, 37);
		Mix(B12, B7, 20);
		Inject(B0, B1, 24, K10, K11);
		Inject(B2, B3, 13, K12, K13);
		Inject(B4, B5, 8, K14, K15);
		Inject(B6, B7, 47, K16, K0);
		Inject(B8, B9, 8, K1, K2);
		Inject(B10, B11, 17, K3, K4);
		Inject(B12, B13, 22, K5, K6 + T1);
		Inject(B14, B15, 37, K7 + T2, K8 + 10);
		Mix(B0, B9, 38);
		Mix(B2, B13, 19);
		Mix(B6, B11, 10);
		Mix(B4, B15, 55);
		Mix(B10, B7, 49);
		Mix(B12, B3, 18);
		Mix(B14, B5, 23);
		Mix(B8, B1, 52);
		Mix(B0, B7, 33);
		Mix(B2, B5, 4);
		Mix(B4, B3, 51);
		Mix(B6, B1, 13);
		Mix(B12, B15, 34);
		Mix(B14, B13, 41);
		Mix(B8, B11, 59);
		Mix(B10, B9, 17);
		Mix(B0, B15, 5);
		Mix(B2, B11, 20);
		Mix(B6, B13, 48);
		Mix(B4, B9, 41);
		Mix(B14, B1, 47);
		Mix(B8, B5, 28);
		Mix(B10, B3, 16);
		Mix(B12, B7, 25);
		Inject(B0, B1, 41, K11, K12);
		Inject(B2, B3, 9, K13, K14);
		Inject(B4, B5, 37, K15, K16);
		Inject(B6, B7, 31, K0, K1);
		Inject(B8, B9, 12, K2, K3);
		Inject(B10, B11, 47, K4, K5);
		Inject(B12, B13, 44, K6, K7 + T2);
		Inject(B14, B15, 30, K8 + T0, K9 + 11);
		Mix(B0, B9, 16);
		Mix(B2, B13, 34);
		Mix(B6, B11, 56);
		Mix(B4, B15, 51);
		Mix(B10, B7, 4);
		Mix(B12, B3, 53);
		Mix(B14, B5, 42);
		Mix(B8, B1, 41);
		Mix(B0, B7, 31);
		Mix(B2, B5, 44);
		Mix(B4, B3, 47);
		Mix(B6, B1, 46);
		Mix(B12, B15, 19);
		Mix(B14, B13, 42);
		Mix(B8, B11, 44);
		Mix(B10, B9, 25);
		Mix(B0, B15, 9);
		Mix(B2, B11, 48);
		Mix(B6, B13, 35);
		Mix(B4, B9, 52);
		Mix(B14, B1, 23);
		Mix(B8, B5, 31);
		Mix(B10, B3, 37);
		Mix(B12, B7, 20);
		Inject(B0, B1, 24, K12, K13);
		Inject(B2, B3, 13, K14, K15);
		Inject(B4, B5, 8, K16, K0);
		Inject(B6, B7, 47, K1, K2);
		Inject(B8, B9, 8, K3, K4);
		Inject(B10, B11, 17, K5, K6);
		Inject(B12, B13, 22, K7, K8 + T0);
		Inject(B14, B15, 37, K9 + T1, K10 + 12);
		Mix(B0, B9, 38);
		Mix(B2, B13, 19);
		Mix(B6, B11, 10);
		Mix(B4, B15, 55);
		Mix(B10, B7, 49);
		Mix(B12, B3, 18);
		Mix(B14, B5, 23);
		Mix(B8, B1, 52);
		Mix(B0, B7, 33);
		Mix(B2, B5, 4);
		Mix(B4, B3, 51);
		Mix(B6, B1, 13);
		Mix(B12, B15, 34);
		Mix(B14, B13, 41);
		Mix(B8, B11, 59);
		Mix(B10, B9, 17);
		Mix(B0, B15, 5);
		Mix(B2, B11, 20);
		Mix(B6, B13, 48);
		Mix(B4, B9, 41);
		Mix(B14, B1, 47);
		Mix(B8, B5, 28);
		Mix(B10, B3, 16);
		Mix(B12, B7, 25);
		Inject(B0, B1, 41, K13, K14);
		Inject(B2, B3, 9, K15, K16);
		Inject(B4, B5, 37, K0, K1);
		Inject(B6, B7, 31, K2, K3);
		Inject(B8, B9, 12, K4, K5);
		Inject(B10, B11, 47, K6, K7);
		Inject(B12, B13, 44, K8, K9 + T1);
		Inject(B14, B15, 30, K10 + T2, K11 + 13);
		Mix(B0, B9, 16);
		Mix(B2, B13, 34);
		Mix(B6, B11, 56);
		Mix(B4, B15, 51);
		Mix(B10, B7, 4);
		Mix(B12, B3, 53);
		Mix(B14, B5, 42);
		Mix(B8, B1, 41);
		Mix(B0, B7, 31);
		Mix(B2, B5, 44);
		Mix(B4, B3, 47);
		Mix(B6, B1, 46);
		Mix(B12, B15, 19);
		Mix(B14, B13, 42);
		Mix(B8, B11, 44);
		Mix(B10, B9, 25);
		Mix(B0, B15, 9);
		Mix(B2, B11, 48);
		Mix(B6, B13, 35);
		Mix(B4, B9, 52);
		Mix(B14, B1, 23);
		Mix(B8, B5, 31);
		Mix(B10, B3, 37);
		Mix(B12, B7, 20);
		Inject(B0, B1, 24, K14, K15);
		Inject(B2, B3, 13, K16, K0);
		Inject(B4, B5, 8, K1, K2);
		Inject(B6, B7, 47, K3, K4);
		Inject(B8, B9, 8, K5, K6);
		Inject(B10, B11, 17, K7, K8);
		Inject(B12, B13, 22, K9, K10 + T2);
		Inject(B14, B15, 37, K11 + T0, K12 + 14);
		Mix(B0, B9, 38);
		Mix(B2, B13, 19);
		Mix(B6, B11, 10);
		Mix(B4, B15, 55);
		Mix(B10, B7, 49);
		Mix(B12, B3, 18);
		Mix(B14, B5, 23);
		Mix(B8, B1, 52);
		Mix(B0, B7, 33);
		Mix(B2, B5, 4);
		Mix(B4, B3, 51);
		Mix(B6, B1, 13);
		Mix(B12, B15, 34);
		Mix(B14, B13, 41);
		Mix(B8, B11, 59);
		Mix(B10, B9, 17);
		Mix(B0, B15, 5);
		Mix(B2, B11, 20);
		Mix(B6, B13, 48);
		Mix(B4, B9, 41);
		Mix(B14, B1, 47);
		Mix(B8, B5, 28);
		Mix(B10, B3, 16);
		Mix(B12, B7, 25);
		Inject(B0, B1, 41, K15, K16);
		Inject(B2, B3, 9, K0, K1);
		Inject(B4, B5, 37, K2, K3);
		Inject(B6, B7, 31, K4, K5);
		Inject(B8, B9, 12, K6, K7);
		Inject(B10, B11, 47, K8, K9);
		Inject(B12, B13, 44, K10, K11 + T0);
		Inject(B14, B15, 30, K12 + T1, K13 + 15);
		Mix(B0, B9, 16);
		Mix(B2, B13, 34);
		Mix(B6, B11, 56);
		Mix(B4, B15, 51);
		Mix(B10, B7, 4);
		Mix(B12, B3, 53);
		Mix(B14, B5, 42);
		Mix(B8, B1, 41);
		Mix(B0, B7, 31);
		Mix(B2, B5, 44);
		Mix(B4, B3, 47);
		Mix(B6, B1, 46);
		Mix(B12, B15, 19);
		Mix(B14, B13, 42);
		Mix(B8, B11, 44);
		Mix(B10, B9, 25);
		Mix(B0, B15, 9);
		Mix(B2, B11, 48);
		Mix(B6, B13, 35);
		Mix(B4, B9, 52);
		Mix(B14, B1, 23);
		Mix(B8, B5, 31);
		Mix(B10, B3, 37);
		Mix(B12, B7, 20);
		Inject(B0, B1, 24, K16, K0);
		Inject(B2, B3, 13, K1, K2);
		Inject(B4, B5, 8, K3, K4);
		Inject(B6, B7, 47, K5, K6);
		Inject(B8, B9, 8, K7, K8);
		Inject(B10, B11, 17, K9, K10);
		Inject(B12, B13, 22, K11, K12 + T1);
		Inject(B14, B15, 37, K13 + T2, K14 + 16);
		Mix(B0, B9, 38);
		Mix(B2, B13, 19);
		Mix(B6, B11, 10);
		Mix(B4, B15, 55);
		Mix(B10, B7, 49);
		Mix(B12, B3, 18);
		Mix(B14, B5, 23);
		Mix(B8, B1, 52);
		Mix(B0, B7, 33);
		Mix(B2, B5, 4);
		Mix(B4, B3, 51);
		Mix(B6, B1, 13);
		Mix(B12, B15, 34);
		Mix(B14, B13, 41);
		Mix(B8, B11, 59);
		Mix(B10, B9, 17);
		Mix(B0, B15, 5);
		Mix(B2, B11, 20);
		Mix(B6, B13, 48);
		Mix(B4, B9, 41);
		Mix(B14, B1, 47);
		Mix(B8, B5, 28);
		Mix(B10, B3, 16);
		Mix(B12, B7, 25);
		Inject(B0, B1, 41, K0, K1);
		Inject(B2, B3, 9, K2, K3);
		Inject(B4, B5, 37, K4, K5);
		Inject(B6, B7, 31, K6, K7);
		Inject(B8, B9, 12, K8, K9);
		Inject(B10, B11, 47, K10, K11);
		Inject(B12, B13, 44, K12, K13 + T2);
		Inject(B14, B15, 30, K14 + T0, K15 + 17);
		Mix(B0, B9, 16);
		Mix(B2, B13, 34);
		Mix(B6, B11, 56);
		Mix(B4, B15, 51);
		Mix(B10, B7, 4);
		Mix(B12, B3, 53);
		Mix(B14, B5, 42);
		Mix(B8, B1, 41);
		Mix(B0, B7, 31);
		Mix(B2, B5, 44);
		Mix(B4, B3, 47);
		Mix(B6, B1, 46);
		Mix(B12, B15, 19);
		Mix(B14, B13, 42);
		Mix(B8, B11, 44);
		Mix(B10, B9, 25);
		Mix(B0, B15, 9);
		Mix(B2, B11, 48);
		Mix(B6, B13, 35);
		Mix(B4, B9, 52);
		Mix(B14, B1, 23);
		Mix(B8, B5, 31);
		Mix(B10, B3, 37);
		Mix(B12, B7, 20);
		Inject(B0, B1, 24, K1, K2);
		Inject(B2, B3, 13, K3, K4);
		Inject(B4, B5, 8, K5, K6);
		Inject(B6, B7, 47, K7, K8);
		Inject(B8, B9, 8, K9, K10);
		Inject(B10, B11, 17, K11, K12);
		Inject(B12, B13, 22, K13, K14 + T0);
		Inject(B14, B15, 37, K15 + T1, K16 + 18);
		Mix(B0, B9, 38);
		Mix(B2, B13, 19);
		Mix(B6, B11, 10);
		Mix(B4, B15, 55);
		Mix(B10, B7, 49);
		Mix(B12, B3, 18);
		Mix(B14, B5, 23);
		Mix(B8, B1, 52);
		Mix(B0, B7, 33);
		Mix(B2, B5, 4);
		Mix(B4, B3, 51);
		Mix(B6, B1, 13);
		Mix(B12, B15, 34);
		Mix(B14, B13, 41);
		Mix(B8, B11, 59);
		Mix(B10, B9, 17);
		Mix(B0, B15, 5);
		Mix(B2, B11, 20);
		Mix(B6, B13, 48);
		Mix(B4, B9, 41);
		Mix(B14, B1, 47);
		Mix(B8, B5, 28);
		Mix(B10, B3, 16);
		Mix(B12, B7, 25);
		Inject(B0, B1, 41, K2, K3);
		Inject(B2, B3, 9, K4, K5);
		Inject(B4, B5, 37, K6, K7);
		Inject(B6, B7, 31, K8, K9);
		Inject(B8, B9, 12, K10, K11);
		Inject(B10, B11, 47, K12, K13);
		Inject(B12, B13, 44, K14, K15 + T1);
		Inject(B14, B15, 30, K16 + T2, K0 + 19);
		Mix(B0, B9, 16);
		Mix(B2, B13, 34);
		Mix(B6, B11, 56);
		Mix(B4, B15, 51);
		Mix(B10, B7, 4);
		Mix(B12, B3, 53);
		Mix(B14, B5, 42);
		Mix(B8, B1, 41);
		Mix(B0, B7, 31);
		Mix(B2, B5, 44);
		Mix(B4, B3, 47);
		Mix(B6, B1, 46);
		Mix(B12, B15, 19);
		Mix(B14, B13, 42);
		Mix(B8, B11, 44);
		Mix(B10, B9, 25);
		Mix(B0, B15, 9);
		Mix(B2, B11, 48);
		Mix(B6, B13, 35);
		Mix(B4, B9, 52);
		Mix(B14, B1, 23);
		Mix(B8, B5, 31);
		Mix(B10, B3, 37);
		Mix(B12, B7, 20);

		Output.S[0] = B0 + K3;
		Output.S[1] = B1 + K4;
		Output.S[2] = B2 + K5;
		Output.S[3] = B3 + K6;
		Output.S[4] = B4 + K7;
		Output.S[5] = B5 + K8;
		Output.S[6] = B6 + K9;
		Output.S[7] = B7 + K10;
		Output.S[8] = B8 + K11;
		Output.S[9] = B9 + K12;
		Output.S[10] = B10 + K13;
		Output.S[11] = B11 + K14;
		Output.S[12] = B12 + K15;
		Output.S[13] = B13 + K16 + T2;
		Output.S[14] = B14 + K0 + T0;
		Output.S[15] = B15 + K1 + 20;
	}
};

NAMESPACE_DIGESTEND
#endif