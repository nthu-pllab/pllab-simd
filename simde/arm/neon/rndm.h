/* SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Copyright:
 *   2020-2021 Evan Nemerson <evan@nemerson.com>
 *   2023      Yi-Yen Chung <eric681@andestech.com> (Copyright owned by Andes Technology)
 */

#if !defined(SIMDE_ARM_NEON_RNDM_H)
#define SIMDE_ARM_NEON_RNDM_H

#include "types.h"

HEDLEY_DIAGNOSTIC_PUSH
SIMDE_DISABLE_UNWANTED_DIAGNOSTICS
SIMDE_BEGIN_DECLS_

SIMDE_FUNCTION_ATTRIBUTES
simde_float16_t
simde_vrndmh_f16(simde_float16_t a) {
  #if defined(SIMDE_ARM_NEON_A32V8_NATIVE) && defined(SIMDE_ARM_NEON_FP16)
    return vrndmh_f16(a);
  #else
    return simde_float16_from_float32(simde_math_floorf(simde_float16_to_float32(a)));
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V8_ENABLE_NATIVE_ALIASES)
  #undef vrndmh_f16
  #define vrndmh_f16(a) simde_vrndmh_f16(a)
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_float16x4_t
simde_vrndm_f16(simde_float16x4_t a) {
  #if defined(SIMDE_ARM_NEON_A32V8_NATIVE) && defined(SIMDE_ARM_NEON_FP16)
    return vrndm_f16(a);
  #else
    simde_float16x4_private
      r_,
      a_ = simde_float16x4_to_private(a);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = simde_vrndmh_f16(a_.values[i]);
    }

    return simde_float16x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V8_ENABLE_NATIVE_ALIASES)
  #undef vrndm_f16
  #define vrndm_f16(a) simde_vrndm_f16(a)
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_float32x2_t
simde_vrndm_f32(simde_float32x2_t a) {
  #if defined(SIMDE_ARM_NEON_A32V8_NATIVE)
    return vrndm_f32(a);
  #else
    simde_float32x2_private
      r_,
      a_ = simde_float32x2_to_private(a);

    #if defined(SIMDE_VECTOR_SUBSCRIPT_OPS) && HEDLEY_HAS_BUILTIN(__builtin_elementwise_floor)
      r_.values = __builtin_elementwise_floor(a_.values);
    #else
      SIMDE_VECTORIZE
      for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
        r_.values[i] = simde_math_floorf(a_.values[i]);
      }
    #endif

    return simde_float32x2_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vrndm_f32
  #define vrndm_f32(a) simde_vrndm_f32(a)
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_float64x1_t
simde_vrndm_f64(simde_float64x1_t a) {
  #if defined(SIMDE_ARM_NEON_A64V8_NATIVE)
    return vrndm_f64(a);
  #else
    simde_float64x1_private
      r_,
      a_ = simde_float64x1_to_private(a);

    #if defined(SIMDE_VECTOR_SUBSCRIPT_OPS) && HEDLEY_HAS_BUILTIN(__builtin_elementwise_floor)
      r_.values = __builtin_elementwise_floor(a_.values);
    #else
      SIMDE_VECTORIZE
      for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
        r_.values[i] = simde_math_floor(a_.values[i]);
      }
    #endif

    return simde_float64x1_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A64V8_ENABLE_NATIVE_ALIASES)
  #undef vrndm_f64
  #define vrndm_f64(a) simde_vrndm_f64(a)
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_float16x8_t
simde_vrndmq_f16(simde_float16x8_t a) {
  #if defined(SIMDE_ARM_NEON_A32V8_NATIVE) && defined(SIMDE_ARM_NEON_FP16)
    return vrndmq_f16(a);
  #else
    simde_float16x8_private
      r_,
      a_ = simde_float16x8_to_private(a);

    SIMDE_VECTORIZE
    for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
      r_.values[i] = simde_vrndmh_f16(a_.values[i]);
    }

    return simde_float16x8_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V8_ENABLE_NATIVE_ALIASES)
  #undef vrndmq_f16
  #define vrndmq_f16(a) simde_vrndmq_f16(a)
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_float32x4_t
simde_vrndmq_f32(simde_float32x4_t a) {
  #if defined(SIMDE_ARM_NEON_A32V8_NATIVE)
    return vrndmq_f32(a);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    return vec_floor(a);
  #else
    simde_float32x4_private
      r_,
      a_ = simde_float32x4_to_private(a);

    #if defined(SIMDE_X86_SSE4_1_NATIVE)
      r_.m128 = _mm_round_ps(a_.m128, _MM_FROUND_TO_NEG_INF);
    #elif defined(SIMDE_X86_SVML_NATIVE) && defined(SIMDE_X86_SSE_NATIVE)
      r_.m128 = _mm_floor_ps(a_.m128);
    #elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS) && HEDLEY_HAS_BUILTIN(__builtin_elementwise_floor)
      r_.values = __builtin_elementwise_floor(a_.values);
    #else
      SIMDE_VECTORIZE
      for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
        r_.values[i] = simde_math_floorf(a_.values[i]);
      }
    #endif

    return simde_float32x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vrndmq_f32
  #define vrndmq_f32(a) simde_vrndmq_f32(a)
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_float64x2_t
simde_vrndmq_f64(simde_float64x2_t a) {
  #if defined(SIMDE_ARM_NEON_A64V8_NATIVE)
    return vrndmq_f64(a);
  #elif defined(SIMDE_POWER_ALTIVEC_P7_NATIVE)
    return vec_floor(a);
  #else
    simde_float64x2_private
      r_,
      a_ = simde_float64x2_to_private(a);

    #if defined(SIMDE_X86_SSE4_1_NATIVE)
      r_.m128d = _mm_round_pd(a_.m128d, _MM_FROUND_TO_NEG_INF);
    #elif defined(SIMDE_X86_SVML_NATIVE) && defined(SIMDE_X86_SSE_NATIVE)
      r_.m128d = _mm_floor_pd(a_.m128d);
    #elif defined(SIMDE_VECTOR_SUBSCRIPT_OPS) && HEDLEY_HAS_BUILTIN(__builtin_elementwise_floor)
      r_.values = __builtin_elementwise_floor(a_.values);
    #else
      SIMDE_VECTORIZE
      for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
        r_.values[i] = simde_math_floor(a_.values[i]);
      }
    #endif

    return simde_float64x2_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A64V8_ENABLE_NATIVE_ALIASES)
  #undef vrndmq_f64
  #define vrndmq_f64(a) simde_vrndmq_f64(a)
#endif

SIMDE_END_DECLS_
HEDLEY_DIAGNOSTIC_POP

#endif /* !defined(SIMDE_ARM_NEON_RNDM_H) */
