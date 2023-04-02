

// https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms

#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

#define COMPL(b) PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0

#define BITAND(x) PRIMITIVE_CAT(BITAND_, x)
#define BITAND_0(y) 0
#define BITAND_1(y) y

#define PPINC(x) PRIMITIVE_CAT(PPINC_, x)
#define PPINC_0 1
#define PPINC_1 2
#define PPINC_2 3
#define PPINC_3 4
#define PPINC_4 5
#define PPINC_5 6
#define PPINC_6 7
#define PPINC_7 8
#define PPINC_8 9
#define PPINC_9 9

#define PPDEC(x) PRIMITIVE_CAT(PPDEC_, x)
#define PPDEC_0 0
#define PPDEC_1 0
#define PPDEC_2 1
#define PPDEC_3 2
#define PPDEC_4 3
#define PPDEC_5 4
#define PPDEC_6 5
#define PPDEC_7 6
#define PPDEC_8 7
#define PPDEC_9 8

#define CHECK_N(x, n, ...) n
#define CHECK(...) CHECK_N(__VA_ARGS__, 0,)
#define PROBE(x) x, 1,

#define IS_PAREN(x) CHECK(IS_PAREN_PROBE x)
#define IS_PAREN_PROBE(...) PROBE(~)

#define NOT(x) CHECK(PRIMITIVE_CAT(NOT_, x))
#define NOT_0 PROBE(~)

#define BOOL(x) COMPL(NOT(x))
#define IF(c) IIF(BOOL(c))

#define EAT(...)
#define EXPAND(...) __VA_ARGS__
#define WHEN(c) IF(c)(EXPAND, EAT)

#define EMPTY()
#define DEFER(id) id EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EXPAND(...) __VA_ARGS__

#define EVAL(...)  EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL3(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL4(...) EVAL5(EVAL5(EVAL5(__VA_ARGS__)))
#define EVAL5(...) __VA_ARGS__

#define REPEAT(_count, macro, ...) \
    WHEN(_count) \
    ( \
        OBSTRUCT(REPEAT_INDIRECT) () \
        ( \
            PPDEC(_count), macro, __VA_ARGS__ \
        ) \
        OBSTRUCT(macro) \
        ( \
            PPDEC(_count), __VA_ARGS__ \
        ) \
    )
#define REPEAT_INDIRECT() REPEAT

