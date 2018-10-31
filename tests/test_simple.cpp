
#define CATCH_CONFIG_RUNNER
#ifdef _WIN32
#define CATCH_CONFIG_WINDOWS_CRTDBG
#endif


#include <filesystem>

#include <catch.hpp>
#include <fmilib.hpp>

namespace fs = std::filesystem;

std::string temp_dir;
std::string fmu_dir;

namespace
{
fmi2_callback_functions_t fmu_cb
    = {fmi2_default_callback_logger, calloc, free, nullptr, nullptr};
jm_callbacks jm_cb
    = {malloc, calloc, realloc, free, jm_default_logger, jm_log_level_all,
       nullptr};
}

TEST_CASE("Simple", "[Integration]")
{
    temp_dir = temp_dir + "/CoupledClutches";
    fs::create_directory(temp_dir);

    fmilib::fmi2_t m(fmu_dir, temp_dir, fmu_cb, jm_cb);
    
}

int main(int argc, char *argv[])
{
    // see https://github.com/mapnik/mapnik/blob/master/test/unit/run.cpp
    using namespace Catch::clara;
    Catch::Session session;
    auto cli = session.cli() // Append options
               | Opt(fmu_dir, "fmu")["--fmu"]("FMU directory")
               | Opt(temp_dir, "temp")["--temp"]("Temp directory");
    session.cli(cli);
    int result = session.applyCommandLine(argc, argv);
    if (result != 0) // Indicates a command line error
        return result;

    return session.run();
}