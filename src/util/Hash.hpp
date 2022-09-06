#ifndef PAQ8PX_HASH_HPP
#define PAQ8PX_HASH_HPP

#include "utils.hpp"
#include <cassert>
#include <cstdint>
#include <cstdio>

//////////////////////// Hash functions //////////////////////////
//
// All hash functions are constructed using multiplicative hashes
// - We usually hash small values
// - Multiplicative hashes promote entropy to the higher bits
// - When combining ( H(x) + H(y) ) entropy is still in higher bits
// - After combining they must be finalized by taking the higher
//   bits only to reduce the range to the desired hash table size

// Multipliers
// - They don't need to be prime, just large odd numbers
// - The golden ratio is usually preferred as a multiplier (PHI64)

#ifdef HASHCONFIGCMD
#define HASHEXPR
#else
#define HASHEXPR constexpr
#endif

static HASHEXPR uint64_t hashes[14] = {UINT64_C(0x9E3779B97F4A7C15), UINT64_C(0x993DDEFFB1462949), UINT64_C(0xE9C91DC159AB0D2D),
                                       UINT64_C(0x83D6A14F1B0CED73), UINT64_C(0xA14F1B0CED5A841F), UINT64_C(0xC0E51314A614F4EF),
                                       UINT64_C(0xDA9CC2600AE45A27), UINT64_C(0x826797AA04A65737), UINT64_C(0x2375BE54C41A08ED),
                                       UINT64_C(0xD39104E950564B37), UINT64_C(0x3091697D5E685623), UINT64_C(0x20EB84EE04A3C7E1),
                                       UINT64_C(0xF501F1D0944B2383), UINT64_C(0xE3E4E8AA829AB9B5)};
// static HASHEXPR uint64_t hashes[14] = {UINT64_C(1185921), UINT64_C(35937), UINT64_C(1089),
//                                        UINT64_C(33), UINT64_C(1), UINT64_C(1),
//                                        UINT64_C(1), UINT64_C(1), UINT64_C(1),
//                                        UINT64_C(1), UINT64_C(1), UINT64_C(1),
//                                        UINT64_C(1), UINT64_C(1)};
static HASHEXPR uint64_t crc64_tab[256] = {
  0x0000000000000000ULL, 0x7ad870c830358979ULL,
  0xf5b0e190606b12f2ULL, 0x8f689158505e9b8bULL,
  0xc038e5739841b68fULL, 0xbae095bba8743ff6ULL,
  0x358804e3f82aa47dULL, 0x4f50742bc81f2d04ULL,
  0xab28ecb46814fe75ULL, 0xd1f09c7c5821770cULL,
  0x5e980d24087fec87ULL, 0x24407dec384a65feULL,
  0x6b1009c7f05548faULL, 0x11c8790fc060c183ULL,
  0x9ea0e857903e5a08ULL, 0xe478989fa00bd371ULL,
  0x7d08ff3b88be6f81ULL, 0x07d08ff3b88be6f8ULL,
  0x88b81eabe8d57d73ULL, 0xf2606e63d8e0f40aULL,
  0xbd301a4810ffd90eULL, 0xc7e86a8020ca5077ULL,
  0x4880fbd87094cbfcULL, 0x32588b1040a14285ULL,
  0xd620138fe0aa91f4ULL, 0xacf86347d09f188dULL,
  0x2390f21f80c18306ULL, 0x594882d7b0f40a7fULL,
  0x1618f6fc78eb277bULL, 0x6cc0863448deae02ULL,
  0xe3a8176c18803589ULL, 0x997067a428b5bcf0ULL,
  0xfa11fe77117cdf02ULL, 0x80c98ebf2149567bULL,
  0x0fa11fe77117cdf0ULL, 0x75796f2f41224489ULL,
  0x3a291b04893d698dULL, 0x40f16bccb908e0f4ULL,
  0xcf99fa94e9567b7fULL, 0xb5418a5cd963f206ULL,
  0x513912c379682177ULL, 0x2be1620b495da80eULL,
  0xa489f35319033385ULL, 0xde51839b2936bafcULL,
  0x9101f7b0e12997f8ULL, 0xebd98778d11c1e81ULL,
  0x64b116208142850aULL, 0x1e6966e8b1770c73ULL,
  0x8719014c99c2b083ULL, 0xfdc17184a9f739faULL,
  0x72a9e0dcf9a9a271ULL, 0x08719014c99c2b08ULL,
  0x4721e43f0183060cULL, 0x3df994f731b68f75ULL,
  0xb29105af61e814feULL, 0xc849756751dd9d87ULL,
  0x2c31edf8f1d64ef6ULL, 0x56e99d30c1e3c78fULL,
  0xd9810c6891bd5c04ULL, 0xa3597ca0a188d57dULL,
  0xec09088b6997f879ULL, 0x96d1784359a27100ULL,
  0x19b9e91b09fcea8bULL, 0x636199d339c963f2ULL,
  0xdf7adabd7a6e2d6fULL, 0xa5a2aa754a5ba416ULL,
  0x2aca3b2d1a053f9dULL, 0x50124be52a30b6e4ULL,
  0x1f423fcee22f9be0ULL, 0x659a4f06d21a1299ULL,
  0xeaf2de5e82448912ULL, 0x902aae96b271006bULL,
  0x74523609127ad31aULL, 0x0e8a46c1224f5a63ULL,
  0x81e2d7997211c1e8ULL, 0xfb3aa75142244891ULL,
  0xb46ad37a8a3b6595ULL, 0xceb2a3b2ba0eececULL,
  0x41da32eaea507767ULL, 0x3b024222da65fe1eULL,
  0xa2722586f2d042eeULL, 0xd8aa554ec2e5cb97ULL,
  0x57c2c41692bb501cULL, 0x2d1ab4dea28ed965ULL,
  0x624ac0f56a91f461ULL, 0x1892b03d5aa47d18ULL,
  0x97fa21650afae693ULL, 0xed2251ad3acf6feaULL,
  0x095ac9329ac4bc9bULL, 0x7382b9faaaf135e2ULL,
  0xfcea28a2faafae69ULL, 0x8632586aca9a2710ULL,
  0xc9622c4102850a14ULL, 0xb3ba5c8932b0836dULL,
  0x3cd2cdd162ee18e6ULL, 0x460abd1952db919fULL,
  0x256b24ca6b12f26dULL, 0x5fb354025b277b14ULL,
  0xd0dbc55a0b79e09fULL, 0xaa03b5923b4c69e6ULL,
  0xe553c1b9f35344e2ULL, 0x9f8bb171c366cd9bULL,
  0x10e3202993385610ULL, 0x6a3b50e1a30ddf69ULL,
  0x8e43c87e03060c18ULL, 0xf49bb8b633338561ULL,
  0x7bf329ee636d1eeaULL, 0x012b592653589793ULL,
  0x4e7b2d0d9b47ba97ULL, 0x34a35dc5ab7233eeULL,
  0xbbcbcc9dfb2ca865ULL, 0xc113bc55cb19211cULL,
  0x5863dbf1e3ac9decULL, 0x22bbab39d3991495ULL,
  0xadd33a6183c78f1eULL, 0xd70b4aa9b3f20667ULL,
  0x985b3e827bed2b63ULL, 0xe2834e4a4bd8a21aULL,
  0x6debdf121b863991ULL, 0x1733afda2bb3b0e8ULL,
  0xf34b37458bb86399ULL, 0x8993478dbb8deae0ULL,
  0x06fbd6d5ebd3716bULL, 0x7c23a61ddbe6f812ULL,
  0x3373d23613f9d516ULL, 0x49aba2fe23cc5c6fULL,
  0xc6c333a67392c7e4ULL, 0xbc1b436e43a74e9dULL,
  0x95ac9329ac4bc9b5ULL, 0xef74e3e19c7e40ccULL,
  0x601c72b9cc20db47ULL, 0x1ac40271fc15523eULL,
  0x5594765a340a7f3aULL, 0x2f4c0692043ff643ULL,
  0xa02497ca54616dc8ULL, 0xdafce7026454e4b1ULL,
  0x3e847f9dc45f37c0ULL, 0x445c0f55f46abeb9ULL,
  0xcb349e0da4342532ULL, 0xb1eceec59401ac4bULL,
  0xfebc9aee5c1e814fULL, 0x8464ea266c2b0836ULL,
  0x0b0c7b7e3c7593bdULL, 0x71d40bb60c401ac4ULL,
  0xe8a46c1224f5a634ULL, 0x927c1cda14c02f4dULL,
  0x1d148d82449eb4c6ULL, 0x67ccfd4a74ab3dbfULL,
  0x289c8961bcb410bbULL, 0x5244f9a98c8199c2ULL,
  0xdd2c68f1dcdf0249ULL, 0xa7f41839ecea8b30ULL,
  0x438c80a64ce15841ULL, 0x3954f06e7cd4d138ULL,
  0xb63c61362c8a4ab3ULL, 0xcce411fe1cbfc3caULL,
  0x83b465d5d4a0eeceULL, 0xf96c151de49567b7ULL,
  0x76048445b4cbfc3cULL, 0x0cdcf48d84fe7545ULL,
  0x6fbd6d5ebd3716b7ULL, 0x15651d968d029fceULL,
  0x9a0d8ccedd5c0445ULL, 0xe0d5fc06ed698d3cULL,
  0xaf85882d2576a038ULL, 0xd55df8e515432941ULL,
  0x5a3569bd451db2caULL, 0x20ed197575283bb3ULL,
  0xc49581ead523e8c2ULL, 0xbe4df122e51661bbULL,
  0x3125607ab548fa30ULL, 0x4bfd10b2857d7349ULL,
  0x04ad64994d625e4dULL, 0x7e7514517d57d734ULL,
  0xf11d85092d094cbfULL, 0x8bc5f5c11d3cc5c6ULL,
  0x12b5926535897936ULL, 0x686de2ad05bcf04fULL,
  0xe70573f555e26bc4ULL, 0x9ddd033d65d7e2bdULL,
  0xd28d7716adc8cfb9ULL, 0xa85507de9dfd46c0ULL,
  0x273d9686cda3dd4bULL, 0x5de5e64efd965432ULL,
  0xb99d7ed15d9d8743ULL, 0xc3450e196da80e3aULL,
  0x4c2d9f413df695b1ULL, 0x36f5ef890dc31cc8ULL,
  0x79a59ba2c5dc31ccULL, 0x037deb6af5e9b8b5ULL,
  0x8c157a32a5b7233eULL, 0xf6cd0afa9582aa47ULL,
  0x4ad64994d625e4daULL, 0x300e395ce6106da3ULL,
  0xbf66a804b64ef628ULL, 0xc5bed8cc867b7f51ULL,
  0x8aeeace74e645255ULL, 0xf036dc2f7e51db2cULL,
  0x7f5e4d772e0f40a7ULL, 0x05863dbf1e3ac9deULL,
  0xe1fea520be311aafULL, 0x9b26d5e88e0493d6ULL,
  0x144e44b0de5a085dULL, 0x6e963478ee6f8124ULL,
  0x21c640532670ac20ULL, 0x5b1e309b16452559ULL,
  0xd476a1c3461bbed2ULL, 0xaeaed10b762e37abULL,
  0x37deb6af5e9b8b5bULL, 0x4d06c6676eae0222ULL,
  0xc26e573f3ef099a9ULL, 0xb8b627f70ec510d0ULL,
  0xf7e653dcc6da3dd4ULL, 0x8d3e2314f6efb4adULL,
  0x0256b24ca6b12f26ULL, 0x788ec2849684a65fULL,
  0x9cf65a1b368f752eULL, 0xe62e2ad306bafc57ULL,
  0x6946bb8b56e467dcULL, 0x139ecb4366d1eea5ULL,
  0x5ccebf68aecec3a1ULL, 0x2616cfa09efb4ad8ULL,
  0xa97e5ef8cea5d153ULL, 0xd3a62e30fe90582aULL,
  0xb0c7b7e3c7593bd8ULL, 0xca1fc72bf76cb2a1ULL,
  0x45775673a732292aULL, 0x3faf26bb9707a053ULL,
  0x70ff52905f188d57ULL, 0x0a2722586f2d042eULL,
  0x854fb3003f739fa5ULL, 0xff97c3c80f4616dcULL,
  0x1bef5b57af4dc5adULL, 0x61372b9f9f784cd4ULL,
  0xee5fbac7cf26d75fULL, 0x9487ca0fff135e26ULL,
  0xdbd7be24370c7322ULL, 0xa10fceec0739fa5bULL,
  0x2e675fb4576761d0ULL, 0x54bf2f7c6752e8a9ULL,
  0xcdcf48d84fe75459ULL, 0xb71738107fd2dd20ULL,
  0x387fa9482f8c46abULL, 0x42a7d9801fb9cfd2ULL,
  0x0df7adabd7a6e2d6ULL, 0x772fdd63e7936bafULL,
  0xf8474c3bb7cdf024ULL, 0x829f3cf387f8795dULL,
  0x66e7a46c27f3aa2cULL, 0x1c3fd4a417c62355ULL,
  0x935745fc4798b8deULL, 0xe98f353477ad31a7ULL,
  0xa6df411fbfb21ca3ULL, 0xdc0731d78f8795daULL,
  0x536fa08fdfd90e51ULL, 0x29b7d047efec8728ULL,
};


#ifdef HASHCONFIGCMD

static void loadHashesFromCmd(const char *hashesFromCommandline) {
  if( strlen(hashesFromCommandline) != 16 * 14 + 13 /*237*/) {
    quit("Bad hash config.");
  }
  for( int i = 0; i < 14; i++ ) { // for each specified hash value
    uint64_t hashVal = 0;
    for( int j = 0; j < 16; j++ ) { // for each hex char
      uint8_t c = hashesFromCommandline[i * 17 + j];
      if( c >= 'a' && c <= 'f' ) {
        c = c + 'A' - 'a';
      }
      if( c >= '0' && c <= '9' ) {
        c -= '0';
      } else if( c >= 'A' && c <= 'F' ) {
        c = c - 'A' + 10;
      } else {
        quit("Bad hash config.");
      }
      hashVal = hashVal << 4U | c;
    }
    hashes[i] = hashVal;
  }
}

#endif //HASHCONFIGCMD

/*
 * use crc64
 */

static ALWAYS_INLINE
uint64_t crc64(const uint64_t crcs, const uint64_t n) {
  int jjjjjjjjjj = 0;
  uint64_t crc = crcs;
	char* s = (char*) &n;
  for (jjjjjjjjjj = 0; jjjjjjjjjj < 8; jjjjjjjjjj++){
    uint8_t byte = s[jjjjjjjjjj];
    crc = crc64_tab[(uint8_t)crc ^ byte] ^ (crc >> 8);
  }
  return crc;
}



// Golden ratio of 2^64 (not a prime)
#define PHI64 hashes[0] // 11400714819323198485

// Some more arbitrary magic (prime) numbers
#define MUL64_1 hashes[1]
#define MUL64_2 hashes[2]
#define MUL64_3 hashes[3]
#define MUL64_4 hashes[4]
#define MUL64_5 hashes[5]
#define MUL64_6 hashes[6]
#define MUL64_7 hashes[7]
#define MUL64_8 hashes[8]
#define MUL64_9 hashes[9]
#define MUL64_10 hashes[10]
#define MUL64_11 hashes[11]
#define MUL64_12 hashes[12]
#define MUL64_13 hashes[13]

/**
 * @todo Is it okay that this function hashes values in reverse order? If so, we can replace the below functions.
 */
template<uint64_t first = 0, uint64_t... rest>
constexpr uint64_t hash() {
  if( sizeof...(rest) == 0 ) {
    return (first + 1) * hashes[0];
  } else {
    return ((first + 1) * hashes[sizeof...(rest)]) + hash<rest...>();
  }
}

/**
 * Finalizers (range reduction).
 * Keep the necessary number of bits after performing a
 * (combination of) multiplicative hash(es).
 * @param hash
 * @param hashBits
 * @return
 */
static ALWAYS_INLINE
auto finalize64(const uint64_t hash, const int hashBits) -> uint32_t {
  assert(hashBits>=0 && hashBits <= 32); // just a reasonable upper limit
  return static_cast<uint32_t>(hash >> (64 - hashBits));
}

/**
 * Get the next 8 or 16 bits following "hashBits" for checksum
 * @param hash
 * @param hashBits
 * @return
 */
static ALWAYS_INLINE
uint8_t checksum8(const uint64_t hash, const int hashBits) {
  constexpr int checksumBits = 8;
  return static_cast<uint8_t>(hash >> (64 - hashBits - checksumBits)) & ((1U << checksumBits) - 1);
}

static ALWAYS_INLINE
uint16_t checksum16(const uint64_t hash, const int hashBits) {
  constexpr int checksumBits = 16;
  return static_cast<uint16_t>(hash >> (64 - hashBits - checksumBits)) & ((1U << checksumBits) - 1);
}

//
// value hashing
//
// - Hash 1-13 64-bit (usually small) integers
//


static ALWAYS_INLINE
uint64_t hash(const uint64_t x0) {
  // return (x0 + 1) * PHI64;
  return crc64((uint64_t)0, x0 + 1);
}

static ALWAYS_INLINE
uint64_t hash(const uint64_t x0, const uint64_t x1) {
  // return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1;
  uint64_t c = crc64((uint64_t)0, x0 + 1);
  return crc64(c, x1 + 1);
}

static ALWAYS_INLINE
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2) {
  // return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2;
  uint64_t c = crc64((uint64_t)0, x0 + 1);
  c = crc64(c, x1 + 1);
  return crc64(c, x2 + 1);
}

static ALWAYS_INLINE
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2, const uint64_t x3) {
  // return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2 + (x3 + 1) * MUL64_3;
  uint64_t c = crc64((uint64_t)0, x0 + 1);
  c = crc64(c, x1 + 1);
  c = crc64(c, x2 + 1);
  return crc64(c, x3 + 1);
}

static ALWAYS_INLINE
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2, const uint64_t x3, const uint64_t x4) {
  return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2 + (x3 + 1) * MUL64_3 + (x4 + 1) * MUL64_4;
}

static ALWAYS_INLINE
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2, const uint64_t x3, const uint64_t x4, const uint64_t x5) {
  return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2 + (x3 + 1) * MUL64_3 + (x4 + 1) * MUL64_4 + (x5 + 1) * MUL64_5;
}

static ALWAYS_INLINE
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2, const uint64_t x3, const uint64_t x4, const uint64_t x5, const uint64_t x6) {
  return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2 + (x3 + 1) * MUL64_3 + (x4 + 1) * MUL64_4 + (x5 + 1) * MUL64_5 +
         (x6 + 1) * MUL64_6;
}

static ALWAYS_INLINE 
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2, const uint64_t x3, const uint64_t x4, const uint64_t x5, const uint64_t x6,
     const uint64_t x7) {
  return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2 + (x3 + 1) * MUL64_3 + (x4 + 1) * MUL64_4 + (x5 + 1) * MUL64_5 +
         (x6 + 1) * MUL64_6 + (x7 + 1) * MUL64_7;
}

static ALWAYS_INLINE 
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2, const uint64_t x3, const uint64_t x4, const uint64_t x5, const uint64_t x6,
     const uint64_t x7, const uint64_t x8) {
  return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2 + (x3 + 1) * MUL64_3 + (x4 + 1) * MUL64_4 + (x5 + 1) * MUL64_5 +
         (x6 + 1) * MUL64_6 + (x7 + 1) * MUL64_7 + (x8 + 1) * MUL64_8;
}

static ALWAYS_INLINE
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2, const uint64_t x3, const uint64_t x4, const uint64_t x5, const uint64_t x6,
     const uint64_t x7, const uint64_t x8, const uint64_t x9) {
  return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2 + (x3 + 1) * MUL64_3 + (x4 + 1) * MUL64_4 + (x5 + 1) * MUL64_5 +
         (x6 + 1) * MUL64_6 + (x7 + 1) * MUL64_7 + (x8 + 1) * MUL64_8 + (x9 + 1) * MUL64_9;
}

static ALWAYS_INLINE
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2, const uint64_t x3, const uint64_t x4, const uint64_t x5, const uint64_t x6,
     const uint64_t x7, const uint64_t x8, const uint64_t x9, const uint64_t x10) {
  return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2 + (x3 + 1) * MUL64_3 + (x4 + 1) * MUL64_4 + (x5 + 1) * MUL64_5 +
         (x6 + 1) * MUL64_6 + (x7 + 1) * MUL64_7 + (x8 + 1) * MUL64_8 + (x9 + 1) * MUL64_9 + (x10 + 1) * MUL64_10;
}

static ALWAYS_INLINE
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2, const uint64_t x3, const uint64_t x4, const uint64_t x5, const uint64_t x6,
     const uint64_t x7, const uint64_t x8, const uint64_t x9, const uint64_t x10, const uint64_t x11) {
  return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2 + (x3 + 1) * MUL64_3 + (x4 + 1) * MUL64_4 + (x5 + 1) * MUL64_5 +
         (x6 + 1) * MUL64_6 + (x7 + 1) * MUL64_7 + (x8 + 1) * MUL64_8 + (x9 + 1) * MUL64_9 + (x10 + 1) * MUL64_10 + (x11 + 1) * MUL64_11;
}

static ALWAYS_INLINE
uint64_t hash(const uint64_t x0, const uint64_t x1, const uint64_t x2, const uint64_t x3, const uint64_t x4, const uint64_t x5, const uint64_t x6,
     const uint64_t x7, const uint64_t x8, const uint64_t x9, const uint64_t x10, const uint64_t x11, const uint64_t x12) {
  return (x0 + 1) * PHI64 + (x1 + 1) * MUL64_1 + (x2 + 1) * MUL64_2 + (x3 + 1) * MUL64_3 + (x4 + 1) * MUL64_4 + (x5 + 1) * MUL64_5 +
         (x6 + 1) * MUL64_6 + (x7 + 1) * MUL64_7 + (x8 + 1) * MUL64_8 + (x9 + 1) * MUL64_9 + (x10 + 1) * MUL64_10 + (x11 + 1) * MUL64_11 +
         (x12 + 1) * MUL64_12;
}

/**
 * Call this function repeatedly for string hashing, or to combine a hash value and a (non-hash) value, or two hash values.
 * @param seed
 * @param x
 * @return
 */
static ALWAYS_INLINE
uint64_t combine64(const uint64_t seed, const uint64_t x) {
  return hash(seed * 33 + x);
}

#endif //PAQ8PX_HASH_HPP
