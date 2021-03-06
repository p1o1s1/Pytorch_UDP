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


void xnn_f32_vmin_ukernel__avx_x16(
    size_t n,
    const float* a,
    const float* b,
    float* y,
    const union xnn_f32_default_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(n != 0);
  assert(n % sizeof(float) == 0);
  assert(a != NULL);
  assert(b != NULL);
  assert(y != NULL);


  for (; n >= 16 * sizeof(float); n -= 16 * sizeof(float)) {
    const __m256 va01234567 = _mm256_loadu_ps(a);
    const __m256 va89ABCDEF = _mm256_loadu_ps(a + 8);
    a += 16;

    const __m256 vb01234567 = _mm256_loadu_ps(b);
    const __m256 vb89ABCDEF = _mm256_loadu_ps(b + 8);
    b += 16;

    __m256 vy01234567 = _mm256_min_ps(va01234567, vb01234567);
    __m256 vy89ABCDEF = _mm256_min_ps(va89ABCDEF, vb89ABCDEF);



    _mm256_storeu_ps(y, vy01234567);
    _mm256_storeu_ps(y + 8, vy89ABCDEF);
    y += 16;
  }
  for (; n >= 8 * sizeof(float); n -= 8 * sizeof(float)) {
    const __m256 va = _mm256_loadu_ps(a);
    a += 8;

    const __m256 vb = _mm256_loadu_ps(b);
    b += 8;

    __m256 vy = _mm256_min_ps(va, vb);
    _mm256_storeu_ps(y, vy);
    y += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    assert(n >= 1 * sizeof(float));
    assert(n <= 7 * sizeof(float));
    const __m256i vmask = _mm256_loadu_si256((const __m256i*) ((uintptr_t) &params->avx.mask_table[7] - n));

    const __m256 va = _mm256_maskload_ps(a, vmask);
    const __m256 vb = _mm256_maskload_ps(b, vmask);

    __m256 vy = _mm256_min_ps(va, vb);

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
