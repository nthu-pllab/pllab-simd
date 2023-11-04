#define SIMDE_TEST_ARM_NEON_INSN ld4

#include "test-neon.h"
#include "../../../simde/arm/neon/ld4.h"

#if !defined(SIMDE_BUG_INTEL_857088)


static int
test_simde_vld4_f16 (SIMDE_MUNIT_TEST_ARGS) {
#if 1
  struct {
    simde_float16_t a[16];
    simde_float16_t r[4][4];
  } test_vec[] = {
     { {  SIMDE_FLOAT16_VALUE(-42.44), SIMDE_FLOAT16_VALUE(16.36), SIMDE_FLOAT16_VALUE(2.71), SIMDE_FLOAT16_VALUE(-2.23), SIMDE_FLOAT16_VALUE(6.68), SIMDE_FLOAT16_VALUE(13.18), SIMDE_FLOAT16_VALUE(-5.39), SIMDE_FLOAT16_VALUE(14.99),
        SIMDE_FLOAT16_VALUE(29.61), SIMDE_FLOAT16_VALUE(5.95), SIMDE_FLOAT16_VALUE(-9.74), SIMDE_FLOAT16_VALUE(-47.17), SIMDE_FLOAT16_VALUE(26.25), SIMDE_FLOAT16_VALUE(48.73), SIMDE_FLOAT16_VALUE(31.37), SIMDE_FLOAT16_VALUE(-28.10) },
     { {  SIMDE_FLOAT16_VALUE(-42.44), SIMDE_FLOAT16_VALUE(6.68), SIMDE_FLOAT16_VALUE(29.61), SIMDE_FLOAT16_VALUE(26.25) },
       {  SIMDE_FLOAT16_VALUE(16.36), SIMDE_FLOAT16_VALUE(13.18), SIMDE_FLOAT16_VALUE(5.95), SIMDE_FLOAT16_VALUE(48.73) },
       {  SIMDE_FLOAT16_VALUE(2.71), SIMDE_FLOAT16_VALUE(-5.39), SIMDE_FLOAT16_VALUE(-9.74), SIMDE_FLOAT16_VALUE(31.37) },
       {  SIMDE_FLOAT16_VALUE(-2.23), SIMDE_FLOAT16_VALUE(14.99), SIMDE_FLOAT16_VALUE(-47.17), SIMDE_FLOAT16_VALUE(-28.10) } } },
     { {  SIMDE_FLOAT16_VALUE(46.80), SIMDE_FLOAT16_VALUE(-2.58), SIMDE_FLOAT16_VALUE(-4.79), SIMDE_FLOAT16_VALUE(-40.09), SIMDE_FLOAT16_VALUE(-41.00), SIMDE_FLOAT16_VALUE(29.02), SIMDE_FLOAT16_VALUE(-2.54), SIMDE_FLOAT16_VALUE(34.40),
        SIMDE_FLOAT16_VALUE(33.54), SIMDE_FLOAT16_VALUE(-47.97), SIMDE_FLOAT16_VALUE(0.26), SIMDE_FLOAT16_VALUE(-5.30), SIMDE_FLOAT16_VALUE(-28.10), SIMDE_FLOAT16_VALUE(-26.53), SIMDE_FLOAT16_VALUE(-36.91), SIMDE_FLOAT16_VALUE(-35.35) },
     { {  SIMDE_FLOAT16_VALUE(46.80), SIMDE_FLOAT16_VALUE(-41.00), SIMDE_FLOAT16_VALUE(33.54), SIMDE_FLOAT16_VALUE(-28.10) },
       {  SIMDE_FLOAT16_VALUE(-2.58), SIMDE_FLOAT16_VALUE(29.02), SIMDE_FLOAT16_VALUE(-47.97), SIMDE_FLOAT16_VALUE(-26.53) },
       {  SIMDE_FLOAT16_VALUE(-4.79), SIMDE_FLOAT16_VALUE(-2.54), SIMDE_FLOAT16_VALUE(0.26), SIMDE_FLOAT16_VALUE(-36.91) },
       {  SIMDE_FLOAT16_VALUE(-40.09), SIMDE_FLOAT16_VALUE(34.40), SIMDE_FLOAT16_VALUE(-5.30), SIMDE_FLOAT16_VALUE(-35.35) } } },
     { {  SIMDE_FLOAT16_VALUE(-19.68), SIMDE_FLOAT16_VALUE(38.44), SIMDE_FLOAT16_VALUE(-20.27), SIMDE_FLOAT16_VALUE(40.73), SIMDE_FLOAT16_VALUE(35.19), SIMDE_FLOAT16_VALUE(-43.88), SIMDE_FLOAT16_VALUE(21.72), SIMDE_FLOAT16_VALUE(-34.52),
        SIMDE_FLOAT16_VALUE(7.13), SIMDE_FLOAT16_VALUE(38.46), SIMDE_FLOAT16_VALUE(-3.30), SIMDE_FLOAT16_VALUE(-19.44), SIMDE_FLOAT16_VALUE(48.82), SIMDE_FLOAT16_VALUE(-43.82), SIMDE_FLOAT16_VALUE(46.77), SIMDE_FLOAT16_VALUE(39.55) },
     { {  SIMDE_FLOAT16_VALUE(-19.68), SIMDE_FLOAT16_VALUE(35.19), SIMDE_FLOAT16_VALUE(7.13), SIMDE_FLOAT16_VALUE(48.82) },
       {  SIMDE_FLOAT16_VALUE(38.44), SIMDE_FLOAT16_VALUE(-43.88), SIMDE_FLOAT16_VALUE(38.46), SIMDE_FLOAT16_VALUE(-43.82) },
       {  SIMDE_FLOAT16_VALUE(-20.27), SIMDE_FLOAT16_VALUE(21.72), SIMDE_FLOAT16_VALUE(-3.30), SIMDE_FLOAT16_VALUE(46.77) },
       {  SIMDE_FLOAT16_VALUE(40.73), SIMDE_FLOAT16_VALUE(-34.52), SIMDE_FLOAT16_VALUE(-19.44), SIMDE_FLOAT16_VALUE(39.55) } } },
     { {  SIMDE_FLOAT16_VALUE(28.68), SIMDE_FLOAT16_VALUE(14.00), SIMDE_FLOAT16_VALUE(15.71), SIMDE_FLOAT16_VALUE(-4.78), SIMDE_FLOAT16_VALUE(-11.15), SIMDE_FLOAT16_VALUE(-35.31), SIMDE_FLOAT16_VALUE(-28.54), SIMDE_FLOAT16_VALUE(-9.91),
        SIMDE_FLOAT16_VALUE(-19.18), SIMDE_FLOAT16_VALUE(-16.31), SIMDE_FLOAT16_VALUE(26.64), SIMDE_FLOAT16_VALUE(-0.60), SIMDE_FLOAT16_VALUE(46.00), SIMDE_FLOAT16_VALUE(29.32), SIMDE_FLOAT16_VALUE(49.37), SIMDE_FLOAT16_VALUE(49.85) },
     { {  SIMDE_FLOAT16_VALUE(28.68), SIMDE_FLOAT16_VALUE(-11.15), SIMDE_FLOAT16_VALUE(-19.18), SIMDE_FLOAT16_VALUE(46.00) },
       {  SIMDE_FLOAT16_VALUE(14.00), SIMDE_FLOAT16_VALUE(-35.31), SIMDE_FLOAT16_VALUE(-16.31), SIMDE_FLOAT16_VALUE(29.32) },
       {  SIMDE_FLOAT16_VALUE(15.71), SIMDE_FLOAT16_VALUE(-28.54), SIMDE_FLOAT16_VALUE(26.64), SIMDE_FLOAT16_VALUE(49.37) },
       {  SIMDE_FLOAT16_VALUE(-4.78), SIMDE_FLOAT16_VALUE(-9.91), SIMDE_FLOAT16_VALUE(-0.60), SIMDE_FLOAT16_VALUE(49.85) } } },
     { {  SIMDE_FLOAT16_VALUE(45.93), SIMDE_FLOAT16_VALUE(-28.16), SIMDE_FLOAT16_VALUE(-45.54), SIMDE_FLOAT16_VALUE(-39.77), SIMDE_FLOAT16_VALUE(5.90), SIMDE_FLOAT16_VALUE(13.00), SIMDE_FLOAT16_VALUE(39.78), SIMDE_FLOAT16_VALUE(5.72),
        SIMDE_FLOAT16_VALUE(-49.14), SIMDE_FLOAT16_VALUE(-9.08), SIMDE_FLOAT16_VALUE(-42.40), SIMDE_FLOAT16_VALUE(33.41), SIMDE_FLOAT16_VALUE(46.84), SIMDE_FLOAT16_VALUE(13.63), SIMDE_FLOAT16_VALUE(-1.26), SIMDE_FLOAT16_VALUE(-22.13) },
     { {  SIMDE_FLOAT16_VALUE(45.93), SIMDE_FLOAT16_VALUE(5.90), SIMDE_FLOAT16_VALUE(-49.14), SIMDE_FLOAT16_VALUE(46.84) },
       {  SIMDE_FLOAT16_VALUE(-28.16), SIMDE_FLOAT16_VALUE(13.00), SIMDE_FLOAT16_VALUE(-9.08), SIMDE_FLOAT16_VALUE(13.63) },
       {  SIMDE_FLOAT16_VALUE(-45.54), SIMDE_FLOAT16_VALUE(39.78), SIMDE_FLOAT16_VALUE(-42.40), SIMDE_FLOAT16_VALUE(-1.26) },
       {  SIMDE_FLOAT16_VALUE(-39.77), SIMDE_FLOAT16_VALUE(5.72), SIMDE_FLOAT16_VALUE(33.41), SIMDE_FLOAT16_VALUE(-22.13) } } },
     { {  SIMDE_FLOAT16_VALUE(28.54), SIMDE_FLOAT16_VALUE(-6.45), SIMDE_FLOAT16_VALUE(2.44), SIMDE_FLOAT16_VALUE(-10.11), SIMDE_FLOAT16_VALUE(-13.99), SIMDE_FLOAT16_VALUE(-2.36), SIMDE_FLOAT16_VALUE(49.31), SIMDE_FLOAT16_VALUE(35.77),
        SIMDE_FLOAT16_VALUE(-40.86), SIMDE_FLOAT16_VALUE(35.64), SIMDE_FLOAT16_VALUE(3.45), SIMDE_FLOAT16_VALUE(42.23), SIMDE_FLOAT16_VALUE(-23.61), SIMDE_FLOAT16_VALUE(-8.50), SIMDE_FLOAT16_VALUE(16.26), SIMDE_FLOAT16_VALUE(-39.89) },
     { {  SIMDE_FLOAT16_VALUE(28.54), SIMDE_FLOAT16_VALUE(-13.99), SIMDE_FLOAT16_VALUE(-40.86), SIMDE_FLOAT16_VALUE(-23.61) },
       {  SIMDE_FLOAT16_VALUE(-6.45), SIMDE_FLOAT16_VALUE(-2.36), SIMDE_FLOAT16_VALUE(35.64), SIMDE_FLOAT16_VALUE(-8.50) },
       {  SIMDE_FLOAT16_VALUE(2.44), SIMDE_FLOAT16_VALUE(49.31), SIMDE_FLOAT16_VALUE(3.45), SIMDE_FLOAT16_VALUE(16.26) },
       {  SIMDE_FLOAT16_VALUE(-10.11), SIMDE_FLOAT16_VALUE(35.77), SIMDE_FLOAT16_VALUE(42.23), SIMDE_FLOAT16_VALUE(-39.89) } } },
     { {  SIMDE_FLOAT16_VALUE(28.38), SIMDE_FLOAT16_VALUE(-38.32), SIMDE_FLOAT16_VALUE(-20.75), SIMDE_FLOAT16_VALUE(-37.97), SIMDE_FLOAT16_VALUE(41.86), SIMDE_FLOAT16_VALUE(-20.13), SIMDE_FLOAT16_VALUE(20.27), SIMDE_FLOAT16_VALUE(12.36),
        SIMDE_FLOAT16_VALUE(13.16), SIMDE_FLOAT16_VALUE(39.48), SIMDE_FLOAT16_VALUE(27.15), SIMDE_FLOAT16_VALUE(3.67), SIMDE_FLOAT16_VALUE(-33.92), SIMDE_FLOAT16_VALUE(36.94), SIMDE_FLOAT16_VALUE(-27.60), SIMDE_FLOAT16_VALUE(-10.49) },
     { {  SIMDE_FLOAT16_VALUE(28.38), SIMDE_FLOAT16_VALUE(41.86), SIMDE_FLOAT16_VALUE(13.16), SIMDE_FLOAT16_VALUE(-33.92) },
       {  SIMDE_FLOAT16_VALUE(-38.32), SIMDE_FLOAT16_VALUE(-20.13), SIMDE_FLOAT16_VALUE(39.48), SIMDE_FLOAT16_VALUE(36.94) },
       {  SIMDE_FLOAT16_VALUE(-20.75), SIMDE_FLOAT16_VALUE(20.27), SIMDE_FLOAT16_VALUE(27.15), SIMDE_FLOAT16_VALUE(-27.60) },
       {  SIMDE_FLOAT16_VALUE(-37.97), SIMDE_FLOAT16_VALUE(12.36), SIMDE_FLOAT16_VALUE(3.67), SIMDE_FLOAT16_VALUE(-10.49) } } },
     { {  SIMDE_FLOAT16_VALUE(5.31), SIMDE_FLOAT16_VALUE(-31.56), SIMDE_FLOAT16_VALUE(17.63), SIMDE_FLOAT16_VALUE(-32.75), SIMDE_FLOAT16_VALUE(-22.50), SIMDE_FLOAT16_VALUE(11.55), SIMDE_FLOAT16_VALUE(21.93), SIMDE_FLOAT16_VALUE(-23.80),
        SIMDE_FLOAT16_VALUE(19.27), SIMDE_FLOAT16_VALUE(0.51), SIMDE_FLOAT16_VALUE(15.52), SIMDE_FLOAT16_VALUE(6.68), SIMDE_FLOAT16_VALUE(48.85), SIMDE_FLOAT16_VALUE(-46.60), SIMDE_FLOAT16_VALUE(-40.92), SIMDE_FLOAT16_VALUE(-9.27) },
     { {  SIMDE_FLOAT16_VALUE(5.31), SIMDE_FLOAT16_VALUE(-22.50), SIMDE_FLOAT16_VALUE(19.27), SIMDE_FLOAT16_VALUE(48.85) },
       {  SIMDE_FLOAT16_VALUE(-31.56), SIMDE_FLOAT16_VALUE(11.55), SIMDE_FLOAT16_VALUE(0.51), SIMDE_FLOAT16_VALUE(-46.60) },
       {  SIMDE_FLOAT16_VALUE(17.63), SIMDE_FLOAT16_VALUE(21.93), SIMDE_FLOAT16_VALUE(15.52), SIMDE_FLOAT16_VALUE(-40.92) },
       {  SIMDE_FLOAT16_VALUE(-32.75), SIMDE_FLOAT16_VALUE(-23.80), SIMDE_FLOAT16_VALUE(6.68), SIMDE_FLOAT16_VALUE(-9.27) } } },
  };

  for (size_t i = 0 ; i < (sizeof(test_vec) / sizeof(test_vec[0])) ; i++) {
    simde_float16x4x4_t r = simde_vld4_f16(test_vec[i].a);

    simde_float16x4x4_t expected = {
        {simde_vld1_f16(test_vec[i].r[0]), simde_vld1_f16(test_vec[i].r[1]),
         simde_vld1_f16(test_vec[i].r[2]), simde_vld1_f16(test_vec[i].r[3])}};

    simde_test_arm_neon_assert_equal_f16x4(r.val[0], expected.val[0], INT_MAX);
    simde_test_arm_neon_assert_equal_f16x4(r.val[1], expected.val[1], INT_MAX);
    simde_test_arm_neon_assert_equal_f16x4(r.val[2], expected.val[2], INT_MAX);
    simde_test_arm_neon_assert_equal_f16x4(r.val[3], expected.val[3], INT_MAX);
  }

  return 0;

#else
  fputc('\n', stdout);
  for (int i = 0 ; i < 8 ; i++) {
    simde_float16x16_t a = simde_test_arm_neon_random_f16x16(-100.0f, 100.0f);
    simde_float16x4_t r[4] = simde_vld4_f16(a);

    simde_test_arm_neon_write_f16x16(2, a, SIMDE_TEST_VEC_POS_FIRST);
    simde_test_arm_neon_write_f16x4(2, r[4], SIMDE_TEST_VEC_POS_LAST);
  }
  return 1;
#endif
}

static int
test_simde_vld4q_f16 (SIMDE_MUNIT_TEST_ARGS) {
#if 1
  static const struct {
    simde_float16_t a[32];
    simde_float16_t r[4][8];
  } test_vec[] = {
    { {  SIMDE_FLOAT16_VALUE(  5.490),  SIMDE_FLOAT16_VALUE( 56.363),  SIMDE_FLOAT16_VALUE( 50.265), SIMDE_FLOAT16_VALUE( - 62.541),
         SIMDE_FLOAT16_VALUE( 32.240),  SIMDE_FLOAT16_VALUE( 80.441),  SIMDE_FLOAT16_VALUE(  9.478), SIMDE_FLOAT16_VALUE( - 73.979),
         SIMDE_FLOAT16_VALUE( 72.907), SIMDE_FLOAT16_VALUE( - 37.547), SIMDE_FLOAT16_VALUE( - 98.897), SIMDE_FLOAT16_VALUE( - 96.841),
        SIMDE_FLOAT16_VALUE( - 64.286), SIMDE_FLOAT16_VALUE( - 46.003),  SIMDE_FLOAT16_VALUE( 60.223),  SIMDE_FLOAT16_VALUE( 83.652),
        SIMDE_FLOAT16_VALUE( - 51.410),  SIMDE_FLOAT16_VALUE( 60.687),  SIMDE_FLOAT16_VALUE( 76.661),  SIMDE_FLOAT16_VALUE( 87.481),
        SIMDE_FLOAT16_VALUE( - 58.057),  SIMDE_FLOAT16_VALUE( 80.682), SIMDE_FLOAT16_VALUE( - 45.694), SIMDE_FLOAT16_VALUE( - 18.375),
         SIMDE_FLOAT16_VALUE(  4.987),  SIMDE_FLOAT16_VALUE( 72.799), SIMDE_FLOAT16_VALUE( - 32.887), SIMDE_FLOAT16_VALUE( - 91.185),
        SIMDE_FLOAT16_VALUE( -  5.996), SIMDE_FLOAT16_VALUE( - 80.690), SIMDE_FLOAT16_VALUE( - 87.372),  SIMDE_FLOAT16_VALUE( 74.876) },
    { {  SIMDE_FLOAT16_VALUE(  5.490),  SIMDE_FLOAT16_VALUE( 32.240),  SIMDE_FLOAT16_VALUE( 72.907), SIMDE_FLOAT16_VALUE( - 64.286),
        SIMDE_FLOAT16_VALUE( - 51.410), SIMDE_FLOAT16_VALUE( - 58.057),  SIMDE_FLOAT16_VALUE(  4.987), SIMDE_FLOAT16_VALUE( -  5.996)  },
      {  SIMDE_FLOAT16_VALUE( 56.363),  SIMDE_FLOAT16_VALUE( 80.441), SIMDE_FLOAT16_VALUE( - 37.547), SIMDE_FLOAT16_VALUE( - 46.003),
         SIMDE_FLOAT16_VALUE( 60.687),  SIMDE_FLOAT16_VALUE( 80.682),  SIMDE_FLOAT16_VALUE( 72.799), SIMDE_FLOAT16_VALUE( - 80.690)  },
      {  SIMDE_FLOAT16_VALUE( 50.265),  SIMDE_FLOAT16_VALUE(  9.478), SIMDE_FLOAT16_VALUE( - 98.897),  SIMDE_FLOAT16_VALUE( 60.223),
         SIMDE_FLOAT16_VALUE( 76.661), SIMDE_FLOAT16_VALUE( - 45.694), SIMDE_FLOAT16_VALUE( - 32.887), SIMDE_FLOAT16_VALUE( - 87.372)  },
      { SIMDE_FLOAT16_VALUE( - 62.541), SIMDE_FLOAT16_VALUE( - 73.979), SIMDE_FLOAT16_VALUE( - 96.841),  SIMDE_FLOAT16_VALUE( 83.652),
         SIMDE_FLOAT16_VALUE( 87.481), SIMDE_FLOAT16_VALUE( - 18.375), SIMDE_FLOAT16_VALUE( - 91.185),  SIMDE_FLOAT16_VALUE( 74.876)  } } },
    { {  SIMDE_FLOAT16_VALUE( 98.260), SIMDE_FLOAT16_VALUE( - 53.720),  SIMDE_FLOAT16_VALUE( 64.055), SIMDE_FLOAT16_VALUE( - 42.434),
        SIMDE_FLOAT16_VALUE( - 30.691), SIMDE_FLOAT16_VALUE( - 38.678),  SIMDE_FLOAT16_VALUE( 54.052), SIMDE_FLOAT16_VALUE( -  8.191),
         SIMDE_FLOAT16_VALUE( 46.474), SIMDE_FLOAT16_VALUE( - 27.690), SIMDE_FLOAT16_VALUE( - 52.522),  SIMDE_FLOAT16_VALUE( 18.409),
         SIMDE_FLOAT16_VALUE( 43.536),  SIMDE_FLOAT16_VALUE( 22.115), SIMDE_FLOAT16_VALUE( - 28.459),  SIMDE_FLOAT16_VALUE(  5.606),
         SIMDE_FLOAT16_VALUE( 61.671), SIMDE_FLOAT16_VALUE( - 91.137), SIMDE_FLOAT16_VALUE( - 60.090),  SIMDE_FLOAT16_VALUE( 52.864),
        SIMDE_FLOAT16_VALUE( - 91.079),  SIMDE_FLOAT16_VALUE( 12.797), SIMDE_FLOAT16_VALUE( -  0.876),  SIMDE_FLOAT16_VALUE( 60.271),
        SIMDE_FLOAT16_VALUE( - 31.039), SIMDE_FLOAT16_VALUE( -  8.129),  SIMDE_FLOAT16_VALUE( 80.874), SIMDE_FLOAT16_VALUE( - 18.920),
         SIMDE_FLOAT16_VALUE( 66.029),  SIMDE_FLOAT16_VALUE( 53.380),  SIMDE_FLOAT16_VALUE( 52.293),  SIMDE_FLOAT16_VALUE( 88.604) },
    { {  SIMDE_FLOAT16_VALUE( 98.260), SIMDE_FLOAT16_VALUE( - 30.691),  SIMDE_FLOAT16_VALUE( 46.474),  SIMDE_FLOAT16_VALUE( 43.536),
         SIMDE_FLOAT16_VALUE( 61.671), SIMDE_FLOAT16_VALUE( - 91.079), SIMDE_FLOAT16_VALUE( - 31.039),  SIMDE_FLOAT16_VALUE( 66.029)  },
      { SIMDE_FLOAT16_VALUE( - 53.720), SIMDE_FLOAT16_VALUE( - 38.678), SIMDE_FLOAT16_VALUE( - 27.690),  SIMDE_FLOAT16_VALUE( 22.115),
        SIMDE_FLOAT16_VALUE( - 91.137),  SIMDE_FLOAT16_VALUE( 12.797), SIMDE_FLOAT16_VALUE( -  8.129),  SIMDE_FLOAT16_VALUE( 53.380)  },
      {  SIMDE_FLOAT16_VALUE( 64.055),  SIMDE_FLOAT16_VALUE( 54.052), SIMDE_FLOAT16_VALUE( - 52.522), SIMDE_FLOAT16_VALUE( - 28.459),
        SIMDE_FLOAT16_VALUE( - 60.090), SIMDE_FLOAT16_VALUE( -  0.876),  SIMDE_FLOAT16_VALUE( 80.874),  SIMDE_FLOAT16_VALUE( 52.293)  },
      { SIMDE_FLOAT16_VALUE( - 42.434), SIMDE_FLOAT16_VALUE( -  8.191),  SIMDE_FLOAT16_VALUE( 18.409),  SIMDE_FLOAT16_VALUE(  5.606),
         SIMDE_FLOAT16_VALUE( 52.864),  SIMDE_FLOAT16_VALUE( 60.271), SIMDE_FLOAT16_VALUE( - 18.920),  SIMDE_FLOAT16_VALUE( 88.604)  } } },
    { { SIMDE_FLOAT16_VALUE( - 77.566),  SIMDE_FLOAT16_VALUE( 49.732),  SIMDE_FLOAT16_VALUE( 13.953),  SIMDE_FLOAT16_VALUE( 73.486),
        SIMDE_FLOAT16_VALUE( - 49.632),  SIMDE_FLOAT16_VALUE( 90.775),  SIMDE_FLOAT16_VALUE( 99.231), SIMDE_FLOAT16_VALUE( - 60.897),
        SIMDE_FLOAT16_VALUE( - 70.236),  SIMDE_FLOAT16_VALUE( 41.518),  SIMDE_FLOAT16_VALUE( 63.677), SIMDE_FLOAT16_VALUE( - 42.192),
        SIMDE_FLOAT16_VALUE( - 77.678), SIMDE_FLOAT16_VALUE( - 20.756), SIMDE_FLOAT16_VALUE( - 63.219), SIMDE_FLOAT16_VALUE( - 32.109),
        SIMDE_FLOAT16_VALUE( - 46.923), SIMDE_FLOAT16_VALUE( - 99.472),  SIMDE_FLOAT16_VALUE( 64.420),  SIMDE_FLOAT16_VALUE( 94.805),
         SIMDE_FLOAT16_VALUE(  7.983),  SIMDE_FLOAT16_VALUE( 71.674),  SIMDE_FLOAT16_VALUE( 25.662), SIMDE_FLOAT16_VALUE( - 88.933),
         SIMDE_FLOAT16_VALUE( 63.394),  SIMDE_FLOAT16_VALUE( 11.632), SIMDE_FLOAT16_VALUE( - 50.221),  SIMDE_FLOAT16_VALUE( 10.966),
        SIMDE_FLOAT16_VALUE( - 89.809),  SIMDE_FLOAT16_VALUE( 42.301),  SIMDE_FLOAT16_VALUE( 27.079),  SIMDE_FLOAT16_VALUE( 10.245) },
    { { SIMDE_FLOAT16_VALUE( - 77.566), SIMDE_FLOAT16_VALUE( - 49.632), SIMDE_FLOAT16_VALUE( - 70.236), SIMDE_FLOAT16_VALUE( - 77.678),
        SIMDE_FLOAT16_VALUE( - 46.923),  SIMDE_FLOAT16_VALUE(  7.983),  SIMDE_FLOAT16_VALUE( 63.394), SIMDE_FLOAT16_VALUE( - 89.809)  },
      {  SIMDE_FLOAT16_VALUE( 49.732),  SIMDE_FLOAT16_VALUE( 90.775),  SIMDE_FLOAT16_VALUE( 41.518), SIMDE_FLOAT16_VALUE( - 20.756),
        SIMDE_FLOAT16_VALUE( - 99.472),  SIMDE_FLOAT16_VALUE( 71.674),  SIMDE_FLOAT16_VALUE( 11.632),  SIMDE_FLOAT16_VALUE( 42.301)  },
      {  SIMDE_FLOAT16_VALUE( 13.953),  SIMDE_FLOAT16_VALUE( 99.231),  SIMDE_FLOAT16_VALUE( 63.677), SIMDE_FLOAT16_VALUE( - 63.219),
         SIMDE_FLOAT16_VALUE( 64.420),  SIMDE_FLOAT16_VALUE( 25.662), SIMDE_FLOAT16_VALUE( - 50.221),  SIMDE_FLOAT16_VALUE( 27.079)  },
      {  SIMDE_FLOAT16_VALUE( 73.486), SIMDE_FLOAT16_VALUE( - 60.897), SIMDE_FLOAT16_VALUE( - 42.192), SIMDE_FLOAT16_VALUE( - 32.109),
         SIMDE_FLOAT16_VALUE( 94.805), SIMDE_FLOAT16_VALUE( - 88.933),  SIMDE_FLOAT16_VALUE( 10.966),  SIMDE_FLOAT16_VALUE( 10.245)  } } },
    { {  SIMDE_FLOAT16_VALUE( 68.132),  SIMDE_FLOAT16_VALUE( 98.448),  SIMDE_FLOAT16_VALUE( 89.569),  SIMDE_FLOAT16_VALUE( 52.859),
        SIMDE_FLOAT16_VALUE( - 30.550),  SIMDE_FLOAT16_VALUE( 60.162),  SIMDE_FLOAT16_VALUE( 64.127), SIMDE_FLOAT16_VALUE( - 36.081),
         SIMDE_FLOAT16_VALUE( 22.014), SIMDE_FLOAT16_VALUE( -  4.060), SIMDE_FLOAT16_VALUE( -  1.772), SIMDE_FLOAT16_VALUE( - 61.338),
         SIMDE_FLOAT16_VALUE( 73.559), SIMDE_FLOAT16_VALUE( - 56.442),  SIMDE_FLOAT16_VALUE( 92.937),  SIMDE_FLOAT16_VALUE( 46.595),
         SIMDE_FLOAT16_VALUE( 96.771),  SIMDE_FLOAT16_VALUE( 67.430),  SIMDE_FLOAT16_VALUE( 36.578),  SIMDE_FLOAT16_VALUE( 86.457),
         SIMDE_FLOAT16_VALUE( 18.411), SIMDE_FLOAT16_VALUE( - 54.887), SIMDE_FLOAT16_VALUE( - 40.622), SIMDE_FLOAT16_VALUE( - 29.592),
         SIMDE_FLOAT16_VALUE( 15.246),  SIMDE_FLOAT16_VALUE(  9.582), SIMDE_FLOAT16_VALUE( - 96.924),  SIMDE_FLOAT16_VALUE( 86.899),
        SIMDE_FLOAT16_VALUE( - 52.384), SIMDE_FLOAT16_VALUE( - 92.867), SIMDE_FLOAT16_VALUE( - 44.117), SIMDE_FLOAT16_VALUE( - 90.337) },
    { {  SIMDE_FLOAT16_VALUE( 68.132), SIMDE_FLOAT16_VALUE( - 30.550),  SIMDE_FLOAT16_VALUE( 22.014),  SIMDE_FLOAT16_VALUE( 73.559),
         SIMDE_FLOAT16_VALUE( 96.771),  SIMDE_FLOAT16_VALUE( 18.411),  SIMDE_FLOAT16_VALUE( 15.246), SIMDE_FLOAT16_VALUE( - 52.384)  },
      {  SIMDE_FLOAT16_VALUE( 98.448),  SIMDE_FLOAT16_VALUE( 60.162), SIMDE_FLOAT16_VALUE( -  4.060), SIMDE_FLOAT16_VALUE( - 56.442),
         SIMDE_FLOAT16_VALUE( 67.430), SIMDE_FLOAT16_VALUE( - 54.887),  SIMDE_FLOAT16_VALUE(  9.582), SIMDE_FLOAT16_VALUE( - 92.867)  },
      {  SIMDE_FLOAT16_VALUE( 89.569),  SIMDE_FLOAT16_VALUE( 64.127), SIMDE_FLOAT16_VALUE( -  1.772),  SIMDE_FLOAT16_VALUE( 92.937),
         SIMDE_FLOAT16_VALUE( 36.578), SIMDE_FLOAT16_VALUE( - 40.622), SIMDE_FLOAT16_VALUE( - 96.924), SIMDE_FLOAT16_VALUE( - 44.117)  },
      {  SIMDE_FLOAT16_VALUE( 52.859), SIMDE_FLOAT16_VALUE( - 36.081), SIMDE_FLOAT16_VALUE( - 61.338),  SIMDE_FLOAT16_VALUE( 46.595),
         SIMDE_FLOAT16_VALUE( 86.457), SIMDE_FLOAT16_VALUE( - 29.592),  SIMDE_FLOAT16_VALUE( 86.899), SIMDE_FLOAT16_VALUE( - 90.337)  } } },
    { {  SIMDE_FLOAT16_VALUE( 86.525), SIMDE_FLOAT16_VALUE( -  7.364),  SIMDE_FLOAT16_VALUE( 76.938), SIMDE_FLOAT16_VALUE( - 33.673),
         SIMDE_FLOAT16_VALUE( 17.284),  SIMDE_FLOAT16_VALUE( 86.559), SIMDE_FLOAT16_VALUE( - 98.760),  SIMDE_FLOAT16_VALUE( 64.214),
        SIMDE_FLOAT16_VALUE( - 82.404), SIMDE_FLOAT16_VALUE( - 55.915), SIMDE_FLOAT16_VALUE( - 99.164),  SIMDE_FLOAT16_VALUE( 57.475),
         SIMDE_FLOAT16_VALUE( 37.359),  SIMDE_FLOAT16_VALUE(  4.426),  SIMDE_FLOAT16_VALUE( 96.766),  SIMDE_FLOAT16_VALUE(  1.291),
         SIMDE_FLOAT16_VALUE( 67.966),  SIMDE_FLOAT16_VALUE( 48.150), SIMDE_FLOAT16_VALUE( - 90.178),  SIMDE_FLOAT16_VALUE( 40.405),
         SIMDE_FLOAT16_VALUE( 38.096),  SIMDE_FLOAT16_VALUE( 52.337), SIMDE_FLOAT16_VALUE( - 71.332),  SIMDE_FLOAT16_VALUE( 60.795),
        SIMDE_FLOAT16_VALUE( - 66.608),  SIMDE_FLOAT16_VALUE( 83.954), SIMDE_FLOAT16_VALUE( - 75.610),  SIMDE_FLOAT16_VALUE( 72.697),
         SIMDE_FLOAT16_VALUE( 88.933),  SIMDE_FLOAT16_VALUE( 58.545),  SIMDE_FLOAT16_VALUE( 55.080), SIMDE_FLOAT16_VALUE( -  1.566) },
    { {  SIMDE_FLOAT16_VALUE( 86.525),  SIMDE_FLOAT16_VALUE( 17.284), SIMDE_FLOAT16_VALUE( - 82.404),  SIMDE_FLOAT16_VALUE( 37.359),
         SIMDE_FLOAT16_VALUE( 67.966),  SIMDE_FLOAT16_VALUE( 38.096), SIMDE_FLOAT16_VALUE( - 66.608),  SIMDE_FLOAT16_VALUE( 88.933)  },
      { SIMDE_FLOAT16_VALUE( -  7.364),  SIMDE_FLOAT16_VALUE( 86.559), SIMDE_FLOAT16_VALUE( - 55.915),  SIMDE_FLOAT16_VALUE(  4.426),
         SIMDE_FLOAT16_VALUE( 48.150),  SIMDE_FLOAT16_VALUE( 52.337),  SIMDE_FLOAT16_VALUE( 83.954),  SIMDE_FLOAT16_VALUE( 58.545)  },
      {  SIMDE_FLOAT16_VALUE( 76.938), SIMDE_FLOAT16_VALUE( - 98.760), SIMDE_FLOAT16_VALUE( - 99.164),  SIMDE_FLOAT16_VALUE( 96.766),
        SIMDE_FLOAT16_VALUE( - 90.178), SIMDE_FLOAT16_VALUE( - 71.332), SIMDE_FLOAT16_VALUE( - 75.610),  SIMDE_FLOAT16_VALUE( 55.080)  },
      { SIMDE_FLOAT16_VALUE( - 33.673),  SIMDE_FLOAT16_VALUE( 64.214),  SIMDE_FLOAT16_VALUE( 57.475),  SIMDE_FLOAT16_VALUE(  1.291),
         SIMDE_FLOAT16_VALUE( 40.405),  SIMDE_FLOAT16_VALUE( 60.795),  SIMDE_FLOAT16_VALUE( 72.697), SIMDE_FLOAT16_VALUE( -  1.566)  } } },
    { {  SIMDE_FLOAT16_VALUE( 61.363), SIMDE_FLOAT16_VALUE( - 94.735),  SIMDE_FLOAT16_VALUE( 71.030), SIMDE_FLOAT16_VALUE( - 54.274),
         SIMDE_FLOAT16_VALUE( 88.136), SIMDE_FLOAT16_VALUE( -  5.456),  SIMDE_FLOAT16_VALUE( 47.220), SIMDE_FLOAT16_VALUE( - 29.565),
         SIMDE_FLOAT16_VALUE( 22.067), SIMDE_FLOAT16_VALUE( - 60.529), SIMDE_FLOAT16_VALUE( - 86.561),  SIMDE_FLOAT16_VALUE( 59.456),
         SIMDE_FLOAT16_VALUE( 18.893), SIMDE_FLOAT16_VALUE( -  3.232), SIMDE_FLOAT16_VALUE( -  5.500),  SIMDE_FLOAT16_VALUE( 38.212),
         SIMDE_FLOAT16_VALUE( 64.008),  SIMDE_FLOAT16_VALUE( 23.363), SIMDE_FLOAT16_VALUE( - 52.655), SIMDE_FLOAT16_VALUE( - 83.862),
         SIMDE_FLOAT16_VALUE( 82.969), SIMDE_FLOAT16_VALUE( - 51.413), SIMDE_FLOAT16_VALUE( -  6.933),  SIMDE_FLOAT16_VALUE( 82.898),
         SIMDE_FLOAT16_VALUE( 66.306), SIMDE_FLOAT16_VALUE( - 60.470),  SIMDE_FLOAT16_VALUE( 30.553),  SIMDE_FLOAT16_VALUE( 84.495),
         SIMDE_FLOAT16_VALUE( 23.918),  SIMDE_FLOAT16_VALUE( 73.675),  SIMDE_FLOAT16_VALUE( 29.921), SIMDE_FLOAT16_VALUE( - 13.427) },
    { {  SIMDE_FLOAT16_VALUE( 61.363),  SIMDE_FLOAT16_VALUE( 88.136),  SIMDE_FLOAT16_VALUE( 22.067),  SIMDE_FLOAT16_VALUE( 18.893),
         SIMDE_FLOAT16_VALUE( 64.008),  SIMDE_FLOAT16_VALUE( 82.969),  SIMDE_FLOAT16_VALUE( 66.306),  SIMDE_FLOAT16_VALUE( 23.918)  },
      { SIMDE_FLOAT16_VALUE( - 94.735), SIMDE_FLOAT16_VALUE( -  5.456), SIMDE_FLOAT16_VALUE( - 60.529), SIMDE_FLOAT16_VALUE( -  3.232),
         SIMDE_FLOAT16_VALUE( 23.363), SIMDE_FLOAT16_VALUE( - 51.413), SIMDE_FLOAT16_VALUE( - 60.470),  SIMDE_FLOAT16_VALUE( 73.675)  },
      {  SIMDE_FLOAT16_VALUE( 71.030),  SIMDE_FLOAT16_VALUE( 47.220), SIMDE_FLOAT16_VALUE( - 86.561), SIMDE_FLOAT16_VALUE( -  5.500),
        SIMDE_FLOAT16_VALUE( - 52.655), SIMDE_FLOAT16_VALUE( -  6.933),  SIMDE_FLOAT16_VALUE( 30.553),  SIMDE_FLOAT16_VALUE( 29.921)  },
      { SIMDE_FLOAT16_VALUE( - 54.274), SIMDE_FLOAT16_VALUE( - 29.565),  SIMDE_FLOAT16_VALUE( 59.456),  SIMDE_FLOAT16_VALUE( 38.212),
        SIMDE_FLOAT16_VALUE( - 83.862),  SIMDE_FLOAT16_VALUE( 82.898),  SIMDE_FLOAT16_VALUE( 84.495), SIMDE_FLOAT16_VALUE( - 13.427)  } } },
    { {  SIMDE_FLOAT16_VALUE( 35.383), SIMDE_FLOAT16_VALUE( - 89.112),  SIMDE_FLOAT16_VALUE( 26.970),  SIMDE_FLOAT16_VALUE(  7.485),
        SIMDE_FLOAT16_VALUE( - 82.298),  SIMDE_FLOAT16_VALUE( 74.716), SIMDE_FLOAT16_VALUE( - 98.970),  SIMDE_FLOAT16_VALUE( 39.884),
        SIMDE_FLOAT16_VALUE( - 46.463),  SIMDE_FLOAT16_VALUE( 57.795), SIMDE_FLOAT16_VALUE( -  1.432),  SIMDE_FLOAT16_VALUE( 99.301),
         SIMDE_FLOAT16_VALUE( 32.827), SIMDE_FLOAT16_VALUE( - 87.007), SIMDE_FLOAT16_VALUE( - 21.524),  SIMDE_FLOAT16_VALUE( 27.532),
        SIMDE_FLOAT16_VALUE( - 80.235), SIMDE_FLOAT16_VALUE( -  6.353), SIMDE_FLOAT16_VALUE( - 90.137),  SIMDE_FLOAT16_VALUE( 13.171),
         SIMDE_FLOAT16_VALUE( 76.820),  SIMDE_FLOAT16_VALUE( 54.313),  SIMDE_FLOAT16_VALUE( 54.393), SIMDE_FLOAT16_VALUE( - 32.240),
        SIMDE_FLOAT16_VALUE( - 70.987), SIMDE_FLOAT16_VALUE( - 72.386),  SIMDE_FLOAT16_VALUE( 33.691),  SIMDE_FLOAT16_VALUE(  8.833),
        SIMDE_FLOAT16_VALUE( - 74.879), SIMDE_FLOAT16_VALUE( -  3.497), SIMDE_FLOAT16_VALUE( - 98.523),  SIMDE_FLOAT16_VALUE( 53.411) },
    { {  SIMDE_FLOAT16_VALUE( 35.383), SIMDE_FLOAT16_VALUE( - 82.298), SIMDE_FLOAT16_VALUE( - 46.463),  SIMDE_FLOAT16_VALUE( 32.827),
        SIMDE_FLOAT16_VALUE( - 80.235),  SIMDE_FLOAT16_VALUE( 76.820), SIMDE_FLOAT16_VALUE( - 70.987), SIMDE_FLOAT16_VALUE( - 74.879)  },
      { SIMDE_FLOAT16_VALUE( - 89.112),  SIMDE_FLOAT16_VALUE( 74.716),  SIMDE_FLOAT16_VALUE( 57.795), SIMDE_FLOAT16_VALUE( - 87.007),
        SIMDE_FLOAT16_VALUE( -  6.353),  SIMDE_FLOAT16_VALUE( 54.313), SIMDE_FLOAT16_VALUE( - 72.386), SIMDE_FLOAT16_VALUE( -  3.497)  },
      {  SIMDE_FLOAT16_VALUE( 26.970), SIMDE_FLOAT16_VALUE( - 98.970), SIMDE_FLOAT16_VALUE( -  1.432), SIMDE_FLOAT16_VALUE( - 21.524),
        SIMDE_FLOAT16_VALUE( - 90.137),  SIMDE_FLOAT16_VALUE( 54.393),  SIMDE_FLOAT16_VALUE( 33.691), SIMDE_FLOAT16_VALUE( - 98.523)  },
      {  SIMDE_FLOAT16_VALUE(  7.485),  SIMDE_FLOAT16_VALUE( 39.884),  SIMDE_FLOAT16_VALUE( 99.301),  SIMDE_FLOAT16_VALUE( 27.532),
         SIMDE_FLOAT16_VALUE( 13.171), SIMDE_FLOAT16_VALUE( - 32.240),  SIMDE_FLOAT16_VALUE(  8.833),  SIMDE_FLOAT16_VALUE( 53.411)  } } },
    { { SIMDE_FLOAT16_VALUE( - 50.312), SIMDE_FLOAT16_VALUE( - 24.102), SIMDE_FLOAT16_VALUE( - 97.391),  SIMDE_FLOAT16_VALUE(  3.022),
         SIMDE_FLOAT16_VALUE( 78.988), SIMDE_FLOAT16_VALUE( - 25.237), SIMDE_FLOAT16_VALUE( - 90.131), SIMDE_FLOAT16_VALUE( - 97.836),
        SIMDE_FLOAT16_VALUE( - 76.208), SIMDE_FLOAT16_VALUE( - 18.262),  SIMDE_FLOAT16_VALUE( 23.059), SIMDE_FLOAT16_VALUE( - 86.163),
         SIMDE_FLOAT16_VALUE( 65.842), SIMDE_FLOAT16_VALUE( - 75.486),  SIMDE_FLOAT16_VALUE( 67.646),  SIMDE_FLOAT16_VALUE( 59.138),
         SIMDE_FLOAT16_VALUE( 55.197),  SIMDE_FLOAT16_VALUE( 52.575), SIMDE_FLOAT16_VALUE( - 52.818),  SIMDE_FLOAT16_VALUE( 58.329),
        SIMDE_FLOAT16_VALUE( - 28.835), SIMDE_FLOAT16_VALUE( - 10.915),  SIMDE_FLOAT16_VALUE( 75.879), SIMDE_FLOAT16_VALUE( -  0.973),
        SIMDE_FLOAT16_VALUE( - 58.718),  SIMDE_FLOAT16_VALUE( 12.668), SIMDE_FLOAT16_VALUE( - 98.837),  SIMDE_FLOAT16_VALUE( 76.632),
         SIMDE_FLOAT16_VALUE( 79.542),  SIMDE_FLOAT16_VALUE( 34.512), SIMDE_FLOAT16_VALUE( - 94.397),  SIMDE_FLOAT16_VALUE( 26.405) },
    { { SIMDE_FLOAT16_VALUE( - 50.312),  SIMDE_FLOAT16_VALUE( 78.988), SIMDE_FLOAT16_VALUE( - 76.208),  SIMDE_FLOAT16_VALUE( 65.842),
         SIMDE_FLOAT16_VALUE( 55.197), SIMDE_FLOAT16_VALUE( - 28.835), SIMDE_FLOAT16_VALUE( - 58.718),  SIMDE_FLOAT16_VALUE( 79.542)  },
      { SIMDE_FLOAT16_VALUE( - 24.102), SIMDE_FLOAT16_VALUE( - 25.237), SIMDE_FLOAT16_VALUE( - 18.262), SIMDE_FLOAT16_VALUE( - 75.486),
         SIMDE_FLOAT16_VALUE( 52.575), SIMDE_FLOAT16_VALUE( - 10.915),  SIMDE_FLOAT16_VALUE( 12.668),  SIMDE_FLOAT16_VALUE( 34.512)  },
      { SIMDE_FLOAT16_VALUE( - 97.391), SIMDE_FLOAT16_VALUE( - 90.131),  SIMDE_FLOAT16_VALUE( 23.059),  SIMDE_FLOAT16_VALUE( 67.646),
        SIMDE_FLOAT16_VALUE( - 52.818),  SIMDE_FLOAT16_VALUE( 75.879), SIMDE_FLOAT16_VALUE( - 98.837), SIMDE_FLOAT16_VALUE( - 94.397)  },
      {  SIMDE_FLOAT16_VALUE(  3.022), SIMDE_FLOAT16_VALUE( - 97.836), SIMDE_FLOAT16_VALUE( - 86.163),  SIMDE_FLOAT16_VALUE( 59.138),
         SIMDE_FLOAT16_VALUE( 58.329), SIMDE_FLOAT16_VALUE( -  0.973),  SIMDE_FLOAT16_VALUE( 76.632),  SIMDE_FLOAT16_VALUE( 26.405)  } } },
  };

  for (size_t i = 0 ; i < (sizeof(test_vec) / sizeof(test_vec[0])) ; i++) {
    simde_float16x8x4_t r = simde_vld4q_f16(test_vec[i].a);

    simde_float16x8x4_t expected = {
        {simde_vld1q_f16(test_vec[i].r[0]), simde_vld1q_f16(test_vec[i].r[1]),
         simde_vld1q_f16(test_vec[i].r[2]), simde_vld1q_f16(test_vec[i].r[3])}};

    simde_test_arm_neon_assert_equal_f16x8(r.val[0], expected.val[0], INT_MAX);
    simde_test_arm_neon_assert_equal_f16x8(r.val[1], expected.val[1], INT_MAX);
    simde_test_arm_neon_assert_equal_f16x8(r.val[2], expected.val[2], INT_MAX);
    simde_test_arm_neon_assert_equal_f16x8(r.val[3], expected.val[3], INT_MAX);
  }

  return 0;

#else
  fputc('\n', stdout);
  for (int i = 0 ; i < 8 ; i++) {
    simde_float16x32_t a = simde_test_arm_neon_random_f16x32(-100.0f, 100.0f);
    simde_float16x4_t r[4] = simde_vld4q_f16(a);

    simde_test_arm_neon_write_f16x32(2, a, SIMDE_TEST_VEC_POS_FIRST);
    simde_test_arm_neon_write_f16x4(2, r[4], SIMDE_TEST_VEC_POS_LAST);
  }
  return 1;
#endif
}

#endif /* !defined(SIMDE_BUG_INTEL_857088) */
SIMDE_TEST_FUNC_LIST_BEGIN
#if !defined(SIMDE_BUG_INTEL_857088)
SIMDE_TEST_FUNC_LIST_ENTRY(vld4_f16)
SIMDE_TEST_FUNC_LIST_ENTRY(vld4q_f16)
#endif /* !defined(SIMDE_BUG_INTEL_857088) */
SIMDE_TEST_FUNC_LIST_END

#include "test-neon-footer.h"
