#include "AsymmetricParameters.h"

NAMESPACE_ENUMERATION

std::string AsymmetricTransformConvert::ToName(AsymmetricParameters Enumeral)
{
	std::string name("");

	switch (Enumeral)
	{
	case AsymmetricParameters::DLMS1N256Q8380417:
		name = std::string("DLMS1N256Q8380417");
		break;
	case AsymmetricParameters::DLMS2N256Q8380417:
		name = std::string("DLMS2N256Q8380417");
		break;
	case AsymmetricParameters::DLMS3N256Q8380417:
		name = std::string("DLMS3N256Q8380417");
		break;
	case AsymmetricParameters::MLWES1Q3329N256:
		name = std::string("MLWES1Q3329N256");
		break;
	case AsymmetricParameters::MLWES2Q3329N256:
		name = std::string("MLWES2Q3329N256");
		break;
	case AsymmetricParameters::MLWES3Q3329N256:
		name = std::string("MLWES3Q3329N256");
		break;
	case AsymmetricParameters::MPKCS1N4096T62:
		name = std::string("MPKCS1N4096T62");
		break;
	case AsymmetricParameters::MPKCS1N6960T119:
		name = std::string("MPKCS1N6960T119");
		break;
	case AsymmetricParameters::MPKCS1N8192T128:
		name = std::string("MPKCS1N8192T128");
		break;
	case AsymmetricParameters::NTRUS1SQ4621N653:
		name = std::string("NTRUS1SQ4621N653");
		break;
	case AsymmetricParameters::NTRUS2SQ4591N761:
		name = std::string("NTRUS2SQ4591N761");
		break;
	case AsymmetricParameters::NTRUS3SQ5167N857:
		name = std::string("NTRUS3SQ5167N857");
		break;
	case AsymmetricParameters::RLWES1Q12289N1024:
		name = std::string("RLWES1Q12289N1024");
		break;
	case AsymmetricParameters::RLWES2Q12289N2048:
		name = std::string("RLWES2Q12289N2048");
		break;
	case AsymmetricParameters::SPXS1S128SHAKE:
		name = std::string("SPXS1S128SHAKE");
		break;
	case AsymmetricParameters::SPXS2S192SHAKE:
		name = std::string("SPXS2S192SHAKE");
		break;
	case AsymmetricParameters::SPXS3S256SHAKE:
		name = std::string("SPXS3S256SHAKE");
		break;
	case AsymmetricParameters::XMSSSHA256H10:
		name = std::string("XMSSSHA256H10");
		break;
	case AsymmetricParameters::XMSSSHA256H16:
		name = std::string("XMSSSHA256H16");
		break;
	case AsymmetricParameters::XMSSSHA256H20:
		name = std::string("XMSSSHA256H20");
		break;
	case AsymmetricParameters::XMSSSHA512H10:
		name = std::string("XMSSSHA512H10");
		break;
	case AsymmetricParameters::XMSSSHA512H16:
		name = std::string("XMSSSHA512H16");
		break;
	case AsymmetricParameters::XMSSSHA512H20:
		name = std::string("XMSSSHA512H20");
		break;
	case AsymmetricParameters::XMSSSHAKE256H10:
		name = std::string("XMSSSHAKE256H10");
		break;
	case AsymmetricParameters::XMSSSHAKE256H16:
		name = std::string("XMSSSHAKE256H16");
		break;
	case AsymmetricParameters::XMSSSHAKE256H20:
		name = std::string("XMSSSHAKE256H20");
		break;
	case AsymmetricParameters::XMSSSHAKE512H10:
		name = std::string("XMSSSHAKE512H10");
		break;
	case AsymmetricParameters::XMSSSHAKE512H16:
		name = std::string("XMSSSHAKE512H16");
		break;	
	case AsymmetricParameters::XMSSSHAKE512H20:
		name = std::string("XMSSSHAKE512H20");
		break;
	case AsymmetricParameters::XMSSMTSHA256H20D2:
		name = std::string("XMSSMTSHA256H20D2");
		break;
	case AsymmetricParameters::XMSSMTSHA256H40D2:
		name = std::string("XMSSMTSHA256H40D2");
		break;
	case AsymmetricParameters::XMSSMTSHA256H20D4:
		name = std::string("XMSSMTSHA256H20D4");
		break;
	case AsymmetricParameters::XMSSMTSHA256H40D4:
		name = std::string("XMSSMTSHA256H40D4");
		break;
	case AsymmetricParameters::XMSSMTSHA256H40D8:
		name = std::string("XMSSMTSHA256H40D8");
		break;
	case AsymmetricParameters::XMSSMTSHA256H60D3:
		name = std::string("XMSSMTSHA256H60D3");
		break;
	case AsymmetricParameters::XMSSMTSHA256H60D6:
		name = std::string("XMSSMTSHA256H60D6");
		break;
	case AsymmetricParameters::XMSSMTSHA256H60D12:
		name = std::string("XMSSMTSHA256H60D12");
		break;
	case AsymmetricParameters::XMSSMTSHA512H20D2:
		name = std::string("XMSSMTSHA512H20D2");
		break;
	case AsymmetricParameters::XMSSMTSHA512H20D4:
		name = std::string("XMSSMTSHA512H20D4");
		break;
	case AsymmetricParameters::XMSSMTSHA512H40D2:
		name = std::string("XMSSMTSHA512H40D2");
		break;
	case AsymmetricParameters::XMSSMTSHA512H40D4:
		name = std::string("XMSSMTSHA512H40D4");
		break;
	case AsymmetricParameters::XMSSMTSHA512H40D8:
		name = std::string("XMSSMTSHA512H40D8");
		break;
	case AsymmetricParameters::XMSSMTSHA512H60D3:
		name = std::string("XMSSMTSHA512H60D3");
		break;
	case AsymmetricParameters::XMSSMTSHA512H60D6:
		name = std::string("XMSSMTSHA512H60D6");
		break;
	case AsymmetricParameters::XMSSMTSHA512H60D12:
		name = std::string("XMSSMTSHA512H60D12");
		break;
	case AsymmetricParameters::XMSSMTSHAKE256H20D2:
		name = std::string("XMSSMTSHAKE256H20D2");
		break;
	case AsymmetricParameters::XMSSMTSHAKE256H20D4:
		name = std::string("XMSSMTSHAKE256H20D4");
		break;
	case AsymmetricParameters::XMSSMTSHAKE256H40D2:
		name = std::string("XMSSMTSHAKE256H40D2");
		break;
	case AsymmetricParameters::XMSSMTSHAKE256H40D4:
		name = std::string("XMSSMTSHAKE256H40D4");
		break;
	case AsymmetricParameters::XMSSMTSHAKE256H40D8:
		name = std::string("XMSSMTSHAKE256H40D8");
		break;	
	case AsymmetricParameters::XMSSMTSHAKE256H60D3:
		name = std::string("XMSSMTSHAKE256H60D3");
		break;
	case AsymmetricParameters::XMSSMTSHAKE256H60D6:
		name = std::string("XMSSMTSHAKE256H60D6");
		break;
	case AsymmetricParameters::XMSSMTSHAKE256H60D12:
		name = std::string("XMSSMTSHAKE256H60D12");
		break;
	case AsymmetricParameters::XMSSMTSHAKE512H20D2:
		name = std::string("XMSSMTSHAKE512H20D2");
		break;
	case AsymmetricParameters::XMSSMTSHAKE512H20D4:
		name = std::string("XMSSMTSHAKE512H20D4");
		break;
	case AsymmetricParameters::XMSSMTSHAKE512H40D2:
		name = std::string("XMSSMTSHAKE512H40D2");
		break;
	case AsymmetricParameters::XMSSMTSHAKE512H40D4:
		name = std::string("XMSSMTSHAKE512H40D4");
		break;
	case AsymmetricParameters::XMSSMTSHAKE512H40D8:
		name = std::string("XMSSMTSHAKE512H40D8");
		break;
	case AsymmetricParameters::XMSSMTSHAKE512H60D3:
		name = std::string("XMSSMTSHAKE512H60D3");
		break;
	case AsymmetricParameters::XMSSMTSHAKE512H60D6:
		name = std::string("XMSSMTSHAKE512H60D6");
		break;
	case AsymmetricParameters::XMSSMTSHAKE512H60D12:
		name = std::string("XMSSMTSHAKE512H60D12");
		break;
	default:
		name = std::string("None");
		break;
	}

	return name;
}

AsymmetricParameters AsymmetricTransformConvert::FromName(std::string &Name)
{
	AsymmetricParameters tname;

	if (Name == std::string("DLMS1N256Q8380417"))
	{
		tname = AsymmetricParameters::DLMS1N256Q8380417;
	}
	else if (Name == std::string("DLMS2N256Q8380417"))
	{
		tname = AsymmetricParameters::DLMS2N256Q8380417;
	}
	else if (Name == std::string("DLMS3N256Q8380417"))
	{
		tname = AsymmetricParameters::DLMS3N256Q8380417;
	}
	else if (Name == std::string("MLWES1Q3329N256"))
	{
		tname = AsymmetricParameters::MLWES1Q3329N256;
	}
	else if (Name == std::string("MLWES2Q3329N256"))
	{
		tname = AsymmetricParameters::MLWES2Q3329N256;
	}
	else if (Name == std::string("MLWES3Q3329N256"))
	{
		tname = AsymmetricParameters::MLWES3Q3329N256;
	}
	else if (Name == std::string("MPKCS1N4096T62"))
	{
		tname = AsymmetricParameters::MPKCS1N4096T62;
	}
	else if (Name == std::string("MPKCS1N6960T119"))
	{
		tname = AsymmetricParameters::MPKCS1N6960T119;
	}
	else if (Name == std::string("MPKCS1N8192T128"))
	{
		tname = AsymmetricParameters::MPKCS1N8192T128;
	}
	else if (Name == std::string("NTRUS1SQ4621N653"))
	{
		tname = AsymmetricParameters::NTRUS1SQ4621N653;
	}
	else if (Name == std::string("NTRUS2SQ4591N761"))
	{
		tname = AsymmetricParameters::NTRUS2SQ4591N761;
	}
	else if (Name == std::string("NTRUS3SQ5167N857"))
	{
		tname = AsymmetricParameters::NTRUS3SQ5167N857;
	}
	else if (Name == std::string("RLWES1Q12289N1024"))
	{
		tname = AsymmetricParameters::RLWES1Q12289N1024;
	}
	else if (Name == std::string("RLWES2Q12289N2048"))
	{
		tname = AsymmetricParameters::RLWES2Q12289N2048;
	}
	else if (Name == std::string("SPXS1S128SHAKE"))
	{
		tname = AsymmetricParameters::SPXS1S128SHAKE;
	}
	else if (Name == std::string("SPXS2S192SHAKE"))
	{
		tname = AsymmetricParameters::SPXS2S192SHAKE;
	}
	else if (Name == std::string("SPXS3S256SHAKE"))
	{
		tname = AsymmetricParameters::SPXS3S256SHAKE;
	}
	else if (Name == std::string("XMSSSHA256H10"))
	{
		tname = AsymmetricParameters::XMSSSHA256H16;
	}
	else if (Name == std::string("XMSSSHA256H16"))
	{
		tname = AsymmetricParameters::XMSSSHA256H16;
	}
	else if (Name == std::string("XMSSSHA256H20"))
	{
		tname = AsymmetricParameters::XMSSSHA256H20;
	}
	else if (Name == std::string("XMSSSHA512H10"))
	{
		tname = AsymmetricParameters::XMSSSHA512H10;
	}
	else if (Name == std::string("XMSSSHA512H16"))
	{
		tname = AsymmetricParameters::XMSSSHA512H16;
	}
	else if (Name == std::string("XMSSSHA512H20"))
	{
		tname = AsymmetricParameters::XMSSSHA512H20;
	}
	else if (Name == std::string("XMSSSHAKE256H10"))
	{
		tname = AsymmetricParameters::XMSSSHAKE256H10;
	}
	else if (Name == std::string("XMSSSHAKE256H16"))
	{
		tname = AsymmetricParameters::XMSSSHAKE256H16;
	}
	else if (Name == std::string("XMSSSHAKE256H20"))
	{
		tname = AsymmetricParameters::XMSSSHAKE256H20;
	}
	else if (Name == std::string("XMSSSHAKE512H10"))
	{
		tname = AsymmetricParameters::XMSSSHAKE512H10;
	}
	else if (Name == std::string("XMSSSHAKE512H16"))
	{
		tname = AsymmetricParameters::XMSSSHAKE512H16;
	}
	else if (Name == std::string("XMSSSHAKE512H20"))
	{
		tname = AsymmetricParameters::XMSSSHAKE512H20;
	}
	else if (Name == std::string("XMSSMTSHA256H20D2"))
	{
		tname = AsymmetricParameters::XMSSMTSHA256H20D2;
	}
	else if (Name == std::string("XMSSMTSHA256H20D4"))
	{
		tname = AsymmetricParameters::XMSSMTSHA256H20D4;
	}
	else if (Name == std::string("XMSSMTSHA256H40D2"))
	{
		tname = AsymmetricParameters::XMSSMTSHA256H40D2;
	}
	else if (Name == std::string("XMSSMTSHA256H40D4"))
	{
		tname = AsymmetricParameters::XMSSMTSHA256H40D4;
	}
	else if (Name == std::string("XMSSMTSHA256H40D8"))
	{
		tname = AsymmetricParameters::XMSSMTSHA256H40D8;
	}
	else if (Name == std::string("XMSSMTSHA256H60D3"))
	{
		tname = AsymmetricParameters::XMSSMTSHA256H60D3;
	}
	else if (Name == std::string("XMSSMTSHA256H60D6"))
	{
		tname = AsymmetricParameters::XMSSMTSHA256H60D6;
	}
	else if (Name == std::string("XMSSMTSHA256H60D12"))
	{
		tname = AsymmetricParameters::XMSSMTSHA256H60D12;
	}
	else if (Name == std::string("XMSSMTSHA512H20D2"))
	{
		tname = AsymmetricParameters::XMSSMTSHA512H20D2;
	}
	else if (Name == std::string("XMSSMTSHA512H20D4"))
	{
		tname = AsymmetricParameters::XMSSMTSHA512H20D4;
	}
	else if (Name == std::string("XMSSMTSHA512H40D2"))
	{
		tname = AsymmetricParameters::XMSSMTSHA512H40D2;
	}
	else if (Name == std::string("XMSSMTSHA512H40D4"))
	{
		tname = AsymmetricParameters::XMSSMTSHA512H40D4;
	}
	else if (Name == std::string("XMSSMTSHA512H40D8"))
	{
		tname = AsymmetricParameters::XMSSMTSHA512H40D8;
	}
	else if (Name == std::string("XMSSMTSHA512H60D3"))
	{
		tname = AsymmetricParameters::XMSSMTSHA512H60D3;
	}
	else if (Name == std::string("XMSSMTSHA512H60D6"))
	{
		tname = AsymmetricParameters::XMSSMTSHA512H60D6;
	}
	else if (Name == std::string("XMSSMTSHA512H60D12"))
	{
		tname = AsymmetricParameters::XMSSMTSHA512H60D12;
	}
	else if (Name == std::string("XMSSMTSHAKE256H20D2"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE256H20D2;
	}
	else if (Name == std::string("XMSSMTSHAKE256H20D4"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE256H20D4;
	}
	else if (Name == std::string("XMSSMTSHAKE256H40D2"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE256H40D2;
	}
	else if (Name == std::string("XMSSMTSHAKE256H40D4"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE256H40D4;
	}
	else if (Name == std::string("XMSSMTSHAKE256H40D8"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE256H40D8;
	}
	else if (Name == std::string("XMSSMTSHAKE256H60D3"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE256H60D3;
	}
	else if (Name == std::string("XMSSMTSHAKE256H60D6"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE256H60D6;
	}
	else if (Name == std::string("XMSSMTSHAKE256H60D12"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE256H60D12;
	}
	else if (Name == std::string("XMSSMTSHAKE512H20D2"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE512H20D2;
	}
	else if (Name == std::string("XMSSMTSHAKE512H20D4"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE512H20D4;
	}
	else if (Name == std::string("XMSSMTSHAKE512H40D2"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE512H40D2;
	}
	else if (Name == std::string("XMSSMTSHAKE512H40D4"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE512H40D4;
	}
	else if (Name == std::string("XMSSMTSHAKE512H40D8"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE512H40D8;
	}
	else if (Name == std::string("XMSSMTSHAKE512H60D3"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE512H60D3;
	}
	else if (Name == std::string("XMSSMTSHAKE512H60D6"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE512H60D6;
	}
	else if (Name == std::string("XMSSMTSHAKE512H60D12"))
	{
		tname = AsymmetricParameters::XMSSMTSHAKE512H60D12;
	}
	else
	{
		tname = AsymmetricParameters::None;
	}

	return tname;
}

NAMESPACE_ENUMERATIONEND