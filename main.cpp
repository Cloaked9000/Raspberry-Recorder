#include <iostream>
#include <atomic>
#include <stdio.h>
#include <memory>
#include <thread>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>

#include <ClipRecorder.h>
#include <Shell.h>

#define MAC_SCAN_INTERVAL std::chrono::seconds(5)
std::atomic<bool> running = true;
std::atomic<bool> is_home = true;

int main(int argc, char *argv[])
{
    //Get config
    std::string recording_filepath, scan_subnet, scan_mac;
    int32_t recording_res_x, recording_res_y, recording_fps;
    if(argc != 7)
    {
        std::cout << "Expected arguments: <recording_filepath> <scan_subnet> <scan_mac> <res_x> <res_y> <fps>\n";
        std::cout << "E.g, to scan the 192.168.1.0/24 subnet for the MAC address 00:00:0A:A0:00:00, storing recordings in 'recordings' in 800x600 resolution at 20fps: \n";
        std::cout << argv[0] << " 192.168.1.0/24 00:00:0A:A0:00:00 recordings 800 600 20\n";
        return EXIT_FAILURE;
    }
    recording_filepath = argv[1];
    scan_subnet = argv[2];
    scan_mac = argv[3];
    recording_res_x = std::stoi(argv[4]);
    recording_res_y = std::stoi(argv[5]);
    recording_fps = std::stoi(argv[6]);

    //Start scanning using nmap repeatedly
    std::thread t1([&]() {
        auto list = Shell::execute("nmap -sn " + scan_subnet);
        is_home = list.find(scan_mac) != std::string::npos;
        std::this_thread::sleep_for(MAC_SCAN_INTERVAL);
    });


    ClipRecorder recorder(recording_filepath, recording_res_x, recording_res_y, recording_fps);
    auto kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, {3,3});
    auto fgbg = cv::createBackgroundSubtractorMOG2();
    cv::Mat frame;

    while(running)
    {
        if(!is_home)
        {
            cv::Mat mask;
            cv::VideoCapture stream;
            stream.open(0);
            stream.set(cv::CAP_PROP_FRAME_WIDTH, recording_res_x);
            stream.set(cv::CAP_PROP_FRAME_HEIGHT, recording_res_y);

            //Read a frame from the camera
            while(stream.read(frame) && !is_home)
            {
                //Use background subtraction to look for changes
                fgbg->apply(frame, mask);
                cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

                //Count contours in the output
                std::vector<std::vector<cv::Point>> contours;
                std::vector<cv::Vec4i> hierarchy;
                cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

                //If we have any changes, start recording. If not, stop.
                if(!contours.empty())
                {
                    recorder.start();
                }
                else
                {
                    recorder.stop();
                }

                //Save frame, if recording
                recorder.save_frame(frame);
                imshow("main", frame);
                if('x' == cv::waitKey(1))
                {
                    running = false;
                }
            }
            continue;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    running = false;
    t1.join();
    return 0;
}
