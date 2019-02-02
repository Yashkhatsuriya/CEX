// The GPL version 3 License (GPLv3)
// Copyright (c) 2019 vtdev.com
// This file is part of the CEX Cryptographic library.
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// 
// Principal Algorithms:
// This cipher is based on the Threefish512 stream cipher designed by Daniel J. Bernstein:
// Threefish512: <a href="http://www.ecrypt.eu.org/stream/salsa20pf.html"/>.
// 
// Implementation Details:
// Threefish512: An implementation if the Threefish512 implemented as a stream cipher
// Written by John Underhill, September 11, 2018
// Updated December 20, 2018
// Updated December 26, 2018
// Contact: develop@vtdev.com

#ifndef CEX_THREEFISH512_H
#define CEX_THREEFISH512_H

#include "IStreamCipher.h"
#include "ShakeModes.h"
#include "SecureVector.h"

NAMESPACE_STREAM

using Enumeration::ShakeModes;

/// <summary>
/// A vectorized and optionally parallelized Threefish512 96-round stream cipher [TSX512] implementation.
/// <para>Uses an optional authentication mode; HMAC(SHA2) or KMAC set through the constructor to authenticate the stream.</para>
/// </summary>
/// 
/// <example>
/// <description>Encrypt an array:</description>
/// <code>
/// SymmetricKey kp(Key, Nonce);
/// Threefish512 cipher(StreamAuthenticators::HMACSHA256);
/// // mac code is appended to the cipher-text stream in authentication mode
/// cipher.Initialize(true, kp);
/// cipher.Transform(Input, InOffset, Output, OutOffset, Length);
/// </code>
///
/// <description>Encrypt and authenticate an array:</description>
/// <code>
/// SymmetricKey kp(Key, Nonce);
/// Threefish512 cipher(StreamAuthenticators::HMACSHA256);
/// // initialize for encryption
/// cipher.Initialize(true, kp);
/// cipher.Transform(Input, InOffset, Output, OutOffset, Length);
/// </code>
///
/// <description>Decrypt and authenticate an array:</description>
/// <code>
/// SymmetricKey kp(Key, Nonce);
/// Threefish512 cipher(StreamAuthenticators::HMACSHA256);
/// // initialize for decryption
/// cipher.Initialize(false, kp);
/// // decrypt the ciphertext
/// try
/// {
///		cipher.Transform(Input, InOffset, Output, OutOffset, Length);
/// }
/// catch (CryptoAuthenticationFailure)
/// {
///		// do something...
/// }
/// </code>
/// </example>
/// 
/// <remarks>
/// <description><B>Overview:</B></description>
/// <para>This cipher is a vectorized and optionally parallelized implementation of the Threefish256 cipher, used by the Skein family of hash functions. \n
/// It is capable of processing data using AVX2 or AVX512 SIMD instructions, and can also optionally employ multi-threaded parallelism. \n
/// An optional authentication component has also been added, and the cipher output can be authenticated using either an SHA2 hash based HMAC, or the Keccak based KMAC message authentication code generators. \n
/// The number of rounds in the permutation function has been increased in this implementation from the standard 72 to 96 rounds to increase the potential security of the cipher.</para>
///
/// <description><B>Multi-Threading:</B></description>
/// <para>The transformation function used by Threefish is not limited by a dependency chain; this mode can be both SIMD pipelined and multi-threaded. \n
/// This is achieved by pre-calculating the counters positional offset over multiple 'chunks' of key-stream, which are then generated independently across threads. \n 
/// The key stream generated by encrypting the counter array(s), is used as a source of random, and XOR'd with the message input to produce the cipher text.</para>
///
/// <description><B>Authentication:</B></description>
/// <para>When operating in authenticated mode; in an encryption cycle the MAC code is automatically appended to the output cipher-text. \n
/// During a decryption cycle, the code is checked against the new code generated by the MAC generator, and a failure will throw a CryptoAuthenticationFailure exception. \n
/// It is recommended that the decryption function is wrapped in a try/catch block, so that CryptoAuthenticationFailure can be handled by the calling function. \n
/// The supported MAC generator options for Threefish512 are HMAC(SHA2-256/512), and the Keccak based KMAC-256, and KMAC-512.</para>
///
/// <description>Implementation Notes:</description>
/// <list type="bullet">
/// <item><description></description></item>
/// <item><description>The Key size is 64 bytes (512 bits).</description></item>
/// <item><description>The mandatory nonce size is 16 bytes (128 bits).</description></item>
/// <item><description>The Info string is optional, but can be used to create a tweakable cipher; it must be 16 bytes in length.</description></item>
/// <item><description>Block size is 64 bytes wide (512 bits).</description></item>
/// <item><description>Permutation rounds are fixed at 96.</description></item>
/// <item><description>This cipher is capable of authentication by setting the constructors StreamAuthenticators enumeration to one of the HMAC or KMAC options.</description></item>
/// <item><description>In authentication mode, during encryption the MAC code is automatically appended to the output cipher-text of each transform call, during decryption, this MAC code is checked and authentication failure will generate a CryptoAuthenticationFailure exception.</description></item>
/// <item><description>In authenticated mode, the cipher-key generated by cSHAKE will be constant even with differing MAC generators; only two cipher-text outputs are possible, authenticated or non-authenticated.</description></item>
/// <item><description>If authentication is enabled, the cipher-key and MAC seed are generated using cSHAKE, this will change the cipher-text output.</description></item>
/// <item><description>Authentication using HMAC or KMAC, can be invoked by setting the StreamAuthenticators parameter in the constructor.</description></item>
/// <item><description>Encryption can both be pipelined (AVX2 or AVX512), and multi-threaded with any even number of threads.</description></item>
/// <item><description>The class functions are virtual, and can be accessed from an IStreamCipher instance.</description></item>
/// <item><description>The transformation functions can not be called until the Initialize(ISymmetricKey) function has been called.</description></item>
/// <item><description>If the system supports Parallel processing, and ParallelProfile().IsParallel() is set to true; passing an input block of ParallelBlockSize() to the transform will be auto parallelized.</description></item>
/// <item><description>The ParallelProfile().ParallelThreadsMax() property is used as the thread count in the parallel loop; this must be an even number no greater than the number of processer cores on the system.</description></item>
/// <item><description>ParallelProfile().ParallelBlockSize() is calculated automatically based on processor(s) cache size but can be user defined, but must be evenly divisible by ParallelProfile().ParallelMinimumSize().</description></item>
/// <item><description>The ParallelBlockSize() can be changed through the ParallelProfile() property</description></item>
/// <item><description>Parallel block calculation ex. <c>ParallelBlockSize = N - (N % .ParallelMinimumSize);</c></description></item>
/// </list>
/// 
/// <description>Guiding Publications:</description>
/// <list type="number">
/// <item><description>The Skein Hash Function Family <a href="https://www.schneier.com/academic/paperfiles/skein1.3.pdf">Skein V1.1</a>.</description></item>
/// <item><description>NIST Round 3 <a href="https://www.schneier.com/academic/paperfiles/skein-1.3-modifications.pdf">Tweak Description</a>.</description></item>
/// <item><description>Skein <a href="https://www.schneier.com/academic/paperfiles/skein-proofs.pdf">Provable Security</a> Support for the Skein Hash Family.</description></item>
/// <item><description>NIST <a href="http://nvlpubs.nist.gov/nistpubs/ir/2012/NIST.IR.7896.pdf">SHA3 Third-Round Report</a> of the SHA-3 Cryptographic Hash Algorithm Competition>.</description></item>
/// <item><description>FIPS 202: <a href="http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf">Permutation Based Hash</a> and Extendable Output Functions</description></item>
/// <item><description>NIST <a href="http://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-185.pdf">SP800-185</a> SHA-3 Derived Functions.</description></item>
/// <item><description>Team Keccak <a href="https://keccak.team/index.html">Homepage</a>.</description></item>
/// </list>
/// 
/// </remarks>
class Threefish512 final : public IStreamCipher
{
private:

	static const size_t BLOCK_SIZE = 64;
	static const std::string CLASS_NAME;
	static const size_t INFO_SIZE = 16;
	static const size_t KEY_SIZE = 64;
	static const size_t NONCE_SIZE = 2;
	static const std::vector<byte> OMEGA_INFO;
	static const size_t ROUND_COUNT = 96;
	static const size_t STATE_PRECACHED = 2048;
	static const size_t STATE_SIZE = 64;

	struct Threefish512State;

	StreamAuthenticators m_authenticatorType;
	std::unique_ptr<Threefish512State> m_cipherState;
	std::vector<byte> m_cShakeCustom;
	bool m_isAuthenticated;
	bool m_isDestroyed;
	bool m_isEncryption;
	bool m_isInitialized;
	std::vector<SymmetricKeySize> m_legalKeySizes;
	std::unique_ptr<IMac> m_macAuthenticator;
	ulong m_macCounter;
	SecureVector<byte> m_macKey;
	std::vector<byte> m_macTag;
	ParallelOptions m_parallelProfile;

public:

	//~~~Constructor~~~//

	/// <summary>
	/// Copy constructor: copy is restricted, this function has been deleted
	/// </summary>
	Threefish512(const Threefish512&) = delete;

	/// <summary>
	/// Copy operator: copy is restricted, this function has been deleted
	/// </summary>
	Threefish512& operator=(const Threefish512&) = delete;

	/// <summary>
	/// Initialize the Threefish-512 cipher.
	/// <para>Setting the optional AuthenticatorType parameter to any value other than None, enables authentication for this cipher.
	/// Use the Finalize function to derive the Mac code once processing of the message stream has completed.
	/// The default authenticator parameter in Threefish-512 is KMAC512; valid options are, None, HMACSHA256, HMACSHA512, KMAC256, and KMAC512.</para>
	/// </summary>
	/// 
	/// <param name="AuthenticatorType">The authentication engine, the default is KMAC512</param>
	///
	/// <exception cref="CryptoSymmetricCipherException">Thrown if an invalid authentication method is chosen</exception>
	explicit Threefish512(StreamAuthenticators AuthenticatorType = StreamAuthenticators::KMAC512);

	/// <summary>
	/// Destructor: finalize this class
	/// </summary>
	~Threefish512() override;

	//~~~Accessors~~~//

	/// <summary>
	/// Read Only: Internal block size of internal cipher in bytes.
	/// <para>Block size is 64 bytes wide.</para>
	/// </summary>
	const size_t BlockSize() override;

	/// <summary>
	/// Read Only: The maximum size of the distribution code in bytes.
	/// <para>The distribution code is set with the ISymmetricKey Info parameter; and can be used as a secondary domain key.</para>
	/// </summary>
	const size_t DistributionCodeMax() override;

	/// <summary>
	/// Read Only: The stream ciphers type name
	/// </summary>
	const StreamCiphers Enumeral() override;

	/// <summary>
	/// Read Only: Cipher has authentication enabled
	/// </summary>
	const bool IsAuthenticator() override;

	/// <summary>
	/// Read Only: Cipher is ready to transform data
	/// </summary>
	const bool IsInitialized() override;

	/// <summary>
	/// Read Only: Processor parallelization availability.
	/// <para>Indicates whether parallel processing is available with this cipher.
	/// If parallel capable, input/output data arrays passed to the transform must be ParallelBlockSize in bytes to trigger parallelization.</para>
	/// </summary>
	const bool IsParallel() override;

	/// <summary>
	/// Read Only: Array of SymmetricKeySize containers, containing legal cipher input key sizes
	/// </summary>
	const std::vector<SymmetricKeySize> &LegalKeySizes() override;

	/// <summary>
	/// Read Only: The stream ciphers implementation name
	/// </summary>
	const std::string Name() override;

	/// <summary>
	/// Read Only: Parallel block size; the byte-size of the input/output data arrays passed to a transform that trigger parallel processing.
	/// <para>This value can be changed through the ParallelProfile class.</para>
	/// </summary>
	const size_t ParallelBlockSize() override;

	/// <summary>
	/// Read/Write: Parallel and SIMD capability flags and recommended sizes.
	/// <para>The maximum number of threads allocated when using multi-threaded processing can be set with the ParallelMaxDegree() property.
	/// The ParallelBlockSize() property is auto-calculated, but can be changed; the value must be evenly divisible by ParallelMinimumSize().
	/// Changes to these values must be made before the Initialize(bool, ISymmetricKey) function is called.</para>
	/// </summary>
	ParallelOptions &ParallelProfile() override;

	/// <summary>
	/// Read Only: The current MAC tag value
	/// </summary>
	const std::vector<byte> &Tag() override;

	/// <summary>
	/// Read Only: The legal tag length in bytes
	/// </summary>
	const size_t TagSize() override;

	//~~~Public Functions~~~//

	/// <summary>
	/// The stream ciphers authentication MAC generator type.
	/// <para>Change the MAC generator (HMAC, KMAK -N), type used to authenticate the stream.</para>
	/// </summary>
	/// 
	/// <param name="AuthenticatorType">The MAC generator type used to calculate the authentication code</param>
	void Authenticator(StreamAuthenticators AuthenticatorType) override;

	/// <summary>
	/// Initialize the cipher with an ISymmetricKey key container.
	/// <para>If authentication is enabled, setting the Encryption parameter to false will decrypt and authenticate a ciphertext stream.
	/// Authentication on a decrypted stream is performed automatically; failure will throw a CryptoAuthenticationFailure exception.
	/// If encryption and authentication are set to true, the MAC code is appended to the ciphertext array.</para>
	/// </summary>
	/// 
	/// <param name="Encryption">Using Encryption or Decryption mode</param>
	/// <param name="KeyParams">Cipher key structure, containing cipher key, nonce, and optional info array</param>
	///
	/// <exception cref="CryptoSymmetricCipherException">Thrown if a null or invalid key is used</exception>
	void Initialize(bool Encryption, ISymmetricKey &KeyParams) override;

	/// <summary>
	/// Set the maximum number of threads allocated when using multi-threaded processing.
	/// <para>When set to zero, thread count is set automatically. If set to 1, sets IsParallel() to false and runs in sequential mode. 
	/// Thread count must be an even number, and not exceed the number of processor [virtual] cores.</para>
	/// </summary>
	///
	/// <param name="Degree">The desired number of threads</param>
	/// 
	/// <exception cref="CryptoCipherModeException">Thrown if the degree parameter is invalid</exception>
	void ParallelMaxDegree(size_t Degree) override;

	/// <summary>
	/// Add additional data to the authentication generator.  
	/// <para>Must be called after Initialize(bool, ISymmetricKey), and can be called before or after a stream segment has been processed.</para>
	/// </summary>
	/// 
	/// <param name="Input">The input array of bytes to process</param>
	/// <param name="Offset">Starting offset within the input array</param>
	/// <param name="Length">The number of bytes to process</param>
	///
	/// <exception cref="CryptoSymmetricCipherException">Thrown if the cipher is not initialized</exception>
	void SetAssociatedData(const std::vector<byte> &Input, const size_t Offset, const size_t Length) override;

	/// <summary>
	/// Encrypt/Decrypt an array of bytes with offset and length parameters.
	/// <para>Initialize(bool, ISymmetricKey) must be called before this method can be used.
	///	In authenticated encryption mode, the MAC code is automatically appended to the output stream at the end of the cipher-text, the output array must be long enough to accommodate this code.
	/// In decryption mode, this code is checked before the stream is decrypted, if the authentication fails a CryptoAuthenticationFailure exception is thrown.</para>
	/// </summary>
	/// 
	/// <param name="Input">The input array of bytes to transform</param>
	/// <param name="InOffset">Starting offset within the input array</param>
	/// <param name="Output">The output array of transformed bytes</param>
	/// <param name="OutOffset">Starting offset within the output array</param>
	/// <param name="Length">Number of bytes to process</param>
	///
	/// <exception cref="CryptoAuthenticationFailure">Thrown during decryption if the the ciphertext fails authentication</exception>
	void Transform(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset, const size_t Length) override;

private:

	void Finalize(std::vector<byte> &Output, const size_t OutOffset, const size_t Length);
	void Generate(std::array<ulong, 2> &Counter, std::vector<byte> &Output, const size_t OutOffset, const size_t Length);
	void Process(const std::vector<byte> &Input, const size_t InOffset, std::vector<byte> &Output, const size_t OutOffset, const size_t Length);
	void Reset();
};

NAMESPACE_STREAMEND
#endif

