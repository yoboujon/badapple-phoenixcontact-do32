#include <iostream>
#include <chrono>

#include <phoenixcontact.h>
#include <opencv2/opencv.hpp>

#define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()

constexpr uint16_t DO_REGISTER = 9000;

constexpr uint8_t FPS = 30;
constexpr uint8_t ROW_SIZE = 4;
constexpr uint8_t COLUMN_SIZE = 8;

bool isPixelWhite(cv::Vec3b color);

auto main() -> int
{
    // Phoenix Contact setup
    PhoenixDevice do32(DO_REGISTER, 2, "192.168.0.21");
    if(!do32.isConnected())
    {
        std::cerr << "Could not connect to Modbus device." << std::endl;
        return -1;
    }
    std::cout << "Connected!" << std::endl;

    // OpenCV setup
    cv::VideoCapture video("resources/badapple_8x4.mp4");
    cv::Mat frame(4, 8, CV_8UC3, cv::Scalar(255, 255, 255));
    // Resize the window
    cv::namedWindow("Frame", cv::WINDOW_NORMAL); // Create a resizable window
    cv::resizeWindow("Frame", 800, 400);         // Set the desired size (width, height)

    // total frame count elapsed
    size_t frameCount(0);
    // For debug only : show the time each frame took
    auto debugFrameTimeClock = std::chrono::high_resolution_clock::now();
    // FPS management
    long long frameTime(0);
    const auto frameCompare = static_cast<long long>((1.0 / static_cast<double>(FPS)) * 1000.0);

    while (video.read(frame))
    {
        // frame for the fps
        frameTime = NOW;
        // Debug only
        debugFrameTimeClock = std::chrono::high_resolution_clock::now();

        //cv::imshow("Frame", frame);
        for (uint8_t column(0); column < COLUMN_SIZE; column++)
        {
            for (uint8_t row(0); row < ROW_SIZE; row++)
            {
                // pixel -> BGR
                cv::Vec3b pixel = frame.at<cv::Vec3b>(row, column);
                do32[static_cast<size_t>(column + row*8)] = isPixelWhite(pixel);
            }
        }
        cv::imshow("Frame", frame);
        do32.writeData();
        std::cout << "\rframe [" << std::setfill(' ') << std::setw(10) << frameCount << "]: took " << std::setfill(' ') << std::setw(8) << (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - debugFrameTimeClock).count()) / 1000.0 << "ms  \t";
        frameCount++;

        // "To cap the frame at a certain amount"
        cv::waitKey(1);
        while ((NOW - frameTime) < frameCompare) {}
        std::cout << "FPS: " << std::setfill(' ') << std::setw(8) << static_cast<double>(1.0 / (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - debugFrameTimeClock).count())) * 1000000.0;
    }
    std::cout << std::endl;

    // Ending video
    cv::destroyAllWindows();
    video.release();
    // Disconnecting modbus
    do32.disconnect();
    return 0;
}

bool isPixelWhite(cv::Vec3b color)
{
    const uint16_t colorSum = color[0] + color[1] + color[2];
    // 384 = 256*3/2
    return colorSum >= 300;
}