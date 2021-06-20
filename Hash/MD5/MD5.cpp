#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

#define byte unsigned char
#define int32 unsigned int
#define int64 unsigned long long


class MD5 {
private:
	string text;
	int64 textLength;
	vector <byte> buf;
	int32 A, B, C, D;
	int32 T[64];
	int32* words;

private:
	// Функции для вычисления
	void flowAlign(vector<byte> &buf) {
		// Добавляем к потоку единичный бит
		buf.push_back(128);
		
		while (buf.size() % 64 != 56)
			buf.push_back(0);
	}
	void addTextLen(vector<byte>& buf, int64 textLength) {
		for (int i = 7; i >= 0; i--)
			buf.push_back(byte((textLength >> (i * 8) & 0xFF)));
	}
	void initBuffer() {
		A = 0x67452301;
		B = 0xEFCDAB89;
		C = 0x98BADCFE;
		D = 0x10325476;

		for (int i = 0; i < 64; i++)
			T[i] = int32(pow(2, 32) * abs(sin(double(i+1))));
	}
	int32 rotateLeft(int32 x, int32 n) {
		return ((x << n) | (x >> (32 - n)));
	}
	int conv512to32(vector<byte>& buf, int32*& words) {
		auto wSize = buf.size();
		words = new int32[wSize / 4];

		for (auto i = 0, j = 0; i < wSize; i += 4, j = i / 4) {
			words[j] = 0;
			words[j] += int32(buf[i]);
			words[j] += int32(buf[i + 1]) << 8;
			words[j] += int32(buf[i + 2]) << 16;
			words[j] += int32(buf[i + 3]) << 24;
		}

		return wSize / 4;
	}
	void calcRounds(int32* buf, int textLength) {
		for (auto i = 0; i < textLength; i += 16) {
			auto AA = A, BB = B, CC = C, DD = D;

			//ROUND I

			A = B + rotateLeft((A + funcF(B, C, D) + buf[i + 0] + T[0]), 7);
			D = A + rotateLeft((D + funcF(A, B, C) + buf[i + 1] + T[1]), 12);
			C = D + rotateLeft((C + funcF(D, A, B) + buf[i + 2] + T[2]), 17);
			B = C + rotateLeft((B + funcF(C, D, A) + buf[i + 3] + T[3]), 22);

			A = B + rotateLeft((A + funcF(B, C, D) + buf[i + 4] + T[4]), 7);
			D = A + rotateLeft((D + funcF(A, B, C) + buf[i + 5] + T[5]), 12);
			C = D + rotateLeft((C + funcF(D, A, B) + buf[i + 6] + T[6]), 17);
			B = C + rotateLeft((B + funcF(C, D, A) + buf[i + 7] + T[7]), 22);

			A = B + rotateLeft((A + funcF(B, C, D) + buf[i + 8] + T[8]), 7);
			D = A + rotateLeft((D + funcF(A, B, C) + buf[i + 9] + T[9]), 12);
			C = D + rotateLeft((C + funcF(D, A, B) + buf[i + 10] + T[10]), 17);
			B = C + rotateLeft((B + funcF(C, D, A) + buf[i + 11] + T[11]), 22);

			A = B + rotateLeft((A + funcF(B, C, D) + buf[i + 12] + T[12]), 7);
			D = A + rotateLeft((D + funcF(A, B, C) + buf[i + 13] + T[13]), 12);
			C = D + rotateLeft((C + funcF(D, A, B) + buf[i + 14] + T[14]), 17);
			B = C + rotateLeft((B + funcF(C, D, A) + buf[i + 15] + T[15]), 22);

			//ROUND II

			A = B + rotateLeft((A + funcG(B, C, D) + buf[i + 1] + T[16]), 5);
			D = A + rotateLeft((D + funcG(A, B, C) + buf[i + 6] + T[17]), 9);
			C = D + rotateLeft((C + funcG(D, A, B) + buf[i + 11] + T[18]), 14);
			B = C + rotateLeft((B + funcG(C, D, A) + buf[i + 0] + T[19]), 20);

			A = B + rotateLeft((A + funcG(B, C, D) + buf[i + 5] + T[20]), 5);
			D = A + rotateLeft((D + funcG(A, B, C) + buf[i + 10] + T[21]), 9);
			C = D + rotateLeft((C + funcG(D, A, B) + buf[i + 15] + T[22]), 14);
			B = C + rotateLeft((B + funcG(C, D, A) + buf[i + 4] + T[23]), 20);

			A = B + rotateLeft((A + funcG(B, C, D) + buf[i + 9] + T[24]), 5);
			D = A + rotateLeft((D + funcG(A, B, C) + buf[i + 14] + T[25]), 9);
			C = D + rotateLeft((C + funcG(D, A, B) + buf[i + 3] + T[26]), 14);
			B = C + rotateLeft((B + funcG(C, D, A) + buf[i + 8] + T[27]), 20);

			A = B + rotateLeft((A + funcG(B, C, D) + buf[i + 13] + T[28]), 5);
			D = A + rotateLeft((D + funcG(A, B, C) + buf[i + 2] + T[29]), 9);
			C = D + rotateLeft((C + funcG(D, A, B) + buf[i + 7] + T[30]), 14);
			B = C + rotateLeft((B + funcG(C, D, A) + buf[i + 12] + T[31]), 20);

			//ROUND III

			A = B + rotateLeft((A + funcH(B, C, D) + buf[i + 5] + T[32]), 4);
			D = A + rotateLeft((D + funcH(A, B, C) + buf[i + 8] + T[33]), 11);
			C = D + rotateLeft((C + funcH(D, A, B) + buf[i + 11] + T[34]), 16);
			B = C + rotateLeft((B + funcH(C, D, A) + buf[i + 14] + T[35]), 23);

			A = B + rotateLeft((A + funcH(B, C, D) + buf[i + 1] + T[36]), 4);
			D = A + rotateLeft((D + funcH(A, B, C) + buf[i + 4] + T[37]), 11);
			C = D + rotateLeft((C + funcH(D, A, B) + buf[i + 7] + T[38]), 16);
			B = C + rotateLeft((B + funcH(C, D, A) + buf[i + 10] + T[39]), 23);

			A = B + rotateLeft((A + funcH(B, C, D) + buf[i + 13] + T[40]), 4);
			D = A + rotateLeft((D + funcH(A, B, C) + buf[i + 0] + T[41]), 11);
			C = D + rotateLeft((C + funcH(D, A, B) + buf[i + 3] + T[42]), 16);
			B = C + rotateLeft((B + funcH(C, D, A) + buf[i + 6] + T[43]), 23);

			A = B + rotateLeft((A + funcH(B, C, D) + buf[i + 9] + T[44]), 4);
			D = A + rotateLeft((D + funcH(A, B, C) + buf[i + 12] + T[45]), 11);
			C = D + rotateLeft((C + funcH(D, A, B) + buf[i + 15] + T[46]), 16);
			B = C + rotateLeft((B + funcH(C, D, A) + buf[i + 2] + T[47]), 23);

			//ROUND IV

			A = B + rotateLeft((A + funcI(B, C, D) + buf[i + 0] + T[48]), 6);
			D = A + rotateLeft((D + funcI(A, B, C) + buf[i + 7] + T[49]), 10);
			C = D + rotateLeft((C + funcI(D, A, B) + buf[i + 14] + T[50]), 15);
			B = C + rotateLeft((B + funcI(C, D, A) + buf[i + 5] + T[51]), 21);

			A = B + rotateLeft((A + funcI(B, C, D) + buf[i + 12] + T[52]), 6);
			D = A + rotateLeft((D + funcI(A, B, C) + buf[i + 3] + T[53]), 10);
			C = D + rotateLeft((C + funcI(D, A, B) + buf[i + 10] + T[54]), 15);
			B = C + rotateLeft((B + funcI(C, D, A) + buf[i + 1] + T[55]), 21);

			A = B + rotateLeft((A + funcI(B, C, D) + buf[i + 8] + T[56]), 6);
			D = A + rotateLeft((D + funcI(A, B, C) + buf[i + 15] + T[57]), 10);
			C = D + rotateLeft((C + funcI(D, A, B) + buf[i + 6] + T[58]), 15);
			B = C + rotateLeft((B + funcI(C, D, A) + buf[i + 13] + T[59]), 21);

			A = B + rotateLeft((A + funcI(B, C, D) + buf[i + 4] + T[60]), 6);
			D = A + rotateLeft((D + funcI(A, B, C) + buf[i + 11] + T[61]), 10);
			C = D + rotateLeft((C + funcI(D, A, B) + buf[i + 2] + T[62]), 15);
			B = C + rotateLeft((B + funcI(C, D, A) + buf[i + 9] + T[63]), 21);

			A += AA; B += BB; C += CC; D += DD;
		}
	}
	string hex(int32 num) {
		stringstream stream;

		for (auto i = 0; i < 4; i++) {
			stream << setfill('0') << std::setw(2) << std::hex << num % 256;
			num /= 256;
		}
		return stream.str();
	}

private:
	// Логические функции
	int32 funcF(int32 x, int32 y, int32 z) { return ((x & y) | (~x & z)); }
	int32 funcG(int32 x, int32 y, int32 z) { return ((x & z) | (~z & y)); }
	int32 funcH(int32 x, int32 y, int32 z) { return (x ^ y ^ z); }
	int32 funcI(int32 x, int32 y, int32 z) { return (y ^ (~z | x)); }

public:
	MD5(string text) {
		this->text = text;
		textLength = text.length();

		buf.resize(textLength);
	}

	string hash() {		
		// Инициализация вектора
		for (int i = 0; i < textLength; i++)
			buf[i] = text[i];

		// Делаем порядок байт little-endian
		textLength = _byteswap_uint64(textLength * 8);

		// Выравнивание потока
		flowAlign(buf);
		// Добавление длины сообщения
		addTextLen(buf, textLength);
		// Инициализируем буффер
		initBuffer();
		// Вычисляем раунды
		calcRounds(words, conv512to32(buf, words));

		return hex(A) + hex(B) + hex(C) + hex(D);
	}
};