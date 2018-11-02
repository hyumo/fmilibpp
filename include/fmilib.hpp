/*
 *   Copyright (c) 2018, Hang Yu
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *   * Neither the name of  nor the names of its contributors may be used to
 *   endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include <iostream>

#include <fmilib.h>

namespace fmilib
{
class display_unit_t
{

private:
    fmi2_import_display_unit_t *_du;

public:
    display_unit_t() = delete;
    explicit display_unit_t(fmi2_import_display_unit_t *du) : _du{du} {}

    // unit_t base_unit() const noexcept
    //{
    //    return unit_t{fmi2_import_get_base_unit(_du)};
    //}

    /**
     * @brief Display unit name
     */
    fmi2_string_t name() const noexcept
    {
        return fmi2_import_get_display_unit_name(_du);
    }

    fmi2_real_t factor() const noexcept
    {
        return fmi2_import_get_display_unit_factor(_du);
    }

    fmi2_real_t offset() const noexcept
    {
        return fmi2_import_get_display_unit_offset(_du);
    }

    /**
     * @brief Convert a value measured in "units" to a value measured with
     * "display units"
     *
     * @param value The value to be converted.
     * @param isRelativeQuantity specifies if "offset" should be incorporated
     * into conversion
     */
    fmi2_real_t convert_to_display_unit(fmi2_real_t value,
                                        int isRelativeQuantity) const noexcept
    {
        return fmi2_import_convert_to_display_unit(value, _du,
                                                   isRelativeQuantity);
    }

    /**
     * @brief Convert a value measured in "display units" to a value measured
     * with "units"
     *
     * @param value The value to be converted.
     * @param isRelativeQuantity specifies if "offset" should be incorporated
     * into conversion
     */
    fmi2_real_t convert_from_display_unit(fmi2_real_t value,
                                          bool isRelativeQuantity) const
        noexcept
    {
        return fmi2_import_convert_from_display_unit(
            value, _du, static_cast<int>(isRelativeQuantity));
    }
};

class unit_t
{
private:
    fmi2_import_unit_t *_u;

public:
    explicit unit_t(fmi2_import_unit_t *u) : _u{u} {}

    const char *name() const noexcept
    {
        return fmi2_import_get_unit_name(_u);
    }

    unsigned int display_unit_size() const noexcept
    {
        return fmi2_import_get_unit_display_unit_number(_u);
    }

    std::array<int, fmi2_SI_base_units_Num> SI_unit_exponents() const noexcept
    {
        const int *exp = fmi2_import_get_SI_unit_exponents(_u);
        std::array<int, fmi2_SI_base_units_Num> ret;
        std::copy(exp, exp + fmi2_SI_base_units_Num, ret.begin());
        return ret;
    }

    fmi2_real_t factor() const noexcept
    {
        return fmi2_import_get_SI_unit_factor(_u);
    }

    fmi2_real_t offset() const noexcept
    {
        return fmi2_import_get_SI_unit_offset(_u);
    }

    fmi2_real_t convert_to_SI_base_unit(fmi2_real_t value) noexcept
    {
        return fmi2_import_convert_to_SI_base_unit(value, _u);
    }

    fmi2_real_t convert_from_SI_base_unit(fmi2_real_t value) noexcept
    {
        return fmi2_import_convert_from_SI_base_unit(value, _u);
    }

    display_unit_t display_unit(size_t index) const noexcept
    {
        return display_unit_t{fmi2_import_get_unit_display_unit(_u, index)};
    }
};

class unit_definitions_t
{
private:
    fmi2_import_unit_definitions_t *_ud;

public:
    unit_definitions_t() = delete;

    explicit unit_definitions_t(fmi2_import_unit_definitions_t *ud) : _ud{ud} {}

    unsigned int size() const noexcept
    {
        return fmi2_import_get_unit_definitions_number(_ud);
    }
};

class variable_t
{
private:
    /** @brief ptr to a certain scalar variable (cannot be const) */
    fmi2_import_variable_t *_v;

public:
    /** @brief delete default ctor */
    variable_t() = delete;

    /** @brief delete default ctor */
    explicit variable_t(fmi2_import_variable_t *v) : _v{v} {}

    /**
     * @brief get variable name
     * @return variable name
     */
    const char *name() const noexcept
    {
        return fmi2_import_get_variable_name(_v);
    }

    /**
     * @brief get variable description
     * @return variable description
     */
    const char *description() const noexcept
    {
        return fmi2_import_get_variable_description(_v);
    }
    /**
     * @brief get variable value reference
     * @return value reference
     */
    fmi2_value_reference_t vr() const noexcept
    {
        return fmi2_import_get_variable_vr(_v);
    }

    // fmi2_import_get_variable_declared_type(fmi2_import_variable_t*)

    /**
     * @brief get variable base type
     * @return base type
     */
    fmi2_base_type_enu_t base_type() const noexcept
    {
        return fmi2_import_get_variable_base_type(_v);
    }

    /**
     * @brief check if variable contains a start value
     * @retval true variable contains start value
     * @retval false variable does not contain start value
     */
    int has_start() const noexcept
    {
        return fmi2_import_get_variable_has_start(_v);
    }
    /**
     * @brief get variable variability
     * @return variability
     */
    fmi2_variability_enu_t variability() const noexcept
    {
        return fmi2_import_get_variability(_v);
    }

    /**
     * @brief get variable causality
     */
    fmi2_causality_enu_t causality() const noexcept
    {
        return fmi2_import_get_causality(_v);
    }

    fmi2_initial_enu_t initial() const noexcept
    {
        return fmi2_import_get_initial(_v);
    }

    variable_t previous() const noexcept
    {
        return variable_t(fmi2_import_get_previous(_v));
    }

    fmi2_boolean_t can_handle_multiple_set_per_time_instant() const noexcept
    {
        return fmi2_import_get_canHandleMultipleSetPerTimeInstant(_v);
    }

    fmi2_variable_alias_kind_enu_t alias_kind() const noexcept
    {
        return fmi2_import_get_variable_alias_kind(_v);
    }

    size_t original_order() const noexcept
    {
        return fmi2_import_get_variable_original_order(_v);
    }

    fmi2_import_variable_t *c_ptr() noexcept
    {
        return _v;
    }

    const fmi2_import_variable_t *c_ptr() const noexcept
    {
        return _v;
    }
};

class real_variable_t
{
private:
    fmi2_import_real_variable_t *_v;

public:
    /**
     * @brief delete default ctor
     */
    real_variable_t() = delete;

    explicit real_variable_t(fmi2_import_real_variable_t *v) : _v{v} {}

    fmi2_real_t start() const noexcept
    {
        return fmi2_import_get_real_variable_start(_v);
    }

    std::optional<real_variable_t> derivative_of() const noexcept
    {
        auto s = fmi2_import_get_real_variable_derivative_of(_v);
        if (!s) {
            return {}; // std::nullopt
        }
        return real_variable_t(s);
    }

    fmi2_boolean_t reinit() const noexcept
    {
        return fmi2_import_get_real_variable_reinit(_v);
    }

    fmi2_real_t max() const noexcept
    {
        return fmi2_import_get_real_variable_max(_v);
    }

    fmi2_real_t min() const noexcept
    {
        return fmi2_import_get_real_variable_min(_v);
    }

    fmi2_real_t nominal() const noexcept
    {
        return fmi2_import_get_real_variable_nominal(_v);
    }

    unit_t unit() const noexcept
    {
        return unit_t{fmi2_import_get_real_variable_unit(_v)};
    }

    display_unit_t display_unit() const noexcept
    {
        return display_unit_t{fmi2_import_get_real_variable_display_unit(_v)};
    }
};

class integer_variable_t
{
private:
    fmi2_import_integer_variable_t *_v;

public:
    /** @brief delete default ctor */
    integer_variable_t() = delete;

    explicit integer_variable_t(fmi2_import_integer_variable_t *v) : _v{v} {}

    int start() const noexcept
    {
        return fmi2_import_get_integer_variable_start(_v);
    }

    int min() const noexcept
    {
        return fmi2_import_get_integer_variable_min(_v);
    }

    int max() const noexcept
    {
        return fmi2_import_get_integer_variable_max(_v);
    }
};

class enum_variable_t
{
private:
    fmi2_import_enum_variable_t *_v;

public:
    enum_variable_t() = delete;
    /** @brief delete default ctor */
    explicit enum_variable_t(fmi2_import_enum_variable_t *v) : _v{v} {}

    int start() const noexcept
    {
        return fmi2_import_get_enum_variable_start(_v);
    }

    int min() const noexcept
    {
        return fmi2_import_get_enum_variable_min(_v);
    }

    int max() const noexcept
    {
        return fmi2_import_get_enum_variable_max(_v);
    }
};

class string_variable_t
{
private:
    fmi2_import_string_variable_t *_v;

public:
    string_variable_t() = delete;
    /** @brief delete default ctor */
    explicit string_variable_t(fmi2_import_string_variable_t *v) : _v{v} {}

    const char *start() const noexcept
    {
        return fmi2_import_get_string_variable_start(_v);
    }
};

class bool_variable_t
{
private:
    fmi2_import_bool_variable_t *_v;

public:
    bool_variable_t() = delete;
    /** @brief delete default ctor */
    explicit bool_variable_t(fmi2_import_bool_variable_t *v) : _v{v} {}

    fmi2_boolean_t start() const noexcept
    {
        return fmi2_import_get_boolean_variable_start(_v);
    }
};
class variable_typedef_t
{
private:
    fmi2_import_variable_typedef_t *_vt;

public:
    variable_typedef_t() = delete;

    explicit variable_typedef_t(fmi2_import_variable_typedef_t *vt) : _vt{vt} {}

    const char *type_name() const noexcept
    {
        return fmi2_import_get_type_name(_vt);
    }

    const char *type_description() const noexcept
    {
        return fmi2_import_get_type_description(_vt);
    }

    fmi2_base_type_enu_t base_type() const noexcept
    {
        return fmi2_import_get_base_type(_vt);
    }

    const char *type_quantity() const noexcept
    {
        return fmi2_import_get_type_quantity(_vt);
    }
};

/**
 *  @brief Type definition list
 */
class type_definitions_t
{
private:
    fmi2_import_type_definitions_t *_td;

public:
    type_definitions_t() = delete;

    explicit type_definitions_t(fmi2_import_type_definitions_t *td) : _td{td} {}

    std::optional<variable_typedef_t> operator[](unsigned int index) const
        noexcept
    {
        auto vt = fmi2_import_get_typedef(_td, index);
        if (!vt) {
            return {};
        }
        return variable_typedef_t{vt};
    }

    unsigned int size() const noexcept
    {
        return fmi2_import_get_type_definition_number(_td);
    }
};

class variable_list_t
{
private:
    std::unique_ptr<fmi2_import_variable_list_t,
                    decltype(&fmi2_import_free_variable_list)>
        _vl;

    std::vector<fmi2_value_reference_t> _vrs;

public:
    variable_list_t() = delete;

    /**
     * @brief variable list c'tor
     *
     * @todo:
     *  - check if `fmi2_import_get_value_referece_list` leaks!
     */
    explicit variable_list_t(fmi2_import_variable_list_t *vl)
        : _vl{vl,
              [](fmi2_import_variable_list_t *vl) {
                  fmi2_import_free_variable_list(vl);
              }},
          _vrs{fmi2_import_get_value_referece_list(vl),
               fmi2_import_get_value_referece_list(vl)
                   + fmi2_import_get_variable_list_size(vl)}
    {
    }

    /**
     * @brief Size of the variable list
     */
    auto size() const noexcept
    {
        return _vrs.size();
    }

    /**
     * @brief Clone the variable list
     */
    variable_list_t clone() const noexcept
    {
        return variable_list_t{fmi2_import_clone_variable_list(_vl.get())};
    }

    /**
     * @brief Returns the const ref to vr vector.
     *
     *  This is
     */
    const std::vector<fmi2_value_reference_t> &vrs() const noexcept
    {
        return _vrs;
    }

    std::vector<fmi2_value_reference_t> &vrs() noexcept
    {
        return _vrs;
    }

    std::optional<variable_t> operator[](size_t index) const noexcept
    {
        auto v = fmi2_import_get_variable(_vl.get(), index);
        if (!v) {
            return {};
        }
        return variable_t{v};
    }

    std::optional<variable_list_t> sublist(size_t from, size_t to) const
        noexcept
    {
        auto vl = fmi2_import_get_sublist(_vl.get(), from, to);
        if (!vl) {
            return {};
        }
        return variable_list_t{vl};
    }

    variable_list_t
    filter_variables(fmi2_import_variable_filter_function_ft filter,
                     void *context) const noexcept
    {
        return variable_list_t{
            fmi2_import_filter_variables(_vl.get(), filter, context)};
    }

    static std::optional<variable_list_t>
    join(const variable_list_t &vl_a, const variable_list_t &vl_b) noexcept
    {
        auto vl = fmi2_import_join_var_list(vl_a._vl.get(), vl_b._vl.get());
        if (!vl) {
            return {};
        }
        return variable_list_t{vl};
    }

    template <typename T = variable_list_t>
    void concatenate(T &&other) noexcept
    {
        auto vl = fmi2_import_join_var_list(_vl.get(),
                                            std::forward<T>(other)._vl.get());
        if (!vl) {
            throw std::runtime_error(
                "Memory allocation failed for new variable list");
        }
        this->_vl.reset(vl);
    }

    template <typename T = variable_t>
    void append(T &&v)
    {
        auto vl = fmi2_import_append_to_var_list(_vl.get(),
                                                 std::forward<T>(v).c_ptr());
        if (!vl) {
            throw std::runtime_error(
                "Memory allocation failed for new variable list");
        }

        this->_vl.reset(vl);
    }

    template <typename T = variable_t>
    void prepend(T &&v)
    {
        auto vl = fmi2_import_prepend_to_var_list(_vl.get(),
                                                  std::forward<T>(v).c_ptr());
        if (!vl) {
            throw std::runtime_error(
                "Memory allocation failed for new variable list");
        }
        this->_vl.reset(vl);
    }

    template <typename T = variable_t>
    jm_status_enu_t push_back(T &&v)
    {
        return fmi2_import_var_list_push_back(_vl.get(),
                                              std::forward<T>(v).c_ptr());
    }
};

template <bool is_model_exchange = true>
class fmi2_t
{
protected:
    /** @brief fmu object */
    std::unique_ptr<fmi2_import_t, decltype(&fmi2_import_free)> _fmu;
    /** @brief unique pointer to fmi import contex */
    std::unique_ptr<fmi_import_context_t, decltype(&fmi_import_free_context)>
        _ctx;
    /** @brief fmu callback functions */
    fmi2_callback_functions_t _fmu_cb;
    /** @brief jm callback functions */
    jm_callbacks _jm_cb;

public:
    fmi2_t()
        : _fmu{nullptr, [](fmi2_import_t *fmu) { fmi2_import_free(fmu); }},
          _ctx{nullptr,
               [](fmi_import_context_t *ctx) { fmi_import_free_context(ctx); }},
          _fmu_cb{fmi2_callback_functions_t{}}, _jm_cb{jm_callbacks{}}
    {
    }
    /**
     *  @brief fmi2_t constructor
     *
     *  @param[in] fmu_path fmu path
     *  @param[in] ext_dir extraction directory
     *  @param[in] fmu_cb fmu callback functions
     *  @param[in] jm_cb jm callback functions
     */
    fmi2_t(const std::string &fmu_path, const std::string &ext_dir,
           fmi2_callback_functions_t fmu_cb, jm_callbacks jm_cb,
           bool extracted = false)
        : fmi2_t{fmu_path.c_str(), ext_dir.c_str(), fmu_cb, jm_cb, extracted}
    {
    }

    /**
     *  @brief fmi2_t constructor
     *
     *  @param[in] fmu_path fmu path
     *  @param[in] ext_dir extraction directory
     *  @param[in] fmu_cb fmu callback functions
     *  @param[in] jm_cb jm callback functions
     */
    fmi2_t(const char *fmu_path, const char *ext_dir,
           fmi2_callback_functions_t fmu_cb, jm_callbacks jm_cb, bool extracted)
        : _fmu{nullptr, [](fmi2_import_t *fmu) { fmi2_import_free(fmu); }},
          _ctx{nullptr,
               [](fmi_import_context_t *ctx) { fmi_import_free_context(ctx); }},
          _fmu_cb{fmu_cb}, _jm_cb{jm_cb}
    {
        /* Init context */
        _ctx = std::unique_ptr<fmi_import_context_t,
                               decltype(&fmi_import_free_context)>(
            fmi_import_allocate_context(&_jm_cb), fmi_import_free_context);
        if (!_ctx) {
            throw std::runtime_error("Failed to initialize jmodelica context");
        }

        /* Extract FMU to `ext_dir` and get FMU fmi version */
        if (extracted == false) {
            switch (auto version
                    = fmi_import_get_fmi_version(_ctx.get(), fmu_path, ext_dir);
                    version) {
                case fmi_version_2_0_enu:
                    break;
                case fmi_version_1_enu:
                    throw std::runtime_error("Only FMI2.0 is supported.");
                    break;
                case fmi_version_unknown_enu:
                case fmi_version_unsupported_enu:
                    throw std::runtime_error(
                        "Unknown/Unsupported fmi version.");
                    break;
                default: /* this should never happen, I think */
                    break;
            }
        }

        // parse modelDescription.xml file
        _fmu = std::unique_ptr<fmi2_import_t, decltype(&fmi2_import_free)>(
            fmi2_import_parse_xml(_ctx.get(), ext_dir, nullptr),
            fmi2_import_free);
        if (!_fmu) {
            throw std::runtime_error("Failed to parse modelDescription.xml");
        }

        jm_status_enu_t jm_stat;

        if (_fmu_cb.componentEnvironment == nullptr) {
            _fmu_cb.componentEnvironment = _fmu.get();
        }

        if constexpr (is_model_exchange) {
            jm_stat = fmi2_import_create_dllfmu(_fmu.get(), fmi2_fmu_kind_me,
                                                &_fmu_cb);
        } else {
            jm_stat = fmi2_import_create_dllfmu(_fmu.get(), fmi2_fmu_kind_cs,
                                                &_fmu_cb);
        }

        if (jm_stat != jm_status_success) {
            fmi2_import_destroy_dllfmu(_fmu.get());
            throw std::runtime_error("Failed to load FMU binary");
        }
    }
    /**
     * @brief Delete copy constructor
     */
    fmi2_t(fmi2_t const &) = delete;
    /**
     * @brief Delete copy assignment constructor
     */
    fmi2_t &operator=(fmi2_t const &) = delete;
    /**
     * @brief Default move assignment ctor
     */
    fmi2_t &operator=(fmi2_t &&m) = default;
    /**
     * @brief Default move ctor
     */
    fmi2_t(fmi2_t &&m) = default;

    virtual ~fmi2_t() = default;

    fmi2_string_t model_name() const noexcept
    {
        return fmi2_import_get_model_name(_fmu.get());
    }

    unsigned int capability(fmi2_capabilities_enu_t id) const noexcept
    {
        return fmi2_import_get_capability(_fmu.get(), id);
    }

    fmi2_string_t identifier_me() const noexcept
    {
        return fmi2_import_get_model_identifier_ME(_fmu.get());
    }

    fmi2_string_t identifier_cs() const noexcept
    {
        return fmi2_import_get_model_identifier_CS(_fmu.get());
    }

    fmi2_string_t GUID() const noexcept
    {
        return fmi2_import_get_GUID(_fmu.get());
    }

    fmi2_string_t description() const noexcept
    {
        return fmi2_import_get_description(_fmu.get());
    }

    fmi2_string_t author() const noexcept
    {
        return fmi2_import_get_author(_fmu.get());
    }

    fmi2_string_t copyright() const noexcept
    {
        return fmi2_import_get_copyright(_fmu.get());
    }

    fmi2_string_t license() const noexcept
    {
        return fmi2_import_get_license(_fmu.get());
    }

    fmi2_string_t standard_version() const noexcept
    {
        return fmi2_import_get_model_standard_version(_fmu.get());
    }

    fmi2_string_t generation_tool() const noexcept
    {
        return fmi2_import_get_generation_tool(_fmu.get());
    }

    fmi2_string_t generation_date_and_time() const noexcept
    {
        return fmi2_import_get_generation_date_and_time(_fmu.get());
    }

    fmi2_variable_naming_convension_enu_t naming_convention() const noexcept
    {
        return fmi2_import_get_naming_convention(_fmu.get());
    }

    size_t number_of_continuous_states() const noexcept
    {
        return fmi2_import_get_number_of_continuous_states(_fmu.get());
    }

    size_t number_of_event_indicators() const noexcept
    {
        return fmi2_import_get_number_of_event_indicators(_fmu.get());
    }

    fmi2_real_t default_experiment_start() const noexcept
    {
        return fmi2_import_get_default_experiment_start(_fmu.get());
    }

    fmi2_real_t default_experiment_stop() const noexcept
    {
        return fmi2_import_get_default_experiment_stop(_fmu.get());
    }

    fmi2_real_t default_experiment_tolerance() const noexcept
    {
        return fmi2_import_get_default_experiment_tolerance(_fmu.get());
    }

    /**
     * @brief Get default experiment step size
     * @note
     * https://github.com/svn2github/FMILibrary/blob/d49ed3ff2dabc6e17cc4a0c6f3fa6d2ae64a1683/src/XML/src/FMI2/fmi2_xml_model_description.c#L70
     * FMI2_DEFAULT_EXPERIMENT_STEPSIZE
     */
    fmi2_real_t default_experiment_step() const noexcept
    {
        return fmi2_import_get_default_experiment_step(_fmu.get());
    }

    fmi2_fmu_kind_enu_t fmu_kind() const noexcept
    {
        return fmi2_import_get_fmu_kind(_fmu.get());
    }

    type_definitions_t type_definitions() const noexcept
    {
        return type_definitions_t{fmi2_import_get_type_definitions(_fmu.get())};
    }

    unit_definitions_t unit_definitions() const noexcept
    {
        return unit_definitions_t{fmi2_import_get_unit_definitions(_fmu.get())};
    }

    std::optional<variable_t> variable_alias_base(variable_t &v) const noexcept
    {
        auto var = fmi2_import_get_variable_alias_base(_fmu.get(), v.c_ptr());
        if (!var) {
            return {};
        }
        return variable_t{var};
    }

    template <typename T = variable_t>
    std::optional<variable_list_t> variable_aliases(T &&v) const noexcept
    {
        auto vl = fmi2_import_get_variable_aliases(_fmu.get(),
                                                   std::forward<T>(v).c_ptr());
        if (!vl) {
            return {};
        }
        return variable_list_t{vl};
    }

    /**
     * @brief Get variable list of the FMU
     *
     * https://github.com/svn2github/FMILibrary/blob/d49ed3ff2dabc6e17cc4a0c6f3fa6d2ae64a1683/src/Import/src/FMI2/fmi2_import.c#L293
     */
    std::optional<variable_list_t> variable_list(int sort_order) const noexcept
    {
        auto vl = fmi2_import_get_variable_list(_fmu.get(), sort_order);
        if (!vl) {
            return {}; // this means memory allocation failed
        }
        return variable_list_t{vl};
    }

    /**
     * @brief Create variable list from const/variable_t/variable_t & etc..
     */
    template <typename T = variable_t>
    std::optional<variable_list_t> create_var_list(T &&v) const noexcept
    {
        auto vl = fmi2_import_create_var_list(_fmu.get(),
                                              std::forward<T>(v).c_ptr());
        if (!vl) {
            return {};
        }
        return variable_list_t{vl};
    }

    size_t vendors_num() const noexcept
    {
        return fmi2_import_get_vendors_num(_fmu.get());
    }

    fmi2_string_t vendor_name(size_t index) const noexcept
    {
        return fmi2_import_get_vendor_name(_fmu.get(), index);
    }

    size_t log_categories_num() const noexcept
    {
        return fmi2_import_get_log_categories_num(_fmu.get());
    }

    fmi2_string_t log_category(size_t index) const noexcept
    {
        return fmi2_import_get_log_category(_fmu.get(), index);
    }

    fmi2_string_t log_category_description(size_t index) const noexcept
    {
        return fmi2_import_get_log_category_description(_fmu.get(), index);
    }

    size_t source_files_me_num() const noexcept
    {
        return fmi2_import_get_source_files_me_num(_fmu.get());
    }

    const char *source_file_me(size_t index) const noexcept
    {
        return fmi2_import_get_source_file_me(_fmu.get(), index);
    }

    size_t source_files_cs_num() const noexcept
    {
        return fmi2_import_get_source_files_cs_num(_fmu.get());
    }

    const char *source_file_cs(size_t index) const noexcept
    {
        return fmi2_import_get_source_file_cs(_fmu.get(), index);
    }

    std::optional<variable_t> get_variable_by_name(const char *name)
    {
        auto v = fmi2_import_get_variable_by_name(_fmu.get(), name);
        if (!v) {
            return {};
        }
        return variable_t{v};
    }

    std::optional<variable_t> get_variable_by_name(const fmi2_string_t &name)
    {
        return get_variable_by_name(name.c_str());
    }

    std::optional<variable_t> get_variable_by_vr(fmi2_base_type_enu_t baseType,
                                                 fmi2_value_reference_t vr)
    {
        auto v = fmi2_import_get_variable_by_vr(_fmu.get(), baseType, vr);
        if (!v) {
            return {};
        }
        return variable_t{v};
    }

    std::optional<variable_list_t> output_list() const noexcept
    {
        auto vl = fmi2_import_get_outputs_list(_fmu.get());
        if (!vl) {
            return {};
        }
        return variable_list_t{vl};
    }

    std::optional<variable_list_t> derivative_list() const noexcept
    {
        auto vl = fmi2_import_get_derivatives_list(_fmu.get());
        if (!vl) {
            return {};
        }
        return variable_list_t{vl};
    }

    std::optional<variable_list_t> discrete_states_list() const noexcept
    {
        auto vl = fmi2_import_get_discrete_states_list(_fmu.get());
        if (!vl) {
            return {};
        }
        return variable_list_t{vl};
    }

    std::optional<variable_list_t> initial_unknowns_list() const noexcept
    {
        auto vl = fmi2_import_get_initial_unknowns_list(_fmu.get());
        if (!vl) {
            return {};
        }
        return variable_list_t{vl};
    }

    void get_outputs_dependencies(size_t **start_index, size_t **dependency,
                                  char **factor_kind) const noexcept
    {
        fmi2_import_get_outputs_dependencies(_fmu.get(), start_index,
                                             dependency, factor_kind);
    }

    void get_derivatives_dependencies(size_t **start_index, size_t **dependency,
                                      char **factor_kind) const noexcept
    {
        fmi2_import_get_derivatives_dependencies(_fmu.get(), start_index,
                                                 dependency, factor_kind);
    }

    void get_discrete_states_dependencies(size_t **start_index,
                                          size_t **dependency,
                                          char **factor_kind) const noexcept
    {
        fmi2_import_get_discrete_states_dependencies(_fmu.get(), start_index,
                                                     dependency, factor_kind);
    }

    void get_initial_unknowns_dependencies(size_t **start_index,
                                           size_t **dependency,
                                           char **factor_kind) const noexcept
    {
        fmi2_import_get_initial_unknowns_dependencies(_fmu.get(), start_index,
                                                      dependency, factor_kind);
    }

    void collect_model_counts(fmi2_import_model_counts_t *counts) const noexcept
    {
        fmi2_import_collect_model_counts(_fmu.get(), counts);
    }

    void expand_variable_references(const char *msgIn, char *msgOut,
                                    size_t maxMsgSize) const noexcept
    {
        fmi2_import_expand_variable_references(_fmu.get(), msgIn, msgOut,
                                               maxMsgSize);
    }

    ///////////////////////////////////////////////////////////////////////////
    //  Common API
    ///////////////////////////////////////////////////////////////////////////
    jm_status_enu_t instantiate(fmi2_string_t instance_name,
                                fmi2_type_t fmu_type,
                                fmi2_string_t resource_location,
                                fmi2_boolean_t visible) noexcept
    {
        return fmi2_import_instantiate(_fmu.get(), instance_name, fmu_type,
                                       resource_location, visible);
    }

    void free_instance() noexcept
    {
        fmi2_import_free(_fmu.get());
    }

    fmi2_string_t get_version() const noexcept
    {
        return fmi2_import_get_version(_fmu.get());
    }

    fmi2_status_t set_debug_logging(fmi2_boolean_t logging_on,
                                    size_t n_categories,
                                    fmi2_string_t categories[]) noexcept
    {
        return fmi2_import_set_debug_logging(_fmu.get(), logging_on,
                                             n_categories, categories);
    }

    fmi2_status_t setup_experiment(fmi2_boolean_t tolerance_defined,
                                   fmi2_real_t tolerance,
                                   fmi2_real_t start_time,
                                   fmi2_boolean_t stop_time_defined,
                                   fmi2_real_t stop_time) noexcept
    {
        return fmi2_import_setup_experiment(_fmu.get(), tolerance_defined,
                                            tolerance, start_time,
                                            stop_time_defined, stop_time);
    }

    fmi2_status_t enter_initialization_mode() noexcept
    {
        return fmi2_import_enter_initialization_mode(_fmu.get());
    }

    fmi2_status_t exit_initialization_mode() noexcept
    {
        return fmi2_import_exit_initialization_mode(_fmu.get());
    }

    fmi2_status_t terminate() noexcept
    {
        return fmi2_import_terminate();
    }

    fmi2_status_t reset() noexcept
    {
        return fmi2_import_reset();
    }

    fmi2_status_t set_real(const fmi2_value_reference_t vrs[], size_t nvr,
                           const fmi2_real_t value[]) noexcept
    {
        return fmi2_import_set_real(_fmu.get(), vrs, nvr, value);
    }

    fmi2_status_t set_real(const std::vector<fmi2_value_reference_t> &vrs,
                           const std::vector<double> &values) noexcept
    {
        assert(vrs.size() == values.size());
        return fmi2_import_set_real(_fmu.get(), vrs.data(), vrs.size(),
                                    values.data());
    }

    fmi2_status_t set_integer(const fmi2_value_reference_t vrs[], size_t nvr,
                              const fmi2_integer_t values[]) noexcept
    {
        return fmi2_import_set_integer(_fmu.get(), vrs, nvr, values);
    }

    fmi2_status_t
    set_integer(const std::vector<fmi2_value_reference_t> &vrs,
                const std::vector<fmi2_integer_t> &values) noexcept
    {
        assert(vrs.size() == values.size());
        return fmi2_import_set_integer(_fmu.get(), vrs.data(), vrs.size(),
                                       values.data());
    }

    fmi2_status_t set_boolean(const fmi2_value_reference_t vrs[], size_t nvr,
                              const fmi2_boolean_t values[]) noexcept
    {
        return fmi2_import_set_boolean(_fmu.get(), vrs, nvr, values);
    }

    fmi2_status_t
    set_boolean(const std::vector<fmi2_value_reference_t> &vrs,
                const std::vector<fmi2_boolean_t> &values) noexcept
    {
        assert(vrs.size() == values.size());
        return fmi2_import_set_boolean(_fmu.get(), vrs.data(), vrs.size(),
                                       values.data());
    }

    fmi2_status_t set_string(const fmi2_value_reference_t vrs[], size_t nvr,
                             const fmi2_string_t values[]) noexcept
    {
        return fmi2_import_set_string(_fmu.get(), vrs, nvr, values);
    }

    fmi2_status_t set_string(const std::vector<fmi2_value_reference_t> &vrs,
                             const std::vector<fmi2_string_t> &values) noexcept
    {
        assert(vrs.size() == values.size());
        return fmi2_import_set_string(_fmu.get(), vrs.data(), vrs.size(),
                                      values.data());
    }

    fmi2_status_t get_real(const fmi2_value_reference_t vrs[], size_t nvr,
                           fmi2_real_t value[]) const noexcept
    {
        return fmi2_import_get_real(_fmu.get(), vrs, nvr, value);
    }

    fmi2_status_t get_real(const std::vector<fmi2_value_reference_t> &vrs,
                           std::vector<fmi2_real_t> &values) const noexcept
    {
        assert(vrs.size() == values.size());
        return fmi2_import_get_real(_fmu.get(), vrs.data(), vrs.size(),
                                    values.data());
    }

    fmi2_status_t get_integer(const fmi2_value_reference_t vrs[], size_t nvr,
                              fmi2_integer_t value[]) const noexcept
    {
        return fmi2_import_get_integer(_fmu.get(), vrs, nvr, value);
    }

    fmi2_status_t get_integer(const std::vector<fmi2_value_reference_t> &vrs,
                              std::vector<fmi2_integer_t> &values) const
        noexcept
    {
        assert(vrs.size() == values.size());
        return fmi2_import_get_integer(_fmu.get(), vrs.data(), vrs.size(),
                                       values.data());
    }

    fmi2_status_t get_boolean(const fmi2_value_reference_t vrs[], size_t nvr,
                              fmi2_boolean_t value[]) const noexcept
    {
        return fmi2_import_get_boolean(_fmu.get(), vrs, nvr, value);
    }

    fmi2_status_t get_boolean(const std::vector<fmi2_value_reference_t> &vrs,
                              std::vector<fmi2_boolean_t> &values) const
        noexcept
    {
        assert(vrs.size() == values.size());
        return fmi2_import_get_boolean(_fmu.get(), vrs.data(), vrs.size(),
                                       values.data());
    }

    fmi2_status_t get_string(const fmi2_value_reference_t vrs[], size_t nvr,
                             fmi2_string_t value[]) const noexcept
    {
        return fmi2_import_get_string(_fmu.get(), vrs, nvr, value);
    }

    fmi2_status_t get_string(const std::vector<fmi2_value_reference_t> &vrs,
                             std::vector<fmi2_string_t> &values) const noexcept
    {
        assert(vrs.size() == values.size());
        return fmi2_import_get_string(_fmu.get(), vrs.data(), vrs.size(),
                                      values.data());
    }

    const char *types_platform() const noexcept
    {
        return fmi2_import_get_types_platform(_fmu.get());
    }

    fmi2_status_t get_fmu_state(fmi2_FMU_state_t *s) const noexcept
    {
        return fmi2_import_get_fmu_state(_fmu.get(), s);
    }

    fmi2_status_t set_fmu_state(fmi2_FMU_state_t s) noexcept
    {
        return fmi2_import_set_fmu_state(_fmu.get(), s);
    }

    fmi2_status_t free_fmu_state(fmi2_FMU_state_t *s) const noexcept
    {
        return fmi2_import_free_fmu_state(_fmu.get(), s);
    }

    fmi2_status_t serialized_fmu_state_size(fmi2_FMU_state_t s,
                                            size_t *sz) const noexcept
    {
        return fmi2_import_serialized_fmu_state_size(_fmu.get(), s, sz);
    }

    fmi2_status_t serialize_fmu_state(fmi2_FMU_state_t s, fmi2_byte_t data[],
                                      size_t sz) const noexcept
    {
        return fmi2_import_serialize_fmu_state(_fmu.get(), s, data, sz);
    }

    fmi2_status_t serialize_fmu_state(fmi2_FMU_state_t s,
                                      std::vector<fmi2_byte_t> &data) const
        noexcept
    {
        return fmi2_import_serialize_fmu_state(_fmu.get(), s, data.data(),
                                               data.size());
    }

    fmi2_status_t de_serialize_fmu_state(const fmi2_byte_t data[], size_t sz,
                                         fmi2_FMU_state_t *s) const noexcept
    {
        return fmi2_import_de_serialize_fmu_state(_fmu.get(), data, sz, s);
    }

    fmi2_status_t de_serialize_fmu_state(const std::vector<fmi2_byte_t> data,
                                         fmi2_FMU_state_t *s) const noexcept
    {
        return fmi2_import_de_serialize_fmu_state(_fmu.get(), data.data(),
                                                  data.size(), s);
    }

    fmi2_status_t
    get_directional_derivative(const fmi2_value_reference_t v_ref[], size_t nv,
                               const fmi2_value_reference_t z_ref[], size_t nz,
                               const fmi2_real_t dv[], fmi2_real_t dz[]) const
        noexcept
    {
        return fmi2_import_get_directional_derivative(_fmu.get(), v_ref, nv,
                                                      z_ref, dv, dz);
    }

    fmi2_status_t
    get_directional_derivative(const std::vector<fmi2_value_reference_t> v_ref,
                               const std::vector<fmi2_value_reference_t> z_ref,
                               const std::vector<fmi2_real_t> dv,
                               std::vector<fmi2_real_t> &dz) const noexcept
    {
        return fmi2_import_get_directional_derivative(
            _fmu.get(), v_ref.data(), v_ref.size(), z_ref.data(), z_ref.size(),
            dz.data());
    }

    ///////////////////////////////////////////////////////////////////////////
    //  ModelExchange API
    ///////////////////////////////////////////////////////////////////////////
    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t> enter_event_mode() noexcept
    {
        return fmi2_import_enter_event_mode(_fmu.get());
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    new_discrete_states(fmi2_event_info_t *event_info) noexcept
    {
        return fmi2_import_enter_event_mode(_fmu.get(), event_info);
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    enter_continuous_time_mode() noexcept
    {
        return fmi2_import_enter_continuous_time_mode(_fmu.get());
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    set_time(fmi2_real_t time) noexcept
    {
        return fmi2_import_set_time(_fmu.get(), time);
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    set_continuous_states(const fmi2_real_t x[], size_t nx) noexcept
    {
        return fmi2_import_set_continuous_states(_fmu.get(), x, nx);
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    set_continuous_states(const std::vector<fmi2_real_t> &x) noexcept
    {
        return fmi2_import_set_continuous_states(_fmu.get(), x.data(),
                                                 x.size());
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t> completed_integrator_step(
        fmi2_boolean_t no_set_fmu_state_prior_to_current_point,
        fmi2_boolean_t *enter_event_mode,
        fmi2_boolean_t *terminate_simulation) noexcept
    {
        return fmi2_import_completed_integrator_step(
            _fmu.get(), no_set_fmu_state_prior_to_current_point,
            enter_event_mode, terminate_simulation);
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    get_derivatives(fmi2_real_t derivatives[], size_t nx) const noexcept
    {
        return fmi2_import_get_derivatives(derivatives, nx);
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    get_derivatives(std::vector<fmi2_real_t> derivatives) const noexcept
    {
        return fmi2_import_get_derivatives(derivatives.data(),
                                           derivatives.size());
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    get_event_indicators(fmi2_real_t event_indicators[], size_t ni) const
        noexcept
    {
        return fmi2_import_get_event_indicators(_fmu.get(), event_indicators,
                                                ni);
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    get_event_indicators(std::vector<fmi2_real_t> event_indicators) const
        noexcept
    {
        return fmi2_import_get_event_indicators(
            _fmu.get(), event_indicators.data(), event_indicators.size());
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    get_continuous_states(fmi2_real_t states[], size_t nx) const noexcept
    {
        return fmi2_import_get_continuous_states(_fmu.get(), states, nx);
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    get_continuous_states(std::vector<fmi2_real_t> states) const noexcept
    {
        return fmi2_import_get_continuous_states(_fmu.get(), states.data(),
                                                 states.size());
    }

    template <bool is_me = is_model_exchange>
    typename std::enable_if_t<is_me, fmi2_status_t>
    get_nominals_of_continuous_states(fmi2_real_t x_nominal[], size_t nx) const
        noexcept
    {
        return fmi2_import_get_nominals_of_continuous_states(_fmu.get(),
                                                             x_nominal, nx);
    }

    ///////////////////////////////////////////////////////////////////////////
    //  CoSimulation API
    ///////////////////////////////////////////////////////////////////////////
    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t>
    set_real_input_derivatives(const fmi2_value_reference_t vr[], size_t nvr,
                               const fmi2_integer_t order[],
                               const fmi2_real_t value[]) noexcept
    {
        return fmi2_import_set_real_input_derivatives(_fmu.get(), vr, nvr,
                                                      order, value);
    }

    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t>
    set_real_input_derivatives(const std::vector<fmi2_value_reference_t> &vrs,
                               const std::vector<fmi2_integer_t> &order,
                               const std::vector<fmi2_real_t> &value) noexcept
    {
        assert(vrs.size() == order.size() && vrs.size() == value.size());

        return fmi2_import_set_real_input_derivatives(
            _fmu.get(), vrs.data(), vrs.size(), order.data(), value.data());
    }

    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t>
    get_real_output_derivatives(const fmi2_value_reference_t vr[], size_t nvr,
                                const fmi2_integer_t order[],
                                fmi2_real_t value[]) const noexcept
    {
        return fmi2_import_get_real_output_derivatives(_fmu.get(), vr, nvr,
                                                       order, value);
    }

    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t>
    get_real_output_derivatives(const std::vector<fmi2_value_reference_t> &vrs,
                                const std::vector<fmi2_integer_t> &order,
                                std::vector<fmi2_real_t> &value) const noexcept
    {
        assert((vrs.size() == order.size()) && (vrs.size() == value.size()));

        return fmi2_import_get_real_input_derivatives(
            _fmu.get(), vrs.data(), vrs.size(), order.data(), value.data());
    }

    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t> cancel_step() noexcept
    {
        return fmi2_import_cancel_step(_fmu.get());
    }

    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t>
    do_step(fmi2_real_t current_communication_point,
            fmi2_real_t communication_step_size,
            fmi2_boolean_t new_step) noexcept
    {
        return fmi2_import_do_step(current_communication_point,
                                   communication_step_size, new_step);
    }

    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t>
    get_status(const fmi2_status_kind_t s, fmi2_status_t *value) const noexcept
    {
        return fmi2_import_get_status(_fmu.get(), s, value);
    }

    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t>
    get_real_status(const fmi2_status_kind_t s, fmi2_real_t *value) const
        noexcept
    {
        return fmi2_import_get_real_status(_fmu.get(), s, value);
    }

    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t>
    get_integer_status(const fmi2_status_kind_t s, fmi2_integer_t *value) const
        noexcept
    {
        return fmi2_import_get_integer_status(_fmu.get(), s, value);
    }

    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t>
    get_boolean_status(const fmi2_status_kind_t s, fmi2_boolean_t *value) const
        noexcept
    {
        return fmi2_import_get_boolean_status(_fmu.get(), s, value);
    }

    template <bool is_cs = !is_model_exchange>
    typename std::enable_if_t<is_cs, fmi2_status_t>
    get_string_status(const fmi2_status_kind_t s, fmi2_string_t *value) const
        noexcept
    {
        return fmi2_import_get_string_status(_fmu.get(), s, value);
    }
}; // class fmi2_t

using fmi2_me_t = fmi2_t<true>;
using fmi2_cs_t = fmi2_t<false>;
} // namespace fmilib
