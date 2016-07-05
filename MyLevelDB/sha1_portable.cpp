#include "../MyLevelDB/sha1_portable.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

namespace MyLevelDB
{
	namespace port
	{
		typedef struct SHA1Context
		{
			unsigned Message_Digest[5];

			unsigned Length_Low;
			unsigned Length_High;

			unsigned char Message_Block[64];//512-bit 
			int Message_Block_Index;

			bool Computed;
			bool Corrupted;

		} SHA1Context;

		//circular shift macro

#define SHA1CircularShift(bits,word) \
		((((word)<<(bits)) & 0xFFFFFFFF) | ((word)>>(32-(bits))))

		static void SHA1ProcessMessageBlock(SHA1Context*);
		static void SHA1PadMessage(SHA1Context*);

		static void SHA1Reset(SHA1Context* context)
		{
			context->Length_Low = 0;
			context->Length_High = 0;
			context->Message_Block_Index = 0;

			context->Message_Digest[0] = 0x67452301;
			context->Message_Digest[1] = 0xEFCDAB89;
			context->Message_Digest[2] = 0x98BADCFE;
			context->Message_Digest[3] = 0x10325476;
			context->Message_Digest[4] = 0xC3D2E1F0;

			context->Computed = false;
			context->Corrupted = false;

		}

		static bool SHA1Result(SHA1Context* context)
		{
			if (context->Corrupted) return false;
			if (!context->Computed)
			{
				SHA1PadMessage(context);
				context->Computed = true;
			}
			return true;
		}

		static void SHA1Input(SHA1Context* context,
			const unsigned char* message_arry,
			unsigned length)
		{
			if (!length) return;
			if (context->Computed || context->Corrupted)
			{
				context->Corrupted = true;
				return;
			}
			while (length-- && !context->Corrupted)
			{
				context->Message_Block[context->Message_Block_Index++] =
					(*message_arry & 0xFF);

				context->Length_Low += 8;
				context->Length_Low &= 0xFFFFFFFF;
				if (context->Length_Low == 0)
				{
					context->Length_High++;
					context->Length_High &= 0xFFFFFFFF;
				
				if (context->Length_High == 0)
					context->Corrupted = true;//messageÌ«³¤
				}

			if (context->Message_Block_Index == 64)
				SHA1ProcessMessageBlock(context);
			message_arry++;
			}
		}

		static void SHA1ProcessMessageBlock(SHA1Context* context)
		{
			const unsigned K[] =
			{
				0x5A827999,
				0x6ED9EBA1,
				0x8F1BBCDC,
				0xCA62C1D6
			};
			int t;
			unsigned temp;
			unsigned W[80];
			unsigned A, B, C, D, E;

			for (t = 0; t < 16; ++t)
			{
				W[t] = ((unsigned)context->Message_Block[t * 4] << 24);
				W[t] |= ((unsigned)context->Message_Block[t * 4 + 1] << 16);
				W[t] |= ((unsigned)context->Message_Block[t * 4 + 2] << 8);
				W[t] |= ((unsigned)context->Message_Block[t * 4 + 3]);
			}
			for (t = 16; t < 80; ++t)
			{
				W[t] = SHA1CircularShift(1, 
					W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
			}
			A = context->Message_Digest[0];
			B = context->Message_Digest[1];
			C = context->Message_Digest[2];
			D = context->Message_Digest[3];
			E = context->Message_Digest[4];

			for (t = 0; t < 20; t++)
			{
				temp = SHA1CircularShift(5, A) +
					((B&C) | ((~B)&D) )+ E + W[t] + K[0];
				temp &= 0xFFFFFFFF;
				E = D;
				D = C;
				C = SHA1CircularShift(30, B);
				B = A;
				A = temp;
			}

			for (t = 20; t < 40; t++)
			{
				temp = SHA1CircularShift(5, A) + (B^C^D) + E + W[t] + K[1];
				temp &= 0xFFFFFFFF;
				E = D;
				D = C;
				C = SHA1CircularShift(30, B);
				B = A;
				A = temp;
			}
			for (t = 40; t < 60; t++)
			{
				temp = SHA1CircularShift(5, A) +
					(B^C^D) + E + W[t] + K[1];
				temp &= 0xFFFFFFFF;
				E = D;
				D = C;
				C = SHA1CircularShift(30, B);
				B = A;
				A = temp;
			}
			for (t = 60; t < 80; t++)
			{
				temp = SHA1CircularShift(5, A)
					+ (B^C^D) + E + W[t] + K[3];
				temp &= 0xFFFFFFFF;
				E = D;
				D = C;
				C = SHA1CircularShift(30, B);
				B = A;
				A = temp;
			}
			context->Message_Digest[0] = (context->Message_Digest[0] + A) & 0xFFFFFFFF;
			context->Message_Digest[1] = (context->Message_Digest[1] + B) & 0xFFFFFFFF;
			context->Message_Digest[2] = (context->Message_Digest[2] + C) & 0xFFFFFFFF;
			context->Message_Digest[3] = (context->Message_Digest[3] + D) & 0xFFFFFFFF;
			context->Message_Digest[4] = (context->Message_Digest[4] + E) & 0xFFFFFFFF;

			context->Message_Block_Index = 0;
		}

		static void SHA1PadMessage(SHA1Context* context)
		{
			if (context->Message_Block_Index > 55)
			{
				context->Message_Block[context->Message_Block_Index++] = 0x80;
				while (context->Message_Block_Index < 64)
					context->Message_Block[context->Message_Block_Index++] = 0;
			
			SHA1ProcessMessageBlock(context);

			while (context->Message_Block_Index < 56)
				context->Message_Block[context->Message_Block_Index++] = 0;
			}
			else
			{
				context->Message_Block[context->Message_Block_Index++] = 0x80;
				while (context->Message_Block_Index < 56)
					context->Message_Block[context->Message_Block_Index++] = 0;
			}

			context->Message_Block[56] = (context->Length_High >> 24) & 0xFF;
			context->Message_Block[57] = (context->Length_High >> 16) & 0xFF;
			context->Message_Block[58] = (context->Length_High >> 8) & 0xFF;
			context->Message_Block[59] = (context->Length_High) & 0xFF;
			context->Message_Block[60] = (context->Length_Low >> 24) & 0xFF;
			context->Message_Block[61] = (context->Length_Low >> 16) & 0xFF;
			context->Message_Block[62] = (context->Length_Low >> 8) & 0xFF;
			context->Message_Block[63] = (context->Length_Low ) & 0xFF;

			SHA1ProcessMessageBlock(context);
		}

		void SHA1_Hash_Portble(const char* data, size_t len, char* hash_arry)
		{
			SHA1Context context;
			SHA1Reset(&context);
			SHA1Input(&context, reinterpret_cast<const unsigned char*>(data), len);
			bool ok = SHA1Result(&context);
			if (!ok)
			{
				fprintf(stderr, "Unexpected error in SHA1_Hash_Portable code\n");
				exit(1);
			}
			for (int i = 0; i < 5; ++i)
			{
				uint32_t value = context.Message_Digest[i];
				hash_arry[i * 4 + 0] = (value >> 24) & 0xFF;
				hash_arry[i * 4 + 1] = (value >> 16) & 0xFF;
				hash_arry[i * 4 + 2] = (value >> 8) & 0xFF;
				hash_arry[i * 4 + 3] = value & 0xFF;
			}
		}






	}
}