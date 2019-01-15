#include "SymmetricKeyGeneratorTest.h"
#include "RandomUtils.h"
#include "../CEX/CpuDetect.h"
#include "../CEX/CryptoGeneratorException.h"
#include "../CEX/SecureRandom.h"
#include "../CEX/SymmetricKeyGenerator.h"

namespace Test
{
	using Exception::CryptoGeneratorException;
	using Prng::SecureRandom;
	using Cipher::SymmetricKeyGenerator;
	using Cipher::SymmetricKey;
	using Cipher::SymmetricKeySize;
	using Cipher::SymmetricSecureKey;

	const std::string SymmetricKeyGeneratorTest::CLASSNAME = "SymmetricKeyGeneratorTest";
	const std::string SymmetricKeyGeneratorTest::DESCRIPTION = "SymmetricKeyGenerator test; verifies initialization and access methods.";
	const std::string SymmetricKeyGeneratorTest::SUCCESS = "SUCCESS! All SymmetricKeyGenerator tests have executed succesfully.";

	SymmetricKeyGeneratorTest::SymmetricKeyGeneratorTest()
		:
		m_progressEvent()
	{
	}

	SymmetricKeyGeneratorTest::~SymmetricKeyGeneratorTest()
	{
	}

	const std::string SymmetricKeyGeneratorTest::Description()
	{
		return DESCRIPTION;
	}

	TestEventHandler &SymmetricKeyGeneratorTest::Progress()
	{
		return m_progressEvent;
	}

	std::string SymmetricKeyGeneratorTest::Run()
	{
		try
		{
			Evaluate();
			OnProgress(std::string("SymmetricKeyGenerator: Passed random output tests.."));
			Exception();
			OnProgress(std::string("SymmetricKeyGenerator: Passed exception handling tests.."));
			Stress();
			OnProgress(std::string("SymmetricKeyGenerator: Passed stress tests.."));

			return SUCCESS;
		}
		catch (TestException const &ex)
		{
			throw TestException(CLASSNAME, ex.Function(), ex.Origin(), ex.Message());
		}
		catch (std::exception const &ex)
		{
			throw TestException(CLASSNAME, std::string("Unknown Origin"), std::string(ex.what()));
		}
	}

	void SymmetricKeyGeneratorTest::Evaluate()
	{
		std::vector<byte> tmps(SAMPLE_SIZE);
		SymmetricKeyGenerator kgen256(Enumeration::SecurityPolicy::SPL256, Enumeration::Providers::CSP);
		OnProgress(std::string("Testing pseudo-random generation with a 256-bit security policy using the system provider"));
		kgen256.Generate(tmps, 0, tmps.size());
		Evaluate(kgen256.Name(), tmps);

		std::memset(tmps.data(), 0x00, tmps.size());
		SymmetricKeyGenerator kgen512(Enumeration::SecurityPolicy::SPL512, Enumeration::Providers::CSP);
		OnProgress(std::string("Testing pseudo-random generation with a 512-bit security policy using the system provider"));
		kgen512.Generate(tmps, 0, tmps.size());
		Evaluate(kgen512.Name(), tmps);

		std::memset(tmps.data(), 0x00, tmps.size());
		SymmetricKeyGenerator kgen1024(Enumeration::SecurityPolicy::SPL1024, Enumeration::Providers::CSP);
		OnProgress(std::string("Testing pseudo-random generation with a 1024-bit security policy using the system provider"));
		kgen1024.Generate(tmps, 0, tmps.size());
		Evaluate(kgen1024.Name(), tmps);
	}

	void SymmetricKeyGeneratorTest::Exception()
	{
		// test initialization with invalid provider type
		try
		{
			SymmetricKeyGenerator kgen(Enumeration::SecurityPolicy::SPL256, Enumeration::Providers::None);

			throw TestException(std::string("Exception"), kgen.Name(), std::string("Exception handling failure! -SE1"));
		}
		catch (CryptoGeneratorException const &)
		{
		}
		catch (TestException const &)
		{
			throw;
		}

		// test initialization with invalid policy
		try
		{
			SymmetricKeyGenerator kgen(Enumeration::SecurityPolicy::None);

			throw TestException(std::string("Exception"), kgen.Name(), std::string("Exception handling failure! -SE2"));
		}
		catch (CryptoGeneratorException const &)
		{
		}
		catch (TestException const &)
		{
			throw;
		}

		// test initialization with invalid customization string
		try
		{
			std::vector<byte> tmps(0);
			SymmetricKeyGenerator kgen(Enumeration::SecurityPolicy::SPL256, tmps);

			throw TestException(std::string("Exception"), kgen.Name(), std::string("Exception handling failure! -SE3"));
		}
		catch (CryptoGeneratorException const &)
		{
		}
		catch (TestException const &)
		{
			throw;
		}

		// test generating a secure-key with a zero-sized key
		try
		{
			SymmetricKeyGenerator kgen(Enumeration::SecurityPolicy::SPL256, Enumeration::Providers::CSP);
			SymmetricKeySize ks(0, 0, 0);
			SymmetricSecureKey* sk = kgen.GetSecureKey(ks);
			delete sk;

			throw TestException(std::string("Exception"), kgen.Name(), std::string("Exception handling failure! -SE4"));
		}
		catch (CryptoGeneratorException const &)
		{
		}
		catch (TestException const &)
		{
			throw;
		}

		// test generating a key with a zero-sized key
		try
		{
			SymmetricKeyGenerator kgen(Enumeration::SecurityPolicy::SPL256, Enumeration::Providers::CSP);
			SymmetricKeySize ks(0, 0, 0);
			SymmetricKey* sk = kgen.GetSymmetricKey(ks);
			delete sk;

			throw TestException(std::string("Exception"), kgen.Name(), std::string("Exception handling failure! -SE5"));
		}
		catch (CryptoGeneratorException const &)
		{
		}
		catch (TestException const &)
		{
			throw;
		}
	}

	void SymmetricKeyGeneratorTest::Stress()
	{
		std::vector<byte> otp;
		SecureRandom rnd;
		size_t i;
		SymmetricKeyGenerator kgen256(Enumeration::SecurityPolicy::SPL256, Enumeration::Providers::CSP);

		otp.reserve(MAXM_ALLOC);

		for (i = 0; i < TEST_CYCLES; ++i)
		{
			otp.resize(static_cast<size_t>(rnd.NextUInt32(MAXM_ALLOC, MINM_ALLOC)));
			
			try
			{ 
				kgen256.Generate(otp, 0, otp.size());
			}
			catch (const std::exception&)
			{
				throw TestException(std::string("Stress"), rnd.Name(), std::string("Stress test random generation failure! -SG1"));
			}

			try
			{
				SymmetricKeySize ks(rnd.NextUInt32(MAXM_ALLOC, MINM_ALLOC), rnd.NextUInt32(MAXM_ALLOC, MINM_ALLOC), rnd.NextUInt32(MAXM_ALLOC, MINM_ALLOC));
				SymmetricSecureKey* sk = kgen256.GetSecureKey(ks);
				delete sk;
			}
			catch (const std::exception&)
			{
				throw TestException(std::string("Stress"), rnd.Name(), std::string("Stress secure key generation failure! -SG2"));
			}

			try
			{
				SymmetricKeySize ks(rnd.NextUInt32(MAXM_ALLOC, MINM_ALLOC), rnd.NextUInt32(MAXM_ALLOC, MINM_ALLOC), rnd.NextUInt32(MAXM_ALLOC, MINM_ALLOC));
				SymmetricKey* sk = kgen256.GetSymmetricKey(ks);
				delete sk;
			}
			catch (const std::exception&)
			{
				throw TestException(std::string("Stress"), rnd.Name(), std::string("Stress key generation failure! -SG3"));
			}
		}
	}

	void SymmetricKeyGeneratorTest::Evaluate(const std::string &Name, std::vector<byte> &Sample)
	{
		RandomUtils::Evaluate(Name, Sample);
	}

	void SymmetricKeyGeneratorTest::OnProgress(const std::string &Data)
	{
		m_progressEvent(Data);
	}
}
