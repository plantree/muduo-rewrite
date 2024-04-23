#include "base/Utils.h"

#include "absl/log/log.h"

#include <fstream>
#include <sstream>

namespace muduo_rewrite {
std::string readFile(fs::path path) {
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if (!ifs.is_open()) {
        LOG(WARNING) << "Failed to open file " << path;
        return std::string();
    }

    std::ostringstream os;

    std::string line;
    while (std::getline(ifs, line)) {
        os << line << '\n';   
    }

    ifs.close();

    return os.str();
}

}   // namespace muduo_rewrite