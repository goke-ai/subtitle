

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

#include "gstring.h"
#include "utilities.h"
#include "subtitle.h"

namespace fs = std::filesystem;

namespace gsubtitle
{
    std::tuple<int, int, int, int> getTimes(std::string token, std::string delimiter)
    {
        int h = 0, m = 0, s = 0, ms = 0;
        auto times = gcore::split(token, ":");
        if (times.size() > 2)
        {
            auto secs = gcore::split(times[2], delimiter);
            times[2] = secs[0];

            if (secs.size() > 1)
            {
                times.push_back(secs[1]);
            }
            else
            {
                times.push_back("0");
            }
        }

        h = stoi(times[0]);
        m = stoi(times[1]);
        s = stoi(times[2]);
        ms = stoi(times[3]);

        return {h, m, s, ms};
    }

    std::tuple<int, int, int, int> getTimesFromVTT(std::string token)
    {
        return getTimes(token, ".");
    }

    std::tuple<int, int, int, int> getTimesFromSRT(std::string token)
    {
        return getTimes(token, ",");
    }

    double toSeconds(int hour, int min, int sec, int msec)
    {
        // convert to seconds
        return hour * 3600 + min * 60 + sec + msec / 1000.0;
    }

    std::tuple<int, int, int, int> secondToTimes(double secs)
    {
        int hour = secs / 3600;
        int min = (secs - hour * 3600) / 60;
        int sec = secs - hour * 3600 - min * 60;
        int msec = (secs - hour * 3600 - min * 60 - sec) * 1000;

        return {hour, min, sec, msec};
    }

    std::tuple<int, int, int, int> addSecond(int hour, int min, int sec, int msec, double secs)
    {
        // convert to seconds
        auto vsec = toSeconds(hour, min, sec, msec);

        vsec += secs;

        // convert seconds to hour, min, sec
        std::tie(hour, min, sec, msec) = secondToTimes(vsec);

        return {hour, min, sec, msec};
    }

    std::tuple<int, int, int, int> addSecond(double originalSecs, double secs)
    {
        // convert to seconds
        originalSecs += secs;

        // convert seconds to hour, min, sec
        int hour, min, sec, msec;
        std::tie(hour, min, sec, msec) = secondToTimes(originalSecs);

        return {hour, min, sec, msec};
    }

    std::string toSRT(int hour, int min, int sec, int msec)
    {
        std::stringstream os;
        os << std::setfill('0') << std::setw(2) << hour << ":" << std::setw(2) << min << ":" << std::setw(2) << sec << "," << std::setw(3) << msec;

        return os.str();
    }

    std::string toVTT(int hour, int min, int sec, int msec)
    {
        std::stringstream os;
        os << std::setfill('0') << std::setw(2) << hour << ":" << std::setw(2) << min << ":" << std::setw(2) << sec << "." << std::setw(3) << msec;

        return os.str();
    }

    std::string addSecondVTT(std::string tokenVTT, double secs)
    {
        int h, m, s, ms;
        std::tie(h, m, s, ms) = getTimesFromVTT(tokenVTT);

        // convert seconds to hour, min, sec
        int hour, min, sec, msec;
        std::tie(hour, min, sec, msec) = addSecond(h, m, s, ms, secs);

        return toVTT(hour, min, sec, msec);
    }

    std::string addSecondSRT(std::string tokenSRT, double secs)
    {
        int h, m, s, ms;
        std::tie(h, m, s, ms) = getTimesFromSRT(tokenSRT);

        // convert seconds to hour, min, sec
        int hour, min, sec, msec;
        std::tie(hour, min, sec, msec) = addSecond(h, m, s, ms, secs);

        return toSRT(hour, min, sec, msec);
    }

    std::string addSecondVTT2SRT(std::string tokenVTT, double secs)
    {
        int h, m, s, ms;
        std::tie(h, m, s, ms) = getTimesFromVTT(tokenVTT);

        // convert seconds to hour, min, sec
        int hour, min, sec, msec;
        std::tie(hour, min, sec, msec) = addSecond(h, m, s, ms, secs);

        return toSRT(hour, min, sec, msec);
    }

    std::string VTT2SRT(std::string tokenVTT)
    {
        int h, m, s, ms;
        std::tie(h, m, s, ms) = getTimesFromVTT(tokenVTT);

        return toSRT(h, m, s, ms);
    }

    std::string SRT2VTT(std::string tokenSRT)
    {
        int h, m, s, ms;
        std::tie(h, m, s, ms) = getTimesFromSRT(tokenSRT);

        return toVTT(h, m, s, ms);
    }

    std::vector<std::string> readVTT(std::string fname)
    {
        if (!gcore::is_extension(fname, ".vtt"))
            return {};

        std::vector<std::string> lines;
        std::string line;
        std::ifstream vttfile(fname);
        if (vttfile.is_open())
        {
            while (getline(vttfile, line))
            {
                // remove WEBVTT header
                // remove extra line spaces
                if (!(line == "WEBVTT" || line.empty() || isspace(line[0])))
                {

                    // remove return
                    auto pos = line.find_last_not_of(" \r\n\t\f\v");
                    if (pos != std::string::npos)
                        line.erase(pos + 1);

                    lines.push_back(line);
                }
            }
            vttfile.close();
        }
        else
        {
            std::cout << "Unable to open file";
        }

        return lines;
    }

    std::vector<std::string> readSRT(std::string fname)
    {
        if (!gcore::is_extension(fname, ".srt"))
            return {};

        std::vector<std::string> lines;
        std::string line;
        std::ifstream srtfile(fname);
        if (srtfile.is_open())
        {
            while (getline(srtfile, line))
            {
                // remove WEBVTT header
                // remove extra line spaces
                if (!(line.empty() || isspace(line[0])))
                {

                    // remove return
                    auto pos = line.find_last_not_of(" \r\n\t\f\v");
                    if (pos != std::string::npos)
                        line.erase(pos + 1);

                    lines.push_back(line);
                }
            }
            srtfile.close();
        }
        else
        {
            std::cout << "Unable to open file";
        }

        return lines;
    }

    bool write(std::vector<std::string> lines, std::string fname)
    {
        int count = 0;
        std::ofstream srtfile(fname);
        if (srtfile.is_open())
        {
            for (auto &&l : lines)
            {
                // check if all std::string are digit
                if (gcore::isnumber(l))
                {
                    if (count == 0)
                    {
                        ++count;
                        srtfile << l << std::endl;
                        continue;
                    }
                    srtfile << std::endl;
                    ++count;
                }

                srtfile << l << std::endl;
            }
            srtfile.close();
        }
        else
        {
            std::cout << "Unable to open file";
            return false;
        }

        return true;
    }

    bool writeSRT(std::vector<std::string> lines, std::string fname)
    {
        if (!gcore::is_extension(fname, ".srt"))
            return false;

        return write(lines, fname);
    }

    bool writeVTT(std::vector<std::string> lines, std::string fname)
    {
        if (!gcore::is_extension(fname, ".vtt"))
            return false;

        return write(lines, fname);
    }

    void convertVTT2SRT(std::string fname)
    {
        // read vtt file
        auto lines = readVTT(fname);

        // convert
        convert(lines,
                [](auto tokens)
                {
                    return VTT2SRT(tokens[0]) + " --> " + VTT2SRT(tokens[1]);
                });

        // write new srt file
        auto new_fname = gcore::change_extension(fname, ".srt");
        writeSRT(lines, new_fname);
    }

    void convertAllVTT2SRT(std::string folder_path)
    {
        const std::filesystem::path fpath{folder_path};

        for (const auto &file : fs::directory_iterator(fpath))
        {
            if (file.is_regular_file())
            {
                auto fname = file.path().string();

                // read vtt file
                auto lines = readVTT(fname);

                // convert
                convert(lines,
                        [](auto tokens)
                        {
                            return VTT2SRT(tokens[0]) + " --> " + VTT2SRT(tokens[1]);
                        });

                // write new srt file
                auto new_fname = gcore::change_extension(fname, ".srt");
                writeSRT(lines, new_fname);
            }
        }
    }

    void convertSRT2VTT(std::string fname)
    {
        // read vtt file
        auto lines = readSRT(fname);

        // convert
        convert(lines,
                [](auto tokens)
                {
                    return SRT2VTT(tokens[0]) + " --> " + SRT2VTT(tokens[1]);
                });

        // write new srt file
        auto new_fname = gcore::change_extension(fname, ".vtt");
        writeVTT(lines, new_fname);
    }

    void convertAllSRT2VTT(std::string folder_path)
    {
        const std::filesystem::path fpath{folder_path};

        for (const auto &file : fs::directory_iterator(fpath))
        {
            if (file.is_regular_file())
            {
                auto fname = file.path().string();

                // read vtt file
                auto lines = readSRT(fname);

                // convert
                convert(lines,
                        [](auto tokens)
                        {
                            return SRT2VTT(tokens[0]) + " --> " + SRT2VTT(tokens[1]);
                        });

                // write new srt file
                auto new_fname = gcore::change_extension(fname, ".vtt");
                writeVTT(lines, new_fname);
            }
        }
    }

    void modifyVTTTime(std::string fname, double addSecs)
    {
        // read vtt file
        auto lines = readVTT(fname);

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
                if (addSecs > 0.0)
                {
                    l = addSecondVTT(tokens[0], addSecs) + " --> " + addSecondVTT(tokens[1], addSecs);
                }

                std::cout << "\t add " << addSecs << " \t" << l << std::endl;
            }
        }

        // write new srt file
        auto new_fname = gcore::change_extension(fname, "2.vtt");
        writeVTT(lines, new_fname);
    }

    void modifySRTTime(std::string fname, double addSecs)
    {
        // read vtt file
        auto lines = readSRT(fname);

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
                if (addSecs > 0.0)
                {
                    l = addSecondSRT(tokens[0], addSecs) + " --> " + addSecondSRT(tokens[1], addSecs);
                }

                std::cout << "\t add " << addSecs << " \t" << l << std::endl;
            }
        }

        // write new srt file
        auto new_fname = gcore::change_extension(fname, "2.srt");
        writeVTT(lines, new_fname);
    }

} // namespace gsubtitle
