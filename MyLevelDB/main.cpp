#include <iostream>
using namespace std;
#include <stdio.h>
#include <endian.h>

char* EncodeVarint32(char* dst, uint32_t v)
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(dst);
	static const int B = 128;
	if (v < (1 << 7))//v<128
	{
		*(ptr++) = v;
	}
	else if (v < (1 << 14))
	{
		*(ptr++) = v | B;
		*(ptr++) = v >> 7;
	}
	else if (v < (1 << 21))
	{
		*(ptr++) = v | B;
		*(ptr++) = (v >> 7) | B;
		*(ptr++) = v >> 14;
	}
	else if (v < (1 << 28))
	{
		*(ptr++) = v | B;
		*(ptr++) = (v >> 7) | B;
		*(ptr++) = (v >> 14) | B;
		*(ptr++) = v >> 21;
	}
	else
	{
		*(ptr++) = v | B;
		*(ptr++) = (v >> 7) | B;
		*(ptr++) = (v >> 14) | B;
		*(ptr++) = (v >> 21) | B;
		*(ptr++) = v >> 28;
	}
	return reinterpret_cast<char*>(ptr);
}

void PutVarint32(std::string* dst, uint32_t v)
{
	char buf[5];
	char* ptr = EncodeVarint32(buf, v);
	dst->append(buf, ptr - buf);
}

int main()
{
	
	string dst("hello");
	PutVarint32(&dst, 120);

	cout << endl << "OK" << endl;
	system("Pause");
	return 0;
}