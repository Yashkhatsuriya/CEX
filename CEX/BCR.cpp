#include "BCR.h"
#include "BCG.h"
#include "MemoryTools.h"
#include "ProviderFromName.h"
#include "SymmetricKey.h"

NAMESPACE_PRNG

using Drbg::BCG;
using Enumeration::BlockCipherConvert;
using Utility::MemoryTools;
using Enumeration::PrngConvert;
using Enumeration::ProviderConvert;

//~~~Constructor~~~//

BCR::BCR(BlockCiphers CipherType, Providers ProviderType, bool Parallel)
	:
	PrngBase(Prngs::BCR, PrngConvert::ToName(Prngs::BCR) + std::string("-") + BlockCipherConvert::ToName(CipherType) + std::string("-") + ProviderConvert::ToName(ProviderType)),
	m_isParallel(Parallel),
	m_pvdType(ProviderType != Providers::None ? ProviderType :
		throw CryptoRandomException(PrngConvert::ToName(Prngs::BCR), std::string("Constructor"), std::string("Provider type can not be none!"), ErrorCodes::InvalidParam)),
	m_rngGenerator(CipherType != BlockCiphers::None ? new BCG(CipherType, ProviderType) :
		throw CryptoRandomException(PrngConvert::ToName(Prngs::BCR), std::string("Constructor"), std::string("Cipher type can not be none!"), ErrorCodes::IllegalOperation))
{
	Reset();
}

BCR::~BCR()
{
	m_isParallel = false;
	m_pvdType = Providers::None;

	if (m_rngGenerator != nullptr)
	{
		m_rngGenerator.reset(nullptr);
	}
}

//~~~Accessors~~~//

//~~~Public Functions~~~//

void BCR::Generate(std::vector<byte> &Output)
{
	GetRandom(Output, 0, Output.size(), m_rngGenerator);
}

void BCR::Generate(std::vector<byte> &Output, size_t Offset, size_t Length)
{
	if ((Output.size() - Offset) < Length)
	{
		throw CryptoRandomException(Name(), std::string("Generate"), std::string("The output buffer is too small!"), ErrorCodes::InvalidSize);
	}

	GetRandom(Output, Offset, Length, m_rngGenerator);
}

void BCR::Generate(SecureVector<byte> &Output)
{

	GetRandom(Output, 0, Output.size(), m_rngGenerator);
}

void BCR::Generate(SecureVector<byte> &Output, size_t Offset, size_t Length)
{
	if ((Output.size() - Offset) < Length)
	{
		throw CryptoRandomException(Name(), std::string("Generate"), std::string("The output buffer is too small!"), ErrorCodes::InvalidSize);
	}

	GetRandom(Output, Offset, Length, m_rngGenerator);
}

void BCR::Reset()
{
	if (m_isParallel)
	{
		static_cast<BCG*>(m_rngGenerator.get())->IsParallel();
	}

	static_cast<BCG*>(m_rngGenerator.get())->ParallelProfile().IsParallel() = m_isParallel;

	Cipher::SymmetricKeySize ks = m_rngGenerator->LegalKeySizes()[1];
	std::vector<byte> key(ks.KeySize());
	std::vector<byte> nonce(ks.NonceSize());

	Provider::IProvider* pvd = Helper::ProviderFromName::GetInstance(m_pvdType);

	if (!pvd->IsAvailable())
	{
		throw CryptoRandomException(Name(), std::string("Reset"), std::string("The random provider can not be instantiated!"), ErrorCodes::NoAccess);
	}

	pvd->Generate(key);
	pvd->Generate(nonce);
	delete pvd;

	Cipher::SymmetricKey kp(key, nonce);
	m_rngGenerator->Initialize(kp);
	MemoryTools::Clear(key, 0, key.size());
	MemoryTools::Clear(nonce, 0, nonce.size());
}

//~~~Private Functions~~~//

void BCR::GetRandom(std::vector<byte> &Output, size_t Offset, size_t Length, std::unique_ptr<IDrbg> &Generator)
{
	Generator->Generate(Output, Offset, Length);
}

void BCR::GetRandom(SecureVector<byte> &Output, size_t Offset, size_t Length, std::unique_ptr<IDrbg> &Generator)
{
	Generator->Generate(Output, Offset, Length);
}

NAMESPACE_PRNGEND
