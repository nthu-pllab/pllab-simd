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
 *   2020      Evan Nemerson <evan@nemerson.com>
 *   2020      Christopher Moore <moore@free.fr>
 *   2023      Chi-Wei Chu <wewe5215@gapp.nthu.edu.tw>
 */

#if !defined(SIMDE_ARM_NEON_SHL_H)
#define SIMDE_ARM_NEON_SHL_H

#include "types.h"

/* Notes from the implementer (Christopher Moore aka rosbif)
 *
 * I have tried to exactly reproduce the documented behaviour of the
 * ARM NEON shl and shlq intrinsics.
 * This is complicated for the following reasons:-
 *
 * a) Negative shift counts shift right.
 *
 * b) Only the low byte of the shift count is used but the shift count
 * is not limited to 8-bit values (-128 to 127).
 *
 * c) Intel SIMD is not nearly as complete as NEON and AltiVec.
 * There were no intrisics with a vector shift count before AVX2 which
 * only has 32 and 64-bit logical ones and only a 32-bit arithmetic
 * one. The others need AVX512. There are no 8-bit shift intrinsics at
 * all, even with a scalar shift count. It is surprising to use AVX2
 * and even AVX512 to implement a 64-bit vector operation.
 *
 * d) Many shift implementations, and the C standard, do not treat a
 * shift count >= the object's size in bits as one would expect.
 * (Personally I feel that > is silly but == can be useful.)
 *
 * Maybe it would be useful for SIMDe to have a flag enabling a fast
 * implementation where the result is only guaranteed for shift counts
 * conforming to the C standard.
 *
 * Note that even the C17/18 standard does not define the behaviour of
 * a right shift of a negative value.
 * However Evan and I agree that all compilers likely to be used
 * implement this as an arithmetic right shift with sign extension.
 * If this is not the case it could be replaced by a logical right shift
 * if negative values are complemented before and after the shift.
 *
 * Some of the SIMD translations may be slower than the portable code,
 * particularly those for vectors with only one or two elements.
 * But I had fun writing them ;-)
 *
 */

HEDLEY_DIAGNOSTIC_PUSH
SIMDE_DISABLE_UNWANTED_DIAGNOSTICS
SIMDE_BEGIN_DECLS_

SIMDE_FUNCTION_ATTRIBUTES
int64_t
simde_vshld_s64 (const int64_t a, const int64_t b) {
  #if defined(SIMDE_ARM_NEON_A64V8_NATIVE)
    return vshld_s64(a, b);
  #else
    int8_t b_ = HEDLEY_STATIC_CAST(int8_t, b);
    return
      (b_ >=   0)
        ? (b_ >=  64)
          ? 0
          : (a << b_)
        : (b_ <= -64)
          ? (a >> 63)
          : (a >> -b_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A64V8_ENABLE_NATIVE_ALIASES)
  #undef vshld_s64
  #define vshld_s64(a, b) simde_vshld_s64((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
uint64_t
simde_vshld_u64 (const uint64_t a, const int64_t b) {
  #if defined(SIMDE_ARM_NEON_A64V8_NATIVE)
    return vshld_u64(a, HEDLEY_STATIC_CAST(int64_t, b));
  #else
    int8_t b_ = HEDLEY_STATIC_CAST(int8_t, b);
    return
      (simde_math_llabs(b_) >= 64)
        ? 0
        : (b_  >=  0)
          ? (a <<  b_)
          : (a >> -b_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A64V8_ENABLE_NATIVE_ALIASES)
  #undef vshld_u64
  #define vshld_u64(a, b) simde_vshld_u64((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int8x8_t
simde_vshl_s8 (const simde_int8x8_t a, const simde_int8x8_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshl_s8(a, b);
  #else
    simde_int8x8_private
      r_,
      a_ = simde_int8x8_to_private(a),
      b_ = simde_int8x8_to_private(b);

    #if defined(SIMDE_X86_AVX512BW_NATIVE) && defined(SIMDE_X86_AVX512VL_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m128i a128 = _mm_cvtepi8_epi16(_mm_movpi64_epi64(a_.m64));
      __m128i b128 = _mm_cvtepi8_epi16(_mm_movpi64_epi64(b_.m64));
      __m128i r128 = _mm_blendv_epi8(_mm_sllv_epi16(a128, b128),
                                    _mm_srav_epi16(a128, _mm_abs_epi16(b128)),
                                    _mm_cmpgt_epi16(_mm_setzero_si128(), b128));
      r_.m64 = _mm_movepi64_pi64(_mm_cvtepi16_epi8(r128));
    #elif defined(SIMDE_X86_AVX2_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m256i a256 = _mm256_cvtepi8_epi32(_mm_movpi64_epi64(a_.m64));
      __m256i b256 = _mm256_cvtepi8_epi32(_mm_movpi64_epi64(b_.m64));
      __m256i r256 = _mm256_blendv_epi8(_mm256_sllv_epi32(a256, b256),
                                        _mm256_srav_epi32(a256, _mm256_abs_epi32(b256)),
                                        _mm256_cmpgt_epi32(_mm256_setzero_si256(), b256));
      r256 = _mm256_shuffle_epi8(r256, _mm256_set1_epi32(0x0C080400));
      r_.m64 = _mm_set_pi32(_mm256_extract_epi32(r256, 4), _mm256_extract_epi32(r256, 0));
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8m1_t bit_shift_rst = __riscv_vmerge_vxm_i8m1(
         __riscv_vsll_vv_i8m1 (a_.sv64, b_.sv64, 8), 0, __riscv_vmsge_vx_i8m1_b8(b_.sv64, 8, 8), 8);
        vint8m1_t b_abs =  __riscv_vmax_vv_i8m1 (b_.sv64, __riscv_vneg_v_i8m1 (b_.sv64, 8), 8);
        vuint8m1_t u_b_abs = __riscv_vreinterpret_v_i8m1_u8m1 (b_abs);
        vint8m1_t scal_shift_rst = __riscv_vmerge_vvm_i8m1(__riscv_vsra_vv_i8m1 (a_.sv64, u_b_abs, 8), \
          __riscv_vsra_vx_i8m1(a_.sv64, 7, 8), __riscv_vmsle_vx_i8m1_b8(b_.sv64, -8, 8), 8);
        r_.sv64 = __riscv_vmerge_vvm_i8m1 (bit_shift_rst, scal_shift_rst, __riscv_vmslt_vx_i8m1_b8 (b_.sv64, 0, 8), 8);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          r_.values[i] = HEDLEY_STATIC_CAST(int8_t,
            (b_.values[i] >=  0) ?
            (b_.values[i] >=  8) ?                   0 : (a_.values[i] <<  b_.values[i]) :
            (b_.values[i] <= -8) ? (a_.values[i] >> 7) : (a_.values[i] >> -b_.values[i]));
        }
      #endif
    #endif

    return simde_int8x8_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshl_s8
  #define vshl_s8(a, b) simde_vshl_s8((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int16x4_t
simde_vshl_s16 (const simde_int16x4_t a, const simde_int16x4_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshl_s16(a, b);
  #else
    simde_int16x4_private
      r_,
      a_ = simde_int16x4_to_private(a),
      b_ = simde_int16x4_to_private(b);

    #if defined(SIMDE_X86_AVX2_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m128i a128 = _mm_cvtepi16_epi32(_mm_movpi64_epi64(a_.m64));
      __m128i b128 = _mm_cvtepi16_epi32(_mm_movpi64_epi64(b_.m64));
      b128 = _mm_srai_epi32(_mm_slli_epi32(b128, 24), 24);
      __m128i r128 = _mm_blendv_epi8(_mm_sllv_epi32(a128, b128),
                                    _mm_srav_epi32(a128, _mm_abs_epi32(b128)),
                                    _mm_cmpgt_epi32(_mm_setzero_si128(), b128));
      r_.m64 = _mm_movepi64_pi64(_mm_shuffle_epi8(r128, _mm_set1_epi64x(0x0D0C090805040100)));
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf2_t b_8mf2 = __riscv_vncvt_x_x_w_i8mf2 (b_.sv64, 4);
        vint16m1_t bit_shift_rst = __riscv_vmerge_vxm_i16m1(__riscv_vsll_vv_i16m1 (a_.sv64, b_.sv64, 4), 0 \
          , __riscv_vmsge_vx_i8mf2_b16(b_8mf2, 16, 8), 4);
        vint16m1_t b_abs =  __riscv_vmax_vv_i16m1 (b_.sv64, __riscv_vneg_v_i16m1 (b_.sv64, 4), 4);
        vuint16m1_t u_b_abs = __riscv_vreinterpret_v_i16m1_u16m1 (b_abs);
        vint16m1_t scal_shift_rst = __riscv_vmerge_vvm_i16m1(__riscv_vsra_vv_i16m1 (a_.sv64, u_b_abs, 4)
          , __riscv_vsra_vx_i16m1(a_.sv64, 15, 4), __riscv_vmsle_vx_i8mf2_b16(b_8mf2, -16, 8), 4);
        r_.sv64 = __riscv_vmerge_vvm_i16m1 (bit_shift_rst, scal_shift_rst, __riscv_vmslt_vx_i8mf2_b16 (b_8mf2, 0, 8), 4);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          b_.values[i] = HEDLEY_STATIC_CAST(int8_t, b_.values[i]);
          r_.values[i] = HEDLEY_STATIC_CAST(int16_t,
            (b_.values[i] >=   0) ?
            (b_.values[i] >=  16) ?                    0 : (a_.values[i] <<  b_.values[i]) :
            (b_.values[i] <= -16) ? (a_.values[i] >> 15) : (a_.values[i] >> -b_.values[i]));
        }
      #endif
    #endif

    return simde_int16x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshl_s16
  #define vshl_s16(a, b) simde_vshl_s16((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int32x2_t
simde_vshl_s32 (const simde_int32x2_t a, const simde_int32x2_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshl_s32(a, b);
  #else
    simde_int32x2_private
      r_,
      a_ = simde_int32x2_to_private(a),
      b_ = simde_int32x2_to_private(b);

    #if defined(SIMDE_X86_AVX2_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m128i a128 = _mm_movpi64_epi64(a_.m64);
      __m128i b128 = _mm_movpi64_epi64(b_.m64);
      b128 = _mm_srai_epi32(_mm_slli_epi32(b128, 24), 24);
      __m128i r128 = _mm_blendv_epi8(_mm_sllv_epi32(a128, b128),
                                    _mm_srav_epi32(a128, _mm_abs_epi32(b128)),
                                    _mm_cmpgt_epi32(_mm_setzero_si128(), b128));
      r_.m64 = _mm_movepi64_pi64(r128);
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf4_t b_8mf4 = __riscv_vncvt_x_x_w_i8mf4 (__riscv_vncvt_x_x_w_i16mf2 (b_.sv64, 2), 4);
        vint32m1_t bit_shift_rst = __riscv_vmerge_vxm_i32m1(__riscv_vsll_vv_i32m1 (a_.sv64, b_.sv64, 2), 0
          , __riscv_vmsge_vx_i8mf4_b32(b_8mf4, 32, 2), 2);
        vint32m1_t b_abs =  __riscv_vmax_vv_i32m1 (b_.sv64, __riscv_vneg_v_i32m1 (b_.sv64, 2), 2);
        vuint32m1_t u_b_abs = __riscv_vreinterpret_v_i32m1_u32m1 (b_abs);
        vint32m1_t scal_shift_rst = __riscv_vmerge_vvm_i32m1(__riscv_vsra_vv_i32m1 (a_.sv64, u_b_abs, 2)
          , __riscv_vsra_vx_i32m1(a_.sv64, 31, 2), __riscv_vmsle_vx_i8mf4_b32(b_8mf4, -32, 2), 2);
        r_.sv64 = __riscv_vmerge_vvm_i32m1 (bit_shift_rst, scal_shift_rst, __riscv_vmslt_vx_i8mf4_b32 (b_8mf4, 0, 8), 2);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          b_.values[i] = HEDLEY_STATIC_CAST(int8_t, b_.values[i]);
          r_.values[i] =
            (b_.values[i] >=   0) ?
            (b_.values[i] >=  32) ?                    0 : (a_.values[i] <<  b_.values[i]) :
            (b_.values[i] <= -32) ? (a_.values[i] >> 31) : (a_.values[i] >> -b_.values[i]);
        }
      #endif
    #endif

    return simde_int32x2_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshl_s32
  #define vshl_s32(a, b) simde_vshl_s32((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int64x1_t
simde_vshl_s64 (const simde_int64x1_t a, const simde_int64x1_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshl_s64(a, b);
  #else
    simde_int64x1_private
      r_,
      a_ = simde_int64x1_to_private(a),
      b_ = simde_int64x1_to_private(b);

    #if defined(SIMDE_X86_AVX512F_NATIVE) && defined(SIMDE_X86_AVX512VL_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m128i zero = _mm_setzero_si128();
      __m128i a128 = _mm_movpi64_epi64(a_.m64);
      __m128i b128 = _mm_movpi64_epi64(b_.m64);
      b128 = _mm_srai_epi64(_mm_slli_epi64(b128, 56), 56);
      __m128i r128 = _mm_blendv_epi8(_mm_sllv_epi64(a128, b128),
                                    _mm_srav_epi64(a128, _mm_sub_epi64(zero, b128)),
                                    _mm_cmpgt_epi64(zero, b128));
      r_.m64 = _mm_movepi64_pi64(r128);
    #elif defined(SIMDE_X86_AVX2_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m128i zero = _mm_setzero_si128();
      __m128i a128 = _mm_movpi64_epi64(a_.m64);
      __m128i b128 = _mm_movpi64_epi64(b_.m64);
      __m128i maska = _mm_cmpgt_epi64(zero, a128);
      __m128i b_abs = _mm_and_si128(_mm_abs_epi8(b128), _mm_set1_epi64x(0xFF));
      __m128i r128 = _mm_blendv_epi8(_mm_sllv_epi64(a128, b_abs),
                                    _mm_xor_si128(_mm_srlv_epi64(_mm_xor_si128(a128, maska), b_abs), maska),
                                    _mm_cmpgt_epi64(zero, _mm_slli_epi64(b128, 56)));
      r_.m64 = _mm_movepi64_pi64(r128);
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf8_t b_8mf8 = __riscv_vncvt_x_x_w_i8mf8 (__riscv_vncvt_x_x_w_i16mf4 (__riscv_vncvt_x_x_w_i32mf2 (b_.sv64, 1), 2), 4);
        vint64m1_t bit_shift_rst = __riscv_vmerge_vxm_i64m1(__riscv_vsll_vv_i64m1 (a_.sv64, b_.sv64, 1), 0
          , __riscv_vmsge_vx_i8mf8_b64(b_8mf8, 64, 1), 1);
        vint64m1_t b_abs =  __riscv_vmax_vv_i64m1 (b_.sv64, __riscv_vneg_v_i64m1 (b_.sv64, 1), 1);
        vuint64m1_t u_b_abs = __riscv_vreinterpret_v_i64m1_u64m1 (b_abs);
        vint64m1_t scal_shift_rst = __riscv_vmerge_vvm_i64m1(__riscv_vsra_vv_i64m1 (a_.sv64, u_b_abs, 1)
          , __riscv_vsra_vx_i64m1(a_.sv64, 63, 1), __riscv_vmsle_vx_i8mf8_b64(b_8mf8, -64, 1), 1);
        r_.sv64 = __riscv_vmerge_vvm_i64m1 (bit_shift_rst, scal_shift_rst, __riscv_vmslt_vx_i8mf8_b64 (b_8mf8, 0, 8), 1);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          r_.values[i] = simde_vshld_s64(a_.values[i], b_.values[i]);
        }
      #endif
    #endif

    return simde_int64x1_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshl_s64
  #define vshl_s64(a, b) simde_vshl_s64((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint8x8_t
simde_vshl_u8 (const simde_uint8x8_t a, const simde_int8x8_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshl_u8(a, b);
  #else
    simde_uint8x8_private
      r_,
      a_ = simde_uint8x8_to_private(a);
    simde_int8x8_private b_ = simde_int8x8_to_private(b);

    #if defined(SIMDE_X86_AVX512BW_NATIVE) && defined(SIMDE_X86_AVX512VL_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m128i a128 = _mm_cvtepu8_epi16(_mm_movpi64_epi64(a_.m64));
      __m128i b128 = _mm_cvtepi8_epi16(_mm_movpi64_epi64(b_.m64));
      __m128i r128 = _mm_blendv_epi8(_mm_sllv_epi16(a128, b128),
                                    _mm_srlv_epi16(a128, _mm_abs_epi16(b128)),
                                    _mm_cmpgt_epi16(_mm_setzero_si128(), b128));
      r_.m64 = _mm_movepi64_pi64(_mm_cvtepi16_epi8(r128));
    #elif defined(SIMDE_X86_AVX2_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m256i a256 = _mm256_cvtepu8_epi32(_mm_movpi64_epi64(a_.m64));
      __m256i b256 = _mm256_cvtepi8_epi32(_mm_movpi64_epi64(b_.m64));
      __m256i r256 = _mm256_blendv_epi8(_mm256_sllv_epi32(a256, b256),
                                        _mm256_srlv_epi32(a256, _mm256_abs_epi32(b256)),
                                        _mm256_cmpgt_epi32(_mm256_setzero_si256(), b256));
      r256 = _mm256_shuffle_epi8(r256, _mm256_set1_epi32(0x0C080400));
      r_.m64 = _mm_set_pi32(_mm256_extract_epi32(r256, 4), _mm256_extract_epi32(r256, 0));
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vuint8m1_t u_b = __riscv_vreinterpret_v_i8m1_u8m1 (b_.sv64);
        vint8m1_t b_abs =  __riscv_vmax_vv_i8m1 (b_.sv64, __riscv_vneg_v_i8m1 (b_.sv64, 8), 8);
        vuint8m1_t u_b_abs = __riscv_vreinterpret_v_i8m1_u8m1 (b_abs);
        r_.sv64 = __riscv_vmerge_vxm_u8m1(__riscv_vmerge_vvm_u8m1(__riscv_vsrl_vv_u8m1(a_.sv64, u_b_abs, 8)
            , __riscv_vsll_vv_u8m1 (a_.sv64, u_b, 8), __riscv_vmsge_vx_i8m1_b8(b_.sv64, 0, 8), 8), 0 \
            ,__riscv_vmsgeu_vx_u8m1_b8(u_b_abs, 8, 8), 8);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          r_.values[i] = HEDLEY_STATIC_CAST(uint8_t,
            (simde_math_abs(b_.values[i]) >= 8) ? 0 :
                (b_.values[i]  >= 0) ? (a_.values[i] <<  b_.values[i]) :
                                      (a_.values[i] >> -b_.values[i]));
        }
      #endif
    #endif

    return simde_uint8x8_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshl_u8
  #define vshl_u8(a, b) simde_vshl_u8((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint16x4_t
simde_vshl_u16 (const simde_uint16x4_t a, const simde_int16x4_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshl_u16(a, b);
  #else
    simde_uint16x4_private
      r_,
      a_ = simde_uint16x4_to_private(a);
    simde_int16x4_private b_ = simde_int16x4_to_private(b);

    #if defined(SIMDE_X86_AVX2_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m128i a128 = _mm_cvtepu16_epi32(_mm_movpi64_epi64(a_.m64));
      __m128i b128 = _mm_cvtepi16_epi32(_mm_movpi64_epi64(b_.m64));
      b128 = _mm_srai_epi32(_mm_slli_epi32(b128, 24), 24);
      __m128i r128 = _mm_blendv_epi8(_mm_sllv_epi32(a128, b128),
                                    _mm_srlv_epi32(a128, _mm_abs_epi32(b128)),
                                    _mm_cmpgt_epi32(_mm_setzero_si128(), b128));
      r_.m64 = _mm_movepi64_pi64(_mm_shuffle_epi8(r128, _mm_set1_epi64x(0x0D0C090805040100)));
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf2_t b_8mf2 = __riscv_vncvt_x_x_w_i8mf2 (b_.sv64, 4);
        vint8mf2_t b_8mf2_abs = __riscv_vmax_vv_i8mf2 (b_8mf2, __riscv_vneg_v_i8mf2 (b_8mf2, 8), 8);
        vuint8mf2_t u_b_8mf2_abs = __riscv_vreinterpret_v_i8mf2_u8mf2 (b_8mf2_abs);
        vuint16m1_t u_b = __riscv_vreinterpret_v_i16m1_u16m1 (b_.sv64);
        vint16m1_t b_abs =  __riscv_vmax_vv_i16m1 (b_.sv64, __riscv_vneg_v_i16m1 (b_.sv64, 4), 4);
        vuint16m1_t u_b_abs = __riscv_vreinterpret_v_i16m1_u16m1 (b_abs);
        r_.sv64 = __riscv_vmerge_vxm_u16m1(__riscv_vmerge_vvm_u16m1(__riscv_vsrl_vv_u16m1(a_.sv64, u_b_abs, 4)
          , __riscv_vsll_vv_u16m1 (a_.sv64, u_b, 4), __riscv_vmsge_vx_i16m1_b16(b_.sv64, 0, 4), 4)
          , 0, __riscv_vmsgeu_vx_u8mf2_b16(u_b_8mf2_abs, 16, 8), 4);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          b_.values[i] = HEDLEY_STATIC_CAST(int8_t, b_.values[i]);
          r_.values[i] = HEDLEY_STATIC_CAST(uint16_t,
            (simde_math_abs(b_.values[i]) >= 16) ? 0 :
                (b_.values[i]  >=  0) ? (a_.values[i] <<  b_.values[i]) :
                                        (a_.values[i] >> -b_.values[i]));
        }
      #endif
    #endif

    return simde_uint16x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshl_u16
  #define vshl_u16(a, b) simde_vshl_u16((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint32x2_t
simde_vshl_u32 (const simde_uint32x2_t a, const simde_int32x2_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshl_u32(a, b);
  #else
    simde_uint32x2_private
      r_,
      a_ = simde_uint32x2_to_private(a);
    simde_int32x2_private b_ = simde_int32x2_to_private(b);

    #if defined(SIMDE_X86_AVX2_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m128i a128 = _mm_movpi64_epi64(a_.m64);
      __m128i b128 = _mm_movpi64_epi64(b_.m64);
      b128 = _mm_srai_epi32(_mm_slli_epi32(b128, 24), 24);
      __m128i r128 = _mm_blendv_epi8(_mm_sllv_epi32(a128, b128),
                                    _mm_srlv_epi32(a128, _mm_abs_epi32(b128)),
                                    _mm_cmpgt_epi32(_mm_setzero_si128(), b128));
      r_.m64 = _mm_movepi64_pi64(r128);
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf4_t b_8mf4 = __riscv_vncvt_x_x_w_i8mf4 (__riscv_vncvt_x_x_w_i16mf2 (b_.sv64, 2), 4);
        vint8mf4_t b_8mf4_abs = __riscv_vmax_vv_i8mf4 (b_8mf4, __riscv_vneg_v_i8mf4 (b_8mf4, 8), 8);
        vuint8mf4_t u_b_8mf4_abs = __riscv_vreinterpret_v_i8mf4_u8mf4 (b_8mf4_abs);
        vuint32m1_t u_b = __riscv_vreinterpret_v_i32m1_u32m1 (b_.sv64);
        vint32m1_t b_abs =  __riscv_vmax_vv_i32m1 (b_.sv64, __riscv_vneg_v_i32m1 (b_.sv64, 2), 2);
        vuint32m1_t u_b_abs = __riscv_vreinterpret_v_i32m1_u32m1 (b_abs);
        r_.sv64 = __riscv_vmerge_vxm_u32m1(__riscv_vmerge_vvm_u32m1(__riscv_vsrl_vv_u32m1(a_.sv64, u_b_abs, 2)
          , __riscv_vsll_vv_u32m1 (a_.sv64, u_b, 2), __riscv_vmsge_vx_i32m1_b32(b_.sv64, 0, 2), 2), 0
          , __riscv_vmsgeu_vx_u8mf4_b32(u_b_8mf4_abs, 32, 8), 2);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          b_.values[i] = HEDLEY_STATIC_CAST(int8_t, b_.values[i]);
          r_.values[i] =
            (simde_math_abs(b_.values[i]) >= 32) ? 0 :
                (b_.values[i]  >=  0) ? (a_.values[i] <<  b_.values[i]) :
                                        (a_.values[i] >> -b_.values[i]);
        }
      #endif
    #endif

    return simde_uint32x2_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshl_u32
  #define vshl_u32(a, b) simde_vshl_u32((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint64x1_t
simde_vshl_u64 (const simde_uint64x1_t a, const simde_int64x1_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshl_u64(a, b);
  #else
    simde_uint64x1_private
      r_,
      a_ = simde_uint64x1_to_private(a);
    simde_int64x1_private b_ = simde_int64x1_to_private(b);

    #if defined(SIMDE_X86_AVX512F_NATIVE) && defined(SIMDE_X86_AVX512VL_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m128i zero = _mm_setzero_si128();
      __m128i a128 = _mm_movpi64_epi64(a_.m64);
      __m128i b128 = _mm_movpi64_epi64(b_.m64);
      b128 = _mm_srai_epi64(_mm_slli_epi64(b128, 56), 56);
      __m128i r128 = _mm_blendv_epi8(_mm_sllv_epi64(a128, b128),
                                    _mm_srlv_epi64(a128, _mm_sub_epi64(zero, b128)),
                                    _mm_cmpgt_epi64(zero, b128));
      r_.m64 = _mm_movepi64_pi64(r128);
    #elif defined(SIMDE_X86_AVX2_NATIVE) && defined(SIMDE_X86_MMX_NATIVE)
      __m128i a128 = _mm_movpi64_epi64(a_.m64);
      __m128i b128 = _mm_movpi64_epi64(b_.m64);
      __m128i b_abs = _mm_and_si128(_mm_abs_epi8(b128), _mm_set1_epi64x(0xFF));
      __m128i r128 = _mm_blendv_epi8(_mm_sllv_epi64(a128, b_abs),
                                    _mm_srlv_epi64(a128, b_abs),
                                    _mm_cmpgt_epi64(_mm_setzero_si128(), _mm_slli_epi64(b128, 56)));
      r_.m64 = _mm_movepi64_pi64(r128);
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        //  change b_ to int8_t
        vint8mf8_t b_8mf8 = __riscv_vncvt_x_x_w_i8mf8(__riscv_vncvt_x_x_w_i16mf4 \
          (__riscv_vncvt_x_x_w_i32mf2 (b_.sv64, 1), 2), 4);
        vint8mf8_t b_8mf8_abs = __riscv_vmax_vv_i8mf8(b_8mf8, __riscv_vneg_v_i8mf8 (b_8mf8, 8), 8);
        vuint8mf8_t u_b_8mf8_abs = __riscv_vreinterpret_v_i8mf8_u8mf8 (b_8mf8_abs);
        vuint64m1_t u_b = __riscv_vreinterpret_v_i64m1_u64m1 (b_.sv64);
        vint64m1_t b_abs =  __riscv_vmax_vv_i64m1 (b_.sv64, __riscv_vneg_v_i64m1 (b_.sv64, 1), 1);
        vuint64m1_t u_b_abs = __riscv_vreinterpret_v_i64m1_u64m1 (b_abs);
        r_.sv64 = __riscv_vmerge_vxm_u64m1(__riscv_vmerge_vvm_u64m1(__riscv_vsrl_vv_u64m1(a_.sv64, u_b_abs, 1)
            , __riscv_vsll_vv_u64m1 (a_.sv64, u_b, 1), __riscv_vmsge_vx_i64m1_b64(b_.sv64, 0, 1), 1), 0
            , __riscv_vmsgeu_vx_u8mf8_b64(u_b_8mf8_abs, 64, 8), 1);
      #else
      SIMDE_VECTORIZE
      for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
        r_.values[i] = simde_vshld_u64(a_.values[i], b_.values[i]);
      }
      #endif
    #endif

  return simde_uint64x1_from_private(r_);
#endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshl_u64
  #define vshl_u64(a, b) simde_vshl_u64((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int8x16_t
simde_vshlq_s8 (const simde_int8x16_t a, const simde_int8x16_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshlq_s8(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    SIMDE_POWER_ALTIVEC_VECTOR(signed char) a_shl, a_shr;
    SIMDE_POWER_ALTIVEC_VECTOR(unsigned char) b_abs, b_max;
    SIMDE_POWER_ALTIVEC_VECTOR(SIMDE_POWER_ALTIVEC_BOOL char) b_mask;
    b_abs = HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(unsigned char), vec_abs(b));
    b_max = vec_splat_u8(7);
    #if defined(SIMDE_POWER_ALTIVEC_P7_NATIVE)
      a_shl = vec_and(vec_sl(a, b_abs), vec_cmple(b_abs, b_max));
    #else
      a_shl = vec_and(vec_sl(a, b_abs), vec_cmplt(b_abs, vec_splat_u8(8)));
    #endif
    a_shr = vec_sra(a, vec_min(b_abs, b_max));
    b_mask = vec_cmplt(b, vec_splat_s8(0));
    return vec_sel(a_shl, a_shr, b_mask);
  #else
    simde_int8x16_private
      r_,
      a_ = simde_int8x16_to_private(a),
      b_ = simde_int8x16_to_private(b);

    #if defined(SIMDE_X86_AVX512BW_NATIVE) && defined(SIMDE_X86_AVX512VL_NATIVE)
      __m256i a256 = _mm256_cvtepi8_epi16(a_.m128i);
      __m256i b256 = _mm256_cvtepi8_epi16(b_.m128i);
      __m256i r256 = _mm256_blendv_epi8(_mm256_sllv_epi16(a256, b256),
                                        _mm256_srav_epi16(a256, _mm256_abs_epi16(b256)),
                                        _mm256_cmpgt_epi16(_mm256_setzero_si256(), b256));
      r_.m128i = _mm256_cvtepi16_epi8(r256);
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8m1_t bit_shift_rst = __riscv_vmerge_vxm_i8m1(__riscv_vsll_vv_i8m1 (a_.sv128, b_.sv128, 16), \
          0, __riscv_vmsge_vx_i8m1_b8(b_.sv128, 8, 16), 16);
        vint8m1_t b_abs =  __riscv_vmax_vv_i8m1 (b_.sv128, __riscv_vneg_v_i8m1 (b_.sv128, 16), 16);
        vuint8m1_t u_b_abs = __riscv_vreinterpret_v_i8m1_u8m1 (b_abs);
        vint8m1_t scal_shift_rst = __riscv_vmerge_vvm_i8m1(__riscv_vsra_vv_i8m1 (a_.sv128, u_b_abs, 16)
          , __riscv_vsra_vx_i8m1(a_.sv128, 7, 16), __riscv_vmsle_vx_i8m1_b8(b_.sv128, -8, 16), 16);
        r_.sv128 = __riscv_vmerge_vvm_i8m1 (bit_shift_rst, scal_shift_rst, __riscv_vmslt_vx_i8m1_b8 (b_.sv128, 0, 16), 16);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          r_.values[i] = HEDLEY_STATIC_CAST(int8_t,
            (b_.values[i] >=  0) ?
            (b_.values[i] >=  8) ?                   0 : (a_.values[i] <<  b_.values[i]) :
            (b_.values[i] <= -8) ? (a_.values[i] >> 7) : (a_.values[i] >> -b_.values[i]));
        }
      #endif
    #endif

    return simde_int8x16_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshlq_s8
  #define vshlq_s8(a, b) simde_vshlq_s8((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int16x8_t
simde_vshlq_s16 (const simde_int16x8_t a, const simde_int16x8_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshlq_s16(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    SIMDE_POWER_ALTIVEC_VECTOR(signed short) a_shl, a_shr;
    SIMDE_POWER_ALTIVEC_VECTOR(unsigned short) b_abs, b_max;
    SIMDE_POWER_ALTIVEC_VECTOR(SIMDE_POWER_ALTIVEC_BOOL short) b_mask;
    b_abs = vec_and(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(unsigned short),
                                            vec_abs(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(signed char), b))),
                    vec_splats(HEDLEY_STATIC_CAST(unsigned short, 0xFF)));
    b_max = vec_splat_u16(15);
    #if defined(SIMDE_POWER_ALTIVEC_P7_NATIVE)
      a_shl = vec_and(vec_sl(a, b_abs), vec_cmple(b_abs, b_max));
    #else
      a_shl = vec_and(vec_sl(a, b_abs), vec_cmplt(b_abs, vec_splats(HEDLEY_STATIC_CAST(unsigned short, 16))));
    #endif
    a_shr = vec_sra(a, vec_min(b_abs, b_max));
    b_mask = vec_cmplt(vec_sl(b, vec_splat_u16(8)), vec_splat_s16(0));
    return vec_sel(a_shl, a_shr, b_mask);
  #else
    simde_int16x8_private
      r_,
      a_ = simde_int16x8_to_private(a),
      b_ = simde_int16x8_to_private(b);

    #if defined(SIMDE_X86_AVX512BW_NATIVE) && defined(SIMDE_X86_AVX512VL_NATIVE)
      __m128i B = _mm_srai_epi16(_mm_slli_epi16(b_.m128i, 8), 8);
      r_.m128i = _mm_blendv_epi8(_mm_sllv_epi16(a_.m128i, B),
                                 _mm_srav_epi16(a_.m128i, _mm_abs_epi16(B)),
                                 _mm_cmpgt_epi16(_mm_setzero_si128(), B));
    #elif defined(SIMDE_X86_AVX2_NATIVE) && defined(SIMDE_ARCH_AMD64)
      __m256i a256 = _mm256_cvtepi16_epi32(a_.m128i);
      __m256i b256 = _mm256_cvtepi16_epi32(b_.m128i);
      b256 = _mm256_srai_epi32(_mm256_slli_epi32(b256, 24), 24);
      __m256i r256 = _mm256_blendv_epi8(_mm256_sllv_epi32(a256, b256),
                                        _mm256_srav_epi32(a256, _mm256_abs_epi32(b256)),
                                        _mm256_cmpgt_epi32(_mm256_setzero_si256(), b256));
      r256 = _mm256_shuffle_epi8(r256, _mm256_set1_epi64x(0x0D0C090805040100));
      r_.m128i = _mm_set_epi64x(_mm256_extract_epi64(r256, 2), _mm256_extract_epi64(r256, 0));
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf2_t b_8mf2 = __riscv_vncvt_x_x_w_i8mf2 (b_.sv128, 8);
        vint8mf2_t b_8mf2_abs = __riscv_vmax_vv_i8mf2 (b_8mf2, __riscv_vneg_v_i8mf2 (b_8mf2, 16), 16);
        vuint8mf2_t u_b_8mf2_abs = __riscv_vreinterpret_v_i8mf2_u8mf2(b_8mf2_abs);
        vuint16m1_t u_b_abs = __riscv_vwcvtu_x_x_v_u16m1 (u_b_8mf2_abs, 16);
        vint16m1_t bit_shift_rst = __riscv_vmerge_vxm_i16m1(__riscv_vsll_vv_i16m1 (a_.sv128, b_.sv128, 8), 0, \
          __riscv_vmsge_vx_i8mf2_b16(b_8mf2, 16, 16), 8);
        vint16m1_t scal_shift_rst = __riscv_vmerge_vvm_i16m1(__riscv_vsra_vv_i16m1 (a_.sv128, u_b_abs, 8),
          __riscv_vsra_vx_i16m1(a_.sv128, 15, 8), __riscv_vmsle_vx_i8mf2_b16(b_8mf2, -16, 16), 8);
        r_.sv128 = __riscv_vmerge_vvm_i16m1 (bit_shift_rst, scal_shift_rst, __riscv_vmslt_vx_i8mf2_b16 (b_8mf2, 0, 16), 8);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          b_.values[i] = HEDLEY_STATIC_CAST(int8_t, b_.values[i]);
          r_.values[i] = HEDLEY_STATIC_CAST(int16_t,
            (b_.values[i] >=   0) ?
            (b_.values[i] >=  16) ?                    0 : (a_.values[i] <<  b_.values[i]) :
            (b_.values[i] <= -16) ? (a_.values[i] >> 15) : (a_.values[i] >> -b_.values[i]));
        }
      #endif
    #endif

    return simde_int16x8_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshlq_s16
  #define vshlq_s16(a, b) simde_vshlq_s16((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int32x4_t
simde_vshlq_s32 (const simde_int32x4_t a, const simde_int32x4_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshlq_s32(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    SIMDE_POWER_ALTIVEC_VECTOR(signed int) a_shl, a_shr;
    SIMDE_POWER_ALTIVEC_VECTOR(unsigned int) b_abs, b_max;
    SIMDE_POWER_ALTIVEC_VECTOR(SIMDE_POWER_ALTIVEC_BOOL int) b_mask;
    b_abs = vec_and(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(unsigned int),
                                            vec_abs(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(signed char), b))),
                    vec_splats(HEDLEY_STATIC_CAST(unsigned int, 0xFF)));
    b_max = vec_splats(HEDLEY_STATIC_CAST(unsigned int, 31));
    #if defined(SIMDE_POWER_ALTIVEC_P7_NATIVE)
      a_shl = vec_and(vec_sl(a, b_abs), vec_cmple(b_abs, b_max));
    #else
    a_shl = vec_and(vec_sl(a, b_abs), vec_cmplt(b_abs, vec_splats(HEDLEY_STATIC_CAST(unsigned int, 32))));
      #endif
    a_shr = vec_sra(a, vec_min(b_abs, b_max));
    b_mask = vec_cmplt(vec_sl(b, vec_splats(HEDLEY_STATIC_CAST(unsigned int, 24))),
                      vec_splat_s32(0));
    return vec_sel(a_shl, a_shr, b_mask);
  #else
    simde_int32x4_private
      r_,
      a_ = simde_int32x4_to_private(a),
      b_ = simde_int32x4_to_private(b);

    #if defined(SIMDE_X86_AVX2_NATIVE)
      __m128i B = _mm_srai_epi32(_mm_slli_epi32(b_.m128i, 24), 24);
      r_.m128i = _mm_blendv_epi8(_mm_sllv_epi32(a_.m128i, B),
                                 _mm_srav_epi32(a_.m128i, _mm_abs_epi32(B)),
                                 _mm_cmpgt_epi32(_mm_setzero_si128(), B));
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf4_t b_8mf4 = __riscv_vncvt_x_x_w_i8mf4 (__riscv_vncvt_x_x_w_i16mf2 (b_.sv128, 4), 8);
        vint8mf4_t b_8mf4_abs =  __riscv_vmax_vv_i8mf4 (b_8mf4, __riscv_vneg_v_i8mf4 (b_8mf4, 16), 16);
        vuint8mf4_t u_b_8mf4_abs = __riscv_vreinterpret_v_i8mf4_u8mf4 (b_8mf4_abs);
        vuint32m1_t u_b_abs = __riscv_vwcvtu_x_x_v_u32m1 (__riscv_vwcvtu_x_x_v_u16mf2 (u_b_8mf4_abs, 16), 8);
        vint32m1_t bit_shift_rst = __riscv_vmerge_vxm_i32m1(__riscv_vsll_vv_i32m1 (a_.sv128, b_.sv128, 4), 0,
          __riscv_vmsge_vx_i8mf4_b32(b_8mf4, 32, 16), 4);
        vint32m1_t scal_shift_rst = __riscv_vmerge_vvm_i32m1(__riscv_vsra_vv_i32m1 (a_.sv128, u_b_abs, 4), \
          __riscv_vsra_vx_i32m1(a_.sv128, 31, 4), __riscv_vmsle_vx_i8mf4_b32(b_8mf4, -32, 4), 4);
        r_.sv128 = __riscv_vmerge_vvm_i32m1 (bit_shift_rst, scal_shift_rst, __riscv_vmslt_vx_i8mf4_b32 (b_8mf4, 0, 16), 4);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          b_.values[i] = HEDLEY_STATIC_CAST(int8_t, b_.values[i]);
          r_.values[i] =
            (b_.values[i] >=   0) ?
            (b_.values[i] >=  32) ?                    0 : (a_.values[i] <<  b_.values[i]) :
            (b_.values[i] <= -32) ? (a_.values[i] >> 31) : (a_.values[i] >> -b_.values[i]);
        }
      #endif
    #endif

    return simde_int32x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshlq_s32
  #define vshlq_s32(a, b) simde_vshlq_s32((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_int64x2_t
simde_vshlq_s64 (const simde_int64x2_t a, const simde_int64x2_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshlq_s64(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P8_NATIVE)
    SIMDE_POWER_ALTIVEC_VECTOR(signed long long) a_shl, a_shr;
    SIMDE_POWER_ALTIVEC_VECTOR(unsigned long long) b_abs, b_max;
    SIMDE_POWER_ALTIVEC_VECTOR(SIMDE_POWER_ALTIVEC_BOOL long long) b_mask;
    b_abs = vec_and(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(unsigned long long),
                                            vec_abs(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(signed char), b))),
                    vec_splats(HEDLEY_STATIC_CAST(unsigned long long, 0xFF)));
    b_max = vec_splats(HEDLEY_STATIC_CAST(unsigned long long, 63));
    a_shl = vec_and(vec_sl(a, b_abs), vec_cmple(b_abs, b_max));
    a_shr = vec_sra(a, vec_min(b_abs, b_max));
    b_mask = vec_cmplt(vec_sl(b, vec_splats(HEDLEY_STATIC_CAST(unsigned long long, 56))),
                      vec_splats(HEDLEY_STATIC_CAST(signed long long, 0)));
    HEDLEY_DIAGNOSTIC_PUSH
    #if defined(SIMDE_BUG_CLANG_46770)
      SIMDE_DIAGNOSTIC_DISABLE_VECTOR_CONVERSION_
    #endif
    return vec_sel(a_shl, a_shr, b_mask);
    HEDLEY_DIAGNOSTIC_POP
  #else
    simde_int64x2_private
      r_,
      a_ = simde_int64x2_to_private(a),
      b_ = simde_int64x2_to_private(b);

    #if defined(SIMDE_X86_AVX512F_NATIVE) && defined(SIMDE_X86_AVX512VL_NATIVE)
      __m128i zero = _mm_setzero_si128();
      __m128i B = _mm_srai_epi64(_mm_slli_epi64(b_.m128i, 56), 56);
      r_.m128i = _mm_blendv_epi8(_mm_sllv_epi64(a_.m128i, B),
                                 _mm_srav_epi64(a_.m128i, _mm_sub_epi64(zero, B)),
                                 _mm_cmpgt_epi64(zero, B));
    #elif defined(SIMDE_X86_AVX2_NATIVE)
      __m128i zero = _mm_setzero_si128();
      __m128i maska = _mm_cmpgt_epi64(zero, a_.m128i);
      __m128i b_abs = _mm_and_si128(_mm_abs_epi8(b_.m128i), _mm_set1_epi64x(0xFF));
      r_.m128i = _mm_blendv_epi8(_mm_sllv_epi64(a_.m128i, b_abs),
                                 _mm_xor_si128(_mm_srlv_epi64(_mm_xor_si128(a_.m128i, maska), b_abs), maska),
                                 _mm_cmpgt_epi64(zero, _mm_slli_epi64(b_.m128i, 56)));
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf8_t b_8mf8 = __riscv_vncvt_x_x_w_i8mf8 (__riscv_vncvt_x_x_w_i16mf4 (__riscv_vncvt_x_x_w_i32mf2 (b_.sv128, 2), 4), 8);
        vint8mf8_t b_8mf8_abs =  __riscv_vmax_vv_i8mf8 (b_8mf8, __riscv_vneg_v_i8mf8 (b_8mf8, 16), 16);
        vuint8mf8_t u_b_8mf8_abs = __riscv_vreinterpret_v_i8mf8_u8mf8 (b_8mf8_abs);
        vuint64m1_t u_b_abs = __riscv_vwcvtu_x_x_v_u64m1(__riscv_vwcvtu_x_x_v_u32mf2 (__riscv_vwcvtu_x_x_v_u16mf4(u_b_8mf8_abs, 16), 8), 4);
        vint64m1_t bit_shift_rst = __riscv_vmerge_vxm_i64m1(__riscv_vsll_vv_i64m1 (a_.sv128, b_.sv128, 2), 0, __riscv_vmsge_vx_i8mf8_b64(b_8mf8, 64, 2), 2);
        vint64m1_t scal_shift_rst = __riscv_vmerge_vvm_i64m1(__riscv_vsra_vv_i64m1 (a_.sv128, u_b_abs, 2)
          , __riscv_vsra_vx_i64m1(a_.sv128, 63, 2), __riscv_vmsle_vx_i8mf8_b64(b_8mf8, -64, 2), 2);
        r_.sv128 = __riscv_vmerge_vvm_i64m1 (bit_shift_rst, scal_shift_rst, __riscv_vmslt_vx_i8mf8_b64 (b_8mf8, 0, 16), 2);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          r_.values[i] = simde_vshld_s64(a_.values[i], b_.values[i]);
        }
        #endif
    #endif

    return simde_int64x2_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshlq_s64
  #define vshlq_s64(a, b) simde_vshlq_s64((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint8x16_t
simde_vshlq_u8 (const simde_uint8x16_t a, const simde_int8x16_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshlq_u8(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    SIMDE_POWER_ALTIVEC_VECTOR(unsigned char) b_abs;
    SIMDE_POWER_ALTIVEC_VECTOR(SIMDE_POWER_ALTIVEC_BOOL char) b_mask;
    b_abs = HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(unsigned char), vec_abs(b));
    b_mask = vec_cmplt(b, vec_splat_s8(0));
    return vec_and(vec_sel(vec_sl(a, b_abs), vec_sr(a, b_abs), b_mask),
                  vec_cmplt(b_abs, vec_splat_u8(8)));
  #else
    simde_uint8x16_private
      r_,
      a_ = simde_uint8x16_to_private(a);
    simde_int8x16_private b_ = simde_int8x16_to_private(b);

    #if defined(SIMDE_X86_AVX512BW_NATIVE) && defined(SIMDE_X86_AVX512VL_NATIVE)
      __m256i a256 = _mm256_cvtepu8_epi16(a_.m128i);
      __m256i b256 = _mm256_cvtepi8_epi16(b_.m128i);
      __m256i r256 = _mm256_blendv_epi8(_mm256_sllv_epi16(a256, b256),
                                        _mm256_srlv_epi16(a256, _mm256_abs_epi16(b256)),
                                        _mm256_cmpgt_epi16(_mm256_setzero_si256(), b256));
      r_.m128i = _mm256_cvtepi16_epi8(r256);
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vuint8m1_t u_b = __riscv_vreinterpret_v_i8m1_u8m1 (b_.sv128);
        vint8m1_t b_abs =  __riscv_vmax_vv_i8m1 (b_.sv128, __riscv_vneg_v_i8m1 (b_.sv128, 16), 16);
        vuint8m1_t u_b_abs = __riscv_vreinterpret_v_i8m1_u8m1 (b_abs);
        r_.sv128 = __riscv_vmerge_vxm_u8m1(__riscv_vmerge_vvm_u8m1(__riscv_vsrl_vv_u8m1(a_.sv128, u_b_abs, 16)
          , __riscv_vsll_vv_u8m1 (a_.sv128, u_b, 16), __riscv_vmsge_vx_i8m1_b8(b_.sv128, 0, 16), 16), 0
          , __riscv_vmsgeu_vx_u8m1_b8(u_b_abs, 8, 16), 16);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          r_.values[i] = HEDLEY_STATIC_CAST(uint8_t,
            (simde_math_abs(b_.values[i]) >= 8) ? 0 :
                (b_.values[i]  >= 0) ? (a_.values[i] <<  b_.values[i]) :
                                      (a_.values[i] >> -b_.values[i]));
        }
      #endif
    #endif

    return simde_uint8x16_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshlq_u8
  #define vshlq_u8(a, b) simde_vshlq_u8((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint16x8_t
simde_vshlq_u16 (const simde_uint16x8_t a, const simde_int16x8_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshlq_u16(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    SIMDE_POWER_ALTIVEC_VECTOR(unsigned short) b_abs;
    SIMDE_POWER_ALTIVEC_VECTOR(SIMDE_POWER_ALTIVEC_BOOL short) b_mask;
    b_abs = vec_and(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(unsigned short),
                                            vec_abs(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(signed char), b))),
                    vec_splats(HEDLEY_STATIC_CAST(unsigned short, 0xFF)));
    b_mask = vec_cmplt(vec_sl(b, vec_splat_u16(8)), vec_splat_s16(0));
    #if defined(SIMDE_POWER_ALTIVEC_P7_NATIVE)
      return vec_and(vec_sel(vec_sl(a, b_abs), vec_sr(a, b_abs), b_mask),
                    vec_cmple(b_abs, vec_splat_u16(15)));
    #else
      return vec_and(vec_sel(vec_sl(a, b_abs), vec_sr(a, b_abs), b_mask),
                    vec_cmplt(b_abs, vec_splats(HEDLEY_STATIC_CAST(unsigned short, 16))));
    #endif
  #else
    simde_uint16x8_private
      r_,
      a_ = simde_uint16x8_to_private(a);
    simde_int16x8_private b_ = simde_int16x8_to_private(b);

    #if defined(SIMDE_X86_AVX512BW_NATIVE) && defined(SIMDE_X86_AVX512VL_NATIVE)
      __m128i B = _mm_srai_epi16(_mm_slli_epi16(b_.m128i, 8), 8);
      r_.m128i = _mm_blendv_epi8(_mm_sllv_epi16(a_.m128i, B),
                                 _mm_srlv_epi16(a_.m128i, _mm_abs_epi16(B)),
                                 _mm_cmpgt_epi16(_mm_setzero_si128(), B));
    #elif defined(SIMDE_X86_AVX2_NATIVE) && defined(SIMDE_ARCH_AMD64)
      __m256i a256 = _mm256_cvtepu16_epi32(a_.m128i);
      __m256i b256 = _mm256_cvtepi16_epi32(b_.m128i);
      b256 = _mm256_srai_epi32(_mm256_slli_epi32(b256, 24), 24);
      __m256i r256 = _mm256_blendv_epi8(_mm256_sllv_epi32(a256, b256),
                                        _mm256_srlv_epi32(a256, _mm256_abs_epi32(b256)),
                                        _mm256_cmpgt_epi32(_mm256_setzero_si256(), b256));
      r256 = _mm256_shuffle_epi8(r256, _mm256_set1_epi64x(0x0D0C090805040100));
      r_.m128i = _mm_set_epi64x(_mm256_extract_epi64(r256, 2), _mm256_extract_epi64(r256, 0));
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf2_t b_8mf2 = __riscv_vncvt_x_x_w_i8mf2 (b_.sv128, 8);
        vint8mf2_t b_8mf2_abs = __riscv_vmax_vv_i8mf2 (b_8mf2, __riscv_vneg_v_i8mf2 (b_8mf2, 16), 16);
        vuint8mf2_t u_b_8mf2_abs = __riscv_vreinterpret_v_i8mf2_u8mf2 (b_8mf2_abs);
        vuint16m1_t u_b = __riscv_vreinterpret_v_i16m1_u16m1 (b_.sv128);
        vuint16m1_t u_b_abs = __riscv_vwcvtu_x_x_v_u16m1 (u_b_8mf2_abs, 16);
        r_.sv128 = __riscv_vmerge_vxm_u16m1(__riscv_vmerge_vvm_u16m1(__riscv_vsrl_vv_u16m1(a_.sv128, u_b_abs, 8),
          __riscv_vsll_vv_u16m1 (a_.sv128, u_b, 8), __riscv_vmsge_vx_i8mf2_b16(b_8mf2, 0, 8), 8),
          0, __riscv_vmsgeu_vx_u8mf2_b16(u_b_8mf2_abs, 16, 16), 8);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          b_.values[i] = HEDLEY_STATIC_CAST(int8_t, b_.values[i]);
          r_.values[i] = HEDLEY_STATIC_CAST(uint16_t,
            (simde_math_abs(b_.values[i]) >= 16) ? 0 :
                (b_.values[i]  >=  0) ? (a_.values[i] <<  b_.values[i]) :
                                        (a_.values[i] >> -b_.values[i]));
        }
      #endif
    #endif

    return simde_uint16x8_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshlq_u16
  #define vshlq_u16(a, b) simde_vshlq_u16((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint32x4_t
simde_vshlq_u32 (const simde_uint32x4_t a, const simde_int32x4_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshlq_u32(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P6_NATIVE)
    SIMDE_POWER_ALTIVEC_VECTOR(unsigned int) b_abs;
    SIMDE_POWER_ALTIVEC_VECTOR(SIMDE_POWER_ALTIVEC_BOOL int) b_mask;
    b_abs = vec_and(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(unsigned int),
                                            vec_abs(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(signed char), b))),
                    vec_splats(HEDLEY_STATIC_CAST(unsigned int, 0xFF)));
    b_mask = vec_cmplt(vec_sl(b, vec_splats(HEDLEY_STATIC_CAST(unsigned int, 24))), vec_splat_s32(0));
    return vec_and(vec_sel(vec_sl(a, b_abs), vec_sr(a, b_abs), b_mask),
                  vec_cmplt(b_abs, vec_splats(HEDLEY_STATIC_CAST(unsigned int, 32))));
  #else
    simde_uint32x4_private
      r_,
      a_ = simde_uint32x4_to_private(a);
    simde_int32x4_private b_ = simde_int32x4_to_private(b);

    #if defined(SIMDE_X86_AVX2_NATIVE)
      __m128i B = _mm_srai_epi32(_mm_slli_epi32(b_.m128i, 24), 24);
      r_.m128i = _mm_blendv_epi8(_mm_sllv_epi32(a_.m128i, B),
                                 _mm_srlv_epi32(a_.m128i, _mm_abs_epi32(B)),
                                 _mm_cmpgt_epi32(_mm_setzero_si128(), B));
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf4_t b_8mf4 = __riscv_vncvt_x_x_w_i8mf4 (
          __riscv_vncvt_x_x_w_i16mf2 (b_.sv128, 4), 8);
        vint8mf4_t b_8mf4_abs = __riscv_vmax_vv_i8mf4 (b_8mf4, __riscv_vneg_v_i8mf4 (b_8mf4, 16), 16);
        vuint8mf4_t u_b_8mf4_abs = __riscv_vreinterpret_v_i8mf4_u8mf4 (b_8mf4_abs);
        vuint32m1_t u_b = __riscv_vreinterpret_v_i32m1_u32m1 (b_.sv128);
        vint32m1_t b_abs =  __riscv_vmax_vv_i32m1 (b_.sv128, __riscv_vneg_v_i32m1 (b_.sv128, 4), 4);
        vuint32m1_t u_b_abs = __riscv_vreinterpret_v_i32m1_u32m1 (b_abs);
        r_.sv128 = __riscv_vmerge_vxm_u32m1(__riscv_vmerge_vvm_u32m1(__riscv_vsrl_vv_u32m1(a_.sv128, u_b_abs, 4)
            , __riscv_vsll_vv_u32m1 (a_.sv128, u_b, 4), __riscv_vmsge_vx_i8mf4_b32(b_8mf4, 0, 4), 4), 0
         , __riscv_vmsgeu_vx_u8mf4_b32(u_b_8mf4_abs, 32, 16), 4);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          b_.values[i] = HEDLEY_STATIC_CAST(int8_t, b_.values[i]);
          r_.values[i] = (simde_math_abs(b_.values[i]) >= 32) ? 0 :
                            (b_.values[i]  >=  0) ? (a_.values[i] <<  b_.values[i]) :
                                                    (a_.values[i] >> -b_.values[i]);
        }
      #endif
    #endif

    return simde_uint32x4_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshlq_u32
  #define vshlq_u32(a, b) simde_vshlq_u32((a), (b))
#endif

SIMDE_FUNCTION_ATTRIBUTES
simde_uint64x2_t
simde_vshlq_u64 (const simde_uint64x2_t a, const simde_int64x2_t b) {
  #if defined(SIMDE_ARM_NEON_A32V7_NATIVE)
    return vshlq_u64(a, b);
  #elif defined(SIMDE_POWER_ALTIVEC_P8_NATIVE)
    SIMDE_POWER_ALTIVEC_VECTOR(unsigned long long) b_abs;
    SIMDE_POWER_ALTIVEC_VECTOR(SIMDE_POWER_ALTIVEC_BOOL long long) b_mask;
    b_abs = vec_and(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(unsigned long long),
                                            vec_abs(HEDLEY_REINTERPRET_CAST(SIMDE_POWER_ALTIVEC_VECTOR(signed char), b))),
                    vec_splats(HEDLEY_STATIC_CAST(unsigned long long, 0xFF)));
    b_mask = vec_cmplt(vec_sl(b, vec_splats(HEDLEY_STATIC_CAST(unsigned long long, 56))),
                      vec_splats(HEDLEY_STATIC_CAST(signed long long, 0)));
    HEDLEY_DIAGNOSTIC_PUSH
    #if defined(SIMDE_BUG_CLANG_46770)
      SIMDE_DIAGNOSTIC_DISABLE_VECTOR_CONVERSION_
    #endif
    return vec_and(vec_sel(vec_sl(a, b_abs), vec_sr(a, b_abs), b_mask),
                  vec_cmplt(b_abs, vec_splats(HEDLEY_STATIC_CAST(unsigned long long, 64))));
    HEDLEY_DIAGNOSTIC_POP
  #else
    simde_uint64x2_private
      r_,
      a_ = simde_uint64x2_to_private(a);
    simde_int64x2_private b_ = simde_int64x2_to_private(b);

    #if defined(SIMDE_X86_AVX512F_NATIVE) && defined(SIMDE_X86_AVX512VL_NATIVE)
      __m128i zero = _mm_setzero_si128();
      __m128i B = _mm_srai_epi64(_mm_slli_epi64(b_.m128i, 56), 56);
      r_.m128i = _mm_blendv_epi8(_mm_sllv_epi64(a_.m128i, B),
                                 _mm_srlv_epi64(a_.m128i, _mm_sub_epi64(zero, B)),
                                 _mm_cmpgt_epi64(zero, B));
    #elif defined(SIMDE_X86_AVX2_NATIVE)
      __m128i b_abs = _mm_and_si128(_mm_abs_epi8(b_.m128i), _mm_set1_epi64x(0xFF));
      r_.m128i = _mm_blendv_epi8(_mm_sllv_epi64(a_.m128i, b_abs),
                                 _mm_srlv_epi64(a_.m128i, b_abs),
                                 _mm_cmpgt_epi64(_mm_setzero_si128(), _mm_slli_epi64(b_.m128i, 56)));
    #else
      #if defined(SIMDE_RISCV_V_NATIVE)
        vint8mf8_t b_8mf8 = __riscv_vncvt_x_x_w_i8mf8 (
          __riscv_vncvt_x_x_w_i16mf4 (
          __riscv_vncvt_x_x_w_i32mf2 (b_.sv128, 2), 4), 8);
        vint8mf8_t b_8mf8_abs = __riscv_vmax_vv_i8mf8 (b_8mf8, __riscv_vneg_v_i8mf8 (b_8mf8, 16), 16);
        vuint8mf8_t u_b_8mf8_abs = __riscv_vreinterpret_v_i8mf8_u8mf8 (b_8mf8_abs);
        vuint64m1_t u_b = __riscv_vreinterpret_v_i64m1_u64m1 (b_.sv128);
        vint64m1_t b_abs =  __riscv_vmax_vv_i64m1 (b_.sv128, __riscv_vneg_v_i64m1 (b_.sv128, 2), 2);
        vuint64m1_t u_b_abs = __riscv_vreinterpret_v_i64m1_u64m1 (b_abs);
        r_.sv128 = __riscv_vmerge_vxm_u64m1(__riscv_vmerge_vvm_u64m1(__riscv_vsrl_vv_u64m1(a_.sv128, u_b_abs, 2)
            , __riscv_vsll_vv_u64m1 (a_.sv128, u_b, 2), __riscv_vmsge_vx_i8mf8_b64(b_8mf8, 0, 2), 2), 0
         , __riscv_vmsgeu_vx_u8mf8_b64(u_b_8mf8_abs, 64, 16), 2);
      #else
        SIMDE_VECTORIZE
        for (size_t i = 0 ; i < (sizeof(r_.values) / sizeof(r_.values[0])) ; i++) {
          r_.values[i] = simde_vshld_u64(a_.values[i], b_.values[i]);
        }
      #endif
    #endif

    return simde_uint64x2_from_private(r_);
  #endif
}
#if defined(SIMDE_ARM_NEON_A32V7_ENABLE_NATIVE_ALIASES)
  #undef vshlq_u64
  #define vshlq_u64(a, b) simde_vshlq_u64((a), (b))
#endif

SIMDE_END_DECLS_
HEDLEY_DIAGNOSTIC_POP

#endif /* !defined(SIMDE_ARM_NEON_SHL_H) */
