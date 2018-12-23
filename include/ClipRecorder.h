//
// Created by fred on 23/12/18.
//

#ifndef RASPBERRYRECORDER_CLIPRECORDER_H
#define RASPBERRYRECORDER_CLIPRECORDER_H


#include <opencv2/videoio.hpp>

class ClipRecorder
{
public:
    /*!
     * Initialise
     *
     * @throws std::exception on failure
     * @param directory Filepath to store clips in. Will be created if it doesn't exist.
     */
    ClipRecorder(std::string directory, int32_t res_x, int32_t res_y, int32_t fps);

    /*!
     * Start recording. Does nothing if already recording.
     */
    void start();

    /*!
     * Stops recording. Does nothing if not recording.
     */
    void stop();

    /*!
     * Saves a frame to the video
     *
     * @param frame Frame to record
     */
    void save_frame(cv::Mat &frame);

private:
    std::string directory;
    std::string filepath;
    int32_t res_x;
    int32_t res_y;
    int32_t fps;
    std::chrono::steady_clock::time_point start_time;
    std::unique_ptr<cv::VideoWriter> video;
};

#endif //RASPBERRYRECORDER_CLIPRECORDER_H
