#pragma once

#include <cbor.h>

#include "project.h"

// Utility
typedef union {
  struct {
    float roll;
    float pitch;
    float yaw;
  };
  float axis[3];
} vector_t;

// Rates
typedef enum {
  RATE_MODE_SILVERWARE,
  RATE_MODE_BETAFLIGHT,
} rate_modes_t;

typedef struct {
  vector_t max_rate;
  vector_t acro_expo;
  vector_t angle_expo;
} rate_mode_silverware_t;

#define SILVERWARE_RATE_MEMBERS \
  MEMBER(max_rate, vector_t)    \
  MEMBER(acro_expo, vector_t)   \
  MEMBER(angle_expo, vector_t)

typedef struct {
  vector_t rc_rate;
  vector_t super_rate;
  vector_t expo;
} rate_mode_betaflight_t;

#define BETAFLIGHT_RATE_MEMBERS \
  MEMBER(rc_rate, vector_t)     \
  MEMBER(super_rate, vector_t)  \
  MEMBER(expo, vector_t)

typedef struct {
  rate_modes_t mode;
  rate_mode_silverware_t silverware;
  rate_mode_betaflight_t betaflight;
  float level_max_angle;
  float low_rate_mulitplier;
  float sticks_deadband;
} rate_t;

#define RATE_MEMBERS                         \
  MEMBER(mode, uint8)                        \
  MEMBER(silverware, rate_mode_silverware_t) \
  MEMBER(betaflight, rate_mode_betaflight_t) \
  MEMBER(level_max_angle, float)             \
  MEMBER(low_rate_mulitplier, float)         \
  MEMBER(sticks_deadband, float)

typedef struct {
  vector_t kp;
  vector_t ki;
  vector_t kd;
} pid_rate_t;

#define PID_RATE_MEMBERS \
  MEMBER(kp, vector_t)   \
  MEMBER(ki, vector_t)   \
  MEMBER(kd, vector_t)

typedef enum {
  PID_PROFILE_1,
  PID_PROFILE_2,
  PID_PROFILE_MAX
} pid_profile_t;

typedef struct {
  vector_t accelerator;
  vector_t transition;
} stick_rate_t;

#define STICK_RATE_MEMBERS      \
  MEMBER(accelerator, vector_t) \
  MEMBER(transition, vector_t)

typedef enum {
  STICK_PROFILE_OFF,
  STICK_PROFILE_ON,
  STICK_PROFILE_MAX
} stick_profile_t;

typedef struct {
  pid_profile_t pid_profile;
  pid_rate_t pid_rates[PID_PROFILE_MAX];
  stick_profile_t stick_profile;
  stick_rate_t stick_rates[STICK_PROFILE_MAX];
} profile_pid_t;

#define PID_MEMBERS                                    \
  MEMBER(pid_profile, uint8)                           \
  ARRAY_MEMBER(pid_rates, PID_PROFILE_MAX, pid_rate_t) \
  MEMBER(stick_profile, uint8)                         \
  ARRAY_MEMBER(stick_rates, STICK_PROFILE_MAX, stick_rate_t)

typedef enum {
  GYRO_ROTATE_NONE = 0x0,
  GYRO_ROTATE_45_CCW = 0x1,
  GYRO_ROTATE_45_CW = 0x2,
  GYRO_ROTATE_90_CW = 0x4,
  GYRO_ROTATE_90_CCW = 0x8,
  GYRO_ROTATE_180 = 0x10,
  GYRO_FLIP_180 = 0x20,
} gyro_rotation_t;

typedef struct {
  float digital_idle;
  uint8_t invert_yaw;
  uint8_t gyro_orientation;
} motor_t;

#define MOTOR_MEMBERS         \
  MEMBER(digital_idle, float) \
  MEMBER(invert_yaw, uint8)   \
  MEMBER(gyro_orientation, uint8)

typedef struct {
  uint8_t lipo_cell_count;
  uint8_t pid_voltage_compensation;
  float vbattlow;
  float actual_battery_voltage;
  float reported_telemetry_voltage;
} voltage_t;

#define VOLTAGE_MEMBERS                   \
  MEMBER(lipo_cell_count, uint8)          \
  MEMBER(pid_voltage_compensation, uint8) \
  MEMBER(vbattlow, float)                 \
  MEMBER(actual_battery_voltage, float)   \
  MEMBER(reported_telemetry_voltage, float)

typedef struct {
  aux_channel_t aux[AUX_FUNCTION_MAX];
} channel_t;

#define CHANNEL_MEMBERS \
  ARRAY_MEMBER(aux, AUX_FUNCTION_MAX, uint8)

typedef struct {
  uint8_t name[36];
  uint32_t datetime;
} metadata_t;

// Full Profile
typedef struct {
  metadata_t meta;
  motor_t motor;
  rate_t rate;
  channel_t channel;
  profile_pid_t pid;
  voltage_t voltage;
} profile_t;

#define PROFILE_MEMBERS      \
  MEMBER(meta, metadata_t)   \
  MEMBER(motor, motor_t)     \
  MEMBER(rate, rate_t)       \
  MEMBER(channel, channel_t) \
  MEMBER(pid, profile_pid_t) \
  MEMBER(voltage, voltage_t)

void profile_set_defaults();
pid_rate_t *profile_current_pid_rates();

cbor_result_t cbor_encode_vector_t(cbor_value_t *enc, const vector_t *vec);
cbor_result_t cbor_decode_profile_t(cbor_value_t *dec, profile_t *p);

cbor_result_t cbor_encode_profile_t(cbor_value_t *enc, const profile_t *p);
cbor_result_t cbor_decode_vector_t(cbor_value_t *dec, vector_t *vec);