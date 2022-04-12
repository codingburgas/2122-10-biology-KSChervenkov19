// clang-format off
#include "pch_dal.h"
#include "statistics_store.h"
// clang-format on

void ss::types::to_json(json &j, const ss::types::Trait &trait)
{
    j = json{{"Sense", trait.sense}, {"Speed", trait.speed}};
}

void ss::types::from_json(const json &j, ss::types::Trait &trait)
{
    j.at("Sense").get_to(trait.sense);
    j.at("Speed").get_to(trait.speed);
}

void ss::types::to_json(json &j, const ss::types::Cycle &cycle)
{
    j = json{{"Alive", cycle.lastedEntities}, {"Traits", json(cycle.traitsInfo)}};
}

void ss::types::from_json(const json &j, ss::types::Cycle &cycle)
{
    j.at("Alive").get_to(cycle.lastedEntities);
    j.at("Traits").get_to(cycle.traitsInfo);
}

std::vector<std::string> ss::dal::StatisticsStore::getStatisticsNames()
{
    std::vector<std::string> fileNames;
    const std::string path = R"(/2122-10-biology-KSChervenkov19\statistics)";

    for (std::string file; const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.path().extension().string() == ".json")
        {
            file = entry.path().filename().string();
            fileNames.push_back(file.substr(0, file.find(".json")));
        }
    }

    return fileNames;
}

void ss::dal::StatisticsStore::saveStatisticTo(const std::string &fileName, const std::vector<ss::types::Cycle> &cycles)
{
    std::ofstream out(fileName + ".json");

    if (!out.is_open())
        throw std::runtime_error("File: " + fileName + " cannot be opened!");

    json j(cycles);
    out << j;

    out.close();
}

// Uses current time as a timestamp
void ss::dal::StatisticsStore::saveStatisticTo(const std::vector<ss::types::Cycle> &cycles)
{
    time_t currentTimeStamp = std::time(0);

    saveStatisticTo(std::to_string(currentTimeStamp), cycles);
}

std::vector<ss::types::Cycle> ss::dal::StatisticsStore::getStatisticFrom(const std::string &fileName)
{
    std::ifstream in("/2122-10-biology-KSChervenkov19\\statistics\\" + fileName + ".json");

    if (!in.is_open())
        throw std::runtime_error("File: " + fileName + " cannot be opened!");

    json j;
    in >> j;

    in.close();

    return j.get<std::vector<ss::types::Cycle>>();
}