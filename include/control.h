#ifndef CONTROL_H
#define CONTROL_H

#include <config.h>
#include <motors.h>
#include <sensors.h>

void set_race_started(bool started);
bool is_race_started();
void set_race_starting(bool starting);
bool is_race_starting();
long get_race_started_ms();
long get_race_stopped_ms();

void set_base_speed(int speed);
void set_base_accel_speed(int accel_speed);
void set_base_fan_speed(int fan_speed);
int get_base_fan_speed();

void set_pid_kp(float kp);
float get_pid_kp();
void set_pid_kd(float kd);
float get_pid_kd();

void initial_control_loop();
void control_loop();

#endif