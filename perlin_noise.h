#pragma once

#include <vector>
#include <cmath>
#include <random> 
#include <numbers>
#include <glm/glm.hpp>

class PerlinNoise {
public:
	PerlinNoise() : m_grads(GRID_SIZE)
	{
        std::random_device rd;
        std::mt19937 gen(rd()); 
        std::uniform_real_distribution<float> dist(0.0f, 2.0f * std::numbers::pi_v<float>);

        for (int i = 0; i < GRID_SIZE; ++i)
        {
            for (int j = 0; j < GRID_SIZE; ++j)
            {
                float rnd = dist(gen);
                m_grads[i].emplace_back(std::sin(rnd), std::cos(rnd));
            }
        }
	}

    float Noise(float x, float y)
    {
        float result = 0.0;
        float amplitude = 1.0;
        float frequency = 0.5;

        for (int i = 0; i < 5; i++)
        {
            result += amplitude * GetValue(x * frequency, y * frequency);
            amplitude *= 0.5;
            frequency *= 2.0;
        }

        return result;
    }

private:
    glm::vec2 GetGradient(int x, int y)
    {
        return m_grads[x % GRID_SIZE][y % GRID_SIZE];
    }

    float GetValue(float x, float y)
    {
        int ix = std::floor(x);
        int iy = std::floor(y);
        
        float dx = x - ix;
        float dy = y - iy;

        auto tl = GetGradient(ix, iy);
        auto tr = GetGradient(ix + 1, iy);
        auto bl = GetGradient(ix, iy + 1);
        auto br = GetGradient(ix + 1, iy + 1);

        auto dot_tl = glm::dot(tl, { dx, dy });
        auto dot_tr = glm::dot(tr, { dx - 1, dy });
        auto dot_bl = glm::dot(bl, { dx, dy - 1 });
        auto dot_br = glm::dot(br, { dx - 1, dy - 1});

        float u = Fade(dx);
        float v = Fade(dy);

        return Lerp(u, 
                Lerp(v, dot_tl, dot_bl),
                Lerp(v, dot_tr, dot_br));
    }

    static float Fade(float t)
    {
        return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
    }

    static float Lerp(float t, float a, float b)
    {
        return a + t * (b - a);
    }

private:
	std::vector<std::vector<glm::vec2>> m_grads;
	static constexpr int GRID_SIZE = 80;
};