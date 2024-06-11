#ifndef ETL_ASYNC_H
#define ETL_ASYNC_H

#include "etl/async_task.h"

namespace Project::etl {
    template <typename F, typename... Args>
    auto async(F&& fn, Args&&... args) {
        return Tasks::self->launch(etl::forward<F>(fn), etl::forward<Args>(args)...);
    }

    template <auto method, typename Class, typename... Args>
    auto async(Class* self, Args&&... args) {
        return Tasks::self->launch(etl::bind<method>(self), etl::forward<Args>(args)...);
    }

    template <typename T> class Async;

    template <typename R, typename... Args>
    class [[nodiscard]] Async<R(Args...)> {
    public:
        template <typename Functor>
        Async(Functor&& f) : fn(std::forward<Functor>(f)) {}

        Future<R> operator()(Args... args) const { 
            return async(fn, etl::forward<Args>(args)...);
        }

    private:
        std::function<R(Args...)> fn;
    };
}

#endif