/*
    Copyright (c) 2018, Hang Yu
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of  nor the names of its contributors may be used to
       endorse or promote products derived from this software without specific
       prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include <fmilib.h>

namespace fmilib
{

using real_type = fmi2_real_t;
using integer_type = fmi2_integer_t;
using boolean_type = bool;
using string_type = std::string;
using enum_type = fmi2_integer_t;
using size_type = std::size_t;
using vr_type = fmi2_value_reference_t;

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
    string_type name() const noexcept
    {
        return fmi2_import_get_variable_name(_v);
    }

    /**
     * @brief get variable description
     * @return variable description
     */
    string_type description() const noexcept
    {
        return fmi2_import_get_variable_description(_v);
    }
    /**
     * @brief get variable value reference
     * @return value reference
     */
    vr_type vr() const noexcept
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
    boolean_type has_start() const noexcept
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

    boolean_type can_handle_multiple_set_per_time_instant() const noexcept
    {
        return fmi2_import_get_canHandleMultipleSetPerTimeInstant(_v);
    }

    fmi2_variable_alias_kind_enu_t alias_kind() const noexcept
    {
        return fmi2_import_get_variable_alias_kind(_v);
    }

    size_type original_order() const noexcept
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
    real_variable_t() = delete;
    /** @brief delete default ctor */
    explicit real_variable_t(fmi2_import_real_variable_t *v) : _v{v} {}

    real_type start() const noexcept
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

    boolean_type reinit() const noexcept
    {
        return fmi2_import_get_real_variable_reinit(_v);
    }

    real_type max() const noexcept
    {
        return fmi2_import_get_real_variable_max(_v);
    }

    real_type min() const noexcept
    {
        return fmi2_import_get_real_variable_min(_v);
    }

    real_type nominal() const noexcept
    {
        return fmi2_import_get_real_variable_nominal(_v);
    }

    // unit
    // display unit
};

class integer_variable_t
{
private:
    fmi2_import_integer_variable_t *_v;

public:
    /** @brief delete default ctor */
    integer_variable_t() = delete;

    explicit integer_variable_t(fmi2_import_integer_variable_t *v) : _v{v} {}

    integer_type start() const noexcept
    {
        return fmi2_import_get_integer_variable_start(_v);
    }

    integer_type min() const noexcept
    {
        return fmi2_import_get_integer_variable_min(_v);
    }

    integer_type max() const noexcept
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

    enum_type start() const noexcept
    {
        return fmi2_import_get_enum_variable_start(_v);
    }

    enum_type min() const noexcept
    {
        return fmi2_import_get_enum_variable_min(_v);
    }

    enum_type max() const noexcept
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

    string_type start() const noexcept
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

    boolean_type start() const noexcept
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

    string_type type_name() const noexcept
    {
        return fmi2_import_get_type_name(_vt);
    }

    string_type type_description() const noexcept
    {
        return fmi2_import_get_type_description(_vt);
    }

    fmi2_base_type_enu_t base_type() const noexcept
    {
        return fmi2_import_get_base_type(_vt);
    }

    string_type type_quantity() const noexcept
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

    size_type size() const noexcept
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

public:
    variable_list_t() = delete;

    variable_list_t(fmi2_import_variable_list_t *vl)
        : _vl{vl, [](fmi2_import_variable_list_t *vl) {
                  fmi2_import_free_variable_list(vl);
              }}
    {
    }

    /**
     * @brief Size of the variable list
     */
    size_type size() const noexcept
    {
        return fmi2_import_get_variable_list_size(_vl.get());
    }

    /**
     * @brief Clone the variable list
     */
    variable_list_t clone() const noexcept
    {
        return variable_list_t{fmi2_import_clone_variable_list(_vl.get())};
    }

    /**
     * @brief get a unique ptr to value reference array of the variable list
     * @return fmi2_value_reference_t unique ptr
     *
     * FMILibrary's `fmi2_import_get_value_referece_list` is expensive and
     * dangerous.
     * - It tries to `malloc` the value reference array every time
     * `fmi2_import_get_value_referece_list` is called
     * - A user must remember to deallocate the value reference array
     * him/herself
     *
     * @warning This function only makes sure fmi2_value_reference_t array gets
     * deallocated, so, avoid using this function if possible.
     */
    std::unique_ptr<const vr_type> value_reference_list() const noexcept
    {
        return std::unique_ptr<const vr_type>(
            fmi2_import_get_value_referece_list(_vl.get()));
    }

    std::optional<variable_t> operator[](size_type index) const noexcept
    {
        auto v = fmi2_import_get_variable(_vl.get(), index);
        if (!v) {
            return {};
        }
        return variable_t{v};
    }

    std::optional<variable_list_t> sublist(size_type from, size_type to) const
        noexcept
    {
        auto vl = fmi2_import_get_sublist(_vl.get(), from, to);
        if (!vl) {
            return {};
        }
        return variable_list_t{vl};
    }

    // template <typename T>
    // std::optional<variable_list_t> filter_variables(
    //    std::function<int(fmi2_import_variable_list_t *vl, T *data)> filter,
    //    T *data)
    //{
    //    return fmi2_import_filter_variables(_vl.get(), filter.target(), data);
    //}

    // filter

    static variable_list_t join(const variable_list_t &vl_a,
                                const variable_list_t &vl_b) noexcept
    {
        return variable_list_t{
            fmi2_import_join_var_list(vl_a._vl.get(), vl_b._vl.get())};
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
    string_type name() const noexcept
    {
        return fmi2_import_get_display_unit_name(_du);
    }

    real_type factor() const noexcept
    {
        return fmi2_import_get_display_unit_factor(_du);
    }

    real_type offset() const noexcept
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
    real_type convert_to_display_unit(real_type value,
                                      bool isRelativeQuantity) const noexcept
    {
        return fmi2_import_convert_to_display_unit(
            value, _du, static_cast<int>(isRelativeQuantity));
    }

    /**
     * @brief Convert a value measured in "display units" to a value measured
     * with "units"
     *
     * @param value The value to be converted.
     * @param isRelativeQuantity specifies if "offset" should be incorporated
     * into conversion
     */
    real_type convert_from_display_unit(real_type value,
                                        bool isRelativeQuantity) const noexcept
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

    string_type name() const noexcept
    {
        return fmi2_import_get_unit_name(_u);
    }

    size_type display_unit_size() const noexcept
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

    double factor() const noexcept
    {
        return fmi2_import_get_SI_unit_factor(_u);
    }

    double offset() const noexcept
    {
        return fmi2_import_get_SI_unit_offset(_u);
    }

    double convert_to_SI_base_unit(double value) noexcept
    {
        return fmi2_import_convert_to_SI_base_unit(value, _u);
    }

    double convert_from_SI_base_unit(double value) noexcept
    {
        return fmi2_import_convert_from_SI_base_unit(value, _u);
    }

    display_unit_t display_unit(size_type index) const noexcept
    {
        return display_unit_t{fmi2_import_get_unit_display_unit(_u, index)};
    }
};

class unit_definitions_t
{
private:
    fmi2_import_unit_definitions_t *_ud;

public:
    explicit unit_definitions_t(fmi2_import_unit_definitions_t *ud) : _ud{ud} {}

    size_type size() const noexcept
    {
        return fmi2_import_get_unit_definitions_number(_ud);
    }
};

template <bool ModelExchange = true>
class fmi2_t
{
protected:
    /** @brief unique pointer to fmi import contex */
    std::unique_ptr<fmi_import_context_t, decltype(&fmi_import_free_context)>
        _ctx;
    /** @brief fmu object */
    std::unique_ptr<fmi2_import_t, decltype(&fmi2_import_free)> _fmu;
    /** @brief fmu callback functions */
    fmi2_callback_functions_t _fmu_cb;
    /** @brief jm callback functions */
    jm_callbacks _jm_cb;

public:
    /**
     *  @brief fmi2_t constructor
     *
     *  @param[in] fmu_path fmu path
     *  @param[in] ext_dir extraction directory
     *  @param[in] fmu_cb fmu callback functions
     *  @param[in] jm_cb jm callback functions
     */
    fmi2_t(const std::string &fmu_path, const std::string &ext_dir,
           fmi2_callback_functions_t fmu_cb, jm_callbacks jm_cb)
        : fmi2_t{fmu_path.c_str(), ext_dir.c_str(), fmu_cb, jm_cb}
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
           fmi2_callback_functions_t fmu_cb, jm_callbacks jm_cb)
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
        fmi_version_enu_t version
            = fmi_import_get_fmi_version(_ctx.get(), fmu_path, ext_dir);
        switch (version) {
            case fmi_version_2_0_enu:
                break;
            case fmi_version_1_enu:
                throw std::runtime_error("Only FMI2.0 is supported.");
                break;
            case fmi_version_unknown_enu:
            case fmi_version_unsupported_enu:
                throw std::runtime_error("Unknown/Unsupported fmi version.");
                break;
            default: /* this should never happen, I think */
                break;
        }

        /* Parse fmi2 modelDescription.xml */
        _fmu = std::unique_ptr<fmi2_import_t, decltype(&fmi2_import_free)>(
            fmi2_import_parse_xml(_ctx.get(), ext_dir, nullptr),
            fmi2_import_free);
        if (!_fmu) {
            throw std::runtime_error("Failed to parse modelDescription.xml");
        }

        jm_status_enu_t jm_stat;
        if constexpr (ModelExchange) {
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

    string_type model_name() const noexcept
    {
        return fmi2_import_get_model_name(_fmu.get());
    }

    unsigned int capability(fmi2_capabilities_enu_t id) const noexcept
    {
        return fmi2_import_get_capability(_fmu.get(), id);
    }

    string_type identifier_me() const noexcept
    {
        return fmi2_import_get_model_identifier_ME(_fmu.get());
    }

    string_type identifier_cs() const noexcept
    {
        return fmi2_import_get_model_identifier_CS(_fmu.get());
    }

    string_type GUID() const noexcept
    {
        return fmi2_import_get_GUID(_fmu.get());
    }

    string_type description() const noexcept
    {
        return fmi2_import_get_description(_fmu.get());
    }

    string_type author() const noexcept
    {
        return fmi2_import_get_author(_fmu.get());
    }

    string_type copyright() const noexcept
    {
        return fmi2_import_get_copyright(_fmu.get());
    }

    string_type license() const noexcept
    {
        return fmi2_import_get_license(_fmu.get());
    }

    string_type standard_version() const noexcept
    {
        return fmi2_import_get_model_standard_version(_fmu.get());
    }

    string_type generation_tool() const noexcept
    {
        return fmi2_import_get_generation_tool(_fmu.get());
    }

    string_type generation_date_and_time() const noexcept
    {
        return fmi2_import_get_generation_date_and_time(_fmu.get());
    }

    fmi2_variable_naming_convension_enu_t naming_convention() const noexcept
    {
        return fmi2_import_get_naming_convention(_fmu.get());
    }

    size_type n_x() const noexcept
    {
        return fmi2_import_get_number_of_continuous_states(_fmu.get());
    }

    size_type n_z() const noexcept
    {
        return fmi2_import_get_number_of_event_indicators(_fmu.get());
    }

    real_type default_experiment_start() const noexcept
    {
        return fmi2_import_get_default_experiment_start(_fmu.get());
    }

    real_type default_experiment_stop() const noexcept
    {
        return fmi2_import_get_default_experiment_stop(_fmu.get());
    }

    real_type default_experiment_tolerance() const noexcept
    {
        return fmi2_import_get_default_experiment_tolerance(_fmu.get());
    }

    real_type default_experiment_step() const noexcept
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

    size_type vendors_num() const noexcept
    {
        return fmi2_import_get_vendors_num(_fmu.get());
    }

    string_type vendor_name(size_type index) const noexcept
    {
        return fmi2_import_get_vendor_name(_fmu.get(), index);
    }

    size_type log_categories_num() const noexcept
    {
        return fmi2_import_get_log_categories_num(_fmu.get());
    }

    string_type log_category(size_type index) const noexcept
    {
        return fmi2_import_get_log_category(_fmu.get(), index);
    }

    string_type log_category_description(size_type index) const noexcept
    {
        return fmi2_import_get_log_category_description(_fmu.get(), index);
    }

    size_type source_files_me_num() const noexcept
    {
        return fmi2_import_get_source_files_me_num(_fmu.get());
    }

    string_type source_file_me(size_type index) const noexcept
    {
        return fmi2_import_get_source_file_me(_fmu.get(), index);
    }

    size_type source_files_cs_num() const noexcept
    {
        return fmi2_import_get_source_files_cs_num(_fmu.get());
    }

    string_type source_file_cs(size_type index) const noexcept
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

    std::optional<variable_t> get_variable_by_name(const std::string &name)
    {
        auto v = fmi2_import_get_variable_by_name(_fmu.get(), name.c_str());
        if (!v) {
            return {};
        }
        return variable_t{v};
    }

    std::optional<variable_t> get_variable_by_vr(fmi2_base_type_enu_t baseType,
                                                 vr_type vr)
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

    // Dependency stuff missing

    ///////////////////////////////////////////////////////////////////////////
    //  Common API
    ///////////////////////////////////////////////////////////////////////////
    jm_status_enu_t instantiate(fmi2_string_t instance_name,
                                fmi2_type_t fmu_type,
                                fmi2_string_t resource_location,
                                fmi2_boolean_t visible) noexcept
    {
        return fmi2_import_instantiate(instance_name, fmu_type,
                                       resource_location, visible);
    }

    template <typename T = std::string>
    jm_status_enu_t instantiate(T &&instance_name, fmi2_type_t fmu_type,
                                T &&resource_location, bool visible) noexcept
    {
        return instantiate(std::forward<T>(instance_name).c_str(), fmu_type,
                           std::forward<T>(resource_location).c_str(), visible);
    }

    void free_instance() noexcept
    {
        fmi2_import_free(_fmu.get());
    }

    template <typename T = const char *>
    T get_version() const noexcept
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

    fmi2_status_t
    set_debug_logging(bool logging_on,
                      const std::vector<std::string> &categories) noexcept
    {
        std::vector<fmi2_string_t> c_cats(categories.size());
        /* see https://gist.github.com/javagg/3714006 */
        std::transform(categories.begin(), categories.end(), c_cats.begin(),
                       std::mem_fun_ref(&std::string::c_str));
        return set_debug_logging(logging_on, c_cats.size(), c_cats.data());
    }

    fmi2_status_t setup_experiment(fmi2_boolean_t tolerance_defined,
                                   fmi2_real_t tolerance,
                                   fmi2_real_t start_time,
                                   fmi2_boolean_t stop_time_defined,
                                   fmi2_real_t stop_time)
    {
        return fmi2_import_setup_experiment(_fmu.get(), tolerance_defined,
                                            tolerance, start_time,
                                            stop_time_defined, stop_time);
    }

    ///////////////////////////////////////////////////////////////////////////
    //  ModelExchange API
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    //  CoSimulation API
    ///////////////////////////////////////////////////////////////////////////

    // variable_list_t
}; // class context_t
} // namespace fmilib
