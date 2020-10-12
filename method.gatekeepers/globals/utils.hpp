#ifndef _UTILS_HPP
#define _UTILS_HPP

class Settings
{
public:
    /* visuals */
    struct visuals {
        struct crosshair {
            bool enabled = true;
            int size = 5;
            int x_offset;
            int y_offset;
        } crosshair;

        struct watermark {
            bool enabled = true;
            float color[4] = { 1, 1, 1, 1 };
        } watermark;

        struct players {
            struct name {
                bool enabled;
                float color[4] = { 0, 0, 0, 1 };
            } name;

            struct box {
                bool enabled;
                bool health_bar;
                float health_bar_color[4] = { 0, 0, 0, 1 };
                float outline_color[4] = { 0, 0, 0, 1 };
                float inline_color[4] = { 0, 0, 0, 1 };
            } box;

            struct skeleton {
                bool enabled;
                int thickness = 1;
                float color[4] = { 0, 0, 0, 1 };
            } skeleton;
        } players;

        int font_size = 8;
        bool font_outline = true;
        bool font_fading = true;
    } visuals;

    /* aimbot */
    struct aimbot {
        bool enabled;
        float fov = 150;
        int hotkey = 0x2;
        int bone_target = 47;

        bool smooth;
        int smoothing = 2;

        bool prediction = true;
        bool attack_team ;
        bool compensate_recoil;
    } aimbot;

    /* features */
    struct features {
        bool fake_admin;
    } features;
};

class Utils
{
public:
    /**
     * getWindowSize -> returns the window size of a HWND
     *
     * input: HWND - the window that you want to get the size of
     * output: std:pair<int, int> - pair representing x, y dimensions of the window
     */
    std::pair<float, float> getWindowSize(HWND window);

    std::string wstring_to_string(const std::wstring& wstring);

    /**
     *  Convert Vec3 world position to Vec2 screen position
     *
     * input: Vector3 In Game Coordinates, ViewMatrix, and Camera OBJ
     * output: Vector2 In Game Coordinates Translated to Screen Position
     */
    Vector2 W2S(Vector3 origin, Matrix* view_matrix, uint64_t camera);

    /**
     * Color Cycles The Watermark
     */
    float* color_cycle();

    float color_hue[3] = { 255, 0, 0 };
    int color_iteration = 0;

};


extern Settings* settings;

extern Utils* utils;

#endif // _UTILS_HPP