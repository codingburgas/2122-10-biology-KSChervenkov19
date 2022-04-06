#include "pch_dal.h"
#include "statistics_store.h"

json ss::dal::StatisticStore::toJson(const std::vector<ss::types::Cycle>& cycles)
{
	return json();
}

std::vector<ss::types::Cycle> ss::dal::StatisticStore::fromJson(const json& j)
{
	return std::vector<ss::types::Cycle>();
}

std::string ss::dal::StatisticStore::generateTimeStamp(const time_t& time)
{
	return std::string();
}

void ss::dal::StatisticStore::saveStatisticTo(const std::string& fileName,
	const std::vector<ss::types::Cycle>& cycles)
{
	return;
}

//Uses current time as a timestamp
void ss::dal::StatisticStore::saveStatisticTo(const std::vector<ss::types::Cycle>& cycles)
{
	time_t currentTime;
	time(&currentTime);

	saveStatisticTo(generateTimeStamp(currentTime), cycles);
}

std::vector<ss::types::Cycle> ss::dal::StatisticStore::getStatisticFrom(const std::string& fileName)
{
	return std::vector<ss::types::Cycle>();
}