#include<iostream>
#include<cmath>
char *encode (char *plaintext, unsigned long key);
char *decode (char *cihpertext, unsigned long key);

void D_armour(int* encrypt, unsigned char h1, unsigned char h2, unsigned char h3, unsigned char h4, unsigned char h5);
void D_armour(int* encrypt, unsigned char h1, unsigned char h2, unsigned char h3, unsigned char h4, unsigned char h5)
{
	h1 = h1 - 33;
	h2 = h2 - 33;
	h3 = h3 - 33;
	h4 = h4 - 33;
	h5 = h5 - 33;

	unsigned long total = (h1*85*85*85);
	total = total*85;
	total = total + (h2*85*85*85) + (h3*85*85) + (h4*85) + h5;

	unsigned int first_encrypt_value = total>>24;
	unsigned int second_encrypt_value = total>>16;
	second_encrypt_value = second_encrypt_value - (first_encrypt_value<<8);
	unsigned int po = total>>8;
	po = po - (second_encrypt_value<<8) - (first_encrypt_value<<16);
	unsigned int p = total;
	p = p - (po<<8) - (second_encrypt_value<<16) - (first_encrypt_value<<24);

	encrypt[0] = first_encrypt_value;
	encrypt[1] = second_encrypt_value;
	encrypt[2] = po;
	encrypt[3] = p;
}

void armour(int a1, int a2, int a3, int a4, char* something);
void armour(int a1, int a2, int a3, int a4, char* something)
{
	long a1long = a1;
	long a2long = a2;
	long a3long = a3;
	long a4long = a4;

	a1long = a1long<<24;
	a2long = a2long<<16;
	a3long = a3long<<8;

	long resultant = a1long + a2long + a3long + a4long;
	int one = resultant%85;
	one = one + 33;
	something[4] = one;
	resultant = resultant/85;

	int two = resultant%85;
	two = two + 33;
	something[3] = two;
	resultant = resultant/85;

	int three = resultant%85;
	three = three + 33;
	something[2] = three;
	resultant = resultant/85;

	int four = resultant%85;
	four = four + 33;
	something[1] = four;
	resultant = resultant/85;

	int five = resultant%85;
	five = five + 33;
	something[0] = five;
	resultant = resultant/85;

}

char *encode( char *plaintext, unsigned long key )
{
	unsigned char S[256];
	int i=0;
	int j=0;
	unsigned char swap{};
	int z=0;
	char R;
	for(int zx=0; zx<256; zx++)
	{
		S[zx]=zx;
	}

	for(int l=0; l<256; l++)
	{
		int k= i%64;
		unsigned long key2 = key;
		int tobit = (key2>>k)&1;
		j = (j + S[i] + tobit)%256;
		swap = S[i];
		S[i] = S[j];
		S[j] = swap;
		i = (i+1)%256;
		key2=key;
	}

	while(plaintext[z]!='\0')
	{
		z++;
	}

	int ogZ=z;
	while(z%4!=0)
		z++;

	unsigned char *str = new unsigned char[z+1]{};
		for (int l = 0; l<ogZ; l++)
		{
			if(plaintext[l]<0)
			str[l] = (int)plaintext[l] + 256;
			else
			str[l] = (int)plaintext[l];
		}

	int r = 0;
	for(int kj=0; kj<z; kj++)
	{
		i=(i+1)%256;
		j = (j + S[i])%256;
		swap = S[i];
		S[i] = S[j];
		S[j] = swap;
		r = (S[i] + S[j])%256;
		R = S[r];
		str[kj] = str[kj]^R;
	}

	char *encode_return = new char[5*z/4 + 1]{};
	char *val = new char[5];

	for(int counter = 0; counter<z/4; counter++)
	{
		int d1=(int)str[4*counter];
		int d2=(int)str[4*counter +1];
		int d3=(int)str[4*counter +2];
		int d4=(int)str[4*counter +3];

		armour(d1, d2, d3, d4, val);

		encode_return[5*counter] = val[0];
		encode_return[5*counter + 1] = val[1];
		encode_return[5*counter + 2] = val[2];
		encode_return[5*counter + 3] = val[3];
		encode_return[5*counter + 4] = val[4];
	}
	return encode_return;
}

char *decode(char *cihpertext, unsigned long key)
{
	int decode_size=0;
	while(cihpertext[decode_size]!=0)
	{
		decode_size++;
	}
	unsigned char *strng = new unsigned char[decode_size+1]{};
	for (int l = 0; l<decode_size; l++)
			{
				if(cihpertext[l]<0)
				strng[l] = (int)cihpertext[l] + 256;
				else
				strng[l] = (int)cihpertext[l];
			}
	int sets = decode_size/5;
	int chars = sets*4;
	unsigned char *assign = new unsigned char[chars + 1]{};
	int *finalvalue = new int [4];
	for(int l = 0; l< sets; l++)
	{
		D_armour(finalvalue, strng[5*l], strng[5*l + 1], strng[5*l + 2], strng[5*l + 3], strng[5*l + 4]);
		assign[4*l]= (char) finalvalue[0];
		assign[4*l + 1] = (char) finalvalue[1];
		assign[4*l + 2]= (char) finalvalue[2];
		assign[4*l + 3] = (char) finalvalue[3];
	}

	unsigned char S[256];
		int i=0;
		int j=0;
		unsigned char swap{};
		char R;
		for(int zx=0; zx<256; zx++)
		{
			S[zx]=zx;
		}

		for(int l=0; l<256; l++)
		{
			int k= i%64;
			unsigned long key2 = key;
			int tobit = (key2>>k)&1;
			j = (j + S[i] + tobit)%256;
			swap = S[i];
			S[i] = S[j];
			S[j] = swap;
			i = (i+1)%256;
			key2=key;
		}

		int r = 0;
		for(int kj=0; kj<4*decode_size/5; kj++)
		{
			i=(i+1)%256;
			j = (j + S[i])%256;
			swap = S[i];
			S[i] = S[j];
			S[j] = swap;
			r = (S[i] + S[j])%256;
			R = S[r];
			assign[kj] = assign[kj]^R;
		}
		char *final_decoded_message = new char[chars+1];
		for(int CoUnTeR = 0; CoUnTeR<chars ;CoUnTeR++){
			int c = (int)assign[CoUnTeR];
			if(c>128)
				c = c-256;
			final_decoded_message[CoUnTeR]=(char)c;
		}
	return final_decoded_message;
}

#ifndef MARMOSET_TESTING
int main() {
	char str0[]{"hello"};
	char *ciphertext{ encode( str0, 51323 ) };
	std::cout << "\"" << ciphertext << "\"" << std::endl;
	char *plaintext{ decode( str0, 89963221 ) };
	std::cout << "\"" << plaintext << "\"" << std::endl;
	/*int v1 = 0b10110101;
	int v2 = 0b01110100;
	int v3 = 0b01010101;
	int v4 = 0b10011110;

	char *res = new char[5];

	armour(v1,v2,v3,v4,res);
	std::cout << res;

	/*unsigned char* c = new unsigned char[5];
	int *a  = new int[10]{};
	D_armour(a,'[','<',',','E','?');*/

	/*int* returner=new int[4];
	unsigned char a='l';
	unsigned char b='=';
	unsigned char c=']';
	unsigned char d='V';
	unsigned char e=']';
	D_armour(returner,a,b,c,d,e);

	std::cout << returner[0] << "\t" << returner[1] << "\t" << returner[2] << "\t" << returner[3] << "\t";*/
	//char str0[]{"l=]V]&9!>trOu*9"};
	//char *c{decode(str0,51323)};
	//std::cout<<c;
	return 0;
}
#endif
