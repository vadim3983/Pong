#pragma once

constexpr auto max_wchar = 256;
const double gap = ball_diameter * (1.0 / 2 - 1 / (2 * sqrt(2)));
constexpr int point_height = 80;
constexpr int point_width = WIDTH / 3;
constexpr int trace_count = (ball_diameter - 1) / BALL_SIZING;
