//
// Created by Cafer SELLI on 1.06.2024.
//
#include "../headers/PerlinNoise.h"

glm::vec2 randomGradient(int ix, int iy) {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3284157443;

    b ^= a << s | a >> (w - s);
    b *= 1911520717;

    a ^= b << s | b >> (w - s);
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi] NOLINT(*-narrowing-conversions)

    // Create the vector from the angle
    glm::vec2 v;
    v.x = sin(random);
    v.y = cos(random);

    return v;
}

float dotGridGradient(int ix, int iy, float x, float y) {
    // Get gradient from integer coordinates
    glm::vec2 gradient = randomGradient(ix, iy);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}

float interpolate(float a, float b, float t){
    //−6 𝑥
    //5 + 15 𝑥
    //4 − 10|𝑥|
    //3 + 1 if 𝑥 < 1
    if(t < 0) t = -t;
    float t2 = -6.0 * (t * t * t * t * t) + 15.0 * (t * t * t * t) - 10.0 * (t * t * t) + 1; // NOLINT(*-narrowing-conversions)
    return a * t2 + (1.0 - t2) * b; // NOLINT(*-narrowing-conversions)
}

//float interpolate(float a0, float a1, float w)
//{
//    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
//}

float perlin_noise(float x, float y){
    int x0 = (int)x;
    int x1 = x0 + 1;
    int y0 = (int)y;
    int y1 = y0 + 1;

    float sx = x - (float)x0;
    float sy = y - (float)y0;

    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);
    float ix0 = interpolate(n0, n1, sx);

    // Compute and interpolate bottom two corners
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    float ix1 = interpolate(n0, n1, sx);


    return interpolate(ix0, ix1, sy);
}
