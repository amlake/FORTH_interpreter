#if ! defined HC_LOGIC_H
#define HC_LOGIC_H

// AT THE MOMENT, THE DIFFERENT SETTINGS FOR CHECK_LOGIC ONLY WORK
//   IF THIS FILE IS #include'D BEFORE ANYTHING THAT MIGHT BRING IN
//   THE SYSTEM assert.h FILE.
//
// Define various logical functions:
//	assert, precondition, invariant
// Each of these takes an argument that is a logical fact and halts the
//	program if that fact is not true.
// The implementation is a bit bizzare due to the fact that we want to
//	print the _text_ of the expression if there is an error.
//
// The "big switch" controlling the logic functions is named CHECK_LOGIC
// In can be set to three values:
// 3 (the default) -- check all logical statements
// 2 -- check function boundaries (preconditions and postconditions)
//       but not intra-module assertions and invariants
// 1 -- check only preconditions,
//       for inter-module consistency checking of modules known to be correct
// 0 -- don't check any logical statements (pre, post, invariant, or assert)

// For now, progress doesn't do anything...

#define progress(X)

#if ! defined CHECK_LOGIC
#define CHECK_LOGIC 3
#endif

#if ! defined USE_SYSTEM_ASSERT
#define USE_SYSTEM_ASSERT 0
#endif

#if CHECK_LOGIC >= 3
// CREATE assert, postcondition, and invariant

#if USE_SYSTEM_ASSERT
#include <assert.h>
#else

// prevent conflicts
#undef assert
// this may avoid including standard one for gcc 3.3?
#define FIXINC_BROKEN_ASSERT_STDLIB_CHECK

// this gets "__STRING" macro
#include <sys/cdefs.h>

int assert_failed(const char *fact_expr, const char *file, unsigned int line);
#define assert(expr)						      \
  ((void) ((expr) ? 0 :							      \
	   (assert_failed(__STRING(expr), __FILE__, __LINE__ ))))
#endif  /* else USE_SYSTEM_ASSERT */


int invariant_failed(const char *fact_expr, const char *file, unsigned int line);
#define invariant(expr)						          \
  ((void) ((expr) ? 0 :							      \
	   (invariant_failed(__STRING(expr), __FILE__, __LINE__ ))))

#define loop_invariant(expr)						          \
  ((void) ((expr) ? 0 :							      \
	   (invariant_failed(__STRING(expr), __FILE__, __LINE__ ))))

#define rep_invariant(expr)						          \
  ((void) ((expr) ? 0 :							      \
	   (invariant_failed(__STRING(expr), __FILE__, __LINE__ ))))

#else  /* if CHECK_LOGIC >= 3 */
// DONT CREATE assert and invariant
#if defined USE_SYSTEM_ASSERT
#define NDEBUG 1
#include <assert.h>
#else
// prevent conflicts if it was already #include'd
#undef assert
// this may avoid including standard one for gcc 3.3?
#define FIXINC_BROKEN_ASSERT_STDLIB_CHECK
#define assert(X)
#endif  /* USE_SYSTEM_ASSERT */
#define invariant(X)
#endif

#if CHECK_LOGIC >= 2
// CREATE postcondition checker
int postcondition_failed(const char *fact_expr, const char *file, unsigned int line);
#define postcondition(expr)						     \
  ((void) ((expr) ? 0 :							     \
	   (postcondition_failed(__STRING(expr), __FILE__, __LINE__ ))))

#else
// don't CREATE postcondition checker
#define postcondition(X)
#endif

#if CHECK_LOGIC >= 1
// CREATE precondition checker

int precondition_failed(const char *fact_expr, const char *file, unsigned int line);
#define precondition(expr)						      \
  ((void) ((expr) ? 0 :							      \
	   (precondition_failed(__STRING(expr), __FILE__, __LINE__ ))))

#else
// DONT CREATE precondition checker
#define precondition(X)
#endif

#endif
