#ifndef ETL_FILTER_H
#define ETL_FILTER_H

#include "etl/iter.h"

namespace Project::etl {

    template <typename Sequence, typename UnaryPredicate>
    class Filter {
        Sequence sequence;
        UnaryPredicate fn;

    public:
        constexpr Filter(Sequence sequence, UnaryPredicate fn) : sequence(sequence), fn(fn) {}

        constexpr const Filter& begin() const& { return *this; }
        constexpr const Filter& end()   const& { return *this; }
        constexpr const Filter& iter()  const& { return *this; }

        constexpr Filter begin() && { return etl::move(*this); }
        constexpr Filter end()   && { return etl::move(*this); }
        constexpr Filter iter()  && { return etl::move(*this); }

        constexpr explicit operator bool() const { return bool(sequence); }

        constexpr bool operator!=(const Filter&) const { return operator bool(); }

        constexpr void operator++() { ++sequence; }

        constexpr decltype(auto) operator*() {
            while (operator bool()) {
                if (fn(*sequence)) return *sequence;
                ++sequence;
            }
            return *sequence;
        }

        constexpr decltype(auto) operator()() {
            using R = decltype(operator*());
            if (!operator bool()) {
                if constexpr (etl::is_reference_v<R>) {
                    // undefined behaviour
                    return *static_cast<etl::add_pointer_t<etl::remove_reference_t<R>>>(nullptr);
                } else if constexpr (etl::has_empty_constructor_v<R>) {
                    // avoid segfault
                    return R();
                } else {
                    // segfault
                    return R(*static_cast<R*>(nullptr));
                }
            }

            decltype(auto) res = operator*();
            operator++();
            return res;
        }
    };

    /// create filter object from iterator
    template <typename Iterator, typename UnaryPredicate> constexpr auto
    filter(Iterator first, Iterator last, UnaryPredicate&& fn, int step = 1) {
        return etl::Filter(etl::iter(first, last, step), etl::forward<UnaryPredicate>(fn));
    }

    /// create filter object from a sequence
    template <typename Sequence, typename UnaryPredicate> constexpr auto
    filter(Sequence&& seq, UnaryPredicate&& fn) { return etl::Filter(etl::iter(seq), etl::forward<UnaryPredicate>(fn)); }

    template <typename UnaryPredicate>
    struct FilterFunction { UnaryPredicate fn; };

    /// create filter function
    template <typename UnaryPredicate> constexpr auto
    filter(UnaryPredicate&& fn) { return FilterFunction<UnaryPredicate> { etl::forward<UnaryPredicate>(fn) }; }

    /// pipe operator to apply filter function to a sequence
    template <typename Sequence, typename UnaryPredicate> constexpr auto
    operator|(Sequence&& seq, FilterFunction<UnaryPredicate> fn) { return etl::filter(etl::forward<Sequence>(seq), etl::move(fn.fn)); }
}

#endif // ETL_FILTER_H
