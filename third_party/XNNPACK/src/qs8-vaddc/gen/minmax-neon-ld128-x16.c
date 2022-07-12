// Auto-generated file. Do not edit!
//   Template: src/qs8-vaddc/neon.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/vaddsub.h>


void xnn_qs8_vaddc_minmax_ukernel__neon_ld128_x16(
    size_t n,
    const int8_t* input_a,
    const int8_t* input_b,
    int8_t* output,
    const union xnn_qs8_addsub_minmax_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_OOB_READS
{
  #if XNN_ARCH_ARM64
    const int8x16_t va_zero_point = vld1q_dup_s8(&params->neon.a_zero_point);
  #else
    const int8x8_t va_zero_point = vld1_dup_s8(&params->neon.a_zero_point);
  #endif
  const int32x4_t va_multiplier = vld1q_dup_s32(&params->neon.a_multiplier);
  const int32x4_t vright_shift = vld1q_dup_s32(&params->neon.right_shift);
  const int16x8_t voutput_zero_point = vld1q_dup_s16(&params->neon.output_zero_point);
  const int8x16_t voutput_min = vld1q_dup_s8(&params->neon.output_min);
  const int8x16_t voutput_max = vld1q_dup_s8(&params->neon.output_max);

  const int32_t vxb = (int32_t) *input_b - (int32_t) params->neon.b_zero_point;
  const int32_t vb = params->neon.b_multiplier;
  const int32x4_t vbias = vdupq_n_s32(vxb * vb);

  for (; n >= 16 * sizeof(int8_t); n -= 16 * sizeof(int8_t)) {
    const int8x16_t va0123456789ABCDEF = vld1q_s8(input_a); input_a += 16;

    #if XNN_ARCH_ARM64
      const int16x8_t vxa01234567 = vsubl_s8(vget_low_s8(va0123456789ABCDEF), vget_low_s8(va_zero_point));
      const int16x8_t vxa89ABCDEF = vsubl_high_s8(va0123456789ABCDEF, va_zero_point);
    #else  // !XNN_ARCH_ARM64
      const int16x8_t vxa01234567 = vsubl_s8(vget_low_s8(va0123456789ABCDEF), va_zero_point);
      const int16x8_t vxa89ABCDEF = vsubl_s8(vget_high_s8(va0123456789ABCDEF), va_zero_point);
    #endif  // XNN_ARCH_ARM64

    int32x4_t vacc0123 = vmlaq_s32(vbias, vmovl_s16(vget_low_s16(vxa01234567)), va_multiplier);
    int32x4_t vacc4567 = vmlaq_s32(vbias, vmovl_s16(vget_high_s16(vxa01234567)), va_multiplier);
    int32x4_t vacc89AB = vmlaq_s32(vbias, vmovl_s16(vget_low_s16(vxa89ABCDEF)), va_multiplier);
    int32x4_t vaccCDEF = vmlaq_s32(vbias, vmovl_s16(vget_high_s16(vxa89ABCDEF)), va_multiplier);

    vacc0123 = vrshlq_s32(vacc0123, vright_shift);
    vacc4567 = vrshlq_s32(vacc4567, vright_shift);
    vacc89AB = vrshlq_s32(vacc89AB, vright_shift);
    vaccCDEF = vrshlq_s32(vaccCDEF, vright_shift);

    const int16x8_t vacc01234567 = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc0123), vqmovn_s32(vacc4567)), voutput_zero_point);
    const int16x8_t vacc89ABCDEF = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc89AB), vqmovn_s32(vaccCDEF)), voutput_zero_point);

    int8x16_t vout0123456789ABCDEF = vcombine_s8(vqmovn_s16(vacc01234567), vqmovn_s16(vacc89ABCDEF));

    vout0123456789ABCDEF = vmaxq_s8(vout0123456789ABCDEF, voutput_min);

    vout0123456789ABCDEF = vminq_s8(vout0123456789ABCDEF, voutput_max);

    vst1q_s8(output, vout0123456789ABCDEF); output += 16;
  }
  if XNN_UNLIKELY(n != 0) {
    do {
      const int8x8_t va01234567 = vld1_s8(input_a); input_a += 8;

      #if XNN_ARCH_ARM64
        const int16x8_t vxa01234567 = vsubl_s8(va01234567, vget_low_s8(va_zero_point));
      #else  // !XNN_ARCH_ARM64
        const int16x8_t vxa01234567 = vsubl_s8(va01234567, va_zero_point);
      #endif

      int32x4_t vacc0123 = vmlaq_s32(vbias, vmovl_s16(vget_low_s16(vxa01234567)), va_multiplier);
      int32x4_t vacc4567 = vmlaq_s32(vbias, vmovl_s16(vget_high_s16(vxa01234567)), va_multiplier);

      vacc0123 = vrshlq_s32(vacc0123, vright_shift);
      vacc4567 = vrshlq_s32(vacc4567, vright_shift);

      const int16x8_t vacc01234567 = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc0123), vqmovn_s32(vacc4567)), voutput_zero_point);

      int8x8_t vout01234567 = vqmovn_s16(vacc01234567);
      vout01234567 = vmax_s8(vout01234567, vget_low_s8(voutput_min));
      vout01234567 = vmin_s8(vout01234567, vget_low_s8(voutput_max));

      if XNN_LIKELY(n >= (8 * sizeof(int8_t))) {
        vst1_s8(output, vout01234567); output += 8;
        n -= 8 * sizeof(int8_t);
      } else {
        if (n & (4 * sizeof(int8_t))) {
          vst1_lane_u32((void*) output, vreinterpret_u32_s8(vout01234567), 0); output += 4;
          vout01234567 = vext_s8(vout01234567, vout01234567, 4);
        }
        if (n & (2 * sizeof(int8_t))) {
          vst1_lane_u16((void*) output, vreinterpret_u16_s8(vout01234567), 0); output += 2;
          vout01234567 = vext_s8(vout01234567, vout01234567, 2);
        }
        if (n & (1 * sizeof(int8_t))) {
          vst1_lane_s8(output, vout01234567, 0);
        }
        n = 0;
      }
    } while (n != 0);
  }
}