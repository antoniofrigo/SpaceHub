/*---------------------------------------------------------------------------*\
        .-''''-.         |
       /        \        |
      /_        _\       |  SpaceHub: The Open Source N-body Toolkit
     // \  <>  / \\      |
     |\__\    /__/|      |  Website:  https://yihanwangastro.github.io/SpaceHub/
      \    ||    /       |
        \  __  /         |  Copyright (C) 2019 Yihan Wang
         '.__.'          |
---------------------------------------------------------------------
License
    This file is part of SpaceHub.
    SpaceHub is free software: you can redistribute it and/or modify it under
    the terms of the GPL-3.0 License. SpaceHub is distributed in the hope that it
    will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GPL-3.0 License
    for more details. You should have received a copy of the GPL-3.0 License along
    with SpaceHub.
\*---------------------------------------------------------------------------*/
/**
 * @file dev-tools.hpp
 *
 * Header file.
 */
#pragma once

#include <array>
#include <iostream>
#include <tuple>

#include "IO.hpp"

namespace hub {

    template <typename STL, typename... Args>
    void emplace_back(STL &container, Args &&...args) {
        (..., container.emplace_back(std::forward<Args>(args)));
    }

    template <typename STL, typename... Args>
    void push_back(STL &container, Args &&...args) {
        (..., container.emplace_back(std::forward<Args>(args)));
    }

    template <typename... Args>
    void resize_all(size_t new_sz, Args &&...args) {
        (..., (args.resize(new_sz)));
    }

    template <typename... Args>
    void reserve_all(size_t new_cap, Args &&...args) {
        (..., (args.reserve(new_cap)));
    }

    template <typename... Args>
    void clear_all(Args &&...args) {
        (..., (args.clear()));
    }

    template <typename... Args>
    void shrink_to_fit_all(Args &&...args) {
        (..., (args.shrink_to_fit()));
    }

#define spacehub_abort(...)                                                                            \
    do {                                                                                               \
        hub::print(std::cout, __FILE__, ": Line :", __LINE__, " within \"", __FUNCTION__, "\"\r\n"); \
        hub::print(std::cout, __VA_ARGS__);                                                          \
        exit(0);                                                                                       \
    } while (0)

    /*template <typename... Args>
    [[noreturn]] void spacehub_abort(Args &&...args) {
        hub::print(std::cout, __FILE__, ": Line :", __LINE__, "\r\n");
        hub::print(std::cout, std::forward<Args>(args)...);
        exit(0);
    }*/

    class Empty {};

    template <typename T>
    struct raw_type {
       private:
        /*If U has member::value_type, getValueType<T>(0) will match this function. See details on SFINAE. */
        template <typename U>
        static typename U::value_type check(typename U::value_type);

        /*If U doesn't have member::value_type, getValueType<T>(0) will match this function. See details on SFINAE. */
        template <typename U>
        static U check(U);

       public:
        using type = decltype(check<T>(0));
    };

    template <typename T>
    using raw_type_t = typename raw_type<T>::type;

#define MACRO_CAT(A, B) MACRO_CAT_I(A, B)
#define MACRO_CAT_I(A, B) MACRO_CAT_II(~, A##B)
#define MACRO_CAT_II(P, REST) REST
#define UNIQ(BASE) MACRO_CAT(BASE, __LINE__)

#define PACK(...) std::forward_as_tuple(__VA_ARGS__)

#ifdef DEBUG
#define DEBUG_MODE(BLOCK) BLOCK
#else
#define DEBUG_MODE(BLOCK)
#endif

/** @brief Macros used to output debug info.  */
#ifdef DEBUG
#define DEBUG_MSG(EXPR, ...) (EXPR ? hub::print(std::cout, __VA_ARGS__) : void(0))
#else
#define DEBUG_MSG(EXPR, ...)
#endif

#ifdef DEBUG
#define DEBUG_MODE_ASSERT(EXPR, MSG) ((EXPR) ? void(0) : (spacehub_abort(MSG)))
#else
#define DEBUG_MODE_ASSERT(EXPR, MSG)
#endif

#define $(EXPR, X, CONTAINER)                                                                  \
    [&]() {                                                                                    \
        using Type = std::remove_reference_v<decltype(CONTAINER)>;                             \
        Type result;                                                                           \
        if constexpr (is_reservable<Type>::value) {                                            \
            result.reserve((CONTAINER).size())                                                 \
        }                                                                                      \
        std::transform(std::begin(CONTAINER), std::end(CONTAINER), std::back_inserter(result), \
                       [](auto &X) { return EXPR; });                                          \
        return result;                                                                         \
    }()

#define SPACEHUB_MAKE_CONSTRUCTORS(CLASS, ATTR1, ATTR2, ATTR3, ATTR4, ATTR5) \
    /** Default constructor. */                                              \
    CLASS() = ATTR1;                                                         \
    /** Default copy constructor. */                                         \
    CLASS(CLASS const &) = ATTR2;                                            \
    /** Default move constructor */                                          \
    CLASS(CLASS &&) = ATTR3;                                                 \
    /** Default copy assignment operator */                                  \
    CLASS &operator=(CLASS const &) = ATTR4;                                 \
    /** Default move assignment operator */                                  \
    CLASS &operator=(CLASS &&) = ATTR5;

#define SPACEHUB_USING_TYPE_SYSTEM_OF(CLASS)                   \
    using TypeSet = typename CLASS::TypeSet;                   \
    template <typename _T_>                                    \
    using Container = typename CLASS::template Container<_T_>; \
                                                               \
    using Scalar = typename CLASS::Scalar;                     \
    using ScalarArray = typename CLASS::ScalarArray;           \
    using IdxArray = typename CLASS::IdxArray;                 \
    using IntArray = typename CLASS::IntArray;                 \
    using Vector = typename CLASS::Vector;                     \
    using VectorArray = typename CLASS::VectorArray;           \
    using StateScalar = typename CLASS::StateScalar;           \
    using StateScalarArray = typename CLASS::StateScalarArray; \
    using StateVector = typename CLASS::StateVector;           \
    using StateVectorArray = typename CLASS::StateVectorArray

#define SPACEHUB_STD_ACCESSOR(TYPE, NAME, MEMBER)                    \
    /** The setter interface of member `MEMBER` in name of `NAME`.*/ \
    inline TYPE &NAME() noexcept { return MEMBER; };                 \
    /** The getter interface of member `MEMBER` in name of `NAME`.*/ \
    inline TYPE const &NAME() const noexcept { return MEMBER; };

#define SPACEHUB_READ_ACCESSOR(TYPE, NAME, MEMBER)                    \
    /** The getter interface of member `MEMBER` in name of `NAME`. */ \
    inline TYPE const &NAME() const noexcept { return MEMBER; };

#define SPACEHUB_ARRAY_ACCESSOR(TYPE, NAME, MEMBER)                                  \
    /** The setter interface of member `MEMBER` in name of `NAME`.*/                 \
    inline TYPE &NAME() noexcept { return MEMBER; };                                 \
    /** The getter interface of member `MEMBER` in name of `NAME`.*/                 \
    inline TYPE const &NAME() const noexcept { return MEMBER; };                     \
    /** The setter interface of member `MEMBER` in name of `NAME`.*/                 \
    inline typename TYPE::value_type &NAME(size_t i) noexcept { return MEMBER[i]; }; \
    /** The getter interface of member `MEMBER` in name of `NAME`.*/                 \
    inline typename TYPE::value_type const &NAME(size_t i) const noexcept { return MEMBER[i]; };

#define SPACEHUB_ARRAY_READ_ACCESSOR(TYPE, NAME, MEMBER)             \
    /** The getter interface of member `MEMBER` in name of `NAME`.*/ \
    inline TYPE const &NAME() const noexcept { return MEMBER; };     \
    /** The getter interface of member `MEMBER` in name of `NAME`.*/ \
    inline typename TYPE::value_type const &NAME(size_t i) const noexcept { return MEMBER[i]; };

#define SPACEHUB_CONDITIONAL_ACCESSOR(CONDITION, TYPE, NAME, MEMBER)                           \
    /** The setter interface of member `MEMBER` in name of `NAME` if CONDITION is satisfied.*/ \
    inline TYPE &NAME() noexcept {                                                             \
        static_assert(CONDITION);                                                              \
        return MEMBER;                                                                         \
    };                                                                                         \
    /** The getter interface of member `MEMBER` in name of `NAME` if CONDITION is satisfied.*/ \
    inline TYPE const &NAME() const noexcept {                                                 \
        static_assert(CONDITION);                                                              \
        return MEMBER;                                                                         \
    };

#define SPACEHUB_CONDITIONAL_READ_ACCESSOR(CONDITION, TYPE, NAME, MEMBER)                       \
    /** The getter interface of member `MEMBER` in name of `NAME` if CONDITION is satisfied. */ \
    inline TYPE const &NAME() const noexcept {                                                  \
        static_assert(CONDITION);                                                               \
        return MEMBER;                                                                          \
    };

#define SPACEHUB_CONDITIONAL_ARRAY_ACCESSOR(CONDITION, TYPE, NAME, MEMBER)                     \
    /** The setter interface of member `MEMBER` in name of `NAME` if CONDITION is satisfied.*/ \
    inline TYPE &NAME() noexcept {                                                             \
        static_assert(CONDITION);                                                              \
        return MEMBER;                                                                         \
    };                                                                                         \
    /** The getter interface of member `MEMBER` in name of `NAME` if CONDITION is satisfied.*/ \
    inline TYPE const &NAME() const noexcept {                                                 \
        static_assert(CONDITION);                                                              \
        return MEMBER;                                                                         \
    };                                                                                         \
    /** The setter interface of member `MEMBER` in name of `NAME` if CONDITION is satisfied.*/ \
    inline typename TYPE::value_type &NAME(size_t i) noexcept {                                \
        static_assert(CONDITION);                                                              \
        return MEMBER[i];                                                                      \
    };                                                                                         \
    /** The getter interface of member `MEMBER` in name of `NAME` if CONDITION is satisfied.*/ \
    inline typename TYPE::value_type const &NAME(size_t i) const noexcept {                    \
        static_assert(CONDITION);                                                              \
        return MEMBER[i];                                                                      \
    };

#define SPACEHUB_CONDITIONAL_ARRAY_READ_ACCESSOR(CONDITION, TYPE, NAME, MEMBER)                \
    /** The getter interface of member `MEMBER` in name of `NAME` if CONDITION is satisfied.*/ \
    inline TYPE const &NAME() const noexcept {                                                 \
        static_assert(CONDITION);                                                              \
        return MEMBER;                                                                         \
    };                                                                                         \
    /** The getter interface of member `MEMBER` in name of `NAME` if CONDITION is satisfied.*/ \
    inline typename TYPE::value_type const &NAME(size_t i) const noexcept {                    \
        static_assert(CONDITION);                                                              \
        return MEMBER[i];                                                                      \
    };

#define CREATE_METHOD_CHECK(NAME)                                                           \
    template <typename T, typename... Args>                                                 \
    struct has_method_##NAME {                                                              \
        template <typename U>                                                               \
        constexpr static auto check(const void *)                                           \
            -> decltype(std::declval<U>().NAME(std::declval<Args>()...), std::true_type()); \
                                                                                            \
        template <typename U>                                                               \
        constexpr static std::false_type check(...);                                        \
                                                                                            \
        static constexpr bool value = decltype(check<T>(nullptr))::value;                   \
    };

#define HAS_METHOD(CLASS, METHOD, ...) has_method_##METHOD<CLASS, ##__VA_ARGS__>::value

#define CREATE_PROTECTED_METHOD_CHECK(NAME)                                                                         \
    template <typename __T, typename... __Args>                                                                     \
    struct has_protected_method_##NAME : public __T {                                                               \
        template <typename U>                                                                                       \
        constexpr static auto check(const void *)                                                                   \
            -> decltype(std::declval<has_protected_method_##NAME<U, __Args...> >().NAME(std::declval<__Args>()...), \
                        std::true_type());                                                                          \
                                                                                                                    \
        template <typename U>                                                                                       \
        constexpr static std::false_type check(...);                                                                \
                                                                                                                    \
        static constexpr bool value = decltype(check<__T>(nullptr))::value;                                         \
    };

#define HAS_PROTECTED_METHOD(CLASS, METHOD, ...) has_protected_method_##METHOD<CLASS, ##__VA_ARGS__>::value

/** @brief Macros used to check if a class has a member.*/
#define CREATE_MEMBER_CHECK(MEMBER)                                                                                 \
    template <typename __T, typename __V = bool>                                                                    \
    struct has_##MEMBER : std::false_type {};                                                                       \
                                                                                                                    \
    template <typename __T>                                                                                         \
    struct has_##MEMBER<                                                                                            \
        __T, typename std::enable_if<!std::is_same<decltype(std::declval<__T>().MEMBER), void>::value, bool>::type> \
        : std::true_type {};

#define HAS_MEMBER(C, member) has_##member<C>::value

/**  Macros used to check if a class has a static member.  */
#define CREATE_STATIC_MEMBER_CHECK(MEMBER)                                                                             \
                                                                                                                       \
    template <typename T, typename V = bool>                                                                           \
    struct has_static##MEMBER : std::false_type {};                                                                    \
                                                                                                                       \
    template <typename T>                                                                                              \
    struct has_static##MEMBER<T, typename std::enable_if<!std::is_same<decltype(T::MEMBER), void>::value, bool>::type> \
        : std::true_type {};

#define HAS_STATIC_MEMBER(C, member) has_static##member<C>::value

/** @brief Macros used to static_assert if a class has a specific method. */
#define CHECK_METHOD(CLASS, METHOD, ...)                            \
                                                                    \
    static_assert(has_method_##METHOD<CLASS, ##__VA_ARGS__>::value, \
                  "Template argument '" #CLASS "' must have method '" #METHOD "(" #__VA_ARGS__ ")'. ");

/** @brief Macros used to static_assert if a class has a member. */
#define CHECK_MEMBER(CLASS, MB) \
    static_assert(has_##MB<C>::value, "Template argument '" #CLASS "' must have member '" #MB "'. ");

/** @brief Macros used to static_assert if a class has a static member.*/
#define CHECK_STATIC_MEMBER(CLASS, MB) \
                                       \
    static_assert(has_static##MB<CLASS>::value, "Template argument '" #CLASS "' must have static member '" #MB "'. ");

/** @brief Macros used to static_assert if two class have the same base type set*/
#define CHECK_TYPE(T1, T2)                                                     \
    static_assert(std::is_same<typename T1::Types, typename T2::Types>::value, \
                  "Template argument '" #T1 "' and '" #T2                      \
                  "' must have the same type of the type member(hub::ProtoType<...>)");

#define CHECK_POD(DATA) static_assert(std::is_trivial<DATA>::value, "Template arg '" #DATA "' must be a POD type!");

    template <typename T, typename... Args>
    struct is_indexable {
        template <typename U>
        constexpr static auto check(const void *)
            -> decltype(std::declval<U>().operator[](std::declval<Args>()...), std::true_type());

        template <typename U>
        constexpr static std::false_type check(...);

        static constexpr bool value = decltype(check<T>(nullptr))::value;
    };

    template <typename T, typename _ = void>
    struct is_ranges : std::false_type {};

    template <typename... Ts>
    struct is_ranges_helper {};

    template <typename T>
    struct is_ranges<
        T, std::conditional_t<false,
                              is_ranges_helper<typename T::value_type,
                                               // typename T::size_type,
                                               // typename T::allocator_type,
                                               // typename T::iterator,
                                               // typename T::const_iterator,
                                               decltype(std::declval<T>().size()), decltype(std::declval<T>().begin()),
                                               decltype(std::declval<T>().end())>,
                              void> > : public std::true_type {};

    template <typename T>
    constexpr bool is_ranges_v = is_ranges<T>::value;

#define IS_BASE_OF(BASE, DERIVED) (std::is_base_of<BASE, DERIVED>::value)

#define TYPE_OF_SELF std::remove_reference<decltype(*this)>::type

#define ALIGNED_OPERATOR_NEW                                                                                   \
    void *operator new(std::size_t count) {                                                                    \
        void *original = ::operator new(count + 32);                                                           \
        void *aligned = reinterpret_cast<void *>((reinterpret_cast<size_t>(original) & ~size_t(32 - 1)) + 32); \
        *(reinterpret_cast<void **>(aligned) - 1) = original;                                                  \
        return aligned;                                                                                        \
    }                                                                                                          \
    void operator delete(void *ptr) { ::operator delete(*(reinterpret_cast<void **>(ptr) - 1)); }

}  // namespace hub
