//
// Created by fred on 23/12/18.
//

#include <sys/stat.h>
#include "ClipRecorder.h"

ClipRecorder::ClipRecorder(std::string directory_, int32_t res_x_, int32_t res_y_, int32_t fps_)
: directory(std::move(directory_)),
  res_x(res_x_),
  res_y(res_y_),
  fps(fps_)
{
    if(directory.empty())
        throw std::logic_error("Directory can't be empty");
    if(directory.back() != '/')
        directory.push_back('/');
    struct stat st{};
    if(stat(directory.c_str(), &st) == -1)
    {
        if(errno == ENOENT)
        {
            if(mkdir(directory.c_str(), S_IRUSR | S_IWUSR) != 0)
                throw std::runtime_error("Failed to create '" + directory + "':" + std::string(strerror(errno)));
            return;
        }
        throw std::runtime_error("Couldn't stat '" + directory + "': " + std::string(strerror(errno)));
    }
}

void ClipRecorder::start()
{
    //Can't start if started
    if(video)
        return;

    //Load up
    auto fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    auto time = std::time(nullptr);
    filepath = directory + std::string(std::asctime(std::localtime(&time))) + ".mp4";
    video = std::make_unique<cv::VideoWriter>(filepath, fourcc, fps, cv::Size{res_x, res_y});
    start_time = std::chrono::steady_clock::now();
}

void ClipRecorder::stop()
{
    //Can't stop if stopped
    if(!video)
        return;

    //Good to stop
    video.reset();

    //Delete if too short
    if(start_time + std::chrono::seconds(2) > std::chrono::steady_clock::now())
    {
        std::remove(filepath.c_str());
    }
}

void ClipRecorder::save_frame(cv::Mat &frame)
{
    if(!video)
        return;
    video->write(frame);
}
