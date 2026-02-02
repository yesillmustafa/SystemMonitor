#include "FormatUtils.h"
#include <sstream>
#include <iomanip>

namespace FormatUtils
{
    std::string FormatPercent(double value, int precision)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value;
        return oss.str();
    }
}