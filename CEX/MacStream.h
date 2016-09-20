// The MIT License (MIT)
// 
// Copyright (c) 2016 vtdev.com
// This file is part of the CEX Cryptographic library.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
// Written by John Underhill, January 21, 2015
// contact: develop@vtdev.com

#ifndef _CEXENGINE_MACSTREAM_H
#define _CEXENGINE_MACSTREAM_H

#include "Common.h"
#include "CryptoProcessingException.h"
#include "Event.h"
#include "IByteStream.h"
#include "IMac.h"
#include "MacDescription.h"
#include "KeyParams.h"

NAMESPACE_PROCESSING

using CEX::Exception::CryptoProcessingException;
using CEX::Event::Event;
using CEX::Common::KeyParams;
using CEX::IO::IByteStream;
using CEX::Mac::IMac;
using CEX::Common::MacDescription;

/// <summary>
/// MAC stream helper class.
/// <para>Wraps Message Authentication Code (MAC) stream functions in an easy to use interface.</para>
/// </summary> 
/// 
/// <example>
/// <description>Example of hashing a Stream:</description>
/// <code>
/// SHA256* eng = new SHA256();
/// HMAC* mac = new HMAC(eng);
/// hmac->Initialize(Key, Iv);
/// MacStream ds(mac);
/// IByteStream* ms = new MemoryStream(Input);
/// Code = ds.ComputeMac(ms);
/// delete eng;
/// delete mac;
/// delete ms;
/// </code>
/// </example>
/// 
/// <seealso cref="CEX::Enumeration::Macs"/>
/// 
/// <remarks>
/// <description>Implementation Notes:</description>
/// <list type="bullet">
/// <item><description>Uses any of the implemented Macs using the IMac interface.</description></item>
/// <item><description>Mac must be fully initialized before passed to the constructor.</description></item>
/// <item><description>Implementation has a Progress counter that returns total sum of bytes processed per either ComputeMac() calls.</description></item>
/// </list>
/// </remarks>
class MacStream
{
private:
	static constexpr size_t BUFFER_SIZE = 64 * 1024;

	size_t m_blockSize;
	bool m_destroyEngine;
	IByteStream* m_inStream;
	bool m_isDestroyed = false;
	IMac* m_macEngine;
	size_t m_progressInterval;

	MacStream() { }

public:

	/// <summary>
	/// The Progress Percent event
	/// </summary>
	Event<int> ProgressPercent;

	/// <summary>
	/// Initialize the class with an 
	/// </summary>
	/// 
	/// <param name="Description">A MacDescription structure containing details about the Mac generator</param>
	/// <param name="MacKey">A KeyParams containing the Mac key and Iv; note the Ikm parameter in KeyParams is not used</param>
	/// 
	/// <exception cref="CryptoProcessingException">Thrown if an uninitialized Mac is used</exception>
	explicit MacStream(MacDescription &Description, KeyParams &MacKey)
		:
		m_blockSize(0),
		m_destroyEngine(false),
		m_inStream(0),
		m_isDestroyed(false),
		m_progressInterval(0)
	{
		CreateMac(Description);
		if (m_macEngine == 0)
			throw CryptoProcessingException("MacStream:CTor", "The Mac could not be created!");

		m_macEngine->Initialize(MacKey.Key(), MacKey.IV());
		m_blockSize = m_macEngine->BlockSize();
	}

	/// <summary>
	/// Initialize the class with an initialized Mac instance
	/// </summary>
	/// 
	/// <param name="Mac">The initialized <see cref="CEX::Mac::IMac"/> instance</param>
	/// 
	/// <exception cref="CEX::Exception::CryptoProcessingException">Thrown if a null or uninitialized Mac is used</exception>
	explicit MacStream(IMac* Mac)
		:
		m_blockSize(Mac->BlockSize()),
		m_destroyEngine(false),
		m_inStream(0),
		m_isDestroyed(false),
		m_macEngine(Mac),
		m_progressInterval(0)
	{
		if (Mac == 0)
			throw CryptoProcessingException("MacStream:CTor", "The Mac can not be null!");
		if (!Mac->IsInitialized())
			throw CryptoProcessingException("MacStream:CTor", "The Mac is not initialized!");
	}

	/// <summary>
	/// Finalize objects
	/// </summary>
	~MacStream()
	{
		Destroy();
	}

	/// <summary>
	/// Process the entire length of the Input Stream
	/// </summary>
	/// 
	/// <returns>The Mac Code</returns>
	/// 
	/// <exception cref="CEX::Exception::CryptoProcessingException">Thrown if ComputeHash is called before Initialize(), or if Size + Offset is longer than Input stream</exception>
	std::vector<byte> ComputeMac(IByteStream* InStream);

	/// <summary>
	/// Process a length within the Input stream using an Offset
	/// </summary>
	/// 
	/// <returns>The Mac Code</returns>
	/// <param name="Input">The Input array to process</param>
	/// <param name="InOffset">The Input array starting offset</param>
	/// <param name="Length">The number of bytes to process</param>
	/// 
	/// <exception cref="CEX::Exception::CryptoProcessingException">Thrown if ComputeHash is called before Initialize(), or if Size + Offset is longer than Input stream</exception>
	std::vector<byte> ComputeMac(const std::vector<byte> &Input, size_t InOffset, size_t Length);

private:
	void CalculateInterval(size_t Length);
	void CalculateProgress(size_t Length, bool Completed = false);
	std::vector<byte> Compute(size_t Length);
	std::vector<byte> Compute(const std::vector<byte> &Input, size_t InOffset, size_t Length);
	void CreateMac(MacDescription &Description);
	void Destroy();
};

NAMESPACE_PROCESSINGEND
#endif
