#define HATE_LIKELY(x)       __builtin_expect(!!(x), 1)
#define HATE_UNLIKELY(x)     __builtin_expect(!!(x), 0)
