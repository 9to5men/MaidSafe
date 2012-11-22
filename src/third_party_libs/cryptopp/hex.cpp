// hex.cpp - written and placed in the public domain by Wei Dai

#include "pch.h"

#ifndef CRYPTOPP_IMPORTS

#include "hex.h"
#include <atomic>
#include <thread>
#include <mutex>

NAMESPACE_BEGIN(CryptoPP)

static const byte s_vecUpper[] = "0123456789ABCDEF";
static const byte s_vecLower[] = "0123456789abcdef";

void HexEncoder::IsolatedInitialize(const NameValuePairs &parameters)
{
	bool uppercase = parameters.GetValueWithDefault(Name::Uppercase(), true);
	m_filter->Initialize(CombinedNameValuePairs(
		parameters,
		MakeParameters(Name::EncodingLookupArray(), uppercase ? &s_vecUpper[0] : &s_vecLower[0], false)(Name::Log2Base(), 4, true)));
}

void HexDecoder::IsolatedInitialize(const NameValuePairs &parameters)
{
	BaseN_Decoder::IsolatedInitialize(CombinedNameValuePairs(
		parameters,
		MakeParameters(Name::DecodingLookupArray(), GetDefaultDecodingLookupArray(), false)(Name::Log2Base(), 4, true)));
}

const int *HexDecoder::GetDefaultDecodingLookupArray()
{
  static std::once_flag s_initialized_flag;
  static std::atomic<bool> s_initialized(false);
	static int s_array[256];

  std::call_once(s_initialized_flag, [] {
      InitializeDecodingLookupArray(s_array, s_vecUpper, 16, true);
      s_initialized = true;
  });

  while (!s_initialized)
    std::this_thread::yield();

  return s_array;
}

NAMESPACE_END

#endif
