#pragma once

#include <iosfwd>
#include <string>

namespace layout_summary {

bool write(std::ostream &out);
bool write_to_file(const std::string &path);

} // namespace layout_summary
