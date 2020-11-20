#include <string>
#include <utility>
#include <algorithm>
namespace wrSamples
{
// if input string is an integer, then it represents a webcam index
// Returns an "optional". If the bool is false, then the int is not a good representation of the
// input string and should be ignored
inline std::pair<int, bool> GetWebcamIndexMaybe(const std::string& s)
{
    bool const isInt = !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return std::isdigit(c);
    });
    if (isInt)
    {
        try
        {
            return std::pair<int, bool>(std::stoi(s), true);
        }
        catch (std::exception&)
        {
        }
    }
    return std::pair<int, bool>(-1, false);
}
}  // namespace wrSamples