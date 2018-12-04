
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
    = {fmi2_log_forwarding, calloc, free, nullptr, nullptr};
jm_callbacks jm_cb
    = {malloc, calloc, realloc, free, jm_default_logger, jm_log_level_debug,
       nullptr};
}

TEST_CASE("fmu2_me_t ctor", "[.][me]")
{
    auto ext_dir = fs::path(temp_dir) / "CoupledClutches";
    fs::create_directory(ext_dir);

    REQUIRE(fs::exists(fmu_path));

    SECTION("Default constructor should not throw")
    {
        CHECK_NOTHROW(fmilib::fmi2_me_t{});
    }

    SECTION("Construct ME-FMU object")
    {
        CHECK_NOTHROW(fmilib::fmi2_me_t{fmu_path, ext_dir.string(), ::fmu_cb,
                                        ::jm_cb, false});
    }

    SECTION("Construct ME-FMU as CS-FMU should throw")
    {
        CHECK_THROWS(fmilib::fmi2_cs_t{fmu_path, ext_dir.string(), ::fmu_cb,
                                       ::jm_cb, false});
    }

    SECTION("Construct ME-FMU with extracted = true")
    {
        CHECK_NOTHROW(fmilib::fmi2_me_t{fmu_path, ext_dir.string(), ::fmu_cb,
                                        ::jm_cb, true});
    }
}

TEST_CASE("fmu2_cs_t ctor", "[.][cs]")
{
    auto ext_dir = fs::path(temp_dir) / "CoupledClutches";
    fs::create_directory(ext_dir);
    REQUIRE(fs::exists(fmu_path));

    SECTION("Default ctor should not throw")
    {
        CHECK_NOTHROW(fmilib::fmi2_cs_t{});
    }

    SECTION("Construct ME-FMU object")
    {
        CHECK_NOTHROW(fmilib::fmi2_cs_t{fmu_path, ext_dir.string(), ::fmu_cb,
                                        ::jm_cb, false});
    }

    SECTION("Construct CS-FMU as ME-FMU should throw")
    {
        CHECK_THROWS(fmilib::fmi2_me_t{fmu_path, ext_dir.string(), ::fmu_cb,
                                       ::jm_cb, false});
    }

    SECTION("Construct CS-FMU object with extracted = true")
    {
        CHECK_NOTHROW(fmilib::fmi2_cs_t{fmu_path, ext_dir.string(), ::fmu_cb,
                                        ::jm_cb, true});
    }
}

TEST_CASE("fmi2_t info", "[.][CoupledClutches]")
{
    auto ext_dir = fs::path(temp_dir) / "CoupledClutches";
    fs::create_directory(ext_dir);

    fmilib::fmi2_me_t m{fmu_path, ext_dir.string(), ::fmu_cb, ::jm_cb, false};

    SECTION("FMU Info")
    {
        // fmiModelDescription
        CHECK(std::string{m.model_name()} == "CoupledClutches");
        CHECK(std::string{m.description()}
              == "Drive train with 3 dynamically coupled clutches");
        CHECK(std::string{m.author()} == "");
        CHECK(std::string{m.license()} == "");
        CHECK(std::string{m.standard_version()} == "2.0");
        //CHECK(std::string{m.generation_tool()}
        //      == "Dymola Version 2017 (64-bit), 2016-06-23");
        //CHECK(std::string{m.generation_date_and_time()}
        //      == "2016-07-05T12:33:41Z");
        CHECK(m.naming_convention() == fmi2_naming_enu_structured);

        // fmiModelDescription/ModelExchange
        CHECK(std::string{m.identifier_me()} == "CoupledClutches");
        CHECK(std::string{m.identifier_cs()} == "");
        CHECK(m.capability(fmi2_me_needsExecutionTool) == 0);
        CHECK(m.capability(fmi2_me_completedIntegratorStepNotNeeded) == 0);
        CHECK(m.capability(fmi2_me_canBeInstantiatedOnlyOncePerProcess) == 0);
        CHECK(m.capability(fmi2_me_canNotUseMemoryManagementFunctions) == 0);
        CHECK(m.capability(fmi2_me_canGetAndSetFMUstate) == 1);
        CHECK(m.capability(fmi2_me_canSerializeFMUstate) == 0);
        CHECK(m.capability(fmi2_me_providesDirectionalDerivatives) == 1);

        CHECK(m.number_of_continuous_states() == 8);
        CHECK(m.number_of_event_indicators() == 54);
        CHECK(m.default_experiment_start() == 0.0);
        CHECK(m.default_experiment_stop() == 1.5);
        CHECK(m.default_experiment_tolerance() == 0.0001);
        CHECK(m.default_experiment_step()
              == 0.01); // FMI2_DEFAULT_EXPERIMENT_STEPSIZE
        CHECK(m.fmu_kind() == fmi2_fmu_kind_me);

        auto td = m.type_definitions();
        CHECK(td.size() == 11);
        CHECK(td[0].has_value() == true);
        CHECK(td[11].has_value() == false);

        auto t = td[0].value();
        CHECK(std::string{t.type_name()}
              == "Modelica.Blocks.Interfaces.RealInput");
        CHECK(t.base_type() == fmi2_base_type_real);
        CHECK(std::string{t.type_description()} == "");
        CHECK(t.type_quantity() == nullptr); // nullptr is returned
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