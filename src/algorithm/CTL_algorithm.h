#include <stdbool.h>

#define CTL_copy(Type_I, Type_O, first, last, result, direction)                     \
    do                                                                    \
    {                                                                     \
        CTL_##Type_I##_iterator __first = (first);                        \
        CTL_##Type_I##_iterator __last = (last);                          \
        CTL_##Type_O##_iterator __result = (result);                      \
                                                                          \
        while (!CTL_##Type_I##_iterator_equal(&__first, &__last))         \
        {                                                                 \
            *__result.data = *__first.data;                               \
            __first = CTL_##Type_I##_iterator_move(&__first, direction);   \
            __result = CTL_##Type_O##_iterator_move(&__result, direction); \
        }                                                                 \
    } while (false);

#define CTL_move(Type_dest, Type_source, dest, source, n)                     \
    do                                                                        \
    {                                                                         \
        dest = CTL_##Type_dest##_iterator_move(&(dest), n, false);            \
        source = CTL_##Type_source##_iterator_move(&(source), n, false);      \
        for (size_t _i = 0; _i <= (n); ++_i)                                  \
        {                                                                     \
            *(dest).data = *(source).data;                                    \
            (dest) = CTL_##Type_dest##_iterator_move(&(dest), 1, true);       \
            (source) = CTL_##Type_source##_iterator_move(&(source), 1, true); \
        }                                                                     \
    } while (false);

#define CTL_swap(Type, a, b) \
    do                       \
    {                        \
        Type _counter = (a); \
        (a) = (b);           \
        (b) = _counter;      \
                             \
    } while (false);
