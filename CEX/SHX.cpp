#include "SHX.h"
#include "Serpent.h"
#include "DigestFromName.h"
#include "HKDF.h"
#include "HMAC.h"
#include "IntUtils.h"
#if defined(HAS_AVX)
#	include "UInt256.h"
#elif defined(HAS_MINSSE)
#	include "UInt128.h"
#endif

NAMESPACE_BLOCK

using CEX::Utility::IntUtils;

void SHX::DecryptBlock(const std::vector<byte> &Input, std::vector<byte> &Output)
{
	Decrypt16(Input, 0, Output, 0);
}

void SHX::DecryptBlock(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
	Decrypt16(Input, InOffset, Output, OutOffset);
}

void SHX::Destroy()
{
	if (!m_isDestroyed)
	{
		m_isDestroyed = true;
		m_dfnRounds = 0;
		m_ikmSize = 0;
		m_isEncryption = false;
		m_isInitialized = false;

		IntUtils::ClearVector(m_expKey);
		IntUtils::ClearVector(m_hkdfInfo);
		IntUtils::ClearVector(m_legalKeySizes);
		IntUtils::ClearVector(m_legalRounds);

		if (m_kdfEngine != 0)
		{
			m_kdfEngine->Destroy();
			if (m_destroyEngine)
				delete m_kdfEngine;
		}
	}
}

void SHX::EncryptBlock(const std::vector<byte> &Input, std::vector<byte> &Output)
{
	Encrypt16(Input, 0, Output, 0);
}

void SHX::EncryptBlock(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
	Encrypt16(Input, InOffset, Output, OutOffset);
}

void SHX::Initialize(bool Encryption, const CEX::Common::KeyParams &KeyParam)
{
	uint dgtsze = GetIkmSize(m_kdfEngineType);

#if defined(DEBUGASSERT_ENABLED)
	assert(KeyParam.Key().size() >= m_legalKeySizes[0] && KeyParam.Key().size() <= m_legalKeySizes[m_legalKeySizes.size() - 1]);
	if (dgtsze != 0)
		assert(KeyParam.Key().size() % dgtsze == 0);
	assert(KeyParam.Key().size() >= m_ikmSize);
#endif
#if defined(CPPEXCEPTIONS_ENABLED)
	std::string msg = "Invalid key size! Key must be either 16, 24, 32, 64 bytes or, a multiple of the hkdf hash output size.";
	if (KeyParam.Key().size() < m_legalKeySizes[0])
		throw CryptoSymmetricCipherException("SHX:Initialize", msg);
	if (KeyParam.Key().size() > m_legalKeySizes[3] && (KeyParam.Key().size() % dgtsze) != 0)
		throw CryptoSymmetricCipherException("SHX:Initialize", msg);

	for (size_t i = 0; i < m_legalKeySizes.size(); ++i)
	{
		if (KeyParam.Key().size() == m_legalKeySizes[i])
			break;
		if (i == m_legalKeySizes.size() - 1)
			throw CryptoSymmetricCipherException("SHX:Initialize", msg);
	}
	if (m_kdfEngineType != CEX::Enumeration::Digests::None)
	{
		if (KeyParam.Key().size() < m_ikmSize)
			throw CryptoSymmetricCipherException("SHX:Initialize", "Invalid key! HKDF extended mode requires key be at least hash output size.");
	}
#endif

	if (m_kdfEngineType != CEX::Enumeration::Digests::None)
		m_kdfEngine = GetDigest(m_kdfEngineType);

	m_isEncryption = Encryption;
	// expand the key
	ExpandKey(KeyParam.Key());
	// ready to transform data
	m_isInitialized = true;
}

void SHX::Transform(const std::vector<byte> &Input, std::vector<byte> &Output)
{
	if (m_isEncryption)
		Encrypt16(Input, 0, Output, 0);
	else
		Decrypt16(Input, 0, Output, 0);
}

void SHX::Transform(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
	if (m_isEncryption)
		Encrypt16(Input, InOffset, Output, OutOffset);
	else
		Decrypt16(Input, InOffset, Output, OutOffset);
}

void SHX::Transform64(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
	if (m_isEncryption)
		Encrypt64(Input, InOffset, Output, OutOffset);
	else
		Decrypt64(Input, InOffset, Output, OutOffset);
}

void SHX::Transform128(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
	if (m_isEncryption)
		Encrypt128(Input, InOffset, Output, OutOffset);
	else
		Decrypt128(Input, InOffset, Output, OutOffset);
}

//~~~Key Schedule~~~//

void SHX::ExpandKey(const std::vector<byte> &Key)
{
	if (m_kdfEngineType != CEX::Enumeration::Digests::None)
	{
		// hkdf key expansion
		SecureExpand(Key);
	}
	else
	{
		// standard serpent key expansion + k512
		StandardExpand(Key);
	}
}

void SHX::SecureExpand(const std::vector<byte> &Key)
{
	// expanded key size
	size_t keySize = 4 * (m_dfnRounds + 1);
	// hkdf return array
	size_t keyBytes = keySize * 4;
	std::vector<byte> rawKey(keyBytes, 0);
	size_t saltSize = Key.size() - m_ikmSize;

	// hkdf input
	std::vector<byte> kdfKey(m_ikmSize, 0);
	std::vector<byte> kdfSalt(0, 0);

	// copy hkdf key and salt from user key
	memcpy(&kdfKey[0], &Key[0], m_ikmSize);
	if (saltSize > 0)
	{
		kdfSalt.resize(saltSize);
		memcpy(&kdfSalt[0], &Key[m_ikmSize], saltSize);
	}

	// HKDF generator expands array using an SHA512 HMAC
	CEX::Mac::HMAC hmac(m_kdfEngine);
	CEX::Generator::HKDF gen(&hmac);
	gen.Initialize(kdfSalt, kdfKey, m_hkdfInfo);
	gen.Generate(rawKey);

	// initialize working key
	std::vector<uint> wK(keySize, 0);
	// copy bytes to working key
	memcpy(&wK[0], &rawKey[0], keyBytes);
	// set the expanded key
	m_expKey = wK;
}

void SHX::StandardExpand(const std::vector<byte> &Key)
{
	uint cnt = 0;
	uint index = 0;
	size_t padSize = Key.size() < 32 ? 16 : Key.size() / 2;
	std::vector<uint> Wp(padSize, 0);
	size_t offset = 0;

	// CHANGE: 512 key gets 8 extra rounds
	m_dfnRounds = (Key.size() == 64) ? 40 : ROUNDS32;
	size_t keySize = 4 * (m_dfnRounds + 1);

	// step 1: reverse copy key to temp array
	for (offset = Key.size(); offset > 0; offset -= 4)
		Wp[index++] = IntUtils::BytesToBe32(Key, offset - 4);

	// pad small key
	if (index < 8)
		Wp[index] = 1;

	// initialize the key
	std::vector<uint> Wk(keySize, 0);

	if (padSize == 16)
	{
		// 32 byte key
		// step 2: rotate k into w(k) ints
		for (size_t i = 8; i < 16; i++)
			Wp[i] = IntUtils::RotL32((uint)(Wp[i - 8] ^ Wp[i - 5] ^ Wp[i - 3] ^ Wp[i - 1] ^ PHI ^ (i - 8)), 11);

		// copy to expanded key
		memcpy(&Wk[0], &Wp[8], 8 * sizeof(uint));

		// step 3: calculate remainder of rounds with rotating primitive
		for (size_t i = 8; i < keySize; i++)
			Wk[i] = IntUtils::RotL32((uint)(Wk[i - 8] ^ Wk[i - 5] ^ Wk[i - 3] ^ Wk[i - 1] ^ PHI ^ i), 11);
	}
	else
	{
		// *extended*: 64 byte key
		// step 3: rotate k into w(k) ints, with extended polynominal
		// Wp := (Wp-16 ^ Wp-13 ^ Wp-11 ^ Wp-10 ^ Wp-8 ^ Wp-5 ^ Wp-3 ^ Wp-1 ^ PHI ^ i) <<< 11
		for (size_t i = 16; i < 32; i++)
			Wp[i] = IntUtils::RotL32((uint)(Wp[i - 16] ^ Wp[i - 13] ^ Wp[i - 11] ^ Wp[i - 10] ^ Wp[i - 8] ^ Wp[i - 5] ^ Wp[i - 3] ^ Wp[i - 1] ^ PHI ^ (i - 16)), 11);

		// copy to expanded key
		memcpy(&Wk[0], &Wp[16], 16 * sizeof(uint));

		// step 3: calculate remainder of rounds with rotating primitive
		for (size_t i = 16; i < keySize; i++)
			Wk[i] = IntUtils::RotL32((uint)(Wk[i - 16] ^ Wk[i - 13] ^ Wk[i - 11] ^ Wk[i - 10] ^ Wk[i - 8] ^ Wk[i - 5] ^ Wk[i - 3] ^ Wk[i - 1] ^ PHI ^ i), 11);
	}

	// step 4: create the working keys by processing with the Sbox and IP
	while (cnt < keySize - 4)
	{
		Sb3(Wk[cnt], Wk[cnt + 1], Wk[cnt + 2], Wk[cnt + 3]); cnt += 4;
		Sb2(Wk[cnt], Wk[cnt + 1], Wk[cnt + 2], Wk[cnt + 3]); cnt += 4;
		Sb1(Wk[cnt], Wk[cnt + 1], Wk[cnt + 2], Wk[cnt + 3]); cnt += 4;
		Sb0(Wk[cnt], Wk[cnt + 1], Wk[cnt + 2], Wk[cnt + 3]); cnt += 4;
		Sb7(Wk[cnt], Wk[cnt + 1], Wk[cnt + 2], Wk[cnt + 3]); cnt += 4;
		Sb6(Wk[cnt], Wk[cnt + 1], Wk[cnt + 2], Wk[cnt + 3]); cnt += 4;
		Sb5(Wk[cnt], Wk[cnt + 1], Wk[cnt + 2], Wk[cnt + 3]); cnt += 4;
		Sb4(Wk[cnt], Wk[cnt + 1], Wk[cnt + 2], Wk[cnt + 3]); cnt += 4;
	}

	// last round
	Sb3(Wk[cnt], Wk[cnt + 1], Wk[cnt + 2], Wk[cnt + 3]);

	m_expKey = Wk;
}

//~~~Rounds Processing~~~//

void SHX::Decrypt16(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
	const size_t LRD = 4;
	size_t keyCtr = m_expKey.size();

	// input round
	uint R3 = IntUtils::BytesToLe32(Input, InOffset + 12);
	uint R2 = IntUtils::BytesToLe32(Input, InOffset + 8);
	uint R1 = IntUtils::BytesToLe32(Input, InOffset + 4);
	uint R0 = IntUtils::BytesToLe32(Input, InOffset);

	R3 ^= m_expKey[--keyCtr];
	R2 ^= m_expKey[--keyCtr];
	R1 ^= m_expKey[--keyCtr];
	R0 ^= m_expKey[--keyCtr];

	// process 8 round blocks
	do
	{
		Ib7(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform(R0, R1, R2, R3);

		Ib6(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform(R0, R1, R2, R3);

		Ib5(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform(R0, R1, R2, R3);

		Ib4(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform(R0, R1, R2, R3);

		Ib3(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform(R0, R1, R2, R3);

		Ib2(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform(R0, R1, R2, R3);

		Ib1(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform(R0, R1, R2, R3);

		Ib0(R0, R1, R2, R3);

		// skip on last block
		if (keyCtr != LRD)
		{
			R3 ^= m_expKey[--keyCtr];
			R2 ^= m_expKey[--keyCtr];
			R1 ^= m_expKey[--keyCtr];
			R0 ^= m_expKey[--keyCtr];
			InverseTransform(R0, R1, R2, R3);
		}
	} 
	while (keyCtr != LRD);

	// last round
	IntUtils::Le32ToBytes(R3 ^ m_expKey[--keyCtr], Output, OutOffset + 12);
	IntUtils::Le32ToBytes(R2 ^ m_expKey[--keyCtr], Output, OutOffset + 8);
	IntUtils::Le32ToBytes(R1 ^ m_expKey[--keyCtr], Output, OutOffset + 4);
	IntUtils::Le32ToBytes(R0 ^ m_expKey[--keyCtr], Output, OutOffset);
}

void SHX::Decrypt64(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
#if defined(HAS_MINSSE) && !defined(HAS_AVX)

	const size_t LRD = 4;
	size_t keyCtr = m_expKey.size();

	// input round
	CEX::Numeric::UInt128 R0(Input, InOffset);
	CEX::Numeric::UInt128 R1(Input, InOffset + 16);
	CEX::Numeric::UInt128 R2(Input, InOffset + 32);
	CEX::Numeric::UInt128 R3(Input, InOffset + 48);
	CEX::Numeric::UInt128::Transpose(R0, R1, R2, R3);

	R3 ^= m_expKey[--keyCtr];
	R2 ^= m_expKey[--keyCtr];
	R1 ^= m_expKey[--keyCtr];
	R0 ^= m_expKey[--keyCtr];

	// process 8 round blocks
	do
	{
		Ib7(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib6(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib5(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib4(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib3(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib2(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib1(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib0(R0, R1, R2, R3);

		// skip on last block
		if (keyCtr != LRD)
		{
			R3 ^= m_expKey[--keyCtr];
			R2 ^= m_expKey[--keyCtr];
			R1 ^= m_expKey[--keyCtr];
			R0 ^= m_expKey[--keyCtr];
			InverseTransform64(R0, R1, R2, R3);
		}
	} 
	while (keyCtr != LRD);

	// last round
	R3 ^= m_expKey[--keyCtr];
	R2 ^= m_expKey[--keyCtr];
	R1 ^= m_expKey[--keyCtr];
	R0 ^= m_expKey[--keyCtr];

	CEX::Numeric::UInt128::Transpose(R0, R1, R2, R3);
	R0.StoreLE(Output, OutOffset);
	R1.StoreLE(Output, OutOffset + 16);
	R2.StoreLE(Output, OutOffset + 32);
	R3.StoreLE(Output, OutOffset + 48);

#else

	Decrypt16(Input, InOffset, Output, OutOffset);
	Decrypt16(Input, InOffset + 16, Output, OutOffset + 16);
	Decrypt16(Input, InOffset + 32, Output, OutOffset + 32);
	Decrypt16(Input, InOffset + 48, Output, OutOffset + 48);

#endif
}

void SHX::Decrypt128(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
#if defined(HAS_AVX)

	const size_t LRD = 4;
	size_t keyCtr = m_expKey.size();

	// input round
	CEX::Numeric::UInt256 R0(Input, InOffset);
	CEX::Numeric::UInt256 R1(Input, InOffset + 32);
	CEX::Numeric::UInt256 R2(Input, InOffset + 64);
	CEX::Numeric::UInt256 R3(Input, InOffset + 96);
	CEX::Numeric::UInt256::Transpose(R0, R1, R2, R3);

	R3 ^= m_expKey[--keyCtr];
	R2 ^= m_expKey[--keyCtr];
	R1 ^= m_expKey[--keyCtr];
	R0 ^= m_expKey[--keyCtr];

	// process 8 round blocks
	do
	{
		Ib7(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib6(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib5(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib4(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib3(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib2(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib1(R0, R1, R2, R3);
		R3 ^= m_expKey[--keyCtr];
		R2 ^= m_expKey[--keyCtr];
		R1 ^= m_expKey[--keyCtr];
		R0 ^= m_expKey[--keyCtr];
		InverseTransform64(R0, R1, R2, R3);

		Ib0(R0, R1, R2, R3);

		// skip on last block
		if (keyCtr != LRD)
		{
			R3 ^= m_expKey[--keyCtr];
			R2 ^= m_expKey[--keyCtr];
			R1 ^= m_expKey[--keyCtr];
			R0 ^= m_expKey[--keyCtr];
			InverseTransform64(R0, R1, R2, R3);
		}
	} while (keyCtr != LRD);

	// last round
	R3 ^= m_expKey[--keyCtr];
	R2 ^= m_expKey[--keyCtr];
	R1 ^= m_expKey[--keyCtr];
	R0 ^= m_expKey[--keyCtr];

	CEX::Numeric::UInt256::Transpose(R0, R1, R2, R3);
	R0.StoreLE(Output, OutOffset);
	R1.StoreLE(Output, OutOffset + 32);
	R2.StoreLE(Output, OutOffset + 64);
	R3.StoreLE(Output, OutOffset + 96);

#else

	Decrypt16(Input, InOffset, Output, OutOffset);
	Decrypt16(Input, InOffset + 16, Output, OutOffset + 16);
	Decrypt16(Input, InOffset + 32, Output, OutOffset + 32);
	Decrypt16(Input, InOffset + 48, Output, OutOffset + 48);
	Decrypt16(Input, InOffset + 64, Output, OutOffset + 64);
	Decrypt16(Input, InOffset + 80, Output, OutOffset + 80);
	Decrypt16(Input, InOffset + 96, Output, OutOffset + 96);
	Decrypt16(Input, InOffset + 112, Output, OutOffset + 112);

#endif
}

void SHX::Encrypt16(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
	const size_t LRD = m_expKey.size() - 5;
	int keyCtr = -1;

	// input round
	uint R0 = IntUtils::BytesToLe32(Input, InOffset);
	uint R1 = IntUtils::BytesToLe32(Input, InOffset + 4);
	uint R2 = IntUtils::BytesToLe32(Input, InOffset + 8);
	uint R3 = IntUtils::BytesToLe32(Input, InOffset + 12);

	// process 8 round blocks
	do
	{
		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb0(R0, R1, R2, R3);
		LinearTransform(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb1(R0, R1, R2, R3);
		LinearTransform(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb2(R0, R1, R2, R3);
		LinearTransform(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb3(R0, R1, R2, R3);
		LinearTransform(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb4(R0, R1, R2, R3);
		LinearTransform(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb5(R0, R1, R2, R3);
		LinearTransform(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb6(R0, R1, R2, R3);
		LinearTransform(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb7(R0, R1, R2, R3);

		// skip on last block
		if (keyCtr != LRD)
			LinearTransform(R0, R1, R2, R3);
	} 
	while (keyCtr != LRD);

	// last round
	IntUtils::Le32ToBytes(m_expKey[++keyCtr] ^ R0, Output, OutOffset);
	IntUtils::Le32ToBytes(m_expKey[++keyCtr] ^ R1, Output, OutOffset + 4);
	IntUtils::Le32ToBytes(m_expKey[++keyCtr] ^ R2, Output, OutOffset + 8);
	IntUtils::Le32ToBytes(m_expKey[++keyCtr] ^ R3, Output, OutOffset + 12);
}

void SHX::Encrypt64(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
#if defined(HAS_MINSSE) && !defined(HAS_AVX)

	const size_t LRD = m_expKey.size() - 5;
	int keyCtr = -1;

	// input round
	CEX::Numeric::UInt128 R0(Input, InOffset);
	CEX::Numeric::UInt128 R1(Input, InOffset + 16);
	CEX::Numeric::UInt128 R2(Input, InOffset + 32);
	CEX::Numeric::UInt128 R3(Input, InOffset + 48);
	CEX::Numeric::UInt128::Transpose(R0, R1, R2, R3);

	// process 8 round blocks
	do
	{
		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb0(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb1(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb2(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb3(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb4(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb5(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb6(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb7(R0, R1, R2, R3);

		// skip on last block
		if (keyCtr != LRD)
			LinearTransform64(R0, R1, R2, R3);
	} 
	while (keyCtr != LRD);

	// last round
	R0 ^= m_expKey[++keyCtr];
	R1 ^= m_expKey[++keyCtr];
	R2 ^= m_expKey[++keyCtr];
	R3 ^= m_expKey[++keyCtr];

	CEX::Numeric::UInt128::Transpose(R0, R1, R2, R3);
	R0.StoreLE(Output, OutOffset);
	R1.StoreLE(Output, OutOffset + 16);
	R2.StoreLE(Output, OutOffset + 32);
	R3.StoreLE(Output, OutOffset + 48);

#else

	Encrypt16(Input, InOffset, Output, OutOffset);
	Encrypt16(Input, InOffset + 16, Output, OutOffset + 16);
	Encrypt16(Input, InOffset + 32, Output, OutOffset + 32);
	Encrypt16(Input, InOffset + 48, Output, OutOffset + 48);

#endif
}

void SHX::Encrypt128(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset)
{
#if defined(HAS_AVX)

	const size_t LRD = m_expKey.size() - 5;
	int keyCtr = -1;

	// input round
	CEX::Numeric::UInt256 R0(Input, InOffset);
	CEX::Numeric::UInt256 R1(Input, InOffset + 32);
	CEX::Numeric::UInt256 R2(Input, InOffset + 64);
	CEX::Numeric::UInt256 R3(Input, InOffset + 96);
	CEX::Numeric::UInt256::Transpose(R0, R1, R2, R3);

	// process 8 round blocks
	do
	{
		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb0(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb1(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb2(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb3(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb4(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb5(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb6(R0, R1, R2, R3);
		LinearTransform64(R0, R1, R2, R3);

		R0 ^= m_expKey[++keyCtr];
		R1 ^= m_expKey[++keyCtr];
		R2 ^= m_expKey[++keyCtr];
		R3 ^= m_expKey[++keyCtr];
		Sb7(R0, R1, R2, R3);

		// skip on last block
		if (keyCtr != LRD)
			LinearTransform64(R0, R1, R2, R3);
	} while (keyCtr != LRD);

	// last round
	R0 ^= m_expKey[++keyCtr];
	R1 ^= m_expKey[++keyCtr];
	R2 ^= m_expKey[++keyCtr];
	R3 ^= m_expKey[++keyCtr];

	CEX::Numeric::UInt256::Transpose(R0, R1, R2, R3);
	R0.StoreLE(Output, OutOffset);
	R1.StoreLE(Output, OutOffset + 32);
	R2.StoreLE(Output, OutOffset + 64);
	R3.StoreLE(Output, OutOffset + 96);

#else

	Encrypt16(Input, InOffset, Output, OutOffset);
	Encrypt16(Input, InOffset + 16, Output, OutOffset + 16);
	Encrypt16(Input, InOffset + 32, Output, OutOffset + 32);
	Encrypt16(Input, InOffset + 48, Output, OutOffset + 48);
	Encrypt16(Input, InOffset + 64, Output, OutOffset + 64);
	Encrypt16(Input, InOffset + 80, Output, OutOffset + 80);
	Encrypt16(Input, InOffset + 96, Output, OutOffset + 96);
	Encrypt16(Input, InOffset + 112, Output, OutOffset + 112);

#endif
}

CEX::Digest::IDigest* SHX::GetDigest(CEX::Enumeration::Digests DigestType)
{
	try
	{
		return CEX::Helper::DigestFromName::GetInstance(DigestType);
	}
	catch (...)
	{
#if defined(DEBUGASSERT_ENABLED)
		assert("SHX:GetDigest The digest could not be instantiated!");
#endif
#if defined(CPPEXCEPTIONS_ENABLED)
		throw CryptoSymmetricCipherException("SHX:GetDigest", "The digest could not be instantiated!");
#else
		return 0;
#endif
	}
}

uint SHX::GetIkmSize(CEX::Enumeration::Digests DigestType)
{
	return CEX::Helper::DigestFromName::GetDigestSize(DigestType);
}

NAMESPACE_BLOCKEND