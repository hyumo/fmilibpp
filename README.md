
# fmilib++

[![Build status](https://ci.appveyor.com/api/projects/status/dbx5a4ht9o0noqu9/branch/develop?svg=true)](https://ci.appveyor.com/project/hyumo/fmilibpp/branch/develop)

A header only modern c++ wrapper for FMILibrary. Still WIP.

C++17 compiler is required. 

# How to use 

Just copy and paste `fmilib.hpp` file into your project then include it.

```C++

#include "fmilib.hpp"

int main()
{
    std::string fmu_path = "C:/CoupledClutches.fmu";
    std::string temp_dir = "C:/temp";
    fmi2_callback_functions_t fmu_cb
        = {fmi2_log_forwarding, calloc, free, nullptr, nullptr};
    jm_callbacks jm_cb
        = {malloc, calloc, realloc, free, jm_default_logger, jm_log_level_debug,
           nullptr};

    fmilib::fmi2_me_t m(fmu_path, temp_dir, fmu_cb, jm_cb);
    std::cout << m.model_name() << '\n';
    std::cout << m.GUID() << '\n';
}
```