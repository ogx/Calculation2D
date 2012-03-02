#include "mmCodecBase64.h"

namespace base64 {

	// encoding lookup table
	static const char enlut[] = 
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	// decoding lookup table
	// first field after lazy init should equal to enlut size
	static unsigned char delut[256] = {!sizeof(enlut)};

	// dynamic lazy reinitialization for decoding
	// LUT table
	void delut_init() {
		if (!*delut) for(int i = 0; i < sizeof(enlut);) delut[enlut[i]] = ++i;
	}

	union cryptex {

	private: struct { unsigned e:8, d:6, c:6, b:6, a:6;}  ;//struct4x6bit
	public: struct  { unsigned char         D, C, B, A;}  ;//struct3x8bit

	public:

		inline void blow() {
			D = enlut[d];
			C = enlut[c];
			B = enlut[b];
			A = enlut[a];
		}

		inline void suck() {
			a = delut[A];
			b = delut[B];
			c = delut[C];
			d = delut[D];
		}
	};

	std::vector<unsigned char> decode(const std::string& input) {

		std::vector<unsigned char> output; // hoping for RVO
		if (input.empty()) return output;

		if (input.size() % 4) throw; //padding is mandatory!

		delut_init(); //lazy init or cache

		output.resize(3*input.size()/4);

		const register char* ptr = &(input.front());
		const register char* const last = &(input.back());
		register unsigned char* optr = &(output.front());
		register cryptex c;

		--ptr; --optr;

		while (ptr != last) {
			c.A = *++ptr;
			c.B = *++ptr;
			c.C = *++ptr;
			c.D = *++ptr;
			c.suck();
			*++optr = c.A;
			*++optr = c.B;
			*++optr = c.C;
		}

		auto tail = input.end();
		if ('='==*--tail) output.resize(output.size() - 1);
		if ('='==*--tail) output.resize(output.size() - 1);

		return output;
	}

	std::string encode(std::vector<unsigned char>& input) {

		std::string output; // hoping for RVO
		if(input.empty()) return output; 

		std::size_t sz = input.size();

		while(input.size() % 3) input.push_back(0);

		output.resize(input.size()*4/3);


		const register unsigned char *ptr = &(input.front());
		const register unsigned char *const last = &(input.back());
		register char* optr = &(output.front());
		register cryptex c;

		--ptr; --optr;

		while (ptr != last ) {
			c.A = *++ptr;
			c.B = *++ptr;
			c.C = *++ptr;
			c.blow();
			*++optr = c.A;
			*++optr = c.B;
			*++optr = c.C;
			*++optr = c.D;
		}

		auto tail = output.end();
		while (sz++ % 3) *(--tail) = '=';

		return output;
	}

}
