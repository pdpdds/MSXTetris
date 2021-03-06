// From: https://www.electro-tech-online.com/threads/ultra-fast-pseudorandom-number-generator-for-8-bit.124249/


//X ABC Algorithm Random Number Generator for 8-Bit Devices:
//This is a small PRNG, experimentally verified to have at least a 50 million byte period
//by generating 50 million bytes and observing that there were no overapping sequences and repeats.
//This generator passes serial correlation, entropy , Monte Carlo Pi value, arithmetic mean,
//And many other statistical tests. This generator may have a period of up to 2^32, but this has
//not been verified.
//
// By XORing 3 bytes into the a,b, and c registers, you can add in entropy from 
//an external source easily.
//
//This generator is free to use, but is not suitable for cryptography due to its short period(by //cryptographic standards) and simple construction. No attempt was made to make this generator 
// suitable for cryptographic use.
//
//Due to the use of a constant counter, the generator should be resistant to latching up.
//A significant performance gain is had in that the x variable is nly ever incremented.
//
//Only 4 bytes of ram are needed for the internal state, and generating a byte requires 3 XORs , //2 ADDs, one bit shift right , and one increment. Difficult or slow operations like multiply, etc 
//were avoided for maximum speed on ultra low power devices.


char Rand_a, Rand_b, Rand_c, Rand_x;


//Can also be used to seed the rng with more entropy during use.
void InitRnd(char s1, char s2, char s3) {
  //XOR new entropy into key state
  Rand_a ^= s1;
  Rand_b ^= s2;
  Rand_c ^= s3;

  Rand_x++;
  Rand_a = (Rand_a ^ Rand_c ^ Rand_x);
  Rand_b = (Rand_b + Rand_a);
  Rand_c = (Rand_c + (Rand_b >> 1) ^ Rand_a);
}

unsigned char GetRandom() {
  Rand_x++;               			//x is incremented every round and is not affected by any other variable
  Rand_a = (Rand_a ^ Rand_c ^ Rand_x);       	//note the mix of addition and XOR
  Rand_b = (Rand_b + Rand_a);         		//And the use of very few instructions
  Rand_c = (Rand_c + (Rand_b >> 1) ^ Rand_a);  	//the right shift is to ensure that high-order bits from b can affect  

  return (Rand_c);          //low order bits of other variables
}

// Functions made by me:

byte GetRandomInInterval(byte max, byte mask) {
  byte random;
  do {
     random = GetRandom() & mask; // AND mask to get a value in the range passed by mask. E.g. 0b00000011, range 0-3
  }
  while(random > max); // Get a number between 0 and max

  return random;
}
