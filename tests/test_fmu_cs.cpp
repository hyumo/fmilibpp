
#define CATCH_CONFIG_RUNNER
#ifdef _WIN32
#define CATCH_CONFIG_WINDOWS_CRTDBG
#endif

#include <filesystem>
#include <string>

#include <catch.hpp>
#include <fmilib.hpp>

namespace fs = std::filesystem;

std::string fmu_path;
std::string temp_dir;
std::string id;

namespace
{
    fmi2_callback_functions_t fmu_cb
        = { fmi2_log_forwarding, calloc, free, nullptr, nullptr };
    jm_callbacks jm_cb
        = { malloc, calloc, realloc, free, jm_default_logger, jm_log_level_debug,
           nullptr };
}

TEST_CASE("fmu2_cs_t ctor")
{
    auto ext_dir = fs::path(temp_dir) / id;
    fs::create_directory(ext_dir);

    REQUIRE(fs::exists(fmu_path));

    SECTION("Default constructor should not throw")
    {
        CHECK_NOTHROW(fmilib::fmi2_cs_t{});
    }

    SECTION("Construct ME-FMU object")
    {
        CHECK_NOTHROW(fmilib::fmi2_cs_t{ fmu_path, ext_dir.string(), ::fmu_cb,
                                        ::jm_cb, false });
    }

    SECTION("Construct ME-FMU as CS-FMU should throw")
    {
        CHECK_THROWS(fmilib::fmi2_cs_t{ fmu_path, ext_dir.string(), ::fmu_cb,
                                       ::jm_cb, false });
    }

    SECTION("Construct ME-FMU with extracted = true")
    {
        CHECK_NOTHROW(fmilib::fmi2_cs_t{ fmu_path, ext_dir.string(), ::fmu_cb,
                                        ::jm_cb, true });
    }
}

TEST_CASE("fmu2_me_t initialization")
{
    auto ext_dir = fs::path(temp_dir) / id;
    fs::create_directory(ext_dir);

    REQUIRE(fs::exists(fmu_path));

    SECTION("Regular argumetns")
    {
        fmilib::fmi2_cs_t m{ fmu_path, ext_dir.string(), ::fmu_cb, ::jm_cb };
        REQUIRE(
            jm_status_success
            == m.instantiate(id.c_str(), fmi2_model_exchange, "", fmi2_false));
        REQUIRE(fmi2_status_ok
            == m.setup_experiment(fmi2_true, 1e-05, 0.0, fmi2_true, 1.0));
        REQUIRE(fmi2_status_ok == m.enter_initialization_mode());
        REQUIRE(fmi2_status_ok == m.exit_initialization_mode());
        m.free_instance();
    }
}

int main(int argc, char *argv[])
{
    // see https://github.com/mapnik/mapnik/blob/master/test/unit/run.cpp
    using namespace Catch::clara;
    Catch::Session session;
    auto cli = session.cli() // Append options
        | Opt(id, "id")["--id"]("FMU id")
        | Opt(fmu_path, "fmu")["--fmu"]("FMU directory")
        | Opt(temp_dir, "temp")["--temp"]("Temp directory");

    session.cli(cli);
    int result = session.applyCommandLine(argc, argv);
    if (result != 0) // Indicates a command line error
        return result;

    return session.run();
}