#include "../include.hpp"
#include "../classes/math/math.hpp"
#include "globals.hpp"

std::pair<float, float> Utils::getWindowSize(HWND window)
{
	RECT rect;
	if (GetWindowRect(window, &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		return { width, height };
	}
	return { 0, 0 };
}

float* Utils::color_cycle()
{
    switch (color_iteration)
    {
    case 0:
        color_hue[1]++; /* 255, ++, 0 */

        if (color_hue[1] == 255)
            color_iteration++;
        break;
    case 1:
        color_hue[0]--; /* --, 255, 0 */

        if (color_hue[0] == 0)
            color_iteration++;
        break;
    case 2:
        color_hue[2]++; /* 0, 255, ++ */

        if (color_hue[2] == 255)
            color_iteration++;
        break;
    case 3:
        color_hue[1]--; /* 0, --, 255 */

        if (color_hue[1] == 0)
            color_iteration++;
        break;
    case 4:
        color_hue[0]++; /* ++, 0, 255 */

        if (color_hue[0] == 255)
            color_iteration++;
        break;
    case 5:
        color_hue[2]--; /* 255, 0, -- */

        if (color_hue[2] == 0)
            color_iteration++;
        break;
    case 6:
        color_iteration = 0;
    }

    float f_color[4] = { color_hue[0] / 255.f, color_hue[1] / 255.f, color_hue[2] / 255.f, 1.f };


    return f_color;
}

std::string Utils::wstring_to_string(const std::wstring& wstring)
{
	int slength = (int)wstring.length() + 1;
	int len = WideCharToMultiByte(CP_ACP, 0, wstring.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, wstring.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

Vector2 Utils::W2S(Vector3 origin, Matrix* view_matrix, uint64_t camera)
{
	Matrix temp;
	*view_matrix = mem->Read<Matrix>(camera + 0x2E4, true);

	MatrixTranspose(&temp, view_matrix);

	Vector3 translationVector = Vector3(temp._41, temp._42, temp._43);
	Vector3 up = Vector3(temp._21, temp._22, temp._23);
	Vector3 right = Vector3(temp._11, temp._12, temp._13);

	float w = Vec3Dot(&translationVector, &origin) + temp._44;

	if (w < 0.098f)
		return { 0.0f, 0.0f };

	float y = Vec3Dot(&up, &origin) + temp._24;
	float x = Vec3Dot(&right, &origin) + temp._14;

	return Vector2
	{
		(globals->ScreenSize.x / 2) * (1.f + x / w),
		(globals->ScreenSize.y / 2) * (1.f - y / w)
	};
}

Utils* utils = new Utils();
Settings* settings = new Settings();