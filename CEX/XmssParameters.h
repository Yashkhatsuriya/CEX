#ifndef CEX_XMSSPARAMETERS_H
#define CEX_XMSSPARAMETERS_H

#include "CexDomain.h"
#include "AsymmetricParameters.h"

NAMESPACE_ENUMERATION

/// <summary>
/// The XMSS/XMSS-MT parameter sets enumeration
/// </summary>
enum class XmssParameters : byte
{
	/// <summary>
	/// No parameter is specified
	/// </summary>
	None = 0,
	/// <summary>
	/// The XMSS SHA256H10 parameter; (Medium Security) SHA2-256 with a tree height of 10
	/// </summary>
	XMSSSHA256H10 = static_cast<byte>(AsymmetricParameters::XMSSSHA256H10),
	/// <summary>
	/// The XMSS SHA256H16 parameter; (Medium Security) SHA2-256 with a tree height of 16
	/// </summary>
	XMSSSHA256H16 = static_cast<byte>(AsymmetricParameters::XMSSSHA256H16),
	/// <summary>
	/// The XMSS SHA256H20 parameter; (Medium Security) The SHA2-256 with a tree height of 20
	/// </summary>
	XMSSSHA256H20 = static_cast<byte>(AsymmetricParameters::XMSSSHA256H20),
	/// <summary>
	/// The XMSS SHA512H10 parameter; (High Security) SHA2-512 with a tree height of 10
	/// </summary>
	XMSSSHA512H10 = static_cast<byte>(AsymmetricParameters::XMSSSHA512H10),
	/// <summary>
	/// The XMSS SHA512H10 parameter; (High Security) SHA2-512 with a tree height of 16
	/// </summary>
	XMSSSHA512H16 = static_cast<byte>(AsymmetricParameters::XMSSSHA512H16),
	/// <summary>
	/// The XMSS SHA512H20 parameter; (Highest Security) SHA2-512 with a tree height of 20
	/// </summary>
	XMSSSHA512H20 = static_cast<byte>(AsymmetricParameters::XMSSSHA512H20),
	/// <summary>
	/// The XMSS SHAKE256H10 parameter; (Medium Security) SHAKE-256 with a tree height of 10
	/// </summary>
	XMSSSHAKE256H10 = static_cast<byte>(AsymmetricParameters::XMSSSHAKE256H10),
	/// <summary>
	/// The SHAKE256H16 S1 parameter; (Medium Security) SHAKE-256 with a tree height of 16
	/// </summary>
	XMSSSHAKE256H16 = static_cast<byte>(AsymmetricParameters::XMSSSHAKE256H16),
	/// <summary>
	/// The XMSS SHAKE256H20 parameter; (Medium Security) SHAKE-256 with a tree height of 20
	/// </summary>
	XMSSSHAKE256H20 = static_cast<byte>(AsymmetricParameters::XMSSSHAKE256H20),
	/// <summary>
	/// The XMSS SHAKE512H10 parameter; (High Security) SHAKE-512 with a tree height of 10
	/// </summary>
	XMSSSHAKE512H10 = static_cast<byte>(AsymmetricParameters::XMSSSHAKE512H10),
	/// <summary>
	/// The XMSS SHAKE512H10 parameter; (High Security) SHAKE-512 with a tree height of 16
	/// </summary>
	XMSSSHAKE512H16 = static_cast<byte>(AsymmetricParameters::XMSSSHAKE512H16),
	/// <summary>
	/// The XMSS SHAKE512H20 parameter; (Highest Security) SHAKE-512 with a tree height of 20
	/// </summary>
	XMSSSHAKE512H20 = static_cast<byte>(AsymmetricParameters::XMSSSHAKE512H20),
	/// <summary>
	/// The XMSS-MT SHA256H20D2 parameter; (Medium Security) SHA2-256 with a tree height of 20, and 2 subtree layers
	/// </summary>
	XMSSMTSHA256H20D2 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA256H20D2),
	/// <summary>
	/// The XMSS-MT SHA256H20D4 parameter; (Medium Security) SHA2-256 with a tree height of 20, and 4 subtree layers
	/// </summary>
	XMSSMTSHA256H20D4 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA256H20D4),
	/// <summary>
	/// The XMSS-MT SHA256H40D2 parameter; (Medium Security) SHA2-256 with a tree height of 40, and 2 subtree layers
	/// </summary>
	XMSSMTSHA256H40D2 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA256H40D2),
	/// <summary>
	/// The XMSS-MT SHA256H40D4 parameter; (Medium Security) SHA2-256 with a tree height of 40, and 4 subtree layers
	/// </summary>
	XMSSMTSHA256H40D4 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA256H40D4),
	/// <summary>
	/// The XMSS-MT SHA256H40D8 parameter; (Medium Security) SHA2-256 with a tree height of 40, and 8 subtree layers
	/// </summary>
	XMSSMTSHA256H40D8 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA256H40D8),
	/// <summary>
	/// The XMSS-MT SHA256H60D3 parameter; (Medium Security) SHA2-256 with a tree height of 60, and 3 subtree layers
	/// </summary>
	XMSSMTSHA256H60D3 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA256H60D3),
	/// <summary>
	/// The XMSS-MT SHA256H60D6 parameter; (Medium Security) SHA2-256 with a tree height of 60, and 6 subtree layers
	/// </summary>
	XMSSMTSHA256H60D6 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA256H60D6),
	/// <summary>
	/// The XMSS-MT SHA256H60D12 parameter; (Medium Security) SHA2-256 with a tree height of 60, and 12 subtree layers
	/// </summary>
	XMSSMTSHA256H60D12 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA256H60D12),
	/// <summary>
	/// The XMSS-MT SHA512H20D2 parameter; (High Security) SHA2-512 with a tree height of 20, and 2 subtree layers
	/// </summary>
	XMSSMTSHA512H20D2 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA512H20D2),
	/// <summary>
	/// The XMSS-MT SHA512H20D4 parameter; (High Security) SHA2-512 with a tree height of 20, and 4 subtree layers
	/// </summary>
	XMSSMTSHA512H20D4 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA512H20D4),
	/// <summary>
	/// The XMSS-MT SHA512H40D2 parameter; (High Security) SHA2-512 with a tree height of 40, and 2 subtree layers
	/// </summary>
	XMSSMTSHA512H40D2 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA512H40D2),
	/// <summary>
	/// The XMSS-MT SHA512H40D4 parameter; (High Security) SHA2-512 with a tree height of 40, and 4 subtree layers
	/// </summary>
	XMSSMTSHA512H40D4 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA512H40D4),
	/// <summary>
	/// The XMSS-MT SHA512H40D8 parameter; (High Security) SHA2-512 with a tree height of 40, and 8 subtree layers
	/// </summary>
	XMSSMTSHA512H40D8 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA512H40D8),
	/// <summary>
	/// The XMSS-MT SHA512H60D3 parameter; (Highest Security) SHA2-512 with a tree height of 60, and 3 subtree layers
	/// </summary>
	XMSSMTSHA512H60D3 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA512H60D3),
	/// <summary>
	/// The XMSS-MT SHA512H60D6 parameter; (Highest Security) SHA2-512 with a tree height of 60, and 6 subtree layers
	/// </summary>
	XMSSMTSHA512H60D6 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA512H60D6),
	/// <summary>
	/// The XMSS-MT SHA512H60D12 parameter; (Highest Security) SHA2-512 with a tree height of 60, and 12 subtree layers
	/// </summary>
	XMSSMTSHA512H60D12 = static_cast<byte>(AsymmetricParameters::XMSSMTSHA512H60D12),
	/// <summary>
	/// The XMSS-MT SHAKE256H20D2 parameter; (Medium Security) SHAKE-256 with a tree height of 20, and 2 subtree layers
	/// </summary>
	XMSSMTSHAKE256H20D2 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE256H20D2),
	/// <summary>
	/// The XMSS-MT SHAKE256H20D4 parameter; (Medium Security) SHAKE-256 with a tree height of 20, and 4 subtree layers
	/// </summary>
	XMSSMTSHAKE256H20D4 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE256H20D4),
	/// <summary>
	/// The XMSS-MT SHAKE256H40D2 parameter; (Medium Security) SHAKE-256 with a tree height of 40, and 2 subtree layers
	/// </summary>
	XMSSMTSHAKE256H40D2 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE256H40D2),
	/// <summary>
	/// The XMSS-MT SHAKE256H40D4 parameter; (Medium Security) SHAKE-256 with a tree height of 40, and 4 subtree layers
	/// </summary>
	XMSSMTSHAKE256H40D4 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE256H40D4),
	/// <summary>
	/// The XMSS-MT SHAKE256H40D8 parameter; (Medium Security) SHAKE-256 with a tree height of 40, and 8 subtree layers
	/// </summary>
	XMSSMTSHAKE256H40D8 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE256H40D8),
	/// <summary>
	/// The XMSS-MT SHAKE256H60D3 parameter; (Medium Security) SHAKE-256 with a tree height of 60, and 3 subtree layers
	/// </summary>
	XMSSMTSHAKE256H60D3 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE256H60D3),
	/// <summary>
	/// The XMSS-MT SHAKE256H60D6 parameter; (Medium Security) SHAKE-256 with a tree height of 60, and 6 subtree layers
	/// </summary>
	XMSSMTSHAKE256H60D6 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE256H60D6),
	/// <summary>
	/// The XMSS-MT SHAKE256H60D12 parameter; (Medium Security) SHAKE-256 with a tree height of 60, and 12 subtree layers
	/// </summary>
	XMSSMTSHAKE256H60D12 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE256H60D12),
	/// <summary>
	/// The XMSS-MT SHAKE512H20D2 parameter; (High Security) SHAKE-512 with a tree height of 20, and 2 subtree layers
	/// </summary>
	XMSSMTSHAKE512H20D2 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE512H20D2),
	/// <summary>
	/// The XMSS-MT SHAKE512H20D4 parameter; (High Security) SHAKE-512 with a tree height of 20, and 4 subtree layers
	/// </summary>
	XMSSMTSHAKE512H20D4 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE512H20D4),
	/// <summary>
	/// The XMSS-MT SHAKE512H40D2 parameter; (High Security) SHAKE-512 with a tree height of 40, and 2 subtree layers
	/// </summary>
	XMSSMTSHAKE512H40D2 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE512H40D2),
	/// <summary>
	/// The XMSS-MT SHA512H40D4 parameter; (High Security) SHAKE-512 with a tree height of 40, and 4 subtree layers
	/// </summary>
	XMSSMTSHAKE512H40D4 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE512H40D4),
	/// <summary>
	/// The XMSS-MT SHA512H40D8 parameter; (High Security) SHAKE-512 with a tree height of 40, and 8 subtree layers
	/// </summary>
	XMSSMTSHAKE512H40D8 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE512H40D8),
	/// <summary>
	/// The XMSS-MT SHAKE512H60D3 parameter; (Highest Security) SHAKE-512 with a tree height of 60, and 3 subtree layers
	/// </summary>
	XMSSMTSHAKE512H60D3 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE512H60D3),
	/// <summary>
	/// The XMSS-MT SHAKE512H60D6 parameter; (Highest Security) SHAKE-512 with a tree height of 60, and 6 subtree layers
	/// </summary>
	XMSSMTSHAKE512H60D6 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE512H60D6),
	/// <summary>
	/// The XMSS-MT SHAKE512H60D12 parameter; (Highest Security) SHAKE-512 with a tree height of 60, and 12 subtree layers
	/// </summary>
	XMSSMTSHAKE512H60D12 = static_cast<byte>(AsymmetricParameters::XMSSMTSHAKE512H60D12)
};

class XmssParameterConvert
{
public:

	/// <summary>
	/// Derive the XmssParameters enumeration type-name from the formal string name
	/// </summary>
	/// 
	/// <param name="Name">The XmssParameters string name</param>
	///
	/// <returns>The matching XmssParameters enumeration type name</returns>
	static XmssParameters FromName(std::string &Name);

	/// <summary>
	/// Derive the SphincsParameters formal string name from the enumeration name
	/// </summary>
	/// 
	/// <param name="Enumeral">The SphincsParameters enumeration member</param>
	///
	/// <returns>The matching XmssParameters string name</returns>
	static std::string ToName(XmssParameters Enumeral);
};

NAMESPACE_ENUMERATIONEND
#endif
