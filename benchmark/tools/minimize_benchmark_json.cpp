#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;
using Path = std::filesystem::path;

/**
 Example json output:
{
  "context": {
    "date": "2000-11-11T14:58:02+08:00",
    "host_name": "YOUR-COMPUTER-NAME",
    "executable": "a\\b\\c\\d.exe",
    "num_cpus": 12,
    "mhz_per_cpu": 3294,
    "library_version": "v1.9.4-89-g04ccbd86",
    "library_build_type": "release",
    "json_schema_version": 1
  },
  "benchmarks": [
{
      "name": "float32_4 test_name(float32_4 lhs, float32_4 rhs)/iterations:8/repeats:8_median",
      "family_index": 0,
      "per_family_instance_index": 0,
      "run_name": "float32_4 test_name(float32_4 lhs, float32_4 rhs)/iterations:8/repeats:8",
      "run_type": "aggregate",
      "repetitions": 8,
      "threads": 1,
      "aggregate_name": "median",
      "aggregate_unit": "time",
      "iterations": 8,
      "real_time": 1.0000007932831068e+02,
      "cpu_time": 0.0000000000000000e+00,
      "time_unit": "ns",
    },
    {
      "name": "float32_4 test_name(float32_4 lhs, float32_4 rhs)/iterations:8/repeats:8_stddev",
      "family_index": 0,
      "per_family_instance_index": 0,
      "run_name": "float32_4 test_name(float32_4 lhs, float32_4 rhs)/iterations:8/repeats:8",
      "run_type": "aggregate",
      "repetitions": 8,
      "threads": 1,
      "aggregate_name": "stddev",
      "aggregate_unit": "time",
      "iterations": 8,
      "real_time": 2.1346937621170738e+02,
      "cpu_time": 0.0000000000000000e+00,
      "time_unit": "ns",
    },
    {
      "name": "float32_4 test_name(float32_4 lhs, float32_4 rhs)/iterations:8/repeats:8_cv",
      "family_index": 0,
      "per_family_instance_index": 0,
      "run_name": "float32_4 test_name(float32_4 lhs, float32_4 rhs)/iterations:8/repeats:8",
      "run_type": "aggregate",
      "repetitions": 8,
      "threads": 1,
      "aggregate_name": "cv",
      "aggregate_unit": "percentage",
      "iterations": 8,
      "real_time": 1.2420036337130129e+00,
      "cpu_time": 0.0000000000000000e+00,
      "time_unit": "ns"
    }
  ]
}
*/


#define TO_JSON(var) json[#var] = obj.var
#define FROM_JSON(var) json.at(#var).get_to(obj.var)
#define FROM_JSON_IF_NOT_THEN_RETURN(var) do { auto it = json.find(#var); if (it == json.end()) { return; } else { it->get_to(obj.var); } } while (0)
#define CHECK(exp, tips) do { if (!(exp)) { throw std::runtime_error(tips); } } while (0)


int32_t g_repetitions = -1;


std::vector<std::string> split(const std::string& str, char separator)
{
    std::vector<std::string> result;
    std::string item;
    for (const char& c : str)
    {
        if (c == separator)
        {
            result.push_back(std::move(item));
            item.clear();
            continue;
        }

        item += c;
    }

    return result;
}

std::string get_cpp_fn_sig(const std::string& fn_sig)
{
    return split(fn_sig, '/').at(0);
}

std::string get_comment(const std::string& fn_sig)
{
    return split(fn_sig, '/').at(1);
}

int get_op_count(const std::string& fn_sig)
{
    return std::stoi(split(fn_sig, '/').at(2));
}


struct File
{
    std::fstream m_file;

    explicit File(const Path& path, int open_mode = std::ios::in)
    {
        m_file.open(path, open_mode);
        CHECK(m_file.is_open(), "can not open the file");
    }

    ~File()
    {
        m_file.close();
    }

    [[nodiscard]] std::string get_string() const
    {
        std::stringstream ss;
        ss << m_file.rdbuf();
        std::string s = ss.str();
        CHECK(!s.empty(), "can not get string from file");
        return s;
    }

    void write_string(const std::string& str)
    {
        m_file << str;
    }
};


// =========================== google ===========================

struct GoogleBenchmarkContext
{
    std::string library_build_type;
    std::string date;
    int32_t num_cpus = -1;
    int32_t mhz_per_cpu = -1;
    std::string library_version;
};

static void from_json(const Json& json, GoogleBenchmarkContext& obj)
{
    FROM_JSON(library_build_type);
    FROM_JSON(date);
    FROM_JSON(num_cpus);
    FROM_JSON(mhz_per_cpu);
    FROM_JSON(library_version);
}

struct GoogleOneBenchmark
{
    double cpu_time = -1;
    int32_t repetitions = -1;
    std::string run_name;
    std::string aggregate_name;
    std::string time_unit; // must be "ns"

    // 以下字段是用于合并benchmark结果的，并非google benchmark的字段
    double internal_cv = -1; // cv
    double internal_cpu_time_median = -1; // median cpu time
};

static void from_json(const Json& json, GoogleOneBenchmark& obj)
{
    FROM_JSON(run_name);
    FROM_JSON_IF_NOT_THEN_RETURN(aggregate_name);
    FROM_JSON(repetitions);
    FROM_JSON(cpu_time);
    FROM_JSON(time_unit);

    if (g_repetitions < 0)
    {
        g_repetitions = obj.repetitions;
    }
    CHECK(g_repetitions == obj.repetitions, "invalid repetitions");

    CHECK(obj.time_unit == "ns", "time_unit must be \"ns\"");
    if (obj.aggregate_name != "cv" && obj.aggregate_name != "stddev") // cv可能等于0，不记录stddev
    {
        CHECK(obj.cpu_time > 0, "cpu_time must > 0");
    }
    CHECK(obj.repetitions > 0, "repetitions must > 0");
}

struct GoogleBenchmarkResult
{
    GoogleBenchmarkContext context;
    std::vector<GoogleOneBenchmark> benchmarks;

    void minimize()
    {
        // 删除无用的benchmarks数据
        // 1. 删除 aggregate_name != { "median" || "cv" } 的字段
        // 2. 合并 fn_sig 相同的，但是 aggregate_name 为 median 和 cv 的两个结果

        std::vector<GoogleOneBenchmark> benchmarks_median_and_cv;
        // delete
        for (auto& bm : benchmarks)
        {
            if (bm.aggregate_name == "median" || bm.aggregate_name == "cv")
            {
                benchmarks_median_and_cv.push_back(std::move(bm));
            }
        }

        // merge median and cv
        // 使用 run_name 作为 key，暂存输出结果
        std::map<std::string, GoogleOneBenchmark> temp;
        for (auto& bm : benchmarks_median_and_cv)
        {
            auto& result = temp[bm.run_name];

            if (result.internal_cpu_time_median < 0 && result.internal_cv < 0)
            {
                result = bm;
            }
            // median
            if (bm.aggregate_name == "median")
            {
                result.internal_cpu_time_median = bm.cpu_time;
            }
            // cv
            if (bm.aggregate_name == "cv")
            {
                result.internal_cv = bm.cpu_time;
            }
        }

        benchmarks.clear();
        for (auto& [fn_sig, bm] : temp)
        {
            benchmarks.push_back(std::move(bm));
        }
    }
};

static void from_json(const Json& json, GoogleBenchmarkResult& obj)
{
    FROM_JSON(context);
    FROM_JSON(benchmarks);
}



// =========================== tMath benchmark info ===========================
struct OneFunction
{
    std::string comment;
    int op_count = -1;
    double cpu_time_median = 0;
    double cv = 0;
};

static void to_json(Json& json, const OneFunction& obj)
{
    TO_JSON(comment);
    TO_JSON(op_count);
    TO_JSON(cpu_time_median);
    TO_JSON(cv);
}

struct FunctionGroup
{
    std::string fn_signature;
    std::vector<OneFunction> functions;
};

static void to_json(Json& json, const FunctionGroup& obj)
{
    TO_JSON(fn_signature);
    TO_JSON(functions);
}

struct BenchmarkResult
{
    int32_t repetitions = 0;
    std::vector<FunctionGroup> function_groups;
};

static void to_json(Json& json, const BenchmarkResult& obj)
{
    TO_JSON(repetitions);
    TO_JSON(function_groups);
}




BenchmarkResult make_tmath_bm_result(const GoogleBenchmarkResult& google_bm_result)
{
    // google benchmark 的每一个测试的名字都是类似这样的 "float32_4 test_name(float32_4 lhs, float32_4 rhs)/repeats:5"，所以我们直接以 '/' 作为分隔符分割字符串，就能判断这个benchmark是否要被分为一组
    BenchmarkResult result;

    result.repetitions = g_repetitions;

    // group all functions
    std::map<std::string, FunctionGroup> groups;

    for (const auto& bm : google_bm_result.benchmarks)
    {
        std::string fn_sig = get_cpp_fn_sig(bm.run_name);
        auto& group = groups[fn_sig];
        group.fn_signature = fn_sig;

        OneFunction fn;
        fn.comment = get_comment(bm.run_name);
        fn.op_count = get_op_count(bm.run_name);
        fn.cpu_time_median = bm.internal_cpu_time_median;
        fn.cv = bm.internal_cv;
        group.functions.push_back(std::move(fn));
    }

    for (auto& [fn_sig, g] : groups)
    {
        result.function_groups.push_back(std::move(g));
    }

    return result;
}



int main(int argc, char** argv)
{
    // argv[0]: ignore
    // argv[1]: json_file_path (input and output)

    // 步骤:
    // 0. 提取 context 字段中的 library_build_type 字段，入如果不等于release，则表示该测试是不准确的，直接退出程序
    // 1. 提取 context 字段中的 { date, num_cpus, mhz_per_cpu, library_version } 表示 { 日期，CPU核心数，推测的CPU频率，benchmark版本号 }
    // 2. 遍历 benchmarks 数组，逐个获取其中的对象的 aggregate_name 字段，如果字段的值等于 median ，就保留他，否则就丢弃
    // {
    //   "name": "float32_4 test_name(float32_4 lhs, float32_4 rhs)/repeats:5_median",
    //   "family_index": 0,
    //   "per_family_instance_index": 0,
    //   "run_name": "float32_4 test_name(float32_4 lhs, float32_4 rhs)/repeats:5",
    //   "run_type": "aggregate",
    //   "repetitions": 5,
    //   "threads": 1,
    //   "aggregate_name": "median",
    //   "aggregate_unit": "time",
    //   "iterations": 5,
    //   "real_time": 5.4217622767842908e+00,
    //   "cpu_time": 5.4059709821428568e+00,
    //   "time_unit": "ns"
    // }
    // 3. 获取到了 median 对象之后，提取其中的 {
    //      run_name(测试函数名称),
    //      aggregate_name(测试方法：多次测试取中位数),
    //      repetitions(测试的次数),
    //      cpu_time(CPU耗时),
    //      time_unit(时间单位)
    // }
    // 4. 获取aggregate_name="cv"的字段，表示 stddev / mean (变异系数，值越小越好，代表抖动越小) (单位: 百分比)
    // 5. 直接覆盖原来的json文件，生成新的json文件:
    // {
    //      "repetitions": 5
    //      "function_groups": [
    //          function_groups(用数组表示)，每个函数组有一个 fn_signature 作为 key，然后又包含一个数组，表示这个函数组一共有多少个函数
    //          {
    //              "fn_signature": "float32_4 dot4(float32_4 lhs, float32_4 rhs)", (完整的函数签名) (可视化的时候，把所有函数名相同的测试结果聚合在一起就行，然后按照lib的名称进行排序，但是tMath放在最前面)
    //              "functions": [
    //                  {
    //                      "comment": "备注"
    //                      "op_count": 一个函数的操作次数，次数越高，信噪比越高
    //                      "cpu_time_median": 1024.12 (ns)
    //                      "cv": 0.01, (变异系数) (不是百分比，而是比例)
    //                  }
    //              ]
    //          }
    //      ]
    // }

    try
    {
        CHECK(argc == 2, "argc must be equal to 2, argv[1] is the path of benchmark.json");

        std::string google_json_str;

        Path file_path = argv[1];
        file_path = std::filesystem::absolute(file_path);

        {
            File file(file_path, std::ios::in);
            google_json_str = file.get_string();
        }

        Json google_bm_json = Json::parse(google_json_str);

        GoogleBenchmarkResult google_bm = google_bm_json;
        google_bm.minimize();

        BenchmarkResult tmath_bm = make_tmath_bm_result(google_bm);

        // 写回去
        {
            File output(file_path, std::ios::out);
            Json tmath_bm_json = tmath_bm;
            std::string json_str = tmath_bm_json.dump(4);
            output.write_string(json_str);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "[Exception]: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}