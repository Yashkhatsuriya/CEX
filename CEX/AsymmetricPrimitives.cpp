#include "AsymmetricPrimitives.h"

NAMESPACE_ENUMERATION

std::string AsymmetricPrimitiveConvert::ToName(AsymmetricPrimitives Enumeral)
{
	std::string name("");

	switch (Enumeral)
	{
		case AsymmetricPrimitives::Dilithium:
			name = std::string("Dilithium");
			break;
		case AsymmetricPrimitives::McEliece:
			name = std::string("McEliece");
			break;
		case AsymmetricPrimitives::ModuleLWE:
			name = std::string("ModuleLWE");
			break;
		case AsymmetricPrimitives::NTRUPrime:
			name = std::string("NTRUPrime");
			break;
		case AsymmetricPrimitives::RingLWE:
			name = std::string("RingLWE");
			break;
		case AsymmetricPrimitives::Sphincs:
			name = std::string("Sphincs");
			break;
		default:
			name = std::string("None");
			break;
	}

	return name;
}

AsymmetricPrimitives AsymmetricPrimitiveConvert::FromName(std::string &Name)
{
	AsymmetricPrimitives tname;

	if (Name == std::string("Dilithium"))
	{
		tname = AsymmetricPrimitives::Dilithium;
	}
	else if (Name == std::string("McEliece"))
	{
		tname = AsymmetricPrimitives::McEliece;
	}	
	else if (Name == std::string("ModuleLWE"))
	{
		tname = AsymmetricPrimitives::ModuleLWE;
	}
	else if (Name == std::string("NTRUPrime"))
	{
		tname = AsymmetricPrimitives::NTRUPrime;
	}
	else if (Name == std::string("RingLWE"))
	{
		tname = AsymmetricPrimitives::RingLWE;
	}
	else if (Name == std::string("Sphincs"))
	{
		tname = AsymmetricPrimitives::Sphincs;
	}
	else
	{
		tname = AsymmetricPrimitives::None;
	}

	return tname;
}

NAMESPACE_ENUMERATIONEND