/*=============================================================================
   Copyright (c) 2014-2017 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_Q_SYNTH_BASE_HPP_NOVEMBER_4_2017)
#define CYCFI_Q_SYNTH_BASE_HPP_NOVEMBER_4_2017

#include <q/fixed_point.hpp>

namespace cycfi { namespace q
{
   ////////////////////////////////////////////////////////////////////////////
   // The synthesizers use fixed point 1.31 format computations where all
   // the bits are fractional and represents phase values that runs from
   // 0 to uint32_max (0 to 2pi).
   ////////////////////////////////////////////////////////////////////////////
   using phase_t = fixed_point<uint32_t, 32>;

   ////////////////////////////////////////////////////////////////////////////
   // osc_freq: given frequency (freq) and samples per second (sps),
   // calculate the fixed point frequency that the phase accumulator
   // (see below) requires.
   ////////////////////////////////////////////////////////////////////////////
   constexpr phase_t osc_freq(double freq, uint32_t sps)
   {
      return phase_t{freq / sps};
   }

   ////////////////////////////////////////////////////////////////////////////
   // osc_period: given period and samples per second (sps),
   // calculate the fixed point frequency that the phase accumulator
   // (see below) requires.
   ////////////////////////////////////////////////////////////////////////////
   constexpr phase_t osc_period(double period, uint32_t sps)
   {
      return phase_t{1.0} / (sps * period);
   }

   ////////////////////////////////////////////////////////////////////////////
   // osc_period: given period in terms of number of samples,
   // calculate the fixed point frequency that the phase accumulator
   // (see below) requires. Argument samples can be fractional.
   ////////////////////////////////////////////////////////////////////////////
   constexpr phase_t osc_period(double samples)
   {
      return phase_t{1.0} / samples;
   }

   ////////////////////////////////////////////////////////////////////////////
   // osc_phase: given phase (in radians), calculate the fixed point phase
   // that the phase accumulator (see below) requires. phase uses fixed
   // point 0.32 format and runs from 0 to uint32_max (0 to 2pi).
   ////////////////////////////////////////////////////////////////////////////
   constexpr phase_t osc_phase(double phase)
   {
      return phase_t{1.0} * (phase / _2pi);
   }

   ////////////////////////////////////////////////////////////////////////////
   // synth_base
   ////////////////////////////////////////////////////////////////////////////
   template <typename Freq, typename Shift>
   struct synth_base
   {
      synth_base(Freq freq_, Shift shift_)
       : freq(freq_)
       , shift(shift_)
      {}

      phase_t           next();
      phase_t           get() const;
      bool              is_start() const;

      Freq              freq;
      Shift             shift;
      phase_t           phase;
   };

   ////////////////////////////////////////////////////////////////////////////
   // Implementation
   ////////////////////////////////////////////////////////////////////////////
   template <typename Freq, typename Shift>
   inline phase_t synth_base<Freq, Shift>::next()
   {
      auto prev_phase = phase;
      phase += freq();
      return shift() + prev_phase;
   }

   template <typename Freq, typename Shift>
   inline phase_t synth_base<Freq, Shift>::get() const
   {
      return shift() + phase;
   }

   template <typename Freq, typename Shift>
   inline bool synth_base<Freq, Shift>::is_start() const
   {
      return phase < freq();
   }
}}

#endif