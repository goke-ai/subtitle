#pragma once

#include <tuple>
#include <vector>
#include <string>

namespace gsubtitle
{
    std::tuple<int, int, int, int> getTimes(std::string token, std::string delimiter);

    std::tuple<int, int, int, int> getTimesFromVTT(std::string token);

    std::tuple<int, int, int, int> getTimesFromSRT(std::string token);

    double toSeconds(int hour, int min, int sec, int msec);

    std::tuple<int, int, int, int> secondToTimes(double secs);

    std::tuple<int, int, int, int> addSecond(int hour, int min, int sec, int msec, double secs);

    std::tuple<int, int, int, int> addSecond(double originalSecs, double secs);

    std::string toSRT(int hour, int min, int sec, int msec);

    std::string toVTT(int hour, int min, int sec, int msec);

    std::string addSecondVTT(std::string tokenVTT, double secs);

    std::string addSecondSRT(std::string tokenSRT, double secs);

    std::string addSecondVTT2SRT(std::string tokenVTT, double secs);

    std::string VTT2SRT(std::string tokenVTT);

    std::string SRT2VTT(std::string tokenSRT);

    std::vector<std::string> readVTT(std::string fname);

    std::vector<std::string> readSRT(std::string fname);

    bool write(std::vector<std::string> lines, std::string fname);

    bool writeSRT(std::vector<std::string> lines, std::string fname = "example.srt");

    bool writeVTT(std::vector<std::string> lines, std::string fname = "example.vtt");

    template <class T>
    void convert(std::vector<std::string>& lines, T func)
    {
        // convert
        for (auto &&l : lines)
        {
            auto timeline = l.find("-->") != std::string::npos;
            if (timeline)
            {
                std::cout << l;

                // check if srt time interval line
                auto tokens = gcore::split(l, "-->");

                //
                l = func(tokens);

                std::cout << " \t" << l << std::endl;
            }
        }
    }

    void convertVTT2SRT(std::string fname);

    void convertAllVTT2SRT(std::string folder);

    void convertSRT2VTT(std::string fname);

    void convertAllSRT2VTT(std::string folder_path);

    void modifyVTTTime(std::string fname, double addSecs);

    void modifySRTTime(std::string fname, double addSecs);

} // namespace gsubtitle
