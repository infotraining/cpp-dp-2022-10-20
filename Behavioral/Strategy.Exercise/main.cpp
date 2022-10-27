#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

struct StatResult
{
    std::string description;
    double value;

    StatResult(const std::string& desc, double val)
        : description(desc)
        , value(val)
    {
    }
};

using Data = std::vector<double>;
using Results = std::vector<StatResult>;
using Strategy = std::function<void(const Data& data, Results& results)>;


class DataAnalyzer
{
    Strategy strategy_;
    Data data_;
    Results results_;

public:
    DataAnalyzer(Strategy strategy)
        : strategy_{strategy}
    {
    }

    void load_data(const std::string& file_name)
    {
        data_.clear();
        results_.clear();

        std::ifstream fin(file_name.c_str());
        if (!fin)
            throw std::runtime_error("File not opened");

        double d;
        while (fin >> d)
        {
            data_.push_back(d);
        }

        std::cout << "File " << file_name << " has been loaded...\n";
    }

    void set_statistics(Strategy strategy)
    {
        strategy_ = strategy;
    }

    void calculate()
    {
        strategy_(data_, results_);
    }

    const Results& results() const
    {
        return results_;
    }
};

void show_results(const Results& results)
{
    for (const auto& rslt : results)
        std::cout << rslt.description << " = " << rslt.value << std::endl;
}

struct Avg
{
    void operator()(const Data& data, Results& results) const
    {
            double sum = std::accumulate(data.begin(), data.end(), 0.0);
            double avg = sum / data.size();

            StatResult result("Avg", avg);
            results.push_back(result);
    }
};

void min_max(const Data& data, Results& results)
{
    double min = *(std::min_element(data.begin(), data.end()));
    double max = *(std::max_element(data.begin(), data.end()));

    results.push_back(StatResult("Min", min));
    results.push_back(StatResult("Max", max));
}

auto sum = [](const Data& data, Results& results)
{
    double sum = std::accumulate(data.begin(), data.end(), 0.0);

    results.push_back(StatResult("Sum", sum));
};

// Composite of Statistics
struct StatGroup 
{
    std::vector<Strategy> stats_;

    void add_statistics(Strategy stat)
    {
        stats_.push_back(stat);
    }

    void operator()(const Data& data, Results& results) const
    {
        for(const auto& stat : stats_)
        {
            stat(data, results);
        }
    }
};

int main()
{
    Avg avg;
    StatGroup std_stats;
    std_stats.add_statistics(avg);
    std_stats.add_statistics(sum);
    StatGroup advanced_stats;
    advanced_stats.add_statistics(std_stats);
    advanced_stats.add_statistics(min_max);

    DataAnalyzer da{advanced_stats};
    da.load_data("data.dat");
    da.calculate();

    show_results(da.results());

    std::cout << "\n\n";

    da.load_data("new_data.dat");
    da.calculate();

    show_results(da.results());
}
