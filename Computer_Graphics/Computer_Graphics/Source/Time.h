#ifndef TIME_H
#define TIME_H

#include <pch.h>

struct Time
{
    double time_last;
    double current_time;
    int frame_count;
    double frame_time;
    double deltaTime;

    Time(double tl = 0, double ct = 0, int fc = 0, double ft = 0, double dt = 0)
        :time_last(tl), current_time(ct), frame_count(fc), frame_time(ft), deltaTime(dt)
    {}

    void ProcessTime(GLFWwindow* window, double currTime) 
    {
        deltaTime = currTime - time_last;
        time_last = currTime;

        // frame rate
        ++frame_count;
        frame_time += deltaTime;

        if (frame_time >= 0.5)
        {
            double fps = frame_count / frame_time;
            frame_count = 0;
            frame_time = 0;
            std::stringstream ss;
            ss << APP_NAME << " [fps=" << int(fps) << "]";
            glfwSetWindowTitle(window, ss.str().c_str());
        }
    }
};

#endif // TIME_H
