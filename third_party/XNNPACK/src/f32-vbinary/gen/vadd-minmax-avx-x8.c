// Auto-generated file. Do not edit!
//   Template: src/f32-vbinary/vop-avx.c.in
//   Generator: tools/xngen
//
// Copyright 2019 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <immintrin.h>

#include <xnnpack/common.h>
#include <xnnpack/vbinary.h>


void xnn_f32_vadd_minmax_ukernel__avx_x8(
    size_t n,
    const float* a,
    const float* b,
    float* y,
    const union xnn_f32_minmax_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(n != 0);
  assert(n % sizeof(float) == 0);
  assert(a != NULL);
  assert(b != NULL);
  assert(y != NULL);

  const __m256 vy_min = _mm256_load_ps(params->avx.min);
  const __m256 vy_max = _mm256_load_ps(params->avx.max);

  for (; n >= 8 * sizeof(float); n -= 8 * sizeof(float)) {
    const __m256 va01234567 = _mm256_loadu_ps(a);
    a += 8;

    const __m256 vb01234567 = _mm256_loadu_ps(b);
    b += 8;

    __m256 vy01234567 = _mm256_add_ps(va01234567, vb01234567);


    vy01234567 = _mm256_max_ps(vy01234567, vy_min);

    vy01234567 = _mm256_min_ps(vy01234567, vy_max);

    _mm256_storeu_ps(y, vy01234567);
    y += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    assert(n >= 1 * sizeof(float));
    assert(n <= 7 * sizeof(float));
    const __m256i vmask = _mm256_loadu_si256((const __m256i*) ((uintptr_t) &params->avx.mask_table[7] - n));

    const __m256 va = _mm256_maskload_ps(a, vmask);
    const __m256 vb = _mm256_maskload_ps(b, vmask);

    __m256 vy = _mm256_add_ps(va, vb);
    vy = _mm256_max_ps(vy, vy_min);
    vy = _mm256_min_ps(vy, vy_max);

    __m128 vy_lo = _mm256_castps256_ps128(vy);
    if (n & (4 * sizeof(float))) {
      _mm_storeu_ps(y, vy_lo);
      vy_lo = _mm256_extractf128_ps(vy, 1);
      y += 4;
    }
    if (n & (2 * sizeof(float))) {
      _mm_storel_pi((__m64*) y, vy_lo);
      vy_lo = _mm_movehl_ps(vy_lo, vy_lo);
      y += 2;
    }
    if (n & (1 * sizeof(float))) {
      _mm_store_ss(y, vy_lo);
    }
  }
}
