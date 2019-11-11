/*
 * RESTinio
 */

/*!
 * \file
 * \brief Various tools for C++ metaprogramming.
 *
 * \since
 * v.0.6.1
 */

#pragma once

#include <type_traits>

namespace restinio
{

namespace utils
{

namespace metaprogramming
{

// See https://en.cppreference.com/w/cpp/types/void_t for details.
template<typename... Ts> struct make_void { using type = void; };
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

namespace impl
{

//
// debug_print
//
/*
 * NOTE: this type is intended to be used just for debugging
 * metaprogramming stuff. That is why it hasn't the definition.
 */
template<typename T>
struct debug_print;

} /* namespace impl */

//
// type_list
//
/*!
 * @brief The basic building block: a type for representation of a type list.
 *
 * @since v.0.6.1
 */
template<typename... Types>
struct type_list {};

namespace impl
{

//
// head_of
//
template<typename T, typename... Rest>
struct head_of
{
	using type = T;
};

template<typename T>
struct head_of<T>
{
	using type = T;
};

} /* namespace impl */

//
// head_of_t
//
/*!
 * @brief Metafunction to get the first item from a list of types.
 *
 * Usage example:
 * @code
 * using T = restinio::utils::metaprogramming::head_of_t<int, float, double>;
 * static_assert(std::is_same_v<T, int>, "T isn't int");
 * @endcode
 *
 * @since v.0.6.1
 */
template<typename... L>
using head_of_t = typename impl::head_of<L...>::type;

namespace impl
{

//
// tail_of
//
template<typename T, typename... Rest>
struct tail_of
{
	using type = type_list<Rest...>;
};

template<typename L>
struct tail_of<L>
{
	using type = type_list<>;
};

} /* namespace impl */

/*!
 * @brief Metafunction to get the tail of a list of types in a form of type_list.
 *
 * Returns all types expect the first one. If input list of types contains
 * just one type then `type_list<>` is returned.
 *
 * Usage example:
 * @code
 * using T = restinio::utils::metaprogramming::tail_of_t<int, float, double>;
 * static_assert(std::is_same_v<T,
 * 		restinio::utils::metaprogramming::typelist<float, double> >, "!Ok");
 * @endcode
 *
 * @since v.0.6.1
 */
template<typename... L>
using tail_of_t = typename impl::tail_of<L...>::type;

namespace impl
{

//
// put_front
//
template<typename T, typename Rest>
struct put_front;

template<typename T, template<class...> class L, typename... Rest>
struct put_front< T, L<Rest...> >
{
	using type = L<T, Rest...>;
};

} /* namespace impl */

//
// put_front_t
//
/*!
 * @brief Metafunction to insert a type to the front of a type_list.
 *
 * Usage example:
 * @code
 * using namespace restinio::utils::metaprogramming;
 *
 * using T = put_front_t<int, type_list<float, double>>;
 * static_assert(std::is_same_v<T, typelist<int, float, double> >, "!Ok");
 * @endcode
 *
 * @since v.0.6.1
 */
template<typename T, typename Rest>
using put_front_t = typename impl::put_front<T, Rest>::type;

namespace impl
{

//
// rename
//
template<typename From, template<class...> class To>
struct rename;

template<
	template<class...> class From,
	typename... Types,
	template<class...> class To>
struct rename<From<Types...>, To>
{
	using type = To<Types...>;
};

} /* namespace impl */

//
// rename_t
//
/*!
 * @brief Allows to pass all template arguments from one type to another.
 *
 * Usage example:
 * @code
 * using namespace restinio::utils::metaprogramming;
 * using T = rename_t<typelist<int, float, double>, std::tuple>;
 * static_assert(std::is_same_v<T, std::tuble<int, float, double>>, "!Ok");
 * @endcode
 *
 * @since v.0.6.1
 */
template<typename From, template<class...> class To>
using rename_t = typename impl::rename<From, To>::type;

namespace impl
{

//
// all_of
//
template<
	template<class...> class Predicate,
	typename H,
	typename... Tail >
struct all_of
{
	static constexpr bool value = Predicate<H>::value &&
			all_of<Predicate, Tail...>::value;
};

template<
	template<class...> class Predicate,
	typename H >
struct all_of< Predicate, H >
{
	static constexpr bool value = Predicate<H>::value;
};

} /* namespace impl */

//
// all_of
//
//FIXME: document this!
template< template<class...> class Predicate, typename... List >
constexpr bool all_of_v = impl::all_of<Predicate, List...>::value;

} /* namespace metaprogramming */

} /* namespace utils */

} /* namespace restinio */

