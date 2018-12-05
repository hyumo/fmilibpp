
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
} // namespace

TEST_CASE("fmu2_me_t ctor", "[.]")
{
    auto ext_dir = fs::path(temp_dir) / id;
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

TEST_CASE("fmu2_me_t initialization", "[.]")
{
    auto ext_dir = fs::path(temp_dir) / id;
    fs::create_directory(ext_dir);

    REQUIRE(fs::exists(fmu_path));

    SECTION("Test init with regular argumetns")
    {
        fmilib::fmi2_me_t m{fmu_path, ext_dir.string(), ::fmu_cb, ::jm_cb};
        REQUIRE(
            jm_status_success
            == m.instantiate(id.c_str(), fmi2_model_exchange, "", fmi2_false));
        REQUIRE(fmi2_status_ok
                == m.setup_experiment(fmi2_true, 1e-05, 0.0, fmi2_true, 1.0));
        REQUIRE(fmi2_status_ok == m.enter_initialization_mode());
        REQUIRE(fmi2_status_ok == m.exit_initialization_mode());

        m.terminate();
        m.free_instance();
    }
}

TEST_CASE("fmu2_me_t test initialization", "[.]")
{
    auto ext_dir = fs::path(temp_dir) / id;
    fs::create_directory(ext_dir);

    REQUIRE(fs::exists(fmu_path));

    SECTION("Before entering continuous time mode")
    {
        fmilib::fmi2_me_t m{fmu_path, ext_dir.string(), ::fmu_cb, ::jm_cb};
        fmi2_event_info_t event_info{fmi2_true,  fmi2_false, fmi2_false,
                                     fmi2_false, fmi2_false, -0.0};
        REQUIRE(
            jm_status_success
            == m.instantiate(id.c_str(), fmi2_model_exchange, "", fmi2_false));
        REQUIRE(fmi2_status_ok
                == m.setup_experiment(fmi2_true, 1e-05, 0.0, fmi2_true, 1.0));
        REQUIRE(fmi2_status_ok == m.enter_initialization_mode());
        REQUIRE(fmi2_status_ok == m.exit_initialization_mode());

        event_info.newDiscreteStatesNeeded = fmi2_true;
        while (event_info.newDiscreteStatesNeeded
               && !event_info.terminateSimulation) {
            CHECK(fmi2_status_ok == m.new_discrete_states(&event_info));
        }
        CHECK(fmi2_status_ok == m.enter_continuous_time_mode());

        m.terminate();
        m.free_instance();
    }
}

TEST_CASE("State Machine of Calling Sequence: CoupledClutches",
          "[.][CoupledClutches]")
{
    auto ext_dir = fs::path(temp_dir) / id;
    fs::create_directory(ext_dir);

    REQUIRE(fs::exists(fmu_path));

    SECTION("Change parameter values before setup experiment should return ok")
    {
        fmilib::fmi2_me_t m{fmu_path, ext_dir.string(), ::fmu_cb, ::jm_cb};
        REQUIRE(
            jm_status_success
            == m.instantiate(id.c_str(), fmi2_model_exchange, "", fmi2_false));
        auto J1 = m.get_variable_by_name("J1.J");
        auto J2 = m.get_variable_by_name("J2.J");
        REQUIRE(J1.has_value());
        REQUIRE(J2.has_value());
        std::vector<fmi2_value_reference_t> vrs{J1.value().vr(),
                                                J2.value().vr()};
        std::vector<fmi2_real_t> values{10.0, 10.0};
        REQUIRE(fmi2_status_ok == m.set_real(vrs, values));
        REQUIRE(fmi2_status_ok
                == m.setup_experiment(fmi2_true, 1e-05, 0.0, fmi2_true, 1.0));
        REQUIRE(fmi2_status_ok == m.enter_initialization_mode());
        REQUIRE(fmi2_status_ok == m.exit_initialization_mode());

        m.terminate();
        m.free_instance();
    }

    SECTION("Enter continuous time mode should return ok")
    {
        fmilib::fmi2_me_t m{fmu_path, ext_dir.string(), ::fmu_cb, ::jm_cb};
        fmi2_event_info_t event_info{fmi2_true,  fmi2_false, fmi2_false,
                                     fmi2_false, fmi2_false, -0.0};

        REQUIRE(
            jm_status_success
            == m.instantiate(id.c_str(), fmi2_model_exchange, "", fmi2_false));
        REQUIRE(fmi2_status_ok
                == m.setup_experiment(fmi2_true, 1e-05, 0.0, fmi2_true, 1.0));
        REQUIRE(fmi2_status_ok == m.enter_initialization_mode());
        REQUIRE(fmi2_status_ok == m.exit_initialization_mode());

        event_info.newDiscreteStatesNeeded = fmi2_true;
        while (event_info.newDiscreteStatesNeeded
               && !event_info.terminateSimulation) {
            CHECK(fmi2_status_ok == m.new_discrete_states(&event_info));
        }
        CHECK(fmi2_true == event_info.nextEventTimeDefined);
        CHECK(0.4 == Approx(event_info.nextEventTime));
        CHECK(fmi2_status_ok == m.enter_continuous_time_mode());
        m.terminate();
        m.free_instance();
    }

    SECTION("Do euler integration before next event happens")
    {
        fmilib::fmi2_me_t m{fmu_path, ext_dir.string(), ::fmu_cb, ::jm_cb};
        fmi2_event_info_t event_info{fmi2_true,  fmi2_false, fmi2_false,
                                     fmi2_false, fmi2_false, -0.0};

        REQUIRE(
            jm_status_success
            == m.instantiate(id.c_str(), fmi2_model_exchange, "", fmi2_false));
        REQUIRE(fmi2_status_ok
                == m.setup_experiment(fmi2_true, 1e-05, 0.0, fmi2_true, 1.0));
        REQUIRE(fmi2_status_ok == m.enter_initialization_mode());
        REQUIRE(fmi2_status_ok == m.exit_initialization_mode());

        event_info.newDiscreteStatesNeeded = fmi2_true;
        while (event_info.newDiscreteStatesNeeded
               && !event_info.terminateSimulation) {
            CHECK(fmi2_status_ok == m.new_discrete_states(&event_info));
        }
        CHECK(fmi2_true == event_info.nextEventTimeDefined);
        CHECK(0.4 == Approx(event_info.nextEventTime));
        CHECK(fmi2_status_ok == m.enter_continuous_time_mode());

        const auto h = 0.001;
        std::vector<double> x(m.number_of_continuous_states());
        std::vector<double> x_dot(m.number_of_continuous_states());
        std::vector<double> x_next(m.number_of_continuous_states());
        for (int i = 0; i < 400; ++i) {
            double time = i * h;
            CAPTURE(time);
            CHECK(fmi2_status_ok == m.set_time(time));
            CHECK(fmi2_status_ok == m.get_continuous_states(x));
            CHECK(fmi2_status_ok == m.get_derivatives(x_dot));
            for (decltype(x.size()) j = 0; j < x.size(); ++j) {
                x_next[j] = x[j] + h * x_dot[j];
            }
            CHECK(fmi2_status_ok == m.set_continuous_states(x_next));
        }

        m.terminate();
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