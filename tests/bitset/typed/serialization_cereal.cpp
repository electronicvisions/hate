#include <gtest/gtest.h>
#include "hate/bitset.h"
#include "hate/cerealization_bitset.h"
#include "helper.h"
#include "serialization_cases.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

#define SERIALIZATION_TEST(OArchive, IArchive)                                                     \
	TYPED_TEST(CommonSerializationTests, Serialization##IArchive##OArchive)                        \
	{                                                                                              \
		TypeParam obj1(random_array<typename TypeParam::word_type, TypeParam::num_words>());       \
		TypeParam obj2;                                                                            \
                                                                                                   \
		using namespace cereal;                                                                    \
		std::ostringstream ostream;                                                                \
		{                                                                                          \
			OArchive oa(ostream);                                                                  \
			oa(obj1);                                                                              \
		}                                                                                          \
                                                                                                   \
		std::istringstream istream(ostream.str());                                                 \
		{                                                                                          \
			IArchive ia(istream);                                                                  \
			ia(obj2);                                                                              \
		}                                                                                          \
                                                                                                   \
		EXPECT_EQ(obj1, obj2);                                                                     \
	}

SERIALIZATION_TEST(BinaryOutputArchive, BinaryInputArchive)
SERIALIZATION_TEST(PortableBinaryOutputArchive, PortableBinaryInputArchive)
SERIALIZATION_TEST(JSONOutputArchive, JSONInputArchive)
SERIALIZATION_TEST(XMLOutputArchive, XMLInputArchive)

#undef SERIALIZATION_TEST
