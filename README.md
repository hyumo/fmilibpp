
# fmilib++

A header only modern c++ wrapper for FMILibrary. Still WIP.

# How to use 

```C++

#include <string>

#include "fmilib.hpp"

int main()
{
    // FMU location
    std::string fmu_path = "C:/CoupledClutches.fmu";
    // Temp folder
    std::string temp_dir = "C:/temp";
    //
    fmi2_callback_functions_t fmu_cb
        = {fmi2_log_forwarding, calloc, free, nullptr, nullptr};
    jm_callbacks jm_cb
        = {malloc, calloc, realloc, free, jm_default_logger, jm_log_level_debug,
           nullptr};

    auto m = fmilib::fmi2_me_t(fmu_path, temp_dir, fmu_cb, jm_cb);
    std::cout << m.model_name() << '\n';
    std::cout << m.GUID() << '\n';
}
```