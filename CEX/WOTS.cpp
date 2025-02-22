#include "WOTS.h"

NAMESPACE_SPHINCS

void WOTS::BaseW(std::vector<int32_t> &Output, size_t OutOffset, size_t OutLength, const std::vector<byte> &Input)
{
	// base_w algorithm as described in draft
	// interprets an array of bytes as integers in base w
	// this only works when log_w is a divisor of 8

	size_t i;
	size_t inoff;
	int32_t bits;
	byte total;

	bits = 0;
	inoff = 0;
	total = 0;

	for (i = 0; i < OutLength; ++i)
	{
		if (bits == 0)
		{
			total = Input[inoff];
			++inoff;
			bits += 8;
		}

		bits -= SPX_WOTS_LOGW;
		Output[OutOffset] = (total >> bits) & (SPX_WOTS_W - 1);
		++OutOffset;
	}
}

void WOTS::ChainLengths(std::vector<int32_t> &Lengths, const std::vector<byte> &Message, size_t N)
{
	const uint SPX_WOTS_LEN1 = (8 * N / SPX_WOTS_LOGW);
	// takes a message and derives the matching chain lengths
	BaseW(Lengths, 0, SPX_WOTS_LEN1, Message);
	WotsChecksum(Lengths, SPX_WOTS_LEN1, Lengths, N);
}

void WOTS::GenChain(std::vector<byte> &Output, size_t OutOffset, const std::vector<byte> &Input, size_t InOffset, uint Start, uint Steps,
	const std::vector<byte> &PkSeed, std::array<uint, 8> &Address, size_t N)
{
	std::vector<byte> buf(N + SPX_ADDR_BYTES + 1 * N);
	std::vector<byte> mask(1 * N);
	uint idx;

	// initialize out with the value at position 'start'
	MemoryTools::Copy(Input, InOffset, Output, OutOffset, N);

	// iterate 'steps' calls to the hash function
	for (idx = Start; idx < (Start + Steps) && idx < SPX_WOTS_W; ++idx)
	{
		SphincsUtils::SetHashAddress(Address, idx);
		THash(Output, OutOffset, Output, OutOffset, 1, PkSeed, Address, buf, mask, N);
	}
}

void WOTS::THash(std::vector<byte> &Output, size_t OutOffset, const std::vector<byte> &Input, size_t InOffset, const size_t InputBlocks,
	const std::vector<byte> &PkSeed, std::array<uint, 8> & Address, std::vector<byte> &Buffer, std::vector<byte> &Mask, size_t N)
{
	size_t i;

	MemoryTools::Clear(Buffer, 0, Buffer.size());
	MemoryTools::Clear(Mask, 0, Mask.size());
	MemoryTools::Copy(PkSeed, 0, Buffer, 0, N);
	SphincsUtils::AddressToBytes(Buffer, N, Address);

	std::vector<byte> k(N + SPX_ADDR_BYTES);
	MemoryTools::Copy(Buffer, 0, k, 0, k.size());

	SphincsUtils::XOF(k, 0, k.size(), Mask, 0, InputBlocks * N, Keccak::KECCAK256_RATE_SIZE);

	for (i = 0; i < InputBlocks * N; ++i)
	{
		Buffer[N + SPX_ADDR_BYTES + i] = Input[InOffset + i] ^ Mask[i];
	}

	k.resize(N + SPX_ADDR_BYTES + InputBlocks * N);
	MemoryTools::Copy(Buffer, 0, k, 0, k.size());

	SphincsUtils::XOF(k, 0, k.size(), Output, OutOffset, N, Keccak::KECCAK256_RATE_SIZE);
}

void WOTS::WotsChecksum(std::vector<int32_t> &CSumBaseW, size_t BaseOffset, const std::vector<int32_t> &MsgBaseW, size_t N)
{
	// computes the WOTS+ checksum over a message (in base_w)

	const uint SPX_WOTS_LEN1 = (8 * N / SPX_WOTS_LOGW);
	std::vector<byte> csumbytes((SPX_WOTS_LEN2 * SPX_WOTS_LOGW + 7) / 8);
	ulong csum;
	uint idx;

	csum = 0;

	// compute checksum
	for (idx = 0; idx < SPX_WOTS_LEN1; ++idx)
	{
		csum += static_cast<ulong>(SPX_WOTS_W - 1) - MsgBaseW[idx];
	}

	// convert checksum to base_w
	// make sure expected empty zero bits are the least significant bits
	csum = csum << (8 - ((SPX_WOTS_LEN2 * SPX_WOTS_LOGW) % 8));
	SphincsUtils::UllToBytes(csumbytes, 0, csum, csumbytes.size());
	BaseW(CSumBaseW, BaseOffset, SPX_WOTS_LEN2, csumbytes);
}

void WOTS::WotsGenLeaf(std::vector<byte> &Leaf, size_t LeafOffset, const std::vector<byte> &SkSeed, const std::vector<byte> &PkSeed, uint AddressIndex,
	const std::array<uint, 8> & TreeAddress, size_t N)
{
	const uint SPX_WOTS_LEN = ((8 * N / SPX_WOTS_LOGW) + SPX_WOTS_LEN2);
	const uint SPX_WOTS_BYTES = (SPX_WOTS_LEN * N);
	// computes the leaf at a given address. First generates the WOTS key pair, then computes leaf by hashing horizontally
	std::vector<byte> buf(N + SPX_ADDR_BYTES + SPX_WOTS_LEN * N);
	std::vector<byte> mask(SPX_WOTS_LEN * N);
	std::vector<byte> pk(SPX_WOTS_BYTES);
	std::array<uint, 8> wotsaddr = { 0 };
	std::array<uint, 8> wotspkaddr = { 0 };

	SphincsUtils::SetType(wotsaddr, SPX_ADDR_TYPE_WOTS);
	SphincsUtils::SetType(wotspkaddr, SPX_ADDR_TYPE_WOTSPK);
	SphincsUtils::CopySubtreeAddress(TreeAddress, wotsaddr);
	SphincsUtils::SetKeypairAddress(wotsaddr, AddressIndex);

	WotsGenPk(pk, SkSeed, PkSeed, wotsaddr, N);
	SphincsUtils::CopyKeypairAddress(wotsaddr, wotspkaddr);
	THash(Leaf, LeafOffset, pk, 0, SPX_WOTS_LEN, PkSeed, wotspkaddr, buf, mask, N);
}

void WOTS::WotsGenPk(std::vector<byte> &PublicKey, const std::vector<byte> &SkSeed, const std::vector<byte> &PkSeed, std::array<uint, 8> & Address, size_t N)
{
	const uint SPX_WOTS_LEN1 = (8 * N / SPX_WOTS_LOGW);
	const uint SPX_WOTS_LEN = (SPX_WOTS_LEN1 + SPX_WOTS_LEN2);
	uint idx;

	for (idx = 0; idx < SPX_WOTS_LEN; ++idx)
	{
		SphincsUtils::SetChainAddress(Address, idx);
		WotsGenSk(PublicKey, idx * N, SkSeed, Address, N);
		GenChain(PublicKey, idx * N, PublicKey, idx * N, 0, SPX_WOTS_W - 1, PkSeed, Address, N);
	}
}

void WOTS::WotsGenSk(std::vector<byte> &Key, size_t Offset, const std::vector<byte> &KeySeed, std::array<uint, 8> & WotsAddress, size_t N)
{
	// make sure that the hash address is actually zeroed
	SphincsUtils::SetHashAddress(WotsAddress, 0);
	// generate sk element
	SphincsUtils::PrfAddress(Key, Offset, KeySeed, WotsAddress, N);
}

void WOTS::WotsPkFromSig(std::vector<byte> &PublicKey, const std::vector<byte> &Signature, size_t SigOffset, const std::vector<byte> &Message,
	const std::vector<byte> &PrivateSeed, std::array<uint, 8> & Address, size_t N)
{
	const uint SPX_WOTS_LEN = ((8 * N / SPX_WOTS_LOGW) + SPX_WOTS_LEN2);
	std::vector<int32_t> lengths(SPX_WOTS_LEN);
	uint idx;

	ChainLengths(lengths, Message, N);

	for (idx = 0; idx < SPX_WOTS_LEN; ++idx)
	{
		SphincsUtils::SetChainAddress(Address, idx);
		GenChain(PublicKey, idx * N, Signature, SigOffset + (idx * N), lengths[idx], (SPX_WOTS_W - 1) - lengths[idx], PrivateSeed, Address, N);
	}
}

void WOTS::WotsSign(std::vector<byte> &Signature, size_t SigOffset, const std::vector<byte> &Message, const std::vector<byte> &SecretSeed,
	const std::vector<byte> &PublicSeed, std::array<uint, 8> & Address, size_t N)
{
	const uint SPX_WOTS_LEN = ((8 * N / SPX_WOTS_LOGW) + SPX_WOTS_LEN2);
	std::vector<int32_t> lengths(SPX_WOTS_LEN);
	uint idx;

	ChainLengths(lengths, Message, N);

	for (idx = 0; idx < SPX_WOTS_LEN; ++idx)
	{
		SphincsUtils::SetChainAddress(Address, idx);
		WotsGenSk(Signature, SigOffset + (idx * N), SecretSeed, Address, N);
		GenChain(Signature, SigOffset + (idx * N), Signature, SigOffset + (idx * N), 0, lengths[idx], PublicSeed, Address, N);
	}
}

NAMESPACE_SPHINCSEND