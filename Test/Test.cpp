
// TRAJECTORY
//
// ##SCHEDULE FOR 1.0.0.8 RELEASE ###
// ##Eta is February 14, 2019 ##
//
// Authenticated stream ciphers MCS,ThreeFish-256/512/1024, and CSX256/512	- done
// Update secure key mechanisms and integrate internally						- started
// Full security, documentation, and code review cycles							- started
// Begin optimization and review of asymmetric primitives						- 
// Create accompanying website and publish documentation						- 
//
// New in Current Release 1.0.0.7 (version A7):
// The Dilithium asymmetric signature scheme
// The SPHINCS + asymmetric signature scheme
// The NTRU-Prime asymmetric cipher
// The cSHAKE XOF function added as an HX symmetric cipher key expansion option
// Update of all base asymmetric ciphers and signature schemes to match NIST PQ entries
// Message digests updated for performance and security
// Integrated an optional built - in authentication generator(HMAC / KMAC) to each stream cipher
// Added the Authenticate and Encrypt and AEAD stream cipher implementation(Authenticated Cipher Stream : MCS).
// Threefish 256/512/1024 authenticated stream ciphers
// CSX256 - P20 and CSX512 - P80 authenticated stream ciphers
//
// ### Planned Release 1.1.0.1 ###
//
// RingLWE							-added
// McEliece							-added
// ModuleLWE						-added
// NTRUPrime						-added
// SPHINCS+							-added
// Dilithium						-added
// Optimization						-started
// AVX512 integration				-started
// Compiler compatibility			-started
// Android/iOS/Linux compatibility	-started
// DLL API (.NET/Java)				-not started
//
// ### Planned Release 1.2.0.1 ###
//
// Direct Trust Model DTM-KEX (external?)
// Post-Quantum Secure DNS, PQ-SECDNS (external)
// Shared Trust Model STM-KEX (external?)
//
// ### Style Rules ###
// 
// namespace: Single capaitalized word, ex. Network::
// class name: Pascal case description, maximum of two words, ex. SymmetricKey()
// function name: Pascal case, maximum of two words, ex. Initialize()
// function parameters: Pascal case, maximum of two words, ex. Initialize(ISymmetricKey &Key)
// global variable: Camel Case, with the prefix 'g_', ex. g_globalState
// class variable: Camel Case, with the prefix 'm_', ex. m_classState
// function variable: a single word or 2 Camel case words in abbreviated form, ex. ctr, or, bctr
// global const: All Caps, a total of three words with the 'CEX_' prefix, ex. CEX_GLOBAL_CONSTANT
// class const: All Caps, a total of two words, ex. CLASS_CONSTANT
// function const: Two capitalized and abbreviated 3 letter words with no underscore divider, ex. FNCCST
//
// 1.0.0.8 TODOs
// rewrite padding modes for constant time						-done
// Rewrite the symmetric secure key with authentication			-done
// Add an asymmetric secure key									-done
// Reorganize the namespace (key and cipher)					-done
// Review all classes for constant time operations				-done
// Update all documentation to current implementations			-done
// Add *nix random provider functions							-done

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "../CEX/CpuDetect.h"
#include "../Test/TestFiles.h"
#include "../Test/TestUtils.h"
#include "../Test/ACPTest.h"
#include "../Test/MCSTest.h"
#include "../Test/AeadTest.h"
#include "../Test/AesAvsTest.h"
#include "../Test/AsymmetricKeyTest.h"
#include "../Test/AsymmetricSpeedTest.h"
#include "../Test/BCGTest.h"
#include "../Test/BCRTest.h"
#include "../Test/Blake2Test.h"
#include "../Test/ChaChaTest.h"
#include "../Test/CipherModeTest.h"
#include "../Test/CipherSpeedTest.h"
#include "../Test/CipherStreamTest.h"
#include "../Test/CJPTest.h"
#include "../Test/CMACTest.h"
#include "../Test/ConsoleUtils.h"
#include "../Test/CSGTest.h"
#include "../Test/CSPTest.h"
#include "../Test/CSRTest.h"
#include "../Test/DigestSpeedTest.h"
#include "../Test/DigestStreamTest.h"
#include "../Test/DilithiumTest.h"
#include "../Test/ECPTest.h"
#include "../Test/GMACTest.h"
#include "../Test/HCRTest.h"
#include "../Test/KDF2Test.h"
#include "../Test/KeccakTest.h"
#include "../Test/KMACTest.h"
#include "../Test/HKDFTest.h"
#include "../Test/HMACTest.h"
#include "../Test/HCGTest.h"
#include "../Test/ITest.h"
#include "../Test/MacStreamTest.h"
#include "../Test/McElieceTest.h"
#include "../Test/MemUtilsTest.h"
#include "../Test/ModuleLWETest.h"
#include "../Test/NTRUTest.h"
#include "../Test/PaddingTest.h"
#include "../Test/ParallelModeTest.h"
#include "../Test/PBKDF2Test.h"
#include "../Test/Poly1305Test.h"
#include "../Test/RandomOutputTest.h"
#include "../Test/RCSTest.h"
#include "../Test/RDPTest.h"
#include "../Test/RijndaelTest.h"
#include "../Test/RingLWETest.h"
#include "../Test/SCRYPTTest.h"
#include "../Test/SecureStreamTest.h"
#include "../Test/SerpentTest.h"
#include "../Test/Sha2Test.h"
#include "../Test/SimdSpeedTest.h"
#include "../Test/SimdWrapperTest.h"
#include "../Test/SHAKETest.h"
#include "../Test/SkeinTest.h"
#include "../Test/SphincsTest.h"
#include "../Test/SymmetricKeyGeneratorTest.h"
#include "../Test/SymmetricKeyTest.h"
#include "../Test/ThreefishTest.h"
#include "../Test/UtilityTest.h"
#include "../Test/XMSSTest.h"

using namespace Test;

void CpuCheck()
{
	CpuDetect detect;
	ConsoleUtils::WriteLine("L1 cache size: " + std::to_string(detect.L1CacheSize()));
	ConsoleUtils::WriteLine("Total L1 cache size: " + std::to_string(detect.L1CacheTotal()));
	ConsoleUtils::WriteLine("L1 cache line size: " + std::to_string(detect.L1CacheLineSize()));
	ConsoleUtils::WriteLine("L2 cache size: " + std::to_string(detect.L2CacheSize()));
	ConsoleUtils::WriteLine("Physical cores: " + std::to_string(detect.PhysicalCores()));
	ConsoleUtils::WriteLine("Virtual cores: " + std::to_string(detect.VirtualCores()));
	ConsoleUtils::WriteLine("HyperThreading: " + std::to_string(detect.HyperThread()));
	ConsoleUtils::WriteLine("AES-NI: " + std::to_string(detect.AESNI()));
	ConsoleUtils::WriteLine("AVX: " + std::to_string(detect.AVX()));
	ConsoleUtils::WriteLine("AVX2: " + std::to_string(detect.AVX2()));
	ConsoleUtils::WriteLine("CMUL: " + std::to_string(detect.CMUL()));
	ConsoleUtils::WriteLine("RDRAND: " + std::to_string(detect.RDRAND()));
	ConsoleUtils::WriteLine("RDTSCP: " + std::to_string(detect.RDTSCP()));
	ConsoleUtils::WriteLine("SHA: " + std::to_string(detect.SHA()));
	ConsoleUtils::WriteLine("SSE2: " + std::to_string(detect.SSE2()));
	ConsoleUtils::WriteLine("SSE3: " + std::to_string(detect.SSE3()));
	ConsoleUtils::WriteLine("SSSE3: " + std::to_string(detect.SSSE3()));
	ConsoleUtils::WriteLine("SSE41: " + std::to_string(detect.SSE41()));
	ConsoleUtils::WriteLine("SSE42: " + std::to_string(detect.SSE42()));
	ConsoleUtils::WriteLine("XOP: " + std::to_string(detect.XOP()));
	ConsoleUtils::WriteLine("");
}

std::string GetResponse()
{
	std::string resp = "";

	try
	{
		std::getline(std::cin, resp);
	}
	catch (std::exception&)
	{
	}

	return resp;
}

std::string GetTime()
{
	time_t res = time(nullptr);
	char str[26];
	ctime_s(str, sizeof(str), &res);

	return std::string(str);
}

void PrintHeader(std::string Data, std::string Decoration = "***")
{
	ConsoleUtils::WriteLine(Decoration + Data + Decoration);
}

void PrintRandom(size_t Lines)
{
	std::string sample;

	for (size_t i = 0; i < Lines; ++i)
	{
		sample = TestUtils::GetRandomString(120);
		ConsoleUtils::WriteLine(sample);
	}

	ConsoleUtils::WriteLine("");
}

void PrintTitle()
{
	ConsoleUtils::WriteLine("************************************************");
	ConsoleUtils::WriteLine("* CEX++ Version 1.0.0.7: CEX Library in C++    *");
	ConsoleUtils::WriteLine("*                                              *");
	ConsoleUtils::WriteLine("* Release:   v1.0.0.7x (A7)                    *");
	ConsoleUtils::WriteLine("* License:   GPLv3                             *");
	ConsoleUtils::WriteLine("* Date:      July 16, 2019                     *");
	ConsoleUtils::WriteLine("* Contact:   develop@vtdev.com                 *");
	ConsoleUtils::WriteLine("************************************************");
	ConsoleUtils::WriteLine("");
}

bool TestConfirm(std::string Message)
{
	const std::string CONFIRM = "y";
	const std::string CONFIRML = "Y";
	std::string resp;
	bool state;

	ConsoleUtils::WriteLine(Message);

	state = false;
	resp = GetResponse();

	if (resp.find(CONFIRM) != std::string::npos || resp.find(CONFIRML) != std::string::npos)
	{
		state = true;
	}

	return state;
}

void Terminate()
{
	std::string resp;

	PrintHeader("An error has occurred! Press any key to close..", "");
	TestUtils::WaitForInput();
	exit(0);
}

void TestRun(ITest* Test)
{
	try
	{
		TestEventHandler handler;
		Test->Progress() += &handler;
		ConsoleUtils::WriteLine(Test->Description());
		ConsoleUtils::WriteLine(Test->Run());
		Test->Progress() -= &handler;
		ConsoleUtils::WriteLine("");

		delete Test;
	}
	catch (TestException const &ex)
	{
		ConsoleUtils::WriteLine("");
		ConsoleUtils::WriteLine("*** ERROR CONDITION ***");
		ConsoleUtils::WriteLine(std::string("Class: ") + ex.Location());
		ConsoleUtils::WriteLine(std::string("Function: ") + ex.Function());
		ConsoleUtils::WriteLine(std::string("Origin: ") + ex.Origin());
		ConsoleUtils::WriteLine(std::string("Message: ") + ex.Message());
		ConsoleUtils::WriteLine(std::string("Time: ") + GetTime());
		ConsoleUtils::WriteLine("");

		ConsoleUtils::WriteLine("");
		ConsoleUtils::WriteLine("Continue Testing? Press 'Y' to continue, all other keys abort..");

		std::string resp;

		if (!TestConfirm(resp))
		{
			Terminate();
		}
	}
}

int main()
{
	bool hasAes;
	bool hasAvx;
	bool hasAvx2;
	bool isx86emu;
	bool is64;

	ConsoleUtils::SizeConsole();
	PrintTitle();

#if !defined(_OPENMP)
	PrintHeader("Warning! This library requires OpenMP support, the test can not coninue!");
	PrintHeader("An error has occurred! Press any key to close..", "");
	TestUtils::WaitForInput();

	return 0;
#endif

	std::string data("");

	try
	{
		TestUtils::Read(TestFiles::AESAVS::AESAVSKEY128, data);
	}
	catch (std::exception&) 
	{
		data = "";
	}

	if (data.size() == 0)
	{
		PrintHeader("Warning! Could not find the cipher test vector KAT files!");
		PrintHeader("The Win/Test/Vectors folder must be in the executables path.", "");
		PrintHeader("An error has occurred! Press any key to close..", "");
		TestUtils::WaitForInput();

		return 0;
	}

	hasAes = false;
	hasAvx = false;
	hasAvx2 = false;
	isx86emu = false;
	is64 = false;

	try
	{
		CpuDetect detect;

		hasAes = detect.AESNI();
		hasAvx = detect.AVX();
		hasAvx2 = detect.AVX2();
		isx86emu = detect.IsX86Emulation();
		is64 = detect.IsX64();
	}
	catch (std::exception&)
	{
		PrintHeader("An error has occurred! This platform does not support cpudetect!", "");
		TestUtils::WaitForInput();

		return 0;
	}

#if (!defined(_M_X64) && !defined(__x86_64__) && !defined(_DEBUG))
	if (is64 || isx86emu)
	{
		PrintHeader("Warning! Compiling x86/Release on a 64bit system will cause memory alignment errors.", "");
		PrintHeader("To test x86/Release, compile on a true x86 system, or run in x86/Debug mode.", "");
		PrintHeader("Tests aborted! Press any key to close..", "");
		TestUtils::WaitForInput();

		return 0;
	}
#endif

	if (hasAes)
	{
		PrintHeader("AES-NI intrinsics support has been detected on this system.");
	}
	else
	{
		PrintHeader("AES-NI intrinsics support was not detected on this system.");
	}
	PrintHeader("", "");

	if (hasAvx2)
	{
#if !defined(__AVX2__)
		PrintHeader("Warning! AVX2 support was detected! Set the enhanced instruction set to arch:AVX2 for best performance.");
#else
		PrintHeader("AVX2 intrinsics support has been enabled.");
#endif
	}
	else if (hasAvx)
	{
#if defined(__AVX2__)
		PrintHeader("AVX2 is not supported on this system! AVX intrinsics support is available, set enable enhanced instruction set to arch:AVX");
#elif !defined(__AVX__)
		PrintHeader("AVX intrinsics support has been detected, set enhanced instruction set to arch:AVX for best performance.");
#else
		PrintHeader("AVX intrinsics support has been enabled.");
#endif
	}
	else
	{
		PrintHeader("The minimum SIMD intrinsics support (AVX) was not detected, intrinsics have been disabled!");
	}
	PrintHeader("", "");

	try
	{
#if defined (_DEBUG)
		PrintHeader("Warning! Compile as Release with correct platform (x86/x64) for accurate timings");
		PrintHeader("", "");
#endif

		if (TestConfirm("Press 'Y' then Enter to run Diagnostic Tests, any other key to cancel: "))
		{
			PrintHeader("TESTING SYMMETRIC BLOCK CIPHERS");

			if (hasAes)
			{
				PrintHeader("Testing the AES-NI implementation (AES-NI)");
				TestRun(new AesAvsTest(true));
			}

			PrintHeader("Testing the AES software implementation (AES)");
			TestRun(new AesAvsTest(false));

			if (hasAes)
			{
				PrintHeader("Testing the AES-NI implementation (AES-NI)");
				TestRun(new RijndaelTest(true));
			}

			PrintHeader("Testing the AES software implementation (RHX)");
			TestRun(new RijndaelTest(false));

			PrintHeader("Testing the Serpent software implementation (SHX)");
			TestRun(new SerpentTest());
			PrintHeader("TESTING SYMMETRIC CIPHER MODES");
			TestRun(new CipherModeTest());
			PrintHeader("TESTING SYMMETRIC CIPHER AEAD MODES");
			TestRun(new AeadTest());
			PrintHeader("TESTING PARALLEL CIPHER MODES");
			TestRun(new ParallelModeTest());
			PrintHeader("TESTING CIPHER PADDING MODES");
			TestRun(new PaddingTest());
			PrintHeader("TESTING SYMMETRIC STREAM CIPHERS");
			TestRun(new ChaChaTest());
			TestRun(new MCSTest());
			TestRun(new RCSTest());
			TestRun(new ThreefishTest());
			PrintHeader("TESTING CRYPTOGRAPHIC STREAM PROCESSORS");
			TestRun(new CipherStreamTest());
			TestRun(new DigestStreamTest());
			TestRun(new MacStreamTest());
			PrintHeader("TESTING CRYPTOGRAPHIC HASH GENERATORS");
			TestRun(new Blake2Test());
			TestRun(new KeccakTest());
			TestRun(new SHA2Test());
			TestRun(new SkeinTest());
			PrintHeader("TESTING MESSAGE AUTHENTICATION CODE GENERATORS");
			TestRun(new CMACTest());
			TestRun(new GMACTest());
			TestRun(new HMACTest());
			TestRun(new KMACTest());
			TestRun(new Poly1305Test());
			PrintHeader("TESTING RANDOM ENTROPY PROVIDERS");
			TestRun(new ACPTest());
			TestRun(new CJPTest());
			TestRun(new CSPTest());
			TestRun(new ECPTest());
			TestRun(new RDPTest());
			PrintHeader("TESTING PSEUDO RANDOM NUMBER GENERATORS");
			TestRun(new BCRTest());
			TestRun(new CSRTest());
			TestRun(new HCRTest());
			PrintHeader("TESTING KEY DERIVATION FUNCTIONS");
			TestRun(new HKDFTest());
			TestRun(new KDF2Test());
			TestRun(new PBKDF2Test());
			TestRun(new SCRYPTTest());
			TestRun(new SHAKETest());
			PrintHeader("TESTING DETERMINISTIC RANDOM BYTE GENERATORS");
			TestRun(new BCGTest());
			TestRun(new CSGTest());
			TestRun(new HCGTest());
			PrintHeader("TESTING KEY GENERATOR AND SECURE KEYS");
			TestRun(new AsymmetricKeyTest());
			TestRun(new SymmetricKeyGeneratorTest());
			TestRun(new SecureStreamTest());
			TestRun(new SymmetricKeyTest());
			PrintHeader("TESTING VECTORIZED MEMORY FUNCTIONS");
			TestRun(new MemUtilsTest());
			TestRun(new SimdWrapperTest());
			PrintHeader("TESTING UTILITY CLASS FUNCTIONS");
			TestRun(new UtilityTest());
			PrintHeader("TESTING ASYMMETRIC CIPHERS");
			TestRun(new McElieceTest());
			TestRun(new ModuleLWETest());
			TestRun(new NTRUTest());
			TestRun(new RingLWETest());
			PrintHeader("TESTING ASYMMETRIC SIGNATURE SCHEMES");
			TestRun(new DilithiumTest());
			TestRun(new SphincsTest());
			TestRun(new XMSSTest());
		}
		else
		{
			ConsoleUtils::WriteLine("Diagnostic tests were Cancelled..");
		}

		ConsoleUtils::WriteLine("");
		ConsoleUtils::WriteLine("");

#if defined(__AVX__)
		if (TestConfirm("Press 'Y' then Enter to run SIMD Memory operations Speed Tests, any other key to cancel: "))
		{
			TestRun(new SimdSpeedTest());
		}
		else
		{
			ConsoleUtils::WriteLine("SIMD Memory Speed tests were Cancelled..");
		}
		ConsoleUtils::WriteLine("");
#endif

		if (TestConfirm("Press 'Y' then Enter to run Symmetric Cipher Speed Tests, any other key to cancel: "))
		{
			TestRun(new CipherSpeedTest());
		}
		else
		{
			ConsoleUtils::WriteLine("Cipher Speed tests were Cancelled..");
		}
		ConsoleUtils::WriteLine("");

		if (TestConfirm("Press 'Y' then Enter to run Message Digest Speed Tests, any other key to cancel: "))
		{
			TestRun(new DigestSpeedTest());
		}
		else
		{
			ConsoleUtils::WriteLine("Digest Speed tests were Cancelled..");
		}
		ConsoleUtils::WriteLine("");

		if (TestConfirm("Press 'Y' then Enter to run Asymmetric Cipher Speed Tests, any other key to cancel: "))
		{
			TestRun(new AsymmetricSpeedTest());
		}
		else
		{
			ConsoleUtils::WriteLine("Asymmetric Cipher Speed tests were Cancelled..");
		}
		ConsoleUtils::WriteLine("");

		PrintHeader("Completed! Press any key to close..", "");
		TestUtils::WaitForInput();

		return 0;
	}
	catch (std::exception&)
	{
		PrintHeader("An error has occurred! Press any key to close..", "");
		TestUtils::WaitForInput();

		return 0;
	}
}
