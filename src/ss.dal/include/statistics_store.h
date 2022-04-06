#pragma once
// clang-format off
#include "pch_dal.h"
#include "statistics.h"
// clang-format on

namespace ss::dal
{
	class StatisticStore
	{
	private:
		static json toJson(const std::vector<ss::types::Cycle>& cycles);
		
		static std::vector<ss::types::Cycle> fromJson(const json& j);

		static std::string generateTimeStamp(const time_t& time);
	public:
		static void saveStatisticTo(const std::string& fileName,
			const std::vector<ss::types::Cycle>& cycles);

		static void saveStatisticTo(const std::vector<ss::types::Cycle>& cycles);

		static std::vector<ss::types::Cycle> getStatisticFrom(const std::string& fileName);
	};
}