#include <iostream>
#include <numbers>
#include <cmath>
#include <format>
#include <cassert>

float sin_approx(float x);
float cos_approx(float x);
void test_sin();
void test_cos();

int main() {
    test_sin();
    test_cos();

    return 0;
}

float sin_approx(float x) {
    const float half_pi = static_cast<float>(std::numbers::pi / 2);
    const float two_pi = static_cast<float>(std::numbers::pi * 2);

    // 2pi clamp.
    x = std::fmod(x, two_pi);

    // Range reduction.
    const float abs_x = std::abs(x);
    const float sign = x / abs_x;
    if (abs_x > half_pi && abs_x <= std::numbers::pi + half_pi)
        x = (half_pi - (abs_x - half_pi)) * sign;
    else if (abs_x > std::numbers::pi + half_pi && abs_x <= two_pi)
        x = (abs_x - two_pi) * sign;

    const float a0 = 0.0f;
    const float a1 = 1.0f;
    const float a2 = 0.0f;
    const float a3 = -1.0f / (2 * 3);
    const float a4 = 0.0f;
    const float a5 = 1.0f / (2 * 3 * 4 * 5);
    const float a6 = 0.0f;
    const float a7 = -1.0f / (2 * 3 * 4 * 5 * 6 * 7);

    return
        a0 +
        a1 * x +
        a2 * (x * x) +
        a3 * (x * x * x) +
        a4 * (x * x * x * x) +
        a5 * (x * x * x * x * x) +
        a6 * (x * x * x * x * x * x) +
        a7 * (x * x * x * x * x * x * x);
}

// NOTE: sin_approx gives better precision as x approaches 0.
// So, as this cos is built on top of sin_approx, it gives better precision when x approaches pi/2 not 0.
float cos_approx(float x) {
    return sin_approx(x + static_cast<float>(std::numbers::pi / 2));
}

void test_sin() {
    const float angle_min = -static_cast<float>(std::numbers::pi * 4);
    const float angle_max = static_cast<float>(std::numbers::pi * 4);
    const int cnt = 1000;
    const float angle_step = (angle_max - angle_min) / cnt;
    float cur_angle = angle_min;
    for (int i = 0; i <= cnt; ++i) {
        const float res_native = std::sin(cur_angle);
        const float res_approx = sin_approx(cur_angle);

        std::cout << std::format("sin_native({})={}\n", cur_angle, res_native);
        std::cout << std::format("sin_approx({})={}\n\n", cur_angle, res_approx);

        assert(std::abs(res_native - res_approx) < 1e-3f);

        cur_angle += angle_step;
    }
}

void test_cos() {
    const float angle_min = -static_cast<float>(std::numbers::pi * 4);
    const float angle_max = static_cast<float>(std::numbers::pi * 4);
    const int cnt = 1000;
    const float angle_step = (angle_max - angle_min) / cnt;
    float cur_angle = angle_min;
    for (int i = 0; i <= cnt; ++i) {
        const float res_native = std::cos(cur_angle);
        const float res_approx = cos_approx(cur_angle);

        std::cout << std::format("cos_native({})={}\n", cur_angle, res_native);
        std::cout << std::format("cos_approx({})={}\n\n", cur_angle, res_approx);

        assert(std::abs(res_native - res_approx) < 1e-3f);

        cur_angle += angle_step;
    }
}
